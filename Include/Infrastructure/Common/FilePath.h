#pragma once

#include "JackCDevService.h"

#pragma warning(push)
#pragma warning( disable : 4251 )

namespace JackC
{
    //文件路径类提供了对文件路径的封装以及操作
    class JACKC_COMMON_EXPORT FilePath
    {
    public:
        FilePath(const std::wstring& filePath);
        virtual ~FilePath(void);

    public:
        //返回路径字符串的目录信息，文件分割符结尾。
        std::wstring GetDirectoryName() const;

        //返回路径字符串的扩展名。
        std::wstring GetExtension() const;

        //返回路径字符串的文件名，包括扩展名。
        std::wstring GetFileName() const;

        //返回不具有扩展名的路径字符串的文件名。
        std::wstring GetFileNameWithoutExtension() const;

        //返回路径字符串的绝对路径。
        std::wstring GetFullPath() const;

        //获取路径的根目录信息。
        std::wstring GetPathRoot() const;

        //确定路径是否包括文件扩展名。
        bool HasExtension() const;

    public:
        //拼接两个文件路径字符串
        static std::wstring Combine(const std::wstring& part1, const std::wstring& part2);

        //拼接三个文件路径字符串
        static std::wstring Combine(const std::wstring& part1, const std::wstring& part2, const std::wstring& part3);

        //判断是否是相对路径
        static bool IsRelativePath(const std::wstring& path);

        //判断两个路径是否在同一个驱动上
        static bool IsOnSameDrive(const std::wstring& path1, const std::wstring& path2);

        //获取绝对路径
        static std::wstring GetAbsolutePath(const std::wstring& relativePath, const std::wstring& parentDir);

        //获取相对路径
        static std::wstring GetRelativePath(const std::wstring& fullPath, const std::wstring& parentDir);

    private:
        std::wstring m_drive;
        std::wstring m_directory;
        std::wstring m_filename;
        std::wstring m_extension;
    };
}

#pragma warning(pop)
