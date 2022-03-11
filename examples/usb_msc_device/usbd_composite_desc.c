#include "align64.h"
#include "usb200.h"
#include "usbch9.h"
#include "usbd_composite_desc_util.h"
#include "usbd_composite_desc_values.h"
#include "usbd_ctlreq.h"
#include "usbd_def.h"
#include "usbd_desc.h"
#include "usbd_ioreq.h"
#include <string.h>

#define MSC_MAX_HS_PACKET 0x200U

static uint8_t MSC_ConfigDesc[] = {
	/********************  Mass Storage interface ********************/
	0x09,			  /* bLength: Interface Descriptor size */
	0x04,			  /* bDescriptorType: */
	0x00,			  /* bInterfaceNumber: Number of Interface */
	0x00,			  /* bAlternateSetting: Alternate setting */
	0x02,			  /* bNumEndpoints */
	0x08,			  /* bInterfaceClass: MSC Class */
	0x06,			  /* bInterfaceSubClass : SCSI transparent */
	0x50,			  /* nInterfaceProtocol */
	STRING_ID_MSC_IF, /* iInterface: */
	/********************  Mass Storage Endpoints ********************/
	0x07,			/* Endpoint descriptor length = 7 */
	0x05,			/* Endpoint descriptor type */
	USBD_EP_MSC_IN, /* Endpoint address (IN) */
	0x02,			/* Bulk endpoint type */
	LO_BYTE(MSC_MAX_HS_PACKET),
	HI_BYTE(MSC_MAX_HS_PACKET),
	0x00,			 /* Polling interval in milliseconds */
	0x07,			 /* Endpoint descriptor length = 7 */
	0x05,			 /* Endpoint descriptor type */
	USBD_EP_MSC_OUT, /* Endpoint address (OUT) */
	0x02,			 /* Bulk endpoint type */
	LO_BYTE(MSC_MAX_HS_PACKET),
	HI_BYTE(MSC_MAX_HS_PACKET),
	0x00 /* Polling interval in milliseconds */
};

static unsigned fill_MSC_function(uint_fast8_t fill, uint8_t *p, unsigned maxsize, int highspeed)
{
	const unsigned sz = sizeof(MSC_ConfigDesc) / sizeof(MSC_ConfigDesc[0]);

	if (maxsize < sz)
		return 0; // not enough room in the buffer

	if (fill != 0 && p != NULL) {
		for (unsigned i = 0; i < sz; i++) {
			*p++ = MSC_ConfigDesc[i];
		}
	}
	return sz;
}

static unsigned fill_Configuration_compound(uint_fast8_t fill, uint8_t *p, unsigned maxsize, int highspeed)
{
	unsigned n = 0;
	n += fill_MSC_function(fill, p + n, maxsize - n, highspeed);
	return n;
}

static __ALIGN4k_BEGIN uint8_t alldescbuffer[256 * 13] __ALIGN4k_END;

struct descholder MsftStringDescr[1];
struct descholder MsftCompFeatureDescr[1]; // Microsoft Compatible ID Feature Descriptor
struct descholder StringDescrTbl[STRING_ID_count];
struct descholder ConfigDescrTbl[USBD_CONFIGCOUNT];
struct descholder DeviceDescrTbl[USBD_CONFIGCOUNT];
struct descholder OtherSpeedConfigurationTbl[USBD_CONFIGCOUNT];
struct descholder DeviceQualifierTbl[USBD_CONFIGCOUNT];
struct descholder BinaryDeviceObjectStoreTbl[1];
struct descholder HIDReportDescrTbl[1];
struct descholder ExtOsPropDescTbl[32];

uint_fast8_t usbd_get_stringsdesc_count(void)
{
	return ARRAY_SIZE(StringDescrTbl);
}

