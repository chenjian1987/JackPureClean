#include "ZipUtils.h"

extern "C"{
#include "zip.h"
#include "unzip.h"
}

#include <vector>
#include <forward_list>
#include "StringUtils.h"
#include "FileSystem.h"
#include "FileUtils.h"
#include <iostream>
#include <fstream>
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;
using namespace std;

namespace
{
    constexpr const int ZIP_BUFFER_SIZE = 1024 * 1024 * 8;

    std::string ToLocalString(const std::wstring & path)
    {
#ifdef _WINDOWS
        return StringUtils::ToStringByCodeType(path, StringUtils::CT_ANSI);
#else
        return StringUtils::ToString(path);
#endif
    }

    // entryList pair<filePath, entryName>
    void GetEntryList(const std::wstring & dirPath, const std::wstring & prefix, std::forward_list<std::pair<std::wstring, std::wstring>> & entryList)
    {
        entryList.emplace_front(std::make_pair(dirPath, prefix));

        for (DirectoryIterator iter(dirPath), end; iter != end; ++iter)
        {
            const std::wstring & name = iter->GetName();
            if (name == L"." || name == L"..")
            {
                continue;
            }
            std::wstring fullName = iter->GetFullName();
            FileInfo fileInfo(fullName);
            if (fileInfo.IsDirectory())
            {
                GetEntryList(fullName, prefix + iter->GetName() + L'/', entryList);
            }
            else if (fileInfo.IsFile())
            {
                entryList.emplace_front(std::make_pair(fullName, prefix + iter->GetName()));
            }
        }
    }
}

ZipOutput::ZipOutput() : m_pZipFile(nullptr), m_level(Z_DEFAULT_COMPRESSION), m_buffer(ZIP_BUFFER_SIZE)
{
}

ZipOutput::~ZipOutput()
{
    CloseZipFile();
}

bool ZipOutput::OpenZipFile(const std::wstring &zipFilePath, EnZipOpenMode mode)
{
    if(m_pZipFile != nullptr)
    {
        CloseZipFile();
    }
    int iMode;
    switch(mode)
    {
    case ZOM_APPEND:
        iMode = APPEND_STATUS_ADDINZIP;
        break;

    default:
        iMode = APPEND_STATUS_CREATE;
        break;
    }
    m_pZipFile = zipOpen64(ToLocalString(zipFilePath).data(), iMode);
    return m_pZipFile != nullptr;
}

void ZipOutput::SetZipLevel(EnZipLevel zipLevel)
{
    switch(zipLevel)
    {
    case ZL_DEFAULT:
        m_level = Z_DEFAULT_COMPRESSION;
        break;

    case ZL_BEST_SPEED:
        m_level = Z_BEST_SPEED;
        break;

    case ZL_BEST_COMPRESSION:
        m_level = Z_BEST_COMPRESSION;
        break;

    default:
        break;
    }
}

ZipOutput::EnZipLevel ZipOutput::GetZipLevel()
{
    EnZipLevel ret = ZL_DEFAULT;
    switch(m_level)
    {
    case Z_DEFAULT_COMPRESSION:
        ret = ZL_DEFAULT;
        break;

    case Z_BEST_SPEED:
        ret = ZL_BEST_SPEED;
        break;

    case Z_BEST_COMPRESSION:
        ret = ZL_BEST_COMPRESSION;
        break;

    default:
        break;
    }
    return ret;
}

bool ZipOutput::PutFileEntry(const std::wstring &filePath, const std::wstring &entryName)
{
    FileOwnerPtr file = FileSystem::OpenFile(filePath, L"rb");
    if (file == nullptr)
    {
        return false;
    }
    bool ret = PutFileEntry(file.get(), entryName);
    return ret;
}

bool ZipOutput::PutDirEntry(const std::wstring &dirPath, const std::wstring &entryName)
{
    if(m_pZipFile == nullptr || !FileInfo(dirPath).IsDirectory())
    {
        return false;
    }

    // pair<filePath, entryName>
    std::forward_list<std::pair<std::wstring, std::wstring>> entryList;
    if (entryName.back() != L'/')
    {
        GetEntryList(dirPath, entryName + L'/', entryList);
    }
    else
    {
        GetEntryList(dirPath, entryName, entryList);
    }

    for (auto ite = entryList.begin(), endIte = entryList.end(); ite != endIte; ++ite)
    {
        if(ite->second.back() == L'/')
        {
            PutDirEntry(ite->second);
        }
        else
        {
            PutFileEntry(ite->first, ite->second);
        }
    }
    return true;
}

