
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

`define ZERO 1'b0
`define ONE  1'b1
//`#end` -- edit above this line, do not edit this line
// Generated on 06/18/2014 at 13:55
// Component: Logic_Function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
module Logic_Function (
	/* Digital Outputs */
	output reg DO_0,
	output reg DO_1,
	output reg DO_2,
	output reg DO_3,
	output reg DO_4,
	output reg DO_5,
	output reg interrupt,
	/* Digital Inputs */
	input   DI_0,
	input   DI_1,
	input   DI_2,
	input   DI_3,
	input   DI_4,
	input   DI_5,
	/* Global Inputs */
	input   GI_0,
	input   GI_1,
	input   GI_2,
	input   GI_3,
	input   GI_4,
	input   GI_5,
	/* Outputs Controls */
	input   OC_0,  // SPWM signal generator ( clk = 100Hz ).
	input   OC_1,  // Output Enable Signal (ON/OFF).
	input   OC_2,  // PWM signal generator ( clk = 10kHz ).
	input   OC_3,  // PWM signal generator ( clk = 10kHz ).
	input   OC_4,  // PWM signal generator ( clk = 10kHz ).
	input   OC_5,  // PWM signal generator ( clk = 10kHz ).
	/* Clock */
	input   Clock,
	/* Reset */
	input   Reset,
	/* Enable */
	input   Enable,
	/* Board type configuration select */
	input [3:0] Function_Select	
);
	parameter Timer_int_value = 1;
	// Local parameters for channel multiplexer
	localparam DISABLE_OUTPUTS	        = 4'b0000;
	localparam HW_VALIDATION	        = 4'b0001;
	localparam SOFT_DEBUG	            = 4'b0010;
	localparam HEATER		            = 4'b0011;
	localparam LIFT_PB_ON_ERROR			= 4'b0100;
	localparam APPLICATOR		        = 4'b0101;
	localparam LIFT_PB_ON_ERROR_GEN_2	= 4'b0110;
	localparam LIFT_PB_ON_ERROR_GEN_3	= 4'b0111;
	// local parameters for timer state machime
	
	localparam RELOAD          = 3'b000;
	localparam COUNT_DOWN      = 3'b001;
	localparam STATES2         = 3'b010;
	localparam STATES3         = 3'b011;
	localparam STATES4         = 3'b100;
	localparam STATES5         = 3'b101;
	localparam STATES6         = 3'b110;
	localparam STATES7         = 3'b111;

	
