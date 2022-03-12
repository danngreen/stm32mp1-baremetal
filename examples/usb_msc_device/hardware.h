/* $Id$ */
//
// Проект HF Dream Receiver (КВ приёмник мечты)
// автор Гена Завидовский mgs2001@mail.ru
// UA1ARN
//

#ifndef HARDWARE_H_INCLUDED
#define HARDWARE_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


// ST dual core A7 + M4

// STM32MP157Axx
// STM32MP157Dxx
// STM32MP157AAB3
// STM32MP157DAB1

#define CORE_CA7	1
#define CPUSTYLE_ARM		1		/* архитектура процессора ARM */

#include "stm32mp1xx.h"
#include "irq_ctrl.h"

#define DCACHEROWSIZE 64
#define ICACHEROWSIZE 32

#define ALIGNX_BEGIN __attribute__((aligned(64)))
#define ALIGNX_END /* nothing */

#if __ARM_NEON
//#define ARM_MATH_NEON 1
//#define ARM_MATH_NEON_EXPERIMENTAL 1
#endif /* __ARM_NEON */
//#define ARM_MATH_AUTOVECTORIZE 1
#define ARM_MATH_LOOPUNROLL 1 // выставляется в hardware.h

/* По значению маски получить значение младшего бита, входящего в неё. */
#define MASK2LSB(mask) ((mask) & (0 - (mask)))

/* макрооределение для расчёта количества "тиков" для выдержки величин задержек. */
#define NTICKS(t_mS) ((uint_fast16_t)(((uint_fast32_t)(t_mS)*TICKS_FREQUENCY + 500) / 1000))
#define NTICKSADC(t_uS) ((uint_fast16_t)(((uint_fast32_t)(t_uS)*ADC_FREQ + 5) / 1000000))
#define NTICKSADC01(t_uS01) ((uint_fast16_t)(((uint_fast32_t)(t_uS01)*ADC_FREQ + 5) / 10000000))

void cpu_initialize(void);
void arm_hardware_reset(void);
void cpu_initdone(void);			   // секция init больше не нужна
uint_fast32_t cpu_getdebugticks(void); // получение из аппаратного счетчика монотонно увеличивающегося кода

void tickers_initialize(void);
void spool_systimerbundle1(void);
void spool_systimerbundle2(void);
void spool_elkeybundle(void);
void sysinit_pll_initialize(void); // PLL initialize
void hardware_adc_startonescan(void);
void stm32mp1_pll1_slow(uint_fast8_t slow);

void hardware_timer_initialize(uint_fast32_t ticksfreq);

void spool_encinterrupt(void); /* прерывание по изменению сигнала на входах от валкодера */
void spool_encinterrupt2(void); /* прерывание по изменению сигнала на входах от валкодера #2 */
void hardware_encoder_initialize(void);
uint_fast8_t hardware_get_encoder_bits(void); /* Состояние фазы A - в бите с весом 2, фазы B - в бите с весом 1 */
uint_fast8_t hardware_get_encoder2_bits(void); /* Состояние фазы A - в бите с весом 2, фазы B - в бите с весом 1 */

void hardware_adc_initialize(void);

void watchdog_initialize(void); /* разрешение сторожевого таймера в устройстве */
void watchdog_ping(void);		/* перезапуск сторожевого таймера */

/* тип для передачи параметра "адрес устройства на SPI шине" */
/* это может быть битовая маска в порту ввода-вывода */
typedef uint_fast32_t spitarget_t;
/* тип для хранения данных, считанных из порта ввода-вывода или для формируемого значения */
typedef uint_fast32_t portholder_t;

#define FLASHMEM   //__flash
#define NOINLINEAT // __attribute__((noinline))
#define strlen_P(s) strlen(s)

#define ATTRWEAK __WEAK
// Use __attribute__ ((weak, alias("Default_Handler")))

#define PSTR(s) (s)
//#define PSTR(s) (__extension__({static const char __c[] FLASHMEM = (s); &__c[0];}))

void local_delay_us(int timeUS);
void local_delay_ms(int timeMS);

