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
//						PIC32 SAMPLE PROJECT C CODE FILE
//  Copyright:			EMBEDDED-CODE.COM
//
//<INSERT LICENCE BLOCK HERE>
//****************************************************************************************
//*****************************************************************************************



//----- INCLUDE FILES FOR THIS SOURCE CODE FILE -----
#include "main.h"					//(Include the project global defines file)
#define	MAIN_C						//(Define used for following header file to flag that it is the header file for this source file)
#include "ap-main.h"				//(Include header file for this source file)

//----- OTHER PROJECT FILES REQUIRED BY THIS SOURCE CODE FILE -----
#include "display.h"

//----- COMPILER LIBRARY FILES REQUIRED BY THIS SOURCE CODE FILE -----






//******************************************
//******************************************
//********** DEVICE CONFIGURATION **********
//******************************************
//******************************************
//These configuration defines do not need to be included, but having them means that the configuration bits will be automatically set and will
//be included in the .hex file created for the project, so that they do not need to be manually set when programming devices at manufacture.
//(The config names are given for each device in MPLAB help, under 'Topics... -> PIC32MX Configuration Settings')

#ifdef __DEBUG				//Debug mode selected in MPLAB
//----- WE'RE IN DEVELOPMENT MODE -----
	//--- PLL Input Divider bits --
	//DIV_1 = Divide by 1, DIV_2 = Divide by 2, DIV_3 = Divide by 3, DIV_4 = Divide by 4, DIV_5 = Divide by 5, DIV_6 = Divide by 6, DIV_10 = Divide by 10, DIV_12 = Divide by 12  
	#pragma config FPLLIDIV = DIV_2


	//--- PLL Output Divider Value ---
	//DIV_1 = Divide by 1, DIV_2 = Divide by 2, DIV_4 = Divide by 4, DIV_8 = Divide by 8, DIV_16 = Divide by 16, DIV_32 = Divide by 32 , DIV_64 = Divide by 64, DIV_256 = Divide by 256  
	#pragma config FPLLODIV = DIV_1

	//--- PLL Multiplier bits ---
	//MUL_15 = Multiply by 15, MUL_16 = Multiply by 16, MUL_17 = Multiply by 17, MUL_18 = Multiply by 18, MUL_19 = Multiply by 19, MUL_20 = Multiply by 20, MUL_21 = Multiply by 21, MUL_24 = Multiply by 24  
	#pragma config FPLLMUL = MUL_20

	//--- Watchdog Timer Enable bit ---
	//OFF = Disabled, ON = Enabled
	#pragma config FWDTEN = ON

	//--- Watchdog Timer Postscale Select bits ---
	//PS1 = 1:1, PS2 = 1:2, PS4 = 1:4, PS8 = 1, PS16 = 1:16, PS32 = 1:32, PS64 = 1:64,, PS128 = 1:128, PS256 = 1:256, PS512 = 1:512, PS1024 = 1:1024, PS2048 = 1:2048,
	//PS4096 = 1:4096, PS8192 = 1:8,192, PS16384 = 1:16,384, PS32768 = 1:32,768, PS65536 = 1:65,536, PS131072 = 1:131,072, PS262144 = 1:262,144, PS524288 = 1:524,288, PS1048576 = 1:1,048,576  
	#pragma config WDTPS = PS32768

	//--- Clock Switching and Monitor Selection bits ---
	//CSECME = Clock Switching Enabled, Clock Monitoring Enabled, CSECMD = Clock Switching Enabled, Clock Monitoring Disabled, CSDCMD = Clock Switching Disabled, Clock Monitoring Disabled
	#pragma config FCKSM = CSDCMD

	//--- Bootup PBCLK divider ---
	//DIV_1 = Divide by 1, DIV_2 = Divide by 2, DIV_4 = Divide by 4, DIV_8 = Divide by 8
	#pragma config FPBDIV = DIV_2

	//--- CLKO Enable bit ---
	//OFF  = Disabled, ON = Enabled
	#pragma config OSCIOFNC = OFF

	//--- Primary Oscillator bits ---
	//EC = EC oscillator, XT = XT oscillator, HS = HS oscillator, OFF = Disabled  
	#pragma config POSCMOD = HS

	//--- Internal External Switch Over bit ---
	//OFF = Disabled, ON = Enabled  
	#pragma config IESO = OFF

	//--- Secondary oscillator Enable bit ---
	//OFF = Disabled, ON = Enabled
	#pragma config FSOSCEN = OFF

	//--- Oscillator Selection bits ---
	//FRC = Fast RC oscillator, FRCPLL = Fast RC oscillator w/ PLL, PRI = Primary oscillator (XT, HS, EC), PRIPLL = Primary oscillator (XT, HS, EC) w/ PLL, 
	//SOSC = Secondary oscillator, LPRC = Low power RC oscillator, FRCDIV16 = Fast RC oscillator with divide by 16, FRCDIV = Fast RC oscillator with divide
	#pragma config FNOSC = PRIPLL

	//--- Code Protect Enable bit ---
	//ON = Enabled, OFF = Disabled  
	#pragma config CP = OFF

	//--- Boot Flash Write Protect bit ---
	//ON = Enabled, OFF = Disabled
	#pragma config BWP = OFF

	//--- Program Flash Write Protect bit ---
	//OFF = Disabled  
	#pragma config PWP = OFF

	//--- ICE/ICD Comm Channel Select ---
	//ICS_PGx1 = ICE pins are shared with PGC1, PGD1, ICS_PGx2 = ICE pins are shared with PGC2, PGD2, 
	#pragma config ICESEL = ICS_PGx2

	//Background Debugger Enable bit:-
	//ON = Enabled , OFF = Disabled  
	#pragma config DEBUG = ON
	
