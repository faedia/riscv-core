import Types::*;

module ALU (
    input t_func_kind func,
    input t_word a_op,
    input t_word b_op,
    
    output t_word result
);
    always_comb begin
        case (func)
            FK_ADD:
                result = a_op + b_op;
            FK_SUB:
                result = a_op - b_op;
            FK_SLT:
                result = t_uword'(a_op < b_op);
            FK_SLTU:
                result = t_uword'(t_uword'(a_op) < t_uword'(b_op));
            FK_AND:
                result = a_op & b_op;
            FK_OR:
                result = a_op | b_op;
            FK_XOR:
                result = a_op ^ b_op;
            FK_SLL:
                result = a_op << b_op;
            FK_SRL:
                result = a_op >> b_op;
            FK_SRA:
                result = a_op >>> b_op;
            default:
                ; 
        endcase
    end
endmodule