// Sample usage:
///* все возможные в данной конфигурации фильтры */
//#define IF3_FMASK	(IF3_FMASK_0P5 | IF3_FMASK_1P8 | IF3_FMASK_2P7 | IF3_FMASK_3P1)

void spool_nmeapps(void); // Обработчик вызывается при приходе очередного импульса PPS

// получить прескалер и значение для программирования таймера
uint_fast8_t hardware_calc_sound_params(uint_least16_t tonefreq, /* tonefreq - частота в десятых долях герца. Минимум -
																	400 герц (определено набором команд CAT). */
										unsigned *pvalue);
// установить прескалер и значение для программирования таймера
// called from interrupt or with disabled interrupts
void hardware_sounds_setfreq(uint_fast8_t prei, unsigned value);
// выключаем звук
void hardware_sounds_disable(void);
/* вызывается при запрещённых прерываниях. */
void hardware_beep_initialize(void);

/* SAI, I2S и подключенная на них периферия */
void hardware_channels_initialize(void);
void hardware_channels_enable(void);

void hardware_dac_initialize(void);				 /* инициализация DAC на STM32F4xx */
void hardware_dac_ch1_setvalue(uint_fast16_t v); // вывод 12-битного значения на ЦАП - канал 1
void hardware_dac_ch2_setvalue(uint_fast16_t v); // вывод 12-битного значения на ЦАП - канал 2

void hardware_spi_io_delay(void);

void cat2_parsechar(uint_fast8_t c); /* вызывается из обработчика прерываний */
void cat2_rxoverflow(void);			 /* вызывается из обработчика прерываний */
void cat2_disconnect(void); /* вызывается из обработчика прерываний произошёл разрыв связи при работе по USB CDC */
void cat2_sendchar(void *ctx); /* вызывается из обработчика прерываний */

// Функции тестирования работы компорта по прерываниям
void cat3_parsechar(uint_fast8_t c); /* вызывается из обработчика прерываний */
void cat3_rxoverflow(void);			 /* вызывается из обработчика прерываний */
void cat3_disconnect(void);			 /* вызывается из обработчика прерываний */
void cat3_sendchar(void *ctx);		 /* вызывается из обработчика прерываний */

// Функции тестирования работы компорта по прерываниям
void cat7_parsechar(uint_fast8_t c); /* вызывается из обработчика прерываний */
void cat7_rxoverflow(void);			 /* вызывается из обработчика прерываний */
void cat7_disconnect(void);			 /* вызывается из обработчика прерываний */
void cat7_sendchar(void *ctx);		 /* вызывается из обработчика прерываний */

void modem_parsechar(uint_fast8_t c); /* вызывается из обработчика прерываний */
void modem_rxoverflow(void);		  /* вызывается из обработчика прерываний */
void modem_disconnect(void);		  /* вызывается из обработчика прерываний */
void modem_sendchar(void *ctx);		  /* вызывается из обработчика прерываний */

void nmea_parsechar(uint_fast8_t c); /* вызывается из обработчика прерываний */
void nmea_rxoverflow(void);			 /* вызывается из обработчика прерываний */
void nmea_sendchar(void *ctx);		 /* вызывается из обработчика прерываний */

void nmea_format(const char *format, ...);
int nmea_putc(int c);

void hardware_uart1_initialize(uint_fast8_t debug);
void hardware_uart1_set_speed(uint_fast32_t baudrate);
void hardware_uart1_tx(void *ctx, uint_fast8_t c); /* передача символа после прерывания о готовности передатчика */
void hardware_uart1_enabletx(uint_fast8_t state); /* вызывается из обработчика прерываний */
void hardware_uart1_enablerx(uint_fast8_t state); /* вызывается из обработчика прерываний */
uint_fast8_t hardware_uart1_putchar(uint_fast8_t c); /* передача символа если готов порт */
uint_fast8_t hardware_uart1_getchar(char *cp);		 /* приём символа, если готов порт */

