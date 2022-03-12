/* $Id$ */
//
// Проект HF Dream Receiver (КВ приёмник мечты)
// автор Гена Завидовский mgs2001@mail.ru
// UA1ARN
//

#include "hardware.h"	/* зависящие от процессора функции работы с портами */
#include "clocks.h"

uint_fast32_t calcdivround2(
	uint_fast32_t ref,	/* частота на входе делителя, в герцах. */
	uint_fast32_t freq	/* требуемая частота на выходе делителя, в герцах. */
	)
{
	return (ref < freq) ? 1 : ((ref + freq / 2) / freq);
}

/* частоты, подающиеся на периферию */
#define BOARD_USART1_FREQ 	(stm32mp1_uart1_get_freq())
#define BOARD_USART2_FREQ 	(stm32mp1_uart2_4_get_freq())
#define BOARD_USART3_FREQ 	(stm32mp1_uart3_5_get_freq())
#define BOARD_UART4_FREQ 	(stm32mp1_uart2_4_get_freq())
#define BOARD_UART5_FREQ 	(stm32mp1_uart3_5_get_freq())
#define BOARD_USART6_FREQ 	(stm32mp1_usart6_get_freq())
#define BOARD_UART7_FREQ 	(stm32mp1_uart7_8_get_freq())
#define BOARD_UART8_FREQ 	(stm32mp1_uart7_8_get_freq())
#define BOARD_TIM3_FREQ 	(stm32mp1_get_timg1_freq())
#define BOARD_TIM5_FREQ 	(stm32mp1_get_timg1_freq())
#define BOARD_ADC_FREQ 		(stm32mp1_get_adc_freq())

unsigned long stm32mp1_get_hse_freq(void)
{
#if WITHCPUXTAL
	return WITHCPUXTAL;
#elif WITHCPUXOSC
	return WITHCPUXOSC;
#else
	#warning WITHCPUXOSC or WITHCPUXTAL should be defined
	return 24000000uL;
#endif
}

// hsi_ck
// hsi_ker_ck
unsigned long stm32mp1_get_hsi_freq(void)
{
	const uint_fast32_t hsi = HSI64FREQ;
	const uint_fast32_t hsidiv = (RCC->HSICFGR & RCC_HSICFGR_HSIDIV_Msk) >> RCC_HSICFGR_HSIDIV_Pos;
	return hsi >> hsidiv;
}

unsigned long stm32mp1_get_pll1_2_ref_freq(void)
{
	// PLL1, PLL2 source mux
	// 0x0: HSI selected as PLL clock (hsi_ck) (default after reset)
	// 0x1: HSE selected as PLL clock (hse_ck)
	switch ((RCC->RCK12SELR & RCC_RCK12SELR_PLL12SRC_Msk) >> RCC_RCK12SELR_PLL12SRC_Pos)
	{
	default:
	case 0x00:
		return stm32mp1_get_hsi_freq();
	case 0x01:
		return stm32mp1_get_hse_freq();
	}
}

unsigned long stm32mp1_get_pll3_ref_freq(void)
{
	// PLL3 source mux
	//	0x0: HSI selected as PLL clock (hsi_ck) (default after reset)
	//	0x1: HSE selected as PLL clock (hse_ck)
	//	0x2: CSI selected as PLL clock (csi_ck)
	//	0x3: No clock send to DIVMx divisor and PLLs
	switch ((RCC->RCK3SELR & RCC_RCK3SELR_PLL3SRC_Msk) >> RCC_RCK3SELR_PLL3SRC_Pos)
	{
	default:
	case 0x00:
		return stm32mp1_get_hsi_freq();
	case 0x01:
		return stm32mp1_get_hse_freq();
	case 0x02:
		return CSI_VALUE;
	}
}

unsigned long stm32mp1_get_pll4_ref_freq(void)
{
	// PLL4 source mux
	//	0x0: HSI selected as PLL clock (hsi_ck) (default after reset)
	//	0x1: HSE selected as PLL clock (hse_ck)
	//	0x2: CSI selected as PLL clock (csi_ck)
	//	0x3: Signal I2S_CKIN used as reference clock
	switch ((RCC->RCK4SELR & RCC_RCK4SELR_PLL4SRC_Msk) >> RCC_RCK4SELR_PLL4SRC_Pos)
	{
	default:
	case 0x00:
		return stm32mp1_get_hsi_freq();
	case 0x01:
		return stm32mp1_get_hse_freq();
	case 0x02:
		return CSI_VALUE;
#if (defined BOARD_I2S_CKIN_FREQ)
	case 0x03: return BOARD_I2S_CKIN_FREQ;
#endif /* (defined BOARD_I2S_CKIN_FREQ) */
	}
}

// PLL1 methods
unsigned long stm32mp1_get_pll1_freq(void)
{
	const uint_fast32_t pll1divn = ((RCC->PLL1CFGR1 & RCC_PLL1CFGR1_DIVN_Msk) >> RCC_PLL1CFGR1_DIVN_Pos) + 1;
	const uint_fast32_t pll1divm = ((RCC->PLL1CFGR1 & RCC_PLL1CFGR1_DIVM1_Msk) >> RCC_PLL1CFGR1_DIVM1_Pos) + 1;
	return (uint_fast64_t) stm32mp1_get_pll1_2_ref_freq() * pll1divn / pll1divm;
}

unsigned long stm32mp1_get_pll1_p_freq(void)
{
	const uint_fast32_t pll1divp = ((RCC->PLL1CFGR2 & RCC_PLL1CFGR2_DIVP_Msk) >> RCC_PLL1CFGR2_DIVP_Pos) + 1;
	return stm32mp1_get_pll1_freq() / pll1divp;
}

// MPU frequency
// mpuss_ck
unsigned long stm32mp1_get_mpuss_freq(void)
{
	//	0x0: The MPUDIV is disabled; i.e. no clock generated
	//	0x1: The mpuss_ck is equal to pll1_p_ck divided by 2 (default after reset)
	//	0x2: The mpuss_ck is equal to pll1_p_ck divided by 4
	//	0x3: The mpuss_ck is equal to pll1_p_ck divided by 8
	//	others: The mpuss_ck is equal to pll1_p_ck divided by 16

	const uint_fast32_t mpudiv = 1uL << ((RCC->MPCKDIVR & RCC_MPCKDIVR_MPUDIV_Msk) >> RCC_MPCKDIVR_MPUDIV_Pos);

	//	0x0: HSI selected as MPU sub-system clock (hsi_ck) (default after reset)
	//	0x1: HSE selected as MPU sub-system clock (hse_ck)
	//	0x2: PLL1 selected as MPU sub-system clock (pll1_p_ck)
	//	0x3: PLL1 via MPUDIV is selected as MPU sub-system clock (pll1_p_ck / (2 ^ MPUDIV)).
	switch ((RCC->MPCKSELR & RCC_MPCKSELR_MPUSRC_Msk) >> RCC_MPCKSELR_MPUSRC_Pos)
	{
	default:
	case 0x00:
		return stm32mp1_get_hsi_freq();
	case 0x01:
		return stm32mp1_get_hse_freq();
	case 0x02:
		return stm32mp1_get_pll1_p_freq();
	case 0x03:
		return stm32mp1_get_pll1_p_freq() / mpudiv;
	}
}

// PLL2 methods
unsigned long stm32mp1_get_pll2_freq(void)
{
	const uint_fast32_t pll2divn = ((RCC->PLL2CFGR1 & RCC_PLL2CFGR1_DIVN_Msk) >> RCC_PLL2CFGR1_DIVN_Pos) + 1;
	const uint_fast32_t pll2divm = ((RCC->PLL2CFGR1 & RCC_PLL2CFGR1_DIVM2_Msk) >> RCC_PLL2CFGR1_DIVM2_Pos) + 1;
	return (uint_fast64_t) stm32mp1_get_pll1_2_ref_freq() * pll2divn / pll2divm;
}

unsigned long stm32mp1_get_pll2_p_freq(void)
{
	const uint_fast32_t pll2divp = ((RCC->PLL2CFGR2 & RCC_PLL2CFGR2_DIVP_Msk) >> RCC_PLL2CFGR2_DIVP_Pos) + 1;
	return stm32mp1_get_pll2_freq() / pll2divp;
}

unsigned long stm32mp1_get_pll2_r_freq(void)
{
	const uint_fast32_t pll2divr = ((RCC->PLL2CFGR2 & RCC_PLL2CFGR2_DIVR_Msk) >> RCC_PLL2CFGR2_DIVR_Pos) + 1;
	return stm32mp1_get_pll2_freq() / pll2divr;
}

// PLL3 methods
unsigned long stm32mp1_get_pll3_freq(void)
{
	const uint_fast32_t pll3divn = ((RCC->PLL3CFGR1 & RCC_PLL3CFGR1_DIVN_Msk) >> RCC_PLL3CFGR1_DIVN_Pos) + 1;
	const uint_fast32_t pll3divm = ((RCC->PLL3CFGR1 & RCC_PLL3CFGR1_DIVM3_Msk) >> RCC_PLL3CFGR1_DIVM3_Pos) + 1;
	return (uint_fast64_t) stm32mp1_get_pll3_ref_freq() * pll3divn / pll3divm;
}

unsigned long stm32mp1_get_pll3_p_freq(void)
{
	const uint_fast32_t pll3divp = ((RCC->PLL3CFGR2 & RCC_PLL3CFGR2_DIVP_Msk) >> RCC_PLL3CFGR2_DIVP_Pos) + 1;
	return stm32mp1_get_pll3_freq() / pll3divp;
}

unsigned long stm32mp1_get_pll3_q_freq(void)
{
	const uint_fast32_t pll3divq = ((RCC->PLL3CFGR2 & RCC_PLL3CFGR2_DIVQ_Msk) >> RCC_PLL3CFGR2_DIVQ_Pos) + 1;
	return stm32mp1_get_pll3_freq() / pll3divq;
}

unsigned long stm32mp1_get_pll3_r_freq(void)
{
	const uint_fast32_t pll3divr = ((RCC->PLL3CFGR2 & RCC_PLL3CFGR2_DIVR_Msk) >> RCC_PLL3CFGR2_DIVR_Pos) + 1;
	return stm32mp1_get_pll3_freq() / pll3divr;
}

