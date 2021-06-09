import Types::*;

module Decoder (
    input t_word instr,
    output t_decoded_instr decoded_instr,
    output logic valid
);
    t_opcode opcode;

    always_comb begin
        opcode = instr[6:0];
        unique case (opcode)
            OP_IMM:
                begin
                    t_op_imm_instr tmp_instr;
                    tmp_instr.dest_register = instr[11:7];
                    tmp_instr.src_register = instr[19:15];

                    unique case (instr[14:12])
                        F3_ADD:
                            tmp_instr.func = FK_ADD;
                        F3_SLT:
                            tmp_instr.func = FK_SLT;
                        F3_SLTU:
                            tmp_instr.func = FK_SLTU;
                        F3_AND:
                            tmp_instr.func = FK_AND;
                        F3_OR:
                            tmp_instr.func = FK_OR;
                        F3_XOR:
                            tmp_instr.func = FK_XOR;
                        F3_SLL:
                            tmp_instr.func = FK_SLL;
                        F3_SRL, F3_SRA:
                            if (instr[30])
                                tmp_instr.func = FK_SRA;
                            else
                                tmp_instr.func = FK_SRL;
                        default:
                            tmp_instr.func = 0;
                    endcase

                    case (tmp_instr.func)
                        FK_ADD,
                        FK_SUB,
                        FK_SLT,
                        // The SLTIU instruction does perform sign extending but just treats the number
                        // as unsigned anyway
                        FK_SLTU,
                        FK_AND,
                        FK_OR,
                        FK_XOR:
                            tmp_instr.immediate_value = {{20{instr[31]}}, instr[31:20]};

                        FK_SLL,
                        FK_SRL,
                        FK_SRA:
                            tmp_instr.immediate_value = {{27{1'b0}}, instr[24:20]};
                        default:
                            tmp_instr.immediate_value = 0;
                    endcase

                    decoded_instr.kind = OK_OP_IMM;
                    decoded_instr.instr_data = tmp_instr;
                    valid = 1;
                end
            OP_LUI:
                begin
                    t_op_lui_instr tmp_instr;
                    tmp_instr.dest_register = instr[11:7];
                    tmp_instr.immediate_value = {instr[31:12], {12{1'b0}}};

                    decoded_instr.kind = OK_OP_LUI;
                    decoded_instr.instr_data = {tmp_instr, {($bits(t_instr_data_union) - $bits(t_op_lui_instr)){1'b0}}};
                    valid = 1;
                end
            default:
                valid = 0;
        endcase
    end
endmodule