void usbd_descriptors_initialize(uint_fast8_t is_HS)
{
	unsigned score = 0;
	unsigned offset;

	const uint_fast8_t bNumConfigurations = 1;
	const uint_fast8_t bNumOtherSpeedConfigurations = 0;
	{
		// Device Descriptor
		unsigned partlen;
		score += fill_align4(alldescbuffer + score);
		partlen = fill_Device_descriptor(alldescbuffer + score, ARRAY_SIZE(alldescbuffer) - score, bNumConfigurations);
		DeviceDescrTbl[0].size = partlen;
		DeviceDescrTbl[0].data = alldescbuffer + score;
		score += partlen;
	}

	{
		// Configuration Descriptors list
		unsigned partlen;
		uint_fast8_t index;
		for (index = 0; index < bNumConfigurations; ++index) {
			// Configuration Descriptor
			score += fill_align4(alldescbuffer + score);
			partlen = fill_Configuration_descriptor(alldescbuffer + score,
													ARRAY_SIZE(alldescbuffer) - score,
													is_HS,
													bNumConfigurations,
													INTERFACE_count,
													fill_Configuration_compound);
			ConfigDescrTbl[index].size = partlen;
			ConfigDescrTbl[index].data = alldescbuffer + score;
			score += partlen;
		}
	}

	{
		// Microsoft OS String Descriptor
		// https://github.com/pbatard/libwdi/wiki/WCID-Devices#Example
		// HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\usbflags
		// HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB
		// See OS_Desc_Intro.doc, Table 3 describes the OS string descriptor’s fields.
		static const uint8_t MsftStringDescrProto[18] = {
			0x12, // Descriptor length (18 bytes)
			0x03, // Descriptor type (3 = String)
			'M',
			0, // Signature: "MSFT100"
			'S',
			0,
			'F',
			0,
			'T',
			0,
			'1',
			0,
			'0',
			0,
			'0',
			0,
			USBD_WCID_VENDOR_CODE, // Vendor Code - for vendor request
			0x00,				   // padding
		};
		unsigned partlen;

		score += fill_align4(alldescbuffer + score);
		partlen = fill_pattern_descriptor(1,
										  alldescbuffer + score,
										  ARRAY_SIZE(alldescbuffer) - score,
										  MsftStringDescrProto,
										  sizeof MsftStringDescrProto);
		MsftStringDescr[0].size = partlen;
		MsftStringDescr[0].data = alldescbuffer + score;
		score += partlen;
	}

	if (is_HS) {
		unsigned partlen;
		// Device Qualifier
		score += fill_align4(alldescbuffer + score);
		partlen = fill_DeviceQualifier_descriptor(
			alldescbuffer + score, ARRAY_SIZE(alldescbuffer) - score, bNumOtherSpeedConfigurations);
		DeviceQualifierTbl[0].size = partlen;
		DeviceQualifierTbl[0].data = alldescbuffer + score;
		score += partlen;
	} else {
		// Device Qualifier
		DeviceQualifierTbl[0].size = 0;
		DeviceQualifierTbl[0].data = NULL;

		// Other Speed Configuration
		OtherSpeedConfigurationTbl[0].size = 0;
		OtherSpeedConfigurationTbl[0].data = NULL;
	}

	// Binary Device Object Store (BOS) Descriptor
	// The value of the bcdUSB field in the standard USB 2.0 Device Descriptor
	// is used to indicate that the device supports the request to
	// read the BOS Descriptor (i.e. GetDescriptor(BOS)).
	// Devices that support the BOS descriptor must have a bcdUSB value of 0201H or larger.
	if (USB_FUNCTION_BCD_USB >= 0x0201) {
		unsigned partlen;
		score += fill_align4(alldescbuffer + score);
		partlen = fill_BinaryDeviceObjectStore_descriptor(alldescbuffer + score, ARRAY_SIZE(alldescbuffer) - score);
		BinaryDeviceObjectStoreTbl[0].size = partlen;
		BinaryDeviceObjectStoreTbl[0].data = alldescbuffer + score;
		score += partlen;
	} else {
		BinaryDeviceObjectStoreTbl[0].size = 0;
		BinaryDeviceObjectStoreTbl[0].data = NULL;
	}

	// String descriptors
	{
		// Language ID (mandatory)
		unsigned partlen;
		score += fill_align4(alldescbuffer + score);
		partlen = fill_langid_descriptor(alldescbuffer + score, ARRAY_SIZE(alldescbuffer) - score, UNICODE_ENGLISH);
		StringDescrTbl[STRING_ID_0].size = partlen;
		StringDescrTbl[STRING_ID_0].data = alldescbuffer + score;
		score += partlen;

		// All string IDs, except serial number
		unsigned i;
		for (i = 0; i < ARRAY_SIZE(strtemplates); ++i) {
			const uint_fast8_t id = strtemplates[i].id;
			ASSERT(id < ARRAY_SIZE(StringDescrTbl));

			score += fill_align4(alldescbuffer + score);
			partlen =
				fill_string_descriptor(alldescbuffer + score, ARRAY_SIZE(alldescbuffer) - score, strtemplates[i].str);
			StringDescrTbl[id].size = partlen;
			StringDescrTbl[id].data = alldescbuffer + score;
			score += partlen;
		}
	}

	{
		unsigned partlen;
		const uint_fast8_t id = STRING_ID_3;
		char b[64] = "SN:123456";
		score += fill_align4(alldescbuffer + score);
		partlen = fill_string_descriptor(alldescbuffer + score, ARRAY_SIZE(alldescbuffer) - score, b);
		StringDescrTbl[id].size = partlen;
		StringDescrTbl[id].data = alldescbuffer + score;
		score += partlen;
	}

	// arm_hardware_flush_invalidate((uintptr_t)alldescbuffer, sizeof alldescbuffer);
}
