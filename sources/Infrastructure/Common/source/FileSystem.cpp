#include "FileSystem.h"
#include "FileUtils.h"
#include <fstream>
#include <sys/stat.h>
#include "DebugMessage.h"
#include "DebugCrash.h"
#include "FilePath.h"
#include "EnvironmentVariables.h"
#include "LogManager.h"

#ifdef __linux__
#include <unistd.h>
#include "StringUtils.h"
#include <dlfcn.h>
#else
#include <Windows.h>
#include <ShlObj.h>
#endif


using namespace JackC;

namespace
{
#ifdef _WINDOWS
    void ShowDebugWarnOnError(DWORD dwError, const std::wstring & errorMessage)
    {
        HLOCAL hlocal = NULL;
        DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
        BOOL fOk = FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
            FORMAT_MESSAGE_ALLOCATE_BUFFER,
            NULL, dwError, systemLocale,
            (PTSTR)&hlocal, 0, NULL
        );
        if (fOk && (hlocal != NULL))
        {
            std::wostringstream woss;
            woss << errorMessage << L"时发生错误：" << (PCTSTR)hlocal;
            DBG_WARN(woss.str(), L"jack.chen", L"2025-07-19");
            LocalFree(hlocal);
            return;
        }
    }
#endif
}

bool FileSystem::CreateDir(const std::wstring &path)
{
    if (FileInfo(path).Exists())
    {
        return false;
    }

#ifdef _WINDOWS
    if (0 != _wmkdir(path.c_str()))
    {
        ShowDebugWarnOnError(GetLastError(), L"创建目录" + path);
        return false;
    }
    return true;
#else
    std::string sPath = StringUtils::ToString(path);
    return (mkdir(sPath.c_str(), 0777) == 0);
#endif
}

std::wstring FileSystem::GetSdkDirPath()
{
#ifdef _WINDOWS
    HMODULE hModuleHandle = GetModuleHandle(L"JackCCommon.dll");
    wchar_t pPath[MAX_PATH];
    if (NULL != hModuleHandle)
    {
        GetModuleFileName(hModuleHandle, pPath, MAX_PATH);
    }
    else
    {
        DBG_CRASH(false, L"hModuleHandle为空，不应该运行到此，请检查文件名");
    }

    FilePath fp(pPath);
    return fp.GetDirectoryName();
#else
    return L"./";
#endif
}

std::wstring FileSystem::GetTempDirPath()
{
#ifdef _WINDOWS
    wchar_t tempPath[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    return tempPath;
#else
    return L"/tmp/";
#endif
}

std::wstring FileSystem::GetLocalDocumentDirPath(const std::wstring &appName)
{
#ifdef _WINDOWS
    wchar_t appDataPath[MAX_PATH];
    if (SHGetSpecialFolderPath(0, appDataPath, CSIDL_PROFILE, FALSE))
    {
        std::wstring appPath = L"\\";
        if (!appName.empty())
        {
            appPath = L"\\" + appName + L"\\";
        }
        wcscat_s(appDataPath, appPath.c_str());
        if (!FileInfo(appDataPath).IsDirectory())
        {
            FileSystem::CreateDir(appDataPath);
        }

        return appDataPath;
    }
    else
    {
        return GetTempDirPath();
    }
#else
    return L"./home/";
#endif
}

std::wstring FileSystem::GetLocalAppDataDirPath(const std::wstring& appName)
{
#ifdef _WINDOWS
    wchar_t appDataPath[MAX_PATH];
    if (SHGetSpecialFolderPath(0, appDataPath, CSIDL_LOCAL_APPDATA, FALSE))
    {
        std::wstring appPath = L"\\";
        if (!appName.empty())
        {
            appPath = L"\\" + appName + L"\\";
        }
        wcscat_s(appDataPath, appPath.c_str());
        if (!FileInfo(appDataPath).IsDirectory())
        {
            FileSystem::CreateDir(appDataPath);
        }

        return appDataPath;
    }
    else
    {
        return GetTempDirPath();
    }
#else
    return L"./home/";
#endif
}

std::wstring FileSystem::GetAppDataDirPath()
{
#ifdef _WINDOWS
    wchar_t appDataPath[MAX_PATH];
    memset(appDataPath, 0, sizeof(wchar_t) * MAX_PATH);

    ::SHGetSpecialFolderPath(NULL, appDataPath, CSIDL_APPDATA, FALSE);
    if (!FileInfo(appDataPath).IsDirectory())
    {
        FileSystem::CreateDir(appDataPath);
    }
    return appDataPath;
#else
    return L"./home/";
#endif
}

std::wstring FileSystem::GetWorkingDirPath()
{
#ifdef _WINDOWS
    wchar_t tempPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, tempPath);
    return tempPath;
#else
    // TODO: Implement
    return L"./";
#endif
}

