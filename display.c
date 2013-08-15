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
//						DRIVER C CODE FILE
//  Copyright:			EMBEDDED-CODE.COM
//
//<INSERT LICENCE BLOCK HERE>
//****************************************************************************************
//****************************************************************************************



//----- INCLUDE FILES FOR THIS SOURCE CODE FILE -----
#include "main.h"					//(Include the project global defines file)
#define	DISPLAY_C					//(Define used for following header file to flag that it is the header file for this source file)
#include "display.h"				//(Include header file for this source file)
#include "display-model.h"			//Contains defines and functions specific to the screen module being used - if changing display
									//screen change the "display-model.h" file that is included in the project




//----- OTHER PROJECT FILES REQUIRED BY THIS SOURCE CODE FILE -----


//----- COMPILER LIBRARY FILES REQUIRED BY THIS SOURCE CODE FILE -----







//***************************
//***************************
//********** DELAY **********
//***************************
//***************************
//(The accuracy of this function is not important as long as calling with a value of 1 means the delay will
//be at least 1mS - it is only used for the initialise function)
void display_delay_ms (BYTE delay_ms)
{
	DWORD count;

	while (delay_ms)
	{
		count = (INSTRUCTION_CLOCK_FREQUENCY / 1000) / 3;	//Divide by 3 as this loop will take a minimum of 3 steps
															//(may be more depending on compiler but we don't care)
		while (count)
		{
			count--;
		}
		delay_ms--;
	}
}




//***************************************
//***************************************
//********** INITIALISE SCREEN **********
//***************************************
//***************************************
void display_initialise (void)
{
	//Call the initialise function for the screen model in use
	display_model_initialise();

	display_clear_screen(0);
}




//**********************************
//**********************************
//********** SET CONTRAST **********
//**********************************
//**********************************
void display_set_contrast (BYTE contrast_value)
{

	//Call the contrast function for the screen model in use
	display_model_set_contrast(contrast_value);

}






//***********************************
//***********************************
//********** CLEAR SCREEN  **********
//***********************************
//***********************************
//invert_pixels = 0 screen cleared as normal, otherwise screen cleared with all pixels on
void display_clear_screen (BYTE invert_pixels)
{
	WORD page_number;
	WORD line_number;

	//Ensure invert_pixels register is either 0x00 or 0xff
	if (invert_pixels)
		invert_pixels = 0xff;

	//Do for each page
	for (page_number = 0; page_number < (DISPLAY_HEIGHT_PIXELS >> 3); page_number++)
	{
		//Do for each line within each page
		for (line_number = 0; line_number < DISPLAY_WIDTH_PIXELS; line_number++)
			display_write_bitmap_byte (0, 0xff, invert_pixels, line_number, page_number);
	}
}





