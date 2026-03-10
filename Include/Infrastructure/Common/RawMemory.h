#pragma once

#include "OwnerPtr.h"
#include "JackCDevService.h"
#include "DebugMessage.h"

namespace JackC 
{
    // 拥有一块内存块。
    class JACKC_COMMON_EXPORT RawMemory
    {
        CANNOT_COPY_OR_ASSIGN(RawMemory);
    public:
        RawMemory();
        // 构造一块内存块
        // @param zeroMemory: 是否将内存块全置0,构造对象时zeroMemory必须为true
        explicit RawMemory(size_t size, bool zeroMemory=false);
        // 析构释放内存块
        virtual ~RawMemory();
        // 内存块首地址
        char* GetStartAddress() const {return m_ptr;}
        // 内存块大小
        size_t GetSize() const {return m_size;}
        //重新分配内存,内存块全置0，已有内存数据将会被清除
        void Resize(size_t size);
        // 在内存块上构造对象
        template<class T> static OwnerPtr<T> ConstructObjectOnZeroMemory()
        {
            RawMemory rawMemory((size_t)sizeof(T), true);
            rawMemory.QuickConstructObject<T>();
            return rawMemory.ReleaseOwnership<T>();
        }
        // 接收一个参数在内存块上构造对象
        template<class T, class Arg> static OwnerPtr<T> ConstructObjectOnZeroMemory(Arg&& arg)
        {
            RawMemory rawMemory((size_t)sizeof(T), true);
            rawMemory.QuickConstructObject<T>(std::forward<Arg>(arg));
            return rawMemory.ReleaseOwnership<T>();
        }
        // 在内存块上构造对象
        template<class T> T* ConstructObject()
        {
            DBG_WARN_AND_RETURN_NULLPTR_UNLESS(sizeof(T) == m_size, L"the class size must be equal to raw memory size.", L"jack.chen", L"2025-07-19");
            DBG_WARN_AND_RETURN_NULLPTR_UNLESS(!m_constructed, L"object had been constructed before", L"jack.chen", L"2025-07-19");
            DBG_WARN_AND_RETURN_NULLPTR_UNLESS(IsValid(), L"should not construct object on invalid raw memory", L"jack.chen", L"2025-07-19");
            DBG_WARN_AND_RETURN_NULLPTR_UNLESS(IsZeroMemory(), L"memory should all be zero.", L"jack.chen", L"2025-07-19");

            return QuickConstructObject<T>();
        }
        // 接收一个参数的在内存块上构造对象
        template<class T, class Arg> T* ConstructObject(Arg&& arg)
        {
            DBG_WARN_AND_RETURN_NULLPTR_UNLESS(sizeof(T) == m_size, L"the class size must be equal to raw memory size.", L"jack.chen", L"2025-07-19");
            DBG_WARN_AND_RETURN_NULLPTR_UNLESS(!m_constructed, L"object had been constructed before", L"jack.chen", L"2025-07-19");
            DBG_WARN_AND_RETURN_NULLPTR_UNLESS(IsValid(), L"should not construct object on invalid raw memory", L"jack.chen", L"2025-07-19");
            DBG_WARN_AND_RETURN_NULLPTR_UNLESS(IsZeroMemory(), L"memory should all be zero.", L"jack.chen", L"2025-07-19");

            return QuickConstructObject<T>(std::forward<Arg>(arg));
        }
        // 将内存块所有权交给构造好的对象
        template<class T> OwnerPtr<T> ReleaseOwnership() 
        {
            DBG_WARN_AND_RETURN_NULLPTR_UNLESS(sizeof(T)==m_size, L"the class size must be equal to raw memory size.", L"jack.chen", L"2025-07-19");
            DBG_WARN_AND_RETURN_NULLPTR_UNLESS(m_constructed, L"object was not constructed", L"jack.chen", L"2025-07-19");
            OwnerPtr<T> opT = JackC::OwnerPtr<T>((T*)m_ptr);
            m_ptr = 0;
            return opT;
        }
        // 是否是有效的内存块
        bool IsValid() const {return m_ptr&&m_size>0;}
        // 内存块是否是全0
        bool IsZeroMemory() const;
    private:
        // 在内存块上构造对象。不做有效性检查。
        template<class T> T* QuickConstructObject()
        {
            new (m_ptr) T();
            m_constructed = true;
            return (T*)m_ptr;
        }
        // 接受一个参数的在内存块上构造对象。不做有效性检查。
        template<class T, class Arg> T* QuickConstructObject(Arg&& arg)
        {
            new (m_ptr) T(arg);
            m_constructed = true;
            return (T*)m_ptr;
        }
    private:
        char* m_ptr;
        size_t m_size;
        bool m_constructed;
    };
}