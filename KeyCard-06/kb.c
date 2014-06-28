/*******************************************************************************
 * File Name	: kb.c
 * Project	: Keycard
 * Date		: 2014/01/09
 * Version	: 1.0
 * Target MCU	: ATMEGA16U2
 * Tool Chain	: Atmel AVR Studio 4.19 730, avr-gcc, avr-libc
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * References	:
 * Device	: TechKeys Business Card
 *
 * Release Notes:
 *
 * Pinout ATmega16U2
 * =================
 *
 * PB0 --               PC0 --           PD0 Conn 3
 * PB1 System LED       PC1 --           PD1 --
 * PB2 Right LED        PC2 Conn 2       PD2 --
 * PB3 Middle LED                        PD3 Conn 5
 * PB4 Left LED         PC4 --           PD4 Conn 6
 * PB5 Right Key        PC5 --           PD5 Conn 7
 * PB6 Middle Key       PC6 --           PD6 Conn 4
 * PB7 Left Key         PC7 --           PD7 --      
 *
 * $Id$
 ******************************************************************************/

#include <includes.h>
#include "usb_cfg.h"
#include <u_des_kb.h>
#include <u_hid_kb.h>
#include <kbusages.h>
#include "kb.h"

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

#define	DDO( b )	_B1(b)		/* data direction: out */
#define	DDI( b )	_B0(b)		/* data direction: in  */
#define	PU1( b )	_B1(b)		/* pull up on (if DDI) */
#define	PU0( b )	_B0(b)		/* pull up off */

// Port B

#define PBPU		(PU1( PB7) | PU1( PB6) | PU1( PB5) | _B1( PB4) | \
                     _B1( PB3) | _B1( PB2) | _B1( PB1) | PU1( PB0))
#define DDB		(DDI(DDB7) | DDI(DDB6) | DDI(DDB5) | DDO(DDB4) | \
                 DDO(DDB3) | DDO(DDB2) | DDO(DDB1) | DDI(DDB0))
// Port C

#define PCPU		(PU1( PC7) | PU1( PC6) | PU1( PC5) | PU1( PC4) | \
                     PU1( PC2) | PU1( PC1) | PU1( PC0))
#define DDC		(DDI(DDC7) | DDI(DDC6) | DDI(DDC5) | DDI(DDC4) | \
                 DDI(DDC2) | DDI(DDC1) | DDI(DDC0))
// Port D

#define PDPU		(PU1( PD7) | PU1( PD6) | PU1( PD5) | PU1( PD4) | \
                     PU1( PD3) | PU1( PD2) | PU1( PD1) | PU1( PD0))
#define DDD		(DDI(DDD7) | DDI(DDD6) | DDI(DDD5) | DDI(DDD4) | \
                 DDI(DDD3) | DDI(DDD2) | DDI(DDD1) | DDI(DDD0))

//------------------------------------------------------------------------------
// 3% Key & LED definitions

#define	KEY_IN_P		PINB

#define	DEBOUNCE_MS		4	/* Cherry says 5ms debounce time, this is 2ms */

// Number of keys on the board, 3 on the TechKeys Business Card, each w/ a LED

#define	KEY_COUNT		3
#define	LED_COUNT		3

// Key bits and masks

#define bKEY_RGT		0			/* PB5 */
#define bKEY_MID		1			/* PB6 */
#define bKEY_LFT		2			/* PB7 */

#define	mKEYS			0b00000111
#define	mKEYS_P			0b11100000

// LED bits and masks

#define	bLED_RGT		PB2
#define	bLED_MID		PB3
#define	bLED_LFT		PB4

#define	mLED_RGT		_BV(bLED_RGT)
#define	mLED_MID		_BV(bLED_MID)
#define	mLED_LFT		_BV(bLED_LFT)

#define	mLEDS			(mLED_RGT | mLED_MID | mLED_LFT)

//------------------------------------------------------------------------------

// Bit definitions for the keyboard LED OUT report

#define	bLED_NUM		0
#define	bLED_CAPS		1
#define	bLED_SCR		2
#define	bLED_COMPOSE		3
#define	bLED_KANA		4

#define	bLED_PWR		5	/* Not implemented yet */
#define	bLED_SHFT		6	/* Not implemented yet */

#define	bLED_MUTE		0
#define	bLED_CAON		1

// Bit definitions for the keyboard modifiers in the IN report

