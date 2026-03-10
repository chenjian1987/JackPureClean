#pragma once

#include "IObject.h"

namespace JackC
{
    //事件接口基类
    //事件管理Handler，但不会负责管理Handler的生命周期
    class JACKC_COMMON_EXPORT IEvent : public IObject
    {
        DEFINE_CAST_DERIVED(IEvent, IObject)

    public:
        //得到事件名称
        virtual std::wstring GetName() const = 0;

        virtual ~IEvent();
    };
}


