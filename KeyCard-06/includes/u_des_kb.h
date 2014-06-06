/******************************************************************************
 * File Name	: u_des_kb.h
 * Project	: KeyCard
 * Date		: 2014/01/09
 * Version	: 1.0
 * Target MCU   : AVR
 * Tool Chain	: Atmel AVR Studio 4.19 730 / gcc 4.7.0 / avr-libc 1.8.0
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Release Notes:
 *
 * $Id$
 *
 * $Log$
 ******************************************************************************/

#ifndef	__u_des_kb_h__
#define	__u_des_kb_h__

//------------------------------------------------------------------------------

#define LSB( n )			( (uint16_t)(n)       & 0xFF)
#define MSB( n )			(((uint16_t)(n) >> 8) & 0xFF)

#define	LVAL( n )			LSB( n ), MSB( n )

#define	wUSB( n )			((uint16_t)(n))

//------------------------------------------------------------------------------

// USB Descriptor definitions

//------------------------------------------------------------------------------

typedef struct
    usb_string_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes
	uint8_t  bDescriptorType ;	// String (0x03)
	uint16_t wString[] ;		// Unicode encoded string
    }
    usb_string_desc_t ;

//------------------------------------------------------------------------------

#define	USB_STRING_DESC_TYPE		0x03
#define USB_STRING_DESC_SZ( sz )	(sizeof( uint8_t ) * 2 + sizeof( int16_t ) * (sz))

#define	USB_STRING_LAN			wUSB( 0x0409 )
#define	USB_STRING_LAN_SZ		1

#define	USB_STRING_DESC_LAN_SZ		USB_STRING_DESC_SZ( USB_STRING_LAN_SZ )
#define	USB_STRING_DESC_MAN_SZ		USB_STRING_DESC_SZ( USB_STRING_MAN_SZ )
#define	USB_STRING_DESC_PRO_SZ		USB_STRING_DESC_SZ( USB_STRING_PRO_SZ )
#define	USB_STRING_DESC_SER_SZ		USB_STRING_DESC_SZ( USB_STRING_SER_SZ )

#define	USB_STRING_IDX_NUL		0
#define	USB_STRING_IDX_LAN		0
#define	USB_STRING_IDX_MAN		1
#define	USB_STRING_IDX_PRO		2
#define	USB_STRING_IDX_SER		3

//------------------------------------------------------------------------------

typedef struct
    usb_device_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes (18)
	uint8_t  bDescriptorType ;	// Device (0x01)
	uint16_t bcdUSB ;		// USB Specification which device complies too
	uint8_t  bDeviceClass ;		// Class code. 0 if interf. defines class
	uint8_t  bDeviceSubClass ;	// Subclass code
	uint8_t  bDeviceProtocol ;	// Protocol code
	uint8_t  bMaxPacketSize ;	// Max. packet size for EP0. 8, 16,32, or 64
	uint16_t idVendor ;		// Vendor ID
	uint16_t idProduct ;		// Product ID
	uint16_t bcdDevice ;		// Device release number
	uint8_t  iManufacturer ;	// Index of manufacturer string descr.
	uint8_t  iProduct ;		// Index of product string descr.
	uint8_t  iSerialNumber ;	// Index of serial number string descr.
	uint8_t  bNumConfigurations ;	// Number of possible configurations (1)
    }
    usb_device_desc_t ;

//------------------------------------------------------------------------------

#define	USB_DEVICE_DESC_SZ		18
#define	USB_DEVICE_DESC_TYPE		0x01

#define	USB_DEVICE_USBVER		wUSB( 0x0200 )
#define	USB_DEVICE_CLASS		0
#define	USB_DEVICE_SUBCLASS		0
#define	USB_DEVICE_PROTOCOL		0
#define	USB_DEVICE_MAXPKTSZ		EP0_SZ

#define USB_DEVICE_MAN_ID		USB_MANUFACTURER_ID
#define	USB_DEVICE_PRO_ID		USB_PRODUCT_ID
#define USB_DEVICE_DEVICE_VERSION	USB_DEVICE_VERSION

