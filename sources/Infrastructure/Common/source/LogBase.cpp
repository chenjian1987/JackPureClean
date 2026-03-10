#include "LogBase.h"
#include "DevServiceHeaders.h"
#include "IBehaviorConfig.h"
#include "FileUtils.h"
#include "FileSystem.h"
#include "FilePathManager.h"
#include <sstream>
#include <iomanip>
#include "JackCNew.h"
#include "OwnerPtr.h"
#include "LogType.h"
#include "IConfigRecord.h"

using namespace JackC;

namespace
{
    // EnLogMgrLogType枚举型和输出字符串的对应表
    std::wstring s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::NUM_OF_TYPES];

    void InitializeLMLTNames()
    {
        s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::INVALID]          = L"[UNKNOWN  ]";
        s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::DEBUG_CRASH]      = L"[DBG_CRASH]";
        s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::DEBUG_ERROR]      = L"[DBG_ERROR]";
        s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::DEBUG_WARN]       = L"[DBG_WARN ]";
        s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::DEBUG_INFO]       = L"[DBG_INFO ]";
        s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::DIAGNOSE]         = L"[DIAGNOSE ]";
        s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::FYI]              = L"[FYI      ]";
        s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::JOURNAL]          = L"[JRN      ]";
        s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::JOURNAL_WARN]     = L"[JRN_WARN ]";
        s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::JOURNAL_ERROR]    = L"[JRN_ERROR]";
        s_logMgrLogTypeNames[(uint32_t)LogTypeOfLogManager::CONSISTENCY]      = L"[CST      ]";

        // 检测EnLogMgrLogType是否都已填上对应名称
        for(int loop = 0; loop < (uint32_t)LogTypeOfLogManager::NUM_OF_TYPES; ++loop)
        {
            if(s_logMgrLogTypeNames[loop].empty())
            {
                DBG_WARN(L"s_logMgrLogTypeNames中有遗漏的项，请补全。", L"jack.chen", L"2025-07-19");
                break;
            }
        }
    }
    ONE_TIME_INITIALIZER(InitializeLMLTNames);

    bool OpenLogFileStream(std::fstream &fileStream, 
        const std::wstring& folderPath, 
        const std::wstring& preferredFileName, 
        const std::wstring& fileExt)
    {
        // 如果不存在, 创建文件夹
        if (!FileInfo(folderPath).Exists())
        {
            FileUtils::CreateDirectories(folderPath);
        }

        const std::wstring pathSplitStr = FileSystem::Separator();

        std::wstring preferredFilePath = folderPath + pathSplitStr + preferredFileName + fileExt;
        if (FileInfo(preferredFilePath).Exists())
        {
            preferredFilePath = LogBase::FindAvailablePath(folderPath, preferredFileName, fileExt);
            std::wstring fileNameWithExt = (*StringUtils::SplitString(preferredFilePath, pathSplitStr, true).rbegin());
        }

        // TODO：目前已经能够将Log放入命令行参数指定的文件夹，而文件名字是和文件夹保持一致，这样在多次跑的时候
        // 对于已经存在的文件，行为方式未定。新名字，覆盖，追加？
         return FileSystem::OpenStream(fileStream, preferredFilePath, std::ios_base::out);
    }
}

LogBase::LogBase(const std::wstring& appConfigKey)
    : m_appConfigKey(appConfigKey) 
    , m_intentSize(0)
    , m_isReady(false)
{
}

LogBase::~LogBase()
{

}

void LogBase::Initialize(const std::wstring& folderPath, const std::wstring& preferredFileName, const std::wstring& fileExt)
{
    m_folderPath = folderPath;
    m_fileExt = fileExt;

    const std::wstring pathSplitStr = FileSystem::Separator();

    std::wstring preferredFilePath = m_folderPath + pathSplitStr + preferredFileName + m_fileExt;
    if (!FileInfo(preferredFilePath).Exists())
    {
        m_fileName = preferredFileName;
    }
    else
    {
        preferredFilePath = FindAvailablePath(folderPath, preferredFileName, m_fileExt);
        std::wstring fileNameWithExt = (*StringUtils::SplitString(preferredFilePath, pathSplitStr, true).rbegin());
        m_fileName = fileNameWithExt.substr(0, fileNameWithExt.find_last_of(L"."));
    }

    m_isReady = OpenLogFileStream(m_fileStream, folderPath, preferredFileName, fileExt);
    DBG_WARN_AND_RETURN_VOID_UNLESS(m_isReady, 
        L"打开文件流失败， folderPath = " + folderPath + L", preferredFileName = " + preferredFileName + L", fileExt = " + fileExt, 
        L"jack.chen", 
        L"2025-12-10");
}