#else
//----- WE'RE NOT IN DEVELOPMENT MODE -----
	//--- PLL Input Divider bits --
	//DIV_1 = Divide by 1, DIV_2 = Divide by 2, DIV_3 = Divide by 3, DIV_4 = Divide by 4, DIV_5 = Divide by 5, DIV_6 = Divide by 6, DIV_10 = Divide by 10, DIV_12 = Divide by 12  
	#pragma config FPLLIDIV = DIV_2


	//--- PLL Output Divider Value ---
	//DIV_1 = Divide by 1, DIV_2 = Divide by 2, DIV_4 = Divide by 4, DIV_8 = Divide by 8, DIV_16 = Divide by 16, DIV_32 = Divide by 32 , DIV_64 = Divide by 64, DIV_256 = Divide by 256  
	#pragma config FPLLODIV = DIV_1

	//--- PLL Multiplier bits ---
	//MUL_15 = Multiply by 15, MUL_16 = Multiply by 16, MUL_17 = Multiply by 17, MUL_18 = Multiply by 18, MUL_19 = Multiply by 19, MUL_20 = Multiply by 20, MUL_21 = Multiply by 21, MUL_24 = Multiply by 24  
	#pragma config FPLLMUL MUL_20

	//--- Watchdog Timer Enable bit ---
	//OFF = Disabled, ON = Enabled
	#pragma config FWDTEN = ON

	//--- Watchdog Timer Postscale Select bits ---
	//PS1 = 1:1, PS2 = 1:2, PS4 = 1:4, PS8 = 1, PS16 = 1:16, PS32 = 1:32, PS64 = 1:64,, PS128 = 1:128, PS256 = 1:256, PS512 = 1:512, PS1024 = 1:1024, PS2048 = 1:2048,
	//PS4096 = 1:4096, PS8192 = 1:8,192, PS16384 = 1:16,384, PS32768 = 1:32,768, PS65536 = 1:65,536, PS131072 = 1:131,072, PS262144 = 1:262,144, PS524288 = 1:524,288, PS1048576 = 1:1,048,576  
	#pragma config WDTPS = PS32768

	//--- Clock Switching and Monitor Selection bits ---
	//CSECME = Clock Switching Enabled, Clock Monitoring Enabled, CSECMD = Clock Switching Enabled, Clock Monitoring Disabled, CSDCMD = Clock Switching Disabled, Clock Monitoring Disabled
	#pragma config FCKSM = CSDCMD

	//--- Bootup PBCLK divider ---
	//DIV_1 = Divide by 1, DIV_2 = Divide by 2, DIV_4 = Divide by 4, DIV_8 = Divide by 8
	#pragma config FPBDIV = DIV_2

	//--- CLKO Enable bit ---
	//OFF  = Disabled, ON = Enabled
	#pragma config OSCIOFNC = OFF

	//--- Primary Oscillator bits ---
	//EC = EC oscillator, XT = XT oscillator, HS = HS oscillator, OFF = Disabled  
	#pragma config POSCMOD = HS

	//--- Internal External Switch Over bit ---
	//OFF = Disabled, ON = Enabled  
	#pragma config IESO = OFF

	//--- Secondary oscillator Enable bit ---
	//OFF = Disabled, ON = Enabled
	#pragma config FSOSCEN OFF

	//--- Oscillator Selection bits ---
	//FRC = Fast RC oscillator, FRCPLL = Fast RC oscillator w/ PLL, PRI = Primary oscillator (XT, HS, EC), PRIPLL = Primary oscillator (XT, HS, EC) w/ PLL, 
	//SOSC = Secondary oscillator, LPRC = Low power RC oscillator, FRCDIV16 = Fast RC oscillator with divide by 16, FRCDIV = Fast RC oscillator with divide
	#pragma config FNOSC = PRIPLL

	//--- Code Protect Enable bit ---
	//ON = Enabled, OFF = Disabled  
	#pragma config CP = OFF

	//--- Boot Flash Write Protect bit ---
	//ON = Enabled, OFF = Disabled
	#pragma config BWP = OFF

	//--- Program Flash Write Protect bit ---
	//OFF = Disabled  
	#pragma config PWP = OFF

	//--- ICE/ICD Comm Channel Select ---
	//ICS_PGx1 = ICE pins are shared with PGC1, PGD1, ICS_PGx2 = ICE pins are shared with PGC2, PGD2, 
	#pragma config ICESEL = ICS_PGx2

	//Background Debugger Enable bit:-
	//ON = Enabled , OFF = Disabled  
	#pragma config DEBUG = OFF