#define	bLFT_CTRL		0
#define	bLFT_SHFT		1
#define	bLFT_ALT		2
#define	bLFT_GUI		3
#define	bRGT_CTRL		4
#define	bRGT_SHFT		5
#define	bRGT_ALT		6
#define	bRGT_GUI		7

//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

static uint8_t
demo_off,				// No demo mode flag (can be irritating..)
    led_r_pat,				// Right LED PWM pattern
    led_m_pat,				// Middle LED PWM pattern
    led_l_pat ;				// Left LED PWM pattern

static uint8_t
    led_active,
    key_active;


//------------------------------------------------------------------------------
//******************************************************************************
//------------------------------------------------------------------------------

// Delay using T1 (16bit) as timing reference

static void Delay_T1 ( uint16_t time )
{
    set_bit( GTCCR, PSRSYNC ) ;		// reset prescaler

    ResetTM( 1, time ) ;		// set timer

    for ( ; ! TMexp( 1 ) ; )		// wait until timer expired
        ;
}

//------------------------------------------------------------------------------

// Reboot into bootloader.

static void FA_NORETURN( bootloader ) ( void )
{
    clr_bit( TIMSK1, OCIE1A ) ;

    UDCON  = _B1(DETACH) ;		// Detach from USB

    cli() ;				// Disable interrupts

    USBCON = _B0(USBE) | _B1(FRZCLK) ;	// Stop USB module
    clr_bit( PLLCSR, PLLE ) ;		// Stop PLL

    wdt_disable() ;			// Leash watchdog

    // Disable peripherals

    DDRB   = 0 ; PORTB  = 0 ;
    DDRC   = 0 ; PORTC  = 0 ;
    DDRD   = 0 ; PORTD  = 0 ;

    TCCR1B = 0 ;
    OCR1A  = 0 ;

    // 15ms to make sure detach is recognized

    _delay_loop_2( (uint16_t)MS2TM( 15, 4 ) ) ;

    jmp_bootloader() ;			// Off we go
}

//------------------------------------------------------------------------------

// Set the PWM pattern (ie. dim level) for the specified LED's

static void set_leds ( uint8_t leds, uint8_t lvl )
{
    static const uint8_t
        pattern[] PROGMEM =
        {
            0b00000000,	//   0%
            0b00000001,	//  12.5%
            0b10001000,	//  25%
            0b01001010,	//  37.5%
            0b01010101,	//  50%
            0b11011010,	//  62.5%
            0b01110111,	//  75
            0b11111110,	//  82.5%
            0b11111111	// 100%
        } ;

    //    if ( lvl >= ARRSZ( pattern ) )		// Should never happen
    //	return ;

    if ( leds & mLED_RGT )
        led_r_pat = pgm_read_byte( pattern + lvl ) ;

    if ( leds & mLED_MID )
        led_m_pat = pgm_read_byte( pattern + lvl ) ;

    if ( leds & mLED_LFT )
        led_l_pat = pgm_read_byte( pattern + lvl ) ;
}
//------------------------------------------------------------------------------

// Maintain LED PWM

void led_pwm ( void )
{
    uint8_t
        pat ;

    if ( (pat = led_r_pat) & 1 )
        LED_on( bLED_RGT ) ;
    else
        LED_off( bLED_RGT ) ;

    led_r_pat = ror8( pat ) ;

    if ( (pat = led_m_pat) & 1 )
        LED_on( bLED_MID ) ;
    else
        LED_off( bLED_MID ) ;

    led_m_pat = ror8( pat ) ;

    if ( (pat = led_l_pat) & 1 )
        LED_on( bLED_LFT ) ;
    else
        LED_off( bLED_LFT ) ;

    led_l_pat = ror8( pat ) ;
}

//------------------------------------------------------------------------------

#define	DEMO_TIMER		10		/* Change LED's every 30ms */

