#include "Prereqs.h"
#include "Instr.h"

namespace SS {

#define SS_INSTRNAME(_name) case Op_##_name: return #_name ;

const char* GetOpcodeName(Opcode opcode)
{
	switch(opcode)
	{		
		SS_INSTRNAME(nop)
		SS_INSTRNAME(jmp)
		SS_INSTRNAME(jmp_true)
		SS_INSTRNAME(jmp_false)
		SS_INSTRNAME(ret)
		SS_INSTRNAME(add)
		SS_INSTRNAME(sub)
		SS_INSTRNAME(mul)
		SS_INSTRNAME(div)
		SS_INSTRNAME(mod)
		SS_INSTRNAME(shl)
		SS_INSTRNAME(shr)
		SS_INSTRNAME(inv)
		SS_INSTRNAME(and)
		SS_INSTRNAME(or)
		SS_INSTRNAME(xor)
		SS_INSTRNAME(neg)
		SS_INSTRNAME(pow)
		SS_INSTRNAME(inc)
		SS_INSTRNAME(dec)
		SS_INSTRNAME(log_and)
		SS_INSTRNAME(log_or)
		SS_INSTRNAME(log_not)
		SS_INSTRNAME(eq)
		SS_INSTRNAME(ne)
		SS_INSTRNAME(le)
		SS_INSTRNAME(lt)
		SS_INSTRNAME(ge)
		SS_INSTRNAME(gt)
		SS_INSTRNAME(convert)
		SS_INSTRNAME(load_imm)
		SS_INSTRNAME(load_arg)
		SS_INSTRNAME(store_arg)
		SS_INSTRNAME(load_local)
		SS_INSTRNAME(store_local)
		SS_INSTRNAME(dup)
		SS_INSTRNAME(dup2)
		SS_INSTRNAME(pop)
		SS_INSTRNAME(swap)
		SS_INSTRNAME(break)
		SS_INSTRNAME(print)
		SS_INSTRNAME(load_true)
		SS_INSTRNAME(load_false)
	default:
		SS_UNREACHABLE;
		return "<Unknown Op>";
	}
}

}
