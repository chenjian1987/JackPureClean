#pragma once

#include "JackCDevService.h"
#include <cstdio>
#include <string>
#include <vector>

namespace JackC
{
    // 用以创建zip文件
    class JACKC_COMMON_EXPORT ZipOutput
    {
    public:
        enum EnZipLevel
        {
            ZL_DEFAULT,
            ZL_BEST_SPEED,
            ZL_BEST_COMPRESSION
        };

        enum EnZipOpenMode
        {
            ZOM_CREATE,
            ZOM_APPEND
        };

        ZipOutput();
        virtual ~ZipOutput();

        // 打开zip文件用以写入
        // mode = ZOM_CREATE 文件存在则覆盖原来的内容，不存在则创建
        // mode = ZOM_APPEND 文件存在则追加内容，不存在则创建失败
        bool OpenZipFile(const std::wstring &zipFilePath, EnZipOpenMode mode = ZOM_CREATE);

        void SetZipLevel(EnZipLevel zipLevel);
        EnZipLevel GetZipLevel();

        // 将文件添加到zip文件中，entryName是在zip中的文件名(如：dir/log.txt)
        bool PutFileEntry(const std::wstring &filePath, const std::wstring& entryName);
        bool PutFileEntry(FILE* file, const std::wstring &entryName);
        
        // 将目录添加到zip文件中，entryName是在zip中的目录名
        bool PutDirEntry(const std::wstring &dirPath, const std::wstring &entryName);

        // 将内容添加到zip文件中，entryName是在zip中的文件名(如：dir/log.txt)
        bool PutContentEntry(const std::string& content, const std::wstring& entryName);

        void CloseZipFile();

    private:
        //创建空entry
        void PutDirEntry(const std::wstring &entryName);

    private:
        void* m_pZipFile;
        int m_level;
        std::vector<char> m_buffer;
    };

    // 用以解压zip文件
    class JACKC_COMMON_EXPORT ZipInput
    {
    public:
        ZipInput();
        virtual ~ZipInput();

        bool OpenZipFile(const std::wstring &zipFilePath);

        bool GoToFirstEntry();
        bool GoToNextEntry();
        bool GoToEntry(const std::wstring &entryName, bool caseSensitive = true);

        std::wstring GetCurrentEntryName();

        // name是取出Entry的文件名/目录名，name为空则名字同zip文件中的名字
        bool TakeCurrentEntry(const std::wstring &dirPath, const std::wstring & name = std::wstring());
        bool TakeAll(const std::wstring &dirPath);

        bool TakeCurrentEntryToBuffer(std::string& buffer);
        bool TakeCurrentEntryToBuffer(std::vector<char>& buffer);
        bool TakeAllToBuffer(std::vector<std::string>& buffer);
        
        void CloseZipFile();

    private:
        void* m_pZipFile;
        std::vector<char> m_buffer;
    };
}
