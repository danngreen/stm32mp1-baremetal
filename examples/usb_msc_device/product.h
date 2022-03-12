#ifndef __PRODUCT_H
#define __PRODUCT_H

#define RAMFUNC
#define FLASHMEMINITFUNC
//#define WITHCPUXTAL 24000000UL
#define WITHCPUXOSC 24000000UL

// PLL1_1600
#define PLL1DIVM 2 // ref1_ck = 12 MHz (8..16 MHz valid)
#define PLL1DIVP 1 // MPU
#define PLL1DIVQ 2
#define PLL1DIVR 2

//#define PLL1DIVN	54	// 12*54 = 648 MHz
//#define PLL1DIVN	66	// 12*66 = 792 MHz
#define PLL1DIVN (stm32mp1_overdrived() ? 66 : 54) // Auto select

#define PLL2DIVM 2	// ref2_ck = 12 MHz (8..16 MHz valid)
#define PLL2DIVN 44 // 528 MHz Valid division rations for DIVN: between 25 and 100

#define PLL2DIVP 2
// AXISS_CK div2=minimum 528/2 = 264 MHz PLL2 selected as AXI sub-system clock (pll2_p_ck) - 266 MHz max for all
// CPU revisions

#define PLL2DIVQ 1 // GPU clock divider = 528 MHz - 533 MHz max for all CPU revisions
#define PLL2DIVR 1 // DDR clock divider = 528 MHz
//#include "src/sdram/stm32mp15-mx_2G.dtsi"	// 128k*16

// PLL3_800
#define PLL3DIVM 2	 // ref3_ck = 12 MHz (4..16 MHz valid)
#define PLL3DIVN 64	 // 768 MHz
#define PLL3DIVQ 125 // I2S, SAI clock divider: 12/2*64 = 768 MHz. 768/125 = 6.144 MHz. 48 kHz * 64 = 3.072 MHz

// PLL4_800
#define PLL4DIVM 2	// ref2_ck = 12 MHz (4..16 MHz valid)
#define PLL4DIVN 64 // 768 MHz
#define PLL4DIVP 2	// div2
//#define PLL4DIVQ	19	// LTDC clock divider = 30.315 MHz
//#define PLL4DIVR	20	// USBPHY clock divider = 38.4 MHz
//#define PLL4DIVR	24	// USBPHY clock divider = 32 MHz
//#define PLL4DIVR	32	// USBPHY clock divider = 24 MHz
#define PLL4DIVR 16 // USBPHY clock divider = 48 MHz (для прямого тактирования USB_OTG FS)

//	In addition, if the USBO is used in full-speed mode only, the application can choose the
//	48 MHz clock source to be provided to the USBO:
// USBOSRC
//	0: pll4_r_ck clock selected as kernel peripheral clock (default after reset)
//	1: clock provided by the USB PHY (rcc_ck_usbo_48m) selected as kernel peripheral clock
// USBPHYSRC
//  0x0: hse_ker_ck clock selected as kernel peripheral clock (default after reset)
//  0x1: pll4_r_ck clock selected as kernel peripheral clock
//  0x2: hse_ker_ck/2 clock selected as kernel peripheral clock
#define RCC_USBCKSELR_USBOSRC_VAL 0x01
#define RCC_USBCKSELR_USBPHYSRC_VAL 0x00

// from paramdepend.h:
//! GPIO Alternate Functions
typedef enum {
	AF_SYSTEM,			  //!< AF0 - SYSTEM*
	AF_TIM1,			  //!< AF1 - TIM1/2
	AF_TIM2 = 1,		  //!< AF1 - TIM1/2
	AF_TIM17 = 1,		  //!< AF1 - TIM17
	AF_TIM3,			  //!< AF2 - TIM3/4/5
	AF_TIM4 = 2,		  //!< AF2 - TIM3/4/5
	AF_TIM5 = 2,		  //!< AF2 - TIM3/4/5
	AF_TIM8,			  //!< AF3 - TIM9/10/11
	AF_TIM9 = 3,		  //!< AF3 - TIM9/10/11
	AF_TIM10 = 3,		  //!< AF3 - TIM9/10/11
	AF_TIM11 = 3,		  //!< AF3 - TIM9/10/11
	AF_USART1,			  //!< AF4 - USART1
	AF_I2C1 = 4,		  //!< AF4 - I2C1/2/3
	AF_I2C2 = 4,		  //!< AF4 - I2C1/2/3
	AF_I2C3 = 4,		  //!< AF4 - I2C1/2/3
	AF_SPI1,			  //!< AF5 - SPI1/2
	AF_SPI2 = 5,		  //!< AF5 - SPI1/2
	AF_SPI3,			  //!< AF6 - SPI3
	AF_SPI2ext = 6,		  //!< AF6 - SPI3
	AF_SAI = 6,			  //!< AF6 - SAI1/SAI2
	AF_USART2,			  //!< AF7 - USART2/3
	AF_USART3 = 7,		  //!< AF7 - USART1/2/3
	AF_USART4,			  //!< AF8 - USART4/5/6
	AF_USART5 = 8,		  //!< AF8 - USART4/5/6
	AF_USART6 = 8,		  //!< AF8 - USART4/5/6
	AF_CAN1,			  //!< AF9 - CAN1/2
	AF_CAN2 = 9,		  //!< AF9 - CAN1/2
	AF_TIM12 = 9,		  //!< AF9 - TIM12/13/14
	AF_TIM13 = 9,		  //!< AF9 - TIM12/13/14
	AF_TIM14 = 9,		  //!< AF9 - TIM12/13/14
	AF_QUADSPI_AF9 = 9,	  //!< AF9 - AF_QUADSPI
	AF_OTGFS,			  //!< AF10 - OTGFS
	AF_QUADSPI_AF10 = 10, //!< AF10 - AF_QUADSPI
	AF_OTGHS = 10,		  //!< AF10 - OTGHS
	AF_SAI2 = 10,		  //!< AF10 - SAI2
	AF_ETH,				  //!< AF11 - ETH
	AF_FSMC,			  //!< AF12 - FSMC
	AF_SDIO = 12,		  //!< AF12 - SDIO
	AF_OTGHS_FS = 12,	  //!< AF12 - OTG HS configured in FS
	AF_DCMI,			  //!< AF13 - DCMI
	AF_AF14,			  //!< AF14 - RI
	AF_EVENT			  //!< AF15 - SYSTEM (EVENTOUT)
} GPIO_AFLH_t;

