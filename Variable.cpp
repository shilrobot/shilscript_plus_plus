#include "Variable.h"
#include "Name.h"
#include "CompileErrors.h"
#include "TypeExpr.h"
#include "Expr.h"
#include "Type.h"

namespace SS {

Variable::Variable() : m_access(ACCESS_PUBLIC),
					   m_typeExpr(0),
					   m_type(0),
					   m_initExpr(0),
					   m_static(false),
					   m_const(false)
{
}

Variable::~Variable()
{
	delete m_typeExpr;
	// TODO: delete m_initExpr;
}

}
