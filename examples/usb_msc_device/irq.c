//
// Проект HF Dream Receiver (КВ приёмник мечты)
// автор Гена Завидовский mgs2001@mail.ru
// UA1ARN
//

#include "paramdepend_irq.h"
//#include "formats.h"
#include "hardware.h"
#include <string.h>

/*
	ARM IHI 0048B.b (IHI0048B_b_gic_architecture_specification.pdf).
	4.3.11 Interrupt Priority Registers, GICD_IPRIORITYRn says:
	These registers are byte-accessible.
*/

uint32_t gARM_OVERREALTIME_PRIORITY;
uint32_t gARM_REALTIME_PRIORITY;
uint32_t gARM_SYSTEM_PRIORITY;
uint32_t gARM_BASEPRI_ONLY_REALTIME;
uint32_t gARM_BASEPRI_ONLY_OVERREALTIME;
uint32_t gARM_BASEPRI_ALL_ENABLED;

#if CPUSTYLE_ARM && WITHSMPSYSTEM
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHEJCHB.html
// Memory attribute SHARED required for ldrex.. and strex.. functionality
void spin_lock(volatile spinlock_t *p, const char *file, int line)
{
#if WITHDEBUG
	unsigned v = 0xFFFFFFF;
#endif /* WITHDEBUG */
	// Note: __LDREXW and __STREXW are CMSIS functions
	int status;
	do {
		while (__LDREXW(&p->lock) != 0) // Wait until
		{
			__NOP(); // !!!! strange, but unstable work without this line...
#if WITHDEBUG
			if (--v == 0) {
				PRINTF("Locked by %s(%d), wait at %s(%d)\n", p->file, p->line, file, line);
				for (;;)
					;
			}
#endif /* WITHDEBUG */
		}
		// Lock_Variable is free
		status = __STREXW(1, &p->lock); // Try to set
										// Lock_Variable
	} while (status != 0);				// retry until lock successfully
	__DMB();							// Do not start any other memory access
										// until memory barrier is completed
#if WITHDEBUG
	p->file = file;
	p->line = line;
#endif /* WITHDEBUG */
}

void spin_unlock(volatile spinlock_t *p)
{
	// Note: __LDREXW and __STREXW are CMSIS functions
	__DMB(); // Ensure memory operations completed before
	// releasing lock
	p->lock = 0;
	return;
}

#endif /* CPUSTYLE_ARM && WITHSMPSYSTEM */

// This processor index (0..n-1)
uint_fast8_t arm_hardware_cpuid(void)
{
	return __get_MPIDR() & 0x03;
}

static RAMDTCM spinlock_t gicpriority = {0};

#if WITHSMPSYSTEM

static USBALIGN_BEGIN uint8_t gicshadow_prio[1024] USBALIGN_END;

static void arm_hardware_gicsfetch(void)
{
	// Get ITLinesNumber
	const unsigned ITLinesNumber = ((GIC_DistributorInfo() & 0x1f) + 1) * 32;
	unsigned int_id;

	SPIN_LOCK(&gicpriority);
	for (int_id = 0; int_id < ITLinesNumber; ++int_id) {
		GIC_SetPriority(int_id, gicshadow_prio[int_id]); // non-atomic operation
	}
	SPIN_UNLOCK(&gicpriority);
	arm_hardware_invalidate((uintptr_t)gicshadow_prio, sizeof gicshadow_prio);
}

/* вызывается на основном процессоре */
static void arm_hardware_populate(int int_id)
{
	gicshadow_prio[int_id] = GIC_GetPriority(int_id);
	arm_hardware_flush((uintptr_t)gicshadow_prio, sizeof gicshadow_prio);

	GIC_SendSGI(BOARD_SGI_IRQ, 0x01uL << 1, 0x00); // CPU1, filer=0
}

/* вызывается на основном процессоре */
static void arm_hardware_populate_initialize(void)
{
	ASSERT(arm_hardware_cpuid() == 0);
	const unsigned ITLinesNumber = ((GIC_DistributorInfo() & 0x1f) + 1) * 32;
	unsigned int_id;
	for (int_id = 0; int_id < ITLinesNumber; ++int_id) {
		gicshadow_prio[int_id] = GIC_GetPriority(int_id);
	}
	arm_hardware_flush_invalidate((uintptr_t)gicshadow_prio, sizeof gicshadow_prio);

	arm_hardware_set_handler(BOARD_SGI_IRQ, arm_hardware_gicsfetch, BOARD_SGI_PRIO, 0x01u << 1);
}

