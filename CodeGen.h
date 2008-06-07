#ifndef SS_CODEGEN_H
#define SS_CODEGEN_H

#include "Prereqs.h"

namespace SS {

class Function;
class Statement;
class Expr;
class Code;
struct CodeLabel;

struct CodeGenContext
{
	CodeLabel* breakLabel;
	CodeLabel* continueLabel;

	CodeGenContext() : breakLabel(0), continueLabel(0)
	{}
};

class CodeGen
{
public:
	CodeGen();

	void Generate(Function* func);
private:
	void EmitStatement(const Statement* stmt, const CodeGenContext& ctx);
	void EmitExpr(const Expr* expr, const CodeGenContext& ctx);

	Function* m_func;
	Code* m_code;
};

}

#endif // SS_CODEGEN_H
