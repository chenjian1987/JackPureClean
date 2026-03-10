#pragma once
#include "JackCDevService.h"
#include "JackCGuid.h"

namespace JackC
{
    class JACKC_COMMON_EXPORT ClassId
    {
    public:
        ClassId() noexcept : m_guid(JackCGuid::GetInvalid()) {}

        explicit ClassId(const JackCGuid &guid) : m_guid(guid) {}

        // 接受格式为：%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X 的字符串，中间允许有空格。空串或者不符合格式的产生Invalid()的ClassId
        explicit ClassId(const std::wstring & wstr);

        bool operator == (const ClassId & another) const;

        bool operator != (const ClassId & another) const;

        bool operator > (const ClassId & another) const;

        bool operator < (const ClassId & another) const;

        bool IsValid() const;

        // 返回格式为：%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X 的字符串表示
        std::wstring AsWString() const;

        // 接受格式为：%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X 的字符串，中间允许有空格。空串或者不符合格式的产生Invalid()的ClassId
        void SetByWString(const std::wstring & wstr);

        const Guid GetGuid() const
        {
            return m_guid.ToGuid();
        }

        const JackCGuid &GetJackCGuid() const
        {
            return m_guid;
        }

        void SetGuid(const JackCGuid &guid)
        {
            m_guid = guid;
        }

    private:
        JackCGuid m_guid;
    };

    struct JACKC_COMMON_EXPORT ClassIdHasher
    {
        std::size_t operator()(const ClassId & key) const;
    };
}