#endif










//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************

//********************************
//********************************
//********** INITIALISE **********
//********************************
//********************************
void initialise (void)
{
	WORD w_temp;

	//##### GENERAL NOTE ABOUT PIC32'S #####
	//Try and use the peripheral libraries instead of special function registers for everything (literally everything!) to avoid
	//bugs that can be caused by the pipeline and interrupts.
	

	//---------------------------------
	//----- CONFIGURE PERFORMANCE -----
	//---------------------------------
	
	//----- SETUP EVERYTHING FOR OPTIMUM PERFORMANCE -----
	SYSTEMConfigPerformance (80000000);		//Note this sets peripheral bus to '1' max speed (regardless of configuration bit setting)
											//Use PBCLK divider of 1:1 to calculate UART baud, timer tick etc

	//----- SET PERIPHERAL BUS DIVISOR -----
	//To minimize dynamic power the PB divisor should be chosen to run the peripherals at the lowest frequency that provides acceptable system performance
	//mOSCSetPBDIV(OSC_PB_DIV_1);			//OSC_PB_DIV_1, OSC_PB_DIV_2, OSC_PB_DIV_4, OSC_PB_DIV_8, 






	//-------------------------
	//----- SETUP IO PINS -----
	//-------------------------
	//(Device will powerup with all IO pins as inputs)

	//----- TURN OFF THE JTAG PORT -----
	//(JTAG is on by default)
	mJTAGPortEnable(0);

	#define	PORTA_IO	0x0080				//Setup the IO pin type (0 = output, 1 = input)
	mPORTAWrite(0x0000);					//Set initial ouput pin states
	mPORTASetPinsDigitalIn(PORTA_IO);		//(Sets high bits as input)
	mPORTASetPinsDigitalOut(~PORTA_IO);		//(Sets high bits as output)

	#define	PORTB_IO	0x00C0				//Setup the IO pin type (0 = output, 1 = input)
	mPORTBWrite(0x0000);					//Set initial ouput pin states
	mPORTBSetPinsDigitalIn(PORTB_IO);		//(Sets high bits as input)
	mPORTBSetPinsDigitalOut(~PORTB_IO);		//(Sets high bits as output)

	#define	PORTC_IO	0x0000				//Setup the IO pin type (0 = output, 1 = input)
	mPORTCWrite(0x0000);					//Set initial ouput pin states
	mPORTCSetPinsDigitalIn(PORTC_IO);		//(Sets high bits as input)
	mPORTCSetPinsDigitalOut(~PORTC_IO);		//(Sets high bits as output)

	#define	PORTD_IO	0x2000				//Setup the IO pin type (0 = output, 1 = input)
	mPORTDWrite(0x0012);					//Set initial ouput pin states
	mPORTDSetPinsDigitalIn(PORTD_IO);		//(Sets high bits as input)
	mPORTDSetPinsDigitalOut(~PORTD_IO);		//(Sets high bits as output)

	#define	PORTE_IO	0x0000				//Setup the IO pin type (0 = output, 1 = input)
	mPORTEWrite(0x0000);					//Set initial ouput pin states
	mPORTESetPinsDigitalIn(PORTE_IO);		//(Sets high bits as input)
	mPORTESetPinsDigitalOut(~PORTE_IO);		//(Sets high bits as output)

	#define	PORTF_IO	0x0080				//Setup the IO pin type (0 = output, 1 = input)
	mPORTFWrite(0x0000);					//Set initial ouput pin states
	mPORTFSetPinsDigitalIn(PORTF_IO);		//(Sets high bits as input)
	mPORTFSetPinsDigitalOut(~PORTF_IO);		//(Sets high bits as output)

	#define	PORTG_IO	0x0000				//Setup the IO pin type (0 = output, 1 = input)
	mPORTGWrite(0x0000);					//Set initial ouput pin states
	mPORTGSetPinsDigitalIn(PORTG_IO);		//(Sets high bits as input)
	mPORTGSetPinsDigitalOut(~PORTG_IO);		//(Sets high bits as output)

	//Read pins using:
	// mPORTAReadBits(BIT_0);
	//Write pins using:
	// mPORTAClearBits(BIT_0);
	// mPORTASetBits(BIT_0);
	// mPORTAToggleBits(BIT_0);

	//----- INPUT CHANGE NOTIFICATION CONFIGURATION -----
	//EnableCN0();
	//ConfigCNPullups(CN10_PULLUP_ENABLE | CN11_PULLUP_ENABLE);


	//----- SETUP THE A TO D PINS -----
	//mPORTBSetPinsAnalogIn(BIT_0 | BIT_1);





	//------------------------
	//----- SETUP TIMERS -----
	//------------------------
	//(INCLUDE THE USAGE OF ALL TIMERS HERE EVEN IF NOT SETUP HERE SO THIS IS THE ONE POINT OF
	//REFERENCE TO KNOW WHICH TIMERS ARE IN USE AND FOR WHAT).

	//----- SETUP TIMER 1 -----
	//Used for: Available
	//OpenTimer1((T1_ON | T1_IDLE_CON | T1_GATE_OFF | T1_PS_1_4 | T1_SOURCE_INT), 20000);

	//----- SETUP TIMER 2 -----
	//Used for: Available
	//OpenTimer2((T2_ON | T2_IDLE_CON | T2_GATE_OFF | T2_PS_1_4 | T2_SOURCE_INT), 20000);

	//----- SETUP TIMER 3 -----
	//Used for: Available
	//OpenTimer3((T3_ON | T3_IDLE_CON | T3_GATE_OFF | T3_PS_1_4 | T3_SOURCE_INT), 20000);

	//----- SETUP TIMER 4 -----
	//Used for: Available
	//OpenTimer4((T4_ON | T4_IDLE_CON | T4_GATE_OFF | T4_PS_1_4 | T4_SOURCE_INT), 20000);

	//----- SETUP TIMER 5 -----
	//Used for: Heartbeat
	OpenTimer5((T5_ON | T5_IDLE_CON | T5_GATE_OFF | T5_PS_1_4 | T5_SOURCE_INT), 20000);
	ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_7);


	//------------------------------------------
	//----- SETUP SYNCHRONOUS SERIAL PORTS -----
	//------------------------------------------

	//----- SETUP SPI 1 -----
	//Used for: Disabled


	//----- SETUP SPI 2 -----
	//Used for: Disabled


	//----- SETUP I2C 1 -----
	//Used for: Disabled


	//----- SETUP I2C 2 -----
	//Used for: Disabled



	//-----------------------
	//----- SETUP USART -----
	//-----------------------

	//----- SETUP UART 1 -----
	//Used for: Disabled


	//----- SETUP UART 2 -----
	//Used for: Disabled





	//-----------------------------
	//----- ENABLE INTERRUPTS -----
	//-----------------------------
	//INTEnableSystemSingleVectoredInt();
	INTEnableSystemMultiVectoredInt();





	//------------------------------
	//----- DISPLAY INITIALISE -----
	//------------------------------
	display_initialise();



	//DISPLAY THE DEFAULT POWERUP SCREEN
	display_bitmap(0, ec_logo_128x64,									//Bitmap
					   DISPLAY_BITMAP_INVERT_PIXELS_OFF,				//Options
					   0, 0);											//X, Y


}






