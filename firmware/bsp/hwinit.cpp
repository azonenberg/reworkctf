/***********************************************************************************************************************
*                                                                                                                      *
* reworkctf                                                                                                            *
*                                                                                                                      *
* Copyright (c) 2023-2026 Andrew D. Zonenberg and contributors                                                         *
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

/**
	@file
	@author	Andrew D. Zonenberg
	@brief	Boot-time hardware initialization
 */
#include <core/platform.h>
#include "hwinit.h"
#include <peripheral/QuadSPI.h>

/*
#include <peripheral/DWT.h>
#include <peripheral/ITM.h>
#include <embedded-utils/CoreSightRom.h>
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common peripherals used by application and bootloader

/**
	@brief UART console

	Default after reset is for UART1 to be clocked by PCLK2 (APB2 clock) which is 118.75 MHz
	So we need a divisor of 1030.81
 */
UART<32, 256> g_cliUART(&USART1, 1031);

/**
	@brief MCU GPIO LEDs
 */
GPIOPin g_leds[16] =
{
	GPIOPin(&GPIOH, 13, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//0
	GPIOPin(&GPIOA, 11, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//1
	GPIOPin(&GPIOA, 12, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//2
	GPIOPin(&GPIOG, 2, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//3

	GPIOPin(&GPIOK, 2, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//4
	GPIOPin(&GPIOA, 8, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//5
	GPIOPin(&GPIOC, 9, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//6
	GPIOPin(&GPIOG, 8, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//7

	GPIOPin(&GPIOC, 6, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//8
	GPIOPin(&GPIOG, 6, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//9
	GPIOPin(&GPIOG, 5, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//10
	GPIOPin(&GPIOG, 4, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//11

	GPIOPin(&GPIOK, 1, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//12
	GPIOPin(&GPIOG, 3, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//13
	GPIOPin(&GPIOK, 0, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),	//14
	GPIOPin(&GPIOJ, 11, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW)	//15
};

/**
	@brief Microkvs and firmware storage for the MCU

	Initialize the QSPI block assuming 128 Mbits to start
	QSPI kernel clock defaults to HCLK3 which is 237.5 MHz
	Divide by 4 gives 59.375 MHz which should be fairly safe to start
 */
//QuadSPI_SpiFlashInterface g_flashQspi(&_QUADSPI, 128 * 1024 * 1024, 4);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Task tables

etl::vector<Task*, MAX_TASKS>  g_tasks;
etl::vector<TimerTask*, MAX_TIMER_TASKS>  g_timerTasks;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do other initialization

void InitITM();
void InitQSPI();

void BSP_InitTasks();

void GradeChallenges();
void GradeChallenge1();
void GradeChallenge2();
void GradeChallenge3();
void GradeChallenge4();
void GradeChallenge5();
void GradeChallenge6();
void GradeChallenge7();
void GradeChallenge8();
void GradeChallenge9();
void GradeChallenge10();
void GradeChallenge11();
void GradeChallenge12();
void GradeChallenge13();
void GradeChallenge14();
void GradeChallenge15();
void GradeChallenge16();

void BSP_Init()
{
	//Set up PLL2 to run the external memory bus
	//We have some freedom with how fast we clock this!
	//Doesn't have to be a multiple of CPU clock since separate VCO from the main system
	RCCHelper::InitializePLL(
		2,		//PLL2
		25,		//input is 25 MHz from the HSE
		2,		//25/2 = 12.5 MHz at the PFD
		20,		//12.5 * 20 = 250 MHz at the VCO
		32,		//div P (not used for now)
		32,		//div Q (not used for now)
		2,		//div R (125 MHz FMC kernel clock = 62.5 MHz FMC clock)
		RCCHelper::CLOCK_SOURCE_HSE
	);

	//Turn off all of the LEDs
	for(int i=0; i<16; i++)
		g_leds[i] = 0;

	InitRTCFromHSE();

	GradeChallenges();

	/*
	InitQSPI();
	DoInitKVS();
	//InitITM();
	*/

	//Standard tasks used by both bootloader and application
	BSP_InitTasks();

	App_Init();
}

void BSP_InitMemory()
{
}

void BSP_MainLoopIteration()
{
}

void BSP_InitTasks()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enable trace

/**
	@brief Initialize tracing
 */
void InitITM()
{
	/*
	g_log("Initializing ITM\n");

	//Enable ITM, enable PC sampling, and turn on forwarding to the TPIU
	#ifdef _DEBUG
		ITM::Enable();
		DWT::EnablePCSampling(DWT::PC_SAMPLE_SLOW);
		ITM::EnableDwtForwarding();
	#endif
	*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BSP overrides for low level init

void BSP_InitUART()
{
	//Initialize the UART for local console: 115.2 Kbps using PA9 for UART1 transmit and PA10 for UART1 receive
	//TODO: nice interface for enabling UART interrupts
	static GPIOPin uart_tx(&GPIOA, 9, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 7);
	static GPIOPin uart_rx(&GPIOA, 10, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 7);

	//Enable the UART interrupt
	NVIC_EnableIRQ(37);

	g_logTimer.Sleep(10);	//wait for UART pins to be high long enough to remove any glitches during powerup
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Higher level initialization we used for a lot of stuff

void InitQSPI()
{
	/*
	g_log("Initializing QSPI...\n");

	//Set up QSPI pins
	g_leds[1] = 1;
	static GPIOPin quadspi_sck(&GPIOB, 2, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_FAST, 9);
	static GPIOPin quadspi_cs_n(&GPIOB, 6, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_FAST, 10);
	static GPIOPin quadspi_dq0(&GPIOF, 8, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_FAST, 10);
	static GPIOPin quadspi_dq1(&GPIOF, 9, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_FAST, 10);
	static GPIOPin quadspi_dq2(&GPIOF, 7, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_FAST, 9);
	static GPIOPin quadspi_dq3(&GPIOF, 6, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_FAST, 9);

	//Bring the QSPI up in memory mapped mode
	g_leds[2] = 1;
	g_flashQspi.SetDoubleRateMode(false);
	g_flashQspi.SetInstructionMode(QuadSPI::MODE_SINGLE);
	g_flashQspi.SetAddressMode(QuadSPI::MODE_SINGLE, 3);
	g_flashQspi.SetAltBytesMode(QuadSPI::MODE_NONE, 0);
	g_flashQspi.SetDataMode(QuadSPI::MODE_SINGLE);
	g_flashQspi.SetDummyCycleCount(8);
	g_flashQspi.SetDeselectTime(1);
	g_flashQspi.SetFifoThreshold(1);
	g_flashQspi.Enable();
	g_leds[3] = 1;

	//break point in case of issues
	g_logTimer.Sleep(5000);

	g_flashQspi.Discover();

	//fail to detect flash if size is implausibly small
	if(g_flashQspi.GetFlashSize() < 4096 )
	{
		g_log(Logger::ERROR, "Failed to detect SPI flash\n");
		while(1)
		{}
	}

	g_flashQspi.MemoryMap();
	*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check each of the challenges

void GradeChallenges()
{
	g_log("Grading challenges...\n");
	LogIndenter li(g_log);

	GradeChallenge1();
	GradeChallenge2();
	GradeChallenge3();
	GradeChallenge4();
	GradeChallenge5();
	GradeChallenge6();
	GradeChallenge7();
	GradeChallenge8();
	GradeChallenge9();
	GradeChallenge10();
	GradeChallenge11();
	GradeChallenge12();
	GradeChallenge13();
	GradeChallenge14();
	GradeChallenge15();
	GradeChallenge16();
}

void GradeChallenge1()
{
	//Challenge 1: short between test signal and ground. Pull weakly high and see how it reads
	GPIOPin challenge_01(&GPIOA, 3, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	challenge_01.SetPullMode(GPIOPin::PULL_UP);
	g_logTimer.Sleep(1);
	if(challenge_01)
	{
		g_log("[ 1]: OK \n");
		g_leds[0] = 1;
	}
	else
		g_log(Logger::ERROR, "[ 1]: FAIL \n");
}

void GradeChallenge2()
{
	//Challenge 2: Two signals shorted together.
	//Verify they are not broken, and connect together properly
	GPIOPin challenge_02_a_1(&GPIOA, 4, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_02_a_2(&GPIOB, 0, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_02_b_1(&GPIOB, 1, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_02_b_2(&GPIOC, 5, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);

	//Check for breaks: Pull 1 end of each high and float 2 end, make sure 2 end reads high.
	challenge_02_a_1.SetPullMode(GPIOPin::PULL_UP);
	challenge_02_b_1.SetPullMode(GPIOPin::PULL_UP);
	g_logTimer.Sleep(1);
	if(!challenge_02_a_2 || !challenge_02_b_2)
	{
		g_log(Logger::ERROR, "[ 2]: FAIL \n");
		return;
	}

	//Repeat with low at the 1 end
	challenge_02_a_1.SetPullMode(GPIOPin::PULL_DOWN);
	challenge_02_b_1.SetPullMode(GPIOPin::PULL_DOWN);
	g_logTimer.Sleep(1);
	if(challenge_02_a_2 || challenge_02_b_2)
	{
		g_log(Logger::ERROR, "[ 2]: FAIL \n");
		return;
	}

	//Now that we know both are unbroken, drive A1 strongly high and pull B2 weakly low and check for a short
	challenge_02_a_1.SetMode(GPIOPin::MODE_OUTPUT);
	challenge_02_a_1 = 1;
	challenge_02_b_1.SetPullMode(GPIOPin::PULL_DOWN);
	if(challenge_02_b_2)
	{
		g_log(Logger::ERROR, "[ 2]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[ 2]: OK \n");
	g_leds[1] = 1;
}

void GradeChallenge3()
{
	//Challenge 3: LED is backwards
	g_log("[ 3]: Not software gradeable, look at the LED \n");
	g_leds[2] = 1;
}

void GradeChallenge4()
{
	//Just check for continuity through the resistor
	GPIOPin challenge_04(&GPIOJ, 3, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	challenge_04.SetPullMode(GPIOPin::PULL_DOWN);
	g_logTimer.Sleep(1);

	if(!challenge_04)
	{
		g_log(Logger::ERROR, "[ 4]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[ 4]: OK \n");
	g_leds[3] = 1;
}

void GradeChallenge5()
{
	//Just check for continuity through the resistor
	GPIOPin challenge_05(&GPIOJ, 4, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	challenge_05.SetPullMode(GPIOPin::PULL_DOWN);
	g_logTimer.Sleep(1);

	if(!challenge_05)
	{
		g_log(Logger::ERROR, "[ 5]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[ 5]: OK \n");
	g_leds[4] = 1;
}

void GradeChallenge6()
{
	//no-op, need to do 7
}

void GradeChallenge7()
{
	g_log("[ 6]: FIXME need to implement qspi test \n");
	g_log("[ 7]: FIXME need to implement qspi test \n");
	//g_leds[5] = 1;
	//g_leds[6] = 1;
}

void GradeChallenge8()
{
	//Just check for continuity through the break
	GPIOPin challenge_08_a(&GPIOG, 1, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_08_b(&GPIOF, 11, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	challenge_08_a = 1;
	challenge_08_b.SetPullMode(GPIOPin::PULL_DOWN);
	g_logTimer.Sleep(1);

	if(!challenge_08_b)
	{
		g_log(Logger::ERROR, "[ 8]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[ 8]: OK \n");
	g_leds[7] = 1;
}

void GradeChallenge9()
{
	//Challenge 9: Inner layer diffpair swap
	//Verify they are not broken, and connect together properly
	GPIOPin challenge_09_p_1(&GPIOE, 14, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_09_p_2(&GPIOE, 9, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_09_n_1(&GPIOE, 7, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_09_n_2(&GPIOF, 14, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);

	//Check for breaks: Pull 1 end of each high and float 2 end, make sure 2 end reads high.
	challenge_09_p_1.SetPullMode(GPIOPin::PULL_UP);
	challenge_09_n_1.SetPullMode(GPIOPin::PULL_UP);
	g_logTimer.Sleep(1);
	if(!challenge_09_p_2 || !challenge_09_n_2)
	{
		g_log(Logger::ERROR, "[ 9]: FAIL \n");
		return;
	}

	//Repeat with low at the 1 end
	challenge_09_p_1.SetPullMode(GPIOPin::PULL_DOWN);
	challenge_09_n_1.SetPullMode(GPIOPin::PULL_DOWN);
	g_logTimer.Sleep(1);
	if(challenge_09_p_2 || challenge_09_n_2)
	{
		g_log(Logger::ERROR, "[ 9]: FAIL \n");
		return;
	}

	//Now that we know both are unbroken, drive A1 strongly high and pull B2 weakly low and check for a short
	challenge_09_p_1.SetMode(GPIOPin::MODE_OUTPUT);
	challenge_09_p_1 = 1;
	challenge_09_n_1.SetPullMode(GPIOPin::PULL_DOWN);
	if(challenge_09_n_2)
	{
		g_log(Logger::ERROR, "[ 9]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[ 9]: OK \n");
	g_leds[8] = 1;
}

void GradeChallenge10()
{
	//Just check for continuity through the break
	GPIOPin challenge_10_a(&GPIOF, 15, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_10_b(&GPIOE, 12, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	challenge_10_a = 1;
	challenge_10_b.SetPullMode(GPIOPin::PULL_DOWN);
	g_logTimer.Sleep(1);

	if(!challenge_10_b)
	{
		g_log(Logger::ERROR, "[10]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[10]: OK \n");
	g_leds[9] = 1;
}

void GradeChallenge11()
{
	//Challenge 11: via top to bottom shorted to inner layer Vdd fill.
	//First, check for shorts from each to Vdd
	GPIOPin challenge_11_a(&GPIOH, 7, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_11_b(&GPIOH, 8, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	challenge_11_a.SetPullMode(GPIOPin::PULL_DOWN);
	challenge_11_b.SetPullMode(GPIOPin::PULL_DOWN);
	g_logTimer.Sleep(1);
	if(challenge_11_a || challenge_11_b)
	{
		g_log(Logger::ERROR, "[11]: FAIL \n");
		return;
	}

	//Then check for continuity
	challenge_11_a.SetMode(GPIOPin::MODE_OUTPUT);
	challenge_11_a = 1;
	if(!challenge_11_b)
	{
		g_log(Logger::ERROR, "[11]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[11]: OK \n");
	g_leds[10] = 1;
}

void GradeChallenge12()
{
	//Challenge 12: bad reflow on BGA balls. Verify continuity
	GPIOPin challenge_12_a(&GPIOI, 5, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_12_b(&GPIOI, 6, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	challenge_12_a = 1;
	challenge_12_b.SetPullMode(GPIOPin::PULL_DOWN);
	g_logTimer.Sleep(1);

	if(!challenge_12_b)
	{
		g_log(Logger::ERROR, "[12]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[12]: OK \n");
	g_leds[11] = 1;
}

void GradeChallenge13()
{
	//Challenge 13: Missing dogbone trace. Verify continuity
	GPIOPin challenge_13_a(&GPIOB, 12, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_13_b(&GPIOB, 13, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	challenge_13_a = 1;
	challenge_13_b.SetPullMode(GPIOPin::PULL_DOWN);
	g_logTimer.Sleep(1);

	if(!challenge_13_b)
	{
		g_log(Logger::ERROR, "[13]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[13]: OK \n");
	g_leds[12] = 1;
}

void GradeChallenge14()
{
	//Challenge 14: Missing dogbone trace. Verify continuity
	GPIOPin challenge_14_a(&GPIOB, 10, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
	GPIOPin challenge_14_b(&GPIOH, 9, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	challenge_14_a = 1;
	challenge_14_b.SetPullMode(GPIOPin::PULL_DOWN);
	g_logTimer.Sleep(1);

	if(!challenge_14_b)
	{
		g_log(Logger::ERROR, "[14]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[14]: OK \n");
	g_leds[13] = 1;
}

void GradeChallenge15()
{
	//Challenge 15: Missing pullup
	//Just check for continuity through the resistor
	GPIOPin challenge_15(&GPIOC, 13, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	challenge_15.SetPullMode(GPIOPin::PULL_DOWN);
	g_logTimer.Sleep(1);

	if(!challenge_15)
	{
		g_log(Logger::ERROR, "[15]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[15]: OK \n");
	g_leds[14] = 1;
}

void GradeChallenge16()
{
	//Challenge 16: Pin strapped to ground rather than VDD
	GPIOPin challenge_16(&GPIOD, 12, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);

	if(!challenge_16)
	{
		g_log(Logger::ERROR, "[16]: FAIL \n");
		return;
	}

	//If we get here, all good
	g_log("[16]: OK \n");
	g_leds[15] = 1;
}