// PLL4 methods
unsigned long stm32mp1_get_pll4_freq(void)
{
	//#define PLL4_FREQ	(REFINFREQ / (PLL4DIVM) * (PLL4DIVN))
	const uint32_t pll4divn = ((RCC->PLL4CFGR1 & RCC_PLL4CFGR1_DIVN_Msk) >> RCC_PLL4CFGR1_DIVN_Pos) + 1;
	const uint32_t pll4divm = ((RCC->PLL4CFGR1 & RCC_PLL4CFGR1_DIVM4_Msk) >> RCC_PLL4CFGR1_DIVM4_Pos) + 1;
	return (uint_fast64_t) stm32mp1_get_pll4_ref_freq() * pll4divn / pll4divm;
}

unsigned long stm32mp1_get_pll4_q_freq(void)
{
	const uint_fast32_t pll4divq = ((RCC->PLL4CFGR2 & RCC_PLL4CFGR2_DIVQ_Msk) >> RCC_PLL4CFGR2_DIVQ_Pos) + 1;
	return stm32mp1_get_pll4_freq() / pll4divq;
}

unsigned long stm32mp1_get_pll4_p_freq(void)
{
	const uint_fast32_t pll4divp = ((RCC->PLL4CFGR2 & RCC_PLL4CFGR2_DIVP_Msk) >> RCC_PLL4CFGR2_DIVP_Pos) + 1;
	return stm32mp1_get_pll4_freq() / pll4divp;
}

unsigned long stm32mp1_get_pll4_r_freq(void)
{
	const uint_fast32_t pll4divr = ((RCC->PLL4CFGR2 & RCC_PLL4CFGR2_DIVR_Msk) >> RCC_PLL4CFGR2_DIVR_Pos) + 1;
	return stm32mp1_get_pll4_freq() / pll4divr;
}

// Ethernet controller freq
// ETHSRC
unsigned long stm32mp1_get_eth_freq(void)
{
	//0x0: pll4_p_ck clock selected as kernel peripheral clock (default after reset)
	//0x1: pll3_q_ck clock selected as kernel peripheral clock
	//others: the kernel clock is disabled
	switch ((RCC->ETHCKSELR & RCC_ETHCKSELR_ETHSRC_Msk) >> RCC_ETHCKSELR_ETHSRC_Pos)
	{
	default:
	case 0x00: return stm32mp1_get_pll4_p_freq();
	case 0x01: return stm32mp1_get_pll3_q_freq();
	}
}

// clk_ptp_ref_i
unsigned long stm32mp1_get_ethptp_freq(void)
{
	const unsigned long d = ((RCC->ETHCKSELR & RCC_ETHCKSELR_ETHPTPDIV_Msk) >> RCC_ETHCKSELR_ETHPTPDIV_Pos) + 1;
	return stm32mp1_get_eth_freq() / d;
}

// Internal AXI clock frequency
unsigned long stm32mp1_get_axiss_freq(void)
{
	//0x0: HSI selected as AXI sub-system clock (hsi_ck) (default after reset)
	//0x1: HSE selected as AXI sub-system clock (hse_ck)
	//0x2: PLL2 selected as AXI sub-system clock (pll2_p_ck)
	//others: axiss_ck is gated
	// axiss_ck 266 MHz Max
	switch ((RCC->ASSCKSELR & RCC_ASSCKSELR_AXISSRC_Msk) >> RCC_ASSCKSELR_AXISSRC_Pos)
	{
	case 0x00: return stm32mp1_get_hsi_freq();
	case 0x01: return stm32mp1_get_hse_freq();
	case 0x02: return stm32mp1_get_pll2_p_freq();
	default: return HSI64FREQ;
	}
}

unsigned long stm32mp1_get_per_freq(void)
{
	// per_ck source clock selection
	//0x0: hsi_ker_ck clock selected (default after reset)
	//0x1: csi_ker_ck clock selected
	//0x2: hse_ker_ck clock selected
	//0x3: Clock disabled
	switch ((RCC->CPERCKSELR & RCC_CPERCKSELR_CKPERSRC_Msk) >> RCC_CPERCKSELR_CKPERSRC_Pos)
	{
	case 0x00:	return stm32mp1_get_hsi_freq();
	case 0x01:	return CSI_VALUE;
	case 0x02:	return stm32mp1_get_hse_freq();
	default: return HSI64FREQ;
	}

}
unsigned long stm32mp1_get_mcuss_freq(void)
{
	// MCUSSRC
	//	0x0: HSI selected as MCU sub-system clock (hsi_ck) (default after reset)
	//	0x1: HSE selected as MCU sub-system clock (hse_ck)
	//	0x2: CSI selected as MCU sub-system clock (csi_ck)
	//	0x3: PLL3 selected as MCU sub-system clock (pll3_p_ck)
	switch ((RCC->MSSCKSELR & RCC_MSSCKSELR_MCUSSRC_Msk) >> RCC_MSSCKSELR_MCUSSRC_Pos)
	{
	default:
	case 0x00: return stm32mp1_get_hsi_freq();
	case 0x01: return stm32mp1_get_hse_freq();
	case 0x02: return CSI_VALUE;
	case 0x03: return stm32mp1_get_pll3_p_freq();
	}
}

// hclk5, hclk6, aclk
unsigned long stm32mp1_get_aclk_freq(void)
{
	// AXI, AHB5 and AHB6 clock divisor
	//	0x0: axiss_ck (default after reset)
	//	0x1: axiss_ck / 2
	//	0x2: axiss_ck / 3
	//	others: axiss_ck / 4
	switch ((RCC->AXIDIVR & RCC_AXIDIVR_AXIDIV_Msk) >> RCC_AXIDIVR_AXIDIV_Pos)
	{
	case 0x00: return stm32mp1_get_axiss_freq();
	case 0x01: return stm32mp1_get_axiss_freq() / 2;
	case 0x02: return stm32mp1_get_axiss_freq() / 3;
	default: return stm32mp1_get_axiss_freq();
	}
}

// Internal AHB5 clock frequency
unsigned long stm32mp1_get_hclk5_freq(void)
{
	return stm32mp1_get_aclk_freq();
}

// Internal AHB6 clock frequency
unsigned long stm32mp1_get_hclk6_freq(void)
{
	return stm32mp1_get_aclk_freq();
}

// mlhclk_ck
// 209 MHz max
unsigned long stm32mp1_get_mlhclk_freq(void)
{
	// MCUDIV
	switch ((RCC->MCUDIVR & RCC_MCUDIVR_MCUDIV_Msk) >> RCC_MCUDIVR_MCUDIV_Pos)
	{
	case 0: return stm32mp1_get_mcuss_freq() / 1;
	case 1: return stm32mp1_get_mcuss_freq() / 2;
	case 2: return stm32mp1_get_mcuss_freq() / 4;
	case 3: return stm32mp1_get_mcuss_freq() / 8;
	case 4: return stm32mp1_get_mcuss_freq() / 16;
	case 5: return stm32mp1_get_mcuss_freq() / 32;
	case 6: return stm32mp1_get_mcuss_freq() / 64;
	case 7: return stm32mp1_get_mcuss_freq() / 128;
	case 8: return stm32mp1_get_mcuss_freq() / 256;
	default: return stm32mp1_get_mcuss_freq() / 512;
	}
}

// Internal APB1 clock frequency
// 104.5 MHz max
unsigned long stm32mp1_get_pclk1_freq(void)
{
	// APB1 Output divisor (output max 104.5 MHz)
	// Input MLHCK (209 MHz max)
	//0x0: mlhclk (default after reset)
	//0x1: mlhclk / 2
	//0x2: mlhclk / 4
	//0x3: mlhclk / 8
	//0x4: mlhclk / 16
	switch ((RCC->APB1DIVR & RCC_APB1DIVR_APB1DIV_Msk) >> RCC_APB1DIVR_APB1DIV_Pos)
	{
	default:
	case 0x00: return stm32mp1_get_mlhclk_freq() / 1;
	case 0x01: return stm32mp1_get_mlhclk_freq() / 2;
	case 0x02: return stm32mp1_get_mlhclk_freq() / 4;
	case 0x03: return stm32mp1_get_mlhclk_freq() / 8;
	case 0x04: return stm32mp1_get_mlhclk_freq() / 16;
	}
}

// Internal APB2 clock frequency
// 104.5 MHz max
unsigned long stm32mp1_get_pclk2_freq(void)
{
	// APB2 Output divisor (output max 104.5 MHz)
	// Input MLHCK (209 MHz max)
	//0x0: mlhclk (default after reset)
	//0x1: mlhclk / 2
	//0x2: mlhclk / 4
	//0x3: mlhclk / 8
	//0x4: mlhclk / 16
	switch ((RCC->APB2DIVR & RCC_APB2DIVR_APB2DIV_Msk) >> RCC_APB2DIVR_APB2DIV_Pos)
	{
	default:
	case 0x00: return stm32mp1_get_mlhclk_freq() / 1;
	case 0x01: return stm32mp1_get_mlhclk_freq() / 2;
	case 0x02: return stm32mp1_get_mlhclk_freq() / 4;
	case 0x03: return stm32mp1_get_mlhclk_freq() / 8;
	case 0x04: return stm32mp1_get_mlhclk_freq() / 16;
	}
}

// Internal APB3 clock frequency
// 104.5 MHz max
unsigned long stm32mp1_get_pclk3_freq(void)
{
	// APB3 Output divisor (output max 104.5 MHz)
	// Input MLHCK (209 MHz max)
	//0x0: mlhclk (default after reset)
	//0x1: mlhclk / 2
	//0x2: mlhclk / 4
	//0x3: mlhclk / 8
	//0x4: mlhclk / 16
	switch ((RCC->APB3DIVR & RCC_APB3DIVR_APB3DIV_Msk) >> RCC_APB3DIVR_APB3DIV_Pos)
	{
	default:
	case 0x00: return stm32mp1_get_mlhclk_freq() / 1;
	case 0x01: return stm32mp1_get_mlhclk_freq() / 2;
	case 0x02: return stm32mp1_get_mlhclk_freq() / 4;
	case 0x03: return stm32mp1_get_mlhclk_freq() / 8;
	case 0x04: return stm32mp1_get_mlhclk_freq() / 16;
	}
}