void FileSystem::SetWorkingDirPath(const std::wstring& path)
{
#ifdef _WINDOWS
    SetCurrentDirectory(path.c_str());
#else
    // TODO: Implement
#endif
}

FileOwnerPtr FileSystem::OpenFile(const std::wstring &path, const std::wstring &mode)
{
#ifdef _WINDOWS
    FILE * fp = nullptr;
    _wfopen_s(&fp, path.c_str(), mode.c_str());
    return FileOwnerPtr(fp);
#else
    std::string sPath = StringUtils::ToString(path);
    std::string sMode = StringUtils::ToString(mode);
    FILE* fp = fopen(sPath.c_str(), sMode.c_str());
    return FileOwnerPtr(fp);
#endif
}

bool FileSystem::OpenStream(std::fstream &stream, const std::wstring &path, std::ios_base::openmode mode)
{
#ifdef _WINDOWS
    stream.open(path, mode);
#else
    stream.open(StringUtils::ToString(path), mode);
#endif
    return stream.good();
}

bool FileSystem::OpenStream(std::ifstream &stream, const std::wstring &path, std::ios_base::openmode mode)
{
#ifdef _WINDOWS
    stream.open(path, mode);
#else
    stream.open(StringUtils::ToString(path), mode);
#endif
    return stream.good();
}

bool FileSystem::OpenStream(std::ofstream &stream, const std::wstring &path, std::ios_base::openmode mode)
{
#ifdef _WINDOWS
    stream.open(path, mode);
#else
    stream.open(StringUtils::ToString(path), mode);
#endif
    return stream.good();
}

bool FileSystem::OpenStream(std::wofstream &stream, const std::wstring &path, std::ios_base::openmode mode)
{
#ifdef _WINDOWS
    stream.open(path, mode);
#else
    stream.open(StringUtils::ToString(path), mode);
#endif
    return stream.good();
}

FileSystem::FunctionHandle FileSystem::GetFunctionAddress(ModuleHandle hModule, const std::string& functionName)
{
#ifdef _WINDOWS
    return GetProcAddress((HINSTANCE)hModule, functionName.c_str());
#else
    return dlsym(hModule, functionName.c_str());
#endif
}

bool FileSystem::UnloadModule(ModuleHandle hModule)
{
#ifdef _WINDOWS
    return ::FreeLibrary((HINSTANCE)hModule);
#else
    int ret = dlclose(hModule);
    if (0 == ret)
    {
        return true;
    }
    return false;
#endif
}

FileSystem::ModuleHandle FileSystem::LoadModule(const std::wstring& moduldPath)
{
#ifdef _WINDOWS
    FileSystem::ModuleHandle hModule = ::LoadLibraryW(moduldPath.c_str());
    DWORD errorCode = GetLastError();
    if (hModule == nullptr && errorCode != 0)
    {
        FYI(L"Loading Module - Windows Last Error: " + StringUtils::ToWString(errorCode) + L", dll path = " + moduldPath);
    }

    if (hModule == nullptr)
    {
        std::wstring absPath(moduldPath);
        if (FilePath::IsRelativePath(absPath))
        {
            absPath = FilePath::Combine(GetExeDirPath(), moduldPath);
        }

        FilePath fp(absPath);
        std::wstring dirPath = EnvironmentVariables::AddPath(fp.GetDirectoryName());
        EnvironmentVariableSetter envSetter(L"Path", dirPath);
        hModule = ::LoadLibraryW(absPath.c_str());

        DWORD errorCode = GetLastError();
        if (hModule == nullptr && errorCode != 0)
        {
            FYI(L"Loading Module - Windows Last Error: " + StringUtils::ToWString(errorCode) + L", dirpath path = " + dirPath);
        }
    }
#else
    std::string path = StringUtils::ToString(moduldPath);
    FileSystem::ModuleHandle hModule = dlopen(path.c_str(), RTLD_NOW);
    if (hModule == nullptr)
    {
        std::wstring fullPath(moduldPath);
        if (FilePath::IsRelativePath(fullPath))
        {
            fullPath = FilePath::Combine(GetExeDirPath(), moduldPath);
            path = StringUtils::ToString(fullPath);
            hModule = dlopen(path.c_str(), RTLD_NOW);
            if (hModule == nullptr)
            {
                FYI(L"Loading Module - Linux Last Error: " + StringUtils::ToWString(dlerror()) + L", dll path = " + moduldPath);
            }
        }
    }
#endif

    return hModule;
}

bool FileSystem::CreateDirectories(const std::wstring &path)
{
    return FileUtils::CreateDirectories(path);
}

std::wstring FileSystem::GetTestDirPath()
{
    return FilePath::Combine(GetExeDirPath(), L"../../tests");
}

