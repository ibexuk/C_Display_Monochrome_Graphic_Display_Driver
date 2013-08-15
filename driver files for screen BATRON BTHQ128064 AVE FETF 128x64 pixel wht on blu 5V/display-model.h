/*
IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) 2013, IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Project Name:		MONOCHROME DISPLAY DRIVER



#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )


//****************************************************************************************
//****************************************************************************************
//  Project Name:		MONOCHROME DISPLAY DRIVER
//						SCREEN MODEL C CODE FILE
//  Copyright:			EMBEDDED-CODE.COM
//
//<INSERT LICENCE BLOCK HERE>
//****************************************************************************************
//****************************************************************************************

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//---------- THIS FILE CONTAINS SCREEN MODEL SPECIFIC FUNCTIONS FOR	 ----------
//---------- SCREEN:	BATRON BTHQ128064 AVE FETF						----------
//---------- Resolution:	128 x 64 pixel								----------
//---------- Colour:		White on Blue								----------
//---------- Driver IC:		SED1565D0B									----------
//---------- Interface:		Parallel									----------
//---------- Voltage:		5V											----------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


/*
###############################################
##### TO USE THIS DRIVER IN A NEW PROJECT #####
###############################################
- Check this header file and the display.h header file for defines to setup and any usage notes
- Configure the control pins as outputs in your applications initialisation.

USING THE SCREEN IN LANDSCAPE WITH THE RIBBON CABLE DOWNWARDS:
Bitmap converter rotation setting: 90
Screen coordinates when calling driver functions:
  0,0               127,0

  0,63              127,63
*/


//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef DISPLAY_MODEL_C_INIT				//(Include this section only once for each source file that includes this header file)
#define	DISPLAY_MODEL_C_INIT


//----------------------------------------------
//----- DEFINE TARGET COMPILER & PROCESSOR -----
//----------------------------------------------
//(ONLY 1 SHOULD BE INCLUDED, COMMENT OUT OTHERS)
#define	DISPLAY_USING_MICROCHIP_C18_COMPILER
//#define	DISPLAY_USING_MICROCHIP_C30_COMPILER
//#define	DISPLAY_USING_MICROCHIP_C32_COMPILER
//<< add other compiler types here


//-------------------------
//----- DISPLAY SETUP -----
//-------------------------
//#define	DISPLAY_USE_LOCAL_RAM_BUFFER			//Include to optionally use a ram array to hold a copy of the bitmap output to speed up display and for displays
													//where you cannot read back display data (e.g. many serial displays).  Comment out if memory not available

#define	DISPLAY_WIDTH_PIXELS			128				//Width is the X coordinate
#define	DISPLAY_HEIGHT_PIXELS			64				//Height is the Y coordinate and is used for the page address (divide this value by 8 to give the number of pages)
#define	DISPLAY_WIDTH_START_OFFSET		4				//Offset to first actual display position (0 if not applicable)
#define	DISPLAY_DEFAULT_CONTRAST		30


//------------------------
//----- USER OPTIONS -----
//------------------------
//#define	DISPLAY_USE_LOCAL_RAM_BUFFER			//Include to optionally use a ram array to hold a copy of the display bitmap output to speed up display and for displays
													//where you cannot read back display data (e.g. many serial displays).  Comment out if memory not available

//#define	ORIENTATION_IS_0							//Only one of these should be defined
#define	ORIENTATION_IS_90							//Only one of these should be defined
//#define	ORIENTATION_IS_180							//Only one of these should be defined
//#define	ORIENTATION_IS_270							//Only one of these should be defined

#ifdef ORIENTATION_IS_180
	#define	INVERT_Y_AXIS_COORDINATES				//Comment out normally, include if you need to reverse the direction of display on the Y axis.
#endif


#ifdef DISPLAY_USING_MICROCHIP_C18_COMPILER
//########################################
//########################################
//##### USING MICROCHIP C18 COMPILER #####
//########################################
//########################################

