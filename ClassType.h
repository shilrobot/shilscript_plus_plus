#ifndef SS_CLASSTYPE_H
#define SS_CLASSTYPE_H

namespace SS {

#include "Type.h"

class Class;

class ClassType : public Type
{
public:
	SS_CLASS(ClassType, "ClassType", Type)

	ClassType(Class* cls) : m_cls(cls)
	{
		SSAssert(cls != 0);
	}

	Class* GetClass() const { return m_cls; }	

	String GetName() const { return m_cls->GetFullName(); }

public:
	Class* m_cls;
};

}

#endif // SS_CLASSTYPE_H