static void led_demo ( uint8_t reset )
{
    static const uint8_t
        gradual[] PROGMEM =
        {
            4,4,4,4,4,4,4,4,4,4, // len: DEMO_TIMER
            3,3,3,3,3,3,3,3,3,3, // ^
            2,2,2,2,2,2,2,2,2,2, // ^ 
            1,1,1,1,1,1,1,1,1,1, // ^ 
        } ;

    // I don't know how storing the result of _BV() works
    // so this is a small mapping for keys-to-leds (where keys
    // is bKEY_n ... a constant)
    static const uint8_t
        mapping[] PROGMEM = 
        {
            mLED_RGT,
            mLED_MID, 
            mLED_LFT, 
        };
    
    static uint8_t
        timer,
        idx;

    if ( reset || !led_active)
    {
        timer = 0 ;
        idx   = 0 ;

        set_leds( mLEDS, 0 ) ;

        return ;
    }


    if ( ! timer-- )
    {
        timer = DEMO_TIMER ;

        set_leds(pgm_read_byte(mapping + key_active),
                 pgm_read_byte(gradual + idx));

        if ( ++idx >= ARRSZ( gradual ) ) {
            idx = 0 ;
            set_leds(pgm_read_byte(mapping + key_active), 0);
            led_active = 0;
        }
    }
}

//------------------------------------------------------------------------------

// Maintain Sleep LED. Note: called every 1ms (system clock throttled)

#define	SLEEP_OFF_DELAY		1				/* 1ms on */
#define	SLEEP_ON_DELAY		(3000 - SLEEP_OFF_DELAY)	/* once w/in 3s */

void sleep_led ( uint8_t reset )
{
    static uint16_t
        timer ;

    if ( reset )
    {
        LED_off( bLED_SYS ) ;
        timer = 0 ;

        return ;
    }

    if ( ! timer-- )			// Timer expired
    {
        if ( LED_sts( bLED_SYS ) )	// LED on ?
        {
            LED_off( bLED_SYS ) ;
            timer = SLEEP_ON_DELAY ;
        }
        else
        {
            LED_on( bLED_SYS ) ;
            timer = SLEEP_OFF_DELAY ;
        }
    }
}

//------------------------------------------------------------------------------

// Maintain LED's

#define	DEMO_DELAY		5000		/* Start demo after 5s w/o LEDs */
#define	BOOT_DELAY		5000		/* Start boot-loader delay, 5s */
#define	LED_TIMER		(1 * 2)		/* 500us * 2 */

void maint_leds ( uint8_t reset )
{
    static uint16_t
        bload,				// Bootloader call timer
        demo ;				// Demo start timer

    static uint8_t
        timer ;

    uint8_t
        mLeds ;

    if ( reset )
    {
        bload = BOOT_DELAY ;
        demo  = DEMO_DELAY ;
        timer = 0 ;

        set_leds( mLEDS, 0 ) ;
        led_pwm() ;			// Make sure LED's are off

        return ;
    }

    if ( ! timer-- )			// Maintain LED's every .5ms * timer
    {
        timer = LED_TIMER ;

        if ( (kbd_leds & (_BV(bLED_NUM) | _BV(bLED_CAPS) | _BV(bLED_SCR))) )
        {					// LOCK LED(s) active
            if ( demo != DEMO_DELAY )		// Demo mode was possibly on
            {
                bload = BOOT_DELAY ;
                demo  = DEMO_DELAY ;		// Reset demo & bload timers
            }
            // Scroll- + Num-Lock on
            if ( kbd_leds == (_BV(bLED_NUM) | _BV(bLED_SCR)) )
                if ( ! --bload )
                    bootloader() ;

            mLeds = 0 ;

            if ( kbd_leds & _BV(bLED_NUM) )
                mLeds |= mLED_LFT ;

            if ( kbd_leds & _BV(bLED_CAPS) )
                mLeds |= mLED_MID ;

            if ( kbd_leds & _BV(bLED_SCR) )
                mLeds |= mLED_RGT ;

            set_leds( mLeds, 1 ) ;		// 1 is lowest dim level
            set_leds( mLeds ^ mLEDS, 0 ) ;
        }
        else				// No LED active
        {
            if ( demo )			// Waiting for demo display
            {
                if ( demo_off )
                {
                    LED_on( bLED_SYS ) ;

                    if ( demo == DEMO_DELAY )	// First run after LED's off
                    {
                        set_leds( mLEDS, 0 ) ;	// Set all LED's off
                        --demo ;
                    }

                    LED_off( bLED_SYS ) ;
                }
                else
                {
                    if ( demo == DEMO_DELAY )	// First run after LED's off
                        set_leds( mLEDS, 0 ) ;	// Set all LED's off

                    if ( ! --demo )		// Demo enable on next run
                        led_demo( SIG_RESET ) ;	// Reset demo routine
                }
            }
            else
                led_demo( SIG_MAINT ) ;
        }
    }
}

