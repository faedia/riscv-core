#include "Vregisterfile.h"
#include <verilated_vcd_c.h>
#include "Wrap.h"
#include <gtest/gtest.h>

TEST(RegisterFileTest, X0NoWrite)
{
    Verilated::traceEverOn(true);
    Vregisterfile rf;
    Wrap<Vregisterfile> wrap("X0NoWrite.vcd", &rf);
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
    for (int i = 1; i < 33; i++)
    {
        Vregisterfile rf;
        Wrap<Vregisterfile> wrap("X1to31Write.vcd", &rf);
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
    Vregisterfile rf;
    Wrap<Vregisterfile> wrap("X1to31Write.vcd", &rf);
    rf.eval();
    rf.en = 1;

    rf.en = 1;
    rf.reg_a = 33;
    rf.reg_c = 33;
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