/* Частоты встроенных RC генераторов процессора */
#define HSI64FREQ 64000000uL
#define CSI_VALUE 4000000U
#define LSIFREQ 32000uL

//
#if WITHCPUXOSC
// с внешним генератором
#define REF1INFREQ WITHCPUXOSC
#define REF2INFREQ REF1INFREQ

#elif WITHCPUXTAL
// с внешним кварцевым резонатором
#define REF1INFREQ WITHCPUXTAL
#define REF2INFREQ REF1INFREQ

#else /* WITHCPUXTAL */
// На внутреннем RC генераторе
#define REF1INFREQ (stm32mp1_get_hsi_freq())
#define REF2INFREQ HSI64FREQ
#endif /* WITHCPUXTAL */

#define CPU_FREQ (stm32mp1_get_mpuss_freq())
//#define AXISS_FREQ	(REFINFREQ / (PLL2DIVM) * (PLL2DIVN) / (PLL2DIVP))
#define DDR_FREQ (REF2INFREQ / (PLL2DIVM) * (PLL2DIVN) / (PLL2DIVR))

#define BOARD_SPI_FREQ (hardware_get_spi_freq())
#define BOARD_QSPI_FREQ (stm32mp1_get_qspi_freq())

#define TICKS_FREQUENCY (200U) // 200 Hz

// ADC clock frequency: 1..20 MHz
#define SCL_CLOCK 400000uL /* 400 kHz I2C/TWI speed */

#define SPISPEED (BOARD_SPI_FREQ / 4) /* 14 MHz на SCLK - требуемая скорость передачи по SPI */
#define SPISPEEDUFAST 24000000uL	  /* 2требуемая скорость передачи по SPI */
#define SPISPEED400k 400000uL		  /* 400 kHz для низкоскоростных микросхем */
#define SPISPEED100k 100000uL		  /* 100 kHz для низкоскоростных микросхем */

#define ADCVREF_CPU 33		// 3.3 volt
#define DACVREF_CPU 33		// 3.3 volt
#define HARDWARE_DACBITS 12 /* ЦАП работает с 12-битными значениями */
//#define HARDWARE_ADCINPUTS	40	/* до 32-ти входов АЦП */
/* тип для хранения данных, считанных с АЦП */
typedef uint_fast16_t adcvalholder_t;
typedef int_fast16_t sadcvalholder_t; // для хранения знаковых значений
//#define HARDWARE_ADCBITS 8	/* АЦП работает с 8-битными значениями */
//#define HARDWARE_ADCBITS 10	/* АЦП работает с 10-битными значениями */
#define HARDWARE_ADCBITS 12 /* АЦП работает с 12-битными значениями */
//#define HARDWARE_ADCBITS 14	/* АЦП работает с 14-битными значениями */
//#define HARDWARE_ADCBITS 16	/* АЦП работает с 16-битными значениями */
#define ADC_FREQ 10000000uL	  /* тактовая частота SAR преобразователя АЦП. */
#define WITHREFSENSORVAL 1240 /* Reference voltage: STM32H743 1.180 1.216 1.255 */

#define ADCVREF_CPU 33		// 3.3 volt
#define HARDWARE_ADCBITS 12 /* АЦП работает с 12-битными значениями */

//#define HARDWARE_ADCINPUTS	40	/* до 8-ти входов АЦП */

#define DACVREF_CPU 33		// 3.3 volt
#define HARDWARE_DACBITS 12 /* ЦАП работает с 12-битными значениями */

#endif
