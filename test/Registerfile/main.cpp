#include "RegisterFile.h"
#include <verilated_vcd_c.h>
#include "Wrap.h"
#include <gtest/gtest.h>

TEST(RegisterFileTest, X0NoWrite)
{
    Verilated::traceEverOn(true);
    RegisterFile rf;
    Wrap<RegisterFile> wrap("X0NoWrite.vcd", &rf);
    rf.eval();
    rf.en = 1;
    rf.reg_a = 0;
    wrap.tick();
    ASSERT_EQ(rf.data_a, 0);

    rf.reg_c = 0;
    rf.data_c = 10;
    rf.we = 1;
    wrap.tick();
    ASSERT_EQ(rf.data_a, 0);

    rf.we = 0;
    wrap.tick();
    ASSERT_EQ(rf.data_a, 0);
}

TEST(RegisterFileTest, X1toPCWrite)
{
    Verilated::traceEverOn(true);
    for (int i = 1; i < 32; i++)
    {
        RegisterFile rf;
        Wrap<RegisterFile> wrap("X1to31Write.vcd", &rf);
        rf.eval();
        rf.en = 1;
        rf.reg_a = i;
        rf.reg_c = i;
        rf.data_c = 10;
        
        rf.we = 0;
        wrap.tick();
        ASSERT_EQ(rf.data_a, 0);

        rf.we = 1;
        wrap.tick();
        ASSERT_EQ(rf.data_a, 10);

        rf.we = 0;
        wrap.tick();
        ASSERT_EQ(rf.data_a, 10);
    }
}

TEST(RegisterFileTest, WriteAbovePC)
{
    Verilated::traceEverOn(true);
    RegisterFile rf;
    Wrap<RegisterFile> wrap("X1to31Write.vcd", &rf);
    rf.eval();
    rf.en = 1;

    rf.en = 1;
    rf.reg_a = 32;
    rf.reg_c = 32;
    rf.data_c = 10;

    rf.we = 0;
    wrap.tick();
    ASSERT_EQ(rf.data_a, 0);

    rf.we = 1;
    wrap.tick();
    ASSERT_EQ(rf.data_a, 0);

    rf.we = 0;
    wrap.tick();
    ASSERT_EQ(rf.data_a, 0);
}