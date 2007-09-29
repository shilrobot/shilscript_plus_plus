#ifdef SS_GEN_SYMBOLS
OP_JUMP,
OP_JUMP_TRUE,
OP_JUMP_FALSE,
OP_RET,
OP_NOP,
OP_ADD_I,
OP_ADD_UI,
OP_ADD_Q,
OP_ADD_UQ,
OP_ADD_F,
OP_ADD_D,
OP_SUB_I,
OP_SUB_UI,
OP_SUB_Q,
OP_SUB_UQ,
OP_SUB_F,
OP_SUB_D,
OP_MUL_I,
OP_MUL_UI,
OP_MUL_Q,
OP_MUL_UQ,
OP_MUL_F,
OP_MUL_D,
OP_DIV_I,
OP_DIV_UI,
OP_DIV_Q,
OP_DIV_UQ,
OP_DIV_F,
OP_DIV_D,
OP_MOD_I,
OP_MOD_UI,
OP_MOD_Q,
OP_MOD_UQ,
OP_AND_I,
OP_AND_UI,
OP_AND_Q,
OP_AND_UQ,
OP_OR_I,
OP_OR_UI,
OP_OR_Q,
OP_OR_UQ,
OP_XOR_I,
OP_XOR_UI,
OP_XOR_Q,
OP_XOR_UQ,
OP_COMPL_I,
OP_COMPL_UI,
OP_COMPL_Q,
OP_COMPL_UQ,
OP_SHL_I,
OP_SHL_UI,
OP_SHL_Q,
OP_SHL_UQ,
OP_SHR_I,
OP_SHR_UI,
OP_SHR_Q,
OP_SHR_UQ,
OP_NEG_I,
OP_NEG_Q,
OP_NEG_F,
OP_NEG_D,
OP_LT_I,
OP_LT_UI,
OP_LT_Q,
OP_LT_UQ,
OP_LT_F,
OP_LT_D,
OP_LE_I,
OP_LE_UI,
OP_LE_Q,
OP_LE_UQ,
OP_LE_F,
OP_LE_D,
OP_GT_I,
OP_GT_UI,
OP_GT_Q,
OP_GT_UQ,
OP_GT_F,
OP_GT_D,
OP_GE_I,
OP_GE_UI,
OP_GE_Q,
OP_GE_UQ,
OP_GE_F,
OP_GE_D,
OP_EQ_I,
OP_EQ_UI,
OP_EQ_Q,
OP_EQ_UQ,
OP_EQ_F,
OP_EQ_D,
OP_NE_I,
OP_NE_UI,
OP_NE_Q,
OP_NE_UQ,
OP_NE_F,
OP_NE_D,
OP_LOAD_CONST_I,
OP_LOAD_CONST_UI,
OP_LOAD_CONST_Q,
OP_LOAD_CONST_UQ,
OP_LOAD_CONST_F,
OP_LOAD_CONST_D,
OP_POP_I,
OP_POP_UI,
OP_POP_Q,
OP_POP_UQ,
OP_POP_F,
OP_POP_D,
OP_PRINT_I,
#endif

#ifdef SS_GEN_STRUCTS

struct Instr_jump
{
	uint32 opcode;
	int32 target;
};

struct Instr_jump_true
{
	uint32 opcode;
	int32 target;
};

struct Instr_jump_false
{
	uint32 opcode;
	int32 target;
};

struct Instr_ret
{
	uint32 opcode;
};

struct Instr_nop
{
	uint32 opcode;
};

struct Instr_add_i
{
	uint32 opcode;
};

struct Instr_add_ui
{
	uint32 opcode;
};

struct Instr_add_q
{
	uint32 opcode;
};

struct Instr_add_uq
{
	uint32 opcode;
};

struct Instr_add_f
{
	uint32 opcode;
};

struct Instr_add_d
{
	uint32 opcode;
};

struct Instr_sub_i
{
	uint32 opcode;
};

struct Instr_sub_ui
{
	uint32 opcode;
};

struct Instr_sub_q
{
	uint32 opcode;
};

struct Instr_sub_uq
{
	uint32 opcode;
};

struct Instr_sub_f
{
	uint32 opcode;
};

struct Instr_sub_d
{
	uint32 opcode;
};

struct Instr_mul_i
{
	uint32 opcode;
};

struct Instr_mul_ui
{
	uint32 opcode;
};

struct Instr_mul_q
{
	uint32 opcode;
};

struct Instr_mul_uq
{
	uint32 opcode;
};

struct Instr_mul_f
{
	uint32 opcode;
};

struct Instr_mul_d
{
	uint32 opcode;
};

struct Instr_div_i
{
	uint32 opcode;
};

struct Instr_div_ui
{
	uint32 opcode;
};

struct Instr_div_q
{
	uint32 opcode;
};

struct Instr_div_uq
{
	uint32 opcode;
};

struct Instr_div_f
{
	uint32 opcode;
};

struct Instr_div_d
{
	uint32 opcode;
};

struct Instr_mod_i
{
	uint32 opcode;
};

struct Instr_mod_ui
{
	uint32 opcode;
};

struct Instr_mod_q
{
	uint32 opcode;
};

struct Instr_mod_uq
{
	uint32 opcode;
};

struct Instr_and_i
{
	uint32 opcode;
};

struct Instr_and_ui
{
	uint32 opcode;
};

struct Instr_and_q
{
	uint32 opcode;
};

struct Instr_and_uq
{
	uint32 opcode;
};

struct Instr_or_i
{
	uint32 opcode;
};

struct Instr_or_ui
{
	uint32 opcode;
};

struct Instr_or_q
{
	uint32 opcode;
};

struct Instr_or_uq
{
	uint32 opcode;
};

struct Instr_xor_i
{
	uint32 opcode;
};

