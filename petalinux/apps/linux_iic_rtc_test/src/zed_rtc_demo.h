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
//                     Copyright(c) 2015 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Jul 21, 2015
// Design Name:         PicoZed FMC2 Real Time Clock Demonstration
// Module Name:         zed_rtc_demo.h
// Project Name:        PicoZed FMC2 Real Time Clock Demonstrtion
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         Real Time Clock Demonstration for PicoZed FMC2 Carrier
//
//                      This application will demonstrate I2C access to Maxim
//                      DS3231M Real Time Clock Pmod.
//
// Dependencies:
//
// Revision:            Jul 21, 2015: 1.00 Initial version
//
//----------------------------------------------------------------------------

#ifndef __ZED_RTC_DEMO_H__
#define __ZED_RTC_DEMO_H__

#include "types.h"

// I2C related.
#include "zed_iic.h"

// This structure contains the context for the
// Zed Real Time Clock Demonstration
struct struct_zed_rtc_demo_t
{
   int32u bVerbose;

   ////////////////////////////////
   // RTC I2C related context
   ////////////////////////////////
   int32u uBaseAddr_IIC_RTC;

    zed_iic_t rtc_iic;

   int8u rtc_date;
   int8u rtc_day;
   int8u rtc_hours;
   int8u rtc_minutes;
   int8u rtc_month;
   int8u rtc_seconds;
   int8u rtc_status;
   int8u rtc_temp_lower;
   int8u rtc_temp_upper;
   int8u rtc_year;
   int32u rtc_events;
   int32u rtc_irqs;
   double rtc_temp;
};
typedef struct struct_zed_rtc_demo_t zed_rtc_demo_t;

int zed_rtc_demo_calculate_datetime(zed_rtc_demo_t *pDemo);
int zed_rtc_demo_init(zed_rtc_demo_t *pDemo);
int zed_rtc_demo_status(zed_rtc_demo_t *pDemo);
int zed_rtc_demo_reset(zed_rtc_demo_t *pDemo);

#endif // __ZED_RTC_DEMO_H__
