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
//DRIVER C CODE FILE





//----- INCLUDE FILES FOR THIS SOURCE CODE FILE -----
#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
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
			display_write_bitmap_byte (0xff, invert_pixels, line_number, page_number);
	}
}







//*************************************
//*************************************
//********** DISPLAY BITMAP  **********
//*************************************
//*************************************
//
//image_flash_address			Start address of bitmap in flash memory (set to 0 if bitmap is in program memory))
//*image_memory_address			Pointer to bitmap in program memory (set to 0 if bitmap is in flash memory)
//
//image_options		Bit 15:13	Unused
//					Bit 12		Font table image: 1 = Scroll text, 0 = static text
//					Bit 11:10	If either is high then do not output to the screen (used for centre and right aligned text)
//					Bit 9		Font table image: 1 = using proportionally spaced characters
//					Bit 8		Font table image: 0 = displayed along X axis, 1 = display along Y axis
//					Bit 7		1 = Invert pixels (i.e. a pixel on becomes off and vice versa)
//					Bit 6:0		0 = image is a normal bitmap.
//								>0 = Image is from a font table.  The address given is the start of the font table.  The value of bits 6:0 is the
//								ASCII character required (0x01 - 0x7F)
//
//x_start_coord = 0 to DISPLAY_WIDTH_PIXELS.	(If = 0xffff then the 'display_auto_x_coordinate' variable is used for the bitmap x coordiante.  This
//												variable is updated with the next x	coordinate each time this routine is called to allow successive
//												display across a screen).
//y_start_coord = 0 to DISPLAY_HEIGHT_PIXELS	(If = 0xffff then the 'display_auto_y_coordinate' variable is used for the bitmap y coordiante.  This
//												variable is updated with the next y	coordinate each time this routine is called to allow successive
//												display across a screen).
//
void display_bitmap (DWORD image_flash_address, CONSTANT BYTE *p_image_memory_address, WORD image_options, WORD x_start_coord, WORD y_start_coord)
{
	BYTE b_temp;
	BYTE b_temp1;
	WORD w_temp;
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
	BYTE prop_width_ascii_first_used_row;
	BYTE prop_width_ascii_last_used_row;
	BYTE prop_width_ascii_first_used_column;
	BYTE prop_width_ascii_last_used_column;
	BYTE prop_width_this_x_row_number;
	BYTE prop_width_this_y_column_number;
	WORD prop_width_number_of_bytes_read;
	BYTE prop_width_ascii_leading_bits_to_mask;
	BYTE prop_width_ascii_trailing_bits_to_mask;


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
	if (p_image_memory_address != 0)
	{
		//----- BITMAP IS IN PROGRAM MEMORY -----
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
		//----- BITMAP IS IN FLASH MEMORY -----
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

	if (image_width > DISPLAY_WIDTH_PIXELS)
		image_width = DISPLAY_WIDTH_PIXELS;

	if (image_height > DISPLAY_HEIGHT_PIXELS)
		image_height = DISPLAY_HEIGHT_PIXELS;
	
	if (image_width == 0)
		return;
	if (image_height == 0)
		return;


	//---------------------------------------------------------------------------------------------
	//----- IF THIS IS AN ASCII FONT CHARACTER OFFSET TO THE START OF THE REQUESTED CHARACTER -----
	//---------------------------------------------------------------------------------------------
	if (image_options & 0x007f)
	{
		if (p_image_memory_address != 0)
		{
			p_image_memory_address += ((0x7f - (image_options & 0x007f)) * (image_width * ((image_height >> 3) + 1)));
		}
		#ifdef DISPLAY_USING_FLASH_MEMORY
		else
		{
			image_flash_address += ((0x7f - (image_options & 0x007f)) * (image_width * ((image_height >> 3) + 1)));
		}
		#endif
	}


	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----- IF DISPLAYING PROPORTIONAL WIDTH ASCII THEN PRE-PROCESS THE CHARACTER BITMAP -----
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL))
	{
		//We want to know where the first and last pixel is on each axis (as we display on screens in any of the 4 possible orientations)
	
		//Getting the values for the rows (X axis):-
		//Read each column bit by bit and store the first and last pixel number
	
		//Getting the values for the columns (Y axis):-
		//For each Y column see if at least 1 pixel is on.
		//If yes and the column first pixel number is blank then store this as the first column.
		//If yes store this as the last column

		//Set default values
		prop_width_ascii_first_used_row = 0xff;
		prop_width_ascii_last_used_row = 0;
		prop_width_ascii_first_used_column = 0xff;
		prop_width_ascii_last_used_column = 0;
		prop_width_this_y_column_number = 0;
		prop_width_number_of_bytes_read = 0;				//(This variable is used to restore the memory address after this section)
	
		//--------------------------------
		//----- READ THE BITMAP DATA -----
		//--------------------------------
		//- We read 1 byte at a time of bitmap data.  The bitmap data is arranged 1 column (Y axis) after the other.  If
		//the Y axis number of pixels is not a multiple of 8 bits then the last byte of each column of bitmap data will
		//contain unused bits to pad the colum length to be byte alligned.
	
		//----- Calculate the number of bytes of bitmap data to read per column -----
		if ((image_height & 0x07) == 0)
			y_no_of_bytes_to_read_per_column = (image_height >> 3);
		else
			y_no_of_bytes_to_read_per_column = (image_height >> 3) + 1;
	
		//------------------------------------
		//----- READ EACH COLUMN LOOP -----
		//------------------------------------
		for (x_byte_coordinate = x_start_coord; (x_byte_coordinate - x_start_coord) < image_width; x_byte_coordinate++)
		{
			prop_width_this_x_row_number = 0;
			//------------------------------------
			//----- READ THIS COLUMN LOOP -----
			//------------------------------------
			for (y_byte_number = 0; y_byte_number < y_no_of_bytes_to_read_per_column; y_byte_number++)
			{
				//----- GET THE NEXT BITMAP BYTE FROM FLASH MEMORY AND DO THE BIT SHIFT -----
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
				prop_width_number_of_bytes_read++;
	
				//CHECK THE BITMAP DATA
				//Read each column bit by bit and store the first and last pixel number
				for (b_temp = 0x80; b_temp > 0; b_temp >>= 1)						//Do for each bit
				{
					if (bitmap_memory_data & b_temp)
					{
						if (prop_width_this_x_row_number < prop_width_ascii_first_used_row)
							prop_width_ascii_first_used_row = prop_width_this_x_row_number;
						if (prop_width_this_x_row_number > prop_width_ascii_last_used_row)
							prop_width_ascii_last_used_row = prop_width_this_x_row_number;
					}
					prop_width_this_x_row_number++;
				}
	
				//For each Y column see if at least 1 pixel is on.
				//If yes and the column first pixel number is blank then store this as the first row.
				//If yes store this as the last column
				if (bitmap_memory_data)
				{
					//At least 1 pixel is on in this column
					prop_width_ascii_last_used_column = prop_width_this_y_column_number;		//Store this as the current last column (will be overwritten if further columns have pixels on)
					if (prop_width_ascii_first_used_column == 0xff)
						prop_width_ascii_first_used_column = prop_width_this_y_column_number;	//Store this as the first column found with a pixel on
				}
	
			}
			
			prop_width_this_y_column_number++;
		}


		//--------------------------------------------------------------------------------
		//----- RESTORE THE MEMORY ADDRESS BACK TO THE START OF THE CHARACTER BITMAP -----
		//--------------------------------------------------------------------------------
		if (p_image_memory_address != 0)
		{
			//----- BITMAP IS IN PROGRAM MEMORY -----
			p_image_memory_address -= prop_width_number_of_bytes_read;
		}
		#ifdef DISPLAY_USING_FLASH_MEMORY
			else
			{
				//----- BITMAP IS IN FLASH MEMORY -----
				image_flash_address -= prop_width_number_of_bytes_read;
			}
		#endif


		//FROM THE ABOVE CODE WE NOW HAVE:-
		//prop_width_ascii_first_used_row 		X Axis (0xff = none found)
		//prop_width_ascii_last_used_row		X Axis (0 = none found)
		//prop_width_ascii_first_used_column	Y Axis (column of bytes) (0xff = non found)
		//prop_width_ascii_last_used_column		Y Axis (column of bytes) (0 = non found)


		if ((prop_width_ascii_last_used_row == 0) || (prop_width_ascii_first_used_row == 0xff))
		{
			//One of the row values has not been found so force to full image width (this is blank space character)
			prop_width_ascii_first_used_row = 0;
			prop_width_ascii_last_used_row = image_height - 1;
		}

		if ((prop_width_ascii_last_used_column == 0) || (prop_width_ascii_first_used_column == 0xff))
		{
			//One of the column values has not been found so force to full image width (this is blank space character)
			prop_width_ascii_first_used_column = 0;
			prop_width_ascii_last_used_column = image_width - 1;
		}


		if (image_options & DISPLAY_STRING_SCROLL)
		{
			//-------------------------------------
			//----- DISPLAYING SCROLLING TEXT -----
			//-------------------------------------
			//Scrolling text must be proportional so we simply alter the proportional first and last values just found and the functionality
			//in the remainder of this function will automatically deal with display of part characters, in the same way as it deals with
			//displaying proportional characters (this is the only section that deals with scrolling text).

			//Set default auto coordinates for next time in case scrolling text exits without displaying
			//(due to there being leading pixels to skip)
			display_auto_x_coordinate = x_start_coord;
			display_auto_y_coordinate = y_start_coord;
	
	
			if (image_options & DISPLAY_STRING_ON_Y_AXIS)
			{
				//--------------------------------
				//----- DISPLAYING ON Y AXIS -----
				//--------------------------------
				if (display_scroll_text_skip_pixels_count)
				{
					//----- CURRENTLY IGNORING LEADING PIXELS -----
					if (((prop_width_ascii_last_used_row - prop_width_ascii_first_used_row) + 1) <= display_scroll_text_skip_pixels_count)
					{
						//THERE ARE MORE PIXELS TO IGNORE THAN THE WIDTH OF THIS CHARACTER
						//Update values and exit without displaying
						display_scroll_text_skip_pixels_count -= ((prop_width_ascii_last_used_row - prop_width_ascii_first_used_row) + 1);
						
						//Flag to main function that it can remove the 1st character of the scrolling text
						if (display_scroll_1st_char_done_skip_pixels == 0)
						{
							if (display_scroll_text_skip_pixels_count)
								display_scroll_1st_char_done_skip_pixels = display_scroll_text_skip_pixels_count;
							else
								display_scroll_1st_char_done_skip_pixels = 0xff;		//Special value if 1st character needs to be removed and skip pixels count should be zero
						}
						
						return;
					}
					else
					{
						//THIS CHARACTER IS WIDER THAN THE NUMBER OF PIXELS LEFT TO IGNORE SO DISPLAY THE REMAINING PART OF IT
						#ifdef INVERT_Y_AXIS_COORDINATES
							prop_width_ascii_last_used_row -= display_scroll_text_skip_pixels_count;
						#else
							prop_width_ascii_first_used_row += display_scroll_text_skip_pixels_count;
						#endif
						
						display_scroll_text_skip_pixels_count = 0;
					}
				}
				//----- CHECK FOR REMOVE TRAILING PIXELS -----
				if (display_scroll_text_display_pixels_count == 0)
				{
					//ALREADY DISPLAYED ALL OF THE PIXELS TO INCUDE TO DON'T DISPLAY THIS CHARACTER
					return;
				}
				else if (display_scroll_text_display_pixels_count > ((prop_width_ascii_last_used_row - prop_width_ascii_first_used_row) + 1))
				{
					//THE WIDTH OF THIS CHARACTER IS LESS THAN THE REMAINING PIXELS TO DISPLAY
					//Update values and display it as normal
					display_scroll_text_display_pixels_count -= ((prop_width_ascii_last_used_row - prop_width_ascii_first_used_row) + 1);
				}
				else
				{
					//THIS CHARACTER IS WIDER THAN OR EQUAL TO THE NUMBER OF PIXELS LEFT TO DISPLAY SO DISPLAY THE LEADING PART OF IT
					#ifdef INVERT_Y_AXIS_COORDINATES
						prop_width_ascii_first_used_row = prop_width_ascii_last_used_row - (display_scroll_text_display_pixels_count - 1);
					#else
						prop_width_ascii_last_used_row = prop_width_ascii_first_used_row + (display_scroll_text_display_pixels_count - 1);
					#endif

					display_scroll_text_display_pixels_count = 0;
				}
			}
			else
			{
				//--------------------------------
				//----- DISPLAYING ON X AXIS -----
				//--------------------------------
				if (display_scroll_text_skip_pixels_count)
				{
					//----- CURRENTLY IGNORING LEADING PIXELS -----
					if (((prop_width_ascii_last_used_column - prop_width_ascii_first_used_column) + 1) <= display_scroll_text_skip_pixels_count)
					{
						//THERE ARE MORE PIXELS TO IGNORE THAN THE WIDTH OF THIS CHARACTER
						//Update values and exit without displaying
						display_scroll_text_skip_pixels_count -= ((prop_width_ascii_last_used_column - prop_width_ascii_first_used_column) + 1);
						
						//Flag to main function that it can remove the 1st character of the scrolling text
						if (display_scroll_1st_char_done_skip_pixels == 0)
						{
							if (display_scroll_text_skip_pixels_count)
								display_scroll_1st_char_done_skip_pixels = display_scroll_text_skip_pixels_count;
							else
								display_scroll_1st_char_done_skip_pixels = 0xff;		//Special value if 1st character needs to be removed and skip pixels count should be zero
						}
						
						return;
					}
					else
					{
						//THIS CHARACTER IS WIDER THAN THE NUMBER OF PIXELS LEFT TO IGNORE SO DISPLAY THE REMAINING PART OF IT
						prop_width_ascii_first_used_column += display_scroll_text_skip_pixels_count;
						display_scroll_text_skip_pixels_count = 0;
					}
				}
				//----- CHECK FOR REMOVE TRAILING PIXELS -----
				if (display_scroll_text_display_pixels_count == 0)
				{
					//ALREADY DISPLAYED ALL OF THE PIXELS TO INCUDE TO DON'T DISPLAY THIS CHARACTER
					return;
				}
				else if (display_scroll_text_display_pixels_count > ((prop_width_ascii_last_used_column - prop_width_ascii_first_used_column) + 1))
				{
					//THE WIDTH OF THIS CHARACTER IS LESS THAN THE REMAINING PIXELS TO DISPLAY
					//Update values and display it as normal
					display_scroll_text_display_pixels_count -= ((prop_width_ascii_last_used_column - prop_width_ascii_first_used_column) + 1);
				}
				else
				{
					//THIS CHARACTER IS WIDER THAN OR EQUAL TO THE NUMBER OF PIXELS LEFT TO DISPLAY SO DISPLAY THE LEADING PART OF IT
					prop_width_ascii_last_used_column = prop_width_ascii_first_used_column + (display_scroll_text_display_pixels_count - 1);
					display_scroll_text_display_pixels_count = 0;
				}
			}
		}


		//---------------------------------------------------------------
		//----- DEAL WITH APPLYING PROPORTIONAL WIDTH IN THE X AXIS -----
		//---------------------------------------------------------------
		//The Y axis is easy to modify as we simply remove unused columns.  The X axis is more complex as this is the axis
		//that is displayed in bytes - it is dealt with in the display bitmap data code below.
		if ((image_options & DISPLAY_STRING_ON_Y_AXIS) == 0)
		{
			//----- UPDATE THE IMAGE WIDTH VALUE -----
			image_width -= ((image_width - prop_width_ascii_last_used_column) - 1);
			image_width -= prop_width_ascii_first_used_column;

			//----- OFFSET THE MEMORY START ADDRESS TO THE FIRST ROW TO BE USED FOR THE BITMAP -----
			if (p_image_memory_address != 0)
			{
				//----- BITMAP IS IN PROGRAM MEMORY -----
				p_image_memory_address += (prop_width_ascii_first_used_column * ((image_height >> 3) + 1));
			}
			#ifdef DISPLAY_USING_FLASH_MEMORY
			else
			{
				//----- BITMAP IS IN FLASH MEMORY -----
				image_flash_address += (prop_width_ascii_first_used_column * ((image_height >> 3) + 1));
			}
			#endif
		}

	}

	//-------------------------------------------------------------------------
	//----- MODIFY THE Y START COORDINATE IF DEFINED TO DISPLAY IN REVERSE -----
	//-------------------------------------------------------------------------
	#ifdef INVERT_Y_AXIS_COORDINATES
		y_start_coord -= (image_height - 1);
		if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL) && (image_options & DISPLAY_STRING_ON_Y_AXIS))
		{
			//MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
			y_start_coord += ((image_height - 1) - (prop_width_ascii_last_used_row - prop_width_ascii_first_used_row));
		}
	#endif



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

	if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL) && (image_options & DISPLAY_STRING_ON_Y_AXIS))
	{
		//MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
		y_coord_shift_right_no_of_bits = ((y_start_coord - prop_width_ascii_first_used_row) & 0x07);
	}


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


	//------------------------------------
	//----- DISPLAY EACH COLUMN LOOP -----
	//------------------------------------
	for (x_byte_coordinate = x_start_coord; (x_byte_coordinate - x_start_coord) < image_width; x_byte_coordinate++)
	{
		//RESET THE BIT SHIFT REGISTERS READY FOR THE NEW COLUMN
		y_bits_to_shift_into_next_byte = 0;

		if (image_options & 0x007f)
		{
			if ((image_options & DISPLAY_STRING_PROPORTIONAL) && (image_options & DISPLAY_STRING_ON_Y_AXIS))		//(If's seperated to speed up display of non text bitmaps)
			{
				//SETUP REGISTERS FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
				prop_width_ascii_leading_bits_to_mask = prop_width_ascii_first_used_row + y_coord_shift_right_no_of_bits;	
				prop_width_ascii_trailing_bits_to_mask = (image_height - prop_width_ascii_last_used_row) - 1;
			}
		}

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

				//--------------------------------------------------
				//----- INVERT THE PIXELS IF OPTION BIT IS SET -----
				//--------------------------------------------------
				if (image_options & 0x0080)
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

			if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL) && (image_options & DISPLAY_STRING_ON_Y_AXIS))
			{
				//------------------------------------------------------------------------
				//----- MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS -----
				//------------------------------------------------------------------------
				//DON'T DISPLAY THE LEADING UNUSED BITS (retain what is already being displayed)
				if (prop_width_ascii_leading_bits_to_mask)
				{
					b_temp = prop_width_ascii_leading_bits_to_mask;
					if (b_temp > 8)
						b_temp = 8;
	
					b_temp1 = 0x80;
					for ( ; b_temp > 0; b_temp--)
					{
						byte_mask &= ~b_temp1;
						b_temp1 >>= 1;
						prop_width_ascii_leading_bits_to_mask--;
					}
				}

				//DON'T DISPLAY THE TRAILING UNUSED BITS (retain what is already being displayed)
				if (prop_width_ascii_trailing_bits_to_mask)
				{

					//Calculate the number of bits of bitmap data remaining to be displayed in this column
					w_temp = image_height + y_coord_shift_right_no_of_bits - (y_byte_number << 3);
					
					if (
						((w_temp - prop_width_ascii_trailing_bits_to_mask) < 8) &&
						(prop_width_ascii_trailing_bits_to_mask)
						)
					{
						//WE NEED TO MASK BITS FROM THIS BYTE
						b_temp = (8 - (w_temp - prop_width_ascii_trailing_bits_to_mask));
						prop_width_ascii_trailing_bits_to_mask -= b_temp;

						b_temp1 = 0xff;
						for ( ; b_temp > 0; b_temp--)			//(We have to do a for loop as a b_temp value of 8 results in no shifting with the C18 compiler)
							b_temp1 <<= 1;

						byte_mask &= b_temp1;
					}


				}
			}

			//-----------------------------------------
			//----- WRITE THE BYTE TO THE DISPLAY -----
			//-----------------------------------------
			if ((image_options & 0x0c00) == 0)  //Don't output if option bits 11 or 10 are set
			{
				if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL) && (image_options & DISPLAY_STRING_ON_Y_AXIS))
				{
					//----- MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS -----
					//(Deal with possible negative y coord values due to the first bits of a start of line character being off the screen)
					display_write_bitmap_byte(byte_mask, bitmap_memory_data, x_byte_coordinate,
					(
					((y_start_coord - prop_width_ascii_first_used_row)
					+ (y_byte_number << 3))
					>> 3)
					);
				}
				else
				{
					//----- NORMAL DISPLAY -----
					display_write_bitmap_byte(byte_mask, bitmap_memory_data, x_byte_coordinate, ((y_start_coord >> 3) + y_byte_number));
				}
			}


			//-------------------------------------------------
			//----- SETUP THE BIT SHIFT FOR THE NEXT BYTE -----
			//-------------------------------------------------
			y_bits_to_shift_into_next_byte = y_bits_into_next_byte_working;

		}
	}

	//------------------------------------------
	//----- FINISHED DISPLAYING THE BITMAP -----
	//------------------------------------------

	//----- UPDATE THE display_auto_x_coordinate & display_auto_y_coordinate VARIABLES -----
	display_auto_x_coordinate = x_byte_coordinate;

	#ifdef INVERT_Y_AXIS_COORDINATES
		//----- Y AXIS COORDINATE IS REVERSED -----
		if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL) && (image_options & DISPLAY_STRING_ON_Y_AXIS))
		{
			//MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
			y_start_coord -= (image_height - (prop_width_ascii_last_used_row - prop_width_ascii_first_used_row));

			display_auto_y_coordinate = y_start_coord;

			//MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
			display_auto_y_coordinate += ((image_height - prop_width_ascii_last_used_row) - 1);
			display_auto_y_coordinate += prop_width_ascii_first_used_row;
		}
		else
		{
			display_auto_y_coordinate = y_start_coord - 1;
		}
	#else
		//----- Y AXIS COORDINATE IS NORMAL -----
		display_auto_y_coordinate = y_start_coord + image_height;
	
		if ((image_options & 0x007f) && (image_options & DISPLAY_STRING_PROPORTIONAL) && (image_options & DISPLAY_STRING_ON_Y_AXIS))
		{
			//MODIFY FOR PROPORTIONAL WIDTH TEXT BEING DISPLAYED ON Y AXIS
			display_auto_y_coordinate -= ((image_height - prop_width_ascii_last_used_row) - 1);
			display_auto_y_coordinate -= prop_width_ascii_first_used_row;
		}
	#endif


}






