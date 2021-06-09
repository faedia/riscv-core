#include <bitset>

#ifndef H_INSTR_DECODE_HELPER
#define H_INSTR_DECODE_HELPER

#define OP_KIND_SZ 2
#define FUNC_KIND_SZ 4
#define REG_SZ 5
#define WORD_SZ 32

#define INNER_INSTR_SZ 46
#define INSTR_SZ (OP_KIND_SZ + INNER_INSTR_SZ)

typedef signed int t_word;
typedef unsigned int t_uword;
typedef unsigned char t_register;

enum t_op_kind : unsigned char
{
    OK_UNKNOWN,
    OK_OP_IMM,
    OK_OP_LUI,
    OK_OP_AUIPC
};

enum t_func_kind : unsigned char
{ 
    FK_ADD,
    FK_SUB,
    FK_SLT,
    FK_SLTU,
    FK_AND,
    FK_OR,
    FK_XOR,
    FK_SLL,
    FK_SRL,
    FK_SRA
};

struct t_op_imm_instr
{
    t_func_kind func;
    t_register src_register;
    union
    {
        t_uword uimm;
        t_word imm;
    };
    t_register dest_register;
};

struct t_op_lui_instr
{
    t_word imm;
    t_register dest_register;
};

struct t_decoded_instr
{
    t_op_kind kind;
    union
    {
        t_op_imm_instr op_imm_instr;
        t_op_lui_instr op_lui_instr;
    };
};

t_decoded_instr convert_decoded_instr(const std::bitset<INSTR_SZ>& decoded_instr);

#endif