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
//PIC24 SAMPLE PROJECT C CODE FILE





//----- INCLUDE FILES FOR THIS SOURCE CODE FILE -----
#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
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
//These configuration defines do not need to be included, but having them means that the configuration bits will be automatically set and will be included in the .hex
//file created for the project, so that they do not need to be manually set when programming devices at manufacture.
//(The config names are given in the '.inc' file for the device being used)

#ifdef __DEBUG				//Debug mode selected in MPLAB
//----- WE'RE IN DEVELOPMENT MODE -----
	//--- CONFIG1 ---
	//JTAG:-
	// JTAGEN_OFF, JTAGEN_ON
	//Code Protect:-
	// GCP_ON = Enabled, GCP_OFF = Disabled
	//Write Protect:-
	// GWRP_ON = Enabled, GWRP_OFF  = Disabled
	//Background Debugger:-
	// BKBUG_ON = Enabled, BKBUG_OFF = Disabled
	//Clip-on Emulation mode:-
	// COE_ON = Enabled, COE_OFF = Disabled
	//ICD pins select:-
	// ICS_PGx1 = EMUC/EMUD share PGC1/PGD1, ICS_PGx2 = EMUC/EMUD share PGC2/PGD2
	//Watchdog Timer:-
	// FWDTEN_OFF = Disabled, FWDTEN_ON = Enabled
	//Windowed WDT:-
	// WINDIS_ON = Enabled, WINDIS_OFF = Disabled
	//Watchdog prescaler:-
	// FWPSA_PR32 = 1:32, FWPSA_PR128 = 1:128
	//Watchdog postscale:-
	//WDTPS_PS1 = 1:1, WDTPS_PS2, WDTPS_PS4, WDTPS_PS8, WDTPS_PS16, WDTPS_PS32, WDTPS_PS64, WDTPS_PS128, WDTPS_PS256, WDTPS_PS512, WDTPS_PS1024
	//WDTPS_PS2048, WDTPS_PS4096, WDTPS_PS8192, WDTPS_PS16384, WDTPS_PS32768 = 1:32,768
	_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx2 & FWDTEN_ON & WINDIS_OFF & WDTPS_PS2048)

	//--- CONFIG2 ---
	//Two Speed Start-up:-
	// IESO_OFF, IESO_ON
	//Oscillator Selection:-
	// FNOSC_FRC = Fast RC oscillator, FNOSC_FRCPLL = Fast RC oscillator w/ divide and PLL, FNOSC_PRI = Primary oscillator (XT, HS, EC), FNOSC_PRIPLL = Primary oscillator (XT, HS, EC) w/ PLL,
	// FNOSC_SOSC = Secondary oscillator, FNOSC_LPRC = Low power RC oscillator, FNOSC_FRCDIV = Fast RC oscillator with divide
	//Clock switching and clock monitor:-
	// FCKSM_CSECME = Both enabled, FCKSM_CSECMD = Only clock switching enabled, FCKSM_CSDCMD = Both disabled
	//OSCO/RC15 function:-
	// OSCIOFNC_ON = RC15, OSCIOFNC_OFF = OSCO or Fosc/2
	//Oscillator Selection:-
	// POSCMOD_EC = External clock, POSCMOD_XT = XT oscillator, POSCMOD_HS = HS oscillator, POSCMOD_NONE = Pimary disabled
	_CONFIG2(IESO_OFF & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS)
	
