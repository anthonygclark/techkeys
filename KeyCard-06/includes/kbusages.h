/*******************************************************************************
 * File Name	: kbusages.h
 * Project	: Keyboard
 * Date		: 2013/11/16
 * Version	: 1.0
 * Target MCU	: AVR
 * Tool Chain	: Atmel AVR Studio 4.19 730 / gcc 4.7.0 / avr-libc 1.8.0
 * Author	: "Detlef Mueller" <detlef@gmail.com>
 * Release Notes:
 *
 * $Id$
 *
 * $Log$
 ******************************************************************************/

#ifndef	_KBUSAGES_H_
#define	_KBUSAGES_H_

//------------------------------------------------------------------------------

// Keyboard Usage numbers, see HID Usage Tables v1.12,
// chap. 10 Keyboard/Keypad Page (0x07), pg. 53

#define	U_None			0x00
#define	U_ErrorRollOver		0x01

#define	U_A			0x04
#define	U_B			0x05
#define	U_C			0x06
#define	U_D			0x07
#define	U_E			0x08
#define	U_F			0x09
#define	U_G			0x0A
#define	U_H			0x0B
#define	U_I			0x0C
#define	U_J			0x0D
#define	U_K			0x0E
#define	U_L			0x0F
#define	U_M			0x10
#define	U_N			0x11
#define	U_O			0x12
#define	U_P			0x13
#define	U_Q			0x14
#define	U_R			0x15
#define	U_S			0x16
#define	U_T			0x17
#define	U_U			0x18
#define	U_V			0x19
#define	U_W			0x1A
#define	U_X			0x1B
#define	U_Y			0x1C
#define	U_Z			0x1D

#define	U_1			0x1E
#define	U_2			0x1F
#define	U_3			0x20
#define	U_4			0x21
#define	U_5			0x22
#define	U_6			0x23
#define	U_7			0x24
#define	U_8			0x25
#define	U_9			0x26
#define	U_0			0x27

#define	U_Enter			0x28
#define	U_Esc			0x29
#define	U_Backspace		0x2A
#define	U_Tab			0x2B
#define	U_Space			0x2C

#define	U_Minus			0x2D	/* - */
#define	U_Equal			0x2E	/* = */
#define	U_SqBrOp		0x2F	/* [ */
#define	U_SqBrCl		0x30	/* ] */
#define	U_Backslash		0x31	/* \ */

#define	U_SemiCol		0x33	/* ; */
#define	U_Quote			0x34	/* ' */
#define	U_GraveAcc		0x35	/* ` */
#define	U_Comma			0x36	/* , */
#define	U_Dot			0x37	/* . */
#define	U_Slash			0x38	/* / */

#define	U_CapsLock		0x39

#define	U_F1			0x3A
#define	U_F2			0x3B
#define	U_F3			0x3C
#define	U_F4			0x3D
#define	U_F5			0x3E
#define	U_F6			0x3F
#define	U_F7			0x40
#define	U_F8			0x41
#define	U_F9			0x42
#define	U_F10			0x43
#define	U_F11			0x44
#define	U_F12			0x45

#define	U_PrtScr		0x46
#define	U_ScrollLock		0x47
#define	U_Pause			0x48
#define	U_Insert		0x49
#define	U_Home			0x4A
#define	U_PgUp			0x4B
#define	U_Del			0x4C
#define	U_End			0x4D
#define	U_PgDwn			0x4E
#define	U_RgtArrow		0x4F
#define	U_LftArrow		0x50
#define	U_DwnArrow		0x51
#define	U_UpArrow		0x52

#define	U_NumLock		0x53
#define	U_NumDiv		0x54
#define	U_NumMul		0x55
#define	U_NumMinus		0x56
#define	U_NumPlus		0x57
#define	U_NumEnter		0x58
#define	U_Num1			0x59
#define	U_Num2			0x5A
#define	U_Num3			0x5B
#define	U_Num4			0x5C
#define	U_Num5			0x5D
#define	U_Num6			0x5E
#define	U_Num7			0x5F
#define	U_Num8			0x60
#define	U_Num9			0x61
#define	U_Num0			0x62
#define	U_NumDot		0x63

#define	U_App			0x65

#define	U_LftCtrl		0xE0
#define	U_LftShift		0xE1
#define	U_LftAlt		0xE2
#define	U_LftGui		0xE3
#define	U_RgtCtrl		0xE4
#define	U_RgtShift		0xE5
#define	U_RgtAlt		0xE6
#define	U_RgtGui		0xE7

// System Control Usage numbers, see HID Usage Tables v1.12,
// chap. 4 Generic Desktop Page (0x01), pg. 27

#define	U_System_Power_Down	0x0081
#define	U_System_Sleep		0x0082
#define	U_System_Wake_up	0x0083

// Consumer Usage numbers, see HID Usage Tables v1.12,
// chap. 15 Consumer Page (0x0C), pg. 75

// Media control

#define	U_NextTrack		0x00B5
#define	U_PrevTrack		0x00B6
#define	U_Stop			0x00B7

#define	U_StopEject		0x00CC
#define	U_PlayPause		0x00CD

// Audio

#define	U_Mute			0x00E2
#define	U_VolUp			0x00E9
#define	U_VolDwn		0x00EA

// App launch

#define	U_AL_CC_Config		0x0183
#define	U_AL_Email		0x018A
#define	U_AL_Calculator		0x0192
#define	U_AL_LocalBrowser	0x0194
#define	U_AL_NetBrowser		0x0195
#define	U_AL_WebBrowser		0x0196

// App control

#define	U_AC_Search		0x0221
#define	U_AC_Home		0x0223
#define	U_AC_Back		0x0224
#define	U_AC_Fwd		0x0225
#define	U_AC_Stop		0x0226
#define	U_AC_Refresh		0x0227
#define	U_AC_Bookmarks		0x022A

//------------------------------------------------------------------------------

#endif