//*************************************
//*************************************
//********** DISPLAY BITMAP  **********
//*************************************
//*************************************
//
//image_flash_address			Start address of bitmap in flash memory (set to 0 if bitmap is in program memory))
//*image_memory_address			Pointer to bitmap in program memory (set to 0 if bitmap is in flash memory).  (Fonts must be in program memory).
//
//image_options		Bit 15		1 = Invert pixels (i.e. a pixel on becomes off and vice versa)
//					Bit 14		Calling function is display__string (internal use flag)
//					Bit 13:5	Unused
//					Bit 4:3		String options:- 0x00 = align left, 0x01 = align centre, 0x02 = align right, 0x03 = scrolling
//					Bit 2:0		Display Bitmap Type:
//								0x00 =	Display normal bitmap
//								0x01 =	Display character from a font table.  The address given is the program memory start address of the character in the font table. 
//										display_bitmap_height and display_bitmap_width have been loaded with the character bitmap dimensions.
//								0x02 =	Display empty block. display_bitmap_height and display_bitmap_width need to have been loaded with the requried size.  This is a really
//										useful way of clearing or turning on a block of pixels in a specified area of the display. image_flash_address & p_image_memory_address
//										are ignored.
//								0x03 =	Invert displayed pixels in block. display_bitmap_height and display_bitmap_width need to have been loaded with the requried size.  Use
//										this to invert all of the currently displayed pixels within the specified block. image_flash_address & p_image_memory_address are ignored.
//								0x## = 	Other values are undefined
//
//x_start_coord = 0 to DISPLAY_WIDTH_PIXELS.	(If = 0xffff then the 'display_auto_x_coordinate' variable is used for the bitmap x coordiante.  This
//												variable is updated with the next x	coordinate each time this routine is called to allow successive
//												display across a screen).
//y_start_coord = 0 to DISPLAY_HEIGHT_PIXELS	(If = 0xffff then the 'display_auto_y_coordinate' variable is used for the bitmap y coordiante.  This
//												variable is updated with the next y	coordinate each time this routine is called to allow successive
//												display across a screen).
void display_bitmap (DWORD image_flash_address, CONSTANT BYTE *p_image_memory_address, WORD image_options, WORD x_start_coord, WORD y_start_coord)
{
	WORD x_byte_coordinate;
	WORD y_byte_number;
	WORD image_width;
	WORD image_height;
	BYTE image_flags;
	BYTE bitmap_memory_data;
	BYTE y_bits_to_shift_into_next_byte;
	BYTE y_bits_into_next_byte_working;
	BYTE y_coord_shift_right_no_of_bits;
	BYTE shift_bits_count;
	BYTE byte_mask;
	WORD y_no_of_bytes_to_read_per_column;
	WORD y_no_of_bytes_to_write_per_column;
	BYTE display_bitmap_flags = 0;


	#ifdef INVERT_USER_X_Y_COORDS
		WORD w_temp;
		if (!(image_options & 0x4000))		//display__string functions will have already have done the reverse
		{
			w_temp = x_start_coord;
			x_start_coord = y_start_coord;
			y_start_coord = w_temp;
		}	
	#endif


	if ((image_options & DISPLAY_BITMAP_TYPE_BIT_MASK) == DISPLAY_INVERT_BLOCK_PIXELS)
	{
		//------------------------------------------------------
		//----- INVERT CURRENTLY DISPLAYED PIXELS IN BLOCK -----
		//------------------------------------------------------
		display_bitmap_flags |= 0x01;			//Set flag for display_write_bitmap_byte function
		
		image_options &= ~(WORD)DISPLAY_BITMAP_TYPE_BIT_MASK;		//Change to display empty block of inverted pixels mode (display_write_bitmap_byte flag will take care of the inverting for us)
		image_options |= (DISPLAY_EMPTY_BLOCK | DISPLAY_INVERT_PIXELS);
	}
	
	
	//-----------------------------------------------------------------------------
	//----- IF X OR Y COODIANTE = 0XFFFF THEN USE THE AUTO UPDATED COORDINATE -----
	//-----------------------------------------------------------------------------
	if (x_start_coord == 0xffff)
		x_start_coord = display_auto_x_coordinate;

	if (y_start_coord == 0xffff)
		y_start_coord = display_auto_y_coordinate;


	//---------------------------------------------------------------------
	//----- GET BITMAP WIDTH, HEIGHT AND FLAGS FROM THE BITMAP HEADER -----
	//---------------------------------------------------------------------
	//(BYTES 0:3)
	if ((image_options & DISPLAY_BITMAP_TYPE_BIT_MASK) == DISPLAY_FONT_CHARACTER)
	{
		//----- DISPLAYING FONT CHARACTER -----
		if (p_image_memory_address == 0)		//Ensure we are reading program memory
			return;
		
		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
			image_width = display_bitmap_height;				//We already have the width and height values from the display_string function calling the get font character values function.
			image_height = display_bitmap_width;
		#else
			image_width = display_bitmap_width;
			image_height = display_bitmap_height;
		#endif
		
		image_flags = 0;
	}
	else if ((image_options & DISPLAY_BITMAP_TYPE_BIT_MASK) == DISPLAY_EMPTY_BLOCK)
	{		
		//----- DISPLAYING EMPTY BLOCK -----
		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
			image_width = display_bitmap_height;				//These should have have been set by calling function
			image_height = display_bitmap_width;
		#else
			image_width = display_bitmap_width;
			image_height = display_bitmap_height;
		#endif
		
		image_flags = 0;
	}
	else if (p_image_memory_address != 0)
	{
		//----- DISPLAYING BITMAP FROM PROGRAM MEMORY -----
		image_width = *p_image_memory_address++;
		image_width <<= 8;
		image_width += *p_image_memory_address++;	

		image_height = *p_image_memory_address++;
		image_height <<= 8;
		image_height += *p_image_memory_address++;
		
		image_flags = *p_image_memory_address++;
	}
	else
	{
		//----- DISPLAYING BITMAP FROM FLASH MEMORY -----
		#ifdef DISPLAY_USING_FLASH_MEMORY
			image_width = DISPLAY_FLASH_READ_FUNCTION(DISPLAY_FLASH_MEMORY_START_ADDRESS + image_flash_address++);
			image_width <<= 8;
			image_width += DISPLAY_FLASH_READ_FUNCTION(DISPLAY_FLASH_MEMORY_START_ADDRESS + image_flash_address++);	

			image_height = DISPLAY_FLASH_READ_FUNCTION(DISPLAY_FLASH_MEMORY_START_ADDRESS + image_flash_address++);
			image_height <<= 8;
			image_height += DISPLAY_FLASH_READ_FUNCTION(DISPLAY_FLASH_MEMORY_START_ADDRESS + image_flash_address++);
		
			image_flags = DISPLAY_FLASH_READ_FUNCTION(DISPLAY_FLASH_MEMORY_START_ADDRESS + image_flash_address++);
		#else
			return;
		#endif
	}


	//DO BASIC CHECKS
	if (image_width > DISPLAY_WIDTH_PIXELS)
		image_width = DISPLAY_WIDTH_PIXELS;

	if (image_height > DISPLAY_HEIGHT_PIXELS)
		image_height = DISPLAY_HEIGHT_PIXELS;
	
	if (image_width == 0)
		return;
	if (image_height == 0)
		return;



	//-------------------------------------------------------------------------
	//----- MODIFY THE Y START COORDINATE IF DEFINED TO DISPLAY IN REVERSE -----
	//-------------------------------------------------------------------------
//	#ifdef INVERT_Y_AXIS_COORDINATES
//		//y_start_coord -= (image_height - 1);
//		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
//			if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL))
//			{
//				//MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
//				y_start_coord += ((image_height - 1) - (prop_width_ascii_last_used_row - prop_width_ascii_first_used_row));
//			}
//		#endif
//	#endif



	//-----------------------------------
	//-----------------------------------
	//----- DISPLAY THE BITMAP DATA -----
	//-----------------------------------
	//-----------------------------------
	//- We read 1 byte at a time of bitmap data.  The bitmap data is arranged 1 column (Y axis) after the other.  If
	//the Y axis number of pixels is not a multiple of 8 bits then the last byte of each column of bitmap data will
	//contain unused bits to pad the colum length to be byte alligned.
	//- The Y coordinate is the axis in which we write 1 byte at a time to the screen.  Therefore aligning the
	//bitmap on the X axis is no problem, but for the Y axis we have to deal with shifting the bits to work with the
	//byte address boundaries.

	//----- CALCULATE THE NUMBER OF BITS TO SHIFT THE BITMAP DATA TO THE RIGHT BASED ON THE Y START COORDINATE -----
	y_coord_shift_right_no_of_bits = (y_start_coord & 0x07);

//	#ifdef DISPLAY_STRINGS_ON_Y_AXIS
//		if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL))
//		{
//			//MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
//			y_coord_shift_right_no_of_bits = ((y_start_coord - prop_width_ascii_first_used_row) & 0x07);
//		}
//	#endif


	//----- CALCULATE THE NUMBER OF BYTES OF BITMAP DATA TO READ PER COLUMN -----
	if ((image_height & 0x07) == 0)
		y_no_of_bytes_to_read_per_column = (image_height >> 3);
	else
		y_no_of_bytes_to_read_per_column = (image_height >> 3) + 1;

	//----- CALCULATE THE NUMBER OF BYTES TO WRITE TO THE DISPLAY PER COLUMN -----
	if (((y_coord_shift_right_no_of_bits + image_height) & 0x07) == 0)
		y_no_of_bytes_to_write_per_column = ((y_coord_shift_right_no_of_bits + image_height) >> 3);
	else
		y_no_of_bytes_to_write_per_column = ((y_coord_shift_right_no_of_bits + image_height) >> 3) + 1;


	if ((image_options & DISPLAY_BITMAP_TYPE_BIT_MASK) == DISPLAY_FONT_CHARACTER)
	{
		//----- FONT CHARACTERS ARE STORED BACKWARDS COMPARED TO BITMAPS SO COMPENSATE -----
		p_image_memory_address += (y_no_of_bytes_to_read_per_column * (image_width - 1));		//Offset to final row
	}

	//------------------------------------
	//----- DISPLAY EACH COLUMN LOOP -----
	//------------------------------------
	for (x_byte_coordinate = x_start_coord; (x_byte_coordinate - x_start_coord) < image_width; x_byte_coordinate++)
	{		
		//RESET THE BIT SHIFT REGISTERS READY FOR THE NEW COLUMN
		y_bits_to_shift_into_next_byte = 0;

//		if (image_options & 0x007f)
//		{
//			#ifdef DISPLAY_STRINGS_ON_Y_AXIS
//				if ((image_options & DISPLAY_STRING_PROPORTIONAL))		//(If's seperated to speed up display of non text bitmaps)
//				{
//					//SETUP REGISTERS FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
//					prop_width_ascii_leading_bits_to_mask = prop_width_ascii_first_used_row + y_coord_shift_right_no_of_bits;	
//					prop_width_ascii_trailing_bits_to_mask = (image_height - prop_width_ascii_last_used_row) - 1;
//				}
//			#endif
//		}

		//------------------------------------
		//----- DISPLAY THIS COLUMN LOOP -----
		//------------------------------------
		for (y_byte_number = 0; y_byte_number < y_no_of_bytes_to_write_per_column; y_byte_number++)
		{
			//---------------------------------------------------------------------------
			//----- GET THE NEXT BITMAP BYTE FROM FLASH MEMORY AND DO THE BIT SHIFT -----
			//---------------------------------------------------------------------------
			if (y_byte_number < y_no_of_bytes_to_read_per_column)
			{
				//-------------------------------------------------------------
				//----- THIS IS NOT A FINAL OVERFLOW BYTE TO BE DISPLAYED -----
				//-------------------------------------------------------------
				if ((image_options & DISPLAY_BITMAP_TYPE_BIT_MASK) == DISPLAY_EMPTY_BLOCK)
				{
					bitmap_memory_data = 0x00;
				}
				else
				{				
					if (p_image_memory_address != 0)
					{
						//----- BITMAP IS IN PROGRAM MEMORY -----
						bitmap_memory_data = *p_image_memory_address++;
					}
					#ifdef DISPLAY_USING_FLASH_MEMORY
						else
						{
							//----- BITMAP IS IN FLASH MEMORY -----
							bitmap_memory_data = DISPLAY_FLASH_READ_FUNCTION(DISPLAY_FLASH_MEMORY_START_ADDRESS + image_flash_address++);
						}
					#endif
				}

				//--------------------------------------------------
				//----- INVERT THE PIXELS IF OPTION BIT IS SET -----
				//--------------------------------------------------
				if (image_options & DISPLAY_INVERT_PIXELS)
					bitmap_memory_data = ~bitmap_memory_data;

				//-----------------------------
				//----- PROCESS BIT SHIFT -----
				//-----------------------------
				y_bits_to_shift_into_next_byte >>= (8 - y_coord_shift_right_no_of_bits);

				y_bits_into_next_byte_working = 0;
				for (shift_bits_count = 0; shift_bits_count < y_coord_shift_right_no_of_bits; shift_bits_count++)
				{
					y_bits_into_next_byte_working >>= 1;
					if (bitmap_memory_data & 0x01)
						y_bits_into_next_byte_working += 0x80;
					bitmap_memory_data >>= 1;
					if ((y_bits_to_shift_into_next_byte >> shift_bits_count) & 0x01)
						bitmap_memory_data |= 0x80;
				}
			}
			else
			{
				//---------------------------------------------------------
				//----- THIS IS A FINAL OVERFLOW BYTE TO BE DISPLAYED -----
				//---------------------------------------------------------
				//DON'T READ A BYTE FROM MEMORY - JUST USE THE OVERFLOW BITS FROM THE LAST BYTE
				bitmap_memory_data = 0;
				y_bits_to_shift_into_next_byte >>= (8 - y_coord_shift_right_no_of_bits);
				for (shift_bits_count = 0; shift_bits_count < y_coord_shift_right_no_of_bits; shift_bits_count++)
				{
					bitmap_memory_data >>= 1;
					if ((y_bits_to_shift_into_next_byte >> shift_bits_count) & 0x01)
						bitmap_memory_data |= 0x80;
				}
			}

			//---------------------------
			//----- CREATE THE MASK -----
			//---------------------------
			//(bit = 1 = this bit shows bitmap data)
			byte_mask = 0xff;		//Default to show all bits

			//-------------------------------------------------------------------------
			//----- IF THIS IS THE FIRST BYTE OF THIS COLUMN THEN ADJUST THE MASK -----
			//-------------------------------------------------------------------------
			if (y_byte_number == 0)
			{
				byte_mask >>= y_coord_shift_right_no_of_bits;
			}

			//------------------------------------------------------------------------
			//----- IF THIS IS THE LAST BYTE OF THIS COLUMN THEN ADJUST THE MASK -----
			//------------------------------------------------------------------------
			if (y_byte_number == y_no_of_bytes_to_write_per_column - 1)
			{
				switch ((y_coord_shift_right_no_of_bits + image_height) & 0x07)
				{
				//case 0:
				//	byte_mask &= 0x00;
				//	break;
				case 1:
					byte_mask &= 0x80;
					break;
				case 2:
					byte_mask &= 0xc0;
					break;
				case 3:
					byte_mask &= 0xe0;
					break;
				case 4:
					byte_mask &= 0xf0;
					break;
				case 5:
					byte_mask &= 0xf8;
					break;
				case 6:
					byte_mask &= 0xfc;
					break;
				case 7:
					byte_mask &= 0xfe;
					break;
				default:
					break;
				}
			}

//			#ifdef DISPLAY_STRINGS_ON_Y_AXIS
//				if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL))
//				{
//					//------------------------------------------------------------------------
//					//----- MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS -----
//					//------------------------------------------------------------------------
//					//DON'T DISPLAY THE LEADING UNUSED BITS (retain what is already being displayed)
//					if (prop_width_ascii_leading_bits_to_mask)
//					{
//						b_temp = prop_width_ascii_leading_bits_to_mask;
//						if (b_temp > 8)
//							b_temp = 8;
//		
//						b_temp1 = 0x80;
//						for ( ; b_temp > 0; b_temp--)
//						{
//							byte_mask &= ~b_temp1;
//							b_temp1 >>= 1;
//							prop_width_ascii_leading_bits_to_mask--;
//						}
//					}
//	
//					//DON'T DISPLAY THE TRAILING UNUSED BITS (retain what is already being displayed)
//					if (prop_width_ascii_trailing_bits_to_mask)
//					{
//	
//						//Calculate the number of bits of bitmap data remaining to be displayed in this column
//						w_temp = image_height + y_coord_shift_right_no_of_bits - (y_byte_number << 3);
//						
//						if (
//							((w_temp - prop_width_ascii_trailing_bits_to_mask) < 8) &&
//							(prop_width_ascii_trailing_bits_to_mask)
//							)
//						{
//							//WE NEED TO MASK BITS FROM THIS BYTE
//							b_temp = (8 - (w_temp - prop_width_ascii_trailing_bits_to_mask));
//							prop_width_ascii_trailing_bits_to_mask -= b_temp;
//	
//							b_temp1 = 0xff;
//							for ( ; b_temp > 0; b_temp--)			//(We have to do a for loop as a b_temp value of 8 results in no shifting with the C18 compiler)
//								b_temp1 <<= 1;
//	
//							byte_mask &= b_temp1;
//						}
//	
//	
//					}
//				}
//			#endif

			//-----------------------------------------
			//----- WRITE THE BYTE TO THE DISPLAY -----
			//-----------------------------------------
//			#ifdef DISPLAY_STRINGS_ON_Y_AXIS
//				if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL))
//				{
//					//----- MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS -----
//					//(Deal with possible negative y coord values due to the first bits of a start of line character being off the screen)
//					display_write_bitmap_byte(display_bitmap_flags, byte_mask, bitmap_memory_data, x_byte_coordinate,
//					(
//					((y_start_coord - prop_width_ascii_first_used_row)
//					+ (y_byte_number << 3))
//					>> 3)
//					);
//				}
//				else
//				{
//					//----- NORMAL DISPLAY -----
//	
//					#ifdef DOING_BITFONTCREATORPRO
//						if (image_options & 0x0001)
//						{
//							//BitFontCreatorPro inverts text rows compared to us (fiers
//							display_write_bitmap_byte(display_bitmap_flags, byte_mask, bitmap_memory_data, (x_start_coord - (x_byte_coordinate - x_start_coord)), ((y_start_coord >> 3) + y_byte_number));	//(x_start_coord - (x_byte_coordinate - x_start_coord)) is just how we reverse the orientation of the x_byte_coordinate for loop incrementing
//						}
//						else
//						{
//							display_write_bitmap_byte(display_bitmap_flags, byte_mask, bitmap_memory_data, x_byte_coordinate, ((y_start_coord >> 3) + y_byte_number));
//						}	
//					#else
//						display_write_bitmap_byte(display_bitmap_flags, byte_mask, bitmap_memory_data, x_byte_coordinate, ((y_start_coord >> 3) + y_byte_number));
//					#endif
//				}
//			#else
				//----- NORMAL DISPLAY -----
				display_write_bitmap_byte(display_bitmap_flags, byte_mask, bitmap_memory_data, x_byte_coordinate, ((y_start_coord >> 3) + y_byte_number));
//			#endif



			//-------------------------------------------------
			//----- SETUP THE BIT SHIFT FOR THE NEXT BYTE -----
			//-------------------------------------------------
			y_bits_to_shift_into_next_byte = y_bits_into_next_byte_working;

		} //for (y_byte_number = 0; y_byte_number < y_no_of_bytes_to_write_per_column; y_byte_number++)
		
		if ((image_options & DISPLAY_BITMAP_TYPE_BIT_MASK) == DISPLAY_FONT_CHARACTER)
		{	
			//----- FONT CHARACTERS ARE STORED BACKWARDS COMPARED TO BITMAPS SO COMPENSATE -----
			p_image_memory_address -= (y_no_of_bytes_to_read_per_column << 1);			//Move to start of previous row
		}


	} //for (x_byte_coordinate = x_start_coord; (x_byte_coordinate - x_start_coord) < image_width; x_byte_coordinate++)

	//------------------------------------------
	//----- FINISHED DISPLAYING THE BITMAP -----
	//------------------------------------------

	//----- UPDATE THE display_auto_x_coordinate & display_auto_y_coordinate VARIABLES -----
	display_auto_x_coordinate = x_byte_coordinate;

	#ifdef INVERT_Y_AXIS_COORDINATES
		//----- Y AXIS COORDINATE IS REVERSED -----
//		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
//			if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL))
//			{
//				//MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
//				y_start_coord -= (image_height - (prop_width_ascii_last_used_row - prop_width_ascii_first_used_row));
//	
//				display_auto_y_coordinate = y_start_coord;
//	
//				//MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
//				display_auto_y_coordinate += ((image_height - prop_width_ascii_last_used_row) - 1);
//				display_auto_y_coordinate += prop_width_ascii_first_used_row;
//			}
//			else
//			{
//				display_auto_y_coordinate = y_start_coord - 1;
//			}
//		#else
			display_auto_y_coordinate = y_start_coord - 1;
//		#endif
	#else
		//----- Y AXIS COORDINATE IS NORMAL -----
		display_auto_y_coordinate = y_start_coord + image_height;
	
//		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
//			if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL))
//			{
//				//MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
//				display_auto_y_coordinate -= ((image_height - prop_width_ascii_last_used_row) - 1);
//				display_auto_y_coordinate -= prop_width_ascii_first_used_row;
//			}
//		#endif
	#endif


}