//------------------------------------------------------------------------------

// Check for any key down in sleep mode

uint8_t check_keys ( uint8_t reset )
{
    static uint8_t
        db ;				// Debounce timer

    uint8_t
        key,
        ret = FALSE ;

    if ( reset )
    {
        db = 0 ;			// Not debouncing

        return ( FALSE ) ;
    }

    key = KEY_IN_P & mKEYS_P ;

    if ( ! db )				// Not debouncing
    {
        if ( key != mKEYS_P )		// Any key down
            db = DEBOUNCE_MS ;		// start debouncing
    }
    else				// Debouncing
    {
        if ( key == mKEYS_P )		// No key down anymore
            db = 0 ;			// false alarm
        else				// Any key still own
            if ( ! --db )			// Debounce period expired
                ret = TRUE ;
    }

    return ( ret ) ;
}

//------------------------------------------------------------------------------

// Build the keyboard report

static uint8_t build_kbd_report ( uint8_t keys )
{
#define REP_M		0x0000		/* Macro */
#define REP_KB	0x1000		/* Keyboard */
#define REP_SC	0x2000		/* System Control */
#define REP_CC	0x4000		/* Consumer Control */

#define U_Macro_1	0x0010
#define U_Macro_2	0x0020
#define U_Macro_3	0x0030

    static const uint16_t
        key_usages[KEY_COUNT] PROGMEM =	// Key-to-usage matrix
        {
            REP_M  | U_Macro_3,		// PB5 Right key
            REP_M  | U_Macro_2,		// PB6 Middle key
            REP_M  | U_Macro_1,		// PB7 Left key
        },
        key_usagesS[KEY_COUNT] PROGMEM = // Key-to-usage matrix if SCR lock is set
        {
            REP_CC | U_NextTrack,	// PB5 Right key
            REP_CC | U_PlayPause,	// PB6 Middle key
            REP_CC | U_PrevTrack	// PB7 Left key
        },
        key_usagesN[KEY_COUNT] PROGMEM = // Key-to-usage matrix if NUM lock is set
        {
            REP_CC | U_VolUp,		// PB5 Right key
            REP_CC | U_Mute,		// PB6 Middle key
            REP_CC | U_VolDwn,		// PB7 Left key
        }
    ;

    static kb_report_t
        last_kbd_rep ;

    static uint16_t
        last_ctrl_rep1,
        last_ctrl_rep2 ;

    uint16_t
        usage ;

    uint8_t
        i,
        ki = 0,
        ret = 0 ;

    const uint16_t
        *pu ;

    //////// Select key-to-usage table

    //////if ( kbd_leds == _BV(bLED_NUM) )
    //////    pu = key_usagesN ;
    //////else
    //////    if ( kbd_leds == _BV(bLED_SCR) )
    //////        pu = key_usagesS;
    //////    else
    //////        pu = key_usages ;

    (void)key_usages;
    (void)key_usagesN;
    pu = key_usagesS;

    if ( ! keys )			// All keys down, toggle demo mode
        demo_off = ! demo_off ;

    ctrl_report1.key = U_None ;		// Clear System Control Report
    ctrl_report2.key = U_None ;		// Clear Consumer Control Report

    kbd_report.mod   = 0 ;		// Clear modifiers

    for ( i = 0 ; i < KEY_COUNT ; ++i, keys >>= 1 )
    {						// Assemble keyboard report
        if ( ! (keys & 1) )			// Key down
        {
            usage = pgm_read_word( pu + i ) ;	// get its usage
            led_active = 1;
            key_active = i;

            if ( (usage & REP_KB) )		// Keyboard usage
            {
                if ( (usage & 0xF0) == 0xE0 )	// Modifier
                    kbd_report.mod |= _BV( usage & 0x0F ) ;	// Magic.
                else
                    kbd_report.keys[ki++] = (usage & 0xFF) ;
            }
            else
                if ( (usage & REP_SC) )		// System Control usage
                    ctrl_report1.key = (usage & 0x0FFF) ;
                else
                    if ( (usage & REP_CC) )		// Consumer Control usage
                        ctrl_report2.key = (usage & 0x0FFF) ;
                    else				// Macro
                        ret |= (usage & 0x00F0) ;
        }
    }

    // Note: assuming NUM_KEYS <= ARRSZ( kbd_report.keys )

    for ( ; ki < ARRSZ( kbd_report.keys ) ; )
        kbd_report.keys[ki++] = U_None ;

    // Check if a report changed and copy reports

    if ( last_kbd_rep.mod != kbd_report.mod )
    {
        last_kbd_rep.mod = kbd_report.mod ;
        ret |= REP0_CHG ;
    }

    for ( ki = ARRSZ( kbd_report.keys ) ; ki-- ; )
        if ( last_kbd_rep.keys[ki] != kbd_report.keys[ki] )
        {
            last_kbd_rep.keys[ki] = kbd_report.keys[ki] ;
            ret |= REP0_CHG ;
        }

    if ( last_ctrl_rep1 != ctrl_report1.key )
    {
        last_ctrl_rep1 = ctrl_report1.key ;
        ret |= REP1_CHG ;
    }

    if ( last_ctrl_rep2 != ctrl_report2.key )
    {
        last_ctrl_rep2 = ctrl_report2.key ;
        ret |= REP2_CHG ;
    }

    return ( ret ) ;
}