//----------------------
//----- IO DEFINES -----
//----------------------
//PORTS:-
#define DISPLAY_DATA_BUS_IP				PORTD				//D7:0 data bus read register
#define DISPLAY_DATA_BUS_OP(data)		LATD = data			//D7:0 data bus write register (same as read register if microcontroller / processor doesn't have separate registers for input and output)
#define DISPLAY_DATA_BUS_TRIS(state)	TRISD = state		//D7:0 data bus input / output register (bit state 0 = output, 1 = input)

//CONTROL PINS:-
#define DISPLAY_RW(state)		LATCbits.LATC1 = state		//0 = WRITE (in 8080 mode)
#define DISPLAY_RD(state)		LATCbits.LATC0 = state		//0 = read (in 8080 mode)
#define DISPLAY_A0(state)		LATEbits.LATE0 = state		//0 = Command, 1=Data
#define DISPLAY_CS1(state)		LATEbits.LATE1 = state		//0 =select controller
#define DISPLAY_RESET(state)	LATEbits.LATE2 = state		//0 =select controller

//-----------------------------------
//----- BUS ACCESS DELAY DEFINE -----
//-----------------------------------
//Depending on the speed of your processor and bus you may need to use a delay when reading and writing to and from the DISPLAY controller.
//Use this define to do this.
#define	DISPLAY_BUS_ACCESS_DELAY	Nop() 				//('Nop();' is a single cycle null instruction for the C18 compiler, include multiple times if required)


//###############################################
//###############################################
//##### END OF USING MICROCHIP C18 COMPILER #####
//###############################################
//###############################################
#endif		//#ifdef DISPLAY_USING_MICROCHIP_C18_COMPILER



#ifdef DISPLAY_USING_MICROCHIP_C30_COMPILER
//########################################
//########################################
//##### USING MICROCHIP C30 COMPILER #####
//########################################
//########################################


//----------------------
//----- IO DEFINES -----
//----------------------
//PORTS:-
#define DISPLAY_DATA_BUS_IP				(BYTE)(PORTE & 0x00ff)					//D7:0 data bus read register
#define DISPLAY_DATA_BUS_OP(data)		LATE &= 0xff00; LATE |= (WORD)data		//D7:0 data bus write register (same as read register if microcontroller / processor doesn't have separate registers for input and output)
#define DISPLAY_DATA_BUS_TRIS(state)	TRISE &= 0xff00; TRISE |= (WORD)state	//D7:0 data bus input / output register (bit state 0 = output, 1 = input)

//CONTROL PINS:-
#define DISPLAY_RW(state)				_LATD3 = state		//0 = WRITE (in 8080 mode)
#define DISPLAY_RD(state)				_LATD2 = state		//0 = read (in 8080 mode)
#define DISPLAY_A0(state)				_LATD0 = state		//0 = Command, 1=Data
#define DISPLAY_CS1(state)				_LATD1 = state		//0 = select controller
#define	DISPLAY_RESET(state)			_LATF6 = state		//0 = reset controller

//-----------------------------------
//----- BUS ACCESS DELAY DEFINE -----
//-----------------------------------
//Depending on the speed of your processor and bus you may need to use a delay when reading and writing to and from the controller.
//Use this define to do this.
#define	DISPLAY_BUS_ACCESS_DELAY	Nop(); Nop()			//('Nop();' is a single cycle null instruction for the C30 compiler, include multiple times if required)



//###############################################
//###############################################
//##### END OF USING MICROCHIP C30 COMPILER #####
//###############################################
//###############################################
#endif		//#ifdef DISPLAY_USING_MICROCHIP_C30_COMPILER


#ifdef DISPLAY_USING_MICROCHIP_C32_COMPILER
//########################################
//########################################
//##### USING MICROCHIP C32 COMPILER #####
//########################################
//########################################


