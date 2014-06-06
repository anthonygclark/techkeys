/*******************************************************************************
 * File Name	: u_des_kb.c
 * Project	: Keyboard
 * Date		: 2013/11/06
 * Version	: 1.0
 * Target MCU   : AVR
 * Tool Chain	: Atmel AVR Studio 4.19 730 / gcc 4.7.0 / avr-libc 1.8.0
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * References	: USB Specification 2.0, chap. 9 USB Device Framework, pg. 239
 *		  Device Class Defininition for Human Interface Devices 1.11
 *		  HID Usage Tables 1.12
 * Description	: Keyboard firmware descriptors
 * Device	:
 * Release Notes:
 *
 * $Id$
 *
 * $Log$
 ******************************************************************************/

#include "includes.h"
#include <usb_cfg.h>
#include "u_des_kb.h"
#include "u_hid_kb.h"

//------------------------------------------------------------------------------

const uint8_t
    usbHidRepDescKbd[USB_HIDREP_DESC_KBD_SZ] PROGMEM =
    {
	0x05, 0x01,		// Usage Page (Generic Desktop),
	0x09, 0x06,		// Usage (Keyboard),
	0xA1, 0x01,		// Collection (Application),
	0x75, 0x01,		//   Report Size (1),
	0x95, 0x08,		//   Report Count (8),
	0x05, 0x07,		//   Usage Page (Key Codes),
	0x19, 0xE0,		//   Usage Minimum (224),
	0x29, 0xE7,		//   Usage Maximum (231),
	0x15, 0x00,		//   Logical Minimum (0),
	0x25, 0x01,		//   Logical Maximum (1),
	0x81, 0x02,		//   Input (Data, Variable, Absolute) Modifier byte
	0x95, 0x01,		//   Report Count (1),
	0x75, 0x08,		//   Report Size (8),
	0x81, 0x03,		//   Input (Constant),		Reserved byte
	0x95, 0x05,		//   Report Count (5),
	0x75, 0x01,		//   Report Size (1),
	0x05, 0x08,		//   Usage Page (LEDs),
	0x19, 0x01,		//   Usage Minimum (1),
	0x29, 0x05,		//   Usage Maximum (5),
	0x91, 0x02,		//   Output (Data, Variable, Absolute) LED report
	0x95, 0x03,		//   Report Count (3),
//	0x75, 0x01,		//   Report Size (1),
	0x91, 0x03,		//   Output (Constant),		LED report padding
	0x95, USB_REPORT_SZ_KBD - 2, //   Report Count (),
	0x75, 0x08,		//   Report Size (8),
	0x15, 0x00,		//   Logical Minimum (0),
	0x25, 0xDD,		//   Logical Maximum(221),
	0x05, 0x07,		//   Usage Page (Key Codes),
	0x19, 0x00,		//   Usage Minimum (0),
	0x29, 0xDD,		//   Usage Maximum (221),
	0x81, 0x00,		//   Input (Data, Array),
	0xC0			// End Collection
    } ;

#if ENABLE_CTRL_KEYS

const uint8_t
    usbHidRepDescCtrl[USB_HIDREP_DESC_CTRL_SZ] PROGMEM =
    {
				// System control
	0x05, 0x01,		// USAGE_PAGE (Generic Desktop)
	0x09, 0x80,		// USAGE (System Control)
	0xA1, 0x01,		// COLLECTION (Application)
	0x85, 1,		//   REPORT_ID (1)
	0x15, 0x81,		//   LOGICAL_MINIMUM (0x81)
	0x25, 0xB7,		//   LOGICAL_MAXIMUM (0xB7)
	0x19, 0x81,		//   USAGE_MINIMUM (0x81)
	0x29, 0xB7,		//   USAGE_MAXIMUM (0xB7)
	0x75, 0x10,		//   REPORT_SIZE (16)
	0x95, 0x01,		//   REPORT_COUNT (1)
	0x81, 0x00,		//   INPUT (Data,Array,Abs)
	0xC0,			// END_COLLECTION		23

				// Consumer
	0x05, 0x0C,		// USAGE_PAGE (Consumer Devices)
	0x09, 0x01,		// USAGE (Consumer Control)
	0xA1, 0x01,		// COLLECTION (Application)
	0x85, 2,		//   REPORT_ID (2)
	0x15, 0x01,		//   LOGICAL_MINIMUM (0x01)
	0x26, 0x9C, 0x02,	//   LOGICAL_MAXIMUM (0x29C)
	0x19, 0x01,		//   USAGE_MINIMUM (0x01)
	0x2A, 0x9C, 0x02,	//   USAGE_MAXIMUM (0x29C)
	0x75, 0x10,		//   REPORT_SIZE (16)
	0x95, 0x01,		//   REPORT_COUNT (1)
	0x81, 0x00,		//   INPUT (Data,Array,Abs)	24

	0x75, 0x01,		//   Report Size (1),
	0x95, 0x02,		//   Report Count (2),
	0x15, 0x00,		//   Logical Minimum (0),
	0x25, 0x01,		//   Logical Maximum (1),
	0x05, 0x08,		//   Usage Page (LEDs),
	0x09, 0x09,		//   Usage (9),			Mute
	0x09, 0x28,		//   Usage (0x28),		Camera On
	0x91, 0x02,		//   Output (Data, Variable, Absolute) LED report
	0x75, 0x01,		//   Report Size (1),
	0x95, 0x06,		//   Report Count (6),
	0x91, 0x03,		//   Output (Constant),		LED report padding

	0xC0			// END_COLLECTION		23
    } ;
