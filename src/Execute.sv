import Types::*;

module Execute (
    input t_decoded_instr instr,
    input t_word pc,
    
    input t_word data_a,
    input t_word data_b,

    output t_register reg_a,
    output t_register reg_b,
    output t_register reg_c,

    output t_word data_c,
    output logic reg_we,

    output t_word new_pc
);

    t_func_kind alu_func;
    t_word alu_a, alu_b;
    t_word alu_result;

    ALU alu(
        .func(alu_func),
        .a(alu_a),
        .b(alu_b),
        .result(alu_result)
    );

    initial begin
        alu_func = 0;
        alu_a = 0;
        alu_b = 0;
    end

    always_comb begin
        case (instr.kind)
            OK_OP_IMM:
                begin
                    t_op_imm_instr imm_instr = instr.instr_data.op_imm_instr;
                    reg_a = imm_instr.src_register;
                    reg_c = imm_instr.dest_register;
                    alu_func = imm_instr.func;
                    alu_a = data_a;
                    alu_b = imm_instr.immediate_value;
                    data_c = alu_result;
                    reg_we = 1;
                    new_pc = pc + 4;
                end
            OK_OP_LUI:
                begin
                    t_op_lui_instr lui_instr = instr.instr_data.op_lui_instr;
                    reg_c = lui_instr.dest_register;
                    data_c = lui_instr.immediate_value;
                    reg_we = 1;
                    new_pc = pc + 4;
                end
            OK_OP_AUIPC:
                begin
                    t_op_auipc_instr auipc_instr = instr.instr_data.op_auipc_instr;
                    alu_func = FK_ADD;
                    alu_a = pc;
                    alu_b = auipc_instr.immediate_value;
                    reg_c = auipc_instr.dest_register;
                    data_c = alu_result;
                    reg_we = 1;
                    new_pc = pc + 4;
                end
            default:
                begin
                    reg_we = 0;
                    new_pc = pc;
                end
        endcase
    end

endmodule