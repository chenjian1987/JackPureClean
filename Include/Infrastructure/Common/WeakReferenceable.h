#pragma once

#include "JackCDevService.h"
#include <atomic>

namespace JackC
{
	// 可以被WeakPtr<T>引用的对象的基类
	class JACKC_COMMON_EXPORT WeakReferenceable
	{
	public:
		WeakReferenceable();
		virtual ~WeakReferenceable(void);

		WeakReferenceable(const WeakReferenceable& other);
		WeakReferenceable& operator=(const WeakReferenceable& other);

	private:
		template<class T> friend class WeakPtr;
		class Proxy
		{
			CANNOT_COPY_OR_ASSIGN(Proxy);
		public:
			explicit Proxy(WeakReferenceable* pObject) : m_pObject(pObject), m_refCount(0) {}
			virtual ~Proxy() { if (m_pObject) m_pObject->m_pProxy = nullptr; }
			WeakReferenceable* operator->() { return m_pObject; }
			const WeakReferenceable* operator->() const { return m_pObject; }
			WeakReferenceable& operator*() { return *m_pObject; }
			const WeakReferenceable& operator*() const { return *m_pObject; }
			WeakReferenceable* Get() { return m_pObject; }
			const WeakReferenceable* Get() const { return m_pObject; }
			void Reset() { m_pObject = nullptr; }
			void Ref() { m_refCount++; }
			void UnRef() { if (--m_refCount == 0) delete this; }
			bool IsValid() { return m_pObject != nullptr; }
		private:
			friend class WeakReferenceable;

			// 指向被代理的对象
			WeakReferenceable* m_pObject;

			// 有多少weakptr引用到m_pObject
			std::atomic<int> m_refCount;
		};

		Proxy* GetOrCreateProxy() const;
	private:
		mutable Proxy* m_pProxy;
	};
}