#define	USB_DEVICE_IDX_MAN		USB_STRING_IDX_MAN
#define	USB_DEVICE_IDX_PRO		USB_STRING_IDX_PRO
#define	USB_DEVICE_IDX_SER		USB_STRING_IDX_SER

#define	USB_DEVICE_NUM_CONFIGS		1

//------------------------------------------------------------------------------

typedef struct
    usb_config_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes (9)
	uint8_t  bDescriptorType ;	// Config (0x02)
	uint16_t wTotalLength ;		// Total size of this configuration
	uint8_t  bNumInterfaces ;	// Number of interfaces in this configuration
	uint8_t  bConfigurationValue ;	// Value to use to select this configuration
	uint8_t  iConfiguration ;	// Index of string descr. this configuration
	uint8_t  bmAttributes ;		// Configuration characteristics
	uint8_t  bMaxPower ;		// Max. power consumption in 2mA units
    }
    usb_config_desc_t ;

//------------------------------------------------------------------------------

#define	USB_CONFIG_DESC_SZ		9
#define	USB_CONFIG_DESC_TYPE		0x02

#define	USB_CONFIG_NUM_INTERF		(1 + ENABLE_CTRL_KEYS + ENABLE_DBG_OUT)

#define	USB_CONFIG_VAL			1
#define	USB_CONFIG_IDX			USB_STRING_IDX_NUL

#define USB_CONFIG_ATTR_BUSPOWER	0x80	/* Mandatory attr., see USB 2.0 9.6.3 */
#define USB_CONFIG_ATTR_SELFPOWER	0x40
#define USB_CONFIG_ATTR_REMOTEWAKE	0x20

#define	USB_CONFIG_ATTR			(USB_CONFIG_ATTR_BUSPOWER | USB_CONFIG_ATTR_REMOTEWAKE)

#define	USB_CONFIG_MAXPOWER		(USB_MAXPOWER / 2)

//------------------------------------------------------------------------------

typedef struct
    usb_interf_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes (9)
	uint8_t  bDescriptorType ;	// Interface (0x04)
	uint8_t  bInterfaceNumber ;	// Number of this interface (0 based)
	uint8_t  bAlternativeSetting ;	// Value to select alternative setting
	uint8_t  bNumEndpoints ;	// Number of EPs for this interface
	uint8_t  bInterfaceClass ;	// Class code
	uint8_t  bInterfaceSubClass ;	// Subclass code
	uint8_t  bInterfaceProtocol ;	// Protocol code
	uint8_t  iInterface ;		// Index of string descr. this interface
    }
    usb_interf_desc_t ;

//------------------------------------------------------------------------------

#define	USB_INTERF_DESC_SZ		9
#define	USB_INTERF_DESC_TYPE		0x04

#define	USB_INTERF_NUM_KBD		0	/* Keyboard interface */
#define	USB_INTERF_NUM_CTRL		1	/* Media & system control interface */
#define	USB_INTERF_NUM_DBG		(1 + ENABLE_CTRL_KEYS)	/* Debug output interface */

#define	USB_INTERF_ALTSET_NONE		0

#define	USB_INTERF_NUMEP_KBD		1
#define	USB_INTERF_NUMEP_CTRL		1
#define	USB_INTERF_NUMEP_DBG		1

#define	USB_INTERF_CLASS_HID		3
#define	USB_INTERF_SUBCLASS_NONE	0
#define	USB_INTERF_SUBCLASS_BOOT	1
#define	USB_INTERF_PROTOCOL_NONE	0
#define	USB_INTERF_PROTOCOL_KBD		1

#define	USB_INTERF_IDX_KBD		USB_STRING_IDX_NUL
#define	USB_INTERF_IDX_CTRL		USB_STRING_IDX_NUL
#define	USB_INTERF_IDX_DBG		USB_STRING_IDX_NUL

//------------------------------------------------------------------------------

