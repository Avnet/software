//----------------------------------------------------------------------------
//      _____
//     *     *
//    *____   *____
//   * *===*   *==*
//  *___*===*___**  AVNET
//       *======*
//        *====*
//----------------------------------------------------------------------------
//
// This design is the property of Avnet.  Publication of this
// design is not authorized without written consent from Avnet.
//
// Please direct any questions to the PicoZed community support forum:
//    http://www.picozed.org/forum
//
// Product information is available at:
//    http://www.picozed.org/product/picozed
//
// Disclaimer:
//    Avnet, Inc. makes no warranty for the use of this code or design.
//    This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//    any errors, which may appear in this code, nor does it make a commitment
//    to update the information contained herein. Avnet, Inc specifically
//    disclaims any implied warranties of fitness for a particular purpose.
//                     Copyright(c) 2013 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Dec 05, 2013
// Design Name:         PicoZed FMC2 FMC Loopback Test
// Module Name:         fmc_lpc.h
// Project Name:        PicoZed FMC2 FMC Loopback Test
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         FMC LPC loopback test routines.
//
// Dependencies:
//
// Revision:            Dec 05, 2013: 1.00 Initial version
//
//----------------------------------------------------------------------------


#ifndef FMC_LPC_H
#define FMC_LPC_H

// ---------------------------------------------------------------------------
// Local include Files.
#include "pl_gpio_udriver.h"
#include "platform.h"
#include "types.h"

// Constant definitions.
// ---------------------------------------------------------------------------
#define FMC_LPC_BIT_MASK					0x0003FFFF
#define FMC_LPC_BIT_WIDTH					18

// ---------------------------------------------------------------------------
// Function prototype declarations.
int fmc_lpc_setup(int32u verbosity);
int fmc_lpc_test(int32u test_pattern, int32u verbosity);
int fmc_lpc_walking_ones(int32u verbosity);
int fmc_lpc_walking_zeros(int32u verbosity);
int fmc_lpc_all_zeros(int32u verbosity);
int fmc_lpc_all_ones(int32u verbosity);

#endif // FMC_LPC_H
