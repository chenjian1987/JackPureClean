#include "JackCGuid.h"
#include "DebugMessage.h"
#include <cwctype>
#include <regex>

#include <sstream>
#include <random>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cctype>

namespace
{
    /// <summary>
    /// 匹配字符'-'位置
    /// </summary>
    /// <param name="buffer"></param>
    /// <param name="aChar"></param>
    /// <returns></returns>
    int GuidMatchChar(const wchar_t *buffer, const wchar_t wchar)
    {
        wchar_t *tmpbuffer = (wchar_t *)buffer;
        int      result    = -1;

        while (*tmpbuffer != '\0' && *tmpbuffer != wchar)
        {
            tmpbuffer++;
            result++;
        }

        if (*tmpbuffer == '\0')
            return -1; // The searched symbol wasn't found

        if (result >= 0)
            result++;

        return result;
    }

    /// <summary>
    /// 获取指定字节转成无符号数
    /// </summary>
    /// <param name="guidBuff"></param>
    /// <param name="b32"></param>
    /// <param name="lenBuff">默认为0时根据'-'取长度，获取后12字节:lenBuff = 2</param>
    /// <returns></returns>
    wchar_t *GuidGetNByteValue(const wchar_t *guidBuff, uint32_t &b32, int lenBuff = 0) 
    {
        wchar_t strTmp[GUID_SIZE_ALLOC];

        if (lenBuff == 0)
        {
            lenBuff = GuidMatchChar(guidBuff, '-');
        }

        if (lenBuff >= 0)
        {
            wcsncpy(strTmp, guidBuff, lenBuff);
            strTmp[lenBuff] = '\0';
            b32 = (uint32_t)wcstoul(strTmp, (wchar_t **)NULL, 16);
        }
        if (lenBuff == 2)
        {
            return (wchar_t *)(guidBuff + lenBuff);
        }
        return (wchar_t *)(guidBuff + lenBuff + 1);
    }

    wchar_t *GuidGetNByteValue(const wchar_t *guidBuff, uint16_t &b16, int lenBuff = 0)
    {
        wchar_t strTmp[GUID_SIZE_ALLOC];

        if (lenBuff == 0)
        {
            lenBuff = GuidMatchChar(guidBuff, '-');
        }

        if (lenBuff >= 0)
        {
            wcsncpy(strTmp, guidBuff, lenBuff);
            strTmp[lenBuff] = '\0';
            b16             = (uint16_t)wcstoul(strTmp, (wchar_t **)NULL, 16);
        }
        if (lenBuff == 2)
        {
            return (wchar_t *)(guidBuff + lenBuff);
        }
        return (wchar_t *)(guidBuff + lenBuff + 1);
    }

    wchar_t *GuidGetNByteValue(const wchar_t *guidBuff, uint8_t &b8, int lenBuff = 0)
    {
        wchar_t strTmp[GUID_SIZE_ALLOC];

        if (lenBuff == 0)
        {
            lenBuff = GuidMatchChar(guidBuff, '-');
        }

        if (lenBuff >= 0)
        {
            wcsncpy(strTmp, guidBuff, lenBuff);
            strTmp[lenBuff] = '\0';
            b8             = (uint8_t)wcstoul(strTmp, (wchar_t **)NULL, 16);
        }
        if (lenBuff == 2)
        {
            return (wchar_t *)(guidBuff + lenBuff);
        }
        return (wchar_t *)(guidBuff + lenBuff + 1);
    }

    unsigned long Random_Char()
    {
        std::random_device              rd;
        std::mt19937                    gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        return static_cast<unsigned long>(dis(gen));
    }

    std::wstring GreateGuid()
    {
        int len = 16; // guid len 16

        std::wstringstream ss;
        for (auto i = 0; i < len; i++)
        {
            auto rc = Random_Char();

            std::wstringstream hexstream;
            hexstream << std::hex << int(rc);
            auto hex = hexstream.str();
            ss << (hex.length() < 2 ? L'0' + hex : hex);

            if (i == 3 || i == 5 || i == 7 || i == 9)
            {
                ss << '-';
            }
        }
        return ss.str();
    }
}

namespace JackC
{

    JackCGuid::JackCGuid() : m_data1(0), m_data2(2), m_data3(0), m_data4 {0}
    {
    }

    JackCGuid::JackCGuid(const std::wstring& guid) : m_data1(0), m_data2(2), m_data3(0), m_data4 {0}
    {
        wchar_t *tmpBuffer = (wchar_t *)guid.c_str();

        if (!IsGuidFormat(guid))
        {
            DBG_WARN(L"Invalid JackCGuid!", L"Xin Li", L"2025/9/19");
        }

        tmpBuffer = GuidGetNByteValue(tmpBuffer, m_data1);
        tmpBuffer = GuidGetNByteValue(tmpBuffer, m_data2);
        tmpBuffer = GuidGetNByteValue(tmpBuffer, m_data3);
        
        uint16_t m16b;
        tmpBuffer  = GuidGetNByteValue(tmpBuffer, m16b);
        m_data4[0] = (uint8_t)(m16b >> 8);
        m_data4[1] = (uint8_t)m16b;

        tmpBuffer = GuidGetNByteValue(tmpBuffer, m_data4[2], 2);
        tmpBuffer = GuidGetNByteValue(tmpBuffer, m_data4[3], 2);
        tmpBuffer = GuidGetNByteValue(tmpBuffer, m_data4[4], 2);
        tmpBuffer = GuidGetNByteValue(tmpBuffer, m_data4[5], 2);
        tmpBuffer = GuidGetNByteValue(tmpBuffer, m_data4[6], 2);
        tmpBuffer = GuidGetNByteValue(tmpBuffer, m_data4[7], 2);
    }

