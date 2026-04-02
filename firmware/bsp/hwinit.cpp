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
GPIOPin g_leds[4] =
{
	GPIOPin(&GPIOH, 13, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),
	GPIOPin(&GPIOA, 11, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),
	GPIOPin(&GPIOA, 12, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),
	GPIOPin(&GPIOG, 2, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW)
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
	for(int i=0; i<4; i++)
		g_leds[i] = 1;

	InitRTCFromHSE();

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
