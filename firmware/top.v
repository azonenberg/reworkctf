`default_nettype none
`timescale 1ns/1ps
/***********************************************************************************************************************
*                                                                                                                      *
* REWORKCTF v0.1                                                                                                       *
*                                                                                                                      *
* Copyright (c) 2017 Andrew D. Zonenberg                                                                               *
* All rights reserved.                                                                                                 *
*                                                                                                                      *
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the     *
* following conditions are met:                                                                                        *
*                                                                                                                      *
*    * Redistributions of source code must retain the above copyright notice, this list of conditions, and the         *
*      following disclaimer.                                                                                           *
*                                                                                                                      *
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       *
*      following disclaimer in the documentation and/or other materials provided with the distribution.                *
*                                                                                                                      *
*    * Neither the name of the author nor the names of any contributors may be used to endorse or promote products     *
*      derived from this software without specific prior written permission.                                           *
*                                                                                                                      *
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   *
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL *
* THE AUTHORS BE HELD LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES        *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR       *
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                                                                          *
*                                                                                                                      *
***********************************************************************************************************************/

module top(

	input wire			clk_1mhz,

	output reg			level1_test		= 0,
	input wire			level1_sense,

	output reg			level3_test		= 0,
	input wire			level3_sense,

	output reg			level4_test		= 0,
	input wire			level4_sense,

	output reg			level5_test		= 0,
	input wire			level5_sense,

	output reg			level6_test		= 0,
	input wire			level6_sense,

	output reg			level7_test_p	= 0,
	output reg			level7_test_n	= 0,

	input wire			level7_sense_p,
	//input wire			level7_sense_n_,

	output reg			level8_test		= 0,
	input wire			level8_sense,

	output reg			level9_test		= 0,
	input wire			level9_sense,

	//output wire			level10_test,
	input wire			level10_sense,

	//output wire			level11_test,
	input wire			level11_sense,

	output reg			level12_test,
	input wire			level12_sense,

	output reg			level13_test_a		= 0,
	output reg			level13_test_b		= 0,
	input wire			level13_sense_a,
	input wire			level13_sense_b,

	output reg			level14_test		= 0,
	input wire			level14_sense,

	output wire			level15_test_p,
	output wire			level15_test_n,

	input wire			level15_sense_p,
	input wire			level15_sense_n,

	output reg			level16_test_a		= 0,
	output reg			level16_test_b		= 0,
	input wire			level16_sense_a,
	//input wire			level16_sense_b,

	output reg			level17_test		= 0,
	input wire			level17_sense,

	output reg			level18_test		= 0,
	input wire			level18_sense,

	output reg			level19_test_a		= 0,
	output reg			level19_test_b		= 0,
	input wire			level19_sense_a,
	input wire			level19_sense_b,

	input wire			level20_sense,

	output reg			level21_test		= 0,
	input wire			level21_sense,

	output reg			level22_test		= 0,
	input wire			level22_sense,

	output reg[21:0]	level_ok
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// I/O BUFFERS

	wire				level15_sense;
	reg					level15_test = 0;

	IBUFDS #(
		.IBUF_DELAY_VALUE("0"),
		.IFD_DELAY_VALUE("AUTO"),
	.IOSTANDARD("LVDS_25")
	) level15_sense_ibuf (
		.O(level15_sense),
		.I(level15_sense_p),
		.IB(level15_sense_n)
	);

	OBUFDS #(
		.IOSTANDARD("LVDS_25")
	) level15_test_obuf (
		.O(level15_test_p),
		.OB(level15_test_n),
		.I(level15_test)
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TEST LOGIC

	//Turn on all outputs by default
	initial begin
		level_ok	<= 22'h3FFFFF;
	end

	//Probe each level in sequence, but test everything
	reg[4:0] current_level		= 0;
	reg[18:0] count 			= 0;
	always @(posedge clk_1mhz) begin

		count					<= count + 1'h1;
		current_level			<= current_level + 1'h1;

		//Sweep test signals one at a time, so that shorts to adjacent lines are detectable
		//(also minimizes SSO)
		case(current_level)

			1:	level1_test 	<= ~level1_test;
			2:	level3_test	 	<= ~level3_test;
			3:	level4_test 	<= ~level4_test;
			4:	level5_test 	<= ~level5_test;
			5:	level6_test 	<= ~level6_test;
			6: begin
				level7_test_p	<= ~level7_test_p;
				level7_test_n	<= level7_test_p;
			end
			7:	level8_test 	<= ~level8_test;

			11:	level12_test	<= ~level12_test;

			12:	begin
				level13_test_a	<= ~level13_test_a;
				level13_test_b	<= level13_test_b;
			end

			13:	level14_test 	<= ~level14_test;

			14:	level15_test	<= ~level15_test;

			15:	begin
				level16_test_a	<= ~level16_test_a;
				level16_test_b	<= level16_test_a;
			end

			16:	level17_test	<= ~level17_test;

			17:	level18_test	<= ~level18_test;

			18:	begin
				level19_test_a	<= ~level19_test_a;
				level19_test_b	<= level19_test_a;
			end

			20:	level21_test	<= ~level21_test;

			21:	level22_test	<= ~level22_test;

		endcase

		//LEVEL 1: Path through inverter shorts to Vdd (should invert)
		if(level1_sense != !level1_test)
			level_ok[0]		<= 1'b0;

		//LEVEL 2: LED is backwards, always drive it high
		level_ok[1]		<= 1'b1;

		//LEVEL 3: Missing input pad on inverter
		if(level3_sense != !level3_test)
			level_ok[2]		<= 1'b0;

		//LEVEL 4: Missing resistor
		if(level4_test != !level4_sense)
			level_ok[3]		<= 1'b0;

		//LEVEL 5: Short to level 1
		if(level5_test != !level5_sense)
			level_ok[4]		<= 1'b0;

		//LEVEL 6: Short to level 5
		if(level6_test != !level6_sense)
			level_ok[5]		<= 1'b0;

		//LEVEL 7: Diffpair swap
		if(level7_test_p != level7_sense_p)
			level_ok[6]		<= 1'b0;

		//LEVEL 8: Missing input pad on inverter
		if(level8_sense != !level8_test)
			level_ok[7]		<= 1'b0;

		//LEVEL 9: Missing resistor
		if(level9_test != !level9_sense)
			level_ok[8]		<= 1'b0;

		//LEVEL 10: Cannot test b/c derpy pinout
		level_ok[9]			<= 1'b0;

		//LEVEL 11: Cannot test b/c derpy pinout
		level_ok[10]		<= 1'b0;

		//LEVEL 12: Missing drill on inverter input via
		if(level12_sense != !level12_test)
			level_ok[11]	<= 1'b0;

		//LEVEL 13: Inner layer short (verify both lines are passthrough)
		if( (level13_sense_a != level13_test_a) || (level13_sense_b != level13_test_b) )
			level_ok[12]	<= 1'b0;

		//LEVEL 14: Missing dogbone connection
		if(level14_sense != !level14_test)
			level_ok[13]	<= 1'b0;

		//LEVEL 15: Diffpair swap on inner layer
		if(level15_test != level15_sense)
			level_ok[14]	<= 1'b0;

		//LEVEL 16: Internal layer short (verify both lines are passthrough)
		if( (level16_sense_a != level16_test_a)/* || (level16_sense_b != level16_test_b) */)
			level_ok[15]	<= 1'b0;

		//LEVEL 17: Signal shorted to ground
		if(level17_test != level17_sense)
			level_ok[16]	<= 1'b0;

		//LEVEL 18: Dogbone via missing drill
		if(level18_test != level18_sense)
			level_ok[17]	<= 1'b0;

		//LEVEL 19: Blind via fun
		if( (level19_sense_a != level19_test_a) || (level19_sense_b != level19_test_b) )
			level_ok[18]	<= 1'b0;

		//LEVEL 20: strap pin is high, should be low
		if(level20_sense != 0)
			level_ok[19]	<= 1'b0;

		//LEVEL 21: Internal layer short with congestion on both sides
		if(level21_test != level21_sense)
			level_ok[20]	<= 1'b0;

		//LEVEL 22: Dogbone via missing drill
		if(level22_test != level22_sense)
			level_ok[21]	<= 1'b0;

		//Reset LEDs so glitches dont keep us off forever
		//if(count == 0)
		//	level_ok		<= 22'h3FFFFF;

	end

endmodule
