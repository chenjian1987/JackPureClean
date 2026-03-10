
#ifndef _GUID_H_
#define _GUID_H_

#include "JackCDevService.h"

#ifdef __linux__
#include <cstring>
#endif

#define GUID_SIZE 36
#define GUID_SIZE_ALLOC GUID_SIZE + 1

typedef struct
{
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} Guid;

inline int operator==(const Guid &left, const Guid &right)
{
    return !memcmp(&left, &right, sizeof(Guid));
}

namespace JackC
{
    struct JACKC_COMMON_EXPORT GuidHasher
    {
        std::size_t operator()(const Guid &key) const
        {
            return (key.Data1 ^ ((key.Data2 << 0x10) | key.Data3)) ^ ((key.Data4[2] << 0x18) | key.Data4[7]);
        }
    };

    /// \brief Guid相等仿函数
    struct JACKC_COMMON_EXPORT GuidEqual
    {
        bool operator()(const Guid &lhs, const Guid &rhs) const
        {
            return !memcmp(&lhs, &rhs, sizeof(Guid));
        }
    };

    /// \brief Guid比较仿函数
    struct JACKC_COMMON_EXPORT GuidComparer
    {
        bool operator()(const Guid &lhs, const Guid &rhs) const
        {
            return memcmp(&lhs, &rhs, sizeof(Guid))<0;
        }
    };

    typedef std::unordered_set<Guid, GuidHasher, GuidEqual> GuidUnorderedSet;

    class JACKC_COMMON_EXPORT JackCGuid
    {
    public:
        JackCGuid();

        /// <summary>
        /// build a GUID from an unicode string with the
        /// following format: "00000000-0000-0000-0000-000000000000"
        /// Length : 36 char
        /// </summary>
        /// <param name="aGuid"></param>
        JackCGuid(const std::wstring& guid);

        /// <summary>
        /// build a GUID 
        /// </summary>
        JackCGuid(const uint32_t data1,
                 const uint16_t data2,
                 const uint16_t data3,
                 const uint8_t  data41,
                 const uint8_t  data42,
                 const uint8_t  data43,
                 const uint8_t  data44,
                 const uint8_t  data45,
                 const uint8_t  data46,
                 const uint8_t  data47,
                 const uint8_t  data48);

        JackCGuid(uint32_t data1, uint16_t data2, uint16_t data3, uint8_t data4[8]); 

        /// <summary>
        /// Copy构造
        /// </summary>
        /// <param name="aGuid"></param>
        JackCGuid(const JackCGuid &guid);

        /// <summary>
        /// Guid
        /// </summary>
        /// <param name="aGuid"></param>
        JackCGuid(const Guid &guid);

        /// <summary>
        /// translate the GUID into unicode string
        /// format:"00000000-0000-0000-0000-000000000000"
        /// </summary>
        /// <param name="aStrGuid"></param>
        std::wstring ToWString() const;
        static std::wstring ToWString(const Guid &guid);

        bool IsSame(const JackCGuid &guid) const;

        bool operator==(const JackCGuid &guid) const
        {
            return IsSame(guid);
        }

        bool operator<(const JackCGuid &guid) const;

        bool operator>(const JackCGuid &guid) const;

        bool operator!=(const JackCGuid &guid) const
        {
            return !IsSame(guid);
        }

        void Assign(const JackCGuid &guid);

        void operator=(const JackCGuid &guid)
        {
            Assign(guid);
        }

        operator Guid();

        Guid ToGuid() const;

        static bool IsGuidFormat(const std::wstring &guid);

        static JackCGuid GetInvalid();

        bool IsValid() const;

        static bool IsValid(const JackCGuid &guid);

        /// <summary>
        /// Returns True  when the two GUID are the same.
        /// </summary>
        /// <param name="JackCGuid1"></param>
        /// <param name="JackCGuid2"></param>
        /// <returns></returns>
        static bool Equal(const JackCGuid &JackCGuid1, const JackCGuid &JackCGuid2);

        static bool Less(const JackCGuid &JackCGuid1, const JackCGuid &JackCGuid2);

        static Guid Generate();

        //! Hash function for GUID.
        int Hash() const;

        //! Computes a hash code for the given GUID of the Standard_Integer type, in the range [1, theUpperBound]
        //! @param theGUID the GUID which hash code is to be computed
        //! @param theUpperBound the upper bound of the range a computing hash code must be within
        //! @return a computed hash code, in the range [1, theUpperBound]
        static int HashCode(const JackCGuid &theGUID, int theUpperBound);

    private:
        uint32_t m_data1;
        uint16_t m_data2;
        uint16_t m_data3;
        uint8_t  m_data4[8];

    };

}


#endif // _GUID_H_
