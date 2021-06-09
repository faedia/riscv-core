#include "Decoder.h"
#include <verilated_vcd_c.h>
#include <verilated_cov.h>
#include <verilated.h>
#include "Wrap.h"
#include <gtest/gtest.h>

#include "instr_decode_helper.h"

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