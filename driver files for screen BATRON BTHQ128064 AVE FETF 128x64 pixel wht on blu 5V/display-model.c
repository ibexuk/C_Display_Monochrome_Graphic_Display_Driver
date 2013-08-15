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
//SCREEN MODEL C CODE FILE





//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//---------- THIS FILE CONTAINS SCREEN MODEL SPECIFIC FUNCTIONS FOR 	----------
//---------- SCREEN:	BATRON BTHQ128064 AVE FETF						----------
//---------- Resolution:	128 x 64 pixel								----------
//---------- Colour:		White on Blue								----------
//---------- Driver IC:		SED1565D0B									----------
//---------- Interface:		Parallel									----------
//---------- Voltage:		5V											----------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------



//----- INCLUDE FILES FOR THIS SOURCE CODE FILE -----
#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
#define	DISPLAY_MODEL_C				//(Define used for following header file to flag that it is the header file for this source file)
#include "display-model.h"			//(Include header file for this source file)
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

	//----- DO THE RESET PIN -----
	DISPLAY_RESET(0);							//Set low to reset

	DISPLAY_CS1(0);
	display_delay_ms(1);

	DISPLAY_RESET(1);							//Set high to run
	
	display_delay_ms(5);

	//----- PAUSE #mS BEFORE ENABLING V0 (The negative driver voltage) -----
	//(Not necessary for this screen)

	//----- DO THE CONFIGURATION COMMAND SEQUENCE -----
	display_write_command(0xa2 | 1);			//0xa2 = set drive voltage bias ratio command (0 = 1/9, 1 = 1/7)

	display_write_command(0xa0 | 0);			//0xa0 = adc command (0 = normal)

	display_write_command(0xc0 | 0);			//0xc0 = common output mode select command (0 = normal)

	display_write_command(0x20 | (4 & 0x07));	//0x20 = set internal Rb/Ra resistor ratio command (0 - 7)

	//Do electronic volume (contrast)
	display_write_command(0x81);				//0x1 = electronic volume set command
	display_write_command(DISPLAY_DEFAULT_CONTRAST & 0x3f);		//electronic volume command (0 - 63, 6bit LSB)
	display_write_command(0xee);				//0xee = clear read / modify / write command


	display_write_command(0xa6 | 0);			//0xa6 = set display display normal / reverse (0 - 1)

	display_write_command(0xac | 0);			//0xac = static indicator on / off (0 - 1)

	display_write_command(0x28 | 4 | 2 | 1);	//0x28 = power control command (internal power supply operating mode) 
												//(bit2 = booster, bit1 = vreg, bit0 = vfollower)

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
	display_write_command(contrast_value & 0x3f); //electronic volume command (0 - 63, 6bit LSB)
	display_write_command(0xee);					//0xee = clear read / modify / write command

}




