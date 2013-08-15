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
//---------- SCREEN:	BATRON BTHQ100032V								----------
//---------- Resolution:	100 x 32 pixel								----------
//---------- Colour:		Black on White								----------
//---------- Driver IC:		SED1520 (x2)								----------
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

	//----- SETUP CONTROL PINS -----
	DISPLAY_A0(0);					// instruction mode
	DISPLAY_RW(1);					// not writiing instruction yet
	DISPLAY_CS1(1);
	DISPLAY_CS2(1);

	//----- DO THE RESET PIN -----
	//None

	//----- PAUSE #mS BEFORE ENABLING V0 (The negative display driver voltage) -----
	display_delay_ms(50);

	//----- DO THE CONFIGURATION COMMAND SEQUENCE -----
	display_write_command(0xE2);			//0xE2 = internal reset command
	display_delay_ms(10);

	display_write_command(0xAF);			//0xAF = switch on display command

	display_write_command(0xA0);			//0xA0 = set display RAM address SEG output to normal command
	display_delay_ms(10);

}




//**********************************
//**********************************
//********** SET CONTRAST **********
//**********************************
//**********************************
void display_model_set_contrast(BYTE contrast_value)
{

	//Contrast is set by external voltage for this screen

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
		if (bitmap_mask == 0xff)
		{
			//Mask is all bits to be written so skip the read as its unessary
			display_working_byte_value = bitmap_data;
		}
		else
		{
			//SET DATA BUS INPUTS
			DISPLAY_DATA_BUS_TRIS(0xFF);

			//SETUP CONTROL PINS
			DISPLAY_A0(1);
			DISPLAY_RW(1);

			//GET THE BYTE
			//Read from the correct driver IC (this display has 2 driver IC's)
			if(((DISPLAY_WIDTH_PIXELS - 1) - x_byte_coord) < 50)
			{
				DISPLAY_CS1(0);				//Need to do a dummy read first
				DISPLAY_BUS_ACCESS_DELAY;
				DISPLAY_CS1(1);
				DISPLAY_BUS_ACCESS_DELAY;
				
				DISPLAY_CS1(0);
				DISPLAY_BUS_ACCESS_DELAY;
				display_working_byte_value = DISPLAY_DATA_BUS_IP;
				DISPLAY_CS1(1);
			}
			else
			{
				DISPLAY_CS2(0);				//Need to do a dummy read first
				DISPLAY_BUS_ACCESS_DELAY;
				DISPLAY_CS2(1);
				DISPLAY_BUS_ACCESS_DELAY;

				DISPLAY_CS2(0);
				DISPLAY_BUS_ACCESS_DELAY;
				display_working_byte_value = DISPLAY_DATA_BUS_IP;
				DISPLAY_CS2(1);
			}

			//SET DATA BUS BACK TO OUTPUTS
			DISPLAY_DATA_BUS_TRIS(0x00);


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
	DISPLAY_RW(1);
	DISPLAY_A0(1);					//A0 high for display data

	//LATCH IN THE BYTE
	DISPLAY_RW(0);
	//Write to the correct driver IC (this display has 2 driver IC's)
	if(((DISPLAY_WIDTH_PIXELS - 1) - x_byte_coord) < 50)
	{
		DISPLAY_CS1(0);
		DISPLAY_BUS_ACCESS_DELAY;
		DISPLAY_CS1(1);
	}
	else
	{
		DISPLAY_CS2(0);
		DISPLAY_BUS_ACCESS_DELAY;
		DISPLAY_CS2(1);
	}
	DISPLAY_RW(1);

	#ifdef DISPLAY_USE_LOCAL_RAM_BUFFER
		//----- WE ARE USING A LOCAL RAM BUFFER COPY THE DISPLAY DATA TO IT -----
		*p_local_ram_buffer = display_working_byte_value;
	#endif

}




//******************************************
//******************************************
//********** SET DISPLAY ADDRESS  **********
//******************************************
//******************************************
void display_set_address (WORD x_coord, WORD y_coord_page)
{
	WORD modifed_x_coord;
	WORD modifed_y_coord_page;

	//----- ENSURE X AND Y COORDINATES ARE IN RANGE ----
	if(x_coord > (DISPLAY_WIDTH_PIXELS - 1))
		x_coord = (DISPLAY_WIDTH_PIXELS - 1);
	if (y_coord_page > ((DISPLAY_HEIGHT_PIXELS >> 3) - 1))
		y_coord_page = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1);



	//----- MODIFY THE X COORDINATE TO SUIT THIS DISPLAY -----
	modifed_x_coord = x_coord + DISPLAY_WIDTH_START_OFFSET;
	modifed_x_coord = (DISPLAY_WIDTH_PIXELS - 1) - modifed_x_coord;		//The lines are displayed in reverse order for this screen so flip the X axis to correct
	if(modifed_x_coord > 49)											//Each DISPLAY controller IC does 50 pixels
		modifed_x_coord -= 50;


	//----- MODIFY THE Y COORDINATE TO SUIT THIS DISPLAY -----
	//modifed_y_coord_page = y_coord_page;
	modifed_y_coord_page = ((DISPLAY_HEIGHT_PIXELS >> 3) - 1) - y_coord_page;	//The byte is displayed in reverse order for this screen so flip the Y axis to correct

	//----- DO SET PAGE COMMAND(0-3) -----
	display_write_command(0xB8 | modifed_y_coord_page);					//0xB8 = page set command, the last 2 bits is addr of page


	//----- DO SET LINE COMMAND (0-49) -----
	display_write_command(0x00 | modifed_x_coord);						//0x00 = line set command, the last 7 bits is the addr of the line

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
	//(This display uses 2 driver IC's and commands get sent to both at once)

	//LOAD THE OUTPUT DATA ONTO THE BUS
	DISPLAY_DATA_BUS_OP(data);

	//SETUP CONTROL PINS
	DISPLAY_RW(1);
	DISPLAY_A0(0);						//A0 low for a command

	//LATCH IN THE BYTE
	//(For this display the CS lines are not chip select, but actually enable lines and are
	//what is used to clock in the command)
	DISPLAY_RW(0);
	DISPLAY_CS1(0);
	DISPLAY_CS2(0);

	DISPLAY_BUS_ACCESS_DELAY;						//Pause

	DISPLAY_CS1(1);
	DISPLAY_CS2(1);
	DISPLAY_RW(1);




}