//`#start body` -- edit after this line, do not edit this line
	reg [2:0] Timer_State;
	reg Timer_Trig;
	wire tc;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Your code goes here

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	always @(posedge Clock)
	begin
		if(Reset)
		begin
			Timer_State <= RELOAD;
			Timer_Trig 	<= 1'b0;	
			DO_0 <= `ZERO;	// Output Reset.
			if(Function_Select == LIFT_PB_ON_ERROR)
				DO_1 <= `ONE;	// Output Set for safety output.
			else
				DO_1 <= `ZERO;	// Output Reset.
			DO_2 <= `ZERO;	// Output Reset.
			DO_3 <= `ZERO;	// Output Reset.
			DO_4 <= `ZERO;	// Output Reset.
			DO_5 <= `ZERO;	// Output Reset.
			interrupt <= `ZERO;
		end
		else
		begin
			if(Enable)
			begin
				////////////////////////////////
				// Logic function multiplexer //
				////////////////////////////////
				/////////////////////////////////////////////////////////////////////////////
				case(Function_Select)
					(DISABLE_OUTPUTS):
					begin
						DO_0 <= `ZERO;	// Output - Not in use.
						DO_1 <= `ZERO;	// Output - Not in use.
						DO_2 <= `ZERO;	// Output - Not in use.
						DO_3 <= `ZERO;	// Output - Not in use.
						DO_4 <= `ZERO;	// Output - Not in use.
						DO_5 <= `ZERO;	// Output - Not in use.	
						interrupt <= `ZERO;
					end
					HEATER:
					begin
						DO_0 <= OC_0;	// Output - Heaters SSR.  
						DO_1 <= OC_1;	// Output - Not in use.
						DO_2 <= OC_2;	// Output - Blower forward.
						DO_3 <= OC_3;	// Output - Not in use. 
						DO_4 <= OC_4;	// Output - Not in use. 
						DO_5 <= OC_5;	// Output - Not in use. 
						interrupt <= `ZERO;
					end
					LIFT_PB_ON_ERROR:				
					begin
						/* Check the lift bar protection */
						if(Enable_Printbar_Down(DI_0, DI_1, DI_2, DI_3, DI_4, DI_5, GI_2))
						begin
							Timer_Trig <= `ZERO; 							
							interrupt <= `ZERO;
						end
						else
						begin
							Timer_Trig <= `ONE; 
							interrupt <= `ONE;
						end
						
						DO_0 <= OC_0;	// Output - Not in use.
						DO_1 <= ~Timer_State[0];
						DO_2 <= OC_2;	     // Output - Tub engage valve.
						DO_3 <= OC_3;		 // Output - Squeegee engage valve.
						DO_4 <= OC_4;		 // Output - Squeegee disengage valve.
						DO_5 <= OC_5;		 // Output - Squeegee air Knife valve.
					end
					LIFT_PB_ON_ERROR_GEN_2:
					begin
						/* Check the lift bar protection */
						if(Enable_Printbar_Down_Gen2(DI_0, DI_1, DI_2, DI_3, GI_0, GI_2))
						begin
							Timer_Trig <= `ZERO; 							
							interrupt <= `ZERO;
						end
						else
						begin
							Timer_Trig <= `ONE; 
							interrupt <= `ONE;
						end
						
						DO_0 <= OC_0;	// Output - Not in use.
						DO_1 <= ~Timer_State[0];
						DO_2 <= OC_2;	     // Output - Tub engage valve.
						DO_3 <= OC_3;		 // Output - Squeegee engage valve.
						DO_4 <= OC_4;		 // Output - Squeegee disengage valve.
						DO_5 <= OC_5;		 // Output - Squeegee air Knife valve.
					end
					SOFT_DEBUG:
					begin
						DO_0 <= OC_0;	// Output - Unconditional Pass.
						DO_1 <= OC_1;	// Output - Unconditional Pass.
						DO_2 <= OC_2;	// Output - Unconditional Pass.
						DO_3 <= OC_3;	// Output - Unconditional Pass.
						DO_4 <= OC_4;	// Output - Unconditional Pass.
						DO_5 <= OC_5;	// Output - Unconditional Pass.
						interrupt <= `ZERO;
					end				
					HW_VALIDATION:
					begin
						DO_0 <= OC_0;	// Output - Unconditional Pass.
						DO_1 <= OC_1;	// Output - Unconditional Pass.
						DO_2 <= OC_2;	// Output - Unconditional Pass.
						DO_3 <= OC_3;	// Output - Unconditional Pass.
						DO_4 <= OC_4;	// Output - Unconditional Pass.
						DO_5 <= OC_5;	// Output - Unconditional Pass.
						interrupt <= `ZERO;					
					end
					APPLICATOR:
					begin
						DO_0 <= OC_0;	// Output - Unconditional Pass.
						DO_1 <= OC_1;	// Output - Unconditional Pass.
						DO_2 <= OC_2;	// Output - Unconditional Pass.
						DO_3 <= OC_3;	// Output - Unconditional Pass.
						DO_4 <= OC_4;	// Output - Unconditional Pass.
						DO_5 <= OC_5;	// Output - Unconditional Pass.
						interrupt <= `ZERO;					
					end
					LIFT_PB_ON_ERROR_GEN_3:
					begin
						DO_0 <= OC_0;	// Output - Unconditional Pass.
						DO_1 <= OC_1;	// Output - Unconditional Pass.
						DO_2 <= OC_2;	// Output - Unconditional Pass.
						DO_3 <= OC_3;	// Output - Unconditional Pass.
						DO_4 <= OC_4;	// Output - Unconditional Pass.
						DO_5 <= OC_5;	// Output - Unconditional Pass.
						interrupt <= `ZERO;					
					end
				endcase
				////////////////////////////////////////////////////////////////////////////////
				
				
				/////////////////////////
				// Timer State machine //
				/////////////////////////
				////////////////////////////////////////////
				case(Timer_State)
					(RELOAD):
					begin
						if(Timer_Trig)
						begin
							Timer_State <= COUNT_DOWN;
							//interrupt <= `ONE;
						end
						else
							Timer_State <= RELOAD;
					end
					COUNT_DOWN:
					begin
						//interrupt <= `ZERO;
						if(tc)
							Timer_State <= RELOAD;
						else
						begin
							Timer_State <= COUNT_DOWN;	
							//Timer_Trig <= 1'b0;
						end
					end
				endcase
				////////////////////////////////////////
			end	
		end		
	end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////
	// Insantiation of an 8 bit one-shot Timer //
	/////////////////////////////////////////////
	
	cy_psoc3_dp8 #(	.a0_init_a(0), .a1_init_a(0), .d0_init_a((Timer_int_value)), 
					.d1_init_a(0), 
					.cy_dpconfig_a({
								    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
								    `CS_SHFT_OP_PASS, `CS_A0_SRC___D0, `CS_A1_SRC_NONE,
								    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
								    `CS_CMP_SEL_CFGA, /*CFGRAM0:   RELOAD*/
								    `CS_ALU_OP__DEC, `CS_SRCA_A0, `CS_SRCB_D0,
								    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
								    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
								    `CS_CMP_SEL_CFGA, /*CFGRAM1:   COUNT_DOWN*/
								    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
								    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
								    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
								    `CS_CMP_SEL_CFGA, /*CFGRAM2:   */
								    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
								    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
								    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
								    `CS_CMP_SEL_CFGA, /*CFGRAM3:   */
								    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
								    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
								    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
								    `CS_CMP_SEL_CFGA, /*CFGRAM4:   */
								    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
								    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
								    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
								    `CS_CMP_SEL_CFGA, /*CFGRAM5:   */
								    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
								    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
								    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
								    `CS_CMP_SEL_CFGA, /*CFGRAM6:   */
								    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
								    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
								    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
								    `CS_CMP_SEL_CFGA, /*CFGRAM7:   */
								    8'hFF, 8'h00,  /*CFG9:   */
								    8'hFF, 8'hFF,  /*CFG11-10:   */
								    `SC_CMPB_A1_D1, `SC_CMPA_A1_D1, `SC_CI_B_ARITH,
								    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
								    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
								    `SC_SI_A_DEFSI, /*CFG13-12:   */
								    `SC_A0_SRC_ACC, `SC_SHIFT_SL, 1'h0,
								    1'h0, `SC_FIFO1_BUS, `SC_FIFO0_BUS,
								    `SC_MSB_DSBL, `SC_MSB_BIT0, `SC_MSB_NOCHN,
								    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
								    `SC_CMP0_NOCHN, /*CFG15-14:   */
								    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
								    `SC_FIFO_LEVEL,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
								    `SC_WRK16CAT_DSBL /*CFG17-16:   */
									})
					) 
	Timer8(
	        /*  input                   */  .reset(1'b0),
	        /*  input                   */  .clk(Clock),
	        /*  input   [02:00]         */  .cs_addr(Timer_State),
	        /*  input                   */  .route_si(1'b0),
	        /*  input                   */  .route_ci(1'b0),
	        /*  input                   */  .f0_load(1'b0),
	        /*  input                   */  .f1_load(1'b0),
	        /*  input                   */  .d0_load(1'b0),
	        /*  input                   */  .d1_load(1'b0),
	        /*  output                  */  .ce0(),
	        /*  output                  */  .cl0(),
	        /*  output                  */  .z0(tc),
	        /*  output                  */  .ff0(),
	        /*  output                  */  .ce1(),
	        /*  output                  */  .cl1(),
	        /*  output                  */  .z1(),
	        /*  output                  */  .ff1(),
	        /*  output                  */  .ov_msb(),
	        /*  output                  */  .co_msb(),
	        /*  output                  */  .cmsb(),
	        /*  output                  */  .so(),
	        /*  output                  */  .f0_bus_stat(),
	        /*  output                  */  .f0_blk_stat(),
	        /*  output                  */  .f1_bus_stat(),
	        /*  output                  */  .f1_blk_stat()
	);	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	
	
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	// logic function for the EMR - Lift PBs signal (To RTC) //
	///////////////////////////////////////////////////////////
	function Enable_Printbar_Down;
		
		input	Tub_engage_pistons_limit_switch_1,	// DI_0
				Tub_engage_pistons_limit_switch_2,	// DI_1
				Tub_engage_pistons_limit_switch_3,	// DI_2
				Tub_engage_pistons_limit_switch_4,	// DI_3
				
				Squeegee_proximity_sensor_front,	// DI_4
				Squeegee_proximity_sensor_rear,		// DI_5
				
				Air_presure_indication;				// GI_2	
		
		reg Squeegee_Engaged;
		reg Tub_Engaged;
		
		begin
			Tub_Engaged      =  Tub_engage_pistons_limit_switch_1 
						     || Tub_engage_pistons_limit_switch_2
						     || Tub_engage_pistons_limit_switch_3
						     || Tub_engage_pistons_limit_switch_4;
							 
			Squeegee_Engaged =  Squeegee_proximity_sensor_front
							 && Squeegee_proximity_sensor_rear;
							 
			Enable_Printbar_Down = (Air_presure_indication && Squeegee_Engaged) || (!Tub_Engaged);
		end
		
	endfunction
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	// logic function for the EMR - Lift PBs signal (To RTC) //
	///////////////////////////////////////////////////////////
	function Enable_Printbar_Down_Gen2;
		
		input	Tub_disengage_pistons_limit_switch_1,	// DI_0
				Tub_engage_pistons_limit_switch_1,		// DI_1
				Tub_disengage_pistons_limit_switch_2,	// DI_2
				Tub_engage_pistons_limit_switch_2,		// DI_3
				
				Blanket_Moving,						// GI_0
				Air_presure_indication;				// GI_2	
		
		begin
		//y = A.B + A.C.D.E.F + A.C.D.E.F
		// A: Air Pressure
		// B: Blanket Moving
		// C,D: Engage
		// E,F: Disengage
			Enable_Printbar_Down_Gen2 = (Air_presure_indication && !Blanket_Moving) ||
									(Air_presure_indication && !Tub_engage_pistons_limit_switch_1 && !Tub_engage_pistons_limit_switch_2 && Tub_disengage_pistons_limit_switch_1 && Tub_disengage_pistons_limit_switch_2) ||
									(Air_presure_indication && Tub_engage_pistons_limit_switch_1 && Tub_engage_pistons_limit_switch_2 && !Tub_disengage_pistons_limit_switch_1 && !Tub_disengage_pistons_limit_switch_2);
		end
		
	endfunction

//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line