void hardware_uart2_initialize(uint_fast8_t debug);
void hardware_uart2_set_speed(uint_fast32_t baudrate);
void hardware_uart2_tx(void *ctx, uint_fast8_t c); /* передача символа после прерывания о готовности передатчика */
void hardware_uart2_enabletx(uint_fast8_t state); /* вызывается из обработчика прерываний */
void hardware_uart2_enablerx(uint_fast8_t state); /* вызывается из обработчика прерываний */
uint_fast8_t hardware_uart2_putchar(uint_fast8_t c); /* передача символа если готов порт */
uint_fast8_t hardware_uart2_getchar(char *cp);		 /* приём символа, если готов порт */

void hardware_uart4_initialize(uint_fast8_t debug);
void hardware_uart4_set_speed(uint_fast32_t baudrate);
void hardware_uart4_tx(void *ctx, uint_fast8_t c); /* передача символа после прерывания о готовности передатчика */
void hardware_uart4_enabletx(uint_fast8_t state); /* вызывается из обработчика прерываний */
void hardware_uart4_enablerx(uint_fast8_t state); /* вызывается из обработчика прерываний */
uint_fast8_t hardware_uart4_putchar(uint_fast8_t c); /* передача символа если готов порт */
uint_fast8_t hardware_uart4_getchar(char *cp);		 /* приём символа, если готов порт */

void hardware_uart5_initialize(uint_fast8_t debug);
void hardware_uart5_set_speed(uint_fast32_t baudrate);
void hardware_uart5_tx(void *ctx, uint_fast8_t c); /* передача символа после прерывания о готовности передатчика */
void hardware_uart5_enabletx(uint_fast8_t state); /* вызывается из обработчика прерываний */
void hardware_uart5_enablerx(uint_fast8_t state); /* вызывается из обработчика прерываний */
uint_fast8_t hardware_uart5_putchar(uint_fast8_t c); /* передача символа если готов порт */
uint_fast8_t hardware_uart5_getchar(char *cp);		 /* приём символа, если готов порт */

void hardware_uart7_initialize(uint_fast8_t debug);
void hardware_uart7_set_speed(uint_fast32_t baudrate);
void hardware_uart7_tx(void *ctx, uint_fast8_t c); /* передача символа после прерывания о готовности передатчика */
void hardware_uart7_enabletx(uint_fast8_t state); /* вызывается из обработчика прерываний */
void hardware_uart7_enablerx(uint_fast8_t state); /* вызывается из обработчика прерываний */
uint_fast8_t hardware_uart7_putchar(uint_fast8_t c); /* передача символа если готов порт */
uint_fast8_t hardware_uart7_getchar(char *cp);		 /* приём символа, если готов порт */

void usbd_cdc_tx(void *ctx, uint_fast8_t c); /* передача символа после прерывания о готовности передатчика - вызывается
												из HARDWARE_CDC_ONTXCHAR */
void usbd_cdc_enabletx(uint_fast8_t state);	 /* вызывается из обработчика прерываний */
void usbd_cdc_enablerx(uint_fast8_t state);	 /* вызывается из обработчика прерываний */

/* отладочная выдача через USB CDC */
void debugusb_initialize(void); /* Вызывается из user-mode программы при запрещённых прерываниях. */
uint_fast8_t debugusb_putchar(uint_fast8_t c); /* передача символа если готов порт */
uint_fast8_t debugusb_getchar(char *cp);	   /* приём символа, если готов порт */
void debugusb_parsechar(uint_fast8_t c); /* вызывается из обработчика прерываний */
void debugusb_sendchar(void *ctx);		 /* вызывается из обработчика прерываний */

/* TWI (I2C) interface */
#define I2C_RETRIES 3 /* количество повторов */
void i2c_initialize(void);

void i2c_start(uint_fast8_t address);
void i2c_read(uint8_t *pdata, uint_fast8_t acknak);

// Работа со вторым каналом I2C
void i2c2_start(uint_fast8_t address);
void i2c2_read(uint8_t *pdata, uint_fast8_t acknak);

#define I2C_READ_ACK 0		// i2c_read parameter
#define I2C_READ_ACK_1 1	// i2c_read parameter
#define I2C_READ_NACK 2		// ack_type - last parameterr in read block
#define I2C_READ_ACK_NACK 3 // чтение первого и единственного байта по I2C

void i2c_write(uint_fast8_t data);
void i2c_write_withrestart(uint_fast8_t data); // запись, после чего restart
void i2c_waitsend(void);					   // Вызвать после последнего i2c_write()
void i2c_stop(void);

