/* $Id$ */
/*
 *   usbch9.h
 *
 *   Def's for USB device communications stack - "Standard Request" layer,
 *   conforming to USB 2.0 specification, Chapter 9.
 *
 *   Adapted from Philips PDIUSBD12 firmware library (CHAP_9.H)
 *   MJB Nov.2005
 */
#pragma once

#define UNICODE_ENGLISH (0x0409) /* US_English (Ref: USB_LANGIDs.pdf) */

enum {
	ep0inxxx = 0x80,
	USBD_EP_MSC_IN,
	epincount,
};

// OUT Endpoints allocation
enum {
	ep0outxxx = 0x00,
	USBD_EP_MSC_OUT,
	epoutcount,
};

enum interfaces_tag {
	INTERFACE_MSCDEVICE_CONTROL, // MSC Device
	INTERFACE_count
};

#define INTERFACE_MSCDEVICE_count 1