std::wstring FileSystem::GetCommonAppDataDirPath()
{
#ifdef _WINDOWS_
    wchar_t commonAppDataPath[MAX_PATH];
    memset(commonAppDataPath, 0, MAX_PATH);

    ::SHGetSpecialFolderPath(NULL, commonAppDataPath, CSIDL_COMMON_APPDATA, false);
    if (!FileInfo(commonAppDataPath).IsDirectory())
    {
        FileSystem::CreateDir(commonAppDataPath);
    }
    return commonAppDataPath;
#else
    return L"./home/";
#endif
}

bool FileSystem::RemoveFile(const std::wstring &path)
{
    //FYI(L"RemoveFile:" + path);
    // 文件不存在，直接返回true
    if (!FileInfo(path).Exists()) return true;

#ifdef _WINDOWS
    if (0 == _wremove(path.c_str()))
    {
        return true;
    }
    int lastError = GetLastError();
    return false;
#else
    return remove(StringUtils::ToString(path).c_str());
#endif
}

bool FileSystem::RemoveDir(const std::wstring &path)
{
#ifdef _WINDOWS
    //FYI(L"removedir:" + path);
    return (_wrmdir(path.c_str()) == 0);
#else
    std::string sPath = StringUtils::ToString(path);
    return (rmdir(sPath.c_str()) == 0);
#endif
}

bool FileSystem::Rename(const std::wstring &oldName, const std::wstring &newName)
{
#ifdef _WINDOWS
    //FYI(L"Rename - oldName：" + oldName);
    //FYI(L"Rename - newName：" + newName);
    if (0 == _wrename(oldName.c_str(), newName.c_str()))
    {
        return true;
    }
    //FYI(L"Error: Rename - oldName：" + oldName);
    //FYI(L"Error: Rename - newName：" + newName);
    int lastError = GetLastError();
    return false;
#else
    return (rename(StringUtils::ToString(oldName).c_str(), StringUtils::ToString(newName).c_str()) == 0);
#endif
}

std::wstring FileSystem::GetPurePath(const std::wstring &path)
{
    std::wstring retPath;
#ifdef _WINDOWS
    const wchar_t from = L'/';
    const wchar_t to = L'\\';
#else
    const wchar_t from = L'\\';
    const wchar_t to = L'/';
#endif
    wchar_t last = L'\0';
    for (int i = 0; i < (int)path.size(); ++i)
    {
        wchar_t ch = path[i];
        if (ch == from)
            ch = to;
        // 去掉连续的分隔符
        if (last == SeparatorChar() && ch == SeparatorChar())
            continue;
        retPath.push_back(ch);
        last = ch;
    }
    // 去掉末尾的分隔符
    if (last == SeparatorChar())
        retPath.pop_back();
    return retPath;
}