// Internal APB4 clock frequency
// 133 MHz max
unsigned long stm32mp1_get_pclk4_freq(void)
{
	// APB4 Output divisor
	//	0x0: aclk (default after reset)
	//	0x1: aclk / 2
	//	0x2: aclk / 4
	//	0x3: aclk / 8
	//	others: aclk / 16
	switch ((RCC->APB4DIVR & RCC_APB4DIVR_APB4DIV_Msk) >> RCC_APB4DIVR_APB4DIV_Pos)
	{
	case 0x00:	return stm32mp1_get_aclk_freq() / 1;
	case 0x01:	return stm32mp1_get_aclk_freq() / 2;
	case 0x02:	return stm32mp1_get_aclk_freq() / 4;
	case 0x03:	return stm32mp1_get_aclk_freq() / 8;
	default:	return stm32mp1_get_aclk_freq() / 16;
	}
}

// Internal APB5 clock frequency
// 133 MHz max
unsigned long stm32mp1_get_pclk5_freq(void)
{
	// APB5 Output divisor
	//	0x0: aclk (default after reset)
	//	0x1: aclk / 2
	//	0x2: aclk / 4
	//	0x3: aclk / 8
	//	others: aclk / 16
	switch ((RCC->APB5DIVR & RCC_APB5DIVR_APB5DIV_Msk) >> RCC_APB5DIVR_APB5DIV_Pos)
	{
	case 0x00:	return stm32mp1_get_aclk_freq() / 1;
	case 0x01:	return stm32mp1_get_aclk_freq() / 2;
	case 0x02:	return stm32mp1_get_aclk_freq() / 4;
	case 0x03:	return stm32mp1_get_aclk_freq() / 8;
	default:	return stm32mp1_get_aclk_freq() / 16;
	}
}

unsigned long stm32mp1_get_usbphy_freq(void)
{
	//	0x0: hse_ker_ck clock selected as kernel peripheral clock (default after reset)
	//	0x1: pll4_r_ck clock selected as kernel peripheral clock
	//	0x2: hse_ker_ck/2 clock selected as kernel peripheral clock
	//	other: Clock disabled
	switch ((RCC->USBCKSELR & RCC_USBCKSELR_USBPHYSRC_Msk) >> RCC_USBCKSELR_USBPHYSRC_Pos)
	{
	default:
	case 0x00:
		return stm32mp1_get_hse_freq();
	case 0x01:
		return stm32mp1_get_pll4_r_freq();
	case 0x02:
		return stm32mp1_get_hse_freq();
	}
}

unsigned long stm32mp1_get_qspi_freq(void)
{
	//	0x0: aclk clock selected as kernel peripheral clock (default after reset)
	//	0x1: pll3_r_ck clock selected as kernel peripheral clock
	//	0x2: pll4_p_ck clock selected as kernel peripheral clock
	//	0x3: per_ck clock selected as kernel peripheral clock
	switch ((RCC->QSPICKSELR & RCC_QSPICKSELR_QSPISRC_Msk) >> RCC_QSPICKSELR_QSPISRC_Pos)
	{
	default:
	case 0x00:
		return stm32mp1_get_aclk_freq();
	case 0x01:
		return stm32mp1_get_pll3_r_freq();
	case 0x02:
		return stm32mp1_get_pll4_p_freq();
	case 0x03:
		return stm32mp1_get_per_freq();
	}
}

unsigned long stm32mp1_get_usbotg_freq(void)
{
	//	0: pll4_r_ck clock selected as kernel peripheral clock (default after reset)
	//	1: clock provided by the USB PHY (rcc_ck_usbo_48m) selected as kernel peripheral clock

	switch ((RCC->USBCKSELR & RCC_USBCKSELR_USBOSRC_Msk) >> RCC_USBCKSELR_USBOSRC_Pos)
	{
	default:
	case 0x00:
		return stm32mp1_get_pll4_r_freq();
	case 0x01:
		return 48000000uL; //rcc_ck_usbo_48m;
	}
}

unsigned long stm32mp1_uart1_get_freq(void)
{
	//	0x0: pclk5 clock selected as kernel peripheral clock (default after reset)
	//	0x1: pll3_q_ck clock selected as kernel peripheral clock
	//	0x2: hsi_ker_ck clock selected as kernel peripheral clock
	//	0x3: csi_ker_ck clock selected as kernel peripheral clock
	//	0x4: pll4_q_ck clock selected as kernel peripheral clock
	//	0x5: hse_ker_ck clock selected as kernel peripheral clock
	switch ((RCC->UART1CKSELR & RCC_UART1CKSELR_UART1SRC_Msk) >> RCC_UART1CKSELR_UART1SRC_Pos)
	{
	case 0x00:
		return stm32mp1_get_pclk5_freq();
	case 0x01:
		return stm32mp1_get_pll3_q_freq();
	case 0x02:
		return stm32mp1_get_hsi_freq();
	case 0x03:
		return CSI_VALUE;
	case 0x04:
		return stm32mp1_get_pll4_q_freq();
	case 0x05:
		return stm32mp1_get_hse_freq();
	default:
		return HSI64FREQ;
	}
}

unsigned long stm32mp1_uart2_4_get_freq(void)
{
	// UART2, UART4
	//	0x0: pclk1 clock selected as kernel peripheral clock (default after reset)
	//	0x1: pll4_q_ck clock selected as kernel peripheral clock
	//	0x2: hsi_ker_ck clock selected as kernel peripheral clock
	//	0x3: csi_ker_ck clock selected as kernel peripheral clock
	//	0x4: hse_ker_ck clock selected as kernel peripheral clock
	switch ((RCC->UART24CKSELR & RCC_UART24CKSELR_UART24SRC_Msk) >> RCC_UART24CKSELR_UART24SRC_Pos)
	{
	case 0x00:
		return stm32mp1_get_pclk1_freq();
	case 0x01:
		return stm32mp1_get_pll4_q_freq();
	case 0x02:
		return stm32mp1_get_hsi_freq();
	case 0x03:
		return CSI_VALUE;
	case 0x04:
		return stm32mp1_get_hse_freq();
	default:
		return HSI64FREQ;
	}

}

unsigned long stm32mp1_uart3_5_get_freq(void)
{
	//	0x0: pclk1 clock selected as kernel peripheral clock (default after reset)
	//	0x1: pll4_q_ck clock selected as kernel peripheral clock
	//	0x2: hsi_ker_ck clock selected as kernel peripheral clock
	//	0x3: csi_ker_ck clock selected as kernel peripheral clock
	//	0x4: hse_ker_ck clock selected as kernel peripheral clock
	//	others: reserved, the kernel clock is disabled
	switch ((RCC->UART35CKSELR & RCC_UART35CKSELR_UART35SRC_Msk) >> RCC_UART35CKSELR_UART35SRC_Pos)
	{
	case 0x00:
		return stm32mp1_get_pclk1_freq();
	case 0x01:
		return stm32mp1_get_pll4_q_freq();
	case 0x02:
		return stm32mp1_get_hsi_freq();
	case 0x03:
		return CSI_VALUE;
	case 0x04:
		return stm32mp1_get_hse_freq();
	default:
		return HSI64FREQ;
	}
}

unsigned long stm32mp1_uart7_8_get_freq(void)
{
	// UART7, UART8
	//0x0: pclk1 clock selected as kernel peripheral clock (default after reset)
	//0x1: pll4_q_ck clock selected as kernel peripheral clock
	//0x2: hsi_ker_ck clock selected as kernel peripheral clock
	//0x3: csi_ker_ck clock selected as kernel peripheral clock
	//0x4: hse_ker_ck clock selected as kernel peripheral clock
	switch ((RCC->UART78CKSELR & RCC_UART78CKSELR_UART78SRC_Msk) >> RCC_UART78CKSELR_UART78SRC_Pos)
	{
	case 0x00:
		return stm32mp1_get_pclk1_freq();
	case 0x01:
		return stm32mp1_get_pll4_q_freq();
	case 0x02:
		return stm32mp1_get_hsi_freq();
	case 0x03:
		return CSI_VALUE;
	case 0x04:
		return stm32mp1_get_hse_freq();
	default:
		return HSI64FREQ;
	}

}

unsigned long stm32mp1_sdmmc1_2_get_freq(void)
{
	// SDMMC1
	//	0x0: hclk6 clock selected as kernel peripheral clock
	//	0x1: pll3_r_ck clock selected as kernel peripheral clock
	//	0x2: pll4_p_ck clock selected as kernel peripheral clock
	//	0x3: hsi_ker_ck clock selected as kernel peripheral clock (default after reset)
	//	others: reserved, the kernel clock is disabled
	switch ((RCC->SDMMC12CKSELR & RCC_SDMMC12CKSELR_SDMMC12SRC_Msk) >> RCC_SDMMC12CKSELR_SDMMC12SRC_Pos)
	{
	case 0x00:
		return stm32mp1_get_hclk6_freq();
	case 0x01:
		return stm32mp1_get_pll3_r_freq();
	case 0x02:
		return stm32mp1_get_pll4_p_freq();
	case 0x03:
		return stm32mp1_get_hsi_freq();
	default:
		return HSI64FREQ;
	}
}

// частота для деления таймером
// timg1_ck: TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM12, TIM13 and TIM14
// kernel: Ftimg1_ck 209 MHz max
// bus: Fpclk1 104.5 MHz max
unsigned long stm32mp1_get_timg1_freq(void)
{
	const unsigned g1 = (RCC->TIMG1PRER & RCC_TIMG1PRER_TIMG1PRE_Msk) != 0;
	switch ((RCC->APB1DIVR & RCC_APB1DIVR_APB1DIV_Msk) >> RCC_APB1DIVR_APB1DIV_Pos)
	{
	default:
	case 0x00: return stm32mp1_get_mlhclk_freq();
	case 0x01: return stm32mp1_get_mlhclk_freq();
	case 0x02: return g1 ? stm32mp1_get_mlhclk_freq() : (stm32mp1_get_mlhclk_freq() / 2);
	case 0x03: return g1 ? (stm32mp1_get_mlhclk_freq() / 2) : (stm32mp1_get_mlhclk_freq() / 4);
	case 0x04: return g1 ? (stm32mp1_get_mlhclk_freq() / 4) : (stm32mp1_get_mlhclk_freq() / 8);
	}
}

