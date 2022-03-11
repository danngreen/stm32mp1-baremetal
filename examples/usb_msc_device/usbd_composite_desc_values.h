#pragma once
#include <stdint.h>

#define DCACHEROWSIZE 64U

#define USB_FUNCTION_VENDOR_ID 0x0483 // STM
#define USB_FUNCTION_PRODUCT_ID 0x079b
#define USB_FUNCTION_RELEASE_NO 0x0001

#define USB_FUNCTION_BCD_USB 0x0200 // USB standard supported == 2.0

enum {
	STRING_ID_0 = 0, /* Language ID */
	STRING_ID_1,	 /* Manufacturer */
	STRING_ID_2,	 /* Product */
	STRING_ID_3,	 /* SerialNumber */
	STRING_ID_MSC_IF,
	STRING_ID_count
};

struct stringtempl {
	uint_fast8_t id;
	const char *str;
};

static const struct stringtempl strtemplates[] = {
	{STRING_ID_1, "MicroGenSF"}, // Manufacturer
	{STRING_ID_2, "Storch"},	 // Product
	{STRING_ID_3, "12345"},		 // SN
	{STRING_ID_MSC_IF, "USB Mass StoragE"},
};
