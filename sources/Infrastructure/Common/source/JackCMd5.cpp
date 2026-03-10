#include "JackCMD5.h"
#include "DebugMessage.h"
#include "StringUtils.h"
#include "Md5.h"
#include <string>
#include <fstream>
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace std;
using namespace JackC;

JackCMD5::JackCMD5() 
{
    m_oMd5 = NEW_AS_OWNER_PTR(MD5);
}

JackCMD5::JackCMD5(const void* input, size_t length) 
{
    m_oMd5 = NEW_AS_OWNER_PTR(MD5);
    Update(input, length);
}

JackCMD5::JackCMD5(const string& str) 
{
    m_oMd5 = NEW_AS_OWNER_PTR(MD5);
    Update(str);
}

JackCMD5::JackCMD5(ifstream& in) 
{
    m_oMd5 = NEW_AS_OWNER_PTR(MD5);
    Update(in);
}

JackCMD5::~JackCMD5()
{
    m_oMd5 = nullptr;
}

const Byte_MD5* JackCMD5::Digest()
{
    return m_oMd5->digest();
}

void JackCMD5::Reset()
{
    m_oMd5->reset();
}

void JackCMD5::Update(const void* input, size_t length) 
{
    m_oMd5->update((const Byte_MD5*)input, length);
}

void JackCMD5::Update(const string& str) 
{
    m_oMd5->update((const Byte_MD5*)str.c_str(), str.length());
}

void JackCMD5::Update(ifstream& in) 
{
    m_oMd5->update(in);
}

string JackCMD5::ToString() 
{
    return m_oMd5->bytesToHexString(Digest(), 16);
}

std::wstring JackCMD5::ComputeFileMD5Value(const std::wstring& filePath)
{
 #ifdef _WINDOWS
    ifstream in(filePath.c_str(), ios::binary);
    DBG_WARN_AND_RETURN_NULLPTR_UNLESS(in,L"in为空",L"jack.chen",L"2025-10-13")
#else
    std::string strFilePath = StringUtils::ToString(filePath);
    ifstream in(strFilePath.c_str(), ios::binary);
    DBG_WARN_AND_RETURN_NULLPTR_UNLESS(in,L"in为空",L"jack.chen",L"2025-10-13")
#endif
    JackCMD5 md5;
    std::streamsize length;
    const int oneSize = 1024;
    char buffer[oneSize];
    while (!in.eof()) 
    {
        in.read(buffer, oneSize);
        length = in.gcount();
        if (length > 0)
        {
            md5.Update(buffer, length);
        }
    }
    in.close();
    return StringUtils::ToWString(md5.ToString());
}