#else
//----- WE'RE NOT IN DEVELOPMENT MODE -----
	//--- CONFIG1 ---
	//JTAG:-
	// JTAGEN_OFF, JTAGEN_ON
	//Code Protect:-
	// GCP_ON = Enabled, GCP_OFF = Disabled
	//Write Protect:-
	// GWRP_ON = Enabled, GWRP_OFF  = Disabled
	//Background Debugger:-
	// BKBUG_ON = Enabled, BKBUG_OFF = Disabled
	//Clip-on Emulation mode:-
	// COE_ON = Enabled, COE_OFF = Disabled
	//ICD pins select:-
	// ICS_PGx1 = EMUC/EMUD share PGC1/PGD1, ICS_PGx2 = EMUC/EMUD share PGC2/PGD2
	//Watchdog Timer:-
	// FWDTEN_OFF = Disabled, FWDTEN_ON = Enabled
	//Windowed WDT:-
	// WINDIS_ON = Enabled, WINDIS_OFF = Disabled
	//Watchdog prescaler:-
	// FWPSA_PR32 = 1:32, FWPSA_PR128 = 1:128
	//Watchdog postscale:-
	//WDTPS_PS1 = 1:1, WDTPS_PS2, WDTPS_PS4, WDTPS_PS8, WDTPS_PS16, WDTPS_PS32, WDTPS_PS64, WDTPS_PS128, WDTPS_PS256, WDTPS_PS512, WDTPS_PS1024
	//WDTPS_PS2048, WDTPS_PS4096, WDTPS_PS8192, WDTPS_PS16384, WDTPS_PS32768 = 1:32,768
	_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & ICS_PGx2 & FWDTEN_ON & WINDIS_OFF & WDTPS_PS2048)

	//--- CONFIG2 ---
	//Two Speed Start-up:-
	// IESO_OFF, IESO_ON
	//Oscillator Selection:-
	// FNOSC_FRC = Fast RC oscillator, FNOSC_FRCPLL = Fast RC oscillator w/ divide and PLL, FNOSC_PRI = Primary oscillator (XT, HS, EC), FNOSC_PRIPLL = Primary oscillator (XT, HS, EC) w/ PLL,
	// FNOSC_SOSC = Secondary oscillator, FNOSC_LPRC = Low power RC oscillator, FNOSC_FRCDIV = Fast RC oscillator with divide
	//Clock switching and clock monitor:-
	// FCKSM_CSECME = Both enabled, FCKSM_CSECMD = Only clock switching enabled, FCKSM_CSDCMD = Both disabled
	//OSCO/RC15 function:-
	// OSCIOFNC_ON = RC15, OSCIOFNC_OFF = OSCO or Fosc/2
	//Oscillator Selection:-
	// POSCMOD_EC = External clock, POSCMOD_XT = XT oscillator, POSCMOD_HS = HS oscillator, POSCMOD_NONE = Pimary disabled
	_CONFIG2(IESO_OFF & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS)
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

	//----- DISABLE INTERRUPTS DURING CONFIGURATION -----
	IFS0 = 0;
	IFS1 = 0;
	IFS2 = 0;
	IFS3 = 0;
	IFS4 = 0;
	IEC0 = 0;
	IEC1 = 0;
	IEC2 = 0;
	IEC3 = 0;
	IEC4 = 0;


	//----- OSCILLATOR CONFIGURATION -----
	//Wait for PLL to lock
	while(OSCCONbits.LOCK != 1)
		;

	//-------------------------
	//----- SETUP IO PINS -----
	//-------------------------
	//(Device will powerup with all IO pins as inputs)

	LATA =  0b0000000000000000;		//Setup the state of any pins that are outputs (inputs are don't care)
	TRISA = 0b0000000010000000;		//Setup the IO pin type (0 = output, 1 = input)
	ODCA =  0b0000000000000000;		//Setup the open drain option (0 = normal push pull, 1 = open drain)

	LATB =  0b0000000000000000;
	TRISB = 0b0000000011000000;
	ODCB =  0b0000000000000000;

	LATC =  0b0000000000000000;
	TRISC = 0b0000000000000000;
	ODCC =  0b0000000000000000;

	LATD =  0b0000000000010010;
	TRISD = 0b0010000000000000;
	ODCD =  0b0000000000000000;

	LATE =  0b0000000000000000;
	TRISE = 0b0000000000000000;
	ODCE =  0b0000000000000000;

	LATF =  0b0000000000000000;
	TRISF = 0b0000000010000000;
	ODCF =  0b0000000000000000;

	LATG =  0b0000000000000000;
	TRISG = 0b0000000000000000;
	ODCG =  0b0000000000000000;

	//----- INPUT CHANGE NOTIFICATION CONFIGURATION -----
	CNEN1 = 0;		//Change notification interrupt enable (0 = off, 1 = enabled)
	CNEN2 = 0;
	CNPU1 = 0;		//Pin  pull ups (0 = no pull up, 1 = pull up enabled)
	CNPU2 = 0;

	//----- SETUP THE A TO D PINS -----
	AD1PCFG = 0xffff;			//Select analog pins (1 = digital, 0 = analog)
	AD1CON1 = 0;				//Analog module off




	//------------------------
	//----- SETUP TIMERS -----
	//------------------------
	//(INCLUDE THE USAGE OF ALL TIMERS HERE EVEN IF NOT SETUP HERE SO THIS IS THE ONE POINT OF
	//REFERENCE TO KNOW WHICH TIMERS ARE IN USE AND FOR WHAT).


	//----- SETUP TIMER 1 -----
	//Used for: Available
	T1CON = 0;
	//TMR1 = 0;
	//PR1 = 0xffff;


	//----- SETUP TIMER 2 -----
	//Used for: Available
	T2CON = 0;
	//TMR2 = 0;
	//PR2 = 0xffff;


	//----- SETUP TIMER 3 -----
	//Used for: Available
	T3CON = 0;
	//TMR3 = 0;
	//TMR3HLD = 0;			//Timer holding register for 32bit
	//PR3 = 0xffff;


	//----- SETUP TIMER 4 -----
	//Used for: Available
	T4CON = 0;
	//TMR4 = 0;
	//PR4 = 0xffff;


	//----- SETUP TIMER 5 -----
	//Used for: Heartbeat
	T5CON = 0x8000;			//16 bit, 1:1 prescale
	TMR5 = 0;
	TMR5HLD = 0;			//Timer holding register for 32bit
	PR5 = 16000;			//16000 = 1mS


	//-------------------------------
	//----- SETUP INPUT CAPTURE -----
	//-------------------------------
	IC1CON = 0;
	IC2CON = 0;
	IC3CON = 0;
	IC4CON = 0;
	IC5CON = 0;


	//--------------------------------
	//----- SETUP OUTPUT COMPARE -----
	//--------------------------------
	OC1CON = 0;
	OC2CON = 0;
	OC3CON = 0;
	OC4CON = 0;
	OC5CON = 0;
	OC1RS = 0;
	OC1R = 0;
	OC1CON = 0;
	OC2RS = 0;
	OC2R = 0;
	OC2CON = 0;
	OC3RS = 0;
	OC3R = 0;
	OC3CON = 0;
	OC4RS = 0;
	OC4R = 0;
	OC4CON = 0;
	OC5RS = 0;
	OC5R = 0;
	OC5CON = 0;


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
	//U1BRG = ;
	U1MODE = 0;
	//U1STA = 0;


	//----- SETUP UART 2 -----
	//Used for: Disabled
	//U2BRG = ;
	U2MODE = 0;
	//U2STA = 0;








	//-----------------------------
	//----- ENABLE INTERRUPTS -----
	//-----------------------------

	//----- INTERRUPT PRIORITIES -----
	IPC0 = 0x4444;
	IPC1 = 0x4444;
	IPC2 = 0x4444;
	IPC3 = 0x4444;
	IPC4 = 0x4444;
	IPC5 = 0x4444;
	IPC6 = 0x4444;
	IPC7 = 0x4444;
	IPC8 = 0x4444;
	IPC9 = 0x4444;
	IPC10 = 0x4444;
	IPC11 = 0x4444;
	IPC12 = 0x4444;
	IPC13 = 0x4444;
	IPC15 = 0x4444;
	IPC16 = 0x4444;

	INTCON1 = 0;
	INTCON2 = 0;

	IEC0 = 0;
	IEC1 = 0;
	IEC2 = 0;
	IEC3 = 0;
	IEC4 = 0;

	//ENABLE TIMER 5 (HEARTBEAT) IRQ
	IPC7bits.T5IP = 3;		//Set irq priority (0-7, 7=highest, 0=disabled)
	IEC1bits.T5IE = 1;		//Enable irq



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
		ClrWdt();


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
	if (!_RD13)
		switches_read |= 0x01;
	if (!_RA7)
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
void __attribute__((__interrupt__,__auto_psv__)) _T5Interrupt(void)
{
	static BYTE hb_10ms_timer = 0;
	static BYTE hb_100ms_timer = 0;
	static BYTE hb_1sec_timer = 0;
	
	_T5IF = 0;						//Reset the timer irq flag

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

