//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************

//***********************************
//***********************************
//********** MAIN FUNCTION **********
//***********************************
//***********************************
int main (void)
{


	//**********************
	//**********************
	//***** INITIALISE *****
	//**********************
	//**********************
	initialise();





	//*********************
	//*********************
	//***** MAIN LOOP *****
	//*********************
	//*********************
	while(1)						//(Do forever)
	{
		//----- RESET THE WATCHDOG TIMEOUT TIMER -----
		ClearWDT();

		//----- READ SWITCHES -----
		read_switches();


		//----- PROCESS MODE -----
		process_mode();




		//---------------------------------------------------------------------------------------
		//---------- EXAMPLE OF HOW TO USE THE DISPLAY BYTE ORDERING TEST SEQUENCE FUNCTION -----
		//---------------------------------------------------------------------------------------
/*
		//RESET THE FUNCTION
		display_byte_ordering_test_sequence(0x01);
		
		//DISPLAY EACH BYTE
		while(1)
		{
			if (general_use_10ms_timer == 0)
			{
				//DISPLAY NEXT BYTE
				general_use_10ms_timer = 80;
				display_byte_ordering_test_sequence(0x00);
			}
		}
*/





		
	}
}





//***********************************
//***********************************
//********** READ SWITCHES **********
//***********************************
//***********************************
void read_switches (void)
{
	BYTE switches_read;
	static BYTE switches_last = 0;
	static BYTE switches_debounced_last = 0;


	//RESET THE NEW SWITCH PRESS REGISTER
	switches_new = 0;

	//ONLY DO EVERY 10MS FOR DEBOUNCE
	if (read_switches_flag == 0)
		return;

	read_switches_flag = 0;

	//GET THE SWITCH INPUTS
	switches_read = 0;
	if (!mPORTDReadBits(BIT_13))
		switches_read |= 0x01;
	if (!mPORTAReadBits(BIT_7))
		switches_read |= 0x02;

	//DEBOUNCE
	switches_debounced = switches_last & switches_read;

	//FLAG NEW BUTTON PRESSES
	switches_new = switches_debounced ^ switches_debounced_last;
	switches_new &= switches_debounced;

	//STORE LAST REGISTERS FOR NEXT TIME
	switches_debounced_last = switches_debounced;
	switches_last = switches_read;

}