//***************************************
//***************************************
//********** WRITE BITMAP BYTE **********
//***************************************
//***************************************
//bitmap_mask - bit high = use the bitmap_data value, bit low = use the existing screen value
void display_write_bitmap_byte (BYTE bitmap_mask, BYTE bitmap_data, WORD x_byte_coord, WORD y_byte_coord)
{
	BYTE display_working_byte_value;
	
	#ifdef DISPLAY_USE_LOCAL_RAM_BUFFER
		BYTE *p_local_ram_buffer;
	#endif


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
			p_local_ram_buffer += x_byte_coord * ((DISPLAY_HEIGHT_PIXELS >> 3) + 1);		//DISPLAY_WIDTH_PIXELS is not a multiple of 8 so need to add 1 for remainder
		#else
			p_local_ram_buffer += x_byte_coord * (DISPLAY_HEIGHT_PIXELS >> 3);			//DISPLAY_WIDTH_PIXELS is a multiple of 8
		#endif
		p_local_ram_buffer += (y_byte_coord);

		display_working_byte_value = *p_local_ram_buffer;
	#else
		//---------------------------------------------------------------------------
		//----- NOT USING A LOCAL RAM BUFFER SO READ CURRENT VALUE FROM DISPLAY -----
		//---------------------------------------------------------------------------
		if (bitmap_mask == 0xff)
		{
			//Mask is all bits to be written so skip the read as its unessary
			display_working_byte_value = bitmap_data;
		}
		else
		{
			//SET DATA BUS INPUTS
			DISPLAY_DATA_BUS_TRIS(0xff);

			//SETUP CONTROL PINS
			DISPLAY_CS1(0);
			DISPLAY_RD(1);
			DISPLAY_RW(1);
			DISPLAY_A0(1);						//A0 high for display data

			//GET THE BYTE
			DISPLAY_RD(0);						//Need to do a dummy read first
			DISPLAY_BUS_ACCESS_DELAY;
			DISPLAY_RD(1);
			DISPLAY_BUS_ACCESS_DELAY;

			DISPLAY_RD(0);
			DISPLAY_BUS_ACCESS_DELAY;
			display_working_byte_value = DISPLAY_DATA_BUS_IP;
			DISPLAY_RD(1);

			//SET DATA BUS BACK TO OUTPUTS
			DISPLAY_DATA_BUS_TRIS(0x00);
			DISPLAY_CS1(1);

			//----- SET THE BYTE ADDRESS AGAIN -----
			display_set_address(x_byte_coord, y_byte_coord);
		}
	#endif

	
	//--------------------------
	//----- APPLY THE MASK -----
	//--------------------------
	display_working_byte_value &= ~bitmap_mask;
	display_working_byte_value |= (bitmap_data & bitmap_mask);


	//-----------------------------------------
	//----- WRITE THE BYTE TO THE DISPLAY -----
	//-----------------------------------------
	//(See notes in display_write_command)
	//LOAD THE OUTPUT DATA ONTO THE BUS
	DISPLAY_DATA_BUS_OP(display_working_byte_value);

	//SETUP CONTROL PINS
	DISPLAY_CS1(0);
	DISPLAY_RW(1);
	DISPLAY_RD(1);
	DISPLAY_A0(1);						//A0 high for display data

	//LATCH IN THE BYTE
	DISPLAY_RW(0);
	DISPLAY_BUS_ACCESS_DELAY;
	DISPLAY_RW(1);

	DISPLAY_CS1(1);

	#ifdef DISPLAY_USE_LOCAL_RAM_BUFFER
		//----- WE ARE USING A LOCAL RAM BUFFER COPY THE DISPLAY DATA TO IT -----
		*p_local_ram_buffer = display_working_byte_value;
	#endif

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

	//modifed_x_coord = (DISPLAY_WIDTH_PIXELS - 1) - modifed_x_coord;		//If lines are displayed in reverse order for this screen flip the X axis to correct (not relevant to this screen)

	//----- MODIFY THE Y COORDINATE TO SUIT THIS DISPLAY -----
	//(The code in each of these orinetation defines may need to be changed for different screen models)

	//modifed_y_coord_page = y_coord_page;
	#ifdef ORIENTATION_IS_0
		modifed_y_coord_page = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1) - y_coord_page;		//The byte is displayed in reverse order for this screen so flip the Y axis to correct
	#endif
	#ifdef ORIENTATION_IS_90
		modifed_y_coord_page = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1) - y_coord_page;		//The byte is displayed in reverse order for this screen so flip the Y axis to correct
	#endif
	#ifdef ORIENTATION_IS_180
		modifed_y_coord_page = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1) - y_coord_page;		//The byte is displayed in reverse order for this screen so flip the Y axis to correct
	#endif
	#ifdef ORIENTATION_IS_270
		modifed_y_coord_page = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1) - y_coord_page;		//The byte is displayed in reverse order for this screen so flip the Y axis to correct
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
//The only difference may be setting the A0 data / command line.  However to allow for displayss that aren't
//so normal we use this write command routine.
void display_write_command (BYTE data)
{

	//Load the output data onto the bus
	DISPLAY_DATA_BUS_OP(data);

	//Setup control pins
	DISPLAY_CS1(0);
	DISPLAY_RW(1);
	DISPLAY_RD(1);
	DISPLAY_A0(0);						//A0 low for a command

	//Latch in the byte
	DISPLAY_RW(0);
	DISPLAY_BUS_ACCESS_DELAY;						//Pause
	DISPLAY_RW(1);

	DISPLAY_CS1(1);

}