//------------------------------------------------------------------------------

// Macro playback

#define	M_DELAY			(USB_EP_POLL_INTERVAL_KBD * 2)

#define	M_ACTIVE		0x80

static uint8_t play_macro ( uint8_t idx )
{
    static const uint8_t
        macro_1[] PROGMEM =		// http://www.techkeys.us
        {
            U_LftCtrl, U_L, U_None,
            U_H, U_T, U_None, U_T, U_P, U_LftShift, U_SemiCol, U_Slash, U_None, U_Slash,
            U_W, U_None, U_W, U_None, U_W, U_Dot,
            U_T, U_E, U_C, U_H, U_K, U_E, U_Y, U_S, U_Dot, U_U, U_S,
            U_Enter, U_None
        },
        macro_2[] PROGMEM =		// mailto:info@techkeys.us
        {
            U_LftCtrl, U_L, U_None,
            U_M, U_A, U_I, U_L, U_T, U_O, U_LftShift, U_SemiCol,
            U_I, U_N, U_F, U_O, U_LftShift, U_2,
            U_T, U_E, U_C, U_H, U_K, U_E, U_Y, U_S, U_Dot, U_U, U_S,
            U_Enter, U_None
        },
        macro_3[] PROGMEM =		// http://www.facebook.com/techkeysus
        {
            U_LftCtrl, U_L, U_None,
            U_H, U_T, U_None, U_T, U_P, U_LftShift, U_SemiCol, U_Slash, U_None, U_Slash,
            U_W, U_None, U_W, U_None, U_W, U_Dot,
            U_F, U_A, U_C, U_E, U_B, U_O, U_None, U_O, U_K, U_Dot, U_C, U_O, U_M,
            U_Slash, U_T, U_E, U_C, U_H, U_K, U_E, U_Y, U_S, U_U, U_S,
            U_Enter, U_None
        } ;

    static const struct
    {
        uint8_t       sz ;		// Macro size, limited to 255 bytes for now
        const uint8_t *macro ;	// -> macro
    }
    macros[] PROGMEM =
    {
        { sizeof( macro_1 ), macro_1 },
        { sizeof( macro_2 ), macro_2 },
        { sizeof( macro_3 ), macro_3 }
    } ;

    static uint8_t
        timer,				// Delay timer
        sz,				// Usages left to send
        *pm ;				// -> next usage to send

    uint8_t
        usage ;

    if ( idx )				// Initialize macro playback
    {
        //	if ( idx > ARRSZ( macros ) )	// Should never happen
        //	    return ( FALSE ) ;

        sz = pgm_read_byte( &macros[idx - 1].sz ) ;
        pm = VP( pgm_read_word( &macros[idx - 1].macro ) ) ;

        timer = 0 ;

        kbd_report.mod = 0 ;

        for ( usage = 0 ; usage < ARRSZ( kbd_report.keys ) ; ++usage )
            kbd_report.keys[usage] = U_None ;

        return ( M_ACTIVE | REP0_CHG ) ;// Signal active macro
    }

    if ( ! timer-- )			// Time to send the next usage
    {
        timer = M_DELAY ;		// reset timer

        kbd_report.mod = 0 ;

        do
        {
            usage = pgm_read_byte( pm ) ;
            ++pm ;
            --sz ;

            if ( (usage & 0xF0) == 0xE0 )	// Modifier
                kbd_report.mod |= _BV( usage & 0x0F ) ;
        }
        while ( (usage & 0xF0) == 0xE0 ) ;

        kbd_report.keys[0] = usage ;

        if ( ! sz )
            return ( REP0_CHG ) ;	// Playback done

        return ( M_ACTIVE | REP0_CHG ) ;// New report and macro still active
    }

    return ( M_ACTIVE ) ;		// Macro still active, no report
}