// частота для деления таймером
// timg2_ck: TIM1, TIM8, TIM15, TIM16, and TIM17
// kernel: Ftimg1_ck 209 MHz max
// bus: Fpclk2 104.5 MHz max
unsigned long stm32mp1_get_timg2_freq(void)
{
	const unsigned g2 = (RCC->TIMG2PRER & RCC_TIMG2PRER_TIMG2PRE_Msk) != 0;
	switch ((RCC->APB2DIVR & RCC_APB2DIVR_APB2DIV_Msk) >> RCC_APB2DIVR_APB2DIV_Pos)
	{
	default:
	case 0x00: return stm32mp1_get_mlhclk_freq();
	case 0x01: return stm32mp1_get_mlhclk_freq();
	case 0x02: return g2 ? stm32mp1_get_mlhclk_freq() : (stm32mp1_get_mlhclk_freq() / 2);
	case 0x03: return g2 ? (stm32mp1_get_mlhclk_freq() / 2) : (stm32mp1_get_mlhclk_freq() / 4);
	case 0x04: return g2 ? (stm32mp1_get_mlhclk_freq() / 4) : (stm32mp1_get_mlhclk_freq() / 8);
	}
}

unsigned long stm32mp1_get_spi1_freq(void)
{
	//	0x0: pll4_p_ck clock selected as kernel peripheral clock (default after reset)
	//	0x1: pll3_q_ck clock selected as kernel peripheral clock
	//	0x2: I2S_CKIN clock selected as kernel peripheral clock
	//	0x3: per_ck clock selected as kernel peripheral clock
	//	0x4: pll3_r_ck clock selected as kernel peripheral clock
	//	others: reserved, the kernel clock is disabled
	switch ((RCC->SPI2S1CKSELR & RCC_SPI2S1CKSELR_SPI1SRC_Msk) >> RCC_SPI2S1CKSELR_SPI1SRC_Pos)
	{
	case 0x00: return stm32mp1_get_pll4_p_freq();
	case 0x01: return stm32mp1_get_pll3_q_freq();
#if (defined BOARD_I2S_CKIN_FREQ)
	case 0x02: return BOARD_I2S_CKIN_FREQ;
#endif /* (defined BOARD_I2S_CKIN_FREQ) */
	case 0x03: return stm32mp1_get_per_freq();
	case 0x04: return stm32mp1_get_pll3_r_freq();
	default: return stm32mp1_get_per_freq();
	}
}

unsigned long stm32mp1_get_adc_freq(void)
{
	//	0x0: pll4_r_ck clock selected as kernel peripheral clock (default after reset)
	//	0x1: per_ck clock selected as kernel peripheral clock
	//	0x2: pll3_q_ck clock selected as kernel peripheral clock
	//	others: the kernel clock is disabled
	switch ((RCC->ADCCKSELR & RCC_ADCCKSELR_ADCSRC_Msk) >> RCC_ADCCKSELR_ADCSRC_Pos)
	{
	case 0x00: return stm32mp1_get_pll4_r_freq();
	case 0x01: return stm32mp1_get_per_freq();
	case 0x02: return stm32mp1_get_pll3_q_freq();
	default: return stm32mp1_get_per_freq();
	}
}

// получение тактовой частоты тактирования блока SPI, использующенося в данной конфигурации
unsigned long hardware_get_spi_freq(void)
{
	return stm32mp1_get_spi1_freq();
}

#define BOARD_SYSTICK_FREQ CPU_FREQ


 // возврат позиции старшего значащего бита в числе
static uint_fast8_t countbits2( unsigned long v	)
{
	uint_fast8_t n;

	for (n = 0; v != 0; ++ n)
		v >>= 1;

	return n;
}


// Вариант функции для расчёта делителя определяющего скорость передачи
// на UART AT91SAM7S, делитель для АЦП ATMega (значение делителя не требуется уменьшать на 1).
static uint_fast8_t
//NOINLINEAT
calcdivider(
	uint_fast32_t divisor, // ожидаемый коэффициент деления всей системы
	uint_fast8_t width,			// количество разрядов в счётчике
	uint_fast16_t taps,			// маска битов - выходов прескалера. 0x01 - означает bypass, 0x02 - делитель на 2... 0x400 - делитель на 1024
	unsigned * dvalue,		// Значение для записи в регистр сравнения делителя
	uint_fast8_t substract)
{
	const uint_fast8_t rbmax = 16; //позиция старшего значащего бита в маске TAPS
	uint_fast8_t rb, rbi;
	uint_fast16_t prescaler = 1U;

	for (rb = rbi = 0; rb <= rbmax; ++ rb, prescaler *= 2)
	{
		if ((taps & prescaler) != 0)
		{
			// такой предделитель существует.
			const uint_fast32_t modulus = ((divisor + prescaler / 2) / prescaler) - substract;
			if (countbits2(modulus) <= width)
			{
				// найдена подходящая комбинация
				// rb - степень двойки - деление предделителя.
				// rbi - номер кода для записи в регистр предделителя.
				// modulus - что загрузить в регистр сравнения делителя.
				* dvalue = (unsigned) modulus;
				return rbi;
			}
			++ rbi;		// переходим к следующему предделителю в списке.
		}
	}
	// Не подобрать комбинацию прескалера и делителя для ожидаемого коэффициента деления.
	* dvalue = (1U << width) - 1;	// просто пустышка - максимальный делитель
	return (rbi - 1);	// если надо обраьатывать невозможность подбора - возврат rbmax
}

static uint_fast32_t stm32f7xx_pllq_initialize(void);	// Настроить выход PLLQ на 48 МГц

	// В stm32 три вида таймеров - General-purpose (2..5, 9..14), Advanced-control (1 & 8) & Basic (6 & 7)
	enum
	{
		// General-purpose timers (TIM2/TIM3/TIM4/TIM5)
		STM32F_TIM2_TIMER_WIDTH = 32,	STM32F_TIM2_TIMER_TAPS = (65535), // General-purpose timers TIM2 and TIM5 on CPUSTYLE_STM32F4XX
		STM32F_TIM5_TIMER_WIDTH = 32,	STM32F_TIM5_TIMER_TAPS = (65535), // General-purpose timers TIM2 and TIM5 on CPUSTYLE_STM32F4XX
		STM32F_TIM1_TIMER_WIDTH = 16,	STM32F_TIM1_TIMER_TAPS = (65535),
		STM32F_TIM3_TIMER_WIDTH = 16,	STM32F_TIM3_TIMER_TAPS = (65535),
		STM32F_TIM4_TIMER_WIDTH = 16,	STM32F_TIM4_TIMER_TAPS = (65535),

		STM32F_SPIBR_WIDTH = 0,	STM32F_SPIBR_TAPS = (256 | 128 | 64 | 32 | 16 | 8 | 4 | 2),

		STM32F_fillPAD
	};

void TIM5_IRQHandler(void)
{
	const portholder_t st = TIM5->SR;
	if ((st & TIM_SR_UIF) != 0)
	{
		TIM5->SR = ~ TIM_SR_UIF;	// clear UIF interrupt request
		spool_systimerbundle1();	// При возможности вызываются столько раз, сколько произошло таймерных прерываний.
		spool_systimerbundle2();	// Если пропущены прерывания, компенсировать дополнительными вызовами нет смысла.
	}
	else
	{
		// ASSERT(0);
	}
}

static uint_fast32_t gtimloadvalue;

void SecurePhysicalTimer_IRQHandler(void)
{
	//IRQ_ClearPending (SecurePhysicalTimer_IRQn);
	PL1_SetLoadValue(gtimloadvalue);

	spool_systimerbundle1();	// При возможности вызываются столько раз, сколько произошло таймерных прерываний.
	spool_systimerbundle2();	// Если пропущены прерывания, компенсировать дополнительными вызовами нет смысла.
}


// Инициализация таймера, вызывающего прерывания с частотой TICKS_FREQUENCY.
// Должо вызываться позже настройки ADC
void hardware_timer_initialize(uint_fast32_t ticksfreq)
{
	//tickers_initialize();

	RCC->MP_APB1ENSETR = RCC_MP_APB1ENSETR_TIM5EN;   // подаем тактирование на TIM5
	(void) RCC->MP_APB1ENSETR;
	RCC->MP_APB1LPENSETR = RCC_MP_APB1LPENSETR_TIM5LPEN;   // подаем тактирование на TIM5
	(void) RCC->MP_APB1LPENSETR;

	TIM5->DIER = TIM_DIER_UIE;        	 // разрешить событие от таймера

	// Use basic timer
	unsigned value;
	const uint_fast8_t prei = calcdivider(calcdivround2(BOARD_TIM5_FREQ, ticksfreq), STM32F_TIM5_TIMER_WIDTH, STM32F_TIM5_TIMER_TAPS, & value, 1);

	TIM5->PSC = ((1UL << prei) - 1);
	TIM5->ARR = value;
	TIM5->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE; /* разрешить перезагрузку и включить таймер = перенесено в установку скорости - если счётчик успевал превысить значение ARR - считал до конца */

	arm_hardware_set_handler_system(TIM5_IRQn, TIM5_IRQHandler);

	// Prepare funcionality: use CNTP
	const uint_fast32_t gtimfreq = stm32mp1_get_hsi_freq();

	PL1_SetCounterFrequency(gtimfreq);	// CNTFRQ

	gtimloadvalue = calcdivround2(gtimfreq, ticksfreq) - 1;
	// Private timer use
	// Disable Private Timer and set load value
	PL1_SetControl(PL1_GetControl() & ~ 0x01);	// CNTP_CTL
	PL1_SetLoadValue(gtimloadvalue);	// CNTP_TVAL
	// Enable timer control
	PL1_SetControl(PL1_GetControl() | 0x01);

	//arm_hardware_set_handler_system(SecurePhysicalTimer_IRQn, SecurePhysicalTimer_IRQHandler);

}


// return 1 if CPU supports 800 MHz clock
uint_fast8_t stm32mp1_overdrived(void)
{
	RCC->MP_APB5ENSETR = RCC_MP_APB5ENSETR_BSECEN;
	(void) RCC->MP_APB5ENSETR;
	RCC->MP_APB5LPENSETR = RCC_MP_APB5LPENSETR_BSECLPEN;
	(void) RCC->MP_APB5LPENSETR;

	const unsigned rpn = ((* (volatile uint32_t *) RPN_BASE) & RPN_ID_Msk) >> RPN_ID_Pos;
	return (rpn & 0x80) != 0;
}

// Снижение тактовой частоты процессора для уменьшения потребления
void stm32mp1_pll1_slow(uint_fast8_t slow)
{
	//	0x0: HSI selected as MPU sub-system clock (hsi_ck) (default after reset)
	//	0x1: HSE selected as MPU sub-system clock (hse_ck)
	//	0x2: PLL1 selected as MPU sub-system clock (pll1_p_ck)
	//	0x3: PLL1 via MPUDIV is selected as MPU sub-system clock (pll1_p_ck / (2 ^ MPUDIV)).
	RCC->MPCKSELR = (RCC->MPCKSELR & ~ (RCC_MPCKSELR_MPUSRC_Msk)) |
			((slow ? 0x03uL : 0x02uL) << RCC_MPCKSELR_MPUSRC_Pos) |
			0;
	while((RCC->MPCKSELR & RCC_MPCKSELR_MPUSRCRDY_Msk) == 0)
		;
}


