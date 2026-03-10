#pragma once

#include "JackCDevService.h"
#include "LogBase.h"
#include "OwnerPtr.h"
#include "LogType.h"
#include "DateTime.h"
#include <iostream>

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace JackC
{
    class DumpLog;
    class JournalDumpLog;
    class JournalLog;
    class JournalFullLog;
    class ConsistencyDetectLog;

    class JACKC_COMMON_EXPORT LogManager
    {
    private:
        LogManager(const std::wstring& subFolderName, const std::wstring& fileNamePrefix, bool numberAutomatically = false);
        bool ValidateLine(LogTypeOfLogManager lmlt, const LogBase * pLogBase, const std::wstring& text) const;

    public:
        virtual ~LogManager(void);

    // Singleton界面
    public:
        // 获取LogManager单例，如果不存在，则先创建。
        // subFolderName如果为空，文件夹格式为“journal+自动编号”。
        // fileNamePrefix如果为空，则和文件夹名字一样。
        // numberJSFile如果为true，则同名.js文件就会添上自动编号，从XXX0001.js到XXX9999.js
        static LogManager* CreateOrGetForWrite(
            const std::wstring& subFolderName = L"",
            const std::wstring& fileNamePrefix = L"",
            bool isNumberJSFile = false);

        // 获取LogManager单例。如果不存在则报错并返回空
        static LogManager* Get();
        static bool IsReady();

    public:
        void Restart();

        // 输出到脚本日志
        void WriteLine(LogTypeOfLogManager lmlt, const std::wstring& text, bool isCopyToCompactedLog = false);

        // 获取Log缩进大小
        int GetLogIndentSize() const;

        // 设置Log缩进大小
        void SetLogIndentSize(int intentSize);

        // 获取Logs文件夹
        std::wstring GetLogsFolder() const;
        std::wstring GetBaseLogsDirPath() const;

        // 相对Log目录的绝对路径/相对路径转换
        std::wstring GetAbsolutePathForLog(const std::wstring& path) const;
        std::wstring GetRelativePathForLog(const std::wstring& fullPath) const;
    public:
        // 获取日志
        const JournalFullLog* GetJournalFullLogInstance() const { return m_opJournalFullLogInstance.get(); }
        JournalFullLog* GetJournalFullLogInstance() { return m_opJournalFullLogInstance.get(); }
    private:
        static JackC::OwnerPtr<LogManager> ms_opInstance;
        std::wstring                      m_logsFolder;
        std::wstring                      m_subFolderName;
        std::wstring                      m_fileNameWithoutExt;

        JackC::OwnerPtr<JournalLog>        m_opJournalLogInstance;
        JackC::OwnerPtr<JournalFullLog>    m_opJournalFullLogInstance;
    };
}

#define FYI(text) \
    JackC::LogManager::CreateOrGetForWrite()->WriteLine(JackC::LogTypeOfLogManager::FYI, text, false);

#define FYI_IN_BOTH_FULL_AND_COMPACT(text) \
    JackC::LogManager::CreateOrGetForWrite()->WriteLine(JackC::LogTypeOfLogManager::FYI, text, true);

#define FYI_IF(condition, text) \
    if(condition) \
    { \
        FYI(text); \
    }

#define JOURNAL_DUMP(text) \
    JackC::LogManager::CreateOrGetForWrite()->JournalDumpLine(text);

#define DUMP_NO_INTENT(text) \
    JackC::LogManager::CreateOrGetForWrite()->DumpLineWithoutIntent(text);
//
#define DUMP_CONSISTENCY_RESULT(text) \
    JackC::LogManager::CreateOrGetForWrite()->ConsistencyDetectDumpLine(text);

#define BEGIN_LOG_TIME(_TAG)\
    uint64_t _begin_##_TAG = DateTime::GetSystemTimeMicros();

#define END_LOG_TIME(_TAG)\
    uint64_t _end_##_TAG = DateTime::GetSystemTimeMicros();\
    uint64_t _span_##_TAG = _end_##_TAG - _begin_##_TAG;\
    std::wstringstream  wstrStream_##_TAG;\
    wstrStream_##_TAG<<L"执行"<<#_TAG<<L"耗时:"<<std::to_wstring(_span_##_TAG)<<L"微秒";\
    FYI(wstrStream_##_TAG.str());

#pragma warning( pop )
