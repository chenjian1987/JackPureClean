#pragma once

namespace JackC
{
    class OneTimeInitializer
    {
    public:
        template<class FuncInitializer>
        OneTimeInitializer(FuncInitializer func)
        {
            func();
        }

        virtual ~OneTimeInitializer(void)
        {
        }
    };
}

// 一次性的初始化操作可以使用这个宏处理
// 原理是构造一个静态对象，利用其构造函数进行指定的初始化操作
#define ONE_TIME_INITIALIZER(initializeRoutine) \
    JackC::OneTimeInitializer theInitializer_##initializeRoutine(initializeRoutine);

// 一次性的初始化操作可以使用这个宏处理(扩展)
//    第一个参数：静态对象名字，用于区分同一个文件中的不同的OneTimeInitializer静态对象
//    第二个参数：定义初始化操作，支持普通函数、函数子、std::function、Lamda函数
#define ONE_TIME_INITIALIZER_EXT(InitializerName, Func) \
    JackC::OneTimeInitializer theInitializer_##InitializerName(Func);
