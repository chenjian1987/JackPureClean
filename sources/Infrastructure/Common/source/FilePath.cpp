#include "FilePath.h"
#include "FileSystem.h"
#include "JackCAssert.h"
#include "StringUtils.h"
#include <cstdarg>
#include <iostream>

#ifdef _WINDOWS
#include <Shlwapi.h>
#endif

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
using namespace JackC;

FilePath::FilePath(const std::wstring& filePath)
{
#ifdef _WINDOWS
    // We have to remove prefix "\\?\".
    // Otherwise the file path cannot be splitted correctly.
    std::wstring tempFilePath = filePath;
    size_t nPos = filePath.find(L"\\\\?\\");
    if (0 == nPos)
        tempFilePath = tempFilePath.substr(4);
    wchar_t drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
    _wsplitpath(tempFilePath.c_str(), drive, dir, fname, ext);

    m_drive = drive;
    m_directory = dir;
    m_filename = fname;
    m_extension = ext;
#else
    std::wstring purePath = FileSystem::GetPurePath(filePath);
    m_drive = L"";
    auto posSeparator = purePath.find_last_of(FileSystem::SeparatorChar());
    auto posDot = purePath.find_last_of(L'.');
    if (posSeparator == std::wstring::npos)
    {
        m_directory = L"";
        if (posDot == std::wstring::npos)
        {
            m_filename = purePath;
            m_extension = L"";
        }
        else
        {
            m_filename = purePath.substr(0, posDot);
            m_extension = purePath.substr(posDot, purePath.size() - posDot);
        }
    }
    else
    {
        m_directory = purePath.substr(0, posSeparator + 1);
        if (posDot == std::wstring::npos)
        {
            m_filename = purePath.substr(posSeparator + 1, purePath.size() - posSeparator - 1);
            m_extension = L"";
        }
        else
        {
            m_filename = purePath.substr(posSeparator + 1, posDot - posSeparator - 1);
            m_extension = purePath.substr(posDot, purePath.size() - posDot);
        }
    }
#endif
}

FilePath::~FilePath()
{
}

std::wstring FilePath::GetDirectoryName() const
{
#ifdef _WINDOWS
    wchar_t pathBuffer[_MAX_PATH];
    _wmakepath_s(pathBuffer, _MAX_PATH, m_drive.c_str(), m_directory.c_str(), NULL, NULL);
    return pathBuffer;
#else
    return m_directory;
#endif
}

std::wstring FilePath::GetExtension() const
{
    return m_extension;
}

std::wstring FilePath::GetFileName() const
{
#ifdef _WINDOWS
    wchar_t filenameBuffer[_MAX_PATH];
    _wmakepath_s(filenameBuffer, _MAX_PATH, NULL, NULL, m_filename.c_str(), m_extension.c_str());
    return filenameBuffer;
#else
    return std::wstring(m_filename).append(m_extension);
#endif
}

std::wstring FilePath::GetFileNameWithoutExtension() const
{
    return m_filename;
}

std::wstring FilePath::GetFullPath() const
{
#ifdef _WINDOWS
    wchar_t pathBuffer[_MAX_PATH];
    _wmakepath_s(pathBuffer, _MAX_PATH, m_drive.c_str(), m_directory.c_str(), m_filename.c_str(), m_extension.c_str());
    return pathBuffer;
#else
    return std::wstring(m_directory).append(m_filename).append(m_extension);
#endif
}

std::wstring FilePath::GetPathRoot() const
{
    return m_drive;
}

bool FilePath::HasExtension() const
{
    return !m_extension.empty();
}

namespace
{
    std::wstring ConcatPathPart(const std::wstring & part) { return part;}

    template<typename T>
    T ConcatPathParts(const T & t) { return ConcatPathPart(t); }

    template<typename T, typename ... Ts>
    T ConcatPathParts(const T & t, const Ts & ... args) { return ConcatPathPart(t) + FileSystem::Separator() + ConcatPathParts(args ...); }
}

std::wstring JackC::FilePath::Combine(const std::wstring &part1, const std::wstring &part2)
{
    std::wstring combinedPath = ConcatPathParts(part1, part2);
    return FileSystem::GetPurePath(combinedPath);
}

std::wstring JackC::FilePath::Combine(const std::wstring &part1, const std::wstring &part2, const std::wstring &part3)
{
    std::wstring combinedPath = ConcatPathParts(part1, part2, part3);
    return FileSystem::GetPurePath(combinedPath);
}

bool JackC::FilePath::IsRelativePath(const std::wstring& path)
{
    if (path.empty())
        return true;
    if (path[0] == L'/')
        return false;
    if (path[0] == L':')
        return false;
    if (path.size() > 1 && path[1] == L':')
        return false;
    return true;
}

bool JackC::FilePath::IsOnSameDrive(const std::wstring& path1, const std::wstring& path2)
{
    if (path1.size()>=2 && path2.size()>=2 && path1[1] == L':' && path2[1] == L':' && path1[0] != path2[0])
        return false;
    return true;
}

std::wstring JackC::FilePath::GetAbsolutePath(const std::wstring& relativePath, const std::wstring &parentDir)
{
    if (!IsRelativePath(relativePath))
        return relativePath;
    std::wstring parentDirTemp;
    if (parentDir.empty())
        parentDirTemp = JackC::FileSystem::GetWorkingDirPath();
    else
        parentDirTemp = parentDir;
    return FilePath::Combine(parentDirTemp, relativePath);
}

std::wstring JackC::FilePath::GetRelativePath(const std::wstring& fullPath, const std::wstring &parentDir)
{
#ifdef _WINDOWS
    wchar_t szOut[_MAX_PATH] = L"";
    std::wstring fullPathPure = JackC::FileSystem::GetPurePath(fullPath);
    std::wstring parentDirPure = JackC::FileSystem::GetPurePath(parentDir);
    PathRelativePathTo(szOut, parentDirPure.data(), FILE_ATTRIBUTE_DIRECTORY, fullPathPure.data(), FILE_ATTRIBUTE_NORMAL);
    return szOut;
#else
    return fullPath;
#endif
}