//*******************************************
//*******************************************
//********** DISPLAY ASCII STRING  **********
//*******************************************
//*******************************************
//
//image_flash_address			Start address of bitmap in flash memory (set to 0 if bitmap is in program memory))
//*image_memory_address			Pointer to bitmap in program memory (set to 0 if bitmap is in flash memory)
//
//image_options		Bits15:12	Unused
//					Bit11		Right Align
//					Bit10		Centre Align
//					Bit9		0 = Monospace / fixed pitch characters, 1 = proportionally spaced characters
//					Bit8		0 = Display string along X axis, 1 = Display string along Y axis
//					Bit7		1 = Invert pixels (i.e. a pixel on becomes off and vice versa)
//					Bits6:0		Not available (used by display bitmap function)
//
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
void display_const_string (DWORD image_flash_address, CONSTANT BYTE *p_image_memory_address, WORD image_options, 
							   WORD x_start_coord, WORD y_start_coord, CONSTANT BYTE *p_ascii_string)
{
	BYTE next_character;
	CONSTANT BYTE *p_ascii_string_copy;
	BYTE doing_leading_blank_bar = 1;
	
	p_ascii_string_copy = p_ascii_string;

	if (image_options & DISPLAY_STRING_SCROLL)
	{
		//----- DISPLAYING SCROLLING TEXT - FORCE TO PROPORTAIONAL AND LEFT ALIGNED -----
		image_options &= ~(DISPLAY_STRING_CENTER_ALIGN | DISPLAY_STRING_RIGHT_ALIGN);
		image_options |= DISPLAY_STRING_PROPORTIONAL;
		display_scroll_text_skip_pixels_count = display_scroll_text_start_pixel;		//Setup for new scrolling text display
		display_scroll_1st_char_done_skip_pixels = 0;
	}


display_const_string_loop:
	//----- GET THE FIRST CHARACTER AND DISPLAY IT -----
	if ((image_options & DISPLAY_STRING_SCROLL) == 0)			//Don't do for scrolling text as we want a leading space
	{
		next_character = *p_ascii_string++;
		if (next_character <= 0x00)			//Check for null terminator
			return;

		display_bitmap(image_flash_address, p_image_memory_address, (image_options | next_character), x_start_coord, y_start_coord);
	}
	else
	{
		display_auto_x_coordinate = x_start_coord;
		display_auto_y_coordinate = y_start_coord;
	}

	while (1)
	{

		//----- GET THE NEXT CHARACTER -----
		next_character = *p_ascii_string++;
		if (next_character <= 0x00)
			break;

		//----- DO SPECIAL BLANK BAR CHARACTER BETWEEN EACH ACTUAL CHARACTER ----
		//(Don't do for proportaion width text 'space' character as it looks better without)
		if (((image_options & DISPLAY_STRING_PROPORTIONAL) == 0) || (next_character != ' '))
		{
			if (image_options & DISPLAY_STRING_ON_Y_AXIS)
			{
				display_bitmap(image_flash_address, p_image_memory_address,
									((image_options & DISPLAY_STRING_ON_Y_AXIS) | (image_options ^ DISPLAY_STRING_INVERT_PIXELS_ON) | DISPLAY_STRING_PROPORTIONAL | 0x7f),
									x_start_coord, 0xffff);
			}
			else
			{
				display_bitmap(image_flash_address, p_image_memory_address,
									((image_options & DISPLAY_STRING_ON_Y_AXIS) | (image_options ^ DISPLAY_STRING_INVERT_PIXELS_ON) | DISPLAY_STRING_PROPORTIONAL | 0x7f),
									0xffff, y_start_coord);
			}
			
			//Avoid the leading blank bar being off screen affecting the remove first scrolling character flag to the main function
			if (doing_leading_blank_bar && display_scroll_1st_char_done_skip_pixels)
				display_scroll_1st_char_done_skip_pixels = 0;
		}
		doing_leading_blank_bar = 0;

		//----- DISPLAY CHARACTER -----
		if (image_options & DISPLAY_STRING_ON_Y_AXIS)
			display_bitmap(image_flash_address, p_image_memory_address, (image_options | next_character), x_start_coord, 0xffff);
		else
			display_bitmap(image_flash_address, p_image_memory_address, (image_options | next_character), 0xffff, y_start_coord);


		if ((image_options & DISPLAY_STRING_SCROLL) && (display_scroll_text_display_pixels_count == 0))
		{
			//DISPLAYING SCROLLING TEXT - ALREADY DISPLAYED ALL OF THE PIXELS TO INCUDE TO NO NEED TO CONTINUE
			break;
		}

	}

	if (image_options & 0x0400)
	{
		//-------------------------------------
		//----- DOING CENTER ALIGNED TEXT -----
		//-------------------------------------
		//Because the align bit was set the display bitmap fucntion will not have actually outputted the text, but we now know how long the text was on the screen
		if (image_options & DISPLAY_STRING_ON_Y_AXIS)
		{
			#ifdef INVERT_Y_AXIS_COORDINATES
				y_start_coord += (y_start_coord - display_auto_y_coordinate) >> 1;
			#else
				y_start_coord -= (display_auto_y_coordinate - y_start_coord) >> 1;
			#endif
		}
		else
		{
			x_start_coord -= (display_auto_x_coordinate - x_start_coord) >> 1; 
		}

		p_ascii_string = p_ascii_string_copy;
		image_options &= 0xf3ff;				//Remove the align bits
		goto display_const_string_loop;		//Re run this function, this time with the display being updated
	}
	else if (image_options & 0x0800)
	{
		//-------------------------------------
		//----- DOING RIGHT ALIGNED TEXT ------
		//-------------------------------------
		//Because the align bit was set the display bitmap fucntion will not have actually outputted the text, but we now know how long the text was on the screen
		if (image_options & DISPLAY_STRING_ON_Y_AXIS)
		{
			#ifdef INVERT_Y_AXIS_COORDINATES
				y_start_coord += (y_start_coord - display_auto_y_coordinate);
			#else
				y_start_coord -= (display_auto_y_coordinate - y_start_coord);
			#endif
		}
		else
		{
			x_start_coord -= (display_auto_x_coordinate - x_start_coord); 
		}

		p_ascii_string = p_ascii_string_copy;
		image_options &= 0xf3ff;				//Remove the align bits
		goto display_const_string_loop;		//Re run this function, this time with the display being updated
	}

}


