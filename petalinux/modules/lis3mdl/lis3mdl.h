
/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
*
* File Name          : lis3mdl.h
* Authors	: AMS - Motion Mems Division - Application Team
*		     : Matteo Dameno (matteo.dameno@st.com)
*		     : Denis Ciocca (denis.ciocca@st.com)
* Version            : V.1.0.2
* Date               : 2013/Sep/19
*
********************************************************************************
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* THE PRESENT SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES
* OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, FOR THE SOLE
* PURPOSE TO SUPPORT YOUR APPLICATION DEVELOPMENT.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
********************************************************************************
********************************************************************************
Version History.

Revision 1-0-1 2012/05/04
 first revision
*******************************************************************************/

#ifndef	__LIS3MDL_H__
#define	__LIS3MDL_H__

#define	LIS3MDL_DEV_NAME	"lis3mdl_mag"


/************************************************/
/* 	Output data			 	*/
/*************************************************
magnetometer: raw
*************************************************/

/************************************************/
/* 	sysfs data			 	*/
/*************************************************
magnetometer:
	- pollrate->ms
	- fullscale->gauss
*************************************************/


/* Magnetometer Sensor Full Scale */
#define LIS3MDL_MAG_FS_MASK	(0x60)
#define LIS3MDL_MAG_FS_4G	(0x00)	/* Full scale 4 Gauss */
#define LIS3MDL_MAG_FS_8G	(0x20)	/* Full scale 8 Gauss */
#define LIS3MDL_MAG_FS_12G	(0x40)	/* Full scale 12 Gauss */
#define LIS3MDL_MAG_FS_16G	(0x60)	/* Full scale 16 Gauss */


#ifdef	__KERNEL__

#define LIS3MDL_MAG_ENABLE_ON_INPUT_OPEN 0

#define DEFAULT_INT1_GPIO		(-EINVAL)
#define DEFAULT_INT2_GPIO		(-EINVAL)

#define LIS3MDL_MAG_MIN_POLL_PERIOD_MS	10


struct lis3mdl_mag_platform_data {

	unsigned int poll_interval;
	unsigned int min_interval;

	u8 fs_range;

	u8 axis_map_x;
	u8 axis_map_y;
	u8 axis_map_z;

	u8 negate_x;
	u8 negate_y;
	u8 negate_z;

	int (*init)(void);
	void (*exit)(void);
	int (*power_on)(void);
	int (*power_off)(void);
};

#endif	/* __KERNEL__ */
#endif	/* __LIS3MDL_H__ */

