#ifndef __Delegate_Handler_H_
#define __Delegate_Handler_H_

#include "JackCNew.h"
#include "TransferOwnership.h"
#include <list>

// zhoudc 2016-07-12
// 委托类实现：使用委托可快速实现观察者模式，优点是免去繁杂重复的代码,支持注册现有类成员函数(包括虚方法）和一般函数响应事件
// 注意：目前最多支持8个参数（可用宏扩展）

#define DECLARE_PARAMS(...) __VA_ARGS__
#define DECLARE_ARGS(...) __VA_ARGS__

//0个参数的委托
#define DELEGATE0(retType, name) \
    DECLARE_DELEGATE(retType, name, DECLARE_PARAMS(void), )

//1个参数的委托
#define DELEGATE1(retType, name, p1) \
    DECLARE_DELEGATE( \
    retType, \
    name, \
    DECLARE_PARAMS(p1 a), \
    DECLARE_ARGS(a))

//2个参数的委托
#define DELEGATE2(retType, name, p1, p2) \
    DECLARE_DELEGATE( \
    retType, \
    name, \
    DECLARE_PARAMS(p1 a, p2 b), \
    DECLARE_ARGS(a, b))

//3个参数的委托
#define DELEGATE3(retType, name, p1, p2, p3) \
    DECLARE_DELEGATE( \
    retType, \
    name, \
    DECLARE_PARAMS(p1 a, p2 b, p3 c), \
    DECLARE_ARGS(a, b, c))

//4个参数的委托
#define DELEGATE4(retType, name, p1, p2, p3, p4) \
    DECLARE_DELEGATE( \
    retType, \
    name, \
    DECLARE_PARAMS(p1 a, p2 b, p3 c, p4 d), \
    DECLARE_ARGS(a, b, c, d))

//5个参数的委托
#define DELEGATE5(retType, name, p1, p2, p3, p4, p5) \
    DECLARE_DELEGATE( \
    retType, \
    name, \
    DECLARE_PARAMS(p1 a, p2 b, p3 c, p4 d, p5 e), \
    DECLARE_ARGS(a, b, c, d, e))

//6个参数的委托
#define DELEGATE6(retType, name, p1, p2, p3, p4, p5, p6) \
    DECLARE_DELEGATE( \
    retType, \
    name, \
    DECLARE_PARAMS(p1 a, p2 b, p3 c, p4 d, p5 e, p6 f), \
    DECLARE_ARGS(a, b, c, d, e, f))

//7个参数的委托
#define DELEGATE7(retType, name, p1, p2, p3, p4, p5, p6, p7) \
    DECLARE_DELEGATE( \
    retType, \
    name, \
    DECLARE_PARAMS(p1 a, p2 b, p3 c, p4 d, p5 e, p6 f, p7 g), \
    DECLARE_ARGS(a, b, c, d, e, f, g))

//8个参数的委托
#define DELEGATE8(retType, name, p1, p2, p3, p4, p5, p6, p7, p8) \
    DECLARE_DELEGATE( \
    retType, \
    name, \
    DECLARE_PARAMS(p1 a, p2 b, p3 c, p4 d, p5 e, p6 f, p7 g, p8 h), \
    DECLARE_ARGS(a, b, c, d, e, f, g, h))


#define DECLARE_DELEGATE(retType, name, params, args)                         \
class I##name {                                                               \
public:                                                                       \
    I##name() { }                                                             \
    virtual ~I##name() { }                                                    \
    virtual bool IsType(const std::type_info& _type) = 0;                     \
    virtual bool Compare(I##name *_delegate) const = 0;                       \
    virtual retType Invoke(params) = 0;                                       \
};                                                                            \
template<typename T>                                                          \
class name : public I##name {                                                 \
public:                                                                       \
    name():m_pType(nullptr),m_pFunc(nullptr) { }                              \
    name(T* pType, retType (T::*pFunc)(params))                               \
    : m_pType(pType), m_pFunc(pFunc) { }                                      \
    virtual bool IsType(const std::type_info& _type) override{                \
        return (typeid(name) == _type);                                       \
    }                                                                         \
    virtual bool Compare(I##name *_delegate) const override{                  \
        if(_delegate || !_delegate->IsType(typeid(name)) ) return false;      \
        name* cast = static_cast<name*>(_delegate);                           \
        return cast->m_pType == m_pType && cast->m_pFunc == m_pFunc;          \
    }                                                                         \
    retType Invoke( params ) override {                                       \
    return (m_pType->*m_pFunc)(args);                                         \
    }                                                                         \
    retType operator()( params ) { return Invoke(args); }                     \
