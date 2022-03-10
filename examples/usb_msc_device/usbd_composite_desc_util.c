#include "usbd_composite_desc_util.h"
#include "align64.h"
#include "usb200.h"
#include "usbd_composite_desc_values.h"
#include "usbd_def.h"
#include <string.h>

static uint_fast16_t encodeMaxPacketSize(uint_fast32_t size);
static unsigned fill_extprop_descriptor(uint8_t *buff, unsigned maxsize, const char *name, const char *value);
static unsigned fill_devcaps_usb20ext(uint_fast8_t fill, uint8_t *buff, unsigned maxsize);
static unsigned fill_devcaps_ContainerID(uint_fast8_t fill, uint8_t *buff, unsigned maxsize);
static unsigned fill_DevCaps_group(uint_fast8_t fill, uint8_t *p, unsigned maxsize, uint_fast8_t bNumDeviceCaps);
static unsigned fill_string_c2descriptor(uint_fast8_t fill, uint8_t *buff, unsigned maxsize, const char *s);
static unsigned fill_string_c4descriptor(uint_fast8_t fill, uint8_t *buff, unsigned maxsize, const char *s);
static unsigned fill_extprop_descriptor(uint8_t *buff, unsigned maxsize, const char *name, const char *value);

// Only for high speed capable devices
// Other Speed Configuration descriptor - pass highspeed=1
// For all devices
// Configuration descriptor - pass highspeed=0
unsigned fill_Configuration_descriptor(uint8_t *buff,
									   unsigned maxsize,
									   int highspeed,
									   const uint_fast8_t bConfigurationValue, // = 0x01;
									   const uint_fast8_t bNumInterfaces,	   // = INTERFACE_count;
									   const fill_func_t fill_main_group	   // fill functional descriptor(s)
)
{
	const int highspeedEPs = highspeed;
	unsigned length = 9;
	unsigned totalsize = length + fill_main_group(0, buff, maxsize - length, highspeedEPs);
	if (maxsize < length)
		return 0;
	if (buff != NULL) {
		*buff++ = length;					   /* bLength */
		*buff++ = USB_DESC_TYPE_CONFIGURATION; /* descriptor type */
		*buff++ = LOBYTE(totalsize);		   /* length of packed config descr. (16 bit) */
		*buff++ = HIBYTE(totalsize);		   /* length of packed config descr. (16 bit) */
		*buff++ = bNumInterfaces;			   /* bNumInterfaces  */
		*buff++ = bConfigurationValue; /* bConfigurationValue - Value to use as an argument to the SetConfiguration()
										  request to select this configuration */
		*buff++ = STRING_ID_0;		   /* iConfiguration - Index of string descriptor describing this configuration */
		*buff++ = 0xC0;				   /* bmAttributes  BUS Powred, self powered. See USBD_SELF_POWERED */
		*buff++ = 125;				   /* bMaxPower = 250 mA. */

		fill_main_group(1, buff, maxsize - length, highspeedEPs);
	}
	return totalsize;
}

// Device Descriptor
unsigned fill_Device_descriptor(uint8_t *buff, unsigned maxsize, uint_fast8_t bNumConfigurations)
{
	const unsigned length = 18;
	if (maxsize < length)
		return 0;
	if (buff != NULL) {
		*buff++ = length;						  /*  0:bLength */
		*buff++ = USB_DEVICE_DESCRIPTOR_TYPE;	  /*  1:bDescriptorType */
		*buff++ = LOBYTE(USB_FUNCTION_BCD_USB);	  /*  2:bcdUSB_FUNCTION_lo */
		*buff++ = HIBYTE(USB_FUNCTION_BCD_USB);	  /*  3:bcdUSB_FUNCTION_hi */
		*buff++ = USB_DEVICE_CLASS_MISCELLANEOUS; /*  4:bDeviceClass */
		*buff++ = 2;							  /*  5:bDeviceSubClass - Common Class Sub Class */
		*buff++ = 1;							  /*  6:bDeviceProtocol - Interface Association Descriptor protocol */
		*buff++ = USB_OTG_MAX_EP0_SIZE; /*  7:bMaxPacketSize0 (for DCP), For 3x: 09H is the only valid value in this
								 field when operating at Gen X speed. */
		*buff++ = LOBYTE(USB_FUNCTION_VENDOR_ID);  /*  8:idVendor_lo */
		*buff++ = HIBYTE(USB_FUNCTION_VENDOR_ID);  /*  9:idVendor_hi */
		*buff++ = LOBYTE(USB_FUNCTION_PRODUCT_ID); /* 10:idProduct_lo */
		*buff++ = HIBYTE(USB_FUNCTION_PRODUCT_ID); /* 11:idProduct_hi */
		*buff++ = LOBYTE(USB_FUNCTION_RELEASE_NO); /* 12:bcdDevice_lo */
		*buff++ = HIBYTE(USB_FUNCTION_RELEASE_NO); /* 13:bcdDevice_hi */
		*buff++ = STRING_ID_1;					   /* 14:iManufacturer */
		*buff++ = STRING_ID_2;					   /* 15:iProduct */
		*buff++ = STRING_ID_3;					   /* 16:iSerialNumber */
		*buff++ = bNumConfigurations;			   /* 17:bNumConfigurations */
	}
	return length;
}

