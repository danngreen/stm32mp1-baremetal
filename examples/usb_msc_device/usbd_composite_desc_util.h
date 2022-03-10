#pragma once
#include <stdint.h>

typedef unsigned (*fill_func_t)(uint_fast8_t fill, uint8_t *p, unsigned maxsize, int highspeed);

unsigned fill_Configuration_descriptor(uint8_t *buff,
									   unsigned maxsize,
									   int highspeed,
									   const uint_fast8_t bConfigurationValue,
									   const uint_fast8_t bNumInterfaces,
									   const fill_func_t fill_main_group);

unsigned fill_Device_descriptor(uint8_t *buff, unsigned maxsize, uint_fast8_t bNumConfigurations);
unsigned fill_align4(uint8_t *buff);
unsigned
fill_pattern_descriptor(uint_fast8_t fill, uint8_t *buff, unsigned maxsize, const void *pattern, unsigned length);
unsigned fill_DeviceQualifier_descriptor(uint8_t *buff, unsigned maxsize, uint_fast8_t bNumOtherSpeedConfigurations);
unsigned fill_BinaryDeviceObjectStore_descriptor(uint8_t *buff, unsigned maxsize);
unsigned fill_langid_descriptor(uint8_t *buff, unsigned maxsize, uint_fast16_t langid);
unsigned fill_string_descriptor(uint8_t *buff, unsigned maxsize, const char *s);

// Put elsewhere:
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#define ASSERT(x)
#define HI_32BY(w) (((w) >> 24) & 0xFF) /* Extract 31..24 bits from unsigned word */
#define HI_24BY(w) (((w) >> 16) & 0xFF) /* Extract 23..16 bits from unsigned word */
#define HI_BYTE(w) (((w) >> 8) & 0xFF)	/* Extract high-order byte from unsigned word */
#define LO_BYTE(w) ((w)&0xFF)			/* Extract low-order byte from unsigned word */
