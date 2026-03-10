#pragma once

// 高效安全的运行时动态类型转换。
// 使用方法：
//// A.h
//namespace JackC
//{
//    class A
//    {
//        DECLARE_CAST_BASE();
//    };
//}
//
//// A.cpp
//IMPLEMENT_CAST_START(JackC::A)
//IMPLEMENT_CAST_END()
//
//// B.h
//namespace JackC
//{
//    class B
//    {
//        DECLARE_CAST_BASE();
//    };
//}
//
//// B.cpp
//IMPLEMENT_CAST_START(JackC::B)
//IMPLEMENT_CAST_END()
//
//// C.h
//namespace JackC
//{
//    class C : public A, public B
//    {
//        DECLARE_CAST_DERIVED();
//    };
//}
//
//// C.cpp
//IMPLEMENT_CAST_START(JackC::C)
//IMPLEMENT_CAST_BASE(JackC::A)
//IMPLEMENT_CAST_BASE(JackC::B)
//IMPLEMENT_CAST_END()
//
//// D.h
//namespace JackC
//{
//    class D : public A, public B
//    {
//        DECLARE_CAST_DERIVED();
//    };
//}
//
//// D.cpp
//IMPLEMENT_CAST_START(JackC::D)
//IMPLEMENT_CAST_BASE(JackC::A)
//IMPLEMENT_CAST_BASE(JackC::B)
//IMPLEMENT_CAST_END()
//
//namespace JackC
//{
//    void Test()
//    {
//        A* pA = new C;
//        B* pB = Cast<B>(pA); // 成功
//        C* pC = Cast<C>(pA); // 成功
//        D* pD = Cast<D>(pA); // 失败
//    }
//}


#define DECLARE_CLASS_TYPE() \
private: \
    static const char* ClassType_(); \
public: \
    static const char* ClassType()

#define DECLARE_CLASS_TYPE_EXPORT(EXPORT) \
private: \
    static const char* ClassType_(); \
public: \
    EXPORT static const char* ClassType()


// 在有继承体系的基类中使用。
#define DECLARE_CAST_BASE() \
    DECLARE_CLASS_TYPE(); \
    virtual void* QueryObject(const char* classType) const

// 在有继承体系的派生类中使用。
#define DECLARE_CAST_DERIVED() \
    DECLARE_CLASS_TYPE(); \
    void* QueryObject(const char* classType) const override

// 在有继承体系的基类中使用。如果该类没有导出，需要导出这些方法。
#define DECLARE_CAST_BASE_EXPORT(EXPORT) \
    DECLARE_CLASS_TYPE_EXPORT(EXPORT); \
    EXPORT virtual void* QueryObject(const char* classType) const

// 在有继承体系的派生类中使用。如果该类没有导出，需要导出这些方法。
#define DECLARE_CAST_DERIVED_EXPORT(EXPORT) \
    DECLARE_CLASS_TYPE_EXPORT(EXPORT); \
    EXPORT void* QueryObject(const char* classType) const override

namespace JackC
{
    template <typename To, typename From>
    To* Cast(const From* pFrom)
    {
        if (!pFrom)
            return nullptr;

        return static_cast<To*>(pFrom->QueryObject(To::ClassType()));
    }
}