//----------------------
//----- IO DEFINES -----
//----------------------
//PORTS:-
#define DISPLAY_DATA_BUS_IP				(BYTE)(mPORTERead() & 0x000000ff)											//D7:0 data bus read register
#define DISPLAY_DATA_BUS_OP(data)		mPORTESetBits((DWORD)data); mPORTEClearBits((DWORD)~data)					//D7:0 data bus write register (same as read register if microcontroller / processor doesn't have separate registers for input and output)
#define DISPLAY_DATA_BUS_TRIS(state)	mPORTESetPinsDigitalIn((DWORD)state); mPORTESetPinsDigitalOut((DWORD)~state) //D7:0 data bus input / output register (bit state 0 = output, 1 = input)

//CONTROL PINS:-
#define DISPLAY_RW(state)				(state ? mPORTDSetBits(BIT_3) : mPORTDClearBits(BIT_3))		//0 = WRITE (in 8080 mode)
#define DISPLAY_RD(state)				(state ? mPORTDSetBits(BIT_2) : mPORTDClearBits(BIT_2))		//0 = read (in 8080 mode)
#define DISPLAY_A0(state)				(state ? mPORTDSetBits(BIT_0) : mPORTDClearBits(BIT_0))		//0 = Command, 1=Data
#define DISPLAY_CS1(state)				(state ? mPORTDSetBits(BIT_1) : mPORTDClearBits(BIT_1))		//0 = select controller
#define	DISPLAY_RESET(state)			(state ? mPORTFSetBits(BIT_6) : mPORTFClearBits(BIT_6))		//0 = reset controller

//-----------------------------------
//----- BUS ACCESS DELAY DEFINE -----
//-----------------------------------
//Depending on the speed of your processor and bus you may need to use a delay when reading and writing to and from the controller.
//Use this define to do this.
#define	DISPLAY_BUS_ACCESS_DELAY	Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop()		//('Nop();' is a single cycle null instruction for the C32 compiler, include multiple times if required)


//###############################################
//###############################################
//##### END OF USING MICROCHIP C32 COMPILER #####
//###############################################
//###############################################
#endif		//#ifdef DISPLAY_USING_MICROCHIP_C32_COMPILER




#endif



//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef DISPLAY_MODEL_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------
void display_set_address (WORD x_coord, WORD y_coord_page);
void display_write_command (BYTE data);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
void display_model_initialise(void);
void display_model_set_contrast(BYTE contrast_value);
void display_write_bitmap_byte (BYTE flags, BYTE bitmap_mask, BYTE bitmap_data, WORD x_byte_coord, WORD y_byte_coord);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern void display_model_initialise(void);
extern void display_model_set_contrast(BYTE contrast_value);
extern void display_write_bitmap_byte (BYTE flags, BYTE bitmap_mask, BYTE bitmap_data, WORD x_byte_coord, WORD y_byte_coord);


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef DISPLAY_MODEL_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------



//----- OPTIONAL BUFFER FOR LOCAL COPY OF SCREEN OUTPUT DATA -----
#ifdef DISPLAY_USE_LOCAL_RAM_BUFFER
	#ifdef DISPLAY_USING_MICROCHIP_C18_COMPILER
		#pragma udata display_local_buffer_ram_section	//This is the PIC C18 compiler command to use the specially defined section in the linker script (PIC18 project uses a modified linker script for this)
	#endif											//(C18 large array requirement to use a special big section of ram defined in the linker script)

	#if (DISPLAY_HEIGHT_PIXELS & 0x0007)
		BYTE display_copy_buffer[DISPLAY_WIDTH_PIXELS][(DISPLAY_HEIGHT_PIXELS >> 3) + 1)];		//DISPLAY_WIDTH_PIXELS is not a multiple of 8 so need to add 1 for remainder
	#else
		BYTE display_copy_buffer[DISPLAY_WIDTH_PIXELS][(DISPLAY_HEIGHT_PIXELS >> 3)];			//DISPLAY_WIDTH_PIXELS is a multiple of 8
	#endif

	#ifdef DISPLAY_USING_MICROCHIP_C18_COMPILER
		#pragma udata
	#endif
#endif


//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------


#endif


