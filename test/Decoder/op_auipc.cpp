#include "Decoder.h"
#include <verilated_vcd_c.h>
#include <verilated_cov.h>
#include <verilated.h>
#include "Wrap.h"
#include <gtest/gtest.h>

#include "instr_decode_helper.h"

#define ASSERT_OP_AUIPC_INSTR(di, a_imm, a_dest_reg)\
    ASSERT_EQ(di.kind, OK_OP_AUIPC);\
    ASSERT_EQ(di.op_auipc_instr.imm, a_imm);\
    ASSERT_EQ(di.op_auipc_instr.dest_register, a_dest_reg);\

TEST(DecoderImmediateIntegerRV32, AUIPC)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("AUIPC.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0x00001097;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_AUIPC_INSTR(instr, 1<<12, 1);
}

TEST(DecoderImmediateIntegerRV32, AUIPC_NEG)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Decoder> dec("AUIPC_NEG.vcd");
    t_decoded_instr instr;
    dec.m.instr = 0xfffff097;
    dec.tick();
    instr = convert_decoded_instr(dec.m.decoded_instr);
    ASSERT_OP_AUIPC_INSTR(instr, -1<<12, 1);
}