#pragma once

#include "JackCDevService.h"
#include "StringUtils.h"
#include "OwnerPtr.h"
#include "FileUtils.h"
#include <iostream>

#ifndef _WINDOWS
    #include <dirent.h>
#else
    #include <io.h>
    #include <Windows.h>
#endif


namespace JackC
{
    struct FileDeleter
    {
        void operator()(FILE* p) const
        {
            fclose(p);
        };
    };

    using FileOwnerPtr = OwnerPtr<FILE, FileDeleter>;
    namespace FileSystem
    {
        // 文件路径分隔符
        FORCE_INLINE const std::wstring Separator()
        {
#ifdef _WINDOWS
            const std::wstring s_separator = L"\\";
#else
            const std::wstring s_separator = L"/";
#endif
            return s_separator;
        }

        FORCE_INLINE static const wchar_t SeparatorChar()
        {
#ifdef _WINDOWS
            return L'\\';
#else
            return L'/';
#endif
        }

        FORCE_INLINE std::wstring GetExeDirPath()
        {
            return FileUtils::GetExeDirPath();
        }

        FORCE_INLINE std::wstring GetCurrentProcessID()
        {
#ifdef _WINDOWS
            DWORD pid = GetCurrentProcessId();
            return StringUtils::ToWString(pid);
#else
            //暂不实现除windows系统外的其他系统
            return L"";
#endif
        }

        JACKC_COMMON_EXPORT std::wstring GetSdkDirPath();

        // 创建目录，不能一次递归创建多级
        // 如果名字是CreateDirectory，VS2010报错，暂定为CreateDir
        JACKC_COMMON_EXPORT bool CreateDir(const std::wstring &path);
        
        // 创建目录，可以一次递归创建多级。
        JACKC_COMMON_EXPORT bool CreateDirectories(const std::wstring &path);

        // 临时文件目录，以文件分隔符结尾
        JACKC_COMMON_EXPORT std::wstring GetTempDirPath();

        // 以文件分隔符结尾
        JACKC_COMMON_EXPORT std::wstring GetLocalAppDataDirPath(const std::wstring& appName = L"AppCube");
        JACKC_COMMON_EXPORT std::wstring GetLocalDocumentDirPath(const std::wstring &appName = L"Documents");

        // 以文件分隔符结尾
        JACKC_COMMON_EXPORT std::wstring GetAppDataDirPath();

        // 获取工作目录
        JACKC_COMMON_EXPORT std::wstring GetWorkingDirPath();

        // 获取测试目录
        JACKC_COMMON_EXPORT std::wstring GetTestDirPath();

        // 获取系统磁盘下的ProgramData文件目录
        JACKC_COMMON_EXPORT std::wstring GetCommonAppDataDirPath();

        // 设置工作目录
        JACKC_COMMON_EXPORT void SetWorkingDirPath(const std::wstring& path);

        JACKC_COMMON_EXPORT FileOwnerPtr OpenFile(const std::wstring &path, const std::wstring &mode);

        JACKC_COMMON_EXPORT bool OpenStream(std::fstream &stream, 
            const std::wstring &path, 
            std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

        JACKC_COMMON_EXPORT bool OpenStream(std::ifstream &stream, 
            const std::wstring &path, 
            std::ios_base::openmode mode = std::ios_base::in);

        JACKC_COMMON_EXPORT bool OpenStream(std::ofstream &stream, 
            const std::wstring &path, 
            std::ios_base::openmode mode = std::ios_base::out);

        JACKC_COMMON_EXPORT bool OpenStream(std::wofstream &stream, 
            const std::wstring &path, 
            std::ios_base::openmode mode = std::ios_base::out);

        JACKC_COMMON_EXPORT bool RemoveFile(const std::wstring &path);
        JACKC_COMMON_EXPORT bool RemoveDir(const std::wstring &path);
       
        // 重命名文件或目录
        JACKC_COMMON_EXPORT bool Rename(const std::wstring &oldName, const std::wstring &newName);

        // 统一路径分隔符，去除多余路径分隔符
        JACKC_COMMON_EXPORT std::wstring GetPurePath(const std::wstring &path);

        // moduldPath - 绝对路径或者相对于exe的相对路径。
        using ModuleHandle = void*;
        using FunctionHandle = void*;
        JACKC_COMMON_EXPORT ModuleHandle LoadModule(const std::wstring& moduldPath);
        JACKC_COMMON_EXPORT FunctionHandle GetFunctionAddress(ModuleHandle hModule, const std::string& functionName);
        JACKC_COMMON_EXPORT bool UnloadModule(ModuleHandle hModule);

        JACKC_COMMON_EXPORT void GetSubFiles(
            const std::wstring& path, 
            std::vector<std::wstring>& files,
            bool isincludedir,
            bool isincludefile);

        JACKC_COMMON_EXPORT void GetAllFileFullPaths(const std::wstring& path,
            std::vector<std::wstring>& files);

    }

    class JACKC_COMMON_EXPORT FileInfo
    {
    public:
        FileInfo(const std::wstring &driveOrPath);

    public:
        bool Exists() { return m_exists; }
        bool IsFile() { return m_isFile; }
        bool IsDirectory() { return m_isDirectory; }
        int64_t Size() { return m_size; }

    private:
        bool m_exists;
        bool m_isFile;
        bool m_isDirectory;
        int64_t m_size;
    };

#pragma warning( push )
#pragma warning( disable: 4251 )
    class JACKC_COMMON_EXPORT DirectoryEntry
    {
    public:
        enum EmDirectoryEntryType
        {
            DET_NotSupport,
            DET_NormalFile,
            DET_Directory,
        };

    public:
        // 返回文件或目录名
        const std::wstring & GetName() const { return m_name; }
        
        // 返回全路径名
        std::wstring GetFullName() const { return FileSystem::GetPurePath(m_parentPath + FileSystem::Separator() + m_name); }
        EmDirectoryEntryType GetType() const { return m_type; }
        bool IsFile() const { return (m_type == DET_NormalFile); }
        bool IsDirectory() const { return (m_type == DET_Directory); }

    private:
        std::wstring m_parentPath;
        std::wstring m_name;
        EmDirectoryEntryType m_type;

    private:
        friend class DirectoryIterator;
    };
#pragma warning( pop )

    // 遍历目录的迭代器
    class JACKC_COMMON_EXPORT DirectoryIterator
    {
    public:
        DirectoryIterator();
        explicit DirectoryIterator(const std::wstring &path);
        ~DirectoryIterator();

    public:
        const DirectoryEntry &operator*() const;
        const DirectoryEntry *operator->() const;

        DirectoryIterator &operator++();

        bool operator!=(const DirectoryIterator &other);
        bool operator==(const DirectoryIterator &other);

    private:
        bool m_isOpen;
        DirectoryEntry m_currentEntry;
#ifdef _WINDOWS
        intptr_t m_hDir;
        struct _wfinddata_t m_dirData;
#else
        DIR *m_dirData;
#endif
    };
}