void LogBase::AddExtraOutputPath(const std::wstring& folderPath, const std::wstring& preferredFileName, const std::wstring& fileExt)
{
    OwnerPtr<std::fstream> opFileStream = NEW_AS_OWNER_PTR(std::fstream);

    bool res = OpenLogFileStream(*opFileStream, folderPath, preferredFileName, fileExt);
    DBG_WARN_AND_RETURN_VOID_UNLESS(res, 
        L"打开文件流失败， folderPath = " + folderPath + L", preferredFileName = " + preferredFileName + L", fileExt = " + fileExt,
        L"jack.chen",
        L"2025-12-10");

    m_extraOutputPath.emplace(std::make_tuple(folderPath, preferredFileName, fileExt), TransferOwnership(opFileStream));
}

void LogBase::DeleteExtraOutputPath(const std::wstring& folderPath, const std::wstring& preferredFileName, const std::wstring& fileExt)
{
    auto iter = m_extraOutputPath.find(std::make_tuple(folderPath, preferredFileName, fileExt));
    if (iter != m_extraOutputPath.end())
    {
        m_extraOutputPath.erase(iter);
    }
}

int LogBase::GetIntentSize() const
{
    return m_intentSize;
}

void LogBase::SetIntentSize(int intentSize)
{
    m_intentSize = intentSize;
}

bool LogBase::IsReady() const
{
    return m_isReady;
}

bool LogBase::IsEnabled() const
{
    const IConfigRecord* pLogEnableCfg = IBehaviorConfig::GetConfigRecord(m_appConfigKey);
    if(nullptr == pLogEnableCfg)
    {
        return true;
    }
    return (pLogEnableCfg->GetValueAsInt() == 1);
}

void LogBase::LogLine(
    bool withTimestamp, 
    bool commentTimestamp, 
    bool withLogType,
    bool commentLogType,
    LogTypeOfLogManager logType, 
    const std::wstring& text )
{
    // 缩进
    std::wostringstream woss;
    for(int loop = 0; loop < m_intentSize; ++loop)
    {
        woss << L" ";
    }
    // 时间戳
    if (withTimestamp)
    {
        std::wstring timeStamp = MakeTimeStamp();
        if (commentTimestamp)
        {
            std::wstring commentedTimestamp = Comment(timeStamp, CommentType::SlashAsterisk);
            woss << commentedTimestamp.c_str() << L" ";
        }
        else
        {
            woss << timeStamp.c_str() << L" ";
        }
    }
    // 类型
    if (withLogType)
    {
        std::wstring logTypeName = s_logMgrLogTypeNames[(uint32_t)logType];
        if (commentLogType)
        {
            std::wstring commentedLogTypeName = Comment(logTypeName, CommentType::DoubleSlash);
            woss << commentedLogTypeName.c_str() << L" ";
        }
        else
        {
            woss << logTypeName.c_str() << L" ";
        }
    }
    // 文本
    woss << text.c_str();

    // 换行
    woss << L"\n";
    
    m_fileStream << StringUtils::ToString(woss.str());

    m_fileStream.flush();

    for (auto &item : m_extraOutputPath)
    {
        *item.second << StringUtils::ToString(woss.str());
        item.second->flush();
    }
}

