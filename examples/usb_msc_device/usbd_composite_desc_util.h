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
