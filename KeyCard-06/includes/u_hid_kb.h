/*******************************************************************************
 * File Name	: usb_hid.h
 * Project	: Keyboard
 * Date		: 2013/11/06
 * Version	: 1.0
 * Target MCU   : AT90USB8/162, ATMEGA16/32U4/U2, AT90USB64/1286
 * Tool Chain	: Atmel AVR Studio 4.19 730 / gcc 4.7.0 / avr-libc 1.8.0
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Release Notes:
 *
 * $Id$
 *
 * $Log$
 ******************************************************************************/

#ifndef __usb_hid_h__
#define __usb_hid_h__

//------------------------------------------------------------------------------

extern void
    usb_init( void ) ;			// Initialize USB module

extern uint8_t
    usb_configured( void ) ;		// Check if USB is active

extern uint8_t
    usb_remote_wakeup( void ) ;		// USB remote wakeup

extern uint8_t
    usb_IN_busy( uint8_t ep ) ;		// Check if IN pipe is busy

extern uint8_t				// Send data via IN pipe
    usb_send_IN( uint8_t *data, uint8_t len, uint8_t ep ) ;

//------------------------------------------------------------------------------

extern volatile uint8_t
    usb_configuration,			// Selected USB configuration, 0 if none
    usb_suspend,			// Suspended if TRUE
    usb_rem_wakeup ;			// Remote wakeup allowed if TRUE

//------------------------------------------------------------------------------

extern kb_report_t
    kbd_report ;			// Keyboard IN report

extern volatile uint8_t
    kbd_protocol,			// 0: Boot protocol, 1: Report protocol
    kbd_leds ;				// LED OUT report

extern volatile uint8_t
    kbd_idle_rate,			// idle rate in 4ms clicks, 0 for indefinite
    kbd_idle_cnt ;			// idle counter

#if ENABLE_CTRL_KEYS

extern ctrl_report_t
    ctrl_report1,			// System Control IN report
    ctrl_report2 ;			// Consumer Control IN report

extern volatile uint8_t
    ctrl_leds ;

#endif

#if ENABLE_DBG_OUT
#endif

//------------------------------------------------------------------------------

#ifdef __u_hid_kb__

//------------------------------------------------------------------------------
// usb_hid.c local defines
//------------------------------------------------------------------------------

#define	USBLV( h, l )		(((uint16_t)(h) << 8) + (l))

//------------------------------------------------------------------------------

#if defined(__AVR_AT90USBX2__) || defined(__AVR_ATmegaXU2__)

 #define UHW_CONFIG()

 #if F_CPU == 8000000
  #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | \
				  _B0(PLLP2)  | _B0(PLLP1)   | _B0(PLLP0))
 #else
  #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | \
				  _B0(PLLP2)  | _B0(PLLP1)   | _B1(PLLP0))
 #endif

 #define USB_CONFIG()	(USBCON = _B1(USBE)   | _B0(FRZLK))

#elif defined(__AVR_AT90USBX6__)

 #define UHW_CONFIG()	(UHWCON = _B1(UVREGE) | \
				  _B1(UIMOD)  | _B0(UIDE)    | _B0(UVCONE))
 #define USB_CONFIG()	(USBCON = _B1(USBE)   | _B0(FRZCLK)  | \
 				  _B0(HOST)   | _B1(OTGPADE) | _B0(IDTE) | _B0(VBUSTE))

 #if F_CPU == 8000000
  #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | \
				  _B0(PLLP2)  | _B1(PLLP1)   | _B1(PLLP0))
 #else
  #if defined(__AVR_AT90USB646__)
   #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | \
				  _B1(PLLP2)  | _B1(PLLP1)   | _B0(PLLP0))
  #elif defined(__AVR_AT90USB1286__)
   #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | \
				  _B1(PLLP2)  | _B0(PLLP1)   | _B1(PLLP0))
  #endif
 #endif

#elif defined(__AVR_ATmegaXU4__)

 #define UHW_CONFIG()	(UHWCON = _B1(UVREGE))

 #if F_CPU == 8000000
  #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | _B0(PINDIV))
 #else
  #define PLL_CONFIG()	(PLLCSR = _B1(PLLE)   | _B0(PLOCK)   | _B1(PINDIV))
 #endif

 #define USB_CONFIG()	(USBCON = _B1(USBE)   | _B0(FRZCLK)  | \
				  _B1(OTGPADE)| _B0(VBUSTE))
#else
 #error "Current device not supported"
#endif

//------------------------------------------------------------------------------

#define EP_TYPE_CONTROL		(_B0(EPTYPE1) | _B0(EPTYPE0) | _B0(EPDIR))
#define EP_TYPE_INTERRUPT_IN	(_B1(EPTYPE1) | _B1(EPTYPE0) | _B1(EPDIR))
#define EP_TYPE_INTERRUPT_OUT	(_B1(EPTYPE1) | _B1(EPTYPE0) | _B0(EPDIR))

#if defined(__AVR_ATmegaXU2__)