//----------------------------------------
//----------------------------------------
//----- VERSION FOR VARIABLE STRINGS -----
//----------------------------------------
//----------------------------------------
void display_variable_string (DWORD image_flash_address, CONSTANT BYTE *p_image_memory_address, WORD image_options,
								  WORD x_start_coord, WORD y_start_coord, BYTE *p_ascii_string)
{
	BYTE next_character;
	BYTE *p_ascii_string_copy;
	BYTE doing_leading_blank_bar = 1;
	
	p_ascii_string_copy = p_ascii_string;


	if (image_options & DISPLAY_STRING_SCROLL)
	{
		//----- DISPLAYING SCROLLING TEXT - FORCE TO PROPORTAIONAL AND LEFT ALIGNED -----
		image_options &= ~(DISPLAY_STRING_CENTER_ALIGN | DISPLAY_STRING_RIGHT_ALIGN);
		image_options |= DISPLAY_STRING_PROPORTIONAL;
		display_scroll_text_skip_pixels_count = display_scroll_text_start_pixel;		//Setup for new scrolling text display
		display_scroll_1st_char_done_skip_pixels = 0;
	}


display_variable_string_loop:
	//----- GET THE FIRST CHARACTER AND DISPLAY IT -----
	if ((image_options & DISPLAY_STRING_SCROLL) == 0)			//Don't do for scrolling text as we want a leading space
	{
		next_character = *p_ascii_string++;
		if (next_character <= 0x00)			//Check for null terminator
			return;

		display_bitmap(image_flash_address, p_image_memory_address, (image_options | next_character), x_start_coord, y_start_coord);
	}
	else
	{
		display_auto_x_coordinate = x_start_coord;
		display_auto_y_coordinate = y_start_coord;
	}

	while (1)
	{
		//----- GET THE NEXT CHARACTER -----
		next_character = *p_ascii_string++;
		if (next_character <= 0x00)
			break;

		//----- DO SPECIAL BLANK BAR CHARACTER BETWEEN EACH ACTUAL CHARACTER ----
		//(Don't do for proportaion width text 'space' character as it looks better without)
		if (((image_options & DISPLAY_STRING_PROPORTIONAL) == 0) || (next_character != ' '))
		{
			if (image_options & DISPLAY_STRING_ON_Y_AXIS)
			{
				display_bitmap(image_flash_address, p_image_memory_address,
									((image_options & DISPLAY_STRING_ON_Y_AXIS) | (image_options ^ DISPLAY_STRING_INVERT_PIXELS_ON) | DISPLAY_STRING_PROPORTIONAL | 0x7f),
									x_start_coord, 0xffff);
			}
			else
			{
				display_bitmap(image_flash_address, p_image_memory_address,
									((image_options & DISPLAY_STRING_ON_Y_AXIS) | (image_options ^ DISPLAY_STRING_INVERT_PIXELS_ON) | DISPLAY_STRING_PROPORTIONAL | 0x7f),
									0xffff, y_start_coord);
			}
			
			//Avoid the leading blank bar being off screen affecting the remove first scrolling character flag to the main function
			if (doing_leading_blank_bar && display_scroll_1st_char_done_skip_pixels)
				display_scroll_1st_char_done_skip_pixels = 0;
		}
		doing_leading_blank_bar = 0;

		//----- DISPLAY CHARACTER -----
		if (image_options & DISPLAY_STRING_ON_Y_AXIS)
			display_bitmap(image_flash_address, p_image_memory_address, (image_options | next_character), x_start_coord, 0xffff);
		else
			display_bitmap(image_flash_address, p_image_memory_address, (image_options | next_character), 0xffff, y_start_coord);

		if ((image_options & DISPLAY_STRING_SCROLL) && (display_scroll_text_display_pixels_count == 0))
		{
			//DISPLAYING SCROLLING TEXT - ALREADY DISPLAYED ALL OF THE PIXELS TO INCUDE TO NO NEED TO CONTINUE
			break;
		}

	}

	if (image_options & 0x0400)
	{
		//-------------------------------------
		//----- DOING CENTER ALIGNED TEXT -----
		//-------------------------------------
		//Because the align bit was set the display bitmap fucntion will not have actually outputted the text, but we now know how long the text was on the screen
		if (image_options & DISPLAY_STRING_ON_Y_AXIS)
		{
			#ifdef INVERT_Y_AXIS_COORDINATES
				y_start_coord += (y_start_coord - display_auto_y_coordinate) >> 1;
			#else
				y_start_coord -= (display_auto_y_coordinate - y_start_coord) >> 1;
			#endif
		}
		else
		{
			x_start_coord -= (display_auto_x_coordinate - x_start_coord) >> 1; 
		}

		p_ascii_string = p_ascii_string_copy;
		image_options &= 0xf3ff;				//Remove the align bits
		goto display_variable_string_loop;	//Re run this function, this time with the display being updated
	}
	else if (image_options & 0x0800)
	{
		//-------------------------------------
		//----- DOING RIGHT ALIGNED TEXT ------
		//-------------------------------------
		//Because the align bit was set the display bitmap fucntion will not have actually outputted the text, but we now know how long the text was on the screen
		if (image_options & DISPLAY_STRING_ON_Y_AXIS)
		{
			#ifdef INVERT_Y_AXIS_COORDINATES
				y_start_coord += (y_start_coord - display_auto_y_coordinate);
			#else
				y_start_coord -= (display_auto_y_coordinate - y_start_coord);
			#endif
		}
		else
		{
			x_start_coord -= (display_auto_x_coordinate - x_start_coord); 
		}

		p_ascii_string = p_ascii_string_copy;
		image_options &= 0xf3ff;				//Remove the align bits
		goto display_variable_string_loop;	//Re run this function, this time with the display being updated
	}

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
		display_write_bitmap_byte(0xff, 0x00, byte_ordering_test_last_x_coord, byte_ordering_test_last_y_coord);


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
	display_write_bitmap_byte(0xff, 0x8f, byte_ordering_test_last_x_coord, byte_ordering_test_last_y_coord);

}
#endif












