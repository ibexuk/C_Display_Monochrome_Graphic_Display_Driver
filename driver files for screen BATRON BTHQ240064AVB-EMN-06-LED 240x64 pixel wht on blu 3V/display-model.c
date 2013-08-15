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
//---------- THIS FILE CONTAINS SCREEN MODEL SPECIFIC FUNCTIONS FOR 	----------
//---------- SCREEN:	BATRON BTHQ240064AVB-EMN-06-LED White-COG		----------
//---------- Resolution:	240 x 64 pixel								----------
//---------- Colour:		White on Blue								----------
//---------- Driver IC:		S1D10605D04B (x2)							----------
//---------- Interface:		Serial (SPI)								----------
//---------- Voltage:		3.0V										----------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------



//----- INCLUDE FILES FOR THIS SOURCE CODE FILE -----
#include "main.h"					//(Include the project global defines file)
#define	DISPLAY_MODEL_C					//(Define used for following header file to flag that it is the header file for this source file)
#include "display-model.h"				//(Include header file for this source file)
#include "display.h"

//----- OTHER PROJECT FILES REQUIRED BY THIS SOURCE CODE FILE -----


//----- COMPILER LIBRARY FILES REQUIRED BY THIS SOURCE CODE FILE -----






//***************************************
//***************************************
//********** INITIALISE SCREEN **********
//***************************************
//***************************************
void display_model_initialise(void)
{

	//----- PAUSE IF NECESSARY FOR DISPLAY TO POWERUP -----
	display_delay_ms(10);

	//----- SETUP CONTROL PINS -----

	//----- DO THE RESET PIN -----
	DISPLAY_RESET(0);							//Set low to reset

	DISPLAY_CS1(0);
	DISPLAY_CS2(0);
	display_delay_ms(1);

	DISPLAY_RESET(1);							//Set high to run
	
	display_delay_ms(5);

	//----- PAUSE #mS BEFORE ENABLING V0 (The negative display driver voltage) -----
	//(Not necessary for this screen)

	//----- DO THE CONFIGURATION COMMAND SEQUENCE -----
	display_write_command(0xe2 | 0);			//0xe2 = reset

	display_write_command(0xa0 | 1);			//0xa0 = adc command (1 = reverse)

	display_write_command(0xc0 | 0);			//0xc0 = common output mode select command (0 = normal)

	display_write_command(0xa6 | 0);			//0xa6 = set display normal / reverse (0 - 1)

	display_write_command(0xa4 | 0);			//0xa4 = display all points (0 = normal)

	display_write_command(0x20 | (4 & 0x07));	//0x20 = set internal Rb/Ra resistor ratio command (0 - 7)

	display_write_command(0xa2 | 0);			//0xa2 = set drive voltage bias ratio command (0 = 1/9, 1 = 1/7)

	display_write_command(0x28 | 4 | 2 | 1);	//0x28 = power control command (internal power supply operating mode) 
												//(bit2 = booster, bit1 = vreg, bit0 = vfollower)

	//Do electronic volume (contrast)
	display_write_command(0x81);				//0x1 = electronic volume set command
	display_write_command(DISPLAY_DEFAULT_CONTRAST & 0x3f);		//electronic volume command (0 - 63, 6bit LSB)
	display_write_command(0xee);				//0xee = clear read / modify / write command

	display_write_command(0xac | 0);			//0xac = static indicator on / off (0 - 1)

	display_write_command(0xae | 1);			//0xae = display on / off command (0 - 1)

	display_write_command(0x40 | (0 & 0x3f));	//0x40 = set display ram display start line

}



//**********************************
//**********************************
//********** SET CONTRAST **********
//**********************************
//**********************************
void display_model_set_contrast(BYTE contrast_value)
{

	//Do electronic volume (contrast)
	display_write_command(0x81);					//0x1 = electronic volume set command
	display_write_command(contrast_value & 0x3f);	//electronic volume command (0 - 63, 6bit LSB)
	display_write_command(0xee);					//0xee = clear read / modify / write command
}



