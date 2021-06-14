import Types::*;

module ALU (
    input t_func_kind func,
    input t_word a,
    input t_word b,
    
    output t_word result
);

    /* verilator lint_off UNOPTFLAT */
    t_word tmp_result;

    initial begin
        tmp_result = 0;
    end

    always_comb begin
        case (func)
            FK_ADD:
                tmp_result = a + b;
            FK_SUB:
                tmp_result = a - b;
            FK_SLT:
                tmp_result = t_uword'(a < b);
            FK_SLTU:
                tmp_result = t_uword'(t_uword'(a) < t_uword'(b));
            FK_AND:
                tmp_result = a & b;
            FK_OR:
                tmp_result = a | b;
            FK_XOR:
                tmp_result = a ^ b;
            FK_SLL:
                tmp_result = a << b;
            FK_SRL:
                tmp_result = a >> b;
            FK_SRA:
                tmp_result = a >>> b;
            default:
                tmp_result = 0; 
        endcase
    end

    assign result = tmp_result;
endmodule