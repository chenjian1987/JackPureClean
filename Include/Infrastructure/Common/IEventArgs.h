#pragma once

#include "IObject.h"
#include "IEvent.h"

namespace JackC
{
    //事件参数接口基类
    class JACKC_COMMON_EXPORT IEventArgs : public IObject
    {
        DEFINE_CAST_DERIVED(IEventArgs, IObject)

    public:
        //得到事件指针
        virtual const IEvent* GetEvent() const = 0;
        virtual ~IEventArgs();
    };
}