// Работа со вторым каналом I2C
void i2c2_write(uint_fast8_t data);
void i2c2_write_withrestart(uint_fast8_t data); // запись, после чего restart
void i2c2_waitsend(void);						// Вызвать после последнего i2c_write()
void i2c2_stop(void);

void hardware_twi_master_configure(void);
void i2chw_initialize(void);

uint16_t i2chw_read(uint16_t slave_address, uint8_t *buf, uint32_t size);
uint16_t i2chw_write(uint16_t slave_address, uint8_t *buf, uint32_t size);

uint32_t hardware_get_random(void);

void arm_hardware_dma2d_initialize(void); // Graphic 2D engine
void arm_hardware_mdma_initialize(void);  // Graphic 2D engine
void arm_hardware_sdram_initialize(void); // External memory region(s)

typedef struct videomode_tag {
	unsigned width;	 /* LCD pixel width            */
	unsigned height; /* LCD pixel height           */
	unsigned hsync;	 /* horizontal synchronization */
	unsigned hbp;	 /* horizontal back porch      */
	unsigned hfp;	 /* horizontal front porch     */

	unsigned vsync; /* vertical synchronization   */
	unsigned vbp;	/* vertical back porch        */
	unsigned vfp;	/* vertical front porch       */

	// mode: de/sync mode select.
	// de mode: mode="1", vs and hs must pull high.
	// sync mode: mode="0". de must be grounded
	unsigned vsyncneg;	/* negative polarity required for vsync signal */
	unsigned hsyncneg;	/* negative polarity required for hsync signal */
	unsigned deneg;		/* negative de polarity: (normal: de is 0 while sync) */
	unsigned lq43reset; /* требуется формирование сигнала RESET для панели по этому выводу после начала формирования
						   синхронизации */
	unsigned fps;		/* frames per secound */

} videomode_t;

extern const videomode_t vdmode0;
void arm_hardware_ltdc_initialize(const uintptr_t *frames,
								  const videomode_t *vdmode); // LCD-TFT Controller (LTDC) with framebuffer
unsigned long display_getdotclock(const videomode_t *vdmode);

void arm_hardware_ltdc_main_set(uintptr_t addr);		  /* Set MAIN frame buffer address. Wait for VSYNC. */
void arm_hardware_ltdc_main_set_no_vsync(uintptr_t addr); /* Set MAIN frame buffer address. No waiting for VSYNC. */
void arm_hardware_ltdc_pip_set(uintptr_t addr);			  // Set PIP frame buffer address.
void arm_hardware_ltdc_pip_off(void);					  // Turn PIP off (main layer only).
void arm_hardware_ltdc_L8_palette(void);				  // Palette reload

uint_fast8_t usbd_cdc1_getrts(void);
uint_fast8_t usbd_cdc1_getdtr(void);

uint_fast8_t usbd_cdc2_getrts(void);
uint_fast8_t usbd_cdc2_getdtr(void);

void AT91F_PIOA_IRQHandler(void);
void AT91F_ADC_IRQHandler(void);

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI5_IRQHandler(void);
void EXTI6_IRQHandler(void);
void EXTI7_IRQHandler(void);
void EXTI8_IRQHandler(void);
void EXTI9_IRQHandler(void);
void EXTI10_IRQHandler(void);
void EXTI11_IRQHandler(void);
void EXTI12_IRQHandler(void);
void EXTI13_IRQHandler(void);
void EXTI14_IRQHandler(void);
void EXTI15_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

void SDIO_IRQHandler(void);
void SDMMC1_IRQHandler(void);
void DMA2_Stream6_IRQHandler(void);

void SDIO0_IRQHandler(void); // ZYNQ

void SysTick_Handler(void);
void Reset_CPUn_Handler(void);

void ADC_IRQHandler(void);
void ADC1_IRQHandler(void);
void ADC2_IRQHandler(void);
void ADC3_IRQHandler(void);
void ADC1_2_IRQHandler(void);
void ADC1_COMP_IRQHandler(void);
void r7s721_adi_irq_handler(void);
void TIM3_IRQHandler(void);
void TIM5_IRQHandler(void);

void GPU_IRQHandler(void); // STM32MP1 specific

void r7s721_intc_initialize(void);

/* Отображение номеров каналов АЦП процессоров STM32Fxxx в каналы процессора STM32H7 */
typedef struct adcinmap_tag {
	uint_fast8_t ch; // номер канала в периферийном блоке процессора
#if CPUSTYLE_STM32H7XX || CPUSTYLE_STM32MP1
	ADC_TypeDef *adc; // периферийный блок процессора
	ADC_Common_TypeDef *adccommon;
#endif						 /* CPUSTYLE_STM32H7XX || CPUSTYLE_STM32MP1 */
	uint_fast8_t thold_uS01; // минимальное время выборки для данного канала - десятые доли микросекунды
} adcinmap_t;

const adcinmap_t *getadcmap(uint_fast8_t adci);

void spool_adcdonebundle(void);
void adcdones_initialize(void);
uint_fast8_t isadchw(uint_fast8_t adci); // Проверка что индекс входа АЦП относится ко встроенной периферии процессора

void cpump_initialize(void);

void cpu_stm32f1xx_setmapr(unsigned long bits);

void hardware_tim21_initialize(void);

void hardware_dcdcfreq_tim16_ch1_initialize(void);
void hardware_dcdcfreq_tim17_ch1_initialize(void);
void hardware_dcdcfreq_tioc0a_mtu0_initialize(void);

void hardware_dcdcfreq_tim16_ch1_setdiv(uint_fast32_t v);
void hardware_dcdcfreq_tim17_ch1_setdiv(uint_fast32_t v);
void hardware_dcdcfreq_tioc0a_mtu0_setdiv(uint_fast32_t v);

uint_fast32_t hardware_dcdc_calcdivider(uint_fast32_t freq);

void hardware_sdhost_initialize(void);
void hardware_sdhost_setspeed(unsigned long ticksfreq);
void hardware_sdhost_setbuswidth(uint_fast8_t use4bit);
void hardware_sdhost_detect(uint_fast8_t present); // в ответ на прерывание изменения состояния card detect

void lowtests(void);
void midtests(void);
void hightests(void);
void looptests(void); // Периодически вызывается в главном цикле

void arm_hardware_invalidate(uintptr_t base, int_fast32_t size); // Сейчас в эту память будем читать по DMA
void arm_hardware_flush(uintptr_t base, int_fast32_t size); // Сейчас эта память будет записываться по DMA куда-то
void arm_hardware_flush_invalidate(
	uintptr_t base,
	int_fast32_t size); // Сейчас эта память будет записываться по DMA куда-то. Потом содержимое не требуется
void arm_hardware_flush_all(void);

void r7s721_sdhi0_dma_handler(void);

uint_fast32_t calcdivround2(uint_fast32_t ref, /* частота на входе делителя, в герцах. */
							uint_fast32_t freq /* требуемая частота на выходе делителя, в герцах. */
);

#define CATPCOUNTSIZE (13)
#define MSGBUFFERSIZE8 (9 + CATPCOUNTSIZE)

enum messagetypes {
	MSGT_EMPTY,
	MSGT_KEYB, // 1 byte - key code
	MSGT_CAT,  // 12 bytes as parameter
	MSGT_DPC,  // 4 bytes - function address, 4 bytes - parameter
	//
	MSGT_count
};

uint_fast8_t takemsgready_user(uint8_t **dest); // Буферы с принятымти от обработчиков прерываний сообщениями
void releasemsgbuffer_user(uint8_t *dest); // Освобождение обработанного буфера сообщения
size_t takemsgbufferfree_low(uint8_t **dest); // Буфер для формирования сообщения
void placesemsgbuffer_low(uint_fast8_t type, uint8_t *dest); // поместить сообщение в очередь к исполнению

typedef struct dpclock_tag dpclock_t;

typedef void (*udpcfn_t)(void *);
typedef void (*udpcfn2_t)(void *, void *);
typedef void (*udpcfn3_t)(void *, void *, void *);
uint_fast8_t board_dpc(dpclock_t *lp, udpcfn_t func, void *arg); // Запрос отложенного вызова user-mode функций
uint_fast8_t
board_dpc2(dpclock_t *lp, udpcfn2_t func, void *arg1, void *arg2); // Запрос отложенного вызова user-mode функций
uint_fast8_t board_dpc3(
	dpclock_t *lp, udpcfn3_t func, void *arg1, void *arg2, void *arg3); // Запрос отложенного вызова user-mode функций

// #include "mslist.h"
// typedef struct ticker_tag
//{
//	VLIST_ENTRY item;
//	unsigned period;
//	//unsigned fired;
//	unsigned ticks;		// текущее количество тиков
//	void (* cb)(void *);
//	void * ctx;
//} ticker_t;

// void ticker_initialize(ticker_t * p, unsigned nticks, void (* cb)(void *), void * ctx);
// void ticker_add(ticker_t * p);
// void ticker_del(ticker_t * p);

// typedef struct adcdone_tag
//{
//	VLIST_ENTRY item;
//	void (* cb)(void *);
//	void * ctx;
// } adcdone_t;

// void adcdone_initialize(adcdone_t * p, void (* cb)(void *), void * ctx);
// void adcdone_add(adcdone_t * p);
// void adcdone_del(adcdone_t * p);

void bootloader_copyapp(uintptr_t apparea);
uint_fast8_t bootloader_get_start(uintptr_t apparea, uintptr_t *ip);
void bootloader_deffereddetach(void *arg);

#define BOARD_ADCX0BASE 24 // on-board ADC base index
#define BOARD_ADCX1BASE 32 // PA board ADC base index
#define BOARD_ADCMRRBASE                                                                                               \
	40 // mirror - значения АЦП устанавливабтся выходами программных компонентов, без считывания с аппаратуры.

#define HARDWARE_ADCINPUTS 56 /* до 16-ти входов АЦП на каждый тип источников */

#define BOARD_ADCXIN(ch) (BOARD_ADCX0BASE + (ch))
#define BOARD_ADCX1IN(ch) (BOARD_ADCX0BASE + (ch))
#define BOARD_ADCX2IN(ch) (BOARD_ADCX1BASE + (ch))
#define BOARD_ADCMRRIN(ch) (BOARD_ADCMRRBASE + (ch))

// Cortex-A7/A9 handlers
void Undef_Handler(void);
void SWI_Handler(void);
void PAbort_Handler(void);
void DAbort_Handler(void);
void FIQ_Handler(void);
void Hyp_Handler(void);

void IRQ_Handler_GICv1(void);
void IRQ_Handler_GICv2(void);
void IRQ_Handler(void); // No GIC

void Reset_CPU1_Handler(void); // startup located function
void Reset_CPUn_Handler(void);

// Set interrupt vector wrappers
void arm_hardware_set_handler(uint_fast16_t int_id,
							  void (*handler)(void),
							  uint_fast8_t priority,
							  uint_fast8_t targetcpu);
void arm_hardware_set_handler_overrealtime(uint_fast16_t int_id, void (*handler)(void));
void arm_hardware_set_handler_realtime(uint_fast16_t int_id, void (*handler)(void));
void arm_hardware_set_handler_system(uint_fast16_t int_id, void (*handler)(void));
void arm_hardware_disable_handler(uint_fast16_t int_id);
uint_fast8_t arm_hardware_cpuid(void); // This processor index (0..n-1)
void arm_hardware_populte_second_initialize(void);

void audioproc_spool_user(void); // вызывать при выполнении длительных операций

void hardware_set_dotclock(unsigned long dotfreq);
unsigned long hardware_get_dotclock(unsigned long dotfreq);
void hardware_nonguiyield(void);
uint_fast8_t stm32mp1_overdrived(void); // return 1 if CPU supports 800 MHz clock

int toshiba_ddr_power_init(void);
void stpmic1_dump_regulators(void);

void network_initialize(void);
void init_netif(void);

void usb_polling(void);		   /* LWIP support */
void sys_check_timeouts(void); /* LWIP support */

#define USBALIGN_BEGIN
#define USBALIGN_END __attribute__((aligned(64))) /* nothing */

//#define UNUSED(x) ((void)(x))

#define AUDIORECBUFFSIZE16 (16384) // размер данных должен быть не меньше размера кластера на SD карте

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a)[0])

#define HWADDR 0x30, 0x89, 0x84, 0x6A, 0x96, 0x34

extern uint8_t myIP[4];
extern uint8_t myNETMASK[4];
extern uint8_t myGATEWAY[4];

/* получить 32-бит значение */
uint_fast32_t
/* Low endian memory layout */
USBD_peek_u32(const uint8_t *buff);

/* записать в буфер для ответа 32-бит значение */
/* Low endian memory layout */
unsigned USBD_poke_u32(uint8_t *buff, uint_fast32_t v);

/* получить 24-бит значение */
/* Low endian memory layout */
uint_fast32_t USBD_peek_u24(const uint8_t *buff);

/* записать в буфер для ответа 24-бит значение */
/* Low endian memory layout */
unsigned USBD_poke_u24(uint8_t *buff, uint_fast32_t v);

/* получить 16-бит значение */
/* Low endian memory layout */
uint_fast16_t USBD_peek_u16(const uint8_t *buff);

/* записать в буфер для ответа 16-бит значение */
unsigned USBD_poke_u16(uint8_t *buff, uint_fast16_t v);

/* получить 8-бит значение */
uint_fast8_t USBD_peek_u8(const uint8_t *buff);

/* записать в буфер для ответа 8-бит значение */
unsigned USBD_poke_u8(uint8_t *buff, uint_fast8_t v);

/* получить 32-бит значение */
/* Big endian memory layout */
uint_fast32_t USBD_peek_u32_BE(const uint8_t *buff);

/* получить 64-бит значение */
/* Big endian memory layout */
uint_fast64_t USBD_peek_u64_BE(const uint8_t *buff);

/* записать в буфер для ответа n-бит значение */
/* Big endian memory layout */
unsigned USBD_poke_u32_BE(uint8_t *buff, uint_fast32_t v);
unsigned USBD_poke_u64_BE(uint8_t *buff, uint_fast64_t v);
unsigned USBD_poke_u24_BE(uint8_t *buff, uint_fast32_t v);
unsigned USBD_poke_u16_BE(uint8_t *buff, uint_fast16_t v);

uint_fast32_t ulmin32(uint_fast32_t a, uint_fast32_t b);
uint_fast32_t ulmax32(uint_fast32_t a, uint_fast32_t b);
uint_fast16_t ulmin16(uint_fast16_t a, uint_fast16_t b);
uint_fast16_t ulmax16(uint_fast16_t a, uint_fast16_t b);
unsigned long ulmin(unsigned long a, unsigned long b);
unsigned long ulmax(unsigned long a, unsigned long b);

#define HI_32BY(w) (((w) >> 24) & 0xFF) /* Extract 31..24 bits from unsigned word */
#define HI_24BY(w) (((w) >> 16) & 0xFF) /* Extract 23..16 bits from unsigned word */
#define HI_BYTE(w) (((w) >> 8) & 0xFF)	/* Extract high-order byte from unsigned word */
#define LO_BYTE(w) ((w)&0xFF)			/* Extract low-order byte from unsigned word */

#ifdef __cplusplus
}
#endif /* __cplusplus */

// #if 1
#include "product.h"
// #else
// 	#include "boards/arm_stm32h7xx_tqfp100_ctlstyle_storch_v7z_vt.h"	// rmainunit_v5km0.pcb, rmainunit_v5km1.pcb
// STM32H743IIT6, TFT 4.3", 2xmini-USB, mini SD-CARD, NAU8822L и FPGA EP4CE22E22I7N 	#include "paramdepend.h"
// /* проверка зависимостей параметров конфигурации */ 	#include "boards/arm_stm32h7xx_tqfp100_cpustyle_storch_v7z_vt.h"
// // Rmainunit_v5l.pcb (mini USBx2, wide display interface) - mini RX #endif

// #include "taildefs.h"
// #include "radio.h"	/* Определения, специфические для устройств, относящихся к радиосвязи. */
#include "clocks.h"
uint32_t sys_now(void);

#endif // HARDWARE_H_INCLUDED
