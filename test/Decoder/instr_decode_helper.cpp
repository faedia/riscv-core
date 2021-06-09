#include "instr_decode_helper.h"

#define OP_IMM_SZ (FUNC_KIND_SZ + REG_SZ + WORD_SZ + REG_SZ)
#define OP_LUI_SZ (WORD_SZ + REG_SZ)

#define OP_IMM_RM_BITS (INNER_INSTR_SZ - OP_IMM_SZ)
#define OP_LUI_RM_BITS (INNER_INSTR_SZ - OP_LUI_SZ)

t_op_imm_instr get_op_imm_instr(const std::bitset<INSTR_SZ>& decoded_instr)
{
    t_op_imm_instr ret_val;
    auto func = (decoded_instr << OP_KIND_SZ) >> (REG_SZ + WORD_SZ + REG_SZ + OP_KIND_SZ + OP_IMM_RM_BITS);
    auto src_reg = (decoded_instr << OP_KIND_SZ + FUNC_KIND_SZ) >> (WORD_SZ + REG_SZ + OP_KIND_SZ + FUNC_KIND_SZ + OP_IMM_RM_BITS);
    auto imm = (decoded_instr << OP_KIND_SZ + FUNC_KIND_SZ + REG_SZ) >> (REG_SZ + OP_KIND_SZ + FUNC_KIND_SZ + REG_SZ + OP_IMM_RM_BITS);
    auto dest_reg = (decoded_instr << OP_KIND_SZ + FUNC_KIND_SZ + REG_SZ + WORD_SZ) >> (OP_KIND_SZ + FUNC_KIND_SZ + REG_SZ + WORD_SZ + OP_IMM_RM_BITS);
    ret_val.func = (t_func_kind)func.to_ullong();
    ret_val.src_register = (t_register)src_reg.to_ullong();
    ret_val.uimm = (t_uword)imm.to_ullong();
    ret_val.dest_register = (t_register)dest_reg.to_ullong();
    return ret_val;
}

t_op_lui_instr get_op_lui_instr(const std::bitset<INSTR_SZ>& decoded_instr)
{
    t_op_lui_instr ret_val;
    auto imm = (decoded_instr << OP_KIND_SZ) >> (REG_SZ + OP_KIND_SZ + OP_LUI_RM_BITS);
    auto dest_reg = (decoded_instr << OP_KIND_SZ + WORD_SZ) >> (WORD_SZ + OP_KIND_SZ + OP_LUI_RM_BITS);
    ret_val.imm = (t_uword)imm.to_ullong();
    ret_val.dest_register = (t_register)dest_reg.to_ullong();
    return ret_val;
}

t_decoded_instr convert_decoded_instr(const std::bitset<INSTR_SZ>& decoded_instr)
{
    t_decoded_instr ret_val;
    ret_val.kind = (t_op_kind) (decoded_instr >> INNER_INSTR_SZ).to_ullong();
    switch (ret_val.kind)
    {
    case OK_OP_IMM:
        ret_val.op_imm_instr = get_op_imm_instr(decoded_instr);
        break;
    case OK_OP_LUI:
        ret_val.op_lui_instr = get_op_lui_instr(decoded_instr);
    default:
        break;
    }
    return ret_val;
}