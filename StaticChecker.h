#ifndef SS_STATICCHECKER_H
#define SS_STATICCHECKER_H

#include "Prereqs.h"
#include "OverloadResolve.h"
#include "BinaryExpr.h"
#include "UnaryExpr.h"

namespace SS {

class Package;
class Class;
class Variable;
class Function;
class Node;
class Scope;
class Statement;
class Expr;
class Type;

// Context info used when compiling statements in a func
struct StatementContext
{
	Function* func;
	bool canBreak;
	bool canContinue;
	Scope* scope;
	const StatementContext* parent;

	StatementContext() : func(0), canBreak(false), canContinue(false), scope(0), parent(0)
	{
	}
};

// Context info used when compiling an expression
// TODO: If no extra context develops, then we can probably just dump this & use statement context everywhere
struct ExprContext
{
	const StatementContext* stmtCtx;

	ExprContext() : stmtCtx(0)
	{
	}
};

// Performs the static checking passes on the code tree.
// This consists of taking the tree of nodes constructed by the parser, and
// performing various static checking operations over multiple passes.
class SS_EXPORT StaticChecker
{
public:
	StaticChecker(Package* pkg);
	bool Check();

private:
	// First phase: Checking classes' names/bases
	void ResolveClassBase(Class* cls);

	// Second phase: Process all classes' members (check variables/functions/etc.)
	void ProcessClassMembersRecursive(Class* cls);
	bool ProcessClassMembers(Class* cls);
	bool CheckVariable(Variable* var);
	bool CheckFunction(Function* func);
	void CheckStatement(Statement* stmt, const StatementContext& ctx);

	// Third phase: Static checking of function contents
	void CheckFunctionBody(Function* func);
	Expr* CheckExpr(Expr* expr, const ExprContext& ctx);
	Expr* CheckExprWithType(Expr* expr, const Type* desiredType, const ExprContext& ctx);
	String DumpExpr(Expr* expr) const;
	const OverloadVector& GetBinaryOverloads(BinaryOp op) const;
	const OverloadVector& GetUnaryOverloads(UnaryOp op) const;

	bool CheckChild(Node* parent, Node* child);

	Package* m_pkg;
};

}

#endif // SS_STATICCHECKER_H