#endif

#if ENABLE_DBG_OUT

const uint8_t
    usbHidRepDescDbg[USB_HIDREP_DESC_DBG_SZ] PROGMEM =
    {
	0x06, 0x31, 0xFF,	// Usage Page 0xFF31 (vendor defined)
	0x09, 0x74,		// Usage 0x74
	0xA1, 0x53,		// Collection 0x53
	0x75, 0x08,		// report size = 8 bits
	0x15, 0x00,		// logical minimum = 0
	0x26, 0xFF, 0x00,	// logical maximum = 255
	0x95, USB_REPORT_SZ_DBG,// report count
	0x09, 0x75,		// usage
	0x81, 0x02,		// Input (array)
	0xC0			// end collection
    } ;
#endif

//------------------------------------------------------------------------------

const usb_string_desc_t
    usbStringDescLan PROGMEM = { USB_STRING_DESC_LAN_SZ, USB_STRING_DESC_TYPE, { USB_STRING_LAN } },
    usbStringDescMan PROGMEM = { USB_STRING_DESC_MAN_SZ, USB_STRING_DESC_TYPE, { USB_STRING_MAN } },
    usbStringDescPro PROGMEM = { USB_STRING_DESC_PRO_SZ, USB_STRING_DESC_TYPE, { USB_STRING_PRO } },
    usbStringDescSer PROGMEM = { USB_STRING_DESC_SER_SZ, USB_STRING_DESC_TYPE, { USB_STRING_SER } } ;

//------------------------------------------------------------------------------

const usb_device_desc_t
    usbDeviceDesc PROGMEM =
    {
	USB_DEVICE_DESC_SZ,		// Size of this descriptor in bytes (18)
	USB_DEVICE_DESC_TYPE,		// Device (0x01)
	USB_DEVICE_USBVER,		// USB Specification which device complies too
	USB_DEVICE_CLASS,		// Class code. 0 if interf. defines class
	USB_DEVICE_SUBCLASS,		// Subclass code
	USB_DEVICE_PROTOCOL,		// Protocol code
	USB_DEVICE_MAXPKTSZ,		// Max. packet size for EP0. 8, 16,32, or 64
	USB_DEVICE_MAN_ID,		// Vendor ID
	USB_DEVICE_PRO_ID,		// Product ID
	USB_DEVICE_DEVICE_VERSION,	// Device release number
	USB_DEVICE_IDX_MAN,		// Index of manufacturer string descr.
	USB_DEVICE_IDX_PRO,		// Index of product string descr.
	USB_DEVICE_IDX_SER,		// Index of serial number string descr.
	USB_DEVICE_NUM_CONFIGS		// Number of possible configurations (1)
    } ;

//------------------------------------------------------------------------------