//*******************************************
//*******************************************
//********** DISPLAY ASCII STRING  **********
//*******************************************
//*******************************************
//*p_font_function			Pointer to the get_font_ function for the font to be used
//
//image_options
//					Bit 15		1 = Invert pixels (i.e. a pixel on becomes off and vice versa)
//					Bit 14:5	Unused
//					Bit 4:3		String options:- 0x00 = align left, 0x01 = align centre, 0x02 = align right, 0x03 = scrolling
//					Bit 2:0		Reserved
//
//minimum_width				If the width of the text is less than this value whitespace will be added as requried to meet the minimum width
//x_start_coord = 0 to DISPLAY_WIDTH_PIXELS.	(If = 0xffff then the 'display_auto_x_coordinate' variable is used for the bitmap x coordiante.  This
//												variable is updated with the next x	coordinate each time this routine is called to allow successive
//												display across a screen).
//y_start_coord = 0 to DISPLAY_HEIGHT_PIXELS	(If = 0xffff then the 'display_auto_y_coordinate' variable is used for the bitmap y coordiante.  This
//												variable is updated with the next y	coordinate each time this routine is called to allow successive
//												display across a screen).
//
//p_ascii_string				Pointer to the string to be displayed (must be null terminated)


//----------------------------------------
//----------------------------------------
//----- VERSION FOR CONSTANT STRINGS -----
//----------------------------------------
//----------------------------------------
//Returns total length (pixels) of string displayed
WORD display_const_string (CONSTANT BYTE *(*p_font_function)(WORD character), WORD image_options, WORD minimum_width,
							   WORD x_start_coord, WORD y_start_coord, CONSTANT BYTE *p_ascii_string)
{
	WORD next_character;
	CONSTANT BYTE *p_ascii_string_copy;
	WORD string_width;
	CONSTANT BYTE *p_character_data;
	WORD total_width = 0;
	WORD gap_image_options;
	WORD leading_pad_width = 0;

	#ifdef INVERT_USER_X_Y_COORDS
		WORD w_temp;
		w_temp = x_start_coord;
		x_start_coord = y_start_coord;
		y_start_coord = w_temp;
	#endif

	p_ascii_string_copy = p_ascii_string;

	if ((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_STRING_SCROLL)
	{
		//-------------------------------------
		//----- DISPLAYING SCROLLING TEXT -----
		//-------------------------------------
		display_scroll_text_skip_pixels_count = display_scroll_text_start_pixel;		//Setup for new scrolling text display
		display_scroll_1st_char_done_skip_pixels = 0;
	}


	if (
	((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_ALIGN_CENTRE) ||
	((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_ALIGN_RIGHT)
	)
	{
		//----------------------------------------------
		//----- DISPLAYING CENTRE OR RIGHT ALIGNED -----
		//----------------------------------------------
		
		//GET TOTAL STRING WIDTH
		string_width = 0;
		next_character = (WORD)*p_ascii_string++;
		while (next_character > 0x00)			//Do until null terminator
		{
			p_font_function(next_character);
			string_width += display_bitmap_width + display_font_spacing;
			
			next_character = *p_ascii_string++;
		}
		string_width -= display_font_spacing;		//Remove the final inter character gap that won't be displayed
		
		if ((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_ALIGN_CENTRE)
		{
			//Centre alignment selected so ensure we always have an even value for this so the start coord will always be the same for a given width
			if (string_width & 0x01)
				string_width++;
		}
		
		//CALCULATE ANY LEADING PAD WHITESPACE REQUIRED
		if (string_width < minimum_width)
			leading_pad_width = (minimum_width - string_width);
		
		//ADJUST START POSITION TO PROVIDE THE ALIGNMENT
		if ((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_ALIGN_CENTRE)
		{
			string_width >>= 1;
			leading_pad_width >>= 1;
		}
		else
		{
			string_width--;		//For right aligned decrement as we want to use the last pixel, not be right aligned just before it
		}

		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
			#ifdef INVERT_Y_AXIS_COORDINATES
				y_start_coord += (string_width + leading_pad_width);
			#else
				y_start_coord -= (string_width + leading_pad_width);
			#endif
		#else
			x_start_coord -= (string_width + leading_pad_width); 
		#endif

		p_ascii_string = p_ascii_string_copy;
	}

	//SETUP FOR DISPLAY
	display_auto_x_coordinate = x_start_coord;
	display_auto_y_coordinate = y_start_coord;
	gap_image_options = 0x4000 | DISPLAY_EMPTY_BLOCK | (image_options & (WORD)DISPLAY_INVERT_PIXELS);
	
	
	//---------------------------------------------------
	//----- DISPLAY LEADING WHITESPACE IF NECESSARY -----
	//---------------------------------------------------
	if (leading_pad_width)
	{
		display_bitmap_width = leading_pad_width;
		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
			display_bitmap(0, 0, gap_image_options, x_start_coord, 0xffff);
		#else
			display_bitmap(0, 0, gap_image_options, 0xffff, y_start_coord);
		#endif
		total_width += leading_pad_width;
	}
	
	//------------------------------
	//----- DISPLAY THE STRING -----
	//------------------------------
	image_options &= ~((WORD)DISPLAY_BITMAP_TYPE_BIT_MASK);
	image_options |= DISPLAY_FONT_CHARACTER;
	
	next_character = (WORD)*p_ascii_string++;
	while (next_character > 0x00)			//Do until null terminator
	{
		//----- DISPLAY THE NEXT CHARACTER -----
		p_character_data = p_font_function(next_character);
		total_width += display_bitmap_width;
		
		//DISPLAY CHARACTER
		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
			display_bitmap(0, p_character_data, (0x4000 | image_options), x_start_coord, 0xffff);
		#else
			display_bitmap(0, p_character_data, (0x4000 | image_options), 0xffff, y_start_coord);
		#endif

		if (
		((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_STRING_SCROLL) &&
		(display_scroll_text_display_pixels_count == 0)
		)
		{
			//DISPLAYING SCROLLING TEXT - ALREADY DISPLAYED ALL OF THE PIXELS TO INCUDE SO NO NEED TO CONTINUE
			break;
		}
		
		next_character = (WORD)*p_ascii_string++;

		if (next_character != 0x00)
		{
			//DISPLAY THE GAP TO NEXT CHARACTER
			display_bitmap_width = display_font_spacing;
			total_width += display_font_spacing;
			#ifdef DISPLAY_STRINGS_ON_Y_AXIS
				display_bitmap(0, 0, gap_image_options, x_start_coord, 0xffff);
			#else
				display_bitmap(0, 0, gap_image_options, 0xffff, y_start_coord);
			#endif
		}

	} //while (next_character > 0x00)

	//----------------------------------------------------
	//----- DISPLAY TRAILING WHITESPACE IF NECESSARY -----
	//----------------------------------------------------
	if (total_width < minimum_width)
	{
		display_bitmap_width = minimum_width - total_width;
		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
			display_bitmap(0, 0, gap_image_options, x_start_coord, 0xffff);
		#else
			display_bitmap(0, 0, gap_image_options, 0xffff, y_start_coord);
		#endif
	}

	return(total_width);
}



//----------------------------------------
//----------------------------------------
//----- VERSION FOR VARIABLE STRINGS -----
//----------------------------------------
//----------------------------------------
//Returns total length (pixels) of string displayed
WORD display_variable_string (CONSTANT BYTE *(*p_font_function)(WORD character), WORD image_options, WORD minimum_width,
							   WORD x_start_coord, WORD y_start_coord, BYTE *p_ascii_string)
{
	WORD next_character;
	BYTE *p_ascii_string_copy;
	WORD string_width;
	CONSTANT BYTE *p_character_data;
	WORD total_width = 0;
	WORD gap_image_options;
	WORD leading_pad_width = 0;

	#ifdef INVERT_USER_X_Y_COORDS
		WORD w_temp;
		w_temp = x_start_coord;
		x_start_coord = y_start_coord;
		y_start_coord = w_temp;
	#endif

	p_ascii_string_copy = p_ascii_string;

	if ((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_STRING_SCROLL)
	{
		//-------------------------------------
		//----- DISPLAYING SCROLLING TEXT -----
		//-------------------------------------
		display_scroll_text_skip_pixels_count = display_scroll_text_start_pixel;		//Setup for new scrolling text display
		display_scroll_1st_char_done_skip_pixels = 0;
	}


	if (
	((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_ALIGN_CENTRE) ||
	((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_ALIGN_RIGHT)
	)
	{
		//----------------------------------------------
		//----- DISPLAYING CENTRE OR RIGHT ALIGNED -----
		//----------------------------------------------
		
		//GET TOTAL STRING WIDTH
		string_width = 0;
		next_character = (WORD)*p_ascii_string++;
		while (next_character > 0x00)			//Do until null terminator
		{
			p_font_function(next_character);
			string_width += display_bitmap_width + display_font_spacing;
			
			next_character = *p_ascii_string++;
		}
		string_width -= display_font_spacing;		//Remove the final inter character gap that won't be displayed

		if ((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_ALIGN_CENTRE)
		{
			//Centre alignment selected so ensure we always have an even value for this so the start coord will always be the same for a given width
			if (string_width & 0x01)
				string_width++;
		}

		//CALCULATE ANY LEADING PAD WHITESPACE REQUIRED
		if (string_width < minimum_width)
			leading_pad_width = (minimum_width - string_width);
		
		//ADJUST START POSITION TO PROVIDE THE ALIGNMENT
		if ((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_ALIGN_CENTRE)
		{
			string_width >>= 1;
			leading_pad_width >>= 1;
		}
		else
		{
			string_width--;		//For right aligned decrement as we want to use the last pixel, not be right aligned just before it
		}	

		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
			#ifdef INVERT_Y_AXIS_COORDINATES
				y_start_coord += (string_width + leading_pad_width);
			#else
				y_start_coord -= (string_width + leading_pad_width);
			#endif
		#else
			x_start_coord -= (string_width + leading_pad_width);
		#endif
		
		p_ascii_string = p_ascii_string_copy;
	}	

	//SETUP FOR DISPLAY
	display_auto_x_coordinate = x_start_coord;
	display_auto_y_coordinate = y_start_coord;
	gap_image_options = 0x4000 | DISPLAY_EMPTY_BLOCK | (image_options & (WORD)DISPLAY_INVERT_PIXELS);


	//---------------------------------------------------
	//----- DISPLAY LEADING WHITESPACE IF NECESSARY -----
	//---------------------------------------------------
	if (leading_pad_width)
	{
		display_bitmap_width = leading_pad_width;
		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
			display_bitmap(0, 0, gap_image_options, x_start_coord, 0xffff);
		#else
			display_bitmap(0, 0, gap_image_options, 0xffff, y_start_coord);
		#endif
		total_width += leading_pad_width;
	}

	//------------------------------
	//----- DISPLAY THE STRING -----
	//------------------------------
	image_options &= ~((WORD)DISPLAY_BITMAP_TYPE_BIT_MASK);
	image_options |= DISPLAY_FONT_CHARACTER;
	
	next_character = (WORD)*p_ascii_string++;
	while (next_character > 0x00)			//Do until null terminator
	{
		//----- DISPLAY THE NEXT CHARACTER -----
		p_character_data = p_font_function(next_character);
		total_width += display_bitmap_width;
		
		//DISPLAY CHARACTER
		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
			display_bitmap(0, p_character_data, (0x4000 | image_options), x_start_coord, 0xffff);
		#else
			display_bitmap(0, p_character_data, (0x4000 | image_options), 0xffff, y_start_coord);
		#endif

		if (
		((image_options & DISPLAY_STRING_OPTION_BIT_MASK) == DISPLAY_STRING_SCROLL) &&
		(display_scroll_text_display_pixels_count == 0)
		)
		{
			//DISPLAYING SCROLLING TEXT - ALREADY DISPLAYED ALL OF THE PIXELS TO INCUDE SO NO NEED TO CONTINUE
			break;
		}
		
		next_character = (WORD)*p_ascii_string++;

		if (next_character != 0x00)
		{
			//DISPLAY THE GAP TO NEXT CHARACTER
			display_bitmap_width = display_font_spacing;
			total_width += display_font_spacing;
			#ifdef DISPLAY_STRINGS_ON_Y_AXIS
				display_bitmap(0, 0, gap_image_options, x_start_coord, 0xffff);
			#else
				display_bitmap(0, 0, gap_image_options, 0xffff, y_start_coord);
			#endif
		}
		
	} //while (next_character > 0x00)

	//----------------------------------------------------
	//----- DISPLAY TRAILING WHITESPACE IF NECESSARY -----
	//----------------------------------------------------
	if (total_width < minimum_width)
	{
		display_bitmap_width = minimum_width - total_width;
		#ifdef DISPLAY_STRINGS_ON_Y_AXIS
			display_bitmap(0, 0, gap_image_options, x_start_coord, 0xffff);
		#else
			display_bitmap(0, 0, gap_image_options, 0xffff, y_start_coord);
		#endif
	}
	
	return(total_width);
}










#ifdef INCLUDE_DISPLAY_BYTE_TEST_SEQUENCE_FUNCTION
//*********************************************************
//*********************************************************
//********** DISPLAY BYTE ORDERING TEST SEQUENCE **********
//*********************************************************
//*********************************************************
//flags		bit 0 set = reset to byte 0
//			bit 1 clear = move forwards 1 byte, set = move back 1 byte
void display_byte_ordering_test_sequence (BYTE flags)
{
	static WORD byte_ordering_test_last_x_coord = 0;
	static WORD byte_ordering_test_last_y_coord = 0;

	//IF MOVING BACKWARDS THEN CLEAR THE LAST BYTE POSITION
	if (flags & 0x02)
		display_write_bitmap_byte(0, 0xff, 0x00, byte_ordering_test_last_x_coord, byte_ordering_test_last_y_coord);


	//MOVE BYTE COUNTER BACKWARDS OR FORWARDS
	if (flags & 0x02)
	{
		//Move backwards
		if (byte_ordering_test_last_y_coord > 0)
		{
			byte_ordering_test_last_y_coord -= 1;
		}
		else
		{
			byte_ordering_test_last_y_coord = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1);
			byte_ordering_test_last_x_coord -= 1;
		}
	}
	else
	{
		//Move forwards
		if (byte_ordering_test_last_y_coord < ((DISPLAY_HEIGHT_PIXELS >> 3) - 1))
		{
			byte_ordering_test_last_y_coord += 1;
		}
		else
		{
			byte_ordering_test_last_y_coord = 0;
			byte_ordering_test_last_x_coord += 1;
		}
	}



	//CHECK FOR RESET TO BYTE 0
	if (flags & 0x01)
	{
		display_clear_screen(0);
		byte_ordering_test_last_x_coord = 0;
		byte_ordering_test_last_y_coord = 0;
	}

	//DISPLAY BYTE POSITION
	display_write_bitmap_byte(0, 0xff, 0x8f, byte_ordering_test_last_x_coord, byte_ordering_test_last_y_coord);

}
#endif






