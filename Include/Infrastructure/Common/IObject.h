#pragma once

#include "JackCDevService.h"
#include "WeakReferenceable.h"
#include "JackCQuickCast.h"

namespace JackC
{
    //所有接口的基类
    //提供弱引用，派生类可以用于WeakPtr<T>
    class JACKC_COMMON_EXPORT IObject : public WeakReferenceable
    {
        DEFINE_CAST_BASE(JackC::IObject)
    public:
        virtual ~IObject(){}
    };
}
