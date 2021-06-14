#include "Execute.h"
#include <verilated_vcd_c.h>
#include <verilated_cov.h>
#include <verilated.h>
#include "Wrap.h"
#include <gtest/gtest.h>

#include "../Decoder/instr_decode_helper.h"

#include <iostream>

TEST(TESTING, TEST)
{
    Verilated::traceEverOn(true);
    UnclockedModule<Execute> exec("TEST.vcd");
    t_decoded_instr instr;
    instr.kind = OK_OP_LUI;
    instr.op_lui_instr.dest_register = 1;
    instr.op_lui_instr.imm = 5;
    exec.m.instr = to_decoded_instr (instr).to_ullong();
    exec.m.pc = 0;
    exec.tick();

    ASSERT_EQ(exec.m.data_c, 5);
}