private:                                                                      \
    T* m_pType;                                                               \
    retType (T::*m_pFunc)(params);                                            \
};                                                                            \
template<>                                                                    \
class name<void> : public I##name {                                           \
public:                                                                       \
    name():m_pFunc(nullptr) { }                                               \
    name(retType (*pFunc)(params))                                            \
    : m_pFunc(pFunc) { }                                                      \
    virtual bool IsType(const std::type_info& _type) override{                \
        return (typeid(name) == _type);                                       \
    }                                                                         \
    virtual bool Compare(I##name *_delegate) const override{                  \
        if(_delegate || !_delegate->IsType(typeid(name)) ) return false;      \
        name* cast = static_cast<name*>(_delegate);                           \
        return cast->m_pFunc == m_pFunc;                                      \
    }                                                                         \
    retType Invoke(params) override {                                         \
    return (*m_pFunc)(args);                                                  \
    }                                                                         \
    retType operator()( params ) { return Invoke(args); }                     \
private:                                                                      \
    retType (*m_pFunc)(params);                                               \
};                                                                            \
inline OwnerPtr<I##name> newDelegate( retType (*pFunc)(params) )              \
{                                                                             \
    return NEW_AS_OWNER_PTR(name<void>, pFunc);                               \
}                                                                             \
template<typename T>                                                          \
inline OwnerPtr<I##name> newDelegate(T* pType, retType (T::*pFunc)(params) )  \
{                                                                             \
    return NEW_AS_OWNER_PTR(name<T>, pType, pFunc);                           \
}                                                                             \
class CMulti##name {                                                          \
public:                                                                       \
    typedef std::list<OwnerPtr<I##name>> ListDelegate;                        \
    CMulti##name () { }                                                       \
    ~CMulti##name () { clear(); }                                             \
    bool empty() const  { return m_listDelegates.empty(); }                   \
    void clear() {  m_listDelegates.clear(); }                                \
    CMulti##name& operator+=(OwnerPtr<I##name> _delegate){                    \
       if(_delegate == nullptr) return *this;                                 \
       for(auto it=m_listDelegates.begin(); it!=m_listDelegates.end(); ++it)  \
       {                                                                      \
           if ((*it) && (*it)->Compare(_delegate.get())){                     \
                it = m_listDelegates.erase(it);                               \
                break;                                                        \
            }                                                                 \
       }                                                                      \
       m_listDelegates.push_back(TransferOwnership(_delegate));               \
       return *this;                                                          \
    }                                                                         \
    CMulti##name& operator-=(OwnerPtr<I##name> _delegate) {                   \
       for(auto it=m_listDelegates.begin(); it!=m_listDelegates.end(); ++it)  \
       {                                                                      \
          if((*it) && (*it)->Compare(_delegate.get())) {                      \
             it = m_listDelegates.erase(it);                                  \
             break;                                                           \
          }                                                                   \
       }                                                                      \
       return *this;                                                          \
    }                                                                         \
    void operator()(params)                                                   \
    {                                                                         \
        auto iter = m_listDelegates.begin();                                  \
        while (iter != m_listDelegates.end())                                 \
        {                                                                     \
            (*iter)->Invoke(args);                                            \
            ++iter;                                                           \
        }                                                                     \
    }                                                                         \
private:                                                                      \
    CMulti##name (const CMulti##name& _event);                                \
    CMulti##name& operator=(const CMulti##name& _event);                      \
private:                                                                      \
    ListDelegate m_listDelegates;                                             \
}

// 预定义的事件响应函数Signature
namespace JackC
{
    DELEGATE0(void, EventVoid);         // void (void)
    DELEGATE1(void, EventBool, bool);   // void (bool)
}
//example 
/*

// 普通函数1  
void func(const std::wstring& a, int b)  
{  
    wprintf(L"func( %s, %d )",  a , b );  
}

//一般基类
class ClassA
{
public:
    void Func(const std::wstring& a, int b)
    {
        wprintf(L"ClassA::Func( %s, %d )",  a , b );
    }
    virtual void VirtualFunc(const std::wstring& a, int b)
    {
        wprintf(L"ClassA::VirtualFunc( %s, %d )",  a , b );
    }
};

//派生类
class DerivedClass : public ClassA
{
    public:
    virtual void VirtualFunc(const std::wstring& a, int b)
    {
    wprintf(L"DerivedClass::VirtualFunc( %s, %d )",  a , b );
    }
};

//定义2个参数的委托代理类
DELEGATE2(void, MyDelegage, const std::wstring&, int);
typedef MyDelegage<ClassA> EvenetHandler; //定义单一委托
typedef CMultiMyDelegage   MultiEvenetHandler; //定义多委托

int _tmain(int argc, _TCHAR* argv[])
{
    ClassA* pClassAObj = new ClassA();
    ClassA* pDerivedObj = new DerivedClass();

    EvenetHandler event(pClassAObj, &ClassA::VirtualFunc);
    event(L"test0", 1);

    MultiEvenetHandler  eventHandles;
    //添加一般函数委托
    eventHandles += newDelegate(func);
    //添加类成员函数委托
    eventHandles += newDelegate(pClassAObj, &ClassA::Func);
    //添加派生类虚函数委托
    eventHandles += newDelegate(pDerivedObj, &ClassA::VirtualFunc);

    eventHandles(L"test", 11);
    return 0;
}
*/


#endif //__Delegate_Handler_H_