void ZipOutput::CloseZipFile()
{
    if(m_pZipFile != nullptr)
    {
        zipClose(m_pZipFile, nullptr);
        m_pZipFile = nullptr;
    }
}

bool ZipOutput::PutFileEntry(FILE* file, const std::wstring &entryName)
{
    if(file == nullptr || m_pZipFile == nullptr)
    {
        return false;
    }
    zipOpenNewFileInZip64(m_pZipFile, StringUtils::ToString(entryName).data(), nullptr, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, m_level, 1);
    while(true)
    {
        auto count = fread(m_buffer.data(), 1, ZIP_BUFFER_SIZE, file);
        if(count == 0)
        {
            break;
        }
        zipWriteInFileInZip(m_pZipFile, m_buffer.data(), (unsigned int)count);
    }
    zipCloseFileInZip(m_pZipFile);
    return true;
}

void ZipOutput::PutDirEntry(const std::wstring &entryName)
{
    zipOpenNewFileInZip64(m_pZipFile, StringUtils::ToString(entryName).data(), nullptr, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, m_level, 1);
    zipCloseFileInZip(m_pZipFile);
}

bool ZipOutput::PutContentEntry(const std::string& content, const std::wstring& entryName)
{
    if (m_pZipFile == nullptr || content.empty())
        return false;
    zipOpenNewFileInZip64(m_pZipFile, StringUtils::ToString(entryName).data(), nullptr, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, m_level, 1);
    int count = 0;
    for (auto iter=content.begin();iter!=content.end();iter++)
    {
        m_buffer[count] = *iter;
        count++;
        if (count == ZIP_BUFFER_SIZE)
        {
            zipWriteInFileInZip(m_pZipFile, m_buffer.data(), (unsigned int)count);
            count = 0;
        }
    }
    if (count > 0)
    {
        zipWriteInFileInZip(m_pZipFile, m_buffer.data(), (unsigned int)count);
    }
    //zipWriteInFileInZip(m_pZipFile, content.data(), content.length());
    zipCloseFileInZip(m_pZipFile);
    return true;
}

ZipInput::ZipInput() : m_pZipFile(nullptr), m_buffer(ZIP_BUFFER_SIZE)
{
}

ZipInput::~ZipInput()
{
    CloseZipFile();
}

bool ZipInput::OpenZipFile(const std::wstring &zipFilePath)
{
    if(m_pZipFile != nullptr)
    {
        CloseZipFile();
    }
    m_pZipFile = unzOpen64(ToLocalString(zipFilePath).data());
    return m_pZipFile != nullptr;
}

bool ZipInput::GoToFirstEntry()
{
    int ok = unzGoToFirstFile(m_pZipFile);
    return ok == UNZ_OK;
}

bool ZipInput::GoToNextEntry()
{
    int ok = unzGoToNextFile(m_pZipFile);
    return ok == UNZ_OK;
}

bool ZipInput::GoToEntry(const std::wstring &entryName, bool caseSensitive)
{
    int iCaseSensitivity = caseSensitive ? 1 : 2;
    int ok = unzLocateFile(m_pZipFile, StringUtils::ToString(entryName).data(), iCaseSensitivity);
    return ok == UNZ_OK;
}

std::wstring ZipInput::GetCurrentEntryName()
{
    const uLong bufSize = 260;
    char buf[bufSize];
    buf[0] = '\0';
    unzGetCurrentFileInfo64(m_pZipFile, nullptr, buf, bufSize, nullptr, 0, nullptr, 0);
    return StringUtils::ToWString(buf);
}