struct Instr_xor_ui
{
	uint32 opcode;
};

struct Instr_xor_q
{
	uint32 opcode;
};

struct Instr_xor_uq
{
	uint32 opcode;
};

struct Instr_compl_i
{
	uint32 opcode;
};

struct Instr_compl_ui
{
	uint32 opcode;
};

struct Instr_compl_q
{
	uint32 opcode;
};

struct Instr_compl_uq
{
	uint32 opcode;
};

struct Instr_shl_i
{
	uint32 opcode;
};

struct Instr_shl_ui
{
	uint32 opcode;
};

struct Instr_shl_q
{
	uint32 opcode;
};

struct Instr_shl_uq
{
	uint32 opcode;
};

struct Instr_shr_i
{
	uint32 opcode;
};

struct Instr_shr_ui
{
	uint32 opcode;
};

struct Instr_shr_q
{
	uint32 opcode;
};

struct Instr_shr_uq
{
	uint32 opcode;
};

struct Instr_neg_i
{
	uint32 opcode;
};

struct Instr_neg_q
{
	uint32 opcode;
};

struct Instr_neg_f
{
	uint32 opcode;
};

struct Instr_neg_d
{
	uint32 opcode;
};

struct Instr_lt_i
{
	uint32 opcode;
};

struct Instr_lt_ui
{
	uint32 opcode;
};

struct Instr_lt_q
{
	uint32 opcode;
};

struct Instr_lt_uq
{
	uint32 opcode;
};

struct Instr_lt_f
{
	uint32 opcode;
};

struct Instr_lt_d
{
	uint32 opcode;
};

struct Instr_le_i
{
	uint32 opcode;
};

struct Instr_le_ui
{
	uint32 opcode;
};

struct Instr_le_q
{
	uint32 opcode;
};

struct Instr_le_uq
{
	uint32 opcode;
};

struct Instr_le_f
{
	uint32 opcode;
};

struct Instr_le_d
{
	uint32 opcode;
};

struct Instr_gt_i
{
	uint32 opcode;
};

struct Instr_gt_ui
{
	uint32 opcode;
};

struct Instr_gt_q
{
	uint32 opcode;
};

struct Instr_gt_uq
{
	uint32 opcode;
};

struct Instr_gt_f
{
	uint32 opcode;
};

struct Instr_gt_d
{
	uint32 opcode;
};

struct Instr_ge_i
{
	uint32 opcode;
};

struct Instr_ge_ui
{
	uint32 opcode;
};

struct Instr_ge_q
{
	uint32 opcode;
};

struct Instr_ge_uq
{
	uint32 opcode;
};

struct Instr_ge_f
{
	uint32 opcode;
};

struct Instr_ge_d
{
	uint32 opcode;
};

struct Instr_eq_i
{
	uint32 opcode;
};

struct Instr_eq_ui
{
	uint32 opcode;
};

struct Instr_eq_q
{
	uint32 opcode;
};

struct Instr_eq_uq
{
	uint32 opcode;
};

struct Instr_eq_f
{
	uint32 opcode;
};

struct Instr_eq_d
{
	uint32 opcode;
};

struct Instr_ne_i
{
	uint32 opcode;
};

struct Instr_ne_ui
{
	uint32 opcode;
};

struct Instr_ne_q
{
	uint32 opcode;
};

struct Instr_ne_uq
{
	uint32 opcode;
};

struct Instr_ne_f
{
	uint32 opcode;
};

struct Instr_ne_d
{
	uint32 opcode;
};

struct Instr_load_const_i
{
	uint32 opcode;
	int32 value;
};

struct Instr_load_const_ui
{
	uint32 opcode;
	uint32 value;
};

struct Instr_load_const_q
{
	uint32 opcode;
	int64 value;
};

struct Instr_load_const_uq
{
	uint32 opcode;
	uint64 value;
};

struct Instr_load_const_f
{
	uint32 opcode;
	float32 value;
};

struct Instr_load_const_d
{
	uint32 opcode;
	float64 value;
};

struct Instr_pop_i
{
	uint32 opcode;
};

struct Instr_pop_ui
{
	uint32 opcode;
};

struct Instr_pop_q
{
	uint32 opcode;
};

struct Instr_pop_uq
{
	uint32 opcode;
};

struct Instr_pop_f
{
	uint32 opcode;
};

struct Instr_pop_d
{
	uint32 opcode;
};

struct Instr_print_i
{
	uint32 opcode;
};

#endif

#ifdef SS_GEN_ASSEMBLE

