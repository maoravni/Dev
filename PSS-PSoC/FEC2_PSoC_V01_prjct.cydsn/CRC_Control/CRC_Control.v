
//`#start header` -- edit after this line, do not edit this line
// ========================================
//
// Copyright YOUR COMPANY, THE YEAR
// All Rights Reserved
// UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF your company.
//
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line
// Generated on 01/26/2014 at 13:48
// Component: CRC_Control
module CRC_Control (
	output reg RxCrcEn,
	output reg TxCrcLd,
	input  clk,
	input  Reset
);
	parameter Rx_CRC_En_Comp = 0;
	parameter Tx_CRC_Load_Comp = 0;

//`#start body` -- edit after this line, do not edit this line

//        Your code goes here
	/* Counter */
	wire [7:0] Count;
	reg [7:0] Temp_Count;
	
	/* Main */
	always@(posedge clk or posedge Reset)
	begin
		if(Reset)
		begin
			Temp_Count <= 8'h00;
			RxCrcEn <= 1'b0;
			RxCrcEn <= 1'b1;
			end
		else
		begin
			Temp_Count <= Count + 1 ;
			
			if (Count > Rx_CRC_En_Comp)
			begin
				RxCrcEn <= 1'b0;
			end
			else
			begin
				RxCrcEn <= 1'b1;
			end
			
			if (Count == Tx_CRC_Load_Comp)
			begin
				TxCrcLd <= 1'b1;
			end	
			else
			begin
				TxCrcLd <= 1'b0;
			end			
		end
	end
	
	assign Count = Temp_Count;

	
	
//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