// Only for high speed capable devices
// Device Qualifier Descriptor
unsigned fill_DeviceQualifier_descriptor(uint8_t *buff, unsigned maxsize, uint_fast8_t bNumOtherSpeedConfigurations)
{
	const unsigned length = 10;
	if (maxsize < length)
		return 0;
	if (buff != NULL) {
		// Вызов для заполнения, а не только для проверки занимаемого места в буфере
		*buff++ = length;								/*  0:bLength */
		*buff++ = USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE; /*  1:bDescriptorType */
		*buff++ = LOBYTE(USB_FUNCTION_BCD_USB);			/*  2:bcdUSB_FUNCTION_lo */
		*buff++ = HIBYTE(USB_FUNCTION_BCD_USB);			/*  3:bcdUSB_FUNCTION_hi */
		*buff++ = USB_DEVICE_CLASS_MISCELLANEOUS;		/*  4:bDeviceClass - Miscellaneous */
		*buff++ = 2;									/*  5:bDeviceSubClass - Common Class Sub Class */
		*buff++ = 1;					/*  6:bDeviceProtocol - Interface Association Descriptor protocol */
		*buff++ = USB_OTG_MAX_EP0_SIZE; /*  7:bMaxPacketSize0 (for DCP), For 3x: 09H is the only valid value in this
										   field when operating at Gen X speed. */
		*buff++ = bNumOtherSpeedConfigurations; /*  8:bNumConfigurations - number of other-speed configurations */
		*buff++ = 0;							/*  9:bReserved */
	}
	return length;
}

// Device Capability Descriptor - USB 2.0 Extension
static unsigned fill_devcaps_usb20ext(uint_fast8_t fill, uint8_t *buff, unsigned maxsize)
{
	const uint_fast8_t length = 7;
	if (maxsize < length)
		return 0;
	if (fill != 0 && buff != NULL) {
		const uint_fast32_t bmAttributes = USBD_LPM_ENABLED ? 1 << 1 : 0;
		*buff++ = length;							   /* bLength */
		*buff++ = USB_DEVICE_CAPABITY_DESCRIPTOR_TYPE; /* bDescriptorType: Device Capability */
		*buff++ = 0x02;								   /* bDevCapabilityType: 0x02: USB 2.0 Extension */
		*buff++ = LOBYTE(bmAttributes);
		*buff++ = HIBYTE(bmAttributes);
		*buff++ = LOBYTE(bmAttributes >> 16UL);
		*buff++ = HIBYTE(bmAttributes >> 16UL);
	}
	return length;
}

// Device Capability Descriptor - Container ID
static unsigned fill_devcaps_ContainerID(uint_fast8_t fill, uint8_t *buff, unsigned maxsize)
{
	typedef struct _GUID {
		uint32_t Data1;
		uint16_t Data2;
		uint16_t Data3;
		uint8_t Data4[8];
	} GUID;

	const uint_fast8_t length = 20;
	if (maxsize < length)
		return 0;
	if (fill != 0 && buff != NULL) {
		static const GUID ContainerID = {0x1d614fe7, 0x58c2, 0x42d8, {0x94, 0x2e, 0xcc, 0x2a, 0x69, 0x62, 0x18, 0xdb}};
		*buff++ = length;							   /* bLength */
		*buff++ = USB_DEVICE_CAPABITY_DESCRIPTOR_TYPE; /* bDescriptorType: Device Capability */
		*buff++ = 0x04;								   /* bDevCapabilityType: 0x04: Container ID */
		*buff++ = 0x00;								   /* bReserved */
		memcpy(buff, &ContainerID, 16);				   /* ContainerID */
	}
	return length;
}