CONSTANT BYTE string_standard_fonts[] = {"Standard Fonts:"};
CONSTANT BYTE string_5x5[] = {"5x5"};
CONSTANT BYTE string_5x7[] = {"5x7"};
CONSTANT BYTE string_6x10[] = {"6x10"};
CONSTANT BYTE string_7x13[] = {"7x13"};
CONSTANT BYTE string_monospace[] = {"Proportional Text Off"};
CONSTANT BYTE string_proportional[] = {"Proportional Text On"};

//**********************************
//**********************************
//********** PROCESS MODE **********
//**********************************
//**********************************
//This is a simple example user mode state machine function.
//The current mode is selected using the switch inputs.
//Each of the 6 modes does a specific function with the screen
void process_mode (void)
{
	static BYTE our_mode = 0;
	BYTE just_entered_new_mode = 0;
	static BYTE graph_pixel_x;
	static BYTE graph_pixel_y;



	//-------------------------------------
	//----- CHECK FOR ADJUST CONTRAST -----
	//-------------------------------------
	if (SWITCH_2_PRESSED)
	{
		//SWITCH IS PRESSED - DO CONTRAST CYCLE
		if (contrast_10ms_timer == 0)
		{
			//TIMER HAS EXPIRED - DO NEXT CONTRAST LEVEL
			contrast_10ms_timer = 8;		//<Delay between contrast steps
			
			display_contrast_level++;
			if (display_contrast_level > 63)
				display_contrast_level = 0;
				
			display_set_contrast(display_contrast_level);
		}
	}
	else
	{
		//Reset timer for when switch is next pressed
		contrast_10ms_timer = 8;
	}



	//--------------------------------------
	//----- CHECK FOR NEW SWITCH PRESS -----
	//--------------------------------------
	if (SWITCH_1_NEW_PRESS)
	{
		our_mode++;
		if (our_mode > 4)
			our_mode = 0;

		//Flag that we've just entered a new mode
		just_entered_new_mode = 1;
	}


	//------------------------------------
	//----- PROCESS THE CURRENT MODE -----
	//------------------------------------
	switch (our_mode)
	{
	case 0:
		//---------------------
		//----- IN MODE 0 -----
		//---------------------
		//DISPLAY LOGO
		if (just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			display_clear_screen(0);

			//Display screen
			display_bitmap(0, ec_logo_128x64,							//Bitmap
							   DISPLAY_BITMAP_INVERT_PIXELS_OFF,		//Options
							   0, 0);									//X, Y


		}
		//----- PROCESS MODE -----

		break;

	case 1:
		//---------------------
		//----- IN MODE 1 -----
		//---------------------
		//DISPLAY INVERTED LOGO
		if (just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			display_clear_screen(1);

			//Display screen
			display_bitmap(0, ec_logo_128x64,							//Bitmap
							   DISPLAY_BITMAP_INVERT_PIXELS_ON,			//Options
							   0, 0);									//X, Y
		}
		//----- PROCESS MODE -----

		break;

	case 2:
		//---------------------
		//----- IN MODE 2 -----
		//---------------------
		//DISPLAY FONTS
		if (just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			display_clear_screen(0);

			display_const_string(0, font_05x07, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_OFF | DISPLAY_STRING_PROPORTIONAL),	//Options
									 1, 1,										//X, Y
									 string_standard_fonts);					//String

			display_const_string(0, font_05x05, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_OFF | DISPLAY_STRING_MONOSPACE),	//Options
									 1, 15,										//X, Y
									 string_5x5);								//String

			display_const_string(0, font_05x07, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_OFF | DISPLAY_STRING_MONOSPACE),	//Options
									 64, 13,										//X, Y
									 string_5x7);								//String

			display_const_string(0, font_06x10, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_OFF | DISPLAY_STRING_MONOSPACE),	//Options
									 1, 29,										//X, Y
									 string_6x10);								//String

			display_const_string(0, font_07x13, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_OFF | DISPLAY_STRING_MONOSPACE),	//Options
									 64, 26,										//X, Y
									 string_7x13);								//String

			display_const_string(0, font_05x07, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_OFF | DISPLAY_STRING_MONOSPACE),	//Options
									 1, 47,										//X, Y
									 string_monospace);							//String

			display_const_string(0, font_05x07, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_OFF | DISPLAY_STRING_PROPORTIONAL),	//Options
									 1, 56,										//X, Y
									 string_proportional);						//String
		}
		//----- PROCESS MODE -----

		break;

	case 3:
		//---------------------
		//----- IN MODE 3 -----
		//---------------------
		//DISPLAY INVERTED FONTS
		if (just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			display_clear_screen(1);

			display_const_string(0, font_05x07, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_ON | DISPLAY_STRING_PROPORTIONAL),	//Options
									 1, 1,										//X, Y
									 string_standard_fonts);					//String

			display_const_string(0, font_05x05, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_ON | DISPLAY_STRING_MONOSPACE),	//Options
									 1, 15,										//X, Y
									 string_5x5);								//String

			display_const_string(0, font_05x07, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_ON | DISPLAY_STRING_MONOSPACE),	//Options
									 64, 13,									//X, Y
									 string_5x7);								//String

			display_const_string(0, font_06x10, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_ON | DISPLAY_STRING_MONOSPACE),	//Options
									 1, 29,										//X, Y
									 string_6x10);								//String

			display_const_string(0, font_07x13, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_ON | DISPLAY_STRING_MONOSPACE),	//Options
									 64, 26,									//X, Y
									 string_7x13);								//String

			display_const_string(0, font_05x07, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_ON | DISPLAY_STRING_MONOSPACE),	//Options
									 1, 47,										//X, Y
									 string_monospace);							//String

			display_const_string(0, font_05x07, 								//Bitmap (flash memory, program memory)
									 (DISPLAY_STRING_ON_X_AXIS | DISPLAY_STRING_INVERT_PIXELS_ON | DISPLAY_STRING_PROPORTIONAL),	//Options
									 1, 56,										//X, Y
									 string_proportional);						//String
		}
		//----- PROCESS MODE -----

		break;

	case 4:
		//---------------------
		//----- IN MODE 4 -----
		//---------------------
		//DISPLAY GRAPH
		if (just_entered_new_mode)
		{
			//----- JUST ENTERED THIS MODE -----
			display_clear_screen(0);

			//Display background
			display_bitmap(0, graph_xy,									//Bitmap (flash memory, program memory)
							   DISPLAY_BITMAP_INVERT_PIXELS_OFF,		//Options
							   0, 0);									//X, Y
			
			//Setup to draw trace line
			graph_pixel_y = 54;
			graph_pixel_x = 9;
			user_mode_10ms_timer = 0;
		}
		//----- PROCESS MODE -----
		if (user_mode_10ms_timer == 0)
		{
			//TIMER HAS EXPIRED - DO NEXT DOT OF GRAPH TRACE
			user_mode_10ms_timer = 4;
			
			//Only do if there is more of the graph left to display
			if (graph_pixel_x < 127)
			{
				graph_pixel_x++;
				
				//Do an up down trace on the Y axis
				if (graph_pixel_x < 60)
					graph_pixel_y--;
				else if (graph_pixel_x < 108)
					graph_pixel_y++;
			
				//Display pixel
				display_bitmap(0, graph_pixel,								//Bitmap (flash memory, program memory)
								   DISPLAY_BITMAP_INVERT_PIXELS_OFF,		//Options
								   graph_pixel_x, graph_pixel_y);			//X, Y
			}
		}
		break;

	default:
		//ERROR - SHOUDLN'T BE ABLE TO GET HERE
		our_mode = 0;
		break;
	}


}