void stm32mp1_pll_initialize(void)
{

	//return;
	// PLL1 DIVN=0x1f. DIVM=0x4, DIVP=0x0
	// HSI 64MHz/5*32 = 409.6 MHz
	// HSI 64MHz/5*42 = 537.6 MHz
	//RCC->MP_APB5ENSETR = RCC_MP_APB5ENSETR_TZPCEN;
	//(void) RCC->MP_APB5ENSETR;
	RCC->TZCR &= ~ (RCC_TZCR_MCKPROT);
	//RCC->TZCR &= ~ (RCC_TZCR_TZEN);

	// compensation cell enable
	if (1)
	{
	//	Sequence to enable IO compensation:
	//	1. Ensure the CSI oscillator is enabled and ready (in RCC)
	//	2. Set SYSCFG_CMPENSETR.MCU_EN or MPU_EN=1
	//	3. Wait SYSCFG_CMPCR.READY = 1
	//	4. Set SYSCFG_CMPCR.SW_CTRL = 0

		/* SYSCFG clock enable */
		RCC->MP_APB3ENSETR = RCC_MP_APB3ENSETR_SYSCFGEN;
		(void) RCC->MP_APB3ENSETR;
		RCC->MP_APB3LPENSETR = RCC_MP_APB3LPENSETR_SYSCFGLPEN;
		(void) RCC->MP_APB3LPENSETR;

		// CSI ON (The CSI oscillator must be enabled and ready (controlled in RCC) before MPU_EN could be set to 1)
		RCC->OCENSETR = RCC_OCENSETR_CSION;
		(void) RCC->OCENSETR;
		while ((RCC->OCRDYR & RCC_OCRDYR_CSIRDY) == 0)
			;

		// IO compoensation cell enable
		SYSCFG->CMPENSETR = SYSCFG_CMPENSETR_MPU_EN;
		(void) SYSCFG->CMPENSETR;

		while ((SYSCFG->CMPCR & SYSCFG_CMPCR_READY_Msk) == 0)
			;

		SYSCFG->CMPCR = (SYSCFG->CMPCR & SYSCFG_CMPCR_SW_CTRL_Msk) |
			//(1uL << SYSCFG_CMPCR_SW_CTRL_Pos) |	// 1: IO compensation values come from RANSRC[3:0] and RAPSRC[3:0]
			(0uL << SYSCFG_CMPCR_SW_CTRL_Pos) |	// 0: IO compensation values come from ANSRC[3:0] and APSRC[3:0]
			0;
	}

	// переключение на HSI на всякий случай перед программированием PLL
	// HSI ON
	RCC->OCENSETR = RCC_OCENSETR_HSION;
	(void) RCC->OCENSETR;
	while ((RCC->OCRDYR & RCC_OCRDYR_HSIRDY) == 0)
		;

	// Wait for HSI ready
	while ((RCC->OCRDYR & RCC_OCRDYR_HSIRDY_Msk) == 0)
		;

	// HSIDIV
	//	0x0: Division by 1, hsi_ck (hsi_ker_ck) = 64 MHz (default after reset)
	//	0x1: Division by 2, hsi_ck (hsi_ker_ck) = 32 MHz
	//	0x2: Division by 4, hsi_ck (hsi_ker_ck) = 16 MHz
	//	0x3: Division by 8, hsi_ck (hsi_ker_ck) = 8 MHz
	RCC->HSICFGR = (RCC->HSICFGR & ! (RCC_HSICFGR_HSIDIV_Msk)) |
		(0uL << RCC_HSICFGR_HSIDIV_Pos) |
		0;
	(void) RCC->HSICFGR;

	// Wait for HSI DIVIDER ready
	while ((RCC->OCRDYR & RCC_OCRDYR_HSIDIVRDY_Msk) == 0)
		;

	//0x0: HSI selected as AXI sub-system clock (hsi_ck) (default after reset)
	//0x1: HSE selected as AXI sub-system clock (hse_ck)
	//0x2: PLL2 selected as AXI sub-system clock (pll2_p_ck)
	//others: axiss_ck is gated
	RCC->ASSCKSELR = (RCC->ASSCKSELR & ~ (RCC_ASSCKSELR_AXISSRC_Msk)) |
			(0x00 << RCC_ASSCKSELR_AXISSRC_Pos) |	// HSI
			0;
	while ((RCC->ASSCKSELR & RCC_ASSCKSELR_AXISSRCRDY_Msk) == 0)
		;

	//	0x0: HSI selected as MPU sub-system clock (hsi_ck) (default after reset)
	//	0x1: HSE selected as MPU sub-system clock (hse_ck)
	//	0x2: PLL1 selected as MPU sub-system clock (pll1_p_ck)
	//	0x3: PLL1 via MPUDIV is selected as MPU sub-system clock (pll1_p_ck / (2 ^ MPUDIV)).
	RCC->MPCKSELR = (RCC->MPCKSELR & ~ (RCC_MPCKSELR_MPUSRC_Msk)) |
		((uint_fast32_t) 0x00 << RCC_MPCKSELR_MPUSRC_Pos) |	// HSI
		0;
	while((RCC->MPCKSELR & RCC_MPCKSELR_MPUSRCRDY_Msk) == 0)
		;

	//	0x0: HSI selected as MCU sub-system clock (hsi_ck) (default after reset)
	//	0x1: HSE selected as MCU sub-system clock (hse_ck)
	//	0x2: CSI selected as MCU sub-system clock (csi_ck)
	//	0x3: PLL3 selected as MCU sub-system clock (pll3_p_ck).
	RCC->MSSCKSELR = (RCC->MSSCKSELR & ~ (RCC_MSSCKSELR_MCUSSRC_Msk)) |
		(0x00 << RCC_MSSCKSELR_MCUSSRC_Pos) |	// HSI
		0;
	while((RCC->MSSCKSELR & RCC_MSSCKSELR_MCUSSRCRDY_Msk) == 0)
		;

	// Для работы функции снижения тактовой частоты процессора
	//	0x0: The MPUDIV is disabled; i.e. no clock generated
	//	0x1: The mpuss_ck is equal to pll1_p_ck divided by 2 (default after reset)
	//	0x2: The mpuss_ck is equal to pll1_p_ck divided by 4
	//	0x3: The mpuss_ck is equal to pll1_p_ck divided by 8
	//	others: The mpuss_ck is equal to pll1_p_ck divided by 16
	RCC->MPCKDIVR = (RCC->MPCKDIVR &= ~ (RCC_MPCKDIVR_MPUDIV_Msk)) |
		(0x02uL << RCC_MPCKDIVR_MPUDIV_Pos) |	// pll1_p_ck divided by 4
		0;

	// Stop PLL4
	RCC->PLL4CR &= ~ RCC_PLL4CR_PLLON_Msk;
	(void) RCC->PLL4CR;
	while ((RCC->PLL4CR & RCC_PLL4CR_PLLON_Msk) != 0)
		;

	// Stop PLL3
	RCC->PLL3CR &= ~ RCC_PLL3CR_PLLON_Msk;
	(void) RCC->PLL3CR;
	while ((RCC->PLL3CR & RCC_PLL3CR_PLLON_Msk) != 0)
		;

	// Stop PLL2
	RCC->PLL2CR &= ~ RCC_PLL2CR_PLLON_Msk;
	(void) RCC->PLL2CR;
	while ((RCC->PLL2CR & RCC_PLL2CR_PLLON_Msk) != 0)
		;

	// Stop PLL1
	//RCC->PLL1CR &= ~ RCC_PLL1CR_DIVPEN_Msk;
	//(void) RCC->PLL1CR;
	RCC->PLL1CR &= ~ RCC_PLL1CR_PLLON_Msk;
	(void) RCC->PLL1CR;
	while ((RCC->PLL1CR & RCC_PLL1CR_PLLON_Msk) != 0)
		;

	#if WITHCPUXOSC
		// с внешним генератором
		RCC->OCENCLRR = RCC_OCENCLRR_HSEON;
		(void) RCC->OCENCLRR;
		while ((RCC->OCRDYR & RCC_OCRDYR_HSERDY) != 0)
			;
		RCC->OCENSETR = RCC_OCENSETR_DIGBYP;
		(void) RCC->OCENSETR;
		RCC->OCENSETR = RCC_OCENSETR_HSEBYP;
		(void) RCC->OCENSETR;
		RCC->OCENSETR = RCC_OCENSETR_HSEON;
		(void) RCC->OCENSETR;
		while ((RCC->OCRDYR & RCC_OCRDYR_HSERDY) == 0)
			;

	#elif WITHCPUXTAL
		//#error rr2
		// с внешним кварцем
		// HSE ON
		RCC->OCENSETR = RCC_OCENSETR_HSEON;
		(void) RCC->OCENSETR;
		while ((RCC->OCRDYR & RCC_OCRDYR_HSERDY) == 0)
			;

	#else
		// На внутреннем генераторе
		// выключаем внешний
		RCC->OCENCLRR = RCC_OCENCLRR_HSEON;
		(void) RCC->OCENCLRR;

	#endif /* WITHCPUXTAL */

#if 1 // pll1
	// PLL1, PLL2 source mux
	// 0x0: HSI selected as PLL clock (hsi_ck) (default after reset)
	// 0x1: HSE selected as PLL clock (hse_ck)
	RCC->RCK12SELR = (RCC->RCK12SELR & ~ (RCC_RCK12SELR_PLL12SRC_Msk)) |
	#if WITHCPUXOSC || WITHCPUXTAL
		// с внешним генератором
		// с внешним кварцем
		(0x01 << RCC_RCK12SELR_PLL12SRC_Pos) |	// HSE
	#else
		// На внутреннем генераторе
		(0x00 << RCC_RCK12SELR_PLL12SRC_Pos) |	// HSI
	#endif
		0;
	while ((RCC->RCK12SELR & RCC_RCK12SELR_PLL12SRCRDY_Msk) == 0)
		;

	RCC->PLL1CR = (RCC->PLL1CR & ~ (RCC_PLL1CR_DIVPEN_Msk | RCC_PLL1CR_DIVQEN_Msk | RCC_PLL1CR_DIVREN_Msk));
	(void) RCC->PLL1CR;

	RCC->PLL1CFGR1 = (RCC->PLL1CFGR1 & ~ (RCC_PLL1CFGR1_DIVN_Msk | RCC_PLL1CFGR1_DIVM1_Msk)) |
		((uint_fast32_t) (PLL1DIVM - 1) << RCC_PLL1CFGR1_DIVM1_Pos) |
		((uint_fast32_t) (PLL1DIVN - 1) << RCC_PLL1CFGR1_DIVN_Pos) |
		0;
	(void) RCC->PLL1CFGR1;

	RCC->PLL1CFGR2 = (RCC->PLL1CFGR2 & ~ (RCC_PLL1CFGR2_DIVP_Msk | RCC_PLL1CFGR2_DIVQ_Msk | RCC_PLL1CFGR2_DIVR_Msk)) |
		((uint_fast32_t) (PLL1DIVP - 1) << RCC_PLL1CFGR2_DIVP_Pos) |
		((uint_fast32_t) (PLL1DIVQ - 1) << RCC_PLL1CFGR2_DIVQ_Pos) |
		((uint_fast32_t) (PLL1DIVR - 1) << RCC_PLL1CFGR2_DIVR_Pos) |
		0;
	(void) RCC->PLL1CFGR2;

	RCC->PLL1CR |= RCC_PLL1CR_PLLON_Msk;
	(void) RCC->PLL1CR;
	while ((RCC->PLL1CR & RCC_PLL1CR_PLL1RDY_Msk) == 0)
		;

	RCC->PLL1CR &= ~ RCC_PLL1CR_SSCG_CTRL_Msk;
	(void) RCC->PLL1CR;

	RCC->PLL1CR |= RCC_PLL1CR_DIVPEN_Msk;	// P output enable
	(void) RCC->PLL1CR;
#endif // pll1

#if 1 // PLL2
	// PLL2
	RCC->PLL2CR = (RCC->PLL2CR & ~ (RCC_PLL2CR_DIVPEN_Msk | RCC_PLL2CR_DIVQEN_Msk | RCC_PLL2CR_DIVREN_Msk));
	(void) RCC->PLL2CR;

	RCC->PLL2CFGR1 = (RCC->PLL2CFGR1 & ~ (RCC_PLL2CFGR1_DIVN_Msk | RCC_PLL2CFGR1_DIVM2_Msk)) |
		((PLL2DIVN - 1) << RCC_PLL2CFGR1_DIVN_Pos) |
		((PLL2DIVM - 1) << RCC_PLL2CFGR1_DIVM2_Pos) |
		0;
	(void) RCC->PLL2CFGR1;

	RCC->PLL2CFGR2 = (RCC->PLL2CFGR2 & ~ (RCC_PLL2CFGR2_DIVP_Msk | RCC_PLL2CFGR2_DIVQ_Msk | RCC_PLL2CFGR2_DIVR_Msk)) |
		((uint_fast32_t) (PLL2DIVP - 1) << RCC_PLL2CFGR2_DIVP_Pos) |	// pll2_p_ck - AXI clock (1..128 -> 0x00..0x7f)
		((uint_fast32_t) (PLL2DIVQ - 1) << RCC_PLL2CFGR2_DIVQ_Pos) |	// GPU clock (1..128 -> 0x00..0x7f)
		((uint_fast32_t) (PLL2DIVR - 1) << RCC_PLL2CFGR2_DIVR_Pos) |	// DDR clock (1..128 -> 0x00..0x7f)
		0;
	(void) RCC->PLL2CFGR2;

	RCC->PLL2CR |= RCC_PLL2CR_PLLON_Msk;
	(void) RCC->PLL2CR;
	while ((RCC->PLL2CR & RCC_PLL2CR_PLL2RDY_Msk) == 0)
		;

	RCC->PLL2CR |= RCC_PLL2CR_DIVPEN_Msk;	// pll2_p_ck - AXI clock
	(void) RCC->PLL2CR;

	RCC->PLL2CR |= RCC_PLL2CR_DIVQEN_Msk;	// GPU clock
	(void) RCC->PLL2CR;

#if 1//WITHSDRAMHW
	// В загрузчике еще может и не быть этой периферии
	RCC->PLL2CR |= RCC_PLL2CR_DIVREN_Msk;	// DDR clock
	(void) RCC->PLL2CR;
#endif /* WITHSDRAMHW */

	RCC->PLL2CR &= ~ RCC_PLL2CR_SSCG_CTRL_Msk;
	(void) RCC->PLL2CR;
#endif // pll2

	// AXI, AHB5 and AHB6 clock divisor
	//	0x0: axiss_ck (default after reset)
	//	0x1: axiss_ck / 2
	//	0x2: axiss_ck / 3
	//	others: axiss_ck / 4
	RCC->AXIDIVR = (RCC->AXIDIVR & ~ (RCC_AXIDIVR_AXIDIV_Msk)) |
		((uint_fast32_t) (0x01 - 1) << RCC_AXIDIVR_AXIDIV_Pos) |	// div1 (no divide)
		0;
	while((RCC->AXIDIVR & RCC_AXIDIVR_AXIDIVRDY_Msk) == 0)
		;

	// APB1 Output divisor (output max 104.5 MHz)
	// Input MLHCK (209 MHz max)
	//0x0: mlhclk (default after reset)
	//0x1: mlhclk / 2
	//0x2: mlhclk / 4
	//0x3: mlhclk / 8
	//0x4: mlhclk / 16
	RCC->APB1DIVR = (RCC->APB1DIVR & ~ (RCC_APB1DIVR_APB1DIV_Msk)) |
		((uint_fast32_t) (1) << RCC_APB1DIVR_APB1DIV_Pos) |	// div2
		0;
	while((RCC->APB1DIVR & RCC_APB1DIVR_APB1DIVRDY_Msk) == 0)
		;

	// APB2 Output divisor (output max 104.5 MHz)
	// Input MLHCK (209 MHz max)
	//0x0: mlhclk (default after reset)
	//0x1: mlhclk / 2
	//0x2: mlhclk / 4
	//0x3: mlhclk / 8
	//0x4: mlhclk / 16
	RCC->APB2DIVR = (RCC->APB2DIVR & ~ (RCC_APB2DIVR_APB2DIV_Msk)) |
		((uint_fast32_t) (1) << RCC_APB2DIVR_APB2DIV_Pos) |	// div2
		0;
	while((RCC->APB2DIVR & RCC_APB2DIVR_APB2DIVRDY_Msk) == 0)
		;

	// APB3 Output divisor (output max 104.5 MHz)
	// Input MLHCK (209 MHz max)
	//0x0: hclk (default after reset)
	//0x1: hclk / 2
	//0x2: hclk / 4
	//0x3: hclk / 8
	//others: hclk / 16
	RCC->APB3DIVR = (RCC->APB3DIVR & ~ (RCC_APB3DIVR_APB3DIV_Msk)) |
		((uint_fast32_t) (1) << RCC_APB3DIVR_APB3DIV_Pos) |	// div2
		0;
	while((RCC->APB3DIVR & RCC_APB3DIVR_APB3DIVRDY_Msk) == 0)
		;

	// APB4 Output divisor
	//	0x0: aclk (default after reset)
	//	0x1: aclk / 2
	//	0x2: aclk / 4
	//	0x3: aclk / 8
	//	others: aclk / 16
	RCC->APB4DIVR = (RCC->APB4DIVR & ~ (RCC_APB4DIVR_APB4DIV_Msk)) |
		((uint_fast32_t) (0x02 - 1) << RCC_APB4DIVR_APB4DIV_Pos) |	// div2
		0;
	while((RCC->APB4DIVR & RCC_APB4DIVR_APB4DIVRDY_Msk) == 0)
		;

	// APB5 Output divisor
	RCC->APB5DIVR = (RCC->APB5DIVR & ~ (RCC_APB5DIVR_APB5DIV_Msk)) |
		((uint_fast32_t) (0x04 - 1) << RCC_APB5DIVR_APB5DIV_Pos) |	// div4
		0;
	while((RCC->APB5DIVR & RCC_APB5DIVR_APB5DIVRDY_Msk) == 0)
		;

	// Значения 0x02 и 0x03 проверены - 0x03 действительно ниже тактовая
	//	0x0: HSI selected as MPU sub-system clock (hsi_ck) (default after reset)
	//	0x1: HSE selected as MPU sub-system clock (hse_ck)
	//	0x2: PLL1 selected as MPU sub-system clock (pll1_p_ck)
	//	0x3: PLL1 via MPUDIV is selected as MPU sub-system clock (pll1_p_ck / (2 ^ MPUDIV)).
	RCC->MPCKSELR = (RCC->MPCKSELR & ~ (RCC_MPCKSELR_MPUSRC_Msk)) |
		((uint_fast32_t) 0x02uL << RCC_MPCKSELR_MPUSRC_Pos) |	// PLL1_P
		0;
	while((RCC->MPCKSELR & RCC_MPCKSELR_MPUSRCRDY_Msk) == 0)
		;

	// per_ck source clock selection
	//0x0: hsi_ker_ck clock selected (default after reset)
	//0x1: csi_ker_ck clock selected
	//0x2: hse_ker_ck clock selected
	//0x3: Clock disabled
	RCC->CPERCKSELR = (RCC->CPERCKSELR & ~ (RCC_CPERCKSELR_CKPERSRC_Msk)) |
		(0x00 << RCC_CPERCKSELR_CKPERSRC_Pos) |	// hsi_ker_ck
		0;
	(void) RCC->CPERCKSELR;

	//0x0: HSI selected as AXI sub-system clock (hsi_ck) (default after reset)
	//0x1: HSE selected as AXI sub-system clock (hse_ck)
	//0x2: PLL2 selected as AXI sub-system clock (pll2_p_ck)
	//others: axiss_ck is gated
	// axiss_ck 266 MHz Max
	RCC->ASSCKSELR = (RCC->ASSCKSELR & ~ (RCC_ASSCKSELR_AXISSRC_Msk)) |
			((uint_fast32_t) 0x02 << RCC_ASSCKSELR_AXISSRC_Pos) |	// pll2_p_ck
			0;
	while ((RCC->ASSCKSELR & RCC_ASSCKSELR_AXISSRCRDY_Msk) == 0)
		;

	// PLL3
	// PLL3 source mux
	//0x0: HSI selected as PLL clock (hsi_ck) (default after reset)
	//0x1: HSE selected as PLL clock (hse_ck)
	//0x2: CSI selected as PLL clock (csi_ck)
	//0x3: No clock send to DIVMx divisor and PLLs
	RCC->RCK3SELR = (RCC->RCK3SELR & ~ (RCC_RCK3SELR_PLL3SRC_Msk)) |
	#if WITHCPUXOSC || WITHCPUXTAL
		// с внешним генератором
		// с внешним кварцем
		((uint_fast32_t) 0x01 << RCC_RCK3SELR_PLL3SRC_Pos) |	// HSE
	#else
		// На внутреннем генераторе
		((uint_fast32_t) 0x00 << RCC_RCK3SELR_PLL3SRC_Pos) |	// HSI
	#endif
		0;
	while ((RCC->RCK3SELR & RCC_RCK3SELR_PLL3SRCRDY_Msk) == 0)
		;

#if 1//WITHUART1HW
	// В загрузчике еще может и не быть этой периферии
	// usart1
	//	0x0: pclk5 clock selected as kernel peripheral clock (default after reset)
	//	0x1: pll3_q_ck clock selected as kernel peripheral clock
	//	0x2: hsi_ker_ck clock selected as kernel peripheral clock
	//	0x3: csi_ker_ck clock selected as kernel peripheral clock
	//	0x4: pll4_q_ck clock selected as kernel peripheral clock
	//	0x5: hse_ker_ck clock selected as kernel peripheral clock
	RCC->UART1CKSELR = (RCC->UART1CKSELR & ~ (RCC_UART1CKSELR_UART1SRC_Msk)) |
		(0x02 << RCC_UART1CKSELR_UART1SRC_Pos) | // HSI
		0;
	(void) RCC->UART1CKSELR;
#endif /* WITHUART1HW */

#if 1//WITHUART2HW || WITHUART4HW
	// В загрузчике еще может и не быть этой периферии
	// UART2, UART4
	//	0x0: pclk1 clock selected as kernel peripheral clock (default after reset)
	//	0x1: pll4_q_ck clock selected as kernel peripheral clock
	//	0x2: hsi_ker_ck clock selected as kernel peripheral clock
	//	0x3: csi_ker_ck clock selected as kernel peripheral clock
	//	0x4: hse_ker_ck clock selected as kernel peripheral clock
	RCC->UART24CKSELR = (RCC->UART24CKSELR & ~ (RCC_UART24CKSELR_UART24SRC_Msk)) |
		((uint_fast32_t) 0x02 << RCC_UART24CKSELR_UART24SRC_Pos) |	// hsi_ker_ck
		//((uint_fast32_t) 0x00 << RCC_UART24CKSELR_UART24SRC_Pos) |	// pclk1
		0;
	(void) RCC->UART24CKSELR;
#endif /* WITHUART2HW || WITHUART4HW */

#if 1//WITHUART3HW || WITHUART5HW
	// В загрузчике еще может и не быть этой периферии
	// UART3, UART5
	//	0x0: pclk1 clock selected as kernel peripheral clock (default after reset)
	//	0x1: pll4_q_ck clock selected as kernel peripheral clock
	//	0x2: hsi_ker_ck clock selected as kernel peripheral clock
	//	0x3: csi_ker_ck clock selected as kernel peripheral clock
	//	0x4: hse_ker_ck clock selected as kernel peripheral clock
	//	others: reserved, the kernel clock is disabled
	RCC->UART35CKSELR = (RCC->UART35CKSELR & ~ (RCC_UART35CKSELR_UART35SRC_Msk)) |
		(0x02 << RCC_UART35CKSELR_UART35SRC_Pos) |	// hsi_ker_ck
		//(0x00 << RCC_UART35CKSELR_UART35SRC_Pos) |	// pclk1
		0;
	(void) RCC->UART35CKSELR;
#endif /* WITHUART3HW || WITHUART5HW */

#if 1//WITHUART7HW || WITHUART8HW
	// В загрузчике еще может и не быть этой периферии
	// UART7, UART8
	//0x0: pclk1 clock selected as kernel peripheral clock (default after reset)
	//0x1: pll4_q_ck clock selected as kernel peripheral clock
	//0x2: hsi_ker_ck clock selected as kernel peripheral clock
	//0x3: csi_ker_ck clock selected as kernel peripheral clock
	//0x4: hse_ker_ck clock selected as kernel peripheral clock
	RCC->UART78CKSELR = (RCC->UART78CKSELR & ~ (RCC_UART78CKSELR_UART78SRC_Msk)) |
		((uint_fast32_t) 0x02 << RCC_UART78CKSELR_UART78SRC_Pos) |	// hsi_ker_ck
		//((uint_fast32_t) 0x00 << RCC_UART78CKSELR_UART78SRC_Pos) |	// pclk1
		0;
	(void) RCC->UART78CKSELR;
#endif /* WITHUART7HW || WITHUART8HW */

#if 1//WITHSDHCHW
	// В загрузчике еще может и не быть этой периферии
	// SDMMC1
	//	0x0: hclk6 clock selected as kernel peripheral clock
	//	0x1: pll3_r_ck clock selected as kernel peripheral clock
	//	0x2: pll4_p_ck clock selected as kernel peripheral clock
	//	0x3: hsi_ker_ck clock selected as kernel peripheral clock (default after reset)
	//	others: reserved, the kernel clock is disabled
	RCC->SDMMC12CKSELR = (RCC->SDMMC12CKSELR & ~ (RCC_SDMMC12CKSELR_SDMMC12SRC_Msk)) |
		((uint_fast32_t) 0x3 << RCC_SDMMC12CKSELR_SDMMC12SRC_Pos) |	// hsi_ker_ck
		0;
	(void) RCC->SDMMC12CKSELR;
#endif /* WITHSDHCHW */

#if 1//WITHSPIHW
	// В загрузчике еще может и не быть этой периферии
	//0x0: pll4_p_ck clock selected as kernel peripheral clock (default after reset)
	//0x1: pll3_q_ck clock selected as kernel peripheral clock
	//0x2: I2S_CKIN clock selected as kernel peripheral clock
	//0x3: per_ck clock selected as kernel peripheral clock
	//0x4: pll3_r_ck clock selected as kernel peripheral clock
	RCC->SPI2S1CKSELR = (RCC->SPI2S1CKSELR & ~ (RCC_SPI2S1CKSELR_SPI1SRC_Msk)) |
		((uint_fast32_t) 0x03 << RCC_SPI2S1CKSELR_SPI1SRC_Pos) |	// per_ck
		0;
	(void) RCC->SPI2S1CKSELR;
#endif /* WITHSPIHW */

#if 1//WIHSPIDFHW
	// В загрузчике еще может и не быть этой периферии
	//0x0: aclk clock selected as kernel peripheral clock (default after reset)
	//0x1: pll3_r_ck clock selected as kernel peripheral clock
	//0x2: pll4_p_ck clock selected as kernel peripheral clock
	//0x3: per_ck clock selected as kernel peripheral clock
	RCC->QSPICKSELR = (RCC->QSPICKSELR & ~ (RCC_QSPICKSELR_QSPISRC_Msk)) |
	((uint_fast32_t) 0x00 << RCC_QSPICKSELR_QSPISRC_Pos) |	// aclk (AXI CLOCK)
		0;
	(void) RCC->QSPICKSELR;
#endif /* WIHSPIDFHW */

	// prescaler value of timers located into APB1 domain
	// TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM12, TIM13 and TIM14.
	//	0: The Timers kernel clock is equal to mlhclk if APB1DIV is corresponding to a division by 1
	//	or 2, else it is equal to 2 x Fpclk1 (default after reset)
	//	1: The Timers kernel clock is equal to mlhclk if APB1DIV is corresponding to division by 1, 2
	//	or 4, else it is equal to 4 x Fpclk1
	RCC->TIMG1PRER = (RCC->TIMG1PRER & ~ (RCC_TIMG1PRER_TIMG1PRE_Msk)) |
		((uint_fast32_t) 0x00 << RCC_TIMG1PRER_TIMG1PRE_Pos) |
		0;
	(void) RCC->TIMG1PRER;
	while ((RCC->TIMG1PRER & RCC_TIMG1PRER_TIMG1PRERDY_Msk) == 0)
		;

	// TIM1, TIM8, TIM15, TIM16, and TIM17.
	//	0: The Timers kernel clock is equal to mlhclk if APB2DIV is corresponding to a division by 1
	//	or 2, else it is equal to 2 x Fpclk2 (default after reset)
	//	1: The Timers kernel clock is equal to mlhclk if APB2DIV is corresponding to division by 1, 2
	//	or 4, else it is equal to 4 x Fpclk2
	RCC->TIMG2PRER = (RCC->TIMG2PRER & ~ (RCC_TIMG2PRER_TIMG2PRE_Msk)) |
		(0x00 << RCC_TIMG2PRER_TIMG2PRE_Pos) |
		0;
	(void) RCC->TIMG2PRER;
	while ((RCC->TIMG2PRER & RCC_TIMG2PRER_TIMG2PRERDY_Msk) == 0)
		;

	//	ADC1 and 2 kernel clock source selection
	//	Set and reset by software.
	//	0x0: pll4_r_ck clock selected as kernel peripheral clock (default after reset)
	//	0x1: per_ck clock selected as kernel peripheral clock
	//	0x2: pll3_q_ck clock selected as kernel peripheral
	RCC->ADCCKSELR = (RCC->ADCCKSELR & ~ (RCC_ADCCKSELR_ADCSRC_Msk)) |
		(0x01 << RCC_ADCCKSELR_ADCSRC_Pos) |
		0;
	(void) RCC->ADCCKSELR;

}

