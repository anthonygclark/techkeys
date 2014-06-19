/*******************************************************************************
 * File Name	: main.c
 * Project	: Keycard
 * Date		: 2014/01/09
 * Version	: 1.0
 * Target MCU   : ATMEGA16U2
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * References	:
 * Device	: TechKeys Business Card
 *
 * Release Notes:
 *
 * $Id$
 ******************************************************************************/

#include <includes.h>
#include "usb_cfg.h"
#include <u_des_kb.h>
#include <u_hid_kb.h>
#include "kb.h"

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

#define	IDLE_PERIOD_MS		4			/* 4ms per idle period */
#define	IDLE_PERIOD		(IDLE_PERIOD_MS * 2)	/* Running on 0.5us clock */

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

// Watchdog is not turned off by a reset, see avr-libc's wdt.h documentation

// uint8_t VA_NOINIT( mcusr_cpy ) ;

void FA_INIT3( Init3 ) ( void )
{
//  mcusr_cpy = MCUSR ;			// If we need to examine reset reason
    MCUSR = 0 ;
    wdt_disable() ;
}

//------------------------------------------------------------------------------

static volatile uint8_t
    alarm ;

ISR( TIMER1_COMPA_vect )
{
    alarm = TRUE ;			// Signal that 500us are up
}

//------------------------------------------------------------------------------

int FA_NORETURN( main ) ( void )
{
    uint8_t
	suspended = TRUE,		// Offline
	idle = IDLE_PERIOD,		// idle period timer
	kbd_sendrep = FALSE ;		// "need to send report" signal

    // Initialize..

    hw_init() ;				// hardware
    usb_init() ;			// USB stack

    // Initial idle rate for keyboards is 500ms

    kbd_idle_rate = 500 / IDLE_PERIOD_MS ;

    set_sleep_mode( SLEEP_MODE_IDLE ) ;	// Idle is the best we can do in this setup

    ResetTM( 1, 0 ) ;			// Reset T1
    ResetOCRFA( 1 ) ;			// Clear T1 Output Compare A match Flag

    set_bit( TIMSK1, OCIE1A ) ;		// Enable T1 COMPA IRQ

    for ( ;; )				// Forever..
    {
	sleep_enable() ;		// Prepare for sleep

        for ( ; ! alarm ; )		// No alarm pending,
	    sleep_cpu() ;		// snooze.

        sleep_disable() ;

	// 500us are up, do house keeping

	alarm = FALSE ;			// reset alarm

	if ( usb_configured() )		// USB link up and running
	{
	    if ( suspended )		// Coming out of suspend or reset
	    {
		suspended = FALSE ;

		sleep_led( SIG_RESET ) ;	// Reset sleep LED

		// Set clock divider to 1, full speed

		clock_prescale_set( clock_div_1 ) ;

		wdt_enable( WDTO_500MS ) ;	// Unleash watchdog

		read_matrix( SIG_RESET ) ;	// Reset key matrix
		maint_leds( SIG_RESET ) ;	// Reset LED system

		idle = IDLE_PERIOD ;
		kbd_sendrep  = FALSE ;		// Reset send report flag
		kbd_idle_cnt = kbd_idle_rate ;	// Reset idle counter
	    }

	    led_pwm() ;				// Maintain LED PWM

	    maint_leds( SIG_MAINT ) ;		// Maintain LEDs

	    kbd_sendrep |=
		read_matrix( SIG_MAINT ) ;	// Read & maintain key matrix

	    if ( ! idle-- )			// Maintain idle counters
	    {
		idle = IDLE_PERIOD ;
						// Idle counter expired, rate not indef.
		if ( ! --kbd_idle_cnt && kbd_idle_rate )
		    kbd_sendrep |= REP0_CHG ;	// need to send current report
	    }

	    if ( (kbd_sendrep & REP0_CHG) && ! usb_IN_busy( EP_HID_KBD ) )
	    {
		LED_on( bLED_SYS ) ;

		usb_send_IN( VP( &kbd_report ), sizeof( kbd_report ), EP_HID_KBD ) ;

		kbd_sendrep &= ~REP0_CHG ;	// reset send report flag
		kbd_idle_cnt = kbd_idle_rate ;	// reset idle counter

		LED_off( bLED_SYS ) ;
	    }

	    if ( (kbd_sendrep & REP1_CHG) && ! usb_IN_busy( EP_HID_CTRL ) )
	    {
		LED_on( bLED_SYS ) ;

		usb_send_IN( VP( &ctrl_report1 ), sizeof( ctrl_report1 ), EP_HID_CTRL ) ;

		kbd_sendrep &= ~REP1_CHG ;

		LED_off( bLED_SYS ) ;
	    }

	    if ( (kbd_sendrep & REP2_CHG) && ! usb_IN_busy( EP_HID_CTRL ) )
	    {
		LED_on( bLED_SYS ) ;

		usb_send_IN( VP( &ctrl_report2 ), sizeof( ctrl_report2 ), EP_HID_CTRL ) ;

		kbd_sendrep &= ~REP2_CHG ;

		LED_off( bLED_SYS ) ;
	    }

	    wdt_reset() ;		// Pet watchdog
	}
	else				// USB link down
	if ( usb_suspend )		// ..because we are suspended
	{
	    if ( ! suspended )		// Just got suspended
	    {
		suspended = TRUE ;

		maint_leds( SIG_RESET ) ;	// Reset LED system
		sleep_led( SIG_RESET ) ;	// Reset sleep LED
		check_keys( SIG_RESET ) ;	// Reset key check

		wdt_disable() ;			// Leash watchdog

		// Set clock divider to 2, half speed

		clock_prescale_set( clock_div_2 ) ;
	    }

	    sleep_led( SIG_MAINT ) ;		// Maintain sleep LED

	    if ( check_keys( SIG_MAINT ) )	// Check for any key down
		usb_remote_wakeup() ;
	}
    }
}

//------------------------------------------------------------------------------