const usb_configuration_desc_t
    usbConfigurationDesc PROGMEM =
    {
	{				// Configuration Descriptor
	    USB_CONFIG_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_CONFIG_DESC_TYPE,	// Config (0x02)
	    wUSB( sizeof( usb_configuration_desc_t ) ),// Total size of this configuration
	    USB_CONFIG_NUM_INTERF,	// Number of interfaces in this configuration
	    USB_CONFIG_VAL,		// Value to use to select this configuration
	    USB_CONFIG_IDX,		// Index of string descr. this configuration
	    USB_CONFIG_ATTR,		// Configuration characteristics
	    USB_CONFIG_MAXPOWER		// Max. power consumption in 2mA units
	},

	{				// Interface Descriptor Keyboard
	    USB_INTERF_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_INTERF_DESC_TYPE,	// Interface (0x04)
	    USB_INTERF_NUM_KBD,		// Number of this interface (0 based)
	    USB_INTERF_ALTSET_NONE,	// Value to select alternative setting
	    USB_INTERF_NUMEP_KBD,	// Number of EPs for this interface
	    USB_INTERF_CLASS_HID,	// Class code
	    USB_INTERF_SUBCLASS_BOOT,	// Subclass code
	    USB_INTERF_PROTOCOL_KBD,	// Protocol code
	    USB_INTERF_IDX_KBD		// Index of string descr. this interface
	},
	{				// HID Descriptor Keyboard
	    USB_HID_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_HID_DESC_TYPE,		// HID (0x21)
	    USB_HID_VERSION,		// HID class version suported (1.11)
	    USB_HID_COUNTRY_ANY,	// Country code
	    USB_HID_NUM_DESC_1,		// Number of class descriptors (1, report)
	    USB_HIDREP_DESC_TYPE,	// Class descriptor type (0x22, HID report)
	    wUSB( sizeof( usbHidRepDescKbd ) )	// Class descriptor length
	},
	{				// EP Descriptor Keyboard
	    USB_EP_DESC_SZ,		// Size of this descriptor in bytes (7)
	    USB_EP_DESC_TYPE,		// Endpoint (0x05)
	    (EP_HID_KBD | USB_EP_IN),	// Endpoint address
	    USB_EP_INTR,		// Endpoint attributes
	    wUSB( EP_HID_KBD_SZ ),	// Max. packet size this EP can transfer
	    USB_EP_POLL_INTERVAL_KBD	// Polling interval for this EP in ms
	},

#if ENABLE_CTRL_KEYS
	{				// Interface Descriptor Control
	    USB_INTERF_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_INTERF_DESC_TYPE,	// Interface (0x04)
	    USB_INTERF_NUM_CTRL,	// Number of this interface (0 based)
	    USB_INTERF_ALTSET_NONE,	// Value to select alternative setting
	    USB_INTERF_NUMEP_CTRL,	// Number of EPs for this interface
	    USB_INTERF_CLASS_HID,	// Class code
	    USB_INTERF_SUBCLASS_NONE,	// Subclass code
	    USB_INTERF_PROTOCOL_NONE,	// Protocol code
	    USB_INTERF_IDX_CTRL		// Index of string descr. this interface
	},
	{				// HID Descriptor Control
	    USB_HID_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_HID_DESC_TYPE,		// HID (0x21)
	    USB_HID_VERSION,		// HID class version suported (1.11)
	    USB_HID_COUNTRY_ANY,	// Country code
	    USB_HID_NUM_DESC_1,		// Number of class descriptors (1, report)
	    USB_HIDREP_DESC_TYPE,	// Class descriptor type (0x22, HID report)
	    wUSB( sizeof( usbHidRepDescCtrl ) )	// Class descriptor length
	},
	{				// EP Descriptor Control
	    USB_EP_DESC_SZ,		// Size of this descriptor in bytes (7)
	    USB_EP_DESC_TYPE,		// Endpoint (0x05)
	    (EP_HID_CTRL | USB_EP_IN),	// Endpoint address
	    USB_EP_INTR,		// Endpoint attributes
	    wUSB( EP_HID_CTRL_SZ ),	// Max. packet size this EP can transfer
	    USB_EP_POLL_INTERVAL_CTRL	// Polling interval for this EP in ms
	},
#endif

#if ENABLE_DBG_OUT
	{				// Interface Descriptor Control
	    USB_INTERF_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_INTERF_DESC_TYPE,	// Interface (0x04)
	    USB_INTERF_NUM_DBG,		// Number of this interface (0 based)
	    USB_INTERF_ALTSET_NONE,	// Value to select alternative setting
	    USB_INTERF_NUMEP_DBG,	// Number of EPs for this interface
	    USB_INTERF_CLASS_HID,	// Class code
	    USB_INTERF_SUBCLASS_NONE,	// Subclass code
	    USB_INTERF_PROTOCOL_NONE,	// Protocol code
	    USB_INTERF_IDX_DBG		// Index of string descr. this interface
	},
	{				// HID Descriptor Control
	    USB_HID_DESC_SZ,		// Size of this descriptor in bytes (9)
	    USB_HID_DESC_TYPE,		// HID (0x21)
	    USB_HID_VERSION,		// HID class version suported (1.11)
	    USB_HID_COUNTRY_ANY,	// Country code
	    USB_HID_NUM_DESC_1,		// Number of class descriptors (1, report)
	    USB_HIDREP_DESC_TYPE,	// Class descriptor type (0x22, HID report)
	    wUSB( sizeof( usbHidRepDescDbg ) )	// Class descriptor length
	},
	{				// EP Descriptor Control
	    USB_EP_DESC_SZ,		// Size of this descriptor in bytes (7)
	    USB_EP_DESC_TYPE,		// Endpoint (0x05)
	    (EP_HID_DBG | USB_EP_IN),	// Endpoint address
	    USB_EP_INTR,		// Endpoint attributes
	    wUSB( EP_HID_DBG_SZ ),	// Max. packet size this EP can transfer
	    USB_EP_POLL_INTERVAL_DBG	// Polling interval for this EP in ms
	}
#endif
    } ;

//------------------------------------------------------------------------------