static unsigned fill_DevCaps_group(uint_fast8_t fill, uint8_t *p, unsigned maxsize, uint_fast8_t bNumDeviceCaps)
{
	unsigned n = 0;
	n += fill_devcaps_usb20ext(fill, p + n, maxsize - n);
	n += fill_devcaps_ContainerID(fill, p + n, maxsize - n);
	return n;
}

// Binary Device Object Store (BOS) Descriptor
unsigned fill_BinaryDeviceObjectStore_descriptor(uint8_t *buff, unsigned maxsize)
{
	const uint_fast8_t bNumDeviceCaps = 2;
	unsigned length = 5;
	unsigned totalsize = length + fill_DevCaps_group(0, buff, maxsize - length, bNumDeviceCaps);
	if (maxsize < length)
		return 0;
	if (buff != NULL) {
		*buff++ = length;			 /*  0:bLength */
		*buff++ = USB_BOS_TYPE;		 /*  1:bDescriptorType */
		*buff++ = LOBYTE(totalsize); /* wTotalLength length of packed config descr. (16 bit) */
		*buff++ = HIBYTE(totalsize); /* wTotalLength length of packed config descr. (16 bit) */
		*buff++ = bNumDeviceCaps;	 /*  4:bNumDeviceCaps */
		fill_DevCaps_group(1, buff, maxsize - length, bNumDeviceCaps);
	}
	return totalsize;
}

unsigned fill_align4(uint8_t *buff)
{
	const uintptr_t granulation = DCACHEROWSIZE;
	return (granulation - ((uintptr_t)buff & (granulation - 1))) & (granulation - 1);
}

unsigned fill_langid_descriptor(uint8_t *buff, unsigned maxsize, uint_fast16_t langid)
{
	const unsigned length = 4;
	if (maxsize < length)
		return 0;
	if (buff != NULL) {
		*buff++ = length;					  /* bLength */
		*buff++ = USB_STRING_DESCRIPTOR_TYPE; /* descriptor type */
		*buff++ = LOBYTE(langid);
		*buff++ = HIBYTE(langid);
	}
	return length;
}

unsigned fill_string_descriptor(uint8_t *buff, unsigned maxsize, const char *s)
{
	const unsigned length = 2 + 2 * strlen(s);
	if (maxsize < length || length >= 256)
		return 0;
	if (buff != NULL) {
		*buff++ = length;					  /* bLength */
		*buff++ = USB_STRING_DESCRIPTOR_TYPE; /* descriptor type */
		for (;;) {
			const int c = (unsigned char)*s++;
			if (c == '\0')
				break;
			const wchar_t wc = c;
			*buff++ = LOBYTE(wc);
			*buff++ = HIBYTE(wc);
		}
	}
	return length;
}

unsigned
fill_pattern_descriptor(uint_fast8_t fill, uint8_t *buff, unsigned maxsize, const void *pattern, unsigned length)
{
	if (maxsize < length)
		return 0;
	if (fill && buff != NULL) {
		memcpy(buff, pattern, length);
	}
	return length;
}

static unsigned fill_string_c2descriptor(uint_fast8_t fill, uint8_t *buff, unsigned maxsize, const char *s)
{
	size_t n = strlen(s) + 1; // nul include
	unsigned length = 2 + n * 2;
	if (maxsize < length)
		return 0;
	if (fill && buff != NULL) {
		*buff++ = LOBYTE(n * 2); /* dwLength */
		*buff++ = HIBYTE(n * 2);
		while (n--) {
			const int c = *s++;
			*buff++ = LOBYTE(c); /* character */
			*buff++ = HIBYTE(c);
		}
	}
	return length;
}

