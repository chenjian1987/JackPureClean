#pragma once

#include <map>
#include "DebugMessage.h"

namespace JackC
{
    //////////////////////////////////////////////////////////////////////////
    // 当 map 的 key 是一个 OwnerPtr，且希望对 OwnerPtr 所指对象进行排序时
    template<class T>
    class OwnerPtrValueLessComparer
    {
    public:
        bool operator()(const OwnerPtr<T>& lhs, const OwnerPtr<T>& rhs) const
        {
            DBG_WARN_AND_RETURN_FALSE_UNLESS(lhs && rhs, L"OwnerPtr<T> 为空指针", L"jack.chen", L"2025-07-18");
            return *lhs < *rhs;
        }
    };

    template<class k, class v>
    using OwnerPtrMap = std::map<OwnerPtr<k>, v, OwnerPtrValueLessComparer<k>>;
}