    JackCGuid::JackCGuid(const uint32_t data1,
                       const uint16_t data2,
                       const uint16_t data3,
                       const uint8_t  b1,
                       const uint8_t  b2,
                       const uint8_t  b3,
                       const uint8_t  b4,
                       const uint8_t  b5,
                       const uint8_t  b6,
                       const uint8_t  b7,
                       const uint8_t  b8)
    {
        m_data1 = data1;
        m_data2 = data2;
        m_data3 = data3;
        m_data4[0] = b1;
        m_data4[1] = b2;
        m_data4[2] = b3;
        m_data4[3] = b4;
        m_data4[4] = b5;
        m_data4[5] = b6;
        m_data4[6] = b7;
        m_data4[7] = b8;
    }

    JackCGuid::JackCGuid(uint32_t data1, uint16_t data2, uint16_t data3, uint8_t data4[8]) 
    {
        m_data1 = data1;
        m_data2 = data2;
        m_data3 = data3;
        memcpy(m_data4, data4, sizeof(m_data4));
    }

    JackCGuid::JackCGuid(const JackCGuid &guid)
    {
        m_data1 = guid.m_data1;
        m_data2 = guid.m_data2;
        m_data3 = guid.m_data3;
        memcpy(m_data4, guid.m_data4, sizeof(m_data4));
    }

    JackCGuid::JackCGuid(const Guid &guid) 
    {
        m_data1 = guid.Data1;
        m_data2 = guid.Data2;
        m_data3 = guid.Data3;
        memcpy(m_data4, guid.Data4, sizeof(m_data4));
    }

    void JackCGuid::Assign(const JackCGuid &guid)
    {
        m_data1 = guid.m_data1;
        m_data2 = guid.m_data2;
        m_data3 = guid.m_data3;
        memcpy(m_data4, guid.m_data4, sizeof(m_data4));
    }
    
    bool JackCGuid::IsSame(const JackCGuid &guid) const
    {
        return !memcmp(this, &guid, sizeof(JackCGuid));
    }

    bool JackCGuid::operator<(const JackCGuid &guid) const
    {
        if (memcmp(this, &guid, sizeof(JackCGuid)) < 0)
            return true;
        return false;
    }

    bool JackCGuid::operator>(const JackCGuid &guid) const
    {
        if (memcmp(this, &guid, sizeof(JackCGuid)) > 0)
            return true;
        return false;
    }

    bool JackCGuid::IsGuidFormat(const std::wstring &guid)
    {
        const wchar_t *pRegex = LR"(^[0-9a-fA-F]{8}-([0-9a-fA-F]{4}-){2}([0-9a-fA-F]{2}){2}-([0-9a-fA-F]{2}){6}$)";

        const std::wregex hexPattern(pRegex);
        return std::regex_match(guid, hexPattern);
    }

    std::wstring JackCGuid::ToWString() const
    {
        wchar_t tmpGuid[46] = {0};
        wmemset(tmpGuid, L'\0', 46);

        swprintf(tmpGuid,
                 46,
                 L"%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X",
                 m_data1,
                 m_data2,
                 m_data3,
                 m_data4[0],
                 m_data4[1],
                 m_data4[2],
                 m_data4[3],
                 m_data4[4],
                 m_data4[5],
                 m_data4[6],
                 m_data4[7]);
        return tmpGuid;
    }

    std::wstring JackCGuid::ToWString(const Guid &guid)
    {
        JackCGuid cGuid(guid);

        wchar_t tmpGuid[46] = {0};
        wmemset(tmpGuid, L'\0', 46);

        swprintf(tmpGuid,
                 46,
                 L"%.8x-%.4x-%.4x-%.2x%.2x-%.2x%.2x%.2x%.2x%.2x%.2x",
                 cGuid.m_data1,
                 cGuid.m_data2,
                 cGuid.m_data3,
                 cGuid.m_data4[0],
                 cGuid.m_data4[1],
                 cGuid.m_data4[2],
                 cGuid.m_data4[3],
                 cGuid.m_data4[4],
                 cGuid.m_data4[5],
                 cGuid.m_data4[6],
                 cGuid.m_data4[7]);

        return std::wstring(tmpGuid);
    }

    JackCGuid::operator Guid() 
    {
        return ToGuid();
    }

    Guid JackCGuid::ToGuid() const
    {
        Guid result;

        result.Data1    = m_data1;
        result.Data2    = m_data2;
        result.Data3    = m_data3;
        memcpy(result.Data4, m_data4, sizeof(m_data4));
        return result;
    }

    bool JackCGuid::Equal(const JackCGuid &JackCGuid1, const JackCGuid &JackCGuid2)
    {
        return (JackCGuid1 == JackCGuid2);
    }

    bool JackCGuid::Less(const JackCGuid &JackCGuid1, const JackCGuid &JackCGuid2)
    {
        return (JackCGuid1 < JackCGuid2);
    }

    Guid JackCGuid::Generate()
    {
        return JackCGuid(GreateGuid());
    }

    JackCGuid JackCGuid::GetInvalid()
    {
        static JackCGuid sGuid;
        return sGuid;
    }

    bool JackCGuid::IsValid() const
    {
        static JackCGuid sGuid;
        return sGuid != *this;
    }

    bool JackCGuid::IsValid(const JackCGuid &guid)
    {
        static JackCGuid sGuid;
        return guid != sGuid;
    }

    int JackCGuid::Hash() const
    {
        return (m_data1 ^ ((m_data2 << 0x10) | m_data3)) ^ ((m_data4[2] << 0x18) | m_data4[7]);
        
    }
} // namespace JackC