void stm32mp1_usb_clocks_initialize(void)
{
	if (RCC_USBCKSELR_USBOSRC_VAL == 0x00 || RCC_USBCKSELR_USBPHYSRC_VAL == 0x01)
	{
		// Stop PLL4
		RCC->PLL4CR &= ~ RCC_PLL4CR_PLLON_Msk;
		(void) RCC->PLL4CR;
		while ((RCC->PLL4CR & RCC_PLL4CR_PLLON_Msk) != 0)
			;
		// PLL4 source mux
		//	0x0: HSI selected as PLL clock (hsi_ck) (default after reset)
		//	0x1: HSE selected as PLL clock (hse_ck)
		//	0x2: CSI selected as PLL clock (csi_ck)
		//	0x3: Signal I2S_CKIN used as reference clock
		RCC->RCK4SELR = (RCC->RCK4SELR & ~ (RCC_RCK4SELR_PLL4SRC_Msk)) |
		#if WITHCPUXOSC || WITHCPUXTAL
			// с внешним генератором
			// с внешним кварцем
			((uint_fast32_t) 0x01 << RCC_RCK4SELR_PLL4SRC_Pos) |	// HSE
		#else
			// На внутреннем генераторе
			((uint_fast32_t) 0x00 << RCC_RCK4SELR_PLL4SRC_Pos) |	// HSI
		#endif
			0;
		while ((RCC->RCK4SELR & RCC_RCK4SELR_PLL4SRCRDY_Msk) == 0)
			;

		RCC->PLL4CFGR1 = (RCC->PLL4CFGR1 & ~ (RCC_PLL4CFGR1_DIVN_Msk | RCC_PLL4CFGR1_DIVM4_Msk)) |
	        ((uint_fast32_t) (PLL4DIVN - 1) << RCC_PLL4CFGR1_DIVN_Pos) |
	        ((uint_fast32_t) (PLL4DIVM - 1) << RCC_PLL4CFGR1_DIVM4_Pos) |
	        0;
	    (void) RCC->PLL4CFGR1;

	    RCC->PLL4CFGR2 = (RCC->PLL4CFGR2 & ~ (RCC_PLL4CFGR2_DIVR_Msk)) |
			((uint_fast32_t) (PLL4DIVR - 1) << RCC_PLL4CFGR2_DIVR_Pos) |	// USBPHY clock (1..128 -> 0x00..0x7f)
			0;
		(void) RCC->PLL4CFGR2;

		RCC->PLL4CR |= RCC_PLL4CR_DIVREN_Msk;	// USBPHY clock
		(void) RCC->PLL4CR;

		// Start PLL4
		RCC->PLL4CR |= RCC_PLL4CR_PLLON_Msk;
		while ((RCC->PLL4CR & RCC_PLL4CR_PLL4RDY_Msk) == 0)
			;
	}

	//	In addition, if the USBO is used in full-speed mode only, the application can choose the
	//	48 MHz clock source to be provided to the USBO:
	// USBOSRC
	//	0: pll4_r_ck clock selected as kernel peripheral clock (default after reset)
	//	1: clock provided by the USB PHY (rcc_ck_usbo_48m) selected as kernel peripheral clock
	// USBPHYSRC
	//  0x0: hse_ker_ck clock selected as kernel peripheral clock (default after reset)
	//  0x1: pll4_r_ck clock selected as kernel peripheral clock
	//  0x2: hse_ker_ck/2 clock selected as kernel peripheral clock
	RCC->USBCKSELR = (RCC->USBCKSELR & ~ (RCC_USBCKSELR_USBOSRC_Msk | RCC_USBCKSELR_USBPHYSRC_Msk)) |
		(RCC_USBCKSELR_USBOSRC_VAL << RCC_USBCKSELR_USBOSRC_Pos) |	// 50 MHz max rcc_ck_usbo_48m or pll4_r_ck (можно использовать только 48 МГц)
		(RCC_USBCKSELR_USBPHYSRC_VAL << RCC_USBCKSELR_USBPHYSRC_Pos) |	// 38.4 MHz max hse_ker_ck	- входная частота для PHYC PLL
		0;
	(void) RCC->USBCKSELR;

}

