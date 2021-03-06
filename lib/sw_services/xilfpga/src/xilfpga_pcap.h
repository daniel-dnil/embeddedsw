/******************************************************************************
*
* Copyright (C) 2016 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
*
*******************************************************************************/
/*****************************************************************************/
/**
*
* @file xilfpga_pcap.h
* @addtogroup xfpga_apis XilFPGA APIs
* @{
*
* The XILFPGA Library provides the interfaces to the application to configure
* the programmable logic (PL) though the PS.
*
* Features
*	Supported:
*		Full Bit-stream loading.
*	To be supported features:
* 		Partial Bit-stream loading.
*		Encrypted Bit-stream loading.
*		Authenticated Bit stream loading.
*
* Xilfpga_PL library Interface modules:
*	Xilfpga_PL library uses the below major components to configure the PL through PS.
*		CSU DMA driver.
*		Xilsecure_library.
*
* CSU DMA driver:
*	It is used to transfer the actual Bit stream file for the PS to PL after PCAP initialization
*
* Xilsecure_library:
*	The LibXilSecure library provides APIs to access secure hardware on the Zynq® UltraScale+™
*	MPSoC devices.
*	This library includes:
*		• SHA-3 engine hash functions
*		• AES for symmetric key encryption
*		• RSA for authentication
*	These algorithms are needed to support to load the Encrypted and Authenticated bit-streams
*	into PL.
*
* @note
*       -xilfpga library is capable of loading only .bin format files into PL.
*        it will not support the other file formates.
*
*       -The current implementation supports only Full Bit-stream.
*
* This is the header file which contains definitions for the PCAP hardware
* registers and declarations of bitstream download functions
*
*
* <b>Initialization & writing bitstream </b>
*
* The fpga driver can be initialized and load the bit-stream
* in the following way:
*
*   - u32 XFpga_PL_BitSream_Load ();
*
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ----- ---- -------- -------------------------------------------------------
* 1.0   Nava   08/06/16 Initial release
*
* </pre>
*
* @note
*
******************************************************************************/

#ifndef XILFPGA_PCAP_H
#define XILFPGA_PCAP_H

/***************************** Include Files *********************************/

#include "xcsudma.h"
/************************** Constant Definitions *****************************/

#define PL_DONE_POLL_COUNT  30000U
#define PL_RESET_PERIOD_IN_US  1U



/* Dummy address to indicate that destination is PCAP */
#define XFPGA_DESTINATION_PCAP_ADDR    (0XFFFFFFFFU)
#define XFPGA_CSU_SSS_SRC_SRC_DMA    0x5U

/**
 * CSU Base Address
 */
#define CSU_BASEADDR      0XFFCA0000U

/**
 * Register: CSU_CSU_SSS_CFG
 */
#define CSU_CSU_SSS_CFG    ( ( CSU_BASEADDR ) + 0X00000008U )
#define CSU_CSU_SSS_CFG_PCAP_SSS_MASK    0X0000000FU
#define CSU_CSU_SSS_CFG_PCAP_SSS_SHIFT   0U

/**
 * Register: CSU_PCAP_STATUS
 */
#define CSU_PCAP_STATUS    ( ( CSU_BASEADDR ) + 0X00003010U )
#define CSU_PCAP_STATUS_PL_INIT_SHIFT   2U
#define CSU_PCAP_STATUS_PL_INIT_MASK    0X00000004U
#define CSU_PCAP_STATUS_PCAP_WR_IDLE_MASK    0X00000001U
#define CSU_PCAP_STATUS_PL_DONE_MASK    0X00000008U

/* Register: CSU_PCAP_RESET */
#define CSU_PCAP_RESET    ( ( CSU_BASEADDR ) + 0X0000300CU )
#define CSU_PCAP_RESET_RESET_MASK    0X00000001U

/* Register: CSU_PCAP_CTRL */
#define CSU_PCAP_CTRL    ( ( CSU_BASEADDR ) + 0X00003008U )
#define CSU_PCAP_CTRL_PCAP_PR_MASK    0X00000001U

/**
 * Register: CSU_PCAP_RDWR
 */
