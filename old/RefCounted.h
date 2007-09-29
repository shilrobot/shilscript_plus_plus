#ifndef SS_REFCOUNTED_H
#define SS_REFCOUNTED_H

#include "Prereqs.h"

namespace SS {

class RefCounted : public Base
{
public:
	SS_CLASS(RefCounted, "RefCounted", Base)

	RefCounted() : m_refs(0) {}

	virtual ~RefCounted()
	{
		SSAssert(m_refs == 0);
	}

	void AddRef()
	{
		++m_refs;
	}

	void Release()
	{
		--m_refs;
		if(m_refs == 0)
			delete this;
	}

private:
	int m_refs;
};

}

#endif // SS_REFCOUNTED_H