void stm32mp1_audio_clocks_initialize(void)
{
	// Stop PLL3
	RCC->PLL3CR &= ~ RCC_PLL3CR_PLLON_Msk;
	(void) RCC->PLL3CR;
	while ((RCC->PLL3CR & RCC_PLL3CR_PLLON_Msk) != 0)
		;
	// PLL3
	// PLL3 source mux
	//	0x0: HSI selected as PLL clock (hsi_ck) (default after reset)
	//	0x1: HSE selected as PLL clock (hse_ck)
	//	0x2: CSI selected as PLL clock (csi_ck)
	//	0x3: No clock send to DIVMx divider and PLLs
	RCC->RCK3SELR = (RCC->RCK3SELR & ~ (RCC_RCK3SELR_PLL3SRC_Msk)) |
	#if WITHCPUXOSC || WITHCPUXTAL
		// с внешним генератором
		// с внешним кварцем
		((uint_fast32_t) 0x01 << RCC_RCK3SELR_PLL3SRC_Pos) |	// HSE
	#else
		// На внутреннем генераторе
		((uint_fast32_t) 0x00 << RCC_RCK3SELR_PLL3SRC_Pos) |	// HSI
	#endif
		0;
	while ((RCC->RCK3SELR & RCC_RCK3SELR_PLL3SRCRDY_Msk) == 0)
		;

//	RCC->PLL3CR = (RCC->PLL3CR & ~ (RCC_PLL3CR_DIVPEN_Msk | RCC_PLL3CR_DIVQEN_Msk | RCC_PLL3CR_DIVREN_Msk));
//	(void) RCC->PLL3CR;

	RCC->PLL3CFGR1 = (RCC->PLL3CFGR1 & ~ (RCC_PLL3CFGR1_DIVM3_Msk | RCC_PLL3CFGR1_DIVN_Msk)) |
		((uint_fast32_t) (PLL3DIVM - 1) << RCC_PLL3CFGR1_DIVM3_Pos) |
		((uint_fast32_t) (PLL3DIVN - 1) << RCC_PLL3CFGR1_DIVN_Pos) |
		0;
	(void) RCC->PLL3CFGR1;

	RCC->PLL3CFGR2 = (RCC->PLL3CFGR2 & ~ (RCC_PLL3CFGR2_DIVQ_Msk)) |
		((uint_fast32_t) (PLL3DIVQ - 1) << RCC_PLL3CFGR2_DIVQ_Pos) |	// pll3_p_ck - xxxxx (1..128 -> 0x00..0x7f)
		0;
	(void) RCC->PLL3CFGR2;

	RCC->PLL3CR |= RCC_PLL2CR_DIVQEN_Msk;
	(void) RCC->PLL3CR;

	RCC->PLL3CR &= ~ RCC_PLL3CR_SSCG_CTRL_Msk;
	(void) RCC->PLL3CR;

	RCC->PLL3CR |= RCC_PLL3CR_PLLON_Msk;
	while ((RCC->PLL3CR & RCC_PLL3CR_PLL3RDY_Msk) == 0)
		;
}