#define CSU_PCAP_RDWR    ( ( CSU_BASEADDR ) + 0X00003004U )
#define CSU_PCAP_RDWR_PCAP_RDWR_B_SHIFT   0U

/* Register: CSU_PCAP_PROG */
#define CSU_PCAP_PROG    ( ( CSU_BASEADDR ) + 0X00003000U )
#define CSU_PCAP_PROG_PCFG_PROG_B_MASK    0X00000001U
#define CSU_PCAP_PROG_PCFG_PROG_B_SHIFT   0U

/* Register: PMU_GLOBAL for PL power-up */
#define PMU_GLOBAL_BASE			0xFFD80000U
#define PMU_GLOBAL_PWRUP_STATUS		(PMU_GLOBAL_BASE + 0x110U)
#define PMU_GLOBAL_PWRUP_EN		(PMU_GLOBAL_BASE + 0x118U)
#define PMU_GLOBAL_PWRUP_TRIG		(PMU_GLOBAL_BASE + 0x120U)
#define PMU_GLOBAL_PWR_PL_MASK		0x800000

#define PMU_GLOBAL_ISO_INT_EN		( PMU_GLOBAL_BASE + 0X318U )
#define PMU_GLOBAL_ISO_TRIG		( PMU_GLOBAL_BASE + 0X320U )
#define PMU_GLOBAL_ISO_STATUS		( PMU_GLOBAL_BASE + 0X310U )

#define GPIO_DIRM_5_EMIO		0xFF0A0344
#define GPIO_MASK_DATA_5_MSW	0xFF0A002C

/* Register: PCAP_CLK_CTRL Address */
#define PCAP_CLK_CTRL		0xFF5E00A4
#define PCAP_CLK_EN_MASK	0x01000000

/* Register: ADMA_CLK_CTRL Address */
#define ADMA_CLK_CTRL		0xFF5E00B8
#define ADMA_CLK_EN_MASK	0X01000000

/* Register: ADMA_CH0 Base Address */
#define ADMA_CH0_BASEADDR      0XFFA80000U

/* Register: ADMA_CH0_ZDMA_CH_STATUS */
#define ADMA_CH0_ZDMA_CH_STATUS    ( ( ADMA_CH0_BASEADDR ) + 0X0000011CU )
#define ADMA_CH0_ZDMA_CH_STATUS_STATE_MASK    0X00000003U
#define ADMA_CH0_ZDMA_CH_STATUS_STATE_DONE    0X00000000U
#define ADMA_CH0_ZDMA_CH_STATUS_STATE_ERR     0X00000003U

/* Register: ADMA_CH0_ZDMA_CH_CTRL0 */
#define ADMA_CH0_ZDMA_CH_CTRL0    ( ( ADMA_CH0_BASEADDR ) + 0X00000110U )
#define ADMA_CH0_ZDMA_CH_CTRL0_POINT_TYPE_MASK    (u32)0X00000040U
#define ADMA_CH0_ZDMA_CH_CTRL0_POINT_TYPE_NORMAL  (u32)0X00000000U
#define ADMA_CH0_ZDMA_CH_CTRL0_MODE_MASK    (u32)0X00000030U
#define ADMA_CH0_ZDMA_CH_CTRL0_MODE_WR_ONLY (u32)0X00000010U

/* Register: ADMA_CH0_ZDMA_CH_WR_ONLY_WORD0 */
#define ADMA_CH0_ZDMA_CH_WR_ONLY_WORD0    ( ( ADMA_CH0_BASEADDR ) + 0X00000148U )

/* Register: ADMA_CH0_ZDMA_CH_WR_ONLY_WORD1 */
#define ADMA_CH0_ZDMA_CH_WR_ONLY_WORD1    ( ( ADMA_CH0_BASEADDR ) + 0X0000014CU )

/* Register: ADMA_CH0_ZDMA_CH_WR_ONLY_WORD2 */
#define ADMA_CH0_ZDMA_CH_WR_ONLY_WORD2    ( ( ADMA_CH0_BASEADDR ) + 0X00000150U )

/* Register: ADMA_CH0_ZDMA_CH_WR_ONLY_WORD3 */
#define ADMA_CH0_ZDMA_CH_WR_ONLY_WORD3    ( ( ADMA_CH0_BASEADDR ) + 0X00000154U )

