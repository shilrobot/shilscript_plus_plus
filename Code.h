#ifndef SS_CODE_H
#define SS_CODE_H

#include "Prereqs.h"
#include "Instr.h"
#include "Type.h"

namespace SS {

class Function;

struct CodeLabel
{
	int targetIdx;

	CodeLabel(int targetIdx) : targetIdx(targetIdx)
	{
	}
};

class Code
{
public:	
	Code( Function* func ) : m_func(func) 
	{
	}

	void Dump();

	// TODO: Destroy instrs & labels

	Instr* jmp(CodeLabel* label)
	{
		Instr* instr = new Instr(Op_jmp);
		instr->SetArgCount(1);
		instr->GetArg(0).SetLabel(label);
		m_instructions.push_back(instr);
		return instr;
	}

	Instr* jmp_true(CodeLabel* label)
	{
		Instr* instr = new Instr(Op_jmp_true);
		instr->SetArgCount(1);
		instr->GetArg(0).SetLabel(label);
		m_instructions.push_back(instr);
		return instr;
	}

	Instr* jmp_false(CodeLabel* label)
	{
		Instr* instr = new Instr(Op_jmp_false);
		instr->SetArgCount(1);
		instr->GetArg(0).SetLabel(label);
		m_instructions.push_back(instr);
		return instr;
	}

	Instr* ret() { return WriteSimple(Op_ret); }
	Instr* nop() { return WriteSimple(Op_nop); }

	Instr* add() { return WriteSimple(Op_add); }
	Instr* sub() { return WriteSimple(Op_sub); }
	Instr* mul() { return WriteSimple(Op_mul); }
	Instr* div() { return WriteSimple(Op_div); }
	Instr* mod() { return WriteSimple(Op_mod); }
	Instr* shl() { return WriteSimple(Op_shl); }
	Instr* shr() { return WriteSimple(Op_shr); }
	Instr* inv() { return WriteSimple(Op_inv); }
	Instr* and() { return WriteSimple(Op_and); }
	Instr* or() { return WriteSimple(Op_or); }
	Instr* xor() { return WriteSimple(Op_xor); }
	Instr* neg() { return WriteSimple(Op_neg); }
	Instr* pow() { return WriteSimple(Op_pow); }
	Instr* inc() { return WriteSimple(Op_inc); }
	Instr* dec() { return WriteSimple(Op_dec); }

	Instr* log_and() { return WriteSimple(Op_log_and); }
	Instr* log_or() { return WriteSimple(Op_log_or); }
	Instr* log_not() { return WriteSimple(Op_log_not); }

	Instr* eq() { return WriteSimple(Op_eq); }
	Instr* ne() { return WriteSimple(Op_ne); }
	Instr* le() { return WriteSimple(Op_le); }
	Instr* lt() { return WriteSimple(Op_lt); }
	Instr* ge() { return WriteSimple(Op_ge); }
	Instr* gt() { return WriteSimple(Op_gt); }

	// TODO: A lot of these will change
	Instr* convert(const Type* type)
	{
		Instr* instr = new Instr(Op_store_arg);
		instr->SetArgCount(1);
		instr->GetArg(0).SetType(type);
		m_instructions.push_back(instr);
		return instr;
	}

	Instr* load_imm_i4(i4 val)
	{
		Instr* instr = new Instr(Op_load_imm);
		instr->SetArgCount(2);
		instr->GetArg(0).SetType(SS_T_I4);
		instr->GetArg(1).SetI4(val);
		m_instructions.push_back(instr);
		return instr;
	}

	Instr* load_arg(int n)
	{
		Instr* instr = new Instr(Op_load_arg);
		instr->SetArgCount(1);
		instr->GetArg(0).SetI4(n);
		m_instructions.push_back(instr);
		return instr;
	}

	Instr* store_arg(int n)
	{
		Instr* instr = new Instr(Op_store_arg);
		instr->SetArgCount(1);
		instr->GetArg(0).SetI4(n);
		m_instructions.push_back(instr);
		return instr;
	}

	Instr* load_local(int n)
	{
		Instr* instr = new Instr(Op_load_local);
		instr->SetArgCount(1);
		instr->GetArg(0).SetI4(n);
		m_instructions.push_back(instr);
		return instr;
	}

	Instr* store_local(int n)
	{
		Instr* instr = new Instr(Op_store_local);
		instr->SetArgCount(1);
		instr->GetArg(0).SetI4(n);
		m_instructions.push_back(instr);
		return instr;
	}

	Instr* dup() { return WriteSimple(Op_dup); }
	Instr* dup2() { return WriteSimple(Op_dup2); }
	Instr* pop() { return WriteSimple(Op_pop); }
	Instr* swap() { return WriteSimple(Op_swap); }

	Instr* brk() { return WriteSimple(Op_break); }
	Instr* print() { return WriteSimple(Op_print); }

	Instr* load_true() { return WriteSimple(Op_load_true); }
	Instr* load_false() { return WriteSimple(Op_load_false); }

	// TODO: More...

	// returns new label index
	CodeLabel* CreateLabel()
	{
		CodeLabel* label = new CodeLabel(-1);
		m_labels.push_back(label);
		return label;
	}

	void PlaceLabel(CodeLabel* label)
	{
		label->targetIdx = (uint32)m_instructions.size();
	}

private:
	Instr* WriteSimple(uint8 opcode)
	{
		Instr* instr = new Instr(opcode);
		m_instructions.push_back(instr);
		return instr;
	}

	Function*				m_func;
	std::vector<Instr*>		m_instructions;
	std::vector<CodeLabel*>		m_labels;
};

}

#endif // SS_CODE_H