void FileSystem::GetSubFiles(const std::wstring& path, std::vector<std::wstring>& files, bool isincludedir, bool isincludefile)
{
#ifdef _WINDOWS
    //文件句柄  
    intptr_t   hFile = 0;
    //文件信息  
    struct _wfinddata_t fileinfo;
    std::wstring p;
    if ((hFile = _wfindfirst(p.assign(path).append(L"\\*").c_str(), &fileinfo)) != (intptr_t)-1)
    {
        do
        {
            //如果是目录,迭代之  
            //如果不是,加入列表  
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (isincludedir)
                    files.push_back(fileinfo.name);

            }
            else
            {
                if (isincludefile)
                    files.push_back(fileinfo.name);

            }
        } while (_wfindnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
#endif
}

JACKC_COMMON_EXPORT void FileSystem::GetAllFileFullPaths(const std::wstring& path, std::vector<std::wstring>& files)
{
    std::vector<std::wstring> allFiles;
    FileSystem::GetSubFiles(path, allFiles, true, true);
    for (const std::wstring& file : allFiles)
    {
        if (L'.' == file.at(0))
            continue;
        std::wstring fullPath = path + FileSystem::Separator() + file;

        FileInfo info(fullPath);
        if (info.IsDirectory())
        {

            GetAllFileFullPaths(fullPath, files);
        }

        else if (info.IsFile())
        {
            files.push_back(fullPath);
        }
    };
}

FileInfo::FileInfo(const std::wstring &driveOrPath) :m_exists(false), m_isFile(false), m_isDirectory(false), m_size(0)
{
#ifdef _WINDOWS
    m_exists = (_waccess(driveOrPath.c_str(), 0) == 0);
    if (m_exists)
    {
        // For normal path, when passed as 1st argument to _wstat(), need remove path separator at tail.
        // But for drive in format of L"C:", need append path separator.
        const std::wstring name = (2 == driveOrPath.length() && L':' == driveOrPath.at(1)) ?
            driveOrPath + FileSystem::Separator() :
            driveOrPath;
        struct __stat64  temp = { 0 };
        if (0 == _wstat64(name.c_str(), &temp))
        {
            m_isFile = ((S_IFREG & temp.st_mode) != 0);
            m_isDirectory = ((S_IFDIR & temp.st_mode) != 0);
            m_size = temp.st_size;
        }
    }
#else
    std::string sPath = StringUtils::ToString(driveOrPath);
    m_exists = (access(sPath.c_str(), F_OK) == 0);
    if (m_exists)
    {
        struct stat temp = { 0 };
        if (0 == stat(sPath.c_str(), &temp))
        {
            m_isFile = ((S_IFREG & temp.st_mode) != 0);
            m_isDirectory = ((S_IFDIR & temp.st_mode) != 0);
            m_size = temp.st_size;
        }
    }
#endif
}

DirectoryIterator::DirectoryIterator() :m_isOpen(false)
{
}

DirectoryIterator::DirectoryIterator(const std::wstring &path) : m_isOpen(false)
{
    m_currentEntry.m_parentPath = path;
#ifdef _WINDOWS
    std::wstring dirPath = path;
    m_hDir = _wfindfirst(dirPath.append(L"\\*").c_str(), &m_dirData);
    if (m_hDir != -1L)
    {
        m_isOpen = true;
        m_currentEntry.m_name = m_dirData.name;
        DirectoryEntry::EmDirectoryEntryType type = DirectoryEntry::DET_NotSupport;
        if ((m_dirData.attrib & _A_HIDDEN) == 0)
        {
            if (m_dirData.attrib & _A_SUBDIR) type = DirectoryEntry::DET_Directory;
            else type = DirectoryEntry::DET_NormalFile;
        }
        m_currentEntry.m_type = type;
    }
#else
    m_dirData = opendir(StringUtils::ToString(path).c_str());
    if (m_dirData == nullptr)
    {
        return;
    }
    struct dirent *ptr = readdir(m_dirData);
    if (ptr)
    {
        m_isOpen = true;
        m_currentEntry.m_name = StringUtils::ToWString(ptr->d_name);
        DirectoryEntry::EmDirectoryEntryType type = DirectoryEntry::DET_NotSupport;
        if (ptr->d_type == DT_DIR) type = DirectoryEntry::DET_Directory;
        else if (ptr->d_type == DT_REG) type = DirectoryEntry::DET_NormalFile;

        m_currentEntry.m_type = type;
    }
    else
    {
        closedir(m_dirData);
    }
#endif
}

DirectoryIterator::~DirectoryIterator()
{
    if (m_isOpen)
    {
#ifdef _WINDOWS
        _findclose(m_hDir);
#else
        closedir(m_dirData);
#endif
        m_isOpen = false;
    }
        }

const DirectoryEntry &DirectoryIterator::operator*() const
{
    return m_currentEntry;
}

const DirectoryEntry *DirectoryIterator::operator->() const
{
    return &m_currentEntry;
}

DirectoryIterator &DirectoryIterator::operator++()
{
    if (!m_isOpen)
        return *this;
#ifdef _WINDOWS
    if (_wfindnext(m_hDir, &m_dirData) == 0)
    {
        m_currentEntry.m_name = m_dirData.name;
        DirectoryEntry::EmDirectoryEntryType type = DirectoryEntry::DET_NotSupport;
        // TODO 暂时忽略隐藏文件
        if ((m_dirData.attrib & _A_HIDDEN) == 0)
        {
            if (m_dirData.attrib & _A_SUBDIR) type = DirectoryEntry::DET_Directory;
            else type = DirectoryEntry::DET_NormalFile;
        }
        m_currentEntry.m_type = type;
    }
    else
    {
        m_isOpen = false;
        _findclose(m_hDir);
    }
#else
    struct dirent *ptr = readdir(m_dirData);
    if (ptr)
    {
        m_currentEntry.m_name = StringUtils::ToWString(ptr->d_name);
        DirectoryEntry::EmDirectoryEntryType type = DirectoryEntry::DET_NotSupport;
        if (ptr->d_type == DT_DIR) type = DirectoryEntry::DET_Directory;
        else if (ptr->d_type == DT_REG) type = DirectoryEntry::DET_NormalFile;

        m_currentEntry.m_type = type;
    }
    else
    {
        m_isOpen = false;
        closedir(m_dirData);
    }
#endif
    return *this;
}

bool DirectoryIterator::operator!=(const DirectoryIterator &other)
{
    return !(*this == other);
}

bool DirectoryIterator::operator==(const DirectoryIterator &other)
{
    // 和自身相等；目录都未打开时相等；其它情况暂视为不相等
    if (!m_isOpen && !other.m_isOpen)
        return true;
    if (this == &other)
        return true;
    return false;
}
