#ifndef SS_REF_H
#define SS_REF_H

#include "Prereqs.h"

namespace SS {

template<class T>
struct Ref
{
public:
	SS_FORCEINLINE Ref(T* ptr)
	{
		m_ptr = ptr;
	}

	SS_FORCEINLINE Ref(const Ref<T>& other)
	{
		m_ptr = other.m_ptr;
		if(m_ptr != 0)
			m_ptr->AddRef();
	}

	template<class U>
	SS_FORCEINLINE Ref(const Ref<U>& other)
	{
		m_ptr = static_cast<T*>(other.m_ptr);
		if(m_ptr != 0)
			m_ptr->AddRef();
	}

	SS_FORCEINLINE ~Ref()
	{
		if(m_ptr != 0)
		{
			m_ptr->Release();
			m_ptr = 0;
		}
	}

	template<class U>
	SS_FORCEINLINE operator U*() const
	{
		return static_cast<U*>(m_ptr);
	}

	SS_FORCEINLINE T& operator*()
	{
		return *m_ptr;
	}

	SS_FORCEINLINE T* operator->() const
	{
		return m_ptr;
	}

	SS_FORCEINLINE operator bool() const
	{
		return m_ptr != 0;
	}

	SS_FORCEINLINE Ref& operator=(const Ref<T>& other)
	{
		if(m_ptr != 0)
			m_ptr->Release();
		m_ptr = other.m_ptr;
		if(m_ptr != 0)
			m_ptr->AddRef();
	}

	template<class U>
	SS_FORCEINLINE Ref& operator=(const Ref<U>& other)
	{
		if(m_ptr != 0)
			m_ptr->Release();
		m_ptr = static_cast<T*>(other.m_ptr);
		if(m_ptr != 0)
			m_ptr->AddRef();
	}

	SS_FORCEINLINE T* Get() const
	{
		return m_ptr;
	}

private:
	T* m_ptr;
};

#define FE_THIS (SS::MakeRef(this))

template<class T>
SS_FORCEINLINE Ref<T> MakeRef(T* ptr)
{
	return Ref<T>(ptr);
}

template<class A, class B>
SS_FORCEINLINE bool operator==(const Ref<A>& a, const B* b)
{
	return a.Get() == b;
}

template<class A, class B>
SS_FORCEINLINE bool operator==(const A* a, const Ref<B>& b)
{
	return a == b.Get();
}

template<class A, class B>
SS_FORCEINLINE bool operator==(const Ref<A>& a, const Ref<B>& b)
{
	return a.Get() == b.getPointer();
}

template<class A, class B>
SS_FORCEINLINE bool operator!=(const Ref<A>& a, const B* b)
{
	return a.Get() != b;
}

template<class A, class B>
SS_FORCEINLINE bool operator!=(const A* a, const Ref<B>& b)
{
	return a != b.Get();
}

template<class A, class B>
SS_FORCEINLINE bool operator!=(const Ref<A>& a, const Ref<B>& b)
{
	return a.Get() != b.getPointer();
}

#define SS_DEFINE_REF(ns, cls) typedef Ref<ns :: cls> cls##Ref;

}

#endif // SS_REF_H
