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

#include "bootloader.h"

//Application region of flash starts at the beginning of external SPI flash
//Firmware version string is put right after vector table by linker script at a constant address
uint32_t* const g_appVector  = reinterpret_cast<uint32_t*>(0x9000'0000);

//Offset of the version string (size of the vector table plus 32 byte alignment)
const uint32_t g_appVersionOffset = 0x2e0;

///@brief Output stream for local serial console
UARTOutputStream g_localConsoleOutputStream;

extern bool g_bootAppPending;
extern uint32_t g_bootAppTimer;

///@brief Number of sectors in the image
const uint32_t g_flashSectorCount = 5;

///@brief Size of a sector
const uint32_t g_flashSectorSize = 128 * 1024;

///@brief Size of the image
const uint32_t g_appImageSize = g_flashSectorCount * g_flashSectorSize;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hooks called by bootloader code

void Bootloader_Init()
{
	g_log("Bootloader init\n");

	RTC::Unlock();
}

void Bootloader_ClearRxBuffer()
{
}

void Bootloader_FinalCleanup()
{
	g_cliUART.Flush();
}

/*
void __attribute__((noreturn)) BSP_MainLoop()
{
	Bootloader_MainLoop();
	while(1)
	{}
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Run the firmware updater

void __attribute__((noreturn)) Bootloader_FirmwareUpdateFlow()
{
	g_log("In DFU mode\n");
	while(1)
	{}
}
