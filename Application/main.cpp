/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    main.cpp
 * @brief   Application entry point.
 */
#include "clock_config.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "App.h"


/*******************************************************************************

 * Prototypes

 ******************************************************************************/


/*!

 * @brief Task responsible for printing of "Hello world." message.

 */
extern "C" {
void HardFault_Handler(void) {
    __asm volatile (
            " movs r0,#4                  \n"  /* load bit mask into R0 */
            " mov r1, lr                  \n"  /* load link register into R1 */
            " tst r0, r1                  \n"  /* compare with bitmask */
            " beq _MSP                    \n"  /* if bitmask is set: stack pointer is in PSP. Otherwise in MSP */
            " mrs r0, psp                 \n"  /* otherwise: stack pointer is in PSP */
            " b _HALT                    \n"  /* go to part which loads the PC */
            "_MSP:                          \n"  /* stack pointer is in MSP register */
            " mrs r0, msp                 \n"  /* load stack pointer into R0 */
            "_HALT:                        \n"  /* find out where the hard fault happened */
            " ldr r1,[r0,#24]             \n"  /* load program counter into R1. R1 contains address of the next instruction where the hard fault happened */
            " bkpt #0                     \n" /* cause the debugger to stop */
            );
}

}

/*

 * @brief   Application entry point.

 */

int main() {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    // NVIC_SetPriority(static_cast<IRQn_Type>(DMA0_IRQn + LPUART0_RX_DMA_CHANNEL), LPSPI0_IRQ_PRIORITY + 1);
    // NVIC_SetPriority(static_cast<IRQn_Type>(DMA0_IRQn + LPUART0_TX_DMA_CHANNEL), LPSPI0_IRQ_PRIORITY + 1);
    App();
    vTaskStartScheduler();
    /* Enter an infinite loop, just incrementing a counter. */

    while (true) {
    }

    return 0;
}
