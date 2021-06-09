#include "Decoder.h"
#include <verilated_vcd_c.h>
#include <verilated_cov.h>
#include <verilated.h>
#include "Wrap.h"
#include <gtest/gtest.h>

#include "instr_decode_helper.h"

#define ASSERT_OP_LUI_INSTR(di, a_imm, a_dest_reg)\
    ASSERT_EQ(di.kind, OK_OP_LUI);\
    ASSERT_EQ(di.op_lui_instr.imm, a_imm);\
    ASSERT_EQ(di.op_lui_instr.dest_register, a_dest_reg);\

TEST(DecoderImmediateIntegerRV32, LUI)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("LUI.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0x000010b7;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_LUI_INSTR(instr, 1<<12, 1);
}

TEST(DecoderImmediateIntegerRV32, LUI_NEG)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("LUI_NEG.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0xfffff0b7;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_LUI_INSTR(instr, -1<<12, 1);
}