#include "Function.h"
#include "TypeExpr.h"
#include "Type.h"
#include "Name.h"
#include "CompileErrors.h"
#include "Class.h"
#include "Code.h"
#include "Local.h"
#include "Statement.h"

namespace SS {

Parameter::~Parameter()
{
	delete m_typeExpr;
}

Function::Function() : m_access(ACCESS_PUBLIC),
						m_returnTypeExpr(0),
						m_returnType(0),
						m_body(0),
						m_static(false),
						m_virtual(false),
						m_sealed(false),
						m_code(0)
{
}

Function::~Function()
{
	delete m_returnTypeExpr;

	delete m_body;

	for(int i=0; i<GetParameterCount(); ++i)
	{
		Parameter* param = GetParameter(i);
		delete param;
	}

	for(int i=0; i<GetLocalCount(); ++i)
		delete GetLocal(i);

	delete m_code;
}


}
