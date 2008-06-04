#ifndef SS_INSTR_H
#define SS_INSTR_H

#include "Prereqs.h"

namespace SS {

struct CodeLabel;

enum Opcode
{
	Op_nop 			= 0x00,
	Op_jmp 			= 0x01,
	Op_jmp_true 	= 0x02,
	Op_jmp_false 	= 0x03,
	Op_ret 			= 0x04,
	Op_add 			= 0x05,
	Op_sub 			= 0x06,
	Op_mul 			= 0x07,
	Op_div 			= 0x08,
	Op_mod 			= 0x09,
	Op_shl 			= 0x0A,
	Op_shr 			= 0x0B,
	Op_inv 			= 0x0C,
	Op_and 			= 0x0D,
	Op_or 			= 0x0E,
	Op_xor 			= 0x0F,
	Op_neg 			= 0x10,
	Op_pow 			= 0x11,
	Op_inc 			= 0x12,
	Op_dec 			= 0x13,
	Op_log_and 		= 0x14,
	Op_log_or 		= 0x15,
	Op_log_not 		= 0x16,
	Op_eq 			= 0x17,
	Op_ne 			= 0x18,
	Op_le 			= 0x19,
	Op_lt 			= 0x1A,
	Op_ge 			= 0x1B,
	Op_gt 			= 0x1C,
	Op_convert 		= 0x1D,
	Op_load_imm 	= 0x1E,
	Op_load_arg 	= 0x1F,
	Op_store_arg 	= 0x20,
	Op_load_local 	= 0x21,
	Op_store_local 	= 0x22,
	Op_dup 			= 0x23,
	Op_dup2 		= 0x24,
	Op_pop 			= 0x25,
	Op_swap 		= 0x26,
	Op_break 		= 0x27,
	Op_print 		= 0x28,
	Op_load_true	= 0x29,
	Op_load_false	= 0x2A,
};

const char* GetOpcodeName(Opcode opcode);

enum InstrArgType
{
	IAT_UNSET,
	IAT_BOOL,
	IAT_U1,
	IAT_I1,
	IAT_U2,
	IAT_I2,
	IAT_U4,
	IAT_I4,
	IAT_U8,
	IAT_I8,
	IAT_F4,
	IAT_F8,
	IAT_TYPE,
	IAT_CLASS,
	IAT_FUNC,
	IAT_VAR,
	IAT_LABEL
};

#define SS_INSTRVALUE_GETSET(_typ, _name, _var, _checktype) \
	_typ Get##_name() const { SSAssert(m_type == (_checktype)); return (_var); } \
	void Set##_name(_typ value) { m_type = _checktype; _var = value; }

class Type;
class Class;
class Function;
class Variable;

struct InstrArg
{
	InstrArg( ) : m_type( IAT_UNSET)
	{
		m_u8 = 0;
	}

	InstrArgType m_type;

	// TODO: Fucking iX/sX mismatch
	SS_INSTRVALUE_GETSET(bool, Bool, m_bool, IAT_BOOL)
	SS_INSTRVALUE_GETSET(u1, U1, m_u1, IAT_U1)
	SS_INSTRVALUE_GETSET(i1, I1, m_i1, IAT_I1)
	SS_INSTRVALUE_GETSET(u2, U2, m_u2, IAT_U2)
	SS_INSTRVALUE_GETSET(i2, I2, m_i2, IAT_I2)
	SS_INSTRVALUE_GETSET(u4, U4, m_u4, IAT_U4)
	SS_INSTRVALUE_GETSET(i4, I4, m_i4, IAT_I4)
	SS_INSTRVALUE_GETSET(u8, U8, m_u8, IAT_U8)
	SS_INSTRVALUE_GETSET(i8, I8, m_i8, IAT_I8)
	SS_INSTRVALUE_GETSET(float, F4, m_f4, IAT_F4)
	SS_INSTRVALUE_GETSET(double, F8, m_f8, IAT_F8)
	SS_INSTRVALUE_GETSET(const Type*, Type, m_typeVal, IAT_TYPE)
	SS_INSTRVALUE_GETSET(const Class*, Class, m_class, IAT_CLASS)
	SS_INSTRVALUE_GETSET(const Function*, Function, m_func, IAT_FUNC)
	SS_INSTRVALUE_GETSET(const Variable*, Variable, m_var, IAT_VAR)
	SS_INSTRVALUE_GETSET(CodeLabel*, Label, m_label, IAT_LABEL)

private:
	union {
		bool m_bool;
		u1	m_u1;
		i1	m_i1;
		u2	m_u2;
		i2	m_i2;
		u4	m_u4;
		i4	m_i4;
		u8	m_u8;
		i8	m_i8;
		float m_f4;
		double m_f8;
		const Type* m_typeVal;
		const Class* m_class;
		const Function* m_func;
		const Variable* m_var;
		CodeLabel* m_label;
	};
};

struct Instr
{
	uint32	index;
	uint8	opcode;
	std::vector<InstrArg> args;

	InstrArg& GetArg(int i)
	{
		SSAssert(i >= 0);
		SSAssert(i < args.size());
		return args[i];
	}

	const InstrArg& GetArg(int i) const
	{
		SSAssert(i >= 0);
		SSAssert(i < args.size());
		return args[i];
	}

	void SetArgCount(int n)
	{
		args.clear();
		args.resize(n);
	}

	Instr() : opcode(Op_nop), index(0)
	{
	}

	Instr(uint8 opcode) : opcode(opcode)
	{
	}
};

}

#endif // SS_INSTR_H
