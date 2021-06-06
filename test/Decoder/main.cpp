#include "Decoder.h"
#include <verilated_vcd_c.h>
#include "Wrap.h"
#include <gtest/gtest.h>

#include <bitset>

enum t_op_kind : unsigned char
{
    OK_UNKNOWN,
    OK_OP_IMM
};

enum t_func_kind
{ 
    FK_ADD,
    FK_SUB,
    FK_SLT,
    FK_SLTU,
    FK_AND,
    FK_OR,
    FK_XOR,
    FK_SLL,
    FK_SRL,
    FK_SRA
};

struct t_op_imm_instr
{
    t_func_kind func;
    unsigned char src_register;
    union
    {
        unsigned int uimm;
        int imm;
    };
    unsigned char dest_register;
};

struct t_decoded_instr
{
    t_op_kind kind;
    union
    {
        t_op_imm_instr op_imm_instr;
    };
};

t_op_imm_instr get_op_imm_instr(const std::bitset<47>& decoded_instr)
{
    t_op_imm_instr ret_val;
    auto func = (decoded_instr << 1) >> (5 + 32 + 5 + 1);
    auto src_reg = (decoded_instr << 1 + 4) >> (32 + 5 + 1 + 4);
    auto imm = (decoded_instr << 1 + 4 + 5) >> (5 + 1 + 4 + 5);
    auto dest_reg = (decoded_instr << 1 + 4 + 5 + 32) >> (1 + 4 + 5 + 32);
    ret_val.func = (t_func_kind)func.to_ulong();
    ret_val.src_register = (unsigned char)src_reg.to_ulong();
    ret_val.imm = (int)imm.to_ulong();
    ret_val.dest_register = (unsigned char)dest_reg.to_ulong();
    return ret_val;
}

t_decoded_instr convert_decoded_instr(const std::bitset<47>& decoded_instr)
{
    t_decoded_instr ret_val;
    ret_val.kind = (t_op_kind) (bool(decoded_instr[46]));
    switch (ret_val.kind)
    {
    case OK_OP_IMM:
        ret_val.op_imm_instr = get_op_imm_instr(decoded_instr);
        break;
    
    default:
        break;
    }
    return ret_val;
}

TEST(DecoderImmediateIntegerRV32, ADDI)
{
    Decoder dec;
    //Wrap<Decoder> wrap("ADDI.vcd", &dec);
    dec.eval();
    dec.instr = 0x00A08093;
    dec.eval();
    t_decoded_instr instr = convert_decoded_instr(dec.decoded_instr);
    ASSERT_EQ(instr.kind, OK_OP_IMM);
    ASSERT_EQ(instr.op_imm_instr.func, FK_ADD);
    ASSERT_EQ(instr.op_imm_instr.src_register, 1);
    ASSERT_EQ(instr.op_imm_instr.imm, 10);
    ASSERT_EQ(instr.op_imm_instr.src_register, 1);
}