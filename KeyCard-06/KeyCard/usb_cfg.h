/*******************************************************************************
 * File Name	: usb_cfg.h
 * Project	: Keycard
 * Date		: 2014/01/23
 * Version	: 1.0
 * Target MCU   : ATMEGA16U2
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Release Notes:
 *
 * $Id$
 ******************************************************************************/

#ifndef __usb_cfg_h__
#define __usb_cfg_h__

//------------------------------------------------------------------------------

#define	ENABLE_CTRL_KEYS		1
#define	ENABLE_DBG_OUT			0 /* not tested or fully implemented yet */

//------------------------------------------------------------------------------

#define USB_MANUFACTURER_ID		wUSB( 0x16C0 )	/* Teensy */
#define USB_PRODUCT_ID			wUSB( 0x047C )	/* KB */
#define USB_STRING_MAN			'T','e','c','h','K','e','y','s'
#define USB_STRING_MAN_SZ		8

#define USB_STRING_PRO			'K','e','y','b','o','a','r','d',' ',	\
					'B','u','s','i','n','e','s','s',' ',	\
					'C','a','r','d'
#define USB_STRING_PRO_SZ		22

#define USB_DEVICE_VERSION		wUSB( 0x0101 )	/* 1.01 */

#define USB_STRING_SER			'0', '0', '0', '2'
#define USB_STRING_SER_SZ		4

#define	USB_MAXPOWER			100		/* 100mA */

#define	USB_EP_POLL_INTERVAL_KBD	8		/*  8ms polling */
#define	USB_EP_POLL_INTERVAL_CTRL	10		/* 10ms polling */
#define	USB_EP_POLL_INTERVAL_DBG	10		/* 10ms polling */

//------------------------------------------------------------------------------
// Report descriptor sizes

#define	USB_HIDREP_DESC_KBD_SZ		61
#define	USB_HIDREP_DESC_CTRL_SZ		70
#define	USB_HIDREP_DESC_DBG_SZ		21

//------------------------------------------------------------------------------
// Report sizes

#define	USB_REPORT_SZ_KBD		8	/* 1 (mods) + 1 (fill) + 6 (keys) */
#define	USB_REPORT_SZ_CTRL		3	/* 1 (id) + 2 (key) */
#define	USB_REPORT_SZ_DBG		16

//------------------------------------------------------------------------------
// Endpoind definitions

#define	EP_HID_KBD			3		/* Endpoint 3 */
#define	EP_HID_KBD_SZ			USB_REPORT_SZ_KBD

#define	EP_HID_CTRL			4		/* Endpoint 4 */
#define	EP_HID_CTRL_SZ			(ENABLE_CTRL_KEYS * USB_REPORT_SZ_CTRL)

#define	EP_HID_DBG			2		/* Endpoint 2 */
#define	EP_HID_DBG_SZ			(ENABLE_DBG_OUT * USB_REPORT_SZ_DBG)

#define EP0_SZ				64
#define EP1_SZ				0
#define EP2_SZ				EP_HID_DBG_SZ
#define EP3_SZ				EP_HID_KBD_SZ
#define EP4_SZ				EP_HID_CTRL_SZ

//-------------------------------------------------------------------------------

#endif	// __usb_cfg_h__
