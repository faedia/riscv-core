import Types::*;

module RegisterFile (
    input logic clk,
    input logic rst,
    input logic en,
    input logic we,
    input t_register reg_a,
    input t_register reg_b,
    input t_register reg_c,
    input t_word data_c,
    
    output t_word data_a,
    output t_word data_b
);
    t_word registers [31:1];

    initial begin
        foreach (registers[i])
            registers[i] = 0;
    end

    always_ff @(posedge clk or posedge rst) begin
        if (en) begin
            if (rst) begin 
                foreach (registers[i])
                    registers[i] <= 0;
            end

            if (we && reg_c != 0) begin
                registers[reg_c] <= data_c;
            end
        end
    end

    assign data_a = en || (reg_a == 0) ? registers[reg_a] : 0;
    assign data_b = en || (reg_b == 0) ? registers[reg_b] : 0;
endmodule