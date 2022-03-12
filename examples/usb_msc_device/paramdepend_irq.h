#include <stdint.h>

#define ICPIDR0 (*(const volatile uint32_t *)(GIC_DISTRIBUTOR_BASE + 0xFE0))
#define ICPIDR1 (*(const volatile uint32_t *)(GIC_DISTRIBUTOR_BASE + 0xFE4))
#define ICPIDR2 (*(const volatile uint32_t *)(GIC_DISTRIBUTOR_BASE + 0xFE8))

// The processor does not process any exception with a priority value greater than or equal to BASEPRI.
extern uint32_t gARM_OVERREALTIME_PRIORITY;
extern uint32_t gARM_REALTIME_PRIORITY;
extern uint32_t gARM_SYSTEM_PRIORITY;
extern uint32_t gARM_BASEPRI_ONLY_REALTIME;
extern uint32_t gARM_BASEPRI_ONLY_OVERREALTIME;
extern uint32_t gARM_BASEPRI_ALL_ENABLED;

#define ARM_CA9_ENCODE_PRIORITY(v) ((v) << (GIC_GetBinaryPoint() + 1))
/*
	GICC_PMR == INTC.ICCPMR

	Provides an interrupt priority filter.
	Only interrupts with higher priority than the value in this
	register are signaled to the processor.

*/
enum {
	PRI_IPC, /* Приоритет SGI прерывания для синхронизации приоритетов GIC на остальных процессорах */
	PRI_OVRT,
	PRI_RT = 1,
	PRI_SYS = 2,
	PRI_USER = 3,
};

#define BOARD_SGI_IRQ SGI1_IRQn /* Прерываниедля синхронизации приоритетов GIC на остальных процессорах  */
#define BOARD_SGI_PRIO ARM_CA9_ENCODE_PRIORITY(PRI_IPC)

#define RUNNING_PRI                                                                                                    \
	((GICInterface->RPR & 0xFF) >> ARM_CA9_PRIORITYSHIFT) // The current running priority on the CPU interface.

// A lower priority value indicating a higher priority
#define ARM_OVERREALTIME_PRIORITY ((const uint32_t)gARM_OVERREALTIME_PRIORITY)
#define ARM_REALTIME_PRIORITY ((const uint32_t)gARM_REALTIME_PRIORITY)
#define ARM_SYSTEM_PRIORITY ((const uint32_t)gARM_SYSTEM_PRIORITY)

// разрешены все
#define system_enableIRQ()                                                                                             \
	do {                                                                                                               \
		/*ASSERT(RUNNING_PRI == 0x1F); */                                                                              \
		GIC_SetInterfacePriorityMask(gARM_BASEPRI_ALL_ENABLED);                                                        \
	} while (0)
// разрешены только realtime
#define system_disableIRQ()                                                                                            \
	do {                                                                                                               \
		/*ASSERT(RUNNING_PRI == 0x1F); */                                                                              \
		GIC_SetInterfacePriorityMask(gARM_BASEPRI_ONLY_REALTIME);                                                      \
	} while (0)
// разрешены все
#define system_enableIRQxxx()                                                                                          \
	do {                                                                                                               \
		__enable_irq();                                                                                                \
	} while (0)
// разрешены только realtime
#define system_disableIRQxxx()                                                                                         \
	do {                                                                                                               \
		__disable_irq();                                                                                               \
	} while (0)

#define global_enableIRQ()                                                                                             \
	do {                                                                                                               \
		__enable_irq();                                                                                                \
	} while (0)
#define global_disableIRQ()                                                                                            \
	do {                                                                                                               \
		__disable_irq();                                                                                               \
	} while (0)
