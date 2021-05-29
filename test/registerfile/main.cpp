#include "Vregisterfile.h"
#include <verilated_vcd_c.h>
#include <iostream>
#include "Wrap.h"
#include <gtest/gtest.h>


int test(int argc, char const *argv[])
{
    Verilated::traceEverOn(true);
    Vregisterfile rf;

    Wrap<Vregisterfile> wrap ("test.vcd", &rf);
    rf.eval();

    rf.RegisterFile__DOT__registers[29] = 13;
    rf.rst = 0;
    rf.en = 1;
    rf.we = 0;    

    for (int x = 0; x < 5; x++) {
        rf.clk = 1;
        rf.reg_a = 30;
        rf.reg_b = 29;
        rf.reg_c = 30;
        rf.we = 1;
        rf.data_c = rf.data_a + rf.data_b;
        wrap.tick();
        std::cout << "After " << x + 1 << " " << rf.data_a << " " << rf.data_b << "\n";
    }

    /* code */
    return 0;
}

TEST(RegisterFileTest, X0NoWrite) {
    Verilated::traceEverOn(true);
    Vregisterfile rf;
    Wrap<Vregisterfile> wrap ("X0NoWrite.vcd", &rf);
    rf.eval();
    rf.en = 1;
    rf.reg_a = 0;
    wrap.tick();
    EXPECT_EQ(rf.data_a, 0);

    rf.reg_c = 0;
    rf.data_c = 10;
    rf.we = 1;
    wrap.tick();
    EXPECT_EQ(rf.data_a, 0);

    rf.we = 0;
    wrap.tick();
    EXPECT_EQ(rf.data_a, 0);
}
