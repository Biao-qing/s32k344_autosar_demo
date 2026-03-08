/* Copyright 2020, 2022-2023, 2025 NXP */
/* License: BSD 3-clause
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "S32K344.h"

#include <stdio.h>

/* AUTOSAR headers */
#include "Platform_Types.h"
#include "Std_Types.h"
#include "Os.h"
#include "EcuM.h"
#include "Rte.h"
#include "Rte_Main.h"

/* Compiler pragma to suppress unreachable code warning */
#if defined (__GNUC__)
    #define __NO_RETURN__ __attribute__((__noreturn__))
#else
    #define __NO_RETURN__
#endif

/**********************************************************************************************************************
 *  TASK IMPLEMENTATIONS
 *********************************************************************************************************************/

/**
 * @brief Default Init Task
 */
TASK(Default_Init_Task)
{
    /* Task initialization */
    for (;;) {
        /* Task main loop - terminate if not needed */
        TerminateTask();
    }
}

/**
 * @brief Default Init Task (Trusted)
 */
TASK(Default_Init_Task_Trusted)
{
    /* Task initialization */
    for (;;) {
        /* Task main loop - terminate if not needed */
        TerminateTask();
    }
}

/**********************************************************************************************************************
 *  MAIN FUNCTION
 *********************************************************************************************************************/

int main(void) {
    /* AUTOSAR initialization sequence */
    Os_Init();
    EcuM_Init();

    /* OS is now running, let EcuM take control */
    StartOS(OSDEFAULTAPPMODE);

    return 0;
}