#define EP_TYPE_BULK_IN		(_B0(EPTYPE1) | _B1(EPTYPE0) | _B1(EPDIR))
#define EP_TYPE_BULK_OUT	(_B0(EPTYPE1) | _B1(EPTYPE0) | _B0(EPDIR))
#define EP_TYPE_ISOCHRONOUS_IN	(_B1(EPTYPE1) | _B0(EPTYPE0) | _B1(EPDIR))
#define EP_TYPE_ISOCHRONOUS_OUT	(_B1(EPTYPE1) | _B0(EPTYPE0) | _B0(EPDIR))

#else

#define EP_TYPE_BULK_IN		(_B1(EPTYPE1) | _B0(EPTYPE0) | _B1(EPDIR))
#define EP_TYPE_BULK_OUT	(_B1(EPTYPE1) | _B0(EPTYPE0) | _B0(EPDIR))
#define EP_TYPE_ISOCHRONOUS_IN	(_B0(EPTYPE1) | _B1(EPTYPE0) | _B1(EPDIR))
#define EP_TYPE_ISOCHRONOUS_OUT	(_B0(EPTYPE1) | _B1(EPTYPE0) | _B0(EPDIR))

#endif

#define EP_SINGLE_BUFFER	(_B0(EPBK1) | _B0(EPBK0) | _B1(ALLOC))
#define EP_DOUBLE_BUFFER	(_B0(EPBK1) | _B1(EPBK0) | _B1(ALLOC))

//------------------------------------------------------------------------------

#if defined(__AVR_AT90USBX2__) || defined(__AVR_ATmegaXU2__)

#define EP_SIZE( s )	((s) <=   8 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <=  16 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) <=  32 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0)) : \
			/* 64 */      (_B0(EPSIZE2) | _B1(EPSIZE1) | _B1(EPSIZE0)))))

#elif defined(__AVR_AT90USBX6__) && ! defined(__AVR_ATmegaXU4__)

// 256 & 128 for EP1 only

#define EP_SIZE( s )	((s) <=   8 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <=  16 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) <=  32 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <=  64 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) <= 128 ? (_B1(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			/* 256 */     (_B1(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)))))))

#elif defined(__AVR_ATmegaXU4__)

#define EP_SIZE( s )	((s) <=   8 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <=  16 ? (_B0(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) <=  32 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <=  64 ? (_B0(EPSIZE2) | _B1(EPSIZE1) | _B1(EPSIZE0)) : \
			((s) <= 128 ? (_B1(EPSIZE2) | _B0(EPSIZE1) | _B0(EPSIZE0)) : \
			((s) <= 256 ? (_B1(EPSIZE2) | _B0(EPSIZE1) | _B1(EPSIZE0)) : \
			/* 512 */     (_B1(EPSIZE2) | _B1(EPSIZE1) | _B0(EPSIZE0))))))))
#endif

//------------------------------------------------------------------------------

#if defined(__AVR_AT90USBX2__) || defined(__AVR_ATmegaXU2__)

 #define MAX_ENDPOINT		4

#elif defined(__AVR_ATmegaXU4__) || defined(__AVR_AT90USBX6__)

 #define MAX_ENDPOINT		6

#endif

//------------------------------------------------------------------------------
// Standard control endpoint request types

#define GET_STATUS		0
#define CLEAR_FEATURE		1
#define SET_FEATURE		3
#define SET_ADDRESS		5
#define GET_DESCRIPTOR		6
#define GET_CONFIGURATION	8
#define SET_CONFIGURATION	9
#define GET_INTERFACE		10
#define SET_INTERFACE		11

//------------------------------------------------------------------------------
// HID (human interface device) request types

#define HID_GET_REPORT		1
#define HID_GET_IDLE		2
#define HID_GET_PROTOCOL	3
#define HID_SET_REPORT		9
#define HID_SET_IDLE		10
#define HID_SET_PROTOCOL	11

//------------------------------------------------------------------------------

#if defined(__AVR_AT90USBX2__) || defined(__AVR_ATmegaXU2__)
 #define EP1_BUFFER		EP_SINGLE_BUFFER
 #define EP2_BUFFER		EP_SINGLE_BUFFER
 #define EP3_BUFFER		EP_DOUBLE_BUFFER
 #define EP4_BUFFER		EP_DOUBLE_BUFFER
#else
 #define EP1_BUFFER		EP_DOUBLE_BUFFER
 #define EP2_BUFFER		EP_DOUBLE_BUFFER
 #define EP3_BUFFER		EP_DOUBLE_BUFFER
 #define EP4_BUFFER		EP_DOUBLE_BUFFER

 #if defined(__AVR_ATmegaXU4__) || defined(__AVR_AT90USBX6__)
  #define EP5_BUFFER		EP_DOUBLE_BUFFER
  #define EP6_BUFFER		EP_DOUBLE_BUFFER
 #endif
#endif

//------------------------------------------------------------------------------

#endif // __usb_hid__

//------------------------------------------------------------------------------

#endif // __usb_hid_h__

//------------------------------------------------------------------------------
