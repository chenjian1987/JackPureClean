#pragma once

#include <memory>
#include <utility>
#include "OwnerPtr.h"
#include "JackCQuickCast.h"

template<class Derived, class Base> inline
    bool IsA(const Base* obj)
{
    return dynamic_cast<const Derived*>(obj) != nullptr;
}

template<class Derived, class Base> inline
    bool IsA(const JackC::OwnerPtr<Base>& obj)
{
    return dynamic_cast<const Derived*>(obj.get()) != nullptr;
}

#define TRANSFER_OWNERSHIP_TO_THIRD_PARTY(OWNER) (OWNER).release()
#define TRANSFER_OWNERSHIP_FROM_THIRD_PARTY(POINTER) TransferOwnership_DontUse(POINTER)

/*!
*@brief  转移资源所有权(只有少量几个地方可以使用TransferOwnership_DontUse)
*@param[in] pointer 指向资源的指针
*/
template<class T> inline JackC::OwnerPtr<T> TransferOwnership_DontUse(T* pointer)
{
    return JackC::OwnerPtr<T>(pointer);
}

/*!
*@brief  转移资源所有权
*@param[in] unique_pointer 拥有资源的OwnerPtr
*/
template<class T> inline typename std::remove_reference<T>::type&&
    TransferOwnership(T&& unique_pointer)
{
    // forward _Arg as movable
    return std::move(unique_pointer);
}


template<class T> inline std::shared_ptr<T> TransferOwnershipToSharedPtr(JackC::OwnerPtr<T>& opT)
{
    return std::shared_ptr<T>(opT.release());
}

template<class T> inline std::shared_ptr<T> TransferOwnershipToSharedPtr(JackC::OwnerPtr<T>&& opT)
{
    return std::shared_ptr<T>(opT.release());
}

template<class Derived, class Base> inline
    JackC::OwnerPtr<Derived> TransferOwnershipCast(JackC::OwnerPtr<Base>& obj)
{
    Derived* pDerived = dynamic_cast<Derived*>(obj.get());
    if (pDerived != nullptr)
    {
        obj.release();
        return TransferOwnership_DontUse(pDerived);
    }
    return nullptr;
}

template<class Derived, class Base> inline
    JackC::OwnerPtr<Derived> TransferOwnershipCast(JackC::OwnerPtr<Base>&& obj)
{
    Derived* pDerived = dynamic_cast<Derived*>(obj.get());
    if (pDerived != nullptr)
    {
        obj.release();
        return TransferOwnership_DontUse(pDerived);
    }
    return nullptr;
}

template<class Derived, class Base> inline
    JackC::OwnerPtr<Derived> TransferOwnershipCast(Base* pObj)
{
    Derived* pDerived = dynamic_cast<Derived*>(pObj);
    if (pDerived != nullptr)
        return TransferOwnership_DontUse(pDerived);
    return nullptr;
}

// dynamic_cast很耗时，特定场景dynamic_cast不是必须的，可以使用static_cast。
// 从基类转换到派生类，慎用static_cast，只有在特定场景，确保不会发生意外才能使用static_cast。
template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipStaticCast(JackC::OwnerPtr<Base>& obj)
{
    return TransferOwnership_DontUse(static_cast<Derived*>(obj.release()));
}

template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipStaticCast(JackC::OwnerPtr<Base>&& obj)
{
    return TransferOwnership_DontUse(static_cast<Derived*>(obj.release()));
}

template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipStaticCast(Base* pObj)
{
    return TransferOwnership_DontUse(static_cast<Derived*>(pObj));
}


// quick_cast
template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipQuickCast(JackC::OwnerPtr<Base>& obj)
{
    return TransferOwnership_DontUse(JackC::quick_cast<Derived>(obj.release()));
}

template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipQuickCast(JackC::OwnerPtr<Base>&& obj)
{
    return TransferOwnership_DontUse(JackC::quick_cast<Derived>(obj.release()));
}

template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipQuickCast(Base* pObj)
{
    return TransferOwnership_DontUse(JackC::quick_cast<Derived>(pObj));
}
