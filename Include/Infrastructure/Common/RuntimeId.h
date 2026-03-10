#pragma once
#include "CommonHeader.h"
#include "JackCDevService.h"
#include <cstdint>


namespace JackC
{
    //用来表示软件运行期的唯一标识
    class JACKC_COMMON_EXPORT RuntimeId
    {
    public:
        DEFINE_VISUAL_TYPE_INT64(JackC::EnumIntBaseType::Dec)

    public:
        FORCE_INLINE explicit RuntimeId(Int32 id = -1) : m_id(id) {}

        FORCE_INLINE bool IsValid() const { return m_id >= 0; }
        FORCE_INLINE Int32 AsInt32() const { return m_id; }
        FORCE_INLINE bool operator == (const RuntimeId& another) const { return m_id == another.m_id; }
        FORCE_INLINE bool operator != (const RuntimeId& another) const { return m_id != another.m_id; }
        FORCE_INLINE bool operator < (const RuntimeId& another) const { return m_id < another.m_id; }
        FORCE_INLINE bool operator > (const RuntimeId& another) const { return m_id > another.m_id; }
        FORCE_INLINE RuntimeId& operator ++ () { ++m_id; return *this; }
        FORCE_INLINE RuntimeId operator ++ (int) { RuntimeId old(*this); m_id++; return old; }

    public:
        int GetId()const {return int(m_id);}
 
    public:
        static const RuntimeId InvalidID;
        void SetId(Int32 id ) {m_id = id;}

    private:
        Int32 m_id;
    };
}



