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

t_op_auipc_instr get_op_auipc_instr(const std::bitset<INSTR_SZ>& decoded_instr)
{
    return get_op_lui_instr(decoded_instr);
}

#define GET_OP(di, i, op_kind, OP_KIND)\
    case OK_OP_##OP_KIND:\
        (di.op_##op_kind##_instr = get_op_##op_kind##_instr(i));\
        break;

t_decoded_instr convert_decoded_instr(const std::bitset<INSTR_SZ>& decoded_instr)
{
    t_decoded_instr ret_val;
    ret_val.kind = (t_op_kind) (decoded_instr >> INNER_INSTR_SZ).to_ullong();
    switch (ret_val.kind)
    {
        GET_OP(ret_val, decoded_instr, imm, IMM)
        GET_OP(ret_val, decoded_instr, lui, LUI)
        GET_OP(ret_val, decoded_instr, auipc, AUIPC)
    default:
        break;
    }
    return ret_val;
}

unsigned long long from_op_imm_instr(t_op_imm_instr instr)
{
    unsigned long long bits = (unsigned long long)instr.func << (REG_SZ + WORD_SZ + REG_SZ + OP_IMM_RM_BITS);
    bits += (unsigned long long)instr.src_register << (WORD_SZ + REG_SZ + OP_IMM_RM_BITS);
    bits += (unsigned long long)instr.uimm << (REG_SZ + OP_IMM_RM_BITS);
    bits += (unsigned long long)instr.dest_register << OP_IMM_RM_BITS;
    return bits;
}

unsigned long long from_op_lui_instr(t_op_lui_instr instr)
{
    unsigned long long bits = (unsigned long long)instr.imm << (REG_SZ + OP_LUI_RM_BITS);
    bits += (unsigned long long)instr.dest_register << OP_LUI_RM_BITS;
    return bits;
}

unsigned long long from_op_auipc_instr(t_op_auipc_instr instr)
{
    return from_op_lui_instr(instr);
}

#define FROM_OP(bits, i, op_kind, OP_KIND)\
    case OK_OP_##OP_KIND:\
        (bits += from_op_##op_kind##_instr(i.op_##op_kind##_instr));\
        break;

std::bitset<INSTR_SZ> to_decoded_instr(t_decoded_instr instr)
{
    unsigned long long bits = 0;
    bits = (unsigned long long)instr.kind << (INNER_INSTR_SZ);
    switch (instr.kind)
    {
        FROM_OP(bits, instr, imm, IMM)
        FROM_OP(bits, instr, lui, LUI)
        FROM_OP(bits, instr, auipc, AUIPC)
    default:
        break;
    }

    return bits;
}