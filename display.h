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
//						DRIVER C CODE HEADER FILE
//  Copyright:			EMBEDDED-CODE.COM
//
//<INSERT LICENCE BLOCK HERE>
//****************************************************************************************
//****************************************************************************************



//##### WHAT WE'VE DONE #####
//- I have deleted the old font handling.  This is inteded to be the new LCD driver files once all orientations are tested.
//- Defines have changed - ensure manual is updated.
//- Moving forward with this driver should be very straightforward.
//- Whilst I've been careful it is possible to display scrolling text may have been damaged, although hopefully not.
//- Notes that this project we've had to use the manager PSV functions of the compiler by altering the main CONSTANT define and declaring our arrays with a special
//attribute marker.  This is C30 compiler specification deal with its 16 bit pointer normal limitation.
//- Several parts of the display_bitmap function are commented out.  They can be deleted, but have been left for now to see how it used to work in case there are any issues to solve.
//- display_write_bitmap_byte has been updated in all of the screen driver files also to include the new flags byte - just need to update documentation
//- INVERT_USER_X_Y_COORDS added, so user can always work in X,Y.  Recomend adding defines for orientation to corect X,Y so 0,0 is always bottom left.
//- New Sales features:- European character sets built in.  Text display functions simplified and much faster.  All fonts now proportional by design with reduced font lookup tables
//space requriement, unicode support possible (the display__string functions woudl just need to accept a word rather than a byte based string - everythign else ready to go).  Supports
//the BitFontCreator Pro software package which makes creating new fonts and adjusting existing fonts a breeze.  Minimum width text width option to ensure previous text is completly
//overwritten without adding leading and trailing spaces or drawing blank box first.  Draw empty block option added - turn all pixels on or clear all pixels in a specified rectangle.
//Invert block pixels - simply invert the existing display pixels in a specified rectangle


//*************************************
//*************************************
//********** DRIVER REVISION **********
//*************************************
//*************************************
//
//This Version: V1.03
//
//Revision History
//	See driver revision history file




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
//#define	INCLUDE_DISPLAY_BYTE_TEST_SEQUENCE_FUNCTION			//Comment out to remove this function to save program space

#define	DISPLAY_STRINGS_ON_Y_AXIS								//Include or comment out.  One option will be right for your screen orientation
#define	INVERT_USER_X_Y_COORDS									//Include or comment out.  One option will be right for your screen orientation.  This define is used so
																//that when calling driver functions you always specify the start coordiante using X,Y if the display is
																//is actually orientated as Y,X from the drivers point of view.
				

//----------------------------------
//----- DISPLAY OPTION DEFINES -----
//----------------------------------
//Display Bitmap Types:
#define	DISPLAY_BITMAP						0x0000
#define	DISPLAY_FONT_CHARACTER				0x0001		//(Used internally - not intended for application use)
#define	DISPLAY_EMPTY_BLOCK					0x0002
#define	DISPLAY_INVERT_BLOCK_PIXELS			0x0003
#define	DISPLAY_BITMAP_TYPE_BIT_MASK		0x0007		//(Used internally - not intended for application use)

//General Display Options:
#define	DISPLAY_INVERT_PIXELS				0x8000

//Display String Options:
#define	DISPLAY_ALIGN_LEFT					0x0000
#define	DISPLAY_ALIGN_CENTER				0x0008
#define	DISPLAY_ALIGN_CENTRE				0x0008
#define	DISPLAY_ALIGN_RIGHT					0x0010
#define	DISPLAY_STRING_SCROLL				0x0018
#define	DISPLAY_STRING_OPTION_BIT_MASK		0x0018

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
void display_bitmap (DWORD image_flash_address, __psv__ CONSTANT BYTE *p_image_memory_address, WORD image_options, WORD x_start_coord, WORD y_start_coord);
WORD display_const_string (CONSTANT BYTE *(*p_font_function)(WORD character), WORD image_options, WORD minimum_width, 
							   WORD x_start_coord, WORD y_start_coord, CONSTANT BYTE *p_ascii_string);
WORD display_variable_string (CONSTANT BYTE *(*p_font_function)(WORD character), WORD image_options, WORD minimum_width, 
							   WORD x_start_coord, WORD y_start_coord, BYTE *p_ascii_string);


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

extern WORD display_const_string (CONSTANT BYTE *(*p_font_function)(WORD character), WORD image_options, WORD minimum_width, 
							   WORD x_start_coord, WORD y_start_coord, CONSTANT BYTE *p_ascii_string);
extern WORD display_variable_string (CONSTANT BYTE *(*p_font_function)(WORD character), WORD image_options, WORD minimum_width, 
							   WORD x_start_coord, WORD y_start_coord, BYTE *p_ascii_string);


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
WORD display_bitmap_height;
WORD display_bitmap_width;
BYTE display_font_spacing;


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------
extern WORD display_scroll_text_start_pixel;
extern WORD display_scroll_text_display_pixels_count;
extern BYTE display_scroll_1st_char_done_skip_pixels;
extern WORD display_bitmap_height;
extern WORD display_bitmap_width;
extern BYTE display_font_spacing;


#endif



