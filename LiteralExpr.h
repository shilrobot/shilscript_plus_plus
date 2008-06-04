#ifndef SS_LITERALEXPR_H
#define SS_LITERALEXPR_H

#include "Expr.h"

namespace SS {

class SS_EXPORT LiteralExpr : public Expr
{
public:
	SS_CLASS(LiteralExpr, "LiteralExpr", Expr);
};

class SS_EXPORT NullLiteralExpr : public LiteralExpr
{
public:
	SS_CLASS(NullLiteralExpr, "NullLiteralExpr", LiteralExpr);
};

class SS_EXPORT BoolLiteralExpr : public LiteralExpr
{
public:
	SS_CLASS(BoolLiteralExpr, "BoolLiteralExpr", Expr);

	BoolLiteralExpr(bool value) : m_value(value)
	{
	}

	SS_GETSET(bool, Value, m_value);
private:
	bool m_value;
};

class SS_EXPORT IntLiteralExpr : public LiteralExpr
{
public:
	SS_CLASS(IntLiteralExpr, "IntLiteralExpr", Expr);

	IntLiteralExpr(i4 val) : m_value(val)
	{
	}

	SS_GETSET(i4, Value, m_value);
private:
	i4	m_value;
};

class SS_EXPORT FloatLiteralExpr : public LiteralExpr
{
public:
	SS_CLASS(FloatLiteralExpr, "FloatLiteralExpr", Expr);

	FloatLiteralExpr(const String& value) : m_value(value)
	{
	}

	SS_GETSET(String, Value, m_value);
private:
	String	m_value;
};

class SS_EXPORT CharLiteralExpr : public LiteralExpr
{
public:
	SS_CLASS(CharLiteralExpr, "CharLiteralExpr", Expr);

	CharLiteralExpr(const String& value) : m_value(value)
	{
	}

	SS_GETSET(String, Value, m_value);
private:
	String	m_value;
};

class SS_EXPORT StringLiteralExpr : public LiteralExpr
{
public:
	SS_CLASS(StringLiteralExpr, "StringLiteralExpr", Expr);

	StringLiteralExpr(const String& value) : m_value(value)
	{
	}

	SS_GETSET(String, Value, m_value);
private:
	String	m_value;
};

}

#endif // SS_LITERALEXPR_H
