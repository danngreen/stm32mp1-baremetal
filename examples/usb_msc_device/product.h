#define RAMFUNC
#define FLASHMEMINITFUNC

static inline int dbg_puts_impl_P(const char *x) {}
void debug_printf_P(const FLASHMEM char *__restrict format, ...) {}
void PRINTF(const FLASHMEM char *__restrict format, ...) {}
// static inline const char *PSTR(const char *x) {}
static inline void dbg_putchar(const char x) {}

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
