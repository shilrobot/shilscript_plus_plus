#include "Function.h"
#include "TypeExpr.h"
#include "Type.h"
#include "Name.h"
#include "CompileErrors.h"
#include "Class.h"

namespace SS {

Function::Function() : m_access(ACCESS_PUBLIC),
						m_returnTypeExpr(0),
						m_returnType(0),
						m_body(0),
						m_static(false),
						m_virtual(false),
						m_sealed(false)
{
}

Function::~Function()
{
	delete m_returnTypeExpr;
	// TODO: delete m_body;
	// TODO: Delete params
}


}