//------------------------------------------------------------------------------

// Read and maintain key matrix

uint8_t read_matrix ( uint8_t reset )
{
    static uint8_t
        macro,				// Flag to signal macro plaayback in progress
        keys ;				// 0 == all keys on ! Call reset before 1st use.

    static int8_t
        db_c[KEY_COUNT] ;		// Debounce counters

    uint8_t
        i,
        ret = FALSE,
        key,
        key_msk ;

    int8_t
        *db = db_c ;

    if ( reset )
    {
        for ( i = KEY_COUNT ; i-- ; )		// Clear debounce counters
            *db++ = 0 ;

        keys = mKEYS ;				// All keys off

        return ( build_kbd_report( keys ) ) ;	// Clear reports
    }

    if ( macro )				// Playing macro
    {
        ret = play_macro( 0 ) ;

        macro = ret & M_ACTIVE ;
        ret  &= 0x0F ;

        return ( ret ) ;
    }

    for ( key_msk = 1, i = KEY_COUNT ; i-- ; ++db, key_msk <<= 1 )
    {
        key = KEY_IN_P & mKEYS_P ;		// Active inputs: 0b11100000

        __asm__ __volatile__
            (
             "swap   %0" "\n\t"
             "lsr    %0" "\n\t"
             : "+r" (key) : "0" (key)
            ) ;					// now 0b00000111

        key &= key_msk ;

        if ( ! *db )				// Not debouncing ATM
        {
            if ( key )				// Key off
            {
                if ( ! (keys & key_msk) )	// Key was on, start debouncing
                    *db = -DEBOUNCE_MS ;
            }
            else				// Key on
            {
                if (   (keys & key_msk) )	// Key was off, start debouncing
                    *db =  DEBOUNCE_MS ;
            }
        }
        else
        {
            if ( key )
                ++*db ;				// Debouncing for off
            else
                --*db ;				// Debouncing for on

            if ( ! *db )			// Debouncing done, key changed
            {
                if ( key )
                    keys |=  key_msk ;		// Key now off
                else
                    keys &= ~key_msk ;		// Key now on

                ret = TRUE ;			// Signal change
            }
            else
                if ( (*db > 0 && *db >= 2 *  DEBOUNCE_MS) ||
                     (*db < 0 && *db <= 2 * -DEBOUNCE_MS) )
                    *db = 0 ;			// False alarm
        }
    }

    if ( ret )				// Key(s) changed
        if ( (ret = build_kbd_report( keys )) & 0xF0 )
        {				// Initialize macro playback
            ret = play_macro( (ret & 0xF0) >> 4 ) ;

            macro = ret & M_ACTIVE ;
            ret  &= 0x0F ;
        }

    return ( ret ) ;
}

//------------------------------------------------------------------------------

// Initialize the hardware

void hw_init ( void )
{
    // Power down..

    ACSR  = _BV(ACD) ;				// analog comp
    PRR0  =					// T0/1, SPI
        _BV(PRTIM0) |
        //	    _BV(PRTIM1) |			// We need T1
        _BV(PRSPI) ;
    PRR1  =					// USART1
        _BV(PRUSART1) ;

    clock_prescale_set( clock_div_1 ) ;		// Set clock divider to 1, full speed

    // Initialize ports

    DDRB  = DDB ; PORTB = PBPU ;
    DDRC  = DDC ; PORTC = PCPU ;
    DDRD  = DDD ; PORTD = PDPU ;

    SetTMPS( 1, 1024 ) ;			// Set T1 prescaler to / 1024 for ms delay

    Delay_T1( -MS2TM( 200, 1024 ) ) ;		// Wait 200ms to settle in

    // T1 is going to be used for a 500us timebase

    SetTMPS( 1, 8 ) ;				// Set prescaler to / 8

    OCR1A = US2TM( 500, 8 ) ;			// Set output compare reg A to 500us

    set_bit( TCCR1B, WGM12 ) ;			// Set CTC mode
}

//------------------------------------------------------------------------------