//***************************************
//***************************************
//********** WRITE BITMAP BYTE **********
//***************************************
//***************************************
//bitmap_mask	bit high = use the bitmap_data value, bit low = use the existing screen value
//flags			Bit 0 high = invert existing pixels (ignore bitmap_data).  Other bits unused.
//y_byte_coord is the address of the byte that contains the pixel, not the literal y coordinate, as this axis is byte based.
void display_write_bitmap_byte (BYTE flags, BYTE bitmap_mask, BYTE bitmap_data, WORD x_byte_coord, WORD y_byte_coord)
{
	BYTE display_working_byte_value;
	BYTE data_rx;
	
	#ifdef DISPLAY_USE_LOCAL_RAM_BUFFER
		BYTE *p_local_ram_buffer;
	#endif


	//ENSURE X AND Y COORDINATES ARE IN RANGE
	if(x_byte_coord > (DISPLAY_WIDTH_PIXELS - 1))
		return;
	if (y_byte_coord > ((DISPLAY_HEIGHT_PIXELS >> 3) - 1))
		return;


	//--------------------------------
	//----- SET THE BYTE ADDRESS -----
	//--------------------------------
	display_set_address(x_byte_coord, y_byte_coord);


	//-----------------------------------------------
	//----- READ THE CURRENT DISPLAY BYTE VALUE -----
	//-----------------------------------------------
	#ifdef DISPLAY_USE_LOCAL_RAM_BUFFER
		//-----------------------------------------------------------------------------------------------------
		//----- WE ARE USING A LOCAL RAM BUFFER COPY OF THE DISPLAY DATA SO READ CURRENT VALUE FROM THERE -----
		//-----------------------------------------------------------------------------------------------------
		p_local_ram_buffer = &display_copy_buffer[0][0];						//We use a pointer to allow for compilers that can't access large arrays directly
		#if (DISPLAY_HEIGHT_PIXELS & 0x0007)
			p_local_ram_buffer += x_byte_coord * ((DISPLAY_HEIGHT_PIXELS >> 3) + 1);	//DISPLAY_WIDTH_PIXELS is not a multiple of 8 so need to add 1 for remainder
		#else
			p_local_ram_buffer += x_byte_coord * (DISPLAY_HEIGHT_PIXELS >> 3);			//DISPLAY_WIDTH_PIXELS is a multiple of 8
		#endif
		p_local_ram_buffer += (y_byte_coord);

		display_working_byte_value = *p_local_ram_buffer;
	#else
		//---------------------------------------------------------------------------
		//----- NOT USING A LOCAL RAM BUFFER SO READ CURRENT VALUE FROM DISPLAY -----
		//---------------------------------------------------------------------------
		/*
		if ((bitmap_mask == 0xff) && !(flags & 0x01))
		{
			//Mask is all bits to be written so skip the read as its unessary
			display_working_byte_value = bitmap_data;
		}
		else
		{

		}
		*/

		//NOT POSSIBLE FOR THIS DISPLAY AS DISPLAY DOES NOT PROVIDE MEANS TO READ DATA BACK
		#error DISPLAY_USE_LOCAL_RAM_BUFFER not defined but it must be for this display.
		
	#endif



	//--------------------------
	//----- APPLY THE MASK -----
	//--------------------------
	if (flags & 0x01)
	{
		//Invert existing pixels (ignore bitmap_data)
		bitmap_data = ~display_working_byte_value;
	}
	display_working_byte_value &= ~bitmap_mask;
	display_working_byte_value |= (bitmap_data & bitmap_mask);


	//-----------------------------------------
	//----- WRITE THE BYTE TO THE DISPLAY -----
	//-----------------------------------------
	//(See notes in display_write_command)

	//SETUP CONTROL PINS
	DISPLAY_A0(1);							//A0 high for display data

	//Write to the correct driver IC (this display has 2 driver IC's)
	if(((DISPLAY_WIDTH_PIXELS - 1) - x_byte_coord) >= 120)
		DISPLAY_CS1(0);
	else
		DISPLAY_CS2(0);

	//Send byte
	DISPLAY_SPI_TX_BYTE(display_working_byte_value);

	#ifdef DISPLAY_USE_LOCAL_RAM_BUFFER
		//----- WE ARE USING A LOCAL RAM BUFFER COPY THE DISPLAY DATA TO IT -----
		*p_local_ram_buffer = display_working_byte_value;
	#endif

	//Wait for tx to complete
	while (!DISPLAY_SPI_BUF_FULL)
		;
	
	//Read the received byte (some SPI peripherals require this)
	data_rx = DISPLAY_SPI_RX_BYTE_BUFFER;

	DISPLAY_CS1(1);
	DISPLAY_CS2(1);
}