/* Register: ADMA_CH0_ZDMA_CH_DST_DSCR_WORD0 */
#define ADMA_CH0_ZDMA_CH_DST_DSCR_WORD0    ( ( ADMA_CH0_BASEADDR ) + 0X00000138U )
#define ADMA_CH0_ZDMA_CH_DST_DSCR_WORD0_LSB_MASK    0XFFFFFFFFU

/* Register: ADMA_CH0_ZDMA_CH_DST_DSCR_WORD1 */
#define ADMA_CH0_ZDMA_CH_DST_DSCR_WORD1    ( ( ADMA_CH0_BASEADDR ) + 0X0000013CU )
#define ADMA_CH0_ZDMA_CH_DST_DSCR_WORD1_MSB_MASK    0X0001FFFFU

/* Register: ADMA_CH0_ZDMA_CH_SRC_DSCR_WORD2 */
#define ADMA_CH0_ZDMA_CH_SRC_DSCR_WORD2    ( ( ADMA_CH0_BASEADDR ) + 0X00000130U )

/* Register: ADMA_CH0_ZDMA_CH_DST_DSCR_WORD2 */
#define ADMA_CH0_ZDMA_CH_DST_DSCR_WORD2    ( ( ADMA_CH0_BASEADDR ) + 0X00000140U )

/* Register: ADMA_CH0_ZDMA_CH_SRC_DSCR_WORD1 */
#define ADMA_CH0_ZDMA_CH_SRC_DSCR_WORD1    ( ( ADMA_CH0_BASEADDR ) + 0X0000012CU )

/* Register: ADMA_CH0_ZDMA_CH_SRC_DSCR_WORD0 */
#define ADMA_CH0_ZDMA_CH_SRC_DSCR_WORD0    ( ( ADMA_CH0_BASEADDR ) + 0X00000128U )

/* Register: ADMA_CH0_ZDMA_CH_SRC_DSCR_WORD3 */
#define ADMA_CH0_ZDMA_CH_SRC_DSCR_WORD3    ( ( ADMA_CH0_BASEADDR ) + 0X00000134U )

/* Register: ADMA_CH0_ZDMA_CH_DST_DSCR_WORD3 */
#define ADMA_CH0_ZDMA_CH_DST_DSCR_WORD3    ( ( ADMA_CH0_BASEADDR ) + 0X00000144U )

/* Register: ADMA_CH0_ZDMA_CH_CTRL2 */
#define ADMA_CH0_ZDMA_CH_CTRL2    ( ( ADMA_CH0_BASEADDR ) + 0X00000200U )
#define ADMA_CH0_ZDMA_CH_CTRL2_EN_MASK    0X00000001U

/* Register: ADMA_CH0_ZDMA_CH_ISR */
#define ADMA_CH0_ZDMA_CH_ISR    ( ( ADMA_CH0_BASEADDR ) + 0X00000100U )
#define ADMA_CH0_ZDMA_CH_ISR_DMA_DONE_MASK    0X00000400U

#define XFPGA_SUCCESS				(0x0U)
#define XFPGA_ERROR_CSUDMA_INIT_FAIL		(0x1U)
#define XFPGA_ERROR_BITSTREAM_LOAD_FAIL		(0x2U)
#define XFPGA_ERROR_PL_POWER_UP			(0x3U)
#define XFPGA_ERROR_PL_ISOLATION		(0x4U)
#define XFPGA_PARAMETER_NULL_ERROR		(0x5U)
#define XFPGA_STRING_INVALID_ERROR		(0x6U)
#define XFPGA_RSA_DECRYPTION_FAIL		(0x7U)
#define XFPGA_SHA2_HASH_FAIL			(0x8U)
#define XFPGA_FAILURE					(0x9U)

/**************************** Type Definitions *******************************/
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
u32 XFpga_PL_BitSream_Load (u32 WrAddrHigh, u32 WrAddrLow,
				u32 WrSize, u32 flags);
u32 XFpga_PcapStatus(void);
/************************** Variable Definitions *****************************/

extern XCsuDma CsuDma;  /* CSU DMA instance */

#endif  /* XILFPGA_PCAP_H */
/** @} */
