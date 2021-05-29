/* verilator lint_off DECLFILENAME */
module RegisterFile (
    input logic clk,
    input logic rst,
    input logic en,
    input logic we,
    input logic [5:0] reg_a,
    input logic [5:0] reg_b,
    input logic [5:0] reg_c,
    input logic [31:0] data_c,
    
    output logic [31:0] data_a,
    output logic [31:0] data_b
);
    logic [32:0] [31:0] registers;

    initial begin
        for (int i = 0; i < $size(registers); i++) begin
            registers[i] = 0;
        end
    end

    always_ff @(posedge clk or posedge rst) begin
        if (en) begin
            if (rst) begin 
                for (int i = 0; i < $size(registers); i++) begin
                    registers[i] <= 0;
                end
            end

            if (we && reg_c != 0) begin
                registers[reg_c] <= data_c;
            end
        end
    end

    assign data_a = en ? registers[reg_a] : 0;
    assign data_b = en ? registers[reg_b] : 0;
endmodule