/* вызывается на дополнительном процессоре */
void arm_hardware_populte_second_initialize(void)
{
	ASSERT(arm_hardware_cpuid() != 0);

	SPIN_LOCK(&gicpriority);
	GIC_SetPriority(BOARD_SGI_IRQ, BOARD_SGI_PRIO); // non-atomic operation
	SPIN_UNLOCK(&gicpriority);
	arm_hardware_invalidate((uintptr_t)gicshadow_prio, sizeof gicshadow_prio);
}

#endif /* WITHSMPSYSTEM */

// Set interrupt vector wrapper
void arm_hardware_set_handler(uint_fast16_t int_id,
							  void (*handler)(void),
							  uint_fast8_t priority,
							  uint_fast8_t targetcpu)
{
	ASSERT(arm_hardware_cpuid() == 0);
	// Cortex-A computers

	VERIFY(IRQ_Disable(int_id) == 0);

	VERIFY(IRQ_SetHandler(int_id, handler) == 0);
	SPIN_LOCK(&gicpriority);
	VERIFY(IRQ_SetPriority(int_id, priority) == 0); // non-atomic operation
	SPIN_UNLOCK(&gicpriority);
	GIC_SetTarget(int_id, targetcpu);

	// peripheral (hardware) interrupts using the GIC 1-N model.
	uint_fast32_t cfg = GIC_GetConfiguration(int_id);
	cfg &= ~0x02;					   /* Set level sensitive configuration */
	cfg |= 0x01;					   /* Set 1-N model - Only one processor handles this interrupt. */
	GIC_SetConfiguration(int_id, cfg); // non-atomic operation

#if WITHSMPSYSTEM
	arm_hardware_populate(int_id); // populate for other CPUs
#endif							   /* WITHSMPSYSTEM */

	VERIFY(IRQ_Enable(int_id) == 0);
}

// Disable interrupt vector
void arm_hardware_disable_handler(uint_fast16_t int_id)
{
	ASSERT(arm_hardware_cpuid() == 0);

	// Cortex-A computers
	VERIFY(IRQ_Disable(int_id) == 0);
#if WITHSMPSYSTEM
	arm_hardware_populate(int_id);
#endif /* WITHSMPSYSTEM */
}

// Set interrupt vector wrapper
void arm_hardware_set_handler_overrealtime(uint_fast16_t int_id, void (*handler)(void))
{
	arm_hardware_set_handler(int_id, handler, ARM_OVERREALTIME_PRIORITY, TARGETCPU_OVRT);
}

// Set interrupt vector wrapper
void arm_hardware_set_handler_realtime(uint_fast16_t int_id, void (*handler)(void))
{
	arm_hardware_set_handler(int_id, handler, ARM_REALTIME_PRIORITY, TARGETCPU_RT);
}

// Set interrupt vector wrapper
void arm_hardware_set_handler_system(uint_fast16_t int_id, void (*handler)(void))
{
	arm_hardware_set_handler(int_id, handler, ARM_SYSTEM_PRIORITY, TARGETCPU_SYSTEM);
}

void cpu_initialize(void)
{
	cpump_initialize();
	IRQ_Initialize(); // GIC_Enable() inside

#if WITHNESTEDINTERRUPTS
	gARM_OVERREALTIME_PRIORITY = ARM_CA9_ENCODE_PRIORITY(PRI_OVRT); // value for GIC_SetPriority
	gARM_REALTIME_PRIORITY = ARM_CA9_ENCODE_PRIORITY(PRI_RT);		// value for GIC_SetPriority
	gARM_SYSTEM_PRIORITY = ARM_CA9_ENCODE_PRIORITY(PRI_SYS);		// value for GIC_SetPriority

	gARM_BASEPRI_ONLY_REALTIME = ARM_CA9_ENCODE_PRIORITY(PRI_SYS);	  // value for GIC_SetInterfacePriorityMask
	gARM_BASEPRI_ONLY_OVERREALTIME = ARM_CA9_ENCODE_PRIORITY(PRI_RT); // value for GIC_SetInterfacePriorityMask
	gARM_BASEPRI_ALL_ENABLED = ARM_CA9_ENCODE_PRIORITY(PRI_USER);	  // value for GIC_SetInterfacePriorityMask

	GIC_SetInterfacePriorityMask(gARM_BASEPRI_ALL_ENABLED);
#endif /* WITHNESTEDINTERRUPTS */

#if WITHSMPSYSTEM
	arm_hardware_populate_initialize();
#endif /* WITHSMPSYSTEM */
}
