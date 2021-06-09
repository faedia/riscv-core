#include "RegisterFile.h"
#include <verilated_vcd_c.h>
#include "Wrap.h"
#include <gtest/gtest.h>

TEST(RegisterFileTest, X0NoWrite)
{
    Verilated::traceEverOn(true);
    ClockedModule<RegisterFile> rf("X0NoWrite.vcd");
    rf.m.eval();
    rf.m.en = 1;
    rf.m.reg_a = 0;
    rf.tick();
    ASSERT_EQ(rf.m.data_a, 0);

    rf.m.reg_c = 0;
    rf.m.data_c = 10;
    rf.m.we = 1;
    rf.tick();
    ASSERT_EQ(rf.m.data_a, 0);

    rf.m.we = 0;
    rf.tick();
    ASSERT_EQ(rf.m.data_a, 0);
}

TEST(RegisterFileTest, X1toPCWrite)
{
    Verilated::traceEverOn(true);
    for (int i = 1; i < 32; i++)
    {
        ClockedModule<RegisterFile> rf("X1to31Write.vcd");
        rf.m.eval();
        rf.m.en = 1;
        rf.m.reg_a = i;
        rf.m.reg_c = i;
        rf.m.data_c = 10;
        
        rf.m.we = 0;
        rf.tick();
        ASSERT_EQ(rf.m.data_a, 0);

        rf.m.we = 1;
        rf.tick();
        ASSERT_EQ(rf.m.data_a, 10);

        rf.m.we = 0;
        rf.tick();
        ASSERT_EQ(rf.m.data_a, 10);
    }
}

TEST(RegisterFileTest, WriteAbovePC)
{
    Verilated::traceEverOn(true);
    ClockedModule<RegisterFile> rf("X1to31Write.vcd");
    rf.m.eval();
    rf.m.en = 1;

    rf.m.en = 1;
    rf.m.reg_a = 32;
    rf.m.reg_c = 32;
    rf.m.data_c = 10;

    rf.m.we = 0;
    rf.tick();
    ASSERT_EQ(rf.m.data_a, 0);

    rf.m.we = 1;
    rf.tick();
    ASSERT_EQ(rf.m.data_a, 0);

    rf.m.we = 0;
    rf.tick();
    ASSERT_EQ(rf.m.data_a, 0);
}
