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
//DRIVER C CODE HEADER FILE


#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )



//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef DISPLAY_C_INIT				//(Include this section only once for each source file that includes this header file)
#define	DISPLAY_C_INIT




//-------------------------------
//----- BITMAP FILE DEFINES -----
//-------------------------------

//----- USING PROGRAM MEMORY TO STORE BITMAPS -----
//(Must be commented out if using external flash memory)
#include "bitmaps\output\cbitmaps.h"

//----- USING EXTERNAL FLASH MEMORY TO STORE BITMAPS -----
//(Must all be commented out if using program memory)
/*
#include "bitmaps\output\bbitmaps.h"
#define	DISPLAY_USING_FLASH_MEMORY
#define	DISPLAY_FLASH_READ_FUNCTION 		flash_read			//The function to call to read bitmap data from the
																//flash memory (provided by your application)
#define	DISPLAY_FLASH_MEMORY_START_ADDRESS	0x0000				//Offset to start of bitmap file in the flash memory
*/


//---------------------------
//----- GENERAL OPTIONS -----
//---------------------------
#define	INCLUDE_DISPLAY_BYTE_TEST_SEQUENCE_FUNCTION			//Comment out to remove this function to save program space


				

//----------------------------------
//----- DISPLAY BITMAP OPTIONS -----
//----------------------------------
//(May be used when calling display functions)
#define	DISPLAY_BITMAP_INVERT_PIXELS_OFF	0x0000
#define	DISPLAY_BITMAP_INVERT_PIXELS_ON		0x0080

//----------------------------------
//----- DISPLAY STRING OPTIONS -----
//----------------------------------
//(May be used when calling display functions)
#define	DISPLAY_STRING_INVERT_PIXELS_OFF	0x0000
#define	DISPLAY_STRING_INVERT_PIXELS_ON		0x0080
#define	DISPLAY_STRING_ON_X_AXIS			0x0000
#define	DISPLAY_STRING_ON_Y_AXIS			0x0100
#define	DISPLAY_STRING_MONOSPACE			0x0000
#define	DISPLAY_STRING_PROPORTIONAL			0x0200
#define	DISPLAY_STRING_LEFT_ALIGN			0x0000
#define	DISPLAY_STRING_CENTER_ALIGN			0x0400
#define	DISPLAY_STRING_CENTRE_ALIGN			0x0400
#define	DISPLAY_STRING_RIGHT_ALIGN			0x0800
#define	DISPLAY_STRING_SCROLL				0x1000

#endif



//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef DISPLAY_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
void display_delay_ms (BYTE delay_ms);
void display_initialise (void);
void display_set_contrast (BYTE contrast_value);
void display_clear_screen (BYTE invert_pixels);
void display_bitmap (DWORD image_flash_address, CONSTANT BYTE *p_image_memory_address, WORD image_options, WORD x_start_coord, WORD y_start_coord);
void display_const_string (DWORD image_flash_address, CONSTANT BYTE *p_image_memory_address, WORD image_options, WORD x_start_coord, WORD y_start_coord, CONSTANT BYTE *p_ascii_string);
void display_variable_string (DWORD image_flash_address, CONSTANT BYTE *p_image_memory_address, WORD image_options, WORD x_start_coord, WORD y_start_coord, BYTE *p_ascii_string);


#ifdef INCLUDE_DISPLAY_BYTE_TEST_SEQUENCE_FUNCTION
void display_byte_ordering_test_sequence (BYTE flags);
#endif


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern void display_delay_ms (BYTE delay_ms);
extern void display_initialise (void);
extern void display_set_contrast (BYTE contrast_value);
extern void display_clear_screen (BYTE invert_pixels);
extern void display_bitmap (DWORD image_flash_address, CONSTANT BYTE *p_image_memory_address, WORD image_options, WORD x_start_coord, WORD y_start_coord);
extern void display_const_string (DWORD image_flash_address, CONSTANT BYTE *p_image_memory_address, WORD image_options, WORD x_start_coord, WORD y_start_coord, CONSTANT BYTE *p_ascii_string);
extern void display_variable_string (DWORD image_flash_address, CONSTANT BYTE *p_image_memory_address, WORD image_options, WORD x_start_coord, WORD y_start_coord, BYTE *p_ascii_string);


#ifdef INCLUDE_DISPLAY_BYTE_TEST_SEQUENCE_FUNCTION
extern void display_byte_ordering_test_sequence (BYTE flags);
#endif


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef DISPLAY_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------
WORD display_auto_x_coordinate = 0;
WORD display_auto_y_coordinate = 0;
WORD display_scroll_text_skip_pixels_count;


//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)
WORD display_scroll_text_start_pixel = 0xffff;
WORD display_scroll_text_display_pixels_count;
BYTE display_scroll_1st_char_done_skip_pixels;


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------
extern WORD display_scroll_text_start_pixel;
extern WORD display_scroll_text_display_pixels_count;
extern BYTE display_scroll_1st_char_done_skip_pixels;


#endif