//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************
//*************************************************************************************************************************************

//************************************************
//************************************************
//********** INTERRUPT VECTOR LOCATIONS **********
//************************************************
//************************************************


//*********************************************
//*********************************************
//********** HEARTBEAT IRQ (Timer 5) **********
//*********************************************
//*********************************************
void __ISR(_TIMER_5_VECTOR, ipl7) Timer5IntHandler(void) 	//(ipl# must match the priority level assigned to the irq where its enabed)
{
	static BYTE hb_10ms_timer = 0;
	static BYTE hb_100ms_timer = 0;
	static BYTE hb_1sec_timer = 0;
	
	INTClearFlag(INT_T5);						//Reset the timer irq flag

		//-----------------------------
		//-----------------------------
		//----- HERE EVERY 1 mSec -----
		//-----------------------------
		//-----------------------------


		hb_10ms_timer++;
		if (hb_10ms_timer == 10)
		{
			//------------------------------
			//------------------------------
			//----- HERE EVERY 10 mSec -----
			//------------------------------
			//------------------------------
			hb_10ms_timer = 0;

	
			//----- GENERAL USE 10mS TIMER -----
			if (general_use_10ms_timer)
				general_use_10ms_timer--;
	
	
			//----- READ SWITCHES FLAG -----
			read_switches_flag = 1;
	
			//----- USER MODE 10mS TIMER -----
			if (user_mode_10ms_timer)
				user_mode_10ms_timer--;
	
			//----- ADJUST CONTRAST 10mS TIMER -----
			if (contrast_10ms_timer)
				contrast_10ms_timer--;



		} //if (hb_10ms_timer == 10)

		hb_100ms_timer++;
		if (hb_100ms_timer == 100)
		{
			//-------------------------------
			//-------------------------------
			//----- HERE EVERY 100 mSec -----
			//-------------------------------
			//-------------------------------
			hb_100ms_timer = 0;

			//----- GENERAL USE 100mS TIMER -----
			if (general_use_100ms_timer)
				general_use_100ms_timer--;






			hb_1sec_timer++;
			if (hb_1sec_timer == 10)
			{
				//----------------------------
				//----------------------------
				//----- HERE EVERY 1 Sec -----
				//----------------------------
				//----------------------------
				hb_1sec_timer = 0;




			} //if (hb_1sec_timer == 10)
		} //if (hb_100ms_timer == 100)

}



//***************************************************
//***************************************************
//********** UNHANDLED EXCEPTION INTERRUPT **********
//***************************************************
//***************************************************
//Useful interrupt to include as it will be called if any exception occurs that is not handled
//There are CPU registers that will tell you where the exception occurred etc.
//View them in the watch window for a more useful description to be shown.
void __attribute__((nomips16)) _general_exception_handler(void)
{
	Nop();



}