void jump(int32 _target)
{
	Instr_jump instr;
	instr.opcode = OP_JUMP;
	instr.target = _target;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void jump_true(int32 _target)
{
	Instr_jump_true instr;
	instr.opcode = OP_JUMP_TRUE;
	instr.target = _target;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void jump_false(int32 _target)
{
	Instr_jump_false instr;
	instr.opcode = OP_JUMP_FALSE;
	instr.target = _target;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ret()
{
	Instr_ret instr;
	instr.opcode = OP_RET;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void nop()
{
	Instr_nop instr;
	instr.opcode = OP_NOP;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void add_i()
{
	Instr_add_i instr;
	instr.opcode = OP_ADD_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void add_ui()
{
	Instr_add_ui instr;
	instr.opcode = OP_ADD_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void add_q()
{
	Instr_add_q instr;
	instr.opcode = OP_ADD_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void add_uq()
{
	Instr_add_uq instr;
	instr.opcode = OP_ADD_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void add_f()
{
	Instr_add_f instr;
	instr.opcode = OP_ADD_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void add_d()
{
	Instr_add_d instr;
	instr.opcode = OP_ADD_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void sub_i()
{
	Instr_sub_i instr;
	instr.opcode = OP_SUB_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void sub_ui()
{
	Instr_sub_ui instr;
	instr.opcode = OP_SUB_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void sub_q()
{
	Instr_sub_q instr;
	instr.opcode = OP_SUB_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void sub_uq()
{
	Instr_sub_uq instr;
	instr.opcode = OP_SUB_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void sub_f()
{
	Instr_sub_f instr;
	instr.opcode = OP_SUB_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void sub_d()
{
	Instr_sub_d instr;
	instr.opcode = OP_SUB_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void mul_i()
{
	Instr_mul_i instr;
	instr.opcode = OP_MUL_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void mul_ui()
{
	Instr_mul_ui instr;
	instr.opcode = OP_MUL_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void mul_q()
{
	Instr_mul_q instr;
	instr.opcode = OP_MUL_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void mul_uq()
{
	Instr_mul_uq instr;
	instr.opcode = OP_MUL_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void mul_f()
{
	Instr_mul_f instr;
	instr.opcode = OP_MUL_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void mul_d()
{
	Instr_mul_d instr;
	instr.opcode = OP_MUL_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void div_i()
{
	Instr_div_i instr;
	instr.opcode = OP_DIV_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void div_ui()
{
	Instr_div_ui instr;
	instr.opcode = OP_DIV_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void div_q()
{
	Instr_div_q instr;
	instr.opcode = OP_DIV_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void div_uq()
{
	Instr_div_uq instr;
	instr.opcode = OP_DIV_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void div_f()
{
	Instr_div_f instr;
	instr.opcode = OP_DIV_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void div_d()
{
	Instr_div_d instr;
	instr.opcode = OP_DIV_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void mod_i()
{
	Instr_mod_i instr;
	instr.opcode = OP_MOD_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void mod_ui()
{
	Instr_mod_ui instr;
	instr.opcode = OP_MOD_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void mod_q()
{
	Instr_mod_q instr;
	instr.opcode = OP_MOD_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void mod_uq()
{
	Instr_mod_uq instr;
	instr.opcode = OP_MOD_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void and_i()
{
	Instr_and_i instr;
	instr.opcode = OP_AND_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void and_ui()
{
	Instr_and_ui instr;
	instr.opcode = OP_AND_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void and_q()
{
	Instr_and_q instr;
	instr.opcode = OP_AND_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void and_uq()
{
	Instr_and_uq instr;
	instr.opcode = OP_AND_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void or_i()
{
	Instr_or_i instr;
	instr.opcode = OP_OR_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void or_ui()
{
	Instr_or_ui instr;
	instr.opcode = OP_OR_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void or_q()
{
	Instr_or_q instr;
	instr.opcode = OP_OR_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void or_uq()
{
	Instr_or_uq instr;
	instr.opcode = OP_OR_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void xor_i()
{
	Instr_xor_i instr;
	instr.opcode = OP_XOR_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void xor_ui()
{
	Instr_xor_ui instr;
	instr.opcode = OP_XOR_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void xor_q()
{
	Instr_xor_q instr;
	instr.opcode = OP_XOR_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void xor_uq()
{
	Instr_xor_uq instr;
	instr.opcode = OP_XOR_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void compl_i()
{
	Instr_compl_i instr;
	instr.opcode = OP_COMPL_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void compl_ui()
{
	Instr_compl_ui instr;
	instr.opcode = OP_COMPL_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void compl_q()
{
	Instr_compl_q instr;
	instr.opcode = OP_COMPL_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void compl_uq()
{
	Instr_compl_uq instr;
	instr.opcode = OP_COMPL_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void shl_i()
{
	Instr_shl_i instr;
	instr.opcode = OP_SHL_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void shl_ui()
{
	Instr_shl_ui instr;
	instr.opcode = OP_SHL_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void shl_q()
{
	Instr_shl_q instr;
	instr.opcode = OP_SHL_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void shl_uq()
{
	Instr_shl_uq instr;
	instr.opcode = OP_SHL_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void shr_i()
{
	Instr_shr_i instr;
	instr.opcode = OP_SHR_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void shr_ui()
{
	Instr_shr_ui instr;
	instr.opcode = OP_SHR_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void shr_q()
{
	Instr_shr_q instr;
	instr.opcode = OP_SHR_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void shr_uq()
{
	Instr_shr_uq instr;
	instr.opcode = OP_SHR_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void neg_i()
{
	Instr_neg_i instr;
	instr.opcode = OP_NEG_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void neg_q()
{
	Instr_neg_q instr;
	instr.opcode = OP_NEG_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void neg_f()
{
	Instr_neg_f instr;
	instr.opcode = OP_NEG_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void neg_d()
{
	Instr_neg_d instr;
	instr.opcode = OP_NEG_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void lt_i()
{
	Instr_lt_i instr;
	instr.opcode = OP_LT_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void lt_ui()
{
	Instr_lt_ui instr;
	instr.opcode = OP_LT_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void lt_q()
{
	Instr_lt_q instr;
	instr.opcode = OP_LT_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void lt_uq()
{
	Instr_lt_uq instr;
	instr.opcode = OP_LT_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void lt_f()
{
	Instr_lt_f instr;
	instr.opcode = OP_LT_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void lt_d()
{
	Instr_lt_d instr;
	instr.opcode = OP_LT_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void le_i()
{
	Instr_le_i instr;
	instr.opcode = OP_LE_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void le_ui()
{
	Instr_le_ui instr;
	instr.opcode = OP_LE_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void le_q()
{
	Instr_le_q instr;
	instr.opcode = OP_LE_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void le_uq()
{
	Instr_le_uq instr;
	instr.opcode = OP_LE_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void le_f()
{
	Instr_le_f instr;
	instr.opcode = OP_LE_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void le_d()
{
	Instr_le_d instr;
	instr.opcode = OP_LE_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void gt_i()
{
	Instr_gt_i instr;
	instr.opcode = OP_GT_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void gt_ui()
{
	Instr_gt_ui instr;
	instr.opcode = OP_GT_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void gt_q()
{
	Instr_gt_q instr;
	instr.opcode = OP_GT_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void gt_uq()
{
	Instr_gt_uq instr;
	instr.opcode = OP_GT_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void gt_f()
{
	Instr_gt_f instr;
	instr.opcode = OP_GT_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void gt_d()
{
	Instr_gt_d instr;
	instr.opcode = OP_GT_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ge_i()
{
	Instr_ge_i instr;
	instr.opcode = OP_GE_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ge_ui()
{
	Instr_ge_ui instr;
	instr.opcode = OP_GE_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ge_q()
{
	Instr_ge_q instr;
	instr.opcode = OP_GE_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ge_uq()
{
	Instr_ge_uq instr;
	instr.opcode = OP_GE_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ge_f()
{
	Instr_ge_f instr;
	instr.opcode = OP_GE_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ge_d()
{
	Instr_ge_d instr;
	instr.opcode = OP_GE_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void eq_i()
{
	Instr_eq_i instr;
	instr.opcode = OP_EQ_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void eq_ui()
{
	Instr_eq_ui instr;
	instr.opcode = OP_EQ_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void eq_q()
{
	Instr_eq_q instr;
	instr.opcode = OP_EQ_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void eq_uq()
{
	Instr_eq_uq instr;
	instr.opcode = OP_EQ_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void eq_f()
{
	Instr_eq_f instr;
	instr.opcode = OP_EQ_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void eq_d()
{
	Instr_eq_d instr;
	instr.opcode = OP_EQ_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ne_i()
{
	Instr_ne_i instr;
	instr.opcode = OP_NE_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ne_ui()
{
	Instr_ne_ui instr;
	instr.opcode = OP_NE_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ne_q()
{
	Instr_ne_q instr;
	instr.opcode = OP_NE_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ne_uq()
{
	Instr_ne_uq instr;
	instr.opcode = OP_NE_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ne_f()
{
	Instr_ne_f instr;
	instr.opcode = OP_NE_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void ne_d()
{
	Instr_ne_d instr;
	instr.opcode = OP_NE_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void load_const_i(int32 _value)
{
	Instr_load_const_i instr;
	instr.opcode = OP_LOAD_CONST_I;
	instr.value = _value;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void load_const_ui(uint32 _value)
{
	Instr_load_const_ui instr;
	instr.opcode = OP_LOAD_CONST_UI;
	instr.value = _value;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void load_const_q(int64 _value)
{
	Instr_load_const_q instr;
	instr.opcode = OP_LOAD_CONST_Q;
	instr.value = _value;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void load_const_uq(uint64 _value)
{
	Instr_load_const_uq instr;
	instr.opcode = OP_LOAD_CONST_UQ;
	instr.value = _value;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void load_const_f(float32 _value)
{
	Instr_load_const_f instr;
	instr.opcode = OP_LOAD_CONST_F;
	instr.value = _value;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void load_const_d(float64 _value)
{
	Instr_load_const_d instr;
	instr.opcode = OP_LOAD_CONST_D;
	instr.value = _value;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void pop_i()
{
	Instr_pop_i instr;
	instr.opcode = OP_POP_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void pop_ui()
{
	Instr_pop_ui instr;
	instr.opcode = OP_POP_UI;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void pop_q()
{
	Instr_pop_q instr;
	instr.opcode = OP_POP_Q;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void pop_uq()
{
	Instr_pop_uq instr;
	instr.opcode = OP_POP_UQ;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void pop_f()
{
	Instr_pop_f instr;
	instr.opcode = OP_POP_F;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void pop_d()
{
	Instr_pop_d instr;
	instr.opcode = OP_POP_D;
	AddInstr((Instr*)&instr, sizeof(instr));
}

void print_i()
{
	Instr_print_i instr;
	instr.opcode = OP_PRINT_I;
	AddInstr((Instr*)&instr, sizeof(instr));
}

#endif

#ifdef SS_GEN_DISASSEMBLE

case OP_JUMP:
{
	printf("jump");
	printf(" ");
	printData( ((Instr_jump*)instr)->target );
	break;
}

case OP_JUMP_TRUE:
{
	printf("jump_true");
	printf(" ");
	printData( ((Instr_jump_true*)instr)->target );
	break;
}

case OP_JUMP_FALSE:
{
	printf("jump_false");
	printf(" ");
	printData( ((Instr_jump_false*)instr)->target );
	break;
}

case OP_RET:
{
	printf("ret");
	break;
}

case OP_NOP:
{
	printf("nop");
	break;
}

case OP_ADD_I:
{
	printf("add_i");
	break;
}

case OP_ADD_UI:
{
	printf("add_ui");
	break;
}

case OP_ADD_Q:
{
	printf("add_q");
	break;
}

case OP_ADD_UQ:
{
	printf("add_uq");
	break;
}

case OP_ADD_F:
{
	printf("add_f");
	break;
}

case OP_ADD_D:
{
	printf("add_d");
	break;
}

case OP_SUB_I:
{
	printf("sub_i");
	break;
}

case OP_SUB_UI:
{
	printf("sub_ui");
	break;
}

case OP_SUB_Q:
{
	printf("sub_q");
	break;
}

case OP_SUB_UQ:
{
	printf("sub_uq");
	break;
}

case OP_SUB_F:
{
	printf("sub_f");
	break;
}

case OP_SUB_D:
{
	printf("sub_d");
	break;
}

case OP_MUL_I:
{
	printf("mul_i");
	break;
}

case OP_MUL_UI:
{
	printf("mul_ui");
	break;
}

case OP_MUL_Q:
{
	printf("mul_q");
	break;
}

case OP_MUL_UQ:
{
	printf("mul_uq");
	break;
}

case OP_MUL_F:
{
	printf("mul_f");
	break;
}

case OP_MUL_D:
{
	printf("mul_d");
	break;
}

case OP_DIV_I:
{
	printf("div_i");
	break;
}

case OP_DIV_UI:
{
	printf("div_ui");
	break;
}

case OP_DIV_Q:
{
	printf("div_q");
	break;
}

case OP_DIV_UQ:
{
	printf("div_uq");
	break;
}

case OP_DIV_F:
{
	printf("div_f");
	break;
}

case OP_DIV_D:
{
	printf("div_d");
	break;
}

case OP_MOD_I:
{
	printf("mod_i");
	break;
}

case OP_MOD_UI:
{
	printf("mod_ui");
	break;
}

case OP_MOD_Q:
{
	printf("mod_q");
	break;
}

case OP_MOD_UQ:
{
	printf("mod_uq");
	break;
}

case OP_AND_I:
{
	printf("and_i");
	break;
}

case OP_AND_UI:
{
	printf("and_ui");
	break;
}

case OP_AND_Q:
{
	printf("and_q");
	break;
}

case OP_AND_UQ:
{
	printf("and_uq");
	break;
}

case OP_OR_I:
{
	printf("or_i");
	break;
}

case OP_OR_UI:
{
	printf("or_ui");
	break;
}

case OP_OR_Q:
{
	printf("or_q");
	break;
}

case OP_OR_UQ:
{
	printf("or_uq");
	break;
}

case OP_XOR_I:
{
	printf("xor_i");
	break;
}

case OP_XOR_UI:
{
	printf("xor_ui");
	break;
}

case OP_XOR_Q:
{
	printf("xor_q");
	break;
}

case OP_XOR_UQ:
{
	printf("xor_uq");
	break;
}

case OP_COMPL_I:
{
	printf("compl_i");
	break;
}

case OP_COMPL_UI:
{
	printf("compl_ui");
	break;
}

case OP_COMPL_Q:
{
	printf("compl_q");
	break;
}

case OP_COMPL_UQ:
{
	printf("compl_uq");
	break;
}

case OP_SHL_I:
{
	printf("shl_i");
	break;
}

case OP_SHL_UI:
{
	printf("shl_ui");
	break;
}

case OP_SHL_Q:
{
	printf("shl_q");
	break;
}

case OP_SHL_UQ:
{
	printf("shl_uq");
	break;
}

case OP_SHR_I:
{
	printf("shr_i");
	break;
}

case OP_SHR_UI:
{
	printf("shr_ui");
	break;
}

case OP_SHR_Q:
{
	printf("shr_q");
	break;
}

case OP_SHR_UQ:
{
	printf("shr_uq");
	break;
}

case OP_NEG_I:
{
	printf("neg_i");
	break;
}

case OP_NEG_Q:
{
	printf("neg_q");
	break;
}

case OP_NEG_F:
{
	printf("neg_f");
	break;
}

case OP_NEG_D:
{
	printf("neg_d");
	break;
}

case OP_LT_I:
{
	printf("lt_i");
	break;
}

case OP_LT_UI:
{
	printf("lt_ui");
	break;
}

case OP_LT_Q:
{
	printf("lt_q");
	break;
}

case OP_LT_UQ:
{
	printf("lt_uq");
	break;
}

case OP_LT_F:
{
	printf("lt_f");
	break;
}

case OP_LT_D:
{
	printf("lt_d");
	break;
}

case OP_LE_I:
{
	printf("le_i");
	break;
}

case OP_LE_UI:
{
	printf("le_ui");
	break;
}

case OP_LE_Q:
{
	printf("le_q");
	break;
}

case OP_LE_UQ:
{
	printf("le_uq");
	break;
}

case OP_LE_F:
{
	printf("le_f");
	break;
}

case OP_LE_D:
{
	printf("le_d");
	break;
}

case OP_GT_I:
{
	printf("gt_i");
	break;
}

case OP_GT_UI:
{
	printf("gt_ui");
	break;
}

case OP_GT_Q:
{
	printf("gt_q");
	break;
}

case OP_GT_UQ:
{
	printf("gt_uq");
	break;
}

case OP_GT_F:
{
	printf("gt_f");
	break;
}

case OP_GT_D:
{
	printf("gt_d");
	break;
}

case OP_GE_I:
{
	printf("ge_i");
	break;
}

case OP_GE_UI:
{
	printf("ge_ui");
	break;
}

case OP_GE_Q:
{
	printf("ge_q");
	break;
}

case OP_GE_UQ:
{
	printf("ge_uq");
	break;
}

case OP_GE_F:
{
	printf("ge_f");
	break;
}

case OP_GE_D:
{
	printf("ge_d");
	break;
}

case OP_EQ_I:
{
	printf("eq_i");
	break;
}

case OP_EQ_UI:
{
	printf("eq_ui");
	break;
}

case OP_EQ_Q:
{
	printf("eq_q");
	break;
}

case OP_EQ_UQ:
{
	printf("eq_uq");
	break;
}

case OP_EQ_F:
{
	printf("eq_f");
	break;
}

case OP_EQ_D:
{
	printf("eq_d");
	break;
}

case OP_NE_I:
{
	printf("ne_i");
	break;
}

case OP_NE_UI:
{
	printf("ne_ui");
	break;
}

case OP_NE_Q:
{
	printf("ne_q");
	break;
}

case OP_NE_UQ:
{
	printf("ne_uq");
	break;
}

case OP_NE_F:
{
	printf("ne_f");
	break;
}

case OP_NE_D:
{
	printf("ne_d");
	break;
}

case OP_LOAD_CONST_I:
{
	printf("load_const_i");
	printf(" ");
	printData( ((Instr_load_const_i*)instr)->value );
	break;
}

case OP_LOAD_CONST_UI:
{
	printf("load_const_ui");
	printf(" ");
	printData( ((Instr_load_const_ui*)instr)->value );
	break;
}

case OP_LOAD_CONST_Q:
{
	printf("load_const_q");
	printf(" ");
	printData( ((Instr_load_const_q*)instr)->value );
	break;
}

case OP_LOAD_CONST_UQ:
{
	printf("load_const_uq");
	printf(" ");
	printData( ((Instr_load_const_uq*)instr)->value );
	break;
}

case OP_LOAD_CONST_F:
{
	printf("load_const_f");
	printf(" ");
	printData( ((Instr_load_const_f*)instr)->value );
	break;
}

case OP_LOAD_CONST_D:
{
	printf("load_const_d");
	printf(" ");
	printData( ((Instr_load_const_d*)instr)->value );
	break;
}

case OP_POP_I:
{
	printf("pop_i");
	break;
}

case OP_POP_UI:
{
	printf("pop_ui");
	break;
}

case OP_POP_Q:
{
	printf("pop_q");
	break;
}

case OP_POP_UQ:
{
	printf("pop_uq");
	break;
}

case OP_POP_F:
{
	printf("pop_f");
	break;
}

case OP_POP_D:
{
	printf("pop_d");
	break;
}

case OP_PRINT_I:
{
	printf("print_i");
	break;
}

#endif

#ifdef SS_GEN_INTERPRETER

case OP_JUMP:
{
	int32& target = ((Instr_jump*)instr)->target;
	SET_PC(target);
	break;
}

case OP_JUMP_TRUE:
{
	int32& target = ((Instr_jump_true*)instr)->target;
	int32 cond;
	POP(int32, cond);
	
				if(cond != 0) {
					SET_PC(target);
				} else {
					INC_PC;
				}
				
	break;
}

case OP_JUMP_FALSE:
{
	int32& target = ((Instr_jump_false*)instr)->target;
	int32 cond;
	POP(int32, cond);
	
				if(cond == 0) {
					SET_PC(target);
				} else {
					INC_PC;
				}
				
	break;
}

case OP_RET:
{
	/*TODO*/
return;
	INC_PC;
	break;
}

case OP_NOP:
{
	
	INC_PC;
	break;
}

case OP_ADD_I:
{
	int32 a;
	int32 b;
	int32 out;
	POP(int32, a);
	POP(int32, b);
	out = a+b;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_ADD_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = a+b;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_ADD_Q:
{
	int64 a;
	int64 b;
	int64 out;
	POP(int64, a);
	POP(int64, b);
	out = a+b;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_ADD_UQ:
{
	uint64 a;
	uint64 b;
	uint64 out;
	POP(uint64, a);
	POP(uint64, b);
	out = a+b;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_ADD_F:
{
	float32 a;
	float32 b;
	float32 out;
	POP(float32, a);
	POP(float32, b);
	out = a+b;
	PUSH(float32, out);
	INC_PC;
	break;
}

case OP_ADD_D:
{
	float64 a;
	float64 b;
	float64 out;
	POP(float64, a);
	POP(float64, b);
	out = a+b;
	PUSH(float64, out);
	INC_PC;
	break;
}

case OP_SUB_I:
{
	int32 a;
	int32 b;
	int32 out;
	POP(int32, a);
	POP(int32, b);
	out = a-b;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_SUB_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = a-b;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_SUB_Q:
{
	int64 a;
	int64 b;
	int64 out;
	POP(int64, a);
	POP(int64, b);
	out = a-b;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_SUB_UQ:
{
	uint64 a;
	uint64 b;
	uint64 out;
	POP(uint64, a);
	POP(uint64, b);
	out = a-b;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_SUB_F:
{
	float32 a;
	float32 b;
	float32 out;
	POP(float32, a);
	POP(float32, b);
	out = a-b;
	PUSH(float32, out);
	INC_PC;
	break;
}

case OP_SUB_D:
{
	float64 a;
	float64 b;
	float64 out;
	POP(float64, a);
	POP(float64, b);
	out = a-b;
	PUSH(float64, out);
	INC_PC;
	break;
}

case OP_MUL_I:
{
	int32 a;
	int32 b;
	int32 out;
	POP(int32, a);
	POP(int32, b);
	out = a*b;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_MUL_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = a*b;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_MUL_Q:
{
	int64 a;
	int64 b;
	int64 out;
	POP(int64, a);
	POP(int64, b);
	out = a*b;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_MUL_UQ:
{
	uint64 a;
	uint64 b;
	uint64 out;
	POP(uint64, a);
	POP(uint64, b);
	out = a*b;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_MUL_F:
{
	float32 a;
	float32 b;
	float32 out;
	POP(float32, a);
	POP(float32, b);
	out = a*b;
	PUSH(float32, out);
	INC_PC;
	break;
}

case OP_MUL_D:
{
	float64 a;
	float64 b;
	float64 out;
	POP(float64, a);
	POP(float64, b);
	out = a*b;
	PUSH(float64, out);
	INC_PC;
	break;
}

case OP_DIV_I:
{
	int32 a;
	int32 b;
	int32 out;
	POP(int32, a);
	POP(int32, b);
	out = a/b;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_DIV_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = a/b;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_DIV_Q:
{
	int64 a;
	int64 b;
	int64 out;
	POP(int64, a);
	POP(int64, b);
	out = a/b;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_DIV_UQ:
{
	uint64 a;
	uint64 b;
	uint64 out;
	POP(uint64, a);
	POP(uint64, b);
	out = a/b;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_DIV_F:
{
	float32 a;
	float32 b;
	float32 out;
	POP(float32, a);
	POP(float32, b);
	out = a/b;
	PUSH(float32, out);
	INC_PC;
	break;
}

case OP_DIV_D:
{
	float64 a;
	float64 b;
	float64 out;
	POP(float64, a);
	POP(float64, b);
	out = a/b;
	PUSH(float64, out);
	INC_PC;
	break;
}

case OP_MOD_I:
{
	int32 a;
	int32 b;
	int32 out;
	POP(int32, a);
	POP(int32, b);
	out = a%b;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_MOD_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = a%b;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_MOD_Q:
{
	int64 a;
	int64 b;
	int64 out;
	POP(int64, a);
	POP(int64, b);
	out = a%b;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_MOD_UQ:
{
	uint64 a;
	uint64 b;
	uint64 out;
	POP(uint64, a);
	POP(uint64, b);
	out = a%b;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_AND_I:
{
	int32 a;
	int32 b;
	int32 out;
	POP(int32, a);
	POP(int32, b);
	out = a & b;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_AND_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = a & b;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_AND_Q:
{
	int64 a;
	int64 b;
	int64 out;
	POP(int64, a);
	POP(int64, b);
	out = a & b;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_AND_UQ:
{
	uint64 a;
	uint64 b;
	uint64 out;
	POP(uint64, a);
	POP(uint64, b);
	out = a & b;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_OR_I:
{
	int32 a;
	int32 b;
	int32 out;
	POP(int32, a);
	POP(int32, b);
	out = a | b;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_OR_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = a | b;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_OR_Q:
{
	int64 a;
	int64 b;
	int64 out;
	POP(int64, a);
	POP(int64, b);
	out = a | b;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_OR_UQ:
{
	uint64 a;
	uint64 b;
	uint64 out;
	POP(uint64, a);
	POP(uint64, b);
	out = a | b;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_XOR_I:
{
	int32 a;
	int32 b;
	int32 out;
	POP(int32, a);
	POP(int32, b);
	out = a ^ b;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_XOR_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = a ^ b;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_XOR_Q:
{
	int64 a;
	int64 b;
	int64 out;
	POP(int64, a);
	POP(int64, b);
	out = a ^ b;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_XOR_UQ:
{
	uint64 a;
	uint64 b;
	uint64 out;
	POP(uint64, a);
	POP(uint64, b);
	out = a ^ b;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_COMPL_I:
{
	int32 a;
	int32 out;
	POP(int32, a);
	out = ~a;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_COMPL_UI:
{
	uint32 a;
	uint32 out;
	POP(uint32, a);
	out = ~a;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_COMPL_Q:
{
	int64 a;
	int64 out;
	POP(int64, a);
	out = ~a;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_COMPL_UQ:
{
	uint64 a;
	uint64 out;
	POP(uint64, a);
	out = ~a;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_SHL_I:
{
	int32 a;
	uint32 b;
	int32 out;
	POP(int32, a);
	POP(uint32, b);
	out = a << b;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_SHL_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = a << b;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_SHL_Q:
{
	int64 a;
	uint32 b;
	int64 out;
	POP(int64, a);
	POP(uint32, b);
	out = a << b;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_SHL_UQ:
{
	uint64 a;
	uint32 b;
	uint64 out;
	POP(uint64, a);
	POP(uint32, b);
	out = a << b;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_SHR_I:
{
	int32 a;
	uint32 b;
	int32 out;
	POP(int32, a);
	POP(uint32, b);
	out = a >> b;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_SHR_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = a >> b;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_SHR_Q:
{
	int64 a;
	uint32 b;
	int64 out;
	POP(int64, a);
	POP(uint32, b);
	out = a >> b;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_SHR_UQ:
{
	uint64 a;
	uint32 b;
	uint64 out;
	POP(uint64, a);
	POP(uint32, b);
	out = a >> b;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_NEG_I:
{
	int32 a;
	int32 out;
	POP(int32, a);
	out = -a;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_NEG_Q:
{
	int64 a;
	int64 out;
	POP(int64, a);
	out = -a;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_NEG_F:
{
	float32 a;
	float32 out;
	POP(float32, a);
	out = -a;
	PUSH(float32, out);
	INC_PC;
	break;
}

case OP_NEG_D:
{
	float64 a;
	float64 out;
	POP(float64, a);
	out = -a;
	PUSH(float64, out);
	INC_PC;
	break;
}

case OP_LT_I:
{
	int32 a;
	int32 b;
	uint32 out;
	POP(int32, a);
	POP(int32, b);
	out = (a < b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LT_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = (a < b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LT_Q:
{
	int64 a;
	int64 b;
	uint32 out;
	POP(int64, a);
	POP(int64, b);
	out = (a < b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LT_UQ:
{
	uint64 a;
	uint64 b;
	uint32 out;
	POP(uint64, a);
	POP(uint64, b);
	out = (a < b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LT_F:
{
	float32 a;
	float32 b;
	uint32 out;
	POP(float32, a);
	POP(float32, b);
	out = (a < b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LT_D:
{
	float64 a;
	float64 b;
	uint32 out;
	POP(float64, a);
	POP(float64, b);
	out = (a < b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LE_I:
{
	int32 a;
	int32 b;
	uint32 out;
	POP(int32, a);
	POP(int32, b);
	out = (a <= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LE_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = (a <= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LE_Q:
{
	int64 a;
	int64 b;
	uint32 out;
	POP(int64, a);
	POP(int64, b);
	out = (a <= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LE_UQ:
{
	uint64 a;
	uint64 b;
	uint32 out;
	POP(uint64, a);
	POP(uint64, b);
	out = (a <= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LE_F:
{
	float32 a;
	float32 b;
	uint32 out;
	POP(float32, a);
	POP(float32, b);
	out = (a <= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LE_D:
{
	float64 a;
	float64 b;
	uint32 out;
	POP(float64, a);
	POP(float64, b);
	out = (a <= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GT_I:
{
	int32 a;
	int32 b;
	uint32 out;
	POP(int32, a);
	POP(int32, b);
	out = (a > b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GT_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = (a > b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GT_Q:
{
	int64 a;
	int64 b;
	uint32 out;
	POP(int64, a);
	POP(int64, b);
	out = (a > b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GT_UQ:
{
	uint64 a;
	uint64 b;
	uint32 out;
	POP(uint64, a);
	POP(uint64, b);
	out = (a > b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GT_F:
{
	float32 a;
	float32 b;
	uint32 out;
	POP(float32, a);
	POP(float32, b);
	out = (a > b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GT_D:
{
	float64 a;
	float64 b;
	uint32 out;
	POP(float64, a);
	POP(float64, b);
	out = (a > b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GE_I:
{
	int32 a;
	int32 b;
	uint32 out;
	POP(int32, a);
	POP(int32, b);
	out = (a >= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GE_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = (a >= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GE_Q:
{
	int64 a;
	int64 b;
	uint32 out;
	POP(int64, a);
	POP(int64, b);
	out = (a >= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GE_UQ:
{
	uint64 a;
	uint64 b;
	uint32 out;
	POP(uint64, a);
	POP(uint64, b);
	out = (a >= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GE_F:
{
	float32 a;
	float32 b;
	uint32 out;
	POP(float32, a);
	POP(float32, b);
	out = (a >= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_GE_D:
{
	float64 a;
	float64 b;
	uint32 out;
	POP(float64, a);
	POP(float64, b);
	out = (a >= b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_EQ_I:
{
	int32 a;
	int32 b;
	uint32 out;
	POP(int32, a);
	POP(int32, b);
	out = (a == b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_EQ_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = (a == b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_EQ_Q:
{
	int64 a;
	int64 b;
	uint32 out;
	POP(int64, a);
	POP(int64, b);
	out = (a == b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_EQ_UQ:
{
	uint64 a;
	uint64 b;
	uint32 out;
	POP(uint64, a);
	POP(uint64, b);
	out = (a == b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_EQ_F:
{
	float32 a;
	float32 b;
	uint32 out;
	POP(float32, a);
	POP(float32, b);
	out = (a == b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_EQ_D:
{
	float64 a;
	float64 b;
	uint32 out;
	POP(float64, a);
	POP(float64, b);
	out = (a == b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_NE_I:
{
	int32 a;
	int32 b;
	uint32 out;
	POP(int32, a);
	POP(int32, b);
	out = (a != b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_NE_UI:
{
	uint32 a;
	uint32 b;
	uint32 out;
	POP(uint32, a);
	POP(uint32, b);
	out = (a != b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_NE_Q:
{
	int64 a;
	int64 b;
	uint32 out;
	POP(int64, a);
	POP(int64, b);
	out = (a != b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_NE_UQ:
{
	uint64 a;
	uint64 b;
	uint32 out;
	POP(uint64, a);
	POP(uint64, b);
	out = (a != b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_NE_F:
{
	float32 a;
	float32 b;
	uint32 out;
	POP(float32, a);
	POP(float32, b);
	out = (a != b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_NE_D:
{
	float64 a;
	float64 b;
	uint32 out;
	POP(float64, a);
	POP(float64, b);
	out = (a != b) ? 1 : 0;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LOAD_CONST_I:
{
	int32& value = ((Instr_load_const_i*)instr)->value;
	int32 out;
	out = value;
	PUSH(int32, out);
	INC_PC;
	break;
}

case OP_LOAD_CONST_UI:
{
	uint32& value = ((Instr_load_const_ui*)instr)->value;
	uint32 out;
	out = value;
	PUSH(uint32, out);
	INC_PC;
	break;
}

case OP_LOAD_CONST_Q:
{
	int64& value = ((Instr_load_const_q*)instr)->value;
	int64 out;
	out = value;
	PUSH(int64, out);
	INC_PC;
	break;
}

case OP_LOAD_CONST_UQ:
{
	uint64& value = ((Instr_load_const_uq*)instr)->value;
	uint64 out;
	out = value;
	PUSH(uint64, out);
	INC_PC;
	break;
}

case OP_LOAD_CONST_F:
{
	float32& value = ((Instr_load_const_f*)instr)->value;
	float32 out;
	out = value;
	PUSH(float32, out);
	INC_PC;
	break;
}

case OP_LOAD_CONST_D:
{
	float64& value = ((Instr_load_const_d*)instr)->value;
	float64 out;
	out = value;
	PUSH(float64, out);
	INC_PC;
	break;
}

case OP_POP_I:
{
	int32 top;
	POP(int32, top);
	top;
	INC_PC;
	break;
}

case OP_POP_UI:
{
	uint32 top;
	POP(uint32, top);
	top;
	INC_PC;
	break;
}

case OP_POP_Q:
{
	int64 top;
	POP(int64, top);
	top;
	INC_PC;
	break;
}

case OP_POP_UQ:
{
	uint64 top;
	POP(uint64, top);
	top;
	INC_PC;
	break;
}

case OP_POP_F:
{
	float32 top;
	POP(float32, top);
	top;
	INC_PC;
	break;
}

case OP_POP_D:
{
	float64 top;
	POP(float64, top);
	top;
	INC_PC;
	break;
}

case OP_PRINT_I:
{
	int32 a;
	POP(int32, a);
	printf("%d\n",a);
	INC_PC;
	break;
}

#endif