//**********************************
//**********************************
//********** SET ADDRESS  **********
//**********************************
//**********************************
void display_set_address (WORD x_coord, WORD y_coord_page)
{
	WORD modifed_x_coord;
	WORD modifed_y_coord_page;

	//----- ENSURE X AND Y COORDINATES ARE IN RANGE -----
	if(x_coord > (DISPLAY_WIDTH_PIXELS - 1))
		x_coord = (DISPLAY_WIDTH_PIXELS - 1);
	if (y_coord_page > ((DISPLAY_HEIGHT_PIXELS >> 3) - 1))
		y_coord_page = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1);

	//----- MODIFY THE X COORDINATE TO SUIT THIS DISPLAY -----
	//(The code in each of these orinetation defines may need to be changed for different screen models)
	#ifdef ORIENTATION_IS_0
		modifed_x_coord = x_coord + DISPLAY_WIDTH_START_OFFSET;
	#endif
	#ifdef ORIENTATION_IS_90
		modifed_x_coord = x_coord + DISPLAY_WIDTH_START_OFFSET;
	#endif
	#ifdef ORIENTATION_IS_180
		modifed_x_coord = x_coord + DISPLAY_WIDTH_START_OFFSET;
	#endif
	#ifdef ORIENTATION_IS_270
		modifed_x_coord = (DISPLAY_WIDTH_PIXELS - 1) - x_coord + DISPLAY_WIDTH_START_OFFSET;
	#endif

	modifed_x_coord = (DISPLAY_WIDTH_PIXELS - 1) - modifed_x_coord;		//The lines are displayed in reverse order for this screen so flip the X axis to correct

	if(modifed_x_coord > 119)							//Each controller IC does 120 pixels (this screen has 2 controllers)
		modifed_x_coord -= 120;


	//----- MODIFY THE Y COORDINATE TO SUIT THIS DISPLAY -----
	//(The code in each of these orinetation defines may need to be changed for different screen models)

	//modifed_y_coord_page = y_coord_page;
	#ifdef ORIENTATION_IS_0
		modifed_y_coord_page = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1) - y_coord_page;
	#endif
	#ifdef ORIENTATION_IS_90
		modifed_y_coord_page = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1) - y_coord_page;
	#endif
	#ifdef ORIENTATION_IS_180
		modifed_y_coord_page = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1) - y_coord_page;
	#endif
	#ifdef ORIENTATION_IS_270
		modifed_y_coord_page = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1) - y_coord_page;
	#endif


	//----- DO SET PAGE COMMAND -----
	display_write_command(0xb0 | modifed_y_coord_page);				//Last 4 bit is addr of page

	//----- DO SET LINE COMMANDS -----
	//Column address set lower 4 bits
	display_write_command(0x00 | (modifed_x_coord & 0x0F));			//Last 4 bit is LSB of line address 

	//Column address set upper 4 bits
	display_write_command(0x10 | (modifed_x_coord >> 4));			//Last 4 bit is MSB of line address 

}




//**********************************************
//**********************************************
//********** WRITE COMMAND TO DISPLAY **********
//**********************************************
//**********************************************
//Modify this routine to suit the screen in use
//This routine is usually that same as the instrucitons in the write part of display_write_bitmap_byte
//The only difference may be setting the A0 data / command line.  However to allow for displays that aren't
//so normal we use this write command routine.
void display_write_command (BYTE data)
{
	BYTE data_rx;

	//Setup control pins
	DISPLAY_CS1(0);						//Commands can be sent to both controller IC's every time
	DISPLAY_CS2(0);
	DISPLAY_A0(0);						//A0 low for a command

	//Send byte
	DISPLAY_SPI_TX_BYTE(data);

	//Wait for tx to complete
	while (!DISPLAY_SPI_BUF_FULL)
		;
	
	//Read the received byte (some SPI peripherals require this)
	data_rx = DISPLAY_SPI_RX_BYTE_BUFFER;

	DISPLAY_CS1(1);
	DISPLAY_CS2(1);
}