typedef struct
    usb_hid_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes (9)
	uint8_t  bDescriptorType ;	// HID (0x21)
	uint16_t bcdHID ;		// HID class version suported (1.11)
	uint8_t  bCountryCode ;		// Country code
	uint8_t  bNumDescriptors ;	// Number of class descriptors (1, report)
	uint8_t  bDescrType ;		// Class descriptor type (0x22, HID report)
	uint16_t wDescrLength ;		// Class descriptor length
    }
    usb_hid_desc_t ;

//------------------------------------------------------------------------------

#define	USB_HID_DESC_SZ			9
#define	USB_HID_DESC_TYPE		0x21

#define	USB_HID_VERSION			wUSB( 0x0111 )
#define	USB_HID_COUNTRY_ANY		0
#define	USB_HID_NUM_DESC_1		1

#define	USB_HIDREP_DESC_TYPE		0x22

//------------------------------------------------------------------------------

typedef struct
    usb_ep_desc_struct
    {
	uint8_t  bLength ;		// Size of this descriptor in bytes (7)
	uint8_t  bDescriptorType ;	// Endpoint (0x05)
	uint8_t  bEndpointAddress ;	// Endpoint address
	uint8_t  bmAttributes ;		// Endpoint attributes
	uint16_t wMaxPacketSize ;	// Max. packet size this EP can transfer
	uint8_t  bInterval ;		// Polling interval for this EP in ms
    }
    usb_ep_desc_t ;

//------------------------------------------------------------------------------

#define	USB_EP_DESC_SZ			7
#define	USB_EP_DESC_TYPE		0x05

#define	USB_EP_IN			0x80
#define	USB_EP_INTR			0x03

//------------------------------------------------------------------------------

typedef struct
    usb_configuration_desc_struct
    {
	usb_config_desc_t cfg ;

	usb_interf_desc_t if_kbd ;
	usb_hid_desc_t    hid_kbd ;
	usb_ep_desc_t     ep_kbd ;

#if ENABLE_CTRL_KEYS
	usb_interf_desc_t if_ctrl ;
	usb_hid_desc_t    hid_ctrl ;
	usb_ep_desc_t     ep_ctrl ;
#endif

#if ENABLE_DBG_OUT
	usb_interf_desc_t if_dbg ;
	usb_hid_desc_t    hid_dbg ;
	usb_ep_desc_t     ep_dbg ;
#endif
    }
    usb_configuration_desc_t ;

//------------------------------------------------------------------------------

typedef struct
    {
	uint8_t  mod ;				// Modifier Keys 224-231
	uint8_t  bReserved ;			// Reserved
	uint8_t  keys[USB_REPORT_SZ_KBD - 2] ;	// Keys
    }
    kb_report_t ;

#if ENABLE_CTRL_KEYS

typedef struct
    {
	uint8_t  id ;				// Report ID
	uint16_t key ;				// Key usage
    }
    ctrl_report_t ;

#endif

#if ENABLE_DBG_OUT

typedef struct
    {
	uint8_t  data[USB_REPORT_SZ_DBG] ;
    }
    cdg_report_t ;

#endif

//------------------------------------------------------------------------------

// Descriptor declarations

extern const uint8_t
    usbHidRepDescKbd[USB_HIDREP_DESC_KBD_SZ] PROGMEM ;

#if ENABLE_CTRL_KEYS

extern const uint8_t
    usbHidRepDescCtrl[USB_HIDREP_DESC_CTRL_SZ] PROGMEM ;
#endif

#if ENABLE_DBG_OUT

extern const uint8_t
    usbHidRepDescDbg[USB_HIDREP_DESC_DBG_SZ] PROGMEM ;
#endif

extern const usb_string_desc_t
    usbStringDescLan PROGMEM,
    usbStringDescMan PROGMEM,
    usbStringDescPro PROGMEM,
    usbStringDescSer PROGMEM ;

extern const usb_device_desc_t
    usbDeviceDesc PROGMEM ;

extern const usb_configuration_desc_t
    usbConfigurationDesc PROGMEM ;

//------------------------------------------------------------------------------

#endif	// _USBDESCR_H_

//------------------------------------------------------------------------------
