#ifndef SS_BASE_H
#define SS_BASE_H

#include "TypeInfo.h"

namespace SS {

#define SS_CLASS(cls, name, base) \
public: \
	virtual SS::TypeInfo* DynamicType() const { return StaticType(); } \
	static SS::TypeInfo* StaticType() { static SS::TypeInfo typeinfo(name, SS::TypeOf<base>()); return &typeinfo; }

class SS_EXPORT Base
{
public:
	Base();
	virtual ~Base();

	virtual TypeInfo* DynamicType() const { return StaticType(); }

	static TypeInfo* StaticType()
	{
		static TypeInfo typeinfo("Base", 0);
		return &typeinfo;
	}

	bool IsA(TypeInfo* type) const
	{
		SSAssert(type != 0);

		TypeInfo* currBase = DynamicType();

		// Search all our base classes for this class
		while(currBase != 0)
		{
			if(currBase == type)
				return true;
			else
			{
				SSAssert(currBase != currBase->GetBase());
				currBase = currBase->GetBase();
			}
		}

		return false;
	}

	template<class T>
	bool IsA() const
	{
		return IsA(TypeOf<T>());
	}

	static void DumpInstances();

private:
	Base*	m_prev;
	Base*	m_next;
	int		m_magic;

	static Base* s_head;
	static Base* s_tail;
};

}

#endif // SS_BASE_H
