#include "Decoder.h"
#include <verilated_vcd_c.h>
#include <verilated_cov.h>
#include <verilated.h>
#include "Wrap.h"
#include <gtest/gtest.h>

#include <bitset>

#define OP_KIND_SZ 1
#define FUNC_KIND_SZ 4
#define REG_SZ 5
#define WORD_SZ 32

#define INNER_INSTR_SZ 46
#define INSTR_SZ (OP_KIND_SZ + INNER_INSTR_SZ)

enum t_op_kind : unsigned char
{
    OK_UNKNOWN,
    OK_OP_IMM
};

enum t_func_kind : unsigned char
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
    auto func = (decoded_instr << OP_KIND_SZ) >> (REG_SZ + WORD_SZ + REG_SZ + OP_KIND_SZ);
    auto src_reg = (decoded_instr << OP_KIND_SZ + FUNC_KIND_SZ) >> (WORD_SZ + REG_SZ + OP_KIND_SZ + FUNC_KIND_SZ);
    auto imm = (decoded_instr << OP_KIND_SZ + FUNC_KIND_SZ + REG_SZ) >> (REG_SZ + OP_KIND_SZ + FUNC_KIND_SZ + REG_SZ);
    auto dest_reg = (decoded_instr << OP_KIND_SZ + FUNC_KIND_SZ + REG_SZ + WORD_SZ) >> (OP_KIND_SZ + FUNC_KIND_SZ + REG_SZ + WORD_SZ);
    ret_val.func = (t_func_kind)func.to_ullong();
    ret_val.src_register = (unsigned char)src_reg.to_ullong();
    ret_val.imm = (int)imm.to_ullong();
    ret_val.dest_register = (unsigned char)dest_reg.to_ullong();
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
    
    default:
        break;
    }
    return ret_val;
}

#define ASSERT_OP_IMM_INSTR(di, a_func, a_src_reg, a_imm, a_dest_reg)\
    ASSERT_EQ(di.kind, OK_OP_IMM);\
    ASSERT_EQ(di.op_imm_instr.func, a_func);\
    ASSERT_EQ(di.op_imm_instr.src_register, a_src_reg);\
    ASSERT_EQ(di.op_imm_instr.imm, a_imm);\
    ASSERT_EQ(di.op_imm_instr.dest_register, a_dest_reg);

TEST(DecoderImmediateIntegerRV32, ADDI)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("ADDI.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0x00A08093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr,FK_ADD, 1, 10, 1);
}

TEST(DecoderImmediateIntegerRV32, ADDINegative)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("ADDINegative.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0xFFF08093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_ADD, 1, -1, 1);
}

TEST(DecoderImmediateIntegerRV32, SLTI_U)
{   
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("SLTI_U.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0x00a12093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_SLT, 2, 10, 1);
    
    dec.m.instr = 0x00a13093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_SLTU, 2, 10, 1);

    dec.m.instr = 0xfff13093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_SLTU, 2, -1, 1);
}

TEST(DecoderImmediateIntegerRV32, ANDI)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("ANDI.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0x0011f093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_AND, 3, 1, 1);
}

TEST(DecoderImmediateIntegerRV32, ORI)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("ORI.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0x00226093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_OR, 4, 2, 1);
}

TEST(DecoderImmediateIntegerRV32, XORI)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("XORI.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0x0032c093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_XOR, 5, 3, 1);
}

TEST(DecoderImmediateIntegerRV32, SLLI)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("SLLI.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0x00431093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_SLL, 6, 4, 1);

    dec.m.instr = 0x01f39093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_SLL, 7, 31, 1);
}

TEST(DecoderImmediateIntegerRV32, SRLI)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("SRLI.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0x00545093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_SRL, 8, 5, 1);

    dec.m.instr = 0x01f4d093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_SRL, 9, 31, 1);
}

TEST(DecoderImmediateIntegerRV32, SRAI)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("SRAI.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0x40655093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_SRA, 10, 6, 1);

    dec.m.instr = 0x41f5d093;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_IMM_INSTR(instr, FK_SRA, 11, 31, 1);
}