void LogBase::LogLine(
    const std::wstring& file,
    bool withTimestamp,
    bool commentTimestamp,
    bool withLogType,
    bool commentLogType,
    LogTypeOfLogManager logType,
    const std::wstring& text
    
)
{
    const std::wstring pathSplitStr = FileSystem::Separator();

    std::wstring preferredFilePath = m_folderPath + pathSplitStr + file + m_fileExt;
    if (!FileInfo(preferredFilePath).Exists())
    {
       // m_fileName = file;
    }
    else
    {
        preferredFilePath = FindAvailablePath(m_folderPath, file, m_fileExt);
        //std::wstring fileNameWithExt = (*StringUtils::SplitString(preferredFilePath, pathSplitStr, true).rbegin());
        //m_fileName = fileNameWithExt.substr(0, fileNameWithExt.find_last_of(L"."));
    }
    static std::fstream fileStream;
    if(!fileStream.is_open())
        FileSystem::OpenStream(fileStream, preferredFilePath, std::ios_base::out);
    // 缩进
    std::wostringstream woss;
    for (int loop = 0; loop < m_intentSize; ++loop)
    {
        woss << L" ";
    }
    // 时间戳
    if (withTimestamp)
    {
        std::wstring timeStamp = MakeTimeStamp();
        if (commentTimestamp)
        {
            std::wstring commentedTimestamp = Comment(timeStamp, CommentType::SlashAsterisk);
            woss << commentedTimestamp.c_str() << L" ";
        }
        else
        {
            woss << timeStamp.c_str() << L" ";
        }
    }
    // 类型
    if (withLogType)
    {
        std::wstring logTypeName = s_logMgrLogTypeNames[(uint32_t)logType];
        if (commentLogType)
        {
            std::wstring commentedLogTypeName = Comment(logTypeName, CommentType::DoubleSlash);
            woss << commentedLogTypeName.c_str() << L" ";
        }
        else
        {
            woss << logTypeName.c_str() << L" ";
        }
    }
    // 文本
    woss << text.c_str();

    // 换行
    woss << L"\n";

    m_fileStream << StringUtils::ToString(woss.str());

    m_fileStream.flush();

    for (auto &item : m_extraOutputPath)
    {
        *item.second << StringUtils::ToString(woss.str());
        item.second->flush();
    }
}

const std::wstring& LogBase::GetFolderPath() const
{
    return m_folderPath;
}

const std::wstring& LogBase::GetFileName() const
{
    return m_fileName;
}

const std::wstring& LogBase::GetFileExt() const
{
    return m_fileExt;
}

std::wstring LogBase::FindAvailablePath(const std::wstring& folder, const std::wstring& prefix, const std::wstring& ext)
{
    const std::wstring& prefixWithFolder = folder + FileSystem::Separator() + prefix;

    std::wstring fullPath;
    for(int loop = 1; loop < 9999; ++loop)
    {
        std::wstringstream buffer;
        buffer << prefixWithFolder << std::setfill(L'0') << std::setw(4) << loop << ext;
        fullPath = buffer.str();
        if(!FileInfo(fullPath).Exists())
        {
            return fullPath;
        }
    }

    return L"";
}

std::wstring LogBase::MakeTimeStamp()
{
    std::wostringstream ossTimeStamp;
#ifdef _WINDOWS
    SYSTEMTIME sysTime; 
    GetLocalTime(&sysTime); 

    ossTimeStamp << L"[" << std::setw(4) << sysTime.wYear << L"-" << std::setw(2) << sysTime.wMonth << L"-"
        << std::setw(2) << sysTime.wDay << L" " << std::setw(2) << sysTime.wHour << L":"
        << std::setw(2) << sysTime.wMinute << L":" << std::setw(2) << sysTime.wSecond << L"("
        << std::setw(3) << sysTime.wMilliseconds << L")]";
#else
    // TODO:
#endif
    return ossTimeStamp.str();
}

std::wstring LogBase::Comment(const std::wstring& text, CommentType commentType)
{
    if (commentType == CommentType::SlashAsterisk)
    {
        return std::wstring(L"/*") + text + std::wstring(L"*/");
    }
    else if (commentType == CommentType::DoubleSlash)
    {
        return std::wstring(L"//") + text;
    }

    return text;
}

std::wstring LogBase::GetAppLogsFolderPath()
{
    return FilePathManager::Get()->GetLogsFolderPath().GetFullPath();
}
