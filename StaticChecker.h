#ifndef SS_STATICCHECKER_H
#define SS_STATICCHECKER_H

#include "Prereqs.h"

namespace SS {

class Package;
class Class;
class Variable;
class Function;
class Node;
class Scope;
class Statement;
class Expr;

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
struct ExprContext
{
	bool lvalue;
	const StatementContext* stmtCtx;

	ExprContext() : lvalue(false), stmtCtx(0)
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
	Expr* CheckAndTransformExpr(Expr* expr, const ExprContext& ctx);
	String DumpExpr(Expr* expr) const;
	bool IsInteger(Type* type) const;
	bool IsNumber(Type* type) const;
	Type* WidenNumbers(Type* a, Type* b) const;

	bool CheckChild(Node* parent, Node* child);

	Package* m_pkg;
};

}

#endif // SS_STATICCHECKER_H
