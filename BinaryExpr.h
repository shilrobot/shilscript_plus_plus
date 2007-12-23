#ifndef SS_BINARYEXPR_H
#define SS_BINARYEXPR_H

#include "Expr.h"

namespace SS {

enum BinaryOp
{
	BINOP_ADD,
	BINOP_SUB,
	BINOP_MUL,
	BINOP_DIV,
	BINOP_MOD,
	BINOP_POW,
	BINOP_BIT_AND,
	BINOP_BIT_OR,
	BINOP_BIT_XOR,
	BINOP_LOG_AND,
	BINOP_LOG_OR,
	BINOP_SHL,
	BINOP_SHR,
	BINOP_LT,
	BINOP_LE,
	BINOP_GT,
	BINOP_GE,
	BINOP_EQ,
	BINOP_NE,

	// Used for assignexpr with no extra op.
	BINOP_NONE
};

class SS_EXPORT BinaryExpr : public Expr
{
public:
	SS_CLASS(BinaryExpr, "BinaryExpr", Expr);

	BinaryExpr(BinaryOp op, Expr* left, Expr* right) : m_op(op), m_left(left), m_right(right) {}

	virtual ~BinaryExpr()
	{
		delete m_left;
		delete m_right;
	}

	SS_GETSET(BinaryOp, Op, m_op);
	SS_GETSET(Expr*, Left, m_left);
	SS_GETSET(Expr*, Right, m_right);

	String GetOpName() const
	{
		switch(m_op)
		{
		case BINOP_ADD: return "+";
		case BINOP_SUB: return "-";
		case BINOP_MUL: return "*";
		case BINOP_DIV: return "/";
		case BINOP_MOD: return "%";
		case BINOP_POW: return "**";
		case BINOP_BIT_AND: return "&";
		case BINOP_BIT_OR: return "|";
		case BINOP_BIT_XOR: return "^";
		case BINOP_LOG_AND: return "&&";
		case BINOP_LOG_OR: return "||";
		case BINOP_SHL: return "<<";
		case BINOP_SHR: return ">>";
		case BINOP_LT: return "<";
		case BINOP_LE: return "<=";
		case BINOP_GT: return ">";
		case BINOP_GE: return ">=";
		case BINOP_EQ: return "==";
		case BINOP_NE: return ">=";
		case BINOP_NONE: return "<NONE>";
		default:
			return "<INVALID BINARY OP>";
		}
	}

	// TODO: Other expr. stuff

private:
	BinaryOp m_op;
	Expr* m_left;
	Expr* m_right;
};

}

#endif // SS_BINARYEXPR_H
