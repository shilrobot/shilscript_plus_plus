#ifndef SS_THISEXPR_H
#define SS_THISEXPR_H

#include "ThisExpr.h"

namespace SS {

class SS_EXPORT ThisExpr : public Expr
{
public:
	SS_CLASS(UnaryExpr, "ThisExpr", Expr);

	ThisExpr() {}
};

}

#endif // SS_THISEXPR_H