bool ZipInput::TakeCurrentEntry(const std::wstring & dirPath, const std::wstring & name)
{
    std::wstring fileOrFolderName = dirPath + L"/" + (name.empty() ? GetCurrentEntryName() : name);
    if(fileOrFolderName.back() == L'/')
    {
        // 是目录，创建目录直接返回
        FileUtils::CreateDirectories(fileOrFolderName);
        return true;
    }
    else
    {
        // 是文件，创建父目录
        if(fileOrFolderName.find(L'/') != std::wstring::npos)
        {
            std::wstring parentPath = fileOrFolderName.substr(0, fileOrFolderName.find_last_of(L'/'));
            if(!FileInfo(parentPath).Exists())
            {
                FileUtils::CreateDirectories(parentPath);
            }
        }
    }
    if(unzOpenCurrentFile(m_pZipFile) != UNZ_OK)
    {
        return false;
    }
    FileOwnerPtr file = FileSystem::OpenFile(fileOrFolderName, L"wb");
    if(file == nullptr)
    {
        unzCloseCurrentFile(m_pZipFile);
        return false;
    }
    int count;
    while(true)
    {
        count = unzReadCurrentFile(m_pZipFile, m_buffer.data(), ZIP_BUFFER_SIZE);
        if(count < 0)
        {
            unzCloseCurrentFile(m_pZipFile);
            return false;
        }
        if(count == 0)
        {
            break;
        }
        fwrite(m_buffer.data(), 1, count, file.get());
    }
    if(unzCloseCurrentFile(m_pZipFile) == UNZ_CRCERROR)
        return false;
    return true;
}

bool ZipInput::TakeAll(const std::wstring &dirPath)
{
    if(!GoToFirstEntry())
        return false;
    do
    {
        if(!TakeCurrentEntry(dirPath))
            return false;
    } while(GoToNextEntry());
    return true;
}

void ZipInput::CloseZipFile()
{
    if(m_pZipFile != nullptr)
    {
        unzClose(m_pZipFile);
        m_pZipFile = nullptr;
    }
}

bool ZipInput::TakeCurrentEntryToBuffer(std::string& buffer)
{
    if (unzOpenCurrentFile(m_pZipFile) != UNZ_OK)
    {
        return false;
    }
    int count;
    while (true)
    {
        count = unzReadCurrentFile(m_pZipFile, m_buffer.data(), ZIP_BUFFER_SIZE);
        if (count < 0)
        {
            unzCloseCurrentFile(m_pZipFile);
            return false;
        }
        if (count == 0)
        {
            break;
        }
        buffer += std::string(m_buffer.data(), count);
    }

    if (unzCloseCurrentFile(m_pZipFile) == UNZ_CRCERROR)
        return false;
    return true;
}

bool ZipInput::TakeCurrentEntryToBuffer(std::vector<char>& buffer)
{
    if (unzOpenCurrentFile(m_pZipFile) != UNZ_OK)
    {
        return false;
    }
    int count;
    while (true)
    {
        count = unzReadCurrentFile(m_pZipFile, m_buffer.data(), ZIP_BUFFER_SIZE);
        if (count < 0)
        {
            unzCloseCurrentFile(m_pZipFile);
            return false;
        }
        if (count == 0)
        {
            break;
        }
        buffer.insert(buffer.end(), m_buffer.begin(), m_buffer.end());
    }

    if (unzCloseCurrentFile(m_pZipFile) == UNZ_CRCERROR)
        return false;
    return true;
}

bool ZipInput::TakeAllToBuffer(std::vector<std::string>& buffer)
{
    if (!GoToFirstEntry())
        return false;
    do
    {
        std::string tempStr;
        if (!TakeCurrentEntryToBuffer(tempStr))
            return false;
        buffer.emplace_back(tempStr);
    } while (GoToNextEntry());
    return true;
}

extern "C" JACKC_COMMON_EXPORT 
void ZipDirectory(const wchar_t* sSrcDirName, 
    const wchar_t* sDestZipFileName, 
    const wchar_t* sEntryName)
{
    ZipOutput zip;
    if (!zip.OpenZipFile(sDestZipFileName))
    {
        return;
    }
    zip.PutDirEntry(sSrcDirName, sEntryName);
    zip.CloseZipFile();
}

extern "C" JACKC_COMMON_EXPORT 
void ZipFile(const wchar_t* sSrcZipFileName, 
    const wchar_t* sDestZipFileName, 
    const wchar_t* sEntryName)
{
    ZipOutput zip;
    if (!zip.OpenZipFile(sDestZipFileName))
    {
        return;
    }
    zip.PutFileEntry(sSrcZipFileName, sEntryName);
    zip.CloseZipFile();
}
