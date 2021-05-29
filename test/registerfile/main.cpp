#include "Vregisterfile.h"
#include <verilated_vcd_c.h>
#include <iostream>
#include "Wrap.h"


int main(int argc, char const *argv[])
{
    Verilated::traceEverOn(true);
    Vregisterfile rf;

    Wrap<Vregisterfile> wrap ("test.vcd", &rf);
    rf.eval();

    rf.RegisterFile__DOT__registers[2] = 13;
    rf.rst = 0;
    rf.en = 1;
    rf.we = 0;    

    for (int x = 0; x < 5; x++) {
        rf.clk = 1;
        rf.reg_a = 1;
        rf.reg_b = 2;
        rf.reg_c = 1;
        rf.we = 1;
        rf.data_c = rf.data_a + rf.data_b;
        wrap.tick();
        std::cout << "After " << x << " " << rf.RegisterFile__DOT__registers[1] << " " << rf.data_a << " " << rf.data_b << "\n";
    }

    /* code */
    return 0;
}
