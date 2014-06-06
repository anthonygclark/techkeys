/*******************************************************************************
 * File Name	: kb.h
 * Project	: Keycard
 * Date		: 2013/11/09
 * Version	: 1.0
 * Target MCU   : ATMEGA16U2
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Release Notes:
 *
 * $Id$
 ******************************************************************************/

#ifndef	__kb_h__
#define	__kb_h__

//-------------------------------------------------------------------------------
// Keycard Hardware Specific Definitions

#define LED_PORT	PORTB
#define bLED_SYS	PB1

#define LED_on( b )	clr_bit( LED_PORT, (b) )
#define LED_off( b )	set_bit( LED_PORT, (b) )
#define LED_sts( b )	bit_is_clear( LED_PORT, (b) )

//-------------------------------------------------------------------------------
// Macros to deal w/ timers

// Timer prescaler settings

#define	TPS_1( n )	(_B0(CS ## n ## 2) | _B0(CS ## n ## 1) | _B1(CS ## n ## 0))
#define	TPS_8( n )	(_B0(CS ## n ## 2) | _B1(CS ## n ## 1) | _B0(CS ## n ## 0))
#define	TPS_64( n )	(_B0(CS ## n ## 2) | _B1(CS ## n ## 1) | _B1(CS ## n ## 0))
#define	TPS_256( n )	(_B1(CS ## n ## 2) | _B0(CS ## n ## 1) | _B0(CS ## n ## 0))
#define	TPS_1024( n )	(_B1(CS ## n ## 2) | _B0(CS ## n ## 1) | _B1(CS ## n ## 0))

#define	ResetTM( n, del )	__WRAP__( {				\
					TCNT ## n = (del) ;		\
					TIFR ## n = _BV( TOV ## n ) ;	\
				} )

#define	TMexp( n )		bit_is_set( TIFR ## n, TOV ## n )

#define	TMocrfa( n )		bit_is_set( TIFR ## n, OCF ## n ## A )

#define	ResetOCRFA( n )		TIFR ## n = _BV( OCF ## n ## A )

#define	SetTMPS( n, ps )	TCCR ## n ## B = TPS_ ## ps ( n )

//-------------------------------------------------------------------------------
// Timer Macros, msec or usec to timer ticks. ps = prescaler value

#define MS2TM( ms, ps ) (int16_t)(((ms) * (F_CPU /    1000.)) / (ps) + .5)
#define US2TM( us, ps ) (int16_t)(((us) * (F_CPU / 1000000.)) / (ps) + .5)

//-------------------------------------------------------------------------------
// kb.c interface

#define	SIG_RESET		TRUE	/* Signal reset to KB routines */
#define	SIG_MAINT		FALSE	/* Signal maintain to KB routines */

//-------------------------------------------------------------------------------

#define REP0_CHG		0x01	/* Report Changed flags */
#define REP1_CHG		0x02
#define REP2_CHG		0x04

//-------------------------------------------------------------------------------

extern uint8_t
    check_keys( uint8_t reset ),	// Check for any key down in sleep mode
    read_matrix( uint8_t reset ) ;	// Read and maintain key matrix

extern void
    led_pwm( void ),			// Maintain LED PWM
    sleep_led( uint8_t reset ),		// Maintain Sleep LED
    maint_leds( uint8_t reset ),	// Maintain LEDs
    hw_init( void ) ;			// Initialize HW

//-------------------------------------------------------------------------------

#endif	// __kb_h__