void hardware_set_dotclock(unsigned long dotfreq)
{
	const uint_fast32_t pll4divq = calcdivround2(stm32mp1_get_pll4_freq(), dotfreq);
	// ASSERT(pll4divq >= 1);

	// Stop PLL4
	RCC->PLL4CR &= ~ RCC_PLL4CR_PLLON_Msk;
	(void) RCC->PLL4CR;
	while ((RCC->PLL4CR & RCC_PLL4CR_PLLON_Msk) != 0)
		;

	// PLL4 source mux
	//	0x0: HSI selected as PLL clock (hsi_ck) (default after reset)
	//	0x1: HSE selected as PLL clock (hse_ck)
	//	0x2: CSI selected as PLL clock (csi_ck)
	//	0x3: Signal I2S_CKIN used as reference clock
	RCC->RCK4SELR = (RCC->RCK4SELR & ~ (RCC_RCK4SELR_PLL4SRC_Msk)) |
	#if WITHCPUXOSC || WITHCPUXTAL
		// с внешним генератором
		// с внешним кварцем
		((uint_fast32_t) 0x01 << RCC_RCK4SELR_PLL4SRC_Pos) |	// HSE
	#else
		// На внутреннем генераторе
		((uint_fast32_t) 0x00 << RCC_RCK4SELR_PLL4SRC_Pos) |	// HSI
	#endif
		0;
	while ((RCC->RCK4SELR & RCC_RCK4SELR_PLL4SRCRDY_Msk) == 0)
		;

	RCC->PLL4CFGR1 = (RCC->PLL4CFGR1 & ~ (RCC_PLL4CFGR1_DIVM4_Msk | RCC_PLL4CFGR1_DIVN_Msk)) |
		((uint_fast32_t) (PLL4DIVM - 1) << RCC_PLL4CFGR1_DIVM4_Pos) |
		((uint_fast32_t) (PLL4DIVN - 1) << RCC_PLL4CFGR1_DIVN_Pos) |
		0;
	(void) RCC->PLL4CFGR1;

	RCC->PLL4CFGR2 = (RCC->PLL4CFGR2 & ~ (RCC_PLL4CFGR2_DIVQ_Msk)) |
		((pll4divq - 1) << RCC_PLL4CFGR2_DIVQ_Pos) |	// LTDC clock (1..128 -> 0x00..0x7f)
		0;
	(void) RCC->PLL4CFGR2;

	RCC->PLL4CR |= RCC_PLL4CR_DIVQEN_Msk;	// LTDC clock
	(void) RCC->PLL4CR;

	RCC->PLL4CR &= ~ RCC_PLL4CR_SSCG_CTRL_Msk;
	(void) RCC->PLL4CR;

	// Start PLL4
	RCC->PLL4CR |= RCC_PLL4CR_PLLON_Msk;
	while ((RCC->PLL4CR & RCC_PLL4CR_PLL4RDY_Msk) == 0)
		;
}

// округление тактовой частоты дисплейного контроллера к возможностям системы синхронизации
unsigned long hardware_get_dotclock(unsigned long dotfreq)
{
	const uint_fast32_t pll4divq = calcdivround2(stm32mp1_get_pll4_freq(), dotfreq);
	return stm32mp1_get_pll4_freq() / pll4divq;
}


uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */

// PLL initialize
void FLASHMEMINITFUNC sysinit_pll_initialize(void)
{
#ifdef USE_HAL_DRIVER
	HAL_Init();
#endif /* USE_HAL_DRIVER */

	#if WITHISBOOTLOADER
		// PLL только в bootloader.
		// посеольку программа выполняется из DDR RAM, перерпрограммировать PLL нельзя.
		stm32mp1_pll_initialize();
	#endif /* WITHISBOOTLOADER */

	stm32mp1_usb_clocks_initialize();
	stm32mp1_audio_clocks_initialize();

	SystemCoreClock = CPU_FREQ;
}


void hardware_uart4_set_speed(uint_fast32_t baudrate)
{
	UART4->BRR = calcdivround2(BOARD_UART4_FREQ, baudrate);
}