static unsigned fill_string_c4descriptor(uint_fast8_t fill, uint8_t *buff, unsigned maxsize, const char *s)
{
	size_t n = strlen(s) + 1; // nul include
	unsigned length = 4 + n * 2;
	if (maxsize < length)
		return 0;
	if (fill && buff != NULL) {
		*buff++ = LO_BYTE(n * 2); /* dwLength */
		*buff++ = HI_BYTE(n * 2);
		*buff++ = HI_24BY((n * 2) >> 16);
		*buff++ = HI_32BY((n * 2) >> 16);
		while (n--) {
			const int c = *s++;
			*buff++ = LO_BYTE(c); /* character */
			*buff++ = HI_BYTE(c);
		}
	}
	return length;
}

// Add value to HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB\VID_F120&PID_0754&MI_01\9&13a6429d&0&0001\Device
// Parameters
static unsigned fill_extprop_descriptor(uint8_t *buff, unsigned maxsize, const char *name, const char *value)
{
	unsigned lengthprops = 4 +												   // dwSize
						   4 +												   // dwPropertyDataType
						   fill_string_c2descriptor(0, buff, maxsize, name) +  // wPropertyNameLength, bPropertyName
						   fill_string_c4descriptor(0, buff, maxsize, value) + // dwPropertyDataLength, bPropertyData
						   0;
	unsigned lengthheader = 4 + // dwLength
							2 + // bcdVersion
							2 + // wIndex
							2 + // wCount
							0;
	unsigned length = lengthheader + lengthprops;
	if (maxsize < length)
		return 0;
	if (buff != NULL) {
		const uint_fast16_t bcdVersion = 0x0100;
		const uint_fast16_t wIndex = 0x05;
		const uint_fast16_t wCount = 1;
		uint_fast16_t n;
		*buff++ = LO_BYTE(length); /* dwLength */
		*buff++ = HI_BYTE(length);
		*buff++ = HI_24BY(length);
		*buff++ = HI_32BY(length);
		*buff++ = LO_BYTE(bcdVersion); /* bcdVersion */
		*buff++ = HI_BYTE(bcdVersion);
		*buff++ = LO_BYTE(wIndex); /* wIndex */
		*buff++ = HI_BYTE(wIndex);
		*buff++ = LO_BYTE(wCount); /* wCount */
		*buff++ = HI_BYTE(wCount);
		n = wCount;
		while (n--) {
			// The DEVICE_LABEL custom property section contains the device label.
			const uint_fast16_t dwPropertyDataType = 0x01; // 0x00000001 (Unicode string)
			*buff++ = LO_BYTE(lengthprops);				   /* dwSize */
			*buff++ = HI_BYTE(lengthprops);
			*buff++ = HI_24BY(lengthprops);
			*buff++ = HI_32BY(lengthprops);
			*buff++ = LO_BYTE(dwPropertyDataType); /* dwPropertyDataType */
			*buff++ = HI_BYTE(dwPropertyDataType);
			*buff++ = HI_24BY(dwPropertyDataType);
			*buff++ = HI_32BY(dwPropertyDataType);
			buff += fill_string_c2descriptor(1, buff, maxsize, name);
			buff += fill_string_c4descriptor(1, buff, maxsize, value);
		}
	}
	return length;
}

static uint_fast16_t encodeMaxPacketSize(uint_fast32_t size)
{
	// usb_20.pdf:
	// 5.9 High-Speed, High Bandwidth Endpoints
	// 9.6.6 Endpoint
	// Table 9-14. Allowed wMaxPacketSize Values for Different Numbers of Transactions per Microframe
	// For all endpoints, bits 10..0 specify the maximum
	// packet size (in bytes).
	// A high-speed endpoint can move up to 3072 bytes per microframe
	// For high-speed isochronous and interrupt endpoints:
	// Bits 12..11 specify the number of additional transaction
	// opportunities per microframe:
	// 00 = None (1 transaction per microframe)
	// 01 = 1 additional (2 per microframe)
	// 10 = 2 additional (3 per microframe)

	if (size <= 1024)
		return size; // 1..1024
	if (size <= 2048)
		return (0x01 << 11) | ((size + 1) / 2); // 513..1024
	else
		return (0x02 << 11) | ((size + 2) / 3); // 683..1024
}
