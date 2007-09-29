#ifndef SS_STATEMENT_H
#define SS_STATEMENT_H

#include "Node.h"
#include "Expr.h"

namespace SS {

class Statement : public Node
{
public:
	SS_CLASS(Statement, "Statement", Node);
};

class EmptyStatement : public Statement
{
public:
	SS_CLASS(EmptyStatement, "EmptyStatement", Statement);
};

class BreakStatement : public Statement
{
public:
	SS_CLASS(BreakStatement, "BreakStatement", Statement);
};

class ContinueStatement : public Statement
{
public:
	SS_CLASS(ContinueStatement, "ContinueStatement", Statement);
};

class ReturnStatement : public Statement
{
public:
	SS_CLASS(ReturnStatement, "ReturnStatement", Statement);

	ReturnStatement(Expr* expr) : m_expr(expr) {}

	~ReturnStatement()
	{
		delete m_expr;
	}

	SS_GETSET(Expr*, Expr, m_expr);
private:
	Expr* m_expr;
};

class ThrowStatement : public Statement
{
public:
	SS_CLASS(ThrowStatement, "ThrowStatement", Statement);

	ThrowStatement(Expr* expr) : m_expr(expr) {}

	~ThrowStatement()
	{
		delete m_expr;
	}

	SS_GETSET(Expr*, Expr, m_expr);
private:
	Expr* m_expr;
};

class ExprStatement : public Statement
{
public:
	SS_CLASS(ExprStatement, "ExprStatement", Statement);

	ExprStatement(Expr* expr) : m_expr(expr) {}

	~ExprStatement()
	{
		delete m_expr;
	}

	SS_GETSET(Expr*, Expr, m_expr);
private:
	Expr* m_expr;
};

class BlockStatement : public Statement
{
public:
	SS_CLASS(ExprStatement, "ExprStatement", Statement);

	BlockStatement() {}

	~BlockStatement()
	{
		for(int i=0; i < GetStatementCount(); ++i)
			delete GetStatement(i);
	}

	SS_ADD(Statement*, AddStatement, m_stmts);
	SS_INDEX(Statement*, Statement, m_stmts);
private:
	std::vector<Statement*>	m_stmts;
};

class IfStatement : public Statement
{
public:
	SS_CLASS(IfStatement, "IfStatement", Statement);

	IfStatement(Expr* cond, Statement* ifTrue, Statement* ifFalse) : m_cond(cond), m_ifTrue(ifTrue), m_ifFalse(ifFalse) {}

	~IfStatement()
	{
		delete m_cond;
		delete m_ifTrue;
		delete m_ifFalse;
	}

	SS_GETSET(Expr*, Condition, m_cond);
	SS_GETSET(Statement*, IfTrue, m_ifTrue);
	SS_GETSET(Statement*, IfFalse, m_ifFalse);
private:
	Expr* m_cond;
	Statement* m_ifTrue;
	Statement* m_ifFalse;
};


class WhileStatement : public Statement
{
public:
	SS_CLASS(WhileStatement, "WhileStatement", Statement);

	WhileStatement(Expr* cond, Statement* body) : m_cond(cond), m_body(body) {}

	~WhileStatement()
	{
		delete m_cond;
		delete m_body;
	}

	SS_GETSET(Expr*, Condition, m_cond);
	SS_GETSET(Statement*, Body, m_body);
private:
	Expr* m_cond;
	Statement* m_body;
};

class DoWhileStatement : public Statement
{
public:
	SS_CLASS(DoWhileStatement, "DoWhileStatement", Statement);

	DoWhileStatement(Expr* cond, Statement* body) : m_cond(cond), m_body(body) {}

	~DoWhileStatement()
	{
		delete m_cond;
		delete m_body;
	}

	SS_GETSET(Expr*, Condition, m_cond);
	SS_GETSET(Statement*, Body, m_body);

private:
	Expr* m_cond;
	Statement* m_body;
};


class ForStatement : public Statement
{
public:
	SS_CLASS(ForStatement, "ForStatement", Statement);

	ForStatement(Statement* init, Expr* cond, Expr* iterate, Statement* body) : 
		m_initStmt(init), m_cond(cond), m_iterate(iterate), m_body(body) {}

	~ForStatement()
	{
		delete m_initStmt;
		delete m_cond;
		delete m_iterate;
		delete m_body;
	}

	SS_GETSET(Statement*, InitStmt, m_initStmt);
	SS_GETSET(Expr*, Condition, m_cond);
	SS_GETSET(Expr*, IterateExpr, m_cond);
	SS_GETSET(Statement*, Body, m_body);
private:
	Statement* m_initStmt;
	Expr* m_cond;
	Expr* m_iterate;
	Statement* m_body;
};

class ForEachStatement : public Statement
{
public:
	SS_CLASS(ForEachStatement, "ForEachStatement", Statement);

	ForEachStatement(TypeExpr* typeExpr, const String& varName, Expr* listExpr, Statement* body) : 
		m_varTypeExpr(typeExpr), m_varType(0), m_varName(varName), m_listExpr(listExpr), m_body(body)
	{
	}

	~ForEachStatement()
	{
		delete m_varTypeExpr;
		delete m_listExpr;
		delete m_body;
	}

	SS_GETSET(TypeExpr*, VarTypeExpr, m_varTypeExpr);
	SS_GETSET(Type*, VarType, m_varType);
	SS_GETSET(const String&, VarName, m_varName);
	SS_GETSET(Expr*, ListExpr, m_listExpr);
	SS_GETSET(Statement*, Body, m_body);
private:
	TypeExpr* m_varTypeExpr;
	Type* m_varType;
	String m_varName;
	Expr* m_listExpr;
	Statement* m_body;
};

// TODO: foreach

/*
Statements--
switch
try-catch
variable-def stmt
*/


}

#endif // SS_STATEMENT_H
