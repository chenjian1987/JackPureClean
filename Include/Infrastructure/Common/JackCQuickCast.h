#pragma once

#include "UniqueString.h"

// 高效安全的运行时动态类型转换。
// 使用方法：
//// A.h
//namespace JackC
//{
//    class A
//    {
//        DEFINE_CAST_BASE(JackC::A);
//    };
//}
//
//// B.h
//namespace JackC
//{
//    class B
//    {
//        DEFINE_CAST_BASE(JackC::B);
//    };
//}
//
//// C.h
//namespace JackC
//{
//    class C : public A, public B
//    {
//        DEFINE_CAST_DERIVED2(JackC::C, JackC::A, JackC::B);
//    };
//}
//
//// D.h
//namespace JackC
//{
//    class D : public A
//    {
//        DEFINE_CAST_DERIVED(JackC::D, JackC::A);
//    };
//}
//
//namespace JackC
//{
//    void Test()
//    {
//        A* pA = new C;
//        B* pB = quick_cast<B>(pA); // 成功
//        C* pC = quick_cast<C>(pA); // 成功
//        D* pD = quick_cast<D>(pA); // 失败
//    }
//}


#define DEFINE_CLASS_TYPE(ClassFullName) \
public: \
    static const char* ClassType() \
    { \
        static const char* s_classType = JackC::UniqueString::Find(#ClassFullName, true)->GetContent().c_str(); \
        return s_classType; \
    } \
    virtual const char* ObjectKind() const    \
    {  \
       return ClassType(); \
    }

#define DEFINE_QUERY_OBJECT_STATIC(ClassFullName) \
    template <class From> \
    static ClassFullName* QueryObjectStatic(const From* pFrom) \
    { \
        if (!pFrom) \
            return nullptr; \
        return static_cast<ClassFullName*>(pFrom->QueryObject(ClassFullName::ClassType())); \
    }

// 在有继承体系的基类中使用。
#define DEFINE_CAST_BASE(ClassFullName) \
    DEFINE_CLASS_TYPE(ClassFullName) \
    virtual void* QueryObject(const char* classType) const \
    { \
        if(!classType) \
            return nullptr; \
        if(ClassType() == classType) \
            return const_cast<ClassFullName*>(this); \
        return nullptr; \
    } \
    DEFINE_QUERY_OBJECT_STATIC(ClassFullName)

#define DEFINE_CAST_DERIVED_START(ClassFullName) \
    DEFINE_CLASS_TYPE(ClassFullName) \
    void* QueryObject(const char* classType) const override \
    { \
        if(!classType) \
            return nullptr; \
        if(ClassType() == classType) \
            return const_cast<ClassFullName*>(this);

#define DEFINE_QUERY_BASE_OBJECT(BaseClassFullName) \
        if(void* pRes = BaseClassFullName::QueryObject(classType)) \
            return pRes;

#define DEFINE_CAST_DERIVED_END() \
        return nullptr; \
    } \

// 在有承体系的派生类中使用。
#define DEFINE_CAST_DERIVED(ClassFullName, BaseClassFullName) \
    DEFINE_CAST_DERIVED_START(ClassFullName) \
    DEFINE_QUERY_BASE_OBJECT(BaseClassFullName) \
    DEFINE_CAST_DERIVED_END() \
    DEFINE_QUERY_OBJECT_STATIC(ClassFullName)

#define DEFINE_CAST_DERIVED2(ClassFullName, BaseClassFullName1, BaseClassFullName2) \
    DEFINE_CAST_DERIVED_START(ClassFullName) \
    static_assert((!std::is_same<BaseClassFullName1, BaseClassFullName2>::value), "The type of ParentClass1 must be not same as ParentClass2."); \
    DEFINE_QUERY_BASE_OBJECT(BaseClassFullName1) \
    DEFINE_QUERY_BASE_OBJECT(BaseClassFullName2) \
    DEFINE_CAST_DERIVED_END() \
    DEFINE_QUERY_OBJECT_STATIC(ClassFullName)

#define DEFINE_CAST_DERIVED3(ClassFullName, BaseClassFullName1, BaseClassFullName2, BaseClassFullName3) \
    DEFINE_CAST_DERIVED_START(ClassFullName) \
    static_assert((!std::is_same<BaseClassFullName1, BaseClassFullName2>::value), "The type of ParentClass1 must be not same as ParentClass2."); \
    static_assert((!std::is_same<BaseClassFullName2, BaseClassFullName3>::value), "The type of ParentClass2 must be not same as ParentClass3."); \
    static_assert((!std::is_same<BaseClassFullName1, BaseClassFullName3>::value), "The type of ParentClass1 must be not same as ParentClass3."); \
    DEFINE_QUERY_BASE_OBJECT(BaseClassFullName1) \
    DEFINE_QUERY_BASE_OBJECT(BaseClassFullName2) \
    DEFINE_QUERY_BASE_OBJECT(BaseClassFullName3) \
    DEFINE_CAST_DERIVED_END() \
    DEFINE_QUERY_OBJECT_STATIC(ClassFullName)


namespace JackC
{
#define quick_cast thread_safe_quick_cast

    //线程安全的类型快速转换
    template <typename To, typename From>
    const To* thread_safe_quick_cast(const From* pFrom)
    {
        static_assert(!std::is_pointer<To>::value, "quick_cast only accepts non-pointer type");

        return To::QueryObjectStatic(pFrom);
    }

    //线程安全的类型快速转换
    template <typename To, typename From>
    To* thread_safe_quick_cast(From* pFrom)
    {
        static_assert(!std::is_pointer<To>::value, "quick_cast only accepts non-pointer type");

        return To::QueryObjectStatic(pFrom);
    }
}
