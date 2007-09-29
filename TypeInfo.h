#ifndef SS_TYPEINFO_H
#define SS_TYPEINFO_H

#include "Prereqs.h"

namespace SS {

class TypeInfo;

template<class T>
inline TypeInfo* TypeOf(T* ptr)
{
	if(!ptr)
		return 0;
	return ptr->DynamicType();
}

template<class T>
inline TypeInfo* TypeOf()
{
	return T::StaticType();
}

class SS_EXPORT TypeInfo
{
public:
	TypeInfo(const char* name, TypeInfo* base) : m_name(name), m_base(base) {}
	const char* GetName() const { return m_name; }
	TypeInfo* GetBase() const { return m_base; }

private:
	const char* m_name;
	TypeInfo* m_base;
};

}

#endif // SS_TYPEINFO_H
