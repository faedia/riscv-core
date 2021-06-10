#include "ALU.h"
#include <verilated_vcd_c.h>
#include <verilated_cov.h>
#include <verilated.h>
#include "Wrap.h"
#include <gtest/gtest.h>

#include "../Decoder/instr_decode_helper.h"

#include <cstdlib>

TEST(ALUIntegerRV32, ADD)
{
    Verilated::traceEverOn(true);
    UnclockedModule<ALU> alu("ADD.vcd");

    alu.m.func = FK_ADD;

    for (int i = 0; i < 1000000; i++)
    {
        t_uword a = std::rand();
        t_uword b = std::rand();
        alu.m.a_op = a;
        alu.m.b_op = b;
        alu.tick();
        ASSERT_EQ((t_word)alu.m.result, (t_word)a + (t_word)b) << "Adding " << (t_word)a << " and " << (t_word)b;
    }
}

TEST(ALUIntegerRV32, SUB)
{
    Verilated::traceEverOn(true);
    UnclockedModule<ALU> alu("SUB.vcd");

    alu.m.func = FK_SUB;

    for (int i = 0; i < 1000000; i++)
    {
        t_uword a = std::rand();
        t_uword b = std::rand();
        alu.m.a_op = a;
        alu.m.b_op = b;
        alu.tick();
        ASSERT_EQ((t_word)alu.m.result, (t_word)a - (t_word)b) << "Subtracting " << (t_word)a << " and " << (t_word)b;
    }
}

TEST(ALUIntegerRV32, SLT)
{
    Verilated::traceEverOn(true);
    UnclockedModule<ALU> alu("SLT.vcd");

    alu.m.func = FK_SLT;

    for (int i = 0; i < 1000000; i++)
    {
        t_uword a = std::rand();
        t_uword b = std::rand();
        alu.m.a_op = a;
        alu.m.b_op = b;
        alu.tick();
        ASSERT_EQ((t_word)alu.m.result, (t_word)a < (t_word)b) << "Set less than " << (t_word)a << " and " << (t_word)b;
    }
}

TEST(ALUIntegerRV32, SLTU)
{
    Verilated::traceEverOn(true);
    UnclockedModule<ALU> alu("SLTU.vcd");

    alu.m.func = FK_SLTU;

    for (int i = 0; i < 1000000; i++)
    {
        t_uword a = std::rand();
        t_uword b = std::rand();
        alu.m.a_op = a;
        alu.m.b_op = b;
        alu.tick();
        ASSERT_EQ((t_word)alu.m.result, a < b) << "Set less than unsigned " << a << " and " << b;
    }
}

TEST(ALUIntegerRV32, AND)
{
    Verilated::traceEverOn(true);
    UnclockedModule<ALU> alu("AND.vcd");

    alu.m.func = FK_AND;

    for (int i = 0; i < 1000000; i++)
    {
        t_uword a = std::rand();
        t_uword b = std::rand();
        alu.m.a_op = a;
        alu.m.b_op = b;
        alu.tick();
        ASSERT_EQ((t_word)alu.m.result, a & b) << "Anding " << (t_word)a << " and " << (t_word)b;
    }
}

TEST(ALUIntegerRV32, OR)
{
    Verilated::traceEverOn(true);
    UnclockedModule<ALU> alu("OR.vcd");

    alu.m.func = FK_OR;

    for (int i = 0; i < 1000000; i++)
    {
        t_uword a = std::rand();
        t_uword b = std::rand();
        alu.m.a_op = a;
        alu.m.b_op = b;
        alu.tick();
        ASSERT_EQ((t_word)alu.m.result, a | b) << "Oring " << (t_word)a << " and " << (t_word)b;
    }
}

TEST(ALUIntegerRV32, XOR)
{
    Verilated::traceEverOn(true);
    UnclockedModule<ALU> alu("XOR.vcd");

    alu.m.func = FK_XOR;

    for (int i = 0; i < 1000000; i++)
    {
        t_uword a = std::rand();
        t_uword b = std::rand();
        alu.m.a_op = a;
        alu.m.b_op = b;
        alu.tick();
        ASSERT_EQ((t_word)alu.m.result, a ^ b) << "Xoring " << (t_word)a << " and " << (t_word)b;
    }
}

TEST(ALUIntegerRV32, SLL)
{
    Verilated::traceEverOn(true);
    UnclockedModule<ALU> alu("SLL.vcd");

    alu.m.func = FK_SLL;

    for (int i = 0; i < 100000; i++)
    {
        t_uword a = std::rand();
        for (t_uword b = 0; b < 32; b++)
        {
            alu.m.a_op = a;
            alu.m.b_op = b;
            alu.tick();
            ASSERT_EQ((t_word)alu.m.result, a << b) << "Shiting left " << (t_word)a << " and " << (t_word)b;
        }
    }
}

TEST(ALUIntegerRV32, SRL)
{
    Verilated::traceEverOn(true);
    UnclockedModule<ALU> alu("SRL.vcd");

    alu.m.func = FK_SRL;

    for (int i = 0; i < 100000; i++)
    {
        t_uword a = std::rand();
        for (t_uword b = 0; b < 32; b++)
        {
            alu.m.a_op = a;
            alu.m.b_op = b;
            alu.tick();
            ASSERT_EQ((t_word)alu.m.result, a >> b) << "Shifting right logical " << (t_word)a << " and " << (t_word)b;
        }
    }
}

TEST(ALUIntegerRV32, SRA)
{
    Verilated::traceEverOn(true);
    UnclockedModule<ALU> alu("SRA.vcd");

    alu.m.func = FK_SRA;

    for (int i = 0; i < 100000; i++)
    {
        t_uword a = std::rand();
        for (t_uword b = 0; b < 32; b++)
        {
            alu.m.a_op = a;
            alu.m.b_op = b;
            alu.tick();
            ASSERT_EQ((t_word)alu.m.result, (t_word)a >> (t_word)b) << "Shifting right athrithmetic " << (t_word)a << " and " << (t_word)b;
        }
    }

    for (int i = 0; i < 100000; i++)
    {
        t_uword a = (std::rand() % (2147483647)) + 2147483647;
        ASSERT_LT ((t_word)a, 0);
        for (t_uword b = 0; b < 32; b++)
        {
            alu.m.a_op = a;
            alu.m.b_op = b;
            alu.tick();
            ASSERT_EQ((t_word)alu.m.result, (t_word)a >> (t_word)b) << "Shifting right athrithmetic " << (t_word)a << " and " << (t_word)b;
        }
    }
}