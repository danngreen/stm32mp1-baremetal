/* $Id$ */
//
// Проект HF Dream Receiver (КВ приёмник мечты)
// автор Гена Завидовский mgs2001@mail.ru
// UA1ARN
//

#include "hardware.h"	/* зависящие от процессора функции работы с портами */

#include <string.h>
#include <math.h>

#include "formats.h"	// for debug prints
#include "gpio.h"

static volatile uint32_t sys_now_counter;
uint32_t sys_now(void)
{
	return sys_now_counter;
}

RAMFUNC void spool_systimerbundle1(void)
{
	HAL_IncTick();
	sys_now_counter += (1000 / TICKS_FREQUENCY);
}

RAMFUNC void spool_systimerbundle2(void)
{
}



/* прерывания от валколера при наличии в системе вложенных прерываний вызываются на уровне приоритета REALTINE */
static RAMFUNC void stm32fxxx_pinirq(portholder_t pr)
{
}


	void EXTI0_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM0;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI1_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM1;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI2_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM2;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI3_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM3;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI4_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM4;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI5_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM5;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI6_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM6;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI7_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM7;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI8_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM8;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI9_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM9;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI10_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM10;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI11_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM11;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI12_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM12;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI13_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM13;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI14_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM14;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}

	void EXTI15_IRQHandler(void)
	{
		const uint_fast32_t mask = EXTI_IMR1_IM15;
		const portholder_t prf = EXTI->FPR1 & mask;
		EXTI->FPR1 = prf;		// reset all faling requests
		const portholder_t prr = EXTI->RPR1 & mask;
		EXTI->RPR1 = prr;		// reset all rising requests
		stm32fxxx_pinirq(prf | prr);
	}


uint_fast8_t hardware_getshutdown(void)
{
	return 0;
}



uint32_t hardware_get_random(void)
{
	return 0;
}



static unsigned long local_delay_uscycles(unsigned timeUS, unsigned cpufreq_MHz)
{
	const unsigned long top = 120uL * cpufreq_MHz * timeUS / 1000;
	return top;
}

void /* RAMFUNC_NONILINE */ local_delay_us(int timeUS)
{
	// Частота процессора приволится к мегагерцам.
	const unsigned long top = local_delay_uscycles(timeUS, CPU_FREQ / 1000000uL);
	//
	volatile unsigned long t;
	for (t = 0; t < top; ++ t)
	{
	}
}
// exactly as required
//
void local_delay_ms(int timeMS)
{
	// Частота процессора приволится к мегагерцам.
	const unsigned long top = local_delay_uscycles(1000, CPU_FREQ / 1000000uL);
	int n;
	for (n = 0; n < timeMS; ++ n)
	{
		volatile unsigned long t;
		for (t = 0; t < top; ++ t)
		{
		}
	}
}


//	MRC p15, 0, <Rt>, c6, c0, 2 ; Read IFAR into Rt
//	MCR p15, 0, <Rt>, c6, c0, 2 ; Write Rt to IFAR

/** \brief  Get IFAR
\return		Instruction Fault Address register value
*/
__STATIC_FORCEINLINE uint32_t __get_IFAR(void)
{
	uint32_t result;
	__get_CP(15, 0, result, 6, 0, 2);
	return result;
}

//	MRC p15, 0, <Rt>, c6, c0, 0 ; Read DFAR into Rt
//	MCR p15, 0, <Rt>, c6, c0, 0 ; Write Rt to DFAR

/** \brief  Get DFAR
\return		Data Fault Address register value
*/
__STATIC_FORCEINLINE uint32_t __get_DFAR(void)
{
	uint32_t result;
	__get_CP(15, 0, result, 6, 0, 0);
	return result;
}

void Undef_Handler(void)
{
	dbg_puts_impl_P(PSTR("UndefHandler trapped.\n"));
	dbg_puts_impl_P((__get_MPIDR() & 0x03) ? PSTR("CPUID=1\n") : PSTR("CPUID=0\n"));
	for (;;)
		;
}

void SWI_Handler(void)
{
	dbg_puts_impl_P(PSTR("SWIHandler trapped.\n"));
	dbg_puts_impl_P((__get_MPIDR() & 0x03) ? PSTR("CPUID=1\n") : PSTR("CPUID=0\n"));
	for (;;)
		;
}

// Prefetch Abort
void PAbort_Handler(void)
{
	const volatile uint32_t marker = 0xDEADBEEF;
	dbg_puts_impl_P(PSTR("PAbort_Handler trapped.\n"));
	dbg_puts_impl_P((__get_MPIDR() & 0x03) ? PSTR("CPUID=1\n") : PSTR("CPUID=0\n"));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
	PRINTF(PSTR("DFSR=%08lX, IFAR=%08lX, pc=%08lX, sp~%08lx\n"), __get_DFSR(), __get_IFAR(), (& marker) [2], (unsigned long) & marker);
#pragma GCC diagnostic pop
	const int WnR = (__get_DFSR() & (1uL << 11)) != 0;
	const int Status = (__get_DFSR() & (0x0FuL << 0));
	/*
		1. 0b000001 alignment fault
		2. 0b000100 instruction cache maintenance fault
		3. 0bx01100 1st level translation, synchronous external abort
		4. 0bx01110 2nd level translation, synchronous external abort
		5. 0b000101 translation fault, section
		6. 0b000111 translation fault, page
		7. 0b000011 access flag fault, section
		8. 0b000110 access flag fault, page
		9. 0b001001 domain fault, section
		10. 0b001011 domain fault, page
		11. 0b001101 permission fault, section
		12. 0b001111 permission fault, page
		13. 0bx01000 synchronous external abort, nontranslation
		14. 0bx10110 asynchronous external abort
		15. 0b000010 debug event.
	*/
	PRINTF(PSTR(" WnR=%d, Status=%02X\n"), (int) WnR, (unsigned) Status);
	switch (Status)
	{
	case 0x01: PRINTF(PSTR("alignment fault\n")); break;
	case 0x04: PRINTF(PSTR("instruction cache maintenance fault\n")); break;
	case 0x0C: PRINTF(PSTR("1st level translation, synchronous external abort\n")); break;
	case 0x0E: PRINTF(PSTR("2nd level translation, synchronous external abort\n")); break;
	case 0x05: PRINTF(PSTR("translation fault, section\n")); break;
	case 0x07: PRINTF(PSTR("translation fault, page\n")); break;
	case 0x03: PRINTF(PSTR("access flag fault, section\n")); break;
	case 0x06: PRINTF(PSTR("access flag fault, page\n")); break;
	case 0x09: PRINTF(PSTR("domain fault, section\n")); break;
	case 0x0B: PRINTF(PSTR("domain fault, page\n")); break;
	case 0x0D: PRINTF(PSTR("permission fault, section\n")); break;
	case 0x0F: PRINTF(PSTR("permission fault, page\n")); break;
	case 0x08: PRINTF(PSTR("synchronous external abort, nontranslation\n")); break;
	case 0x16: PRINTF(PSTR("asynchronous external abort\n")); break;
	case 0x02: PRINTF(PSTR("debug event.\n")); break;
	default: PRINTF(PSTR("undefined Status=%02X\n"), Status); break;
	}
//	unsigned i;
//	for (i = 0; i < 8; ++ i)
//	{
//		PRINTF("marker [%2d] = %08lX\n", i, (& marker) [i]);
//	}
	for (;;)
	{
	}
}

// Data Abort.
void DAbort_Handler(void)
{
	const volatile uint32_t marker = 0xDEADBEEF;
	dbg_puts_impl_P(PSTR("DAbort_Handler trapped.\n"));
	dbg_puts_impl_P((__get_MPIDR() & 0x03) ? PSTR("CPUID=1\n") : PSTR("CPUID=0\n"));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
	PRINTF(PSTR("DFSR=%08lX, DFAR=%08lX, pc=%08lX, sp~%08lx\n"), __get_DFSR(), __get_DFAR(), (& marker) [2], (unsigned long) & marker);
#pragma GCC diagnostic pop
	const int WnR = (__get_DFSR() & (1uL << 11)) != 0;
	const int Status = (__get_DFSR() & (0x0FuL << 0));
	/*
		1. 0b000001 alignment fault
		2. 0b000100 instruction cache maintenance fault
		3. 0bx01100 1st level translation, synchronous external abort
		4. 0bx01110 2nd level translation, synchronous external abort
		5. 0b000101 translation fault, section
		6. 0b000111 translation fault, page
		7. 0b000011 access flag fault, section
		8. 0b000110 access flag fault, page
		9. 0b001001 domain fault, section
		10. 0b001011 domain fault, page
		11. 0b001101 permission fault, section
		12. 0b001111 permission fault, page
		13. 0bx01000 synchronous external abort, nontranslation
		14. 0bx10110 asynchronous external abort
		15. 0b000010 debug event.
	*/
	PRINTF(PSTR(" WnR=%d, Status=%02X\n"), (int) WnR, (unsigned) Status);
	switch (Status)
	{
	case 0x01: PRINTF(PSTR("alignment fault\n")); break;
	case 0x04: PRINTF(PSTR("instruction cache maintenance fault\n")); break;
	case 0x0C: PRINTF(PSTR("1st level translation, synchronous external abort\n")); break;
	case 0x0E: PRINTF(PSTR("2nd level translation, synchronous external abort\n")); break;
	case 0x05: PRINTF(PSTR("translation fault, section\n")); break;
	case 0x07: PRINTF(PSTR("translation fault, page\n")); break;
	case 0x03: PRINTF(PSTR("access flag fault, section\n")); break;
	case 0x06: PRINTF(PSTR("access flag fault, page\n")); break;
	case 0x09: PRINTF(PSTR("domain fault, section\n")); break;
	case 0x0B: PRINTF(PSTR("domain fault, page\n")); break;
	case 0x0D: PRINTF(PSTR("permission fault, section\n")); break;
	case 0x0F: PRINTF(PSTR("permission fault, page\n")); break;
	case 0x08: PRINTF(PSTR("synchronous external abort, nontranslation\n")); break;
	case 0x16: PRINTF(PSTR("asynchronous external abort\n")); break;
	case 0x02: PRINTF(PSTR("debug event.\n")); break;
	default: PRINTF(PSTR("undefined Status=%02X\n"), Status); break;
	}
	unsigned i;
	for (i = 0; i < 8; ++ i)
	{
		PRINTF("marker [%2d] = %08lX\n", i, (& marker) [i]);
	}
	for (;;)
	{
	}
}

void FIQ_Handler(void)
{
	dbg_puts_impl_P(PSTR("FIQHandler trapped.\n"));
	dbg_puts_impl_P((__get_MPIDR() & 0x03) ? PSTR("CPUID=1\n") : PSTR("CPUID=0\n"));
	for (;;)
		;
}

void Hyp_Handler(void)
{
	dbg_puts_impl_P(PSTR("Hyp_Handler trapped.\n"));
	dbg_puts_impl_P((__get_MPIDR() & 0x03) ? PSTR("CPUID=1\n") : PSTR("CPUID=0\n"));
	for (;;)
		;
}


//	MVA
//	For more information about the possible meaning when the table shows that an MVA is required
// 	see Terms used in describing the maintenance operations on page B2-1272.
// 	When the data is stated to be an MVA, it does not have to be cache line aligned.

__STATIC_FORCEINLINE void L1_CleanDCache_by_Addr(volatile void *addr, int32_t dsize)
{
	if (dsize > 0)
	{
		int32_t op_size = dsize + (((uintptr_t) addr) & (DCACHEROWSIZE - 1U));
		uintptr_t op_mva = (uintptr_t) addr;
		__DSB();
		do
		{
			__set_DCCMVAC(op_mva);	// Clean data cache line by address.
			op_mva += DCACHEROWSIZE;
			op_size -= DCACHEROWSIZE;
		} while (op_size > 0);
		__DMB();     // ensure the ordering of data cache maintenance operations and their effects
	}
}

__STATIC_FORCEINLINE void L1_CleanInvalidateDCache_by_Addr(volatile void *addr, int32_t dsize)
{
	if (dsize > 0)
	{
		int32_t op_size = dsize + (((uintptr_t) addr) & (DCACHEROWSIZE - 1U));
		uintptr_t op_mva = (uintptr_t) addr;
		__DSB();
		do
		{
			__set_DCCIMVAC(op_mva);	// Clean and Invalidate data cache by address.
			op_mva += DCACHEROWSIZE;
			op_size -= DCACHEROWSIZE;
		} while (op_size > 0);
		__DMB();     // ensure the ordering of data cache maintenance operations and their effects
	}
}

__STATIC_FORCEINLINE void L1_InvalidateDCache_by_Addr(volatile void *addr, int32_t dsize)
{
	if (dsize > 0)
	{
		int32_t op_size = dsize + (((uintptr_t) addr) & (DCACHEROWSIZE - 1U));
		uintptr_t op_mva = (uintptr_t) addr;
		do
		{
			__set_DCIMVAC(op_mva);	// Invalidate data cache line by address.
			op_mva += DCACHEROWSIZE;
			op_size -= DCACHEROWSIZE;
		} while (op_size > 0);
		// Cache Invalidate operation is not follow by memory-writes
		__DMB();     // ensure the ordering of data cache maintenance operations and their effects
	}
}

#if (__L2C_PRESENT == 1)

__STATIC_FORCEINLINE void L2_CleanDCache_by_Addr(volatile void *addr, int32_t dsize)
{
	if (dsize > 0)
	{
		int32_t op_size = dsize + (((uint32_t) addr) & (DCACHEROWSIZE - 1U));
		uint32_t op_addr = (uint32_t) addr /* & ~(DCACHEROWSIZE - 1U) */;
		do
		{
			// Clean cache by physical address
			L2C_310->CLEAN_LINE_PA = op_addr;	// Atomic operation. These operations stall the slave ports until they are complete.
			op_addr += DCACHEROWSIZE;
			op_size -= DCACHEROWSIZE;
		} while (op_size > 0);
	}
}

__STATIC_FORCEINLINE void L2_CleanInvalidateDCache_by_Addr(volatile void *addr, int32_t dsize)
{
	if (dsize > 0)
	{
		int32_t op_size = dsize + (((uint32_t) addr) & (DCACHEROWSIZE - 1U));
		uint32_t op_addr = (uint32_t) addr /* & ~(DCACHEROWSIZE - 1U) */;
		do
		{
			// Clean and Invalidate cache by physical address
			L2C_310->CLEAN_INV_LINE_PA = op_addr;	// Atomic operation. These operations stall the slave ports until they are complete.
			op_addr += DCACHEROWSIZE;
			op_size -= DCACHEROWSIZE;
		} while (op_size > 0);
	}
}

__STATIC_FORCEINLINE void L2_InvalidateDCache_by_Addr(volatile void *addr, int32_t dsize)
{
	if (dsize > 0)
	{
		int32_t op_size = dsize + (((uint32_t) addr) & (DCACHEROWSIZE - 1U));
		uint32_t op_addr = (uint32_t) addr /* & ~(DCACHEROWSIZE - 1U) */;
		do
		{
			// Invalidate cache by physical address
			L2C_310->INV_LINE_PA = op_addr;	// Atomic operation. These operations stall the slave ports until they are complete.
			op_addr += DCACHEROWSIZE;
			op_size -= DCACHEROWSIZE;
		} while (op_size > 0);
	}
}
#endif /* (__L2C_PRESENT == 1) */

// Записать содержимое кэша данных в память
// применяетмся после начальной инициализации среды выполнния
void FLASHMEMINITFUNC arm_hardware_flush_all(void)
{
	L1C_CleanInvalidateDCacheAll();
#if (__L2C_PRESENT == 1)
	L2C_CleanInvAllByWay();
#endif
}

// Сейчас в эту память будем читать по DMA
void arm_hardware_invalidate(uintptr_t addr, int_fast32_t dsize)
{
	L1_InvalidateDCache_by_Addr((void *) addr, dsize);
#if (__L2C_PRESENT == 1)
	L2_InvalidateDCache_by_Addr((void *) addr, dsize);
#endif /* (__L2C_PRESENT == 1) */
}

// Сейчас эта память будет записываться по DMA куда-то
void arm_hardware_flush(uintptr_t addr, int_fast32_t dsize)
{
	L1_CleanDCache_by_Addr((void *) addr, dsize);
#if (__L2C_PRESENT == 1)
	L2_CleanDCache_by_Addr((void *) addr, dsize);
#endif /* (__L2C_PRESENT == 1) */
}

// Сейчас эта память будет записываться по DMA куда-то. Потом содержимое не требуется
void arm_hardware_flush_invalidate(uintptr_t addr, int_fast32_t dsize)
{
	L1_CleanInvalidateDCache_by_Addr((void *) addr, dsize);
#if (__L2C_PRESENT == 1)
	L2_CleanInvalidateDCache_by_Addr((void *) addr, dsize);
#endif /* (__L2C_PRESENT == 1) */
}



// получение из аппаратного счетчика монотонно увеличивающегося кода
// see SystemInit() in hardware.c
uint_fast32_t cpu_getdebugticks(void)
{
	uint32_t result;
	// Read CCNT Register
	//	MRC p15, 0, <Rt>, c9, c13, 0 : Read PMCCNTR into Rt
	//	MCR p15, 0, <Rt>, c9, c13, 0 : Write Rt to PMCCNTR
	//asm volatile ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(value));  
	__get_CP(15, 0, result, 9, 13, 0);
	return(result);
}


// #include "hardware.h"
// #include "formats.h"
//#include "hardware_r7s721.h"

//#define INTC_LEVEL_SENSITIVE    (0)     /* Level sense  */
//#define INTC_EDGE_TRIGGER       (1)     /* Edge trigger */

/* ==== Interrupt detection ==== */


#define INT_ID_MASK		0x3ffuL
/* Interrupt IDs reported by the HPPIR and IAR registers */
#define PENDING_G1_INTID	1022uL
/* Constant to indicate a spurious interrupt in all GIC versions */
#define GIC_SPURIOUS_INTERRUPT		1023uL
/*
 * Constant passed to the interrupt handler in the 'id' field when the
 * framework does not read the gic registers to determine the interrupt id.
 */
#define INTR_ID_UNAVAILABLE		0xFFFFFFFFuL

/*******************************************************************************
 * This function returns the id of the highest priority pending interrupt at
 * the GIC cpu interface. GIC_SPURIOUS_INTERRUPT is returned when there is no
 * interrupt pending.
 ******************************************************************************/
unsigned int gicv2_get_pending_interrupt_id(void)
{
	unsigned int id;

	id = GIC_GetHighPendingIRQ() & INT_ID_MASK;	// HIPPR

	/*
	 * Find out which non-secure interrupt it is under the assumption that
	 * the GICC_CTLR.AckCtl bit is 0.
	 */
	if (id == PENDING_G1_INTID)
		id = GICInterface->AHPPIR & INT_ID_MASK;

	return id;
}

//static RAMDTCM SPINLOCK_t giclock = SPINLOCK_INIT;

/* Вызывается из crt_CortexA.S со сброшенным флагом разрешения прерываний */
// See ARM IHI 0048B.b document
void IRQ_Handler_GIC(void)
{
	// per-cpu:
	// GICC_AHPPIR
	// GICC_HPPIR
	// GICC_IAR
	// GICC_EOIR
	// GICC_BPR
	// GICC_PMR
	//
	// global:
	// GICD_IPRIORITYR

//	const unsigned int gicver = (GIC_GetInterfaceId() >> 16) & 0x0F;


//	switch (gicver)
//	{
//	case 0x01:	// GICv1
//		/* Dummy read to avoid GIC 390 errata 801120 */
//		(void) GICInterface->HPPIR;
//		break;
//	default:
//		break;
//	}
	(void) GICInterface->HPPIR;

	//const uint_fast32_t gicc_iar = GIC_AcknowledgePending(); // CPUID in high bits, Interrupt ID
	const uint_fast32_t gicc_iar = GICInterface->IAR; // CPUID, Interrupt ID - use GIC_AcknowledgePending

	const IRQn_ID_t int_id = gicc_iar & 0x3ffuL;

	// IHI0048B_b_gic_architecture_specification.pdf
	// See ARM IHI 0048B.b 3.4.2 Special interrupt numbers when a GIC supports interrupt grouping

	if (int_id == 1022)
	{
	}

	if (int_id >= 1020)
	{
		//dbg_putchar('2');
		//SPIN_LOCK(& giclock);
		//GIC_SetPriority(0, GIC_GetPriority(0));	// GICD_IPRIORITYRn(0) = GICD_IPRIORITYRn(0);
		GICDistributor->IPRIORITYR [0] = GICDistributor->IPRIORITYR [0];
		//SPIN_UNLOCK(& giclock);

	}
	else if (int_id != 0 /* || GIC_GetIRQStatus(0) != 0 */)
	{
		const IRQHandler_t f = IRQ_GetHandler(int_id);

	#if WITHNESTEDINTERRUPTS

		if (f != (IRQHandler_t) 0)
		{
//			static const char hex [16] = "0123456789ABCDEF";
//			if ((int_id >> 8) & 0x0F)
//				dbg_putchar(hex [(int_id >> 8) & 0x0F]);
//			dbg_putchar(hex [(int_id >> 4) & 0x0F]);
//			dbg_putchar(hex [(int_id >> 0) & 0x0F]);
			__enable_irq();						/* modify I bit in CPSR */
			(* f)();	    /* Call interrupt handler */
			__disable_irq();					/* modify I bit in CPSR */
			//dbg_putchar('_');
		}

	#else /* WITHNESTEDINTERRUPTS */

		if (f != (IRQHandler_t) 0)
		{
			(* f)();	    /* Call interrupt handler */
		}

	#endif /* WITHNESTEDINTERRUPTS */

		//dbg_putchar('5');
	}
	else
	{
		//dbg_putchar('3');
		//SPIN_LOCK(& giclock);
		//GIC_SetPriority(0, GIC_GetPriority(0));	// GICD_IPRIORITYRn(0) = GICD_IPRIORITYRn(0);
		GICDistributor->IPRIORITYR [0] = GICDistributor->IPRIORITYR [0];
		//SPIN_UNLOCK(& giclock);
	}
	//dbg_putchar(' ');

	//GIC_EndInterrupt(gicc_iar);	/* CPUID, EOINTID */
	GICInterface->EOIR = gicc_iar;
}

uint8_t __attribute__ ((section(".stack"), used, aligned(64))) mystack [2048];
/******************************************************************************/

// TTB initialize

// SAM9XE512 bits
//#define TLB_NCNB 0x0DF2 // Noncachable, Nonbufferable 11 0 1111 1 00 10
//#define TLB_WT 0x0DFA // Write-through 11 0 1111 1 10 10
//#define TLB_WB 0x0DFE // Write-back 11 0 1111 1 11 10
//#define TLB_INVALID 0x0000 // Write-back 11 0 1111 1 11 10


/*

; ---- Parameter setting to level1 descriptor (bits 19:0) ----
; setting for Strongly-ordered memory
TTB_PARA_STRGLY             EQU     2_00000000000000000000110111100010
; setting for Outer and inner not cache normal memory
TTB_PARA_NORMAL_NOT_CACHE   EQU     2_00000000000000000001110111100010
; setting for Outer and inner write back, write allocate normal memory (Cacheable)
TTB_PARA_NORMAL_CACHE       EQU     2_00000000000000000001110111101110

; ---- Memory area size (MB) ----
M_SIZE_NOR      EQU     128             ; [Area00] CS0, CS1 area (for NOR flash)
M_SIZE_SDRAM    EQU     128             ; [Area01] CS2, CS3 area (for SDRAM)
M_SIZE_CS45     EQU     128             ; [Area02] CS4, CS5 area
M_SIZE_SPI      EQU     128             ; [Area03] SPI, SP2 area (for Serial flash)
M_SIZE_RAM      EQU     10              ; [Area04] Internal RAM
M_SIZE_IO_1     EQU     502             ; [Area05] I/O area 1
M_SIZE_NOR_M    EQU     128             ; [Area06] CS0, CS1 area (for NOR flash) (mirror)
M_SIZE_SDRAM_M  EQU     128             ; [Area07] CS2, CS3 area (for SDRAM) (mirror)
M_SIZE_CS45_M   EQU     128             ; [Area08] CS4, CS5 area (mirror)
M_SIZE_SPI_M    EQU     128             ; [Area09] SPI, SP2 area (for Serial flash) (mirror)
M_SIZE_RAM_M    EQU     10              ; [Area10] Internal RAM (mirror)
M_SIZE_IO_2     EQU     2550            ; [Area11] I/O area 2
;===================================================================
; Cortex-A9 MMU Configuration
; Set translation table base
;===================================================================
    ;;; Cortex-A9 supports two translation tables
    ;;; Configure translation table base (TTB) control register cp15,c2
    ;;; to a value of all zeros, indicates we are using TTB register 0.
    MOV  r0,#0x0
    MCR  p15, 0, r0, c2, c0, 2      ;;; TTBCR

    ;;; write the address of our page table base to TTB register 0
    LDR  r0,=||Image$$TTB$$ZI$$Base||
    MOV  r1, #0x08                  ;;; RGN=b01  (outer cacheable write-back cached, write allocate)
                                    ;;; S=0      (translation table walk to non-shared memory)
    ORR  r1,r1,#0x40                ;;; IRGN=b01 (inner cacheability for the translation table walk is Write-back Write-allocate)
    ORR  r0,r0,r1
    MCR  p15, 0, r0, c2, c0, 0      ;;; TTBR0

;===================================================================
; PAGE TABLE generation
; Generate the page tables
; Build a flat translation table for the whole address space.
; ie: Create 4096 1MB sections from 0x000xxxxx to 0xFFFxxxxx
; 31                 20 19  18  17  16 15  14   12 11 10  9  8     5   4    3 2   1 0
; |section base address| 0  0  |nG| S |AP2|  TEX  |  AP | P | Domain | XN | C B | 1 0|
;
; Bits[31:20]   - Top 12 bits of VA is pointer into table
; nG[17]=0      - Non global, enables matching against ASID in the TLB when set.
; S[16]=0       - Indicates normal memory is shared when set.
; AP2[15]=0
; AP[11:10]=11  - Configure for full read/write access in all modes
; TEX[14:12]=000
; CB[3:2]= 00   - Set attributes to Strongly-ordered memory.
;                 (except for the descriptor where code segment is based, see below)
; IMPP[9]=0     - Ignored
; Domain[5:8]=1111   - Set all pages to use domain 15
; XN[4]=0       - Execute never disabled
; Bits[1:0]=10  - Indicate entry is a 1MB section
;===================================================================

  */

// Short-descriptor format memory region attributes, without TEX remap
// When using the Short-descriptor translation table formats, TEX remap is disabled when SCTLR.TRE is set to 0.

// For TRE - see
// B4.1.127 PRRR, Primary Region Remap Register, VMSA

#define APRWval 		0x03	/* Full access */
#define APROval 		0x06	/* All write accesses generate Permission faults */
#define DOMAINval	0x0F
#define SECTIONval	0x02	/* 0b10, Section or Supersection */

/* Table B3-10 TEX, C, and B encodings when TRE == 0 */

#define TEXval_STGORD		0x00	/* Strongly-ordered, shareable */
#define Bval_STGORD			0x00	/* Strongly-ordered, shareable */
#define Cval_STGORD			0x00	/* Strongly-ordered, shareable */

#define TEXval_WBCACHE		0x01	/* Outer and Inner Write-Back, Write-Allocate */
#define Bval_WBCACHE		0x01	/* Outer and Inner Write-Back, Write-Allocate */
#define Cval_WBCACHE		0x01	/* Outer and Inner Write-Back, Write-Allocate */

#define TEXval_NOCACHE		0x01	/* Outer and Inner Non-cacheable, S bit */
#define Bval_NOCACHE		0x00	/* Outer and Inner Non-cacheable, S bit */
#define Cval_NOCACHE		0x00	/* Outer and Inner Non-cacheable, S bit */

// normal
#define TEXval_DEVICE		0x02	/* Non-shareable Device */
#define Bval_DEVICE			0x00	/* Non-shareable Device */
#define Cval_DEVICE			0x00	/* Non-shareable Device */

#if WITHSMPSYSTEM
	#define SHAREDval 1		// required for ldrex.. and strex.. functionality
#else /* WITHSMPSYSTEM */
	#define SHAREDval 0
#endif /* WITHSMPSYSTEM */

#define NoSHAREDval 0

// See B3.5.2 in DDI0406C_C_arm_architecture_reference_manual.pdf


#define	TTB_PARA(TEX, B, C, DOMAIN, SHARED, AP, XN) ( \
		(SECTIONval) * (1uL << 0) |	/* 0b10, Section or Supersection */ \
		(B) * (1uL << 2) |	/* B */ \
		(C) * (1uL << 3) |	/* C */ \
		(XN) * (1uL << 4) |	/* XN The Execute-never bit. */ \
		(DOMAIN) * (1uL << 5) |	/* DOMAIN */ \
		0 * (1uL << 9) |	/* implementation defined */ \
		(((AP) >> 0) & 0x03) * (1uL << 10) |	/* AP [1..0] */ \
		(TEX) * (1uL << 12) |	/* TEX */ \
		(((AP) >> 2) & 0x01) * (1uL << 15) |	/* AP[2] */ \
		(SHARED) * (1uL << 16) |	/* S */ \
		0 * (1uL << 17) |	/* nG */ \
		0 * (1uL << 18) |	/* 0 */ \
		0 * (1uL << 19) |	/* NS */ \
		0 \
	)

//; setting for Strongly-ordered memory
//#define	TTB_PARA_STRGLY             0b_0000_0000_1101_1110_0010
// not used
#define	TTB_PARA_STRGLY TTB_PARA(TEXval_STGORD, Bval_STGORD, Cval_STGORD, DOMAINval, 0 /* Shareable mot depend of this bit */, APRWval, 1)


//; setting for Outer and inner not cache normal memory
// not used
//#define	TTB_PARA_NORMAL_NOT_CACHE(ro, xn) TTB_PARA(TEXval_NOCACHE, Bval_NOCACHE, Cval_NOCACHE, DOMAINval, SHAREDval, (ro) ? APROval : APRWval, (xn) != 0)

//; setting for Outer and inner write back, write allocate normal memory (Cacheable)
//#define	TTB_PARA_NORMAL_CACHE       0b_0000_0001_1101_1110_1110
#define	TTB_PARA_NORMAL_CACHE(ro, xn) TTB_PARA(TEXval_WBCACHE, Bval_WBCACHE, Cval_WBCACHE, DOMAINval, SHAREDval, (ro) ? APROval : APRWval, (xn) != 0)

#define	TTB_PARA_DEVICE TTB_PARA(TEXval_DEVICE, Bval_DEVICE, Cval_DEVICE, DOMAINval, NoSHAREDval, APRWval, 1 /* XN=1 */)

#define	TTB_PARA_NO_ACCESS 0

/*
 * https://community.st.com/s/question/0D73W000000UagD/what-a-type-of-mmu-memory-regions-recommended-for-regions-with-peripheralsstronglyordered-or-device?s1oid=00Db0000000YtG6&s1nid=0DB0X000000DYbd&emkind=chatterCommentNotification&s1uid=0050X000007vtUt&emtm=1599464922440&fromEmail=1&s1ext=0&t=1599470826880
 *
 *
 * PatrickF (ST Employee)

2 hours ago
Hello,

"Device" should be used for all peripherals to get best performance.
There is no rationale to use "Strongly-Ordered" with Cortex-A7
(explanation should be found by deep analysis/understanding of various ARM documents).
 *
 *
*/

static uint32_t
FLASHMEMINITFUNC
ttb_accessbits(uintptr_t a, int ro, int xn)
{
	const uint32_t addrbase = a & 0xFFF00000uL;

	if (a < 0x10000000uL)			// BOOT
		return addrbase | TTB_PARA_NO_ACCESS;			// NULL pointers access trap

	if (a >= 0x20000000uL && a < 0x30000000uL)			// SYSRAM
		return addrbase | TTB_PARA_NORMAL_CACHE(ro, 0);

	if (a >= 0x40000000uL && a < 0x60000000uL)			//  peripherials 1, peripherials 2
		return addrbase | TTB_PARA_DEVICE;

	if (a >= 0x60000000uL && a < 0x70000000uL)			//  FMC NOR
		return addrbase | TTB_PARA_NORMAL_CACHE(ro, 0);

	if (a >= 0x70000000uL && a < 0xA0000000uL)			//  QUADSPI, FMC NAND, ...
		return addrbase | TTB_PARA_NORMAL_CACHE(ro || 1, 0);

	if (a >= 0xA0000000uL && a < 0xC0000000uL)			//  GIC
		return addrbase | TTB_PARA_DEVICE;
	// 1 GB DDR RAM memory size allowed
	if (a >= 0xC0000000uL)								// DDR memory
		return addrbase | TTB_PARA_NORMAL_CACHE(ro, 0);

	return addrbase | TTB_PARA_NO_ACCESS;
}

/* Загрузка TTBR, инвалидация кеш памяти и включение MMU */
static void FLASHMEMINITFUNC sysinit_ttbr_initialize(void)
{
	extern volatile uint32_t __TTB_BASE;		// получено из скрипта линкера
	volatile uint32_t * const tlbbase = & __TTB_BASE;

	//CP15_writeTTBCR(0);
	   /* Set location of level 1 page table
	    ; 31:14 - Translation table base addr (31:14-TTBCR.N, TTBCR.N is 0 out of reset)
	    ; 13:7  - 0x0
	    ; 6     - IRGN[0] 0x1  (Inner WB WA)
	    ; 5     - NOS     0x0  (Non-shared)
	    ; 4:3   - RGN     0x01 (Outer WB WA)
	    ; 2     - IMP     0x0  (Implementation Defined)
	    ; 1     - S       0x0  (Non-shared)
	    ; 0     - IRGN[1] 0x0  (Inner WB WA) */
	__set_TTBR0((unsigned int) tlbbase | 0x48);	// TTBR0
	//CP15_writeTTB1((unsigned int) tlbbase | 0x48);	// TTBR1
	  __ISB();

	// Program the domain access register
	//__set_DACR(0x55555555); // domain 15: access are not checked
	__set_DACR(0xFFFFFFFF); // domain 15: access are not checked

	MMU_InvalidateTLB();

	// Обеспечиваем нормальную обработку RESEТ
	L1C_InvalidateDCacheAll();
	L1C_InvalidateICacheAll();
	L1C_InvalidateBTAC();
#if (__L2C_PRESENT == 1)
	L2C_InvAllByWay();
#endif

	MMU_Enable();
}

static void FLASHMEMINITFUNC
ttb_initialize(uint32_t (* accessbits)(uintptr_t a, int ro, int xn), uintptr_t textstart, uint_fast32_t textsize)
{
	extern volatile uint32_t __TTB_BASE;		// получено из скрипта линкера
	volatile uint32_t * const tlbbase = & __TTB_BASE;
	unsigned i;
	const uint_fast32_t pagesize = (1uL << 20);

	for (i = 0; i < 4096; ++ i)
	{
		const uintptr_t address = (uintptr_t) i << 20;
		tlbbase [i] =  accessbits(address, 0, 0);
	}
	/* Установить R/O атрибуты для указанной области */
	while (textsize >= pagesize)
	{
		tlbbase [textstart / pagesize] =  accessbits(textstart, 0 * 1, 0);
		textsize -= pagesize;
		textstart += pagesize;
	}

	/* R/O, XN for pages table. - 1 MB size. */
	tlbbase [(uintptr_t) tlbbase / pagesize] = accessbits((uintptr_t) tlbbase, 1, 1);
}

// TODO: use MMU_TTSection. See also MMU_TTPage4k MMU_TTPage64k and MMU_CreateTranslationTable
// с точностью до 1 мегабайта
static void FLASHMEMINITFUNC
ttb_map(
	uintptr_t va,	/* virtual address */
	uintptr_t la,	/* linear (physical) address */
	uint32_t (* accessbits)(uintptr_t a)
	)
{
	volatile extern uint32_t __TTB_BASE;		// получено из скрипта линкера
	volatile uint32_t * const tlbbase = & __TTB_BASE;
	unsigned i = va >> 20;
	tlbbase [i] =  accessbits(la);
}


static void FLASHMEMINITFUNC sysinit_fpu_initialize(void)
{
	// FPU
	__FPU_Enable();
}

static void FLASHMEMINITFUNC sysintt_sdram_initialize(void)
{
}

static void FLASHMEMINITFUNC sysinit_debug_initialize(void)
{
#if WITHDEBUG
	{
		// Поддержка для функций диагностики быстродействия BEGINx_STAMP/ENDx_STAMP - audio.c
		// From https://stackoverflow.com/questions/3247373/how-to-measure-program-execution-time-in-arm-cortex-a8-processor

		enum { do_reset = 0, enable_divider = 0 };
		// in general enable all counters (including cycle counter)
		int32_t value = 1;

		// peform reset:
		if (do_reset)
		{
			value |= 2;     // reset all counters to zero.
			value |= 4;     // reset cycle counter to zero.
		}

		if (enable_divider)
			value |= 8;     // enable "by 64" divider for CCNT.

		value |= 16;

		// program the performance-counter control-register:
		//asm volatile ("MCR p15, 0, %0, c9, c12, 0\t\n" :: "r"(value));
		__set_CP(15, 0, value, 9, 12, 0);

		// enable all counters:
		//asm volatile ("MCR p15, 0, %0, c9, c12, 1\t\n" :: "r"(0x8000000f));
		__set_CP(15, 0, 0x8000000f, 9, 12, 1);

		// clear overflows:
		//asm volatile ("MCR p15, 0, %0, c9, c12, 3\t\n" :: "r"(0x8000000f));
		__set_CP(15, 0, 0x8000000f, 9, 12, 3);
	}

	HARDWARE_DEBUG_INITIALIZE();
	HARDWARE_DEBUG_SET_SPEED(DEBUGSPEED);
#endif /* WITHDEBUG */
}

/** \brief  Set HVBAR

    This function assigns the given value to the Hyp Vector Base Address Register.

    \param [in]    hvbar  Hyp Vector Base Address Register value to set
 */
__STATIC_FORCEINLINE void __set_HVBAR(uint32_t hvbar)
{
	// cp, op1, Rt, CRn, CRm, op2
  __set_CP(15, 4, hvbar, 12, 0, 0);
}

static void FLASHMEMINITFUNC sysinit_vbar_initialize(void)
{
	extern unsigned long __Vectors;

	const uintptr_t vbase = (uintptr_t) & __Vectors;

	__set_VBAR(vbase);	 // Set Vector Base Address Register (bits 4..0 should be zero)
	__set_MVBAR(vbase);	 // Set Monitor Vector Base Address Register (bits 4..0 should be zero) - на работу не вличет... но на всякий случай
	//__set_HVBAR(vbase);	 // Set Hyp Vector Base Address Register

	__set_SCTLR(__get_SCTLR() & ~ SCTLR_V_Msk);	// v=0 - use VBAR as vectors address
	__set_SCTLR(__get_SCTLR() & ~ SCTLR_A_Msk);	// 0 = Strict alignment fault checking disabled. This is the reset value.

	//PRINTF("vbar=%08lX, mvbar=%08lX\n", __get_VBAR(), __get_MVBAR());

}

static void FLASHMEMINITFUNC sysinit_mmu_initialize(void)
{
	//PRINTF("sysinit_mmu_initialize\n");
	// MMU iniitialize
	extern uint32_t __data_start__;
	// MMU iniitialize
	ttb_initialize(ttb_accessbits, 0xC0000000, (uintptr_t) & __data_start__ - 0xC0000000);
	sysinit_ttbr_initialize();	/* Загрузка TTBR, инвалидация кеш памяти и включение MMU */
	//PRINTF("MMU initialized\n");
}

static void FLASHMEMINITFUNC sysinit_cache_initialize(void)
{
	L1C_InvalidateDCacheAll();
	L1C_InvalidateICacheAll();
	L1C_InvalidateBTAC();
	L1C_EnableCaches();
	L1C_EnableBTAC();
	L1C_EnableCaches();
	L1C_EnableBTAC();
	// set the ACTLR.SMP
	__set_ACTLR(__get_ACTLR() | ACTLR_SMP_Msk);
	__ISB();
	__DSB();
}

static void FLASHMEMINITFUNC
sysinit_cache_L2_cpu0_initialize(void)
{
#if (__CORTEX_A == 7U) || (__CORTEX_A == 9U) || CPUSTYLE_ARM9
	#if (__L2C_PRESENT == 1) && defined (PL310_DATA_RAM_LATENCY)
		L2C_Disable();
		* (volatile uint32_t *) ((uintptr_t) L2C_310 + 0x010C) = PL310_DATA_RAM_LATENCY;	// reg1_data_ram_control
		* (volatile uint32_t *) ((uintptr_t) L2C_310 + 0x0108) = PL310_TAG_RAM_LATENCY;	// reg1_tag_ram_control
	#endif /* (__L2C_PRESENT == 1) */
	#if (__L2C_PRESENT == 1)
		// Enable Level 2 Cache
		L2C_InvAllByWay();
		L2C_Enable();
	#endif
	//arm_hardware_flush_all();
#endif /* (__CORTEX_A == 7U) || (__CORTEX_A == 9U) */
}

static void FLASHMEMINITFUNC sysinit_cache_cpu1_initialize(void)
{
}

/* функция вызывается из start-up до копирования в SRAM всех "быстрых" функций и до инициализации переменных
*/
// watchdog disable, clock initialize, cache enable
void FLASHMEMINITFUNC SystemInit(void)
{
	sysinit_fpu_initialize();
	sysinit_pll_initialize();	// PLL iniitialize
	sysinit_debug_initialize();
	sysintt_sdram_initialize();
	sysinit_vbar_initialize();		// interrupt vectors relocate
	sysinit_mmu_initialize();
	sysinit_cache_initialize();	// caches iniitialize
	sysinit_cache_L2_cpu0_initialize();	// L2 cache, SCU initialize
}


static void cortexa_cpuinfo(void)
{
	volatile uint_fast32_t vvv;
	dbg_putchar('$');
	PRINTF(PSTR("CPU%u: VBAR=%p, TTBR0=%p, cpsr=%08lX, SCTLR=%08lX, ACTLR=%08lX, sp=%08lX\n"), (unsigned) (__get_MPIDR() & 0x03),  (unsigned long) __get_VBAR(), (unsigned long) __get_TTBR0(), (unsigned long) __get_CPSR(), (unsigned long) __get_SCTLR(), (unsigned long) __get_ACTLR(),(unsigned long) & vvv);
}

#if WITHSMPSYSTEM

/*
 * Cores secure magic numbers
 * Constant to be stored in bakcup register
 * BOOT_API_MAGIC_NUMBER_TAMP_BCK_REG_IDX
 */
//#define BOOT_API_A7_CORE0_MAGIC_NUMBER				0xCA7FACE0U
#define BOOT_API_A7_CORE1_MAGIC_NUMBER				0xCA7FACE1U

/*
 * TAMP_BCK4R register index
 * This register is used to write a Magic Number in order to restart
 * Cortex A7 Core 1 and make it execute @ branch address from TAMP_BCK5R
 */
//#define BOOT_API_CORE1_MAGIC_NUMBER_TAMP_BCK_REG_IDX		4U

/*
 * TAMP_BCK5R register index
 * This register is used to contain the branch address of
 * Cortex A7 Core 1 when restarted by a TAMP_BCK4R magic number writing
 */
//#define BOOT_API_CORE1_BRANCH_ADDRESS_TAMP_BCK_REG_IDX		5U

/*******************************************************************************
 * STM32MP1 handler called when a power domain is about to be turned on. The
 * mpidr determines the CPU to be turned on.
 * call by core 0 to activate core 1
 ******************************************************************************/
static void cortexa_mp_cpu1_start(uintptr_t startfunc)
{
	PWR->CR1 |= PWR_CR1_DBP;	// 1: Write access to RTC and backup domain registers enabled.
	(void) PWR->CR1;
	while ((PWR->CR1 & PWR_CR1_DBP) == 0)
		;

	RCC->MP_APB5ENSETR = RCC_MP_APB5ENSETR_RTCAPBEN;
	(void) RCC->MP_APB5ENSETR;
	RCC->MP_APB5LPENSETR = RCC_MP_APB5LPENSETR_RTCAPBLPEN;  // Включить тактирование
	(void) RCC->MP_APB5LPENSETR;

	RCC->MP_AHB5ENSETR = RCC_MP_AHB5ENSETR_BKPSRAMEN;
	(void) RCC->MP_AHB5ENSETR;
	RCC->MP_AHB5LPENSETR = RCC_MP_AHB5LPENSETR_BKPSRAMLPEN;
	(void) RCC->MP_AHB5LPENSETR;

	/* Write entrypoint in backup RAM register */
	TAMP->BKP5R = startfunc;	// Invoke at SVC context
	(void) TAMP->BKP5R;

	/* Write magic number in backup register */
	TAMP->BKP4R = BOOT_API_A7_CORE1_MAGIC_NUMBER;
	(void) TAMP->BKP4R;

//	PWR->CR1 &= ~ PWR_CR1_DBP;	// 0: Write access to RTC and backup domain registers disabled.
//	(void) PWR->CR1;
//	while ((PWR->CR1 & PWR_CR1_DBP) != 0)
//		;

	arm_hardware_flush_all();	// startup code should be copyed in to sysram for example.

	/* Generate an IT to core 1 */
	GIC_SendSGI(SGI8_IRQn, 0x01 << 1, 0x00);	// CPU1, filer=0
}


static RAMDTCM SPINLOCK_t cpu1init;

// Инициализация второго ппрцессора
void Reset_CPUn_Handler(void)
{
#if (__CORTEX_A == 9U)
	// set the ACTLR.SMP
	// 0x02: L2 Prefetch hint enable
	__set_ACTLR(__get_ACTLR() | ACTLR_SMP_Msk | ACTLR_L1PE_Msk | ACTLR_FW_Msk | 0x02);
	__ISB();
	__DSB();
#elif (__CORTEX_A == 7U)
	// set the ACTLR.SMP
	__set_ACTLR(__get_ACTLR() | ACTLR_SMP_Msk);
	__ISB();
	__DSB();
#endif /* (__CORTEX_A == 9U) */

	sysinit_fpu_initialize();
	sysinit_vbar_initialize();		// interrupt vectors relocate
	sysinit_ttbr_initialize();		// TODO: убрать работу с L2 для второго процессора - Загрузка TTBR, инвалидация кеш памяти и включение MMU
	sysinit_cache_initialize();	// caches iniitialize
	sysinit_cache_cpu1_initialize();

	{
		GIC_Enable();
	#if WITHNESTEDINTERRUPTS
		GIC_SetInterfacePriorityMask(ARM_CA9_ENCODE_PRIORITY(PRI_USER));
	#endif /* WITHNESTEDINTERRUPTS */
	}

	L1C_InvalidateDCacheAll();
	L1C_InvalidateICacheAll();
	L1C_InvalidateBTAC();
	L1C_EnableCaches();
	L1C_EnableBTAC();
	L1C_EnableCaches();
	L1C_EnableBTAC();
	#if (__L2C_PRESENT == 1)
		// L2 контроллерп едминственный и уже инициализирован
		// Enable Level 2 Cache
		//L2C_Enable();
		//L2C_InvAllByWay();
	#endif

	cortexa_cpuinfo();

	arm_hardware_populte_second_initialize();
	__enable_irq();
	SPIN_UNLOCK(& cpu1init);

	// Idle loop
	for (;;)
	{
		__WFI();
	}
}

// Вызывается из main
void cpump_initialize(void)
{
#if (__CORTEX_A != 0) || (__CORTEX_A == 9U)

	SystemCoreClock = CPU_FREQ;

#if WITHSMPSYSTEM

#if (__CORTEX_A == 9U)
	// set the ACTLR.SMP
	// 0x02: L2 Prefetch hint enable
	__set_ACTLR(__get_ACTLR() | ACTLR_SMP_Msk | ACTLR_L1PE_Msk | ACTLR_FW_Msk | 0x02);
	__ISB();
	__DSB();
#elif (__CORTEX_A == 7U)
	// set the ACTLR.SMP
	// STM32MP1: already set
	__set_ACTLR(__get_ACTLR() | ACTLR_SMP_Msk);
	__ISB();
	__DSB();
#endif /* (__CORTEX_A == 9U) */

	cortexa_cpuinfo();
	SPINLOCK_INITIALIZE(& cpu1init);
	SPIN_LOCK(& cpu1init);
	cortexa_mp_cpu1_start((uintptr_t) Reset_CPU1_Handler);
	SPIN_LOCK(& cpu1init);
	SPIN_UNLOCK(& cpu1init);

#else /* WITHSMPSYSTEM */
	cortexa_cpuinfo();

#endif /* WITHSMPSYSTEM */
#endif /* (__CORTEX_A == 7U) || (__CORTEX_A == 9U) */

}

#else /* WITHSMPSYSTEM */

void Reset_CPUn_Handler(void)
{

}

// Вызывается из main
void cpump_initialize(void)
{
	SystemCoreClock = CPU_FREQ;
	cortexa_cpuinfo();

}

#endif /* WITHSMPSYSTEM */





// секция init больше не нужна
void cpu_initdone(void)
{
}

void arm_hardware_reset(void)
{
	RCC->MP_GRSTCSETR = RCC_MP_GRSTCSETR_MPSYSRST_Msk;
	for (;;)
		;
}

/* разрешение сторожевого таймера в устройстве */
void watchdog_initialize(void)
{
}

/* перезапуск сторожевого таймера */
void watchdog_ping(void)
{
}

// optimizer test: from electronix.ru - should be one divmod call
/*
uint8_t xxxxxpos(uint8_t num) // num = 0..8
{
    uint8_t pos = 0x40;
    while (num >= 3) {
        pos += 0x40;
        num -= 3;
    }
    pos += num * 7;
    return pos;
}

*/


unsigned long ulmin(
	unsigned long a,
	unsigned long b)
{
	return a < b ? a : b;
}

unsigned long ulmax(
	unsigned long a,
	unsigned long b)
{
	return a > b ? a : b;
}

uint_fast32_t ulmin32(uint_fast32_t a, uint_fast32_t b)
{
	return a < b ? a : b;
}

uint_fast32_t ulmax32(uint_fast32_t a, uint_fast32_t b)
{
	return a > b ? a : b;
}

uint_fast16_t ulmin16(uint_fast16_t a, uint_fast16_t b)
{
	return a < b ? a : b;
}

uint_fast16_t ulmax16(uint_fast16_t a, uint_fast16_t b)
{
	return a > b ? a : b;
}

/* получить 16-бит значение */
uint_fast16_t
USBD_peek_u16(
	const uint8_t * buff
	)
{
	return
		((uint_fast16_t) buff [1] << 8) +
		((uint_fast16_t) buff [0] << 0);
}

/* получить 24-бит значение */
uint_fast32_t
USBD_peek_u24(
	const uint8_t * buff
	)
{
	return
		((uint_fast32_t) buff [2] << 16) +
		((uint_fast32_t) buff [1] << 8) +
		((uint_fast32_t) buff [0] << 0);
}

/* получить 32-бит значение */
uint_fast32_t
USBD_peek_u32(
	const uint8_t * buff
	)
{
	return
		((uint_fast32_t) buff [3] << 24) +
		((uint_fast32_t) buff [2] << 16) +
		((uint_fast32_t) buff [1] << 8) +
		((uint_fast32_t) buff [0] << 0);
}

/* записать в буфер для ответа 32-бит значение */
unsigned USBD_poke_u32(uint8_t * buff, uint_fast32_t v)
{
	buff [0] = LO_BYTE(v);
	buff [1] = HI_BYTE(v);
	buff [2] = HI_24BY(v);
	buff [3] = HI_32BY(v);

	return 4;
}

/* получить 32-бит значение */
/* Big endian memory layout */
uint_fast32_t
USBD_peek_u32_BE(
	const uint8_t * buff
	)
{
	return
		((uint_fast32_t) buff [0] << 24) +
		((uint_fast32_t) buff [1] << 16) +
		((uint_fast32_t) buff [2] << 8) +
		((uint_fast32_t) buff [3] << 0);
}

/* получить 64-бит значение */
/* Big endian memory layout */
uint_fast64_t
USBD_peek_u64_BE(
	const uint8_t * buff
	)
{
	return
		((uint_fast64_t) buff [0] << 56) +
		((uint_fast64_t) buff [1] << 48) +
		((uint_fast64_t) buff [2] << 40) +
		((uint_fast64_t) buff [3] << 32) +
		((uint_fast64_t) buff [4] << 24) +
		((uint_fast64_t) buff [5] << 16) +
		((uint_fast64_t) buff [6] << 8) +
		((uint_fast64_t) buff [7] << 0);
}

/* записать в буфер для ответа 32-бит значение */
/* Big endian memory layout */
unsigned USBD_poke_u32_BE(uint8_t * buff, uint_fast32_t v)
{
	buff [3] = LO_BYTE(v);
	buff [2] = HI_BYTE(v);
	buff [1] = HI_24BY(v);
	buff [0] = HI_32BY(v);

	return 4;
}

/* записать в буфер для ответа 64-бит значение */
/* Big endian memory layout */
unsigned USBD_poke_u64_BE(uint8_t * buff, uint_fast64_t v)
{
	buff [0] = (v >> 56) & 0xFF;
	buff [1] = (v >> 48) & 0xFF;
	buff [2] = (v >> 40) & 0xFF;
	buff [3] = (v >> 32) & 0xFF;
	buff [4] = (v >> 24) & 0xFF;
	buff [5] = (v >> 16) & 0xFF;
	buff [6] = (v >> 8) & 0xFF;
	buff [7] = (v >> 0) & 0xFF;

	return 8;
}

/* записать в буфер для ответа 24-бит значение */
unsigned USBD_poke_u24(uint8_t * buff, uint_fast32_t v)
{
	buff [0] = LO_BYTE(v);
	buff [1] = HI_BYTE(v);
	buff [2] = HI_24BY(v);

	return 3;
}

/* записать в буфер для ответа 16-бит значение */
unsigned USBD_poke_u16(uint8_t * buff, uint_fast16_t v)
{
	buff [0] = LO_BYTE(v);
	buff [1] = HI_BYTE(v);

	return 2;
}

/* записать в буфер для ответа 16-бит значение */
/* Big endian memory layout */
unsigned USBD_poke_u16_BE(uint8_t * buff, uint_fast16_t v)
{
	buff [1] = LO_BYTE(v);
	buff [0] = HI_BYTE(v);

	return 2;
}

/* записать в буфер для ответа 8-бит значение */
unsigned USBD_poke_u8(uint8_t * buff, uint_fast8_t v)
{
	buff [0] = v;

	return 1;
}

#if CPUSTYLE_ARM

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if (__CORTEX_M == 0)

// Используется в случае наличия ключа ld -nostartfiles
// Так же смотреть вокруг software_init_hook
// see https://github.com/gmarkall/newlib/blob/de33102df7309e77441dfc76aa8cdef6d26da209/newlib/libc/sys/arm/crt0.S#L1

extern int main(void);
extern void __libc_init_array(void);

void __NO_RETURN _start(void)
{
	__libc_init_array();	// invoke constructors
    /* Branch to main function */
    main();

     /* Infinite loop */
	for (;;)
		;
}

// call after __preinit_array_xxx and before __init_array_xxx passing
void _init(void)
{
}

void * __dso_handle;

void _fini(void)
{
	for (;;)
		;
}

#endif /* (__CORTEX_M == 0) */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#if 1//WITHUSEMALLOC

/*
 *
 * ****************************
 */
#include <sys/stat.h>
#include <string.h>
#include <errno.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static int SER_GetChar(void)
{

	return (-1);
}

/*-- GCC - Newlib runtime support --------------------------------------------*/

int __attribute__((used)) (_open)(const char * path, int flags, ...)
{
	return (-1);
}

int __attribute__((used)) (_close)(int fd) {
	return (-1);
}

int __attribute__((used)) (_lseek)(int fd, int ptr, int dir)
{
	return (0);
}

int __attribute__((used)) (_fstat)(int fd, struct stat * st)
{
	memset(st, 0, sizeof(*st));
	st->st_mode = S_IFCHR;
	return (0);
}

int __attribute__((used)) (_isatty)(int fd)
{
	return (1);
}

int __attribute__((used)) (_read)(int fd, char * ptr, int len) {
	char c;
	int i;

	for (i = 0; i < len; i++) {
		c = SER_GetChar();
		if (c == 0x0D)
			break;
		*ptr++ = c;
		dbg_putchar(c);
	}
	return (len - i);
}

int __attribute__((used)) (_write)(int fd, char * ptr, int len)
{
	int i;

	for (i = 0; i < len; i++)
		dbg_putchar(* ptr ++);
	return (i);
}

#if WITHUSEMALLOC
#if (CPUSTYLE_STM32MP1 || CPUSTYLE_XC7Z || CPUSTYPE_ALLWNV3S) && ! WITHISBOOTLOADER

	static RAMHEAP uint8_t heapplace [48 * 1024uL * 1024uL];

#else /* CPUSTYLE_STM32MP1 */

	static RAMHEAP uint8_t heapplace [8 * 1024uL];

#endif /* CPUSTYLE_STM32MP1 */

extern int __HeapBase;
extern int __HeapLimit;

caddr_t __attribute__((used)) (_sbrk)(int incr)
{
	unsigned alignment = DCACHEROWSIZE;
	static char * heap;
	char * prev_heap;

	incr = (incr + (alignment - 1u)) & ~ (alignment - 1u);

	if (heap == NULL)
	{
		heap = (char *) &__HeapBase;
	}

	//PRINTF(PSTR("_sbrk: incr=%X, new heap=%X, & __HeapBase=%p, & __HeapLimit=%p\n"), incr, heap + incr, & __HeapBase, & __HeapLimit);

	prev_heap = heap;

	if ((heap + incr) > (char *) &__HeapLimit)
	{
		//errno = ENOMEM;
		return (caddr_t) -1;
	}

	heap += incr;

	return (caddr_t) prev_heap;
}
#endif /* WITHUSEMALLOC */

// Corte-A9 reauire


void __attribute__((used)) (_exit)(int code)
{
	for (;;)
		;
}

int __attribute__((used)) (_kill)(int id)
{
	return (-1);
}

int __attribute__((used)) (_getpid)(int id)
{
	return (-1);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	// at all

#endif /* CPUSTYLE_ARM */

