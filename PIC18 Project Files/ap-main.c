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
//						PIC24 SAMPLE PROJECT C CODE FILE
//  Copyright:			EMBEDDED-CODE.COM
//
//<INSERT LICENCE BLOCK HERE>
//****************************************************************************************
//****************************************************************************************


//----- INCLUDE FILES FOR THIS SOURCE CODE FILE -----
#include "main.h"					//(Include the project global defines file)
#define	MAIN_C						//(Define used for following header file to flag that it is the header file for this source file)
#include "ap-main.h"				//(Include header file for this source file)

//----- OTHER PROJECT FILES REQUIRED BY THIS SOURCE CODE FILE -----
#include "display.h"

//----- COMPILER LIBRARY FILES REQUIRED BY THIS SOURCE CODE FILE -----
//#include <stdio.h>					//(Needed for printf)
//#include <stdlib.h>
//#include <usart.h>





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
	#pragma config OSC = HSPLL			//Ocsillator type
	#pragma config FCMEN = OFF			//Fail safe clock monitor (On = if osc fails the internal osc will be used)
	#pragma config IESO = OFF			//Internal / external ocsillator switchover mode
	#pragma config BOREN = SBORDIS		//Brown out reset (OFF = Disabled, ON = SBOREN bit enabled in software, NOSLP = enabled except SLEEP, SBOREN Disabled, SBORDIS enabled, SBOREN Disabled)
	#pragma config PWRT = OFF			//Powerup timer (not available when debugging)
#ifdef USING_3V3_POWER
	#pragma config BORV = 3				//Brown out reset voltage (0 = maximum, 3 = minimum - see device datasheet)
#else
	#pragma config BORV = 1				//Brown out reset voltage (0 = maximum, 3 = minimum - see device datasheet)
#endif
	#pragma config WDT = OFF			//Watchdog timer (not available when debugging)
	#pragma config WDTPS = 128			//Watchdog postscaller
	#pragma config MCLRE = ON			//MCLR pin enable
	#pragma config LPT1OSC = OFF		//Low power timer 1 oscillator
	#pragma config PBADEN = OFF			//PortB AtoD enable
	#pragma config CCP2MX = PORTC		//CCP2 pin
	#pragma config DEBUG = ON			//Background Debugger
	#pragma config LVP = OFF			//Low voltage in circuit programming
	#pragma config STVREN = ON			//Stack overflow reset
	#pragma config XINST = OFF			//Extended instruction set enable
	#pragma config CP0 = OFF			//Code protect block
	#pragma config CP1 = OFF			//Code protect block
	#pragma config CP2 = OFF			//Code protect block
	#pragma config CP3 = OFF			//Code protect block
	#pragma config CPB = OFF			//Code protect boot block
	#pragma config CPD = OFF			//Protect data eeprom
	#pragma config WRT0 = OFF			//Table write protect block
	#pragma config WRT1 = OFF			//Table write protect block
	#pragma config WRT2 = OFF			//Table write protect block
	#pragma config WRT3 = OFF			//Table write protect block
	#pragma config WRTB = OFF			//Boot block write protect
	#pragma config WRTC = OFF			//Config registers write protect
	#pragma config WRTD = OFF			//Data eeprom write protect
	#pragma config EBTR0 = OFF			//Table read protect block
	#pragma config EBTR1 = OFF			//Table read protect block
	#pragma config EBTR2 = OFF			//Table read protect block
	#pragma config EBTR3 = OFF			//Table read protect block
	#pragma config EBTRB = OFF			//Table read protect boot block

#else
//----- WE'RE NOT IN DEVELOPMENT MODE -----
	#pragma config OSC = HSPLL			//Ocsillator type
	#pragma config FCMEN = OFF			//Fail safe clock monitor (On = if osc fails the internal osc will be used)
	#pragma config IESO = OFF			//Internal / external ocsillator switchover mode
	#pragma config BOREN = SBORDIS		//Brown out reset (OFF = Disabled, ON = SBOREN bit enabled in software, NOSLP = enabled except SLEEP, SBOREN Disabled, SBORDIS = enabled, SBOREN Disabled)
	#pragma config PWRT = ON			//Powerup timer
#ifdef USING_3V3_POWER
	#pragma config BORV = 3				//Brown out reset voltage (0 = maximum, 3 = minimum - see device datasheet)
#else
	#pragma config BORV = 1				//Brown out reset voltage (0 = maximum, 3 = minimum - see device datasheet)
#endif
	#pragma config WDT = ON				//Watchdog timer
	#pragma config WDTPS = 128			//Watchdog postscaller
	#pragma config MCLRE = ON			//MCLR pin enable
	#pragma config LPT1OSC = OFF		//Low power timer 1 oscillator
	#pragma config PBADEN = OFF			//PortB AtoD enable
	#pragma config CCP2MX = PORTC		//CCP2 pin
	#pragma config DEBUG = OFF			//Background Debugger
	#pragma config LVP = OFF			//Low voltage in circuit programming
	#pragma config STVREN = ON			//Stack overflow reset
	#pragma config XINST = OFF			//Extended instruction set enable
	#pragma config CP0 = OFF			//Code protect block		(Note that setting code protection bits for devices powered at < 4.5V will mean that the
	#pragma config CP1 = OFF			//Code protect block		device will need to be powered at >4.5V if it ever needs to be programmed in circuit
	#pragma config CP2 = OFF			//Code protect block		again - i.e. for 3V3 circuits 5V power will have to be applied to reset the protection)
	#pragma config CP3 = OFF			//Code protect block
	#pragma config CPB = OFF			//Code protect boot block
	#pragma config CPD = OFF			//Protect data eeprom
	#pragma config WRT0 = OFF			//Table write protect block
	#pragma config WRT1 = OFF			//Table write protect block
	#pragma config WRT2 = OFF			//Table write protect block
	#pragma config WRT3 = OFF			//Table write protect block
	#pragma config WRTB = OFF			//Boot block write protect
	#pragma config WRTC = OFF			//Config registers write protect
	#pragma config WRTD = OFF			//Data eeprom write protect
	#pragma config EBTR0 = OFF			//Table read protect block
	#pragma config EBTR1 = OFF			//Table read protect block
	#pragma config EBTR2 = OFF			//Table read protect block
	#pragma config EBTR3 = OFF			//Table read protect block
	#pragma config EBTRB = OFF			//Table read protect boot block
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

	ClrWdt();

	//-------------------------
	//----- SETUP IO PINS -----
	//-------------------------
	//(Device will powerup with all IO pins as inputs)

	//Setup the state of any pins that are outputs (inputs are don't care)
	LATA = 0b00000000;
	LATB = 0b00000000;
	LATC = 0b00000000;
	LATD = 0b00000000;
	LATE = 0b00000000;

	//Setup the IO pin type (0 = output, 1 = input)
	TRISA = 0b00000000;
	TRISB = 0b00111111;
	TRISC = 0b10010000;
	TRISD = 0b00000000;
	TRISE = 0b00000000;

	//Setup internal pull ups
	INTCON2bits.RBPU = 0;				//Port B pull ups (0 = on)

	//----- SETUP THE A TO D PINS -----
	ADCON0 = 0b00000000;
	ADCON1 = 0b00001111;				//All AtoD pins are digital


	//----------------------------
	//----- SETUP INTERRUPTS -----
	//----------------------------


	//------------------------
	//----- SETUP TIMERS -----
	//------------------------
	//(INCLUDE THE USAGE OF ALL TIMERS HERE EVEN IF NOT SETUP HERE SO THIS IS THE ONE POINT OF
	//REFERENCE TO KNOW WHICH TIMERS ARE IN USE AND FOR WHAT).

	//----- SETUP TIMER 0 -----
	//Used for: Heartbeat
	T0CONbits.T08BIT = 0;				//Configured as a 16bit timer
	T0CONbits.T0CS = 0;					//Use internal instruction clock
	T0CONbits.T0SE = 0;
	T0CONbits.PSA = 0;					//Prescaller is in use (0 = in use)
	T0CONbits.T0PS2 = 0;				//Prescaler 1:2
	T0CONbits.T0PS1 = 0;
	T0CONbits.T0PS0 = 0;
	T0CONbits.TMR0ON = 1;				//Timer 0 on

	INTCONbits.TMR0IE = 1;				//Enable the Timer 0 irq

	//----- SETUP TIMER 1 -----
	//Used for: Available
	/*
	T1CONbits.RD16 = 1;					//R/W 16bits in 1 operation
	T1CONbits.T1RUN = 0;				//Don't use timer 1 dedicated oscillator
	T1CONbits.T1OSCEN = 0;				//Timer 1 dedicated oscillator disabled
	T1CONbits.T1CKPS1 = 0;				//1:1 prescale
	T1CONbits.T1CKPS0 = 0;
	T1CONbits.T1SYNC = 0;
	T1CONbits.TMR1CS = 0;				//Use internal instruction  clock
	T1CONbits.TMR1ON = 1;				//Enable timer 1
	*/

	//----- SETUP TIMER 2 -----
	//Used for: Display Backlight PWM output
	T2CONbits.T2OUTPS3 = 0;				//1:1 postscale
	T2CONbits.T2OUTPS2 = 0;
	T2CONbits.T2OUTPS1 = 0;
	T2CONbits.T2OUTPS0 = 0;
	T2CONbits.T2CKPS1 = 1;				//1:16 prescale
	T2CONbits.T2CKPS0 = 0;
	T2CONbits.TMR2ON = 1;				//Enable timer 2

	//----- SETUP TIMER 3 -----
	//Used for: Available
	/*
	T3CONbits.RD16 = 1;					// R/W 16bits in 1 operation
	T3CONbits.T3CCP2 = 0;				// R/W 16bits in 1 operation
	T3CONbits.T3CCP1 = 0;
	T3CONbits.T3CKPS1 = 0;				//1:1 prescale
	T3CONbits.T3CKPS0 = 0;
	T3CONbits.T3SYNC = 0;
	T3CONbits.TMR3CS = 0;				//Use internal instruction  clock
	T3CONbits.TMR3ON = 1;				//Enable timer 3
	*/

	//----- SETUP CCP1 FOR BACKLIGHT PWM -----
	PR2 = 0xff;							//Set PWM frequency (40MHz osc = 2.44kHz, 24MHz osc =  1.465kHz (slowest available))
	CCPR1L = 0;							//Backlight off

	CCP1CONbits.DC1B1 = 0;
	CCP1CONbits.DC1B0 = 0;
	CCP1CONbits.CCP1M3 = 1;				//Set to PWM mode
	CCP1CONbits.CCP1M2 = 1;
	CCP1CONbits.CCP1M1 = 0;
	CCP1CONbits.CCP1M0 = 0;




	//-----------------------------------------------------
	//----- SETUP SYNCHRONOUS SERIAL PORT (I2C / SSP) -----
	//-----------------------------------------------------



	//-----------------------
	//----- SETUP USART -----
	//-----------------------
	//Setup to use for debug 'printf' output

	//SETUP BAUD RATE
	TXSTAbits.BRGH = 0;					//Low speed mode
	BAUDCONbits.BRG16 = 1;				//16 bit baud rate generator
	SPBRGH = (((((DWORD)INSTRUCTION_CLOCK_FREQUENCY << 2) / 9600) / 16) - 1) >> 8;			//(((OSCILLATOR_FREQUENCY / DESIRED_BAUD) / DIVIDE_VALUE) - 1)
	SPBRG = (((((DWORD)INSTRUCTION_CLOCK_FREQUENCY << 2) / 9600) / 16) - 1) & 0x00ff;		//Note that different BRGH & BRG16 settings use different divide values - see device datasheet

	//SETUP TX
	TXSTAbits.TX9 = 0;					//8 bit tx
	TXSTAbits.TXEN = 1;					//Transmit enabled
	TXSTAbits.SYNC = 0;					//Asynchronous mode
	
	//SETUP RX
	RCSTAbits.RX9 = 0;					//8 bit rx
	RCSTAbits.ADDEN = 0;				//Address detect disabled
	RCSTAbits.CREN = 1;					//Enable receiver
	RCSTAbits.SPEN = 1;					//Serial port enabled

	//------------------------
	//----- USING PRINTF -----
	//------------------------
	//If using C18 and the compiler small memory model (devices <= 64K of program memory) you can use '(rom far char*)' to stop a build warning
	//being generated, otherwise omit the (rom far char*)

	//Example displaying just text:-
	//printf ((rom far char*)"\n----- POWER-UP ----- \n\r");

	//Example displaying a value:-
	//temp_value = 199;
	//itoa((int) temp_value, temp_string);
	//printf ((rom far char*)"%s\n\r", temp_string);

	//Example of getting terminal input:-
	//printf ((rom far char*)"\nEnter 5 digit value\n\r");
	//getsUSART((char*)temp_string,5);
	//temp_string[5] = 0x00;
	//printf ((rom far char*)"You typed: %s\n\r", temp_string);


	//-----------------------------
	//----- ENABLE INTERRUPTS -----
	//-----------------------------
   	INTCONbits.GIEL = 1;        	// enable lo priority interrupts
	INTCONbits.GIEH = 1;        	// enable hi priority interrupts 





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
void main ( void )
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












//################################################>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//################################################>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//########## MANUFACTURE TEST FUNCTIONS ##########>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//################################################>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//################################################>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//REMOVE FROM NORMAL PROJECTS - THIS SECTION IS ONLY USED FOR FACTORY MANUFACTURE TEST

		//CHECK FOR TEST BOX USART RX PIN TEST
		//(We want to see a USART framing error for at least 100mS)
		if (PORTCbits.RC7 == 0)
		{
			//Pause 50mS
			general_use_10ms_timer = 5;
			while(general_use_10ms_timer)
				ClrWdt();
	
			//Reset the framing error flag and check for another framing error

			general_use_10ms_timer = 2;
			while(general_use_10ms_timer)
				ClrWdt();
			if (PORTCbits.RC7 == 0)
			{
				//RX PIN IS TRIPPED - WAIT FOR IT TO CLEAR
				while (PORTCbits.RC7 == 0)
				{
					ClrWdt();
					general_use_10ms_timer = 5;
					while(general_use_10ms_timer);
				}
				//PAUSE 200mS
				general_use_10ms_timer = 20;
				while(general_use_10ms_timer)
					ClrWdt();

				//NOW DO CONTINUOUS TX FOR 800mS
				general_use_10ms_timer = 80;
				LATAbits.LATA5 = 1;	//RS485_DIRECTION_TO_TX = 1;
				while(general_use_10ms_timer)
				{
					ClrWdt();
					TXREG = 0x00;
					while (TXSTAbits.TRMT == 0);
				}
				LATAbits.LATA5 = 0;	//RS485_DIRECTION_TO_TX = 0;
				
				//NOW FLASH ON SCL LED
				LATCbits.LATC3 = 0;
				TRISCbits.TRISC3 = 0;
				general_use_10ms_timer = 30;
				while(general_use_10ms_timer)
					ClrWdt();
				TRISCbits.TRISC3 = 1;

				//NOW FLASH ON SDA LED
				LATCbits.LATC4 = 0;
				TRISCbits.TRISC4 = 0;
				general_use_10ms_timer = 30;
				while(general_use_10ms_timer)
					ClrWdt();
				TRISCbits.TRISC4 = 1;
	
			}


			if ((RCSTAbits.FERR) || (RCSTAbits.OERR))
			{
				RCSTAbits.CREN = 0;
				RCSTAbits.CREN = 1;
			}
			
		}

//################################################<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//################################################<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//################################################<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//################################################<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//################################################<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




		
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
	BYTE switches_read_working;
	static BYTE switches_last = 0;
	static BYTE switches_debounced_last = 0;


	//RESET THE NEW SWITCH PRESS REGISTER
	switches_new = 0;

	//ONLY DO EVERY 10MS FOR DEBOUNCE
	if (read_switches_flag == 0)
		return;

	read_switches_flag = 0;

	//GET THE SWITCH INPUTS
	switches_read = ~(PORTB | 0xc0);

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
		
		//Set the backlight to full bridghtness
		display_backlight_target = 0xff;
		display_backlight_current = 0xff;
		display_backlight_off_timer = DISPLAY_BACKLIGHT_DIM_DELAY;
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

//***********************************
//***** HIGH PRIORITY INTERRUPT *****
//***********************************
#pragma code highVector=0x08			//This is the vector address for a PIC18 high priority interrupt 
void athighvector (void)
{
	_asm 	GOTO	interruptisrh	_endasm
}
#pragma code 		//(Return to normal)

//**********************************
//***** LOW PRIORITY INTERRUPT *****
//**********************************
#pragma code lowVector=0x18			//This is the vector address for a PIC18 low priority interrupt 
void atlowvector (void)
{
	_asm	goto	interruptisrl	_endasm
}
#pragma code 		//(Return to normal)



//*************************************************************
//*************************************************************
//********** HIGH PRIORITY INTERRUPT SERVICE ROUTINE **********
//*************************************************************
//*************************************************************
//#pragma interrupt interruptisrh			//<<< This is the normal C18 compiler definition to indicate the interrupt high priority function.
#pragma interruptlow interruptisrh			//<<<For PIC18 devices with the silicon bug that means the RETFIE FAST instruction cannot be used use this C18
											//compiler definition so that the low priority context saving aproach is used (if not sure just use this)
void interruptisrh (void)
{
	static BYTE hb_100ms_timer = 0;
	static BYTE hb_1sec_timer = 0;

	//************************
	//***** USART TX IRQ *****
	//************************
	//if(PIE1bits.TXIE && PIR1bits.TXIF)
	//	usart_transmit_irq();

	//************************
	//***** USART RX IRQ *****
	//************************
	//if(PIE1bits.RCIE && PIR1bits.RCIF)
	//	usart_receive_irq();


	//***************************************
	//********** TIMER 0 HEARTBEAT **********
	//***************************************
	if(INTCONbits.TMR0IF && INTCONbits.TMR0IE)
	{
		INTCONbits.TMR0IF = 0;					//Reset the timer 0 irq flag

		_asm
		//Reset timer for the next rollover (in assembler for accuracy)
		movff	TMR0L,asm_irq_temp				//read the current timer value (read low byte loads the high byte)
		movff	TMR0H,asm_irq_temp1
		movlw	((0x10000 - (((INSTRUCTION_CLOCK_FREQUENCY / 100) / 2) - 14)) & 0x00ff)		//<<< /100 for 10mS irq, /1000 for 1mS irq
		addwf	asm_irq_temp,1,0				//(1 = file register, 0 = access ram)
		movlw	((0x10000 - (((INSTRUCTION_CLOCK_FREQUENCY / 100) / 2) - 14)) >> 8)			//<<< /100 for 10mS irq, /1000 for 1mS irq
		addwfc	asm_irq_temp1,1,0				//(1 = file register, 0 = access ram)
		movff	asm_irq_temp1,TMR0H				//Store new value (high byte first)
		movff	asm_irq_temp,TMR0L
		_endasm

		//The above code takes 12 instruction cycles
		//The timer is inhibited for 2 instruction cycles after the low byte is written
		//Therefore subtract 14 from the value to be written
		//For 24MHz oscillator 1mS irq = 6,000 cycles.  Minus 14 = 5986.  /2 as timer uses 1:2 prescaller = 2993.  0x10000 - 2993 = 0xF44F
		//For 24MHz oscillator 10mS irq = 60,000 cycles.  Minus 14 = 59986.  /2 as timer uses 1:2 prescaller = 29993.  0x10000 - 29993 = 0x8AD7
		//For 40MHz oscillator 1mS irq = 10,000 cycles.  Minus 14 = 9986.  /2 as timer uses 1:2 prescaller = 4993.  0x10000 - 4993 = 0xec7f
		//For 40MHz oscillator 10mS irq = 100,000 cycles.  Minus 14 = 99986.  /2 as timer uses 1:2 prescaller = 49993.  0x10000 - 49993 = 0x3cb7


		//------------------------------
		//------------------------------
		//----- HERE EVERY 10 mSec -----
		//------------------------------
		//------------------------------

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


		//----- DISPLAY BACKLIGHT CONTROL -----
		//(Write to display_backlight_target to set a new level to be faded to)
		//(Write to display_backlight_target and then display_backlight_current to set a new level to be snapped to)
		display_backlight_current &= 0xfe;			//Clear the low bit if set as we change by 2 each time
		display_backlight_target &= 0xfe;
		if (display_backlight_current != display_backlight_target)
		{
			//BACKLIGHT LEVEL HAS NOT YET REACHED TARGET
			//Backlight resolution is 8 bits.  If we change the value by 2 every 10mS we get a 0 to full fade in 1.28 seconds (((256 steps) / 2) * 10mS)
			if (display_backlight_current > display_backlight_target)
			{
				//NEED TO FADE DOWN
				display_backlight_current -= 2;
			}
			else
			{
				//NEED TO FADE UP
				display_backlight_current += 2;
			}
		}

		//OUTPUT THE NEW PWM LEVEL
		if (display_backlight_current == 0xfe)
		{
			//At max level (special case)
			if (CCPR1L != 0xff)
			{
				CCP1CONbits.DC1B1 = 1;
				CCP1CONbits.DC1B0 = 1;
				CCPR1L = 0xff;
			}
		}
		else if (CCPR1L != display_backlight_current)
		{
			// < max level
			CCP1CONbits.DC1B1 = 0;
			CCP1CONbits.DC1B0 = 0;
			CCPR1L = display_backlight_current;
		}





		hb_100ms_timer++;
		if (hb_100ms_timer == 10)
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


			//----- DISPLAY BACKLIGHT OFF TIMER -----
			if (display_backlight_off_timer)
			{
				display_backlight_off_timer--;
				if (display_backlight_off_timer == 0)
					display_backlight_target = DISPLAY_BACKLIGHT_DIM_VALUE;
			}




			hb_1sec_timer++;
			if (hb_1sec_timer == 10)
			{
				//----------------------------
				//----------------------------
				//----- HERE EVERY 1 Sec -----
				//----------------------------
				//----------------------------
				hb_1sec_timer = 0;


			}
		}
	}



	//<<<<< ADD OTHER HIGH PRIORITY INTERRUPTS HERE <<<<<
	
	
}	
	

//************************************************************
//************************************************************
//********** LOW PRIORITY INTERRUPT SERVICE ROUTINE **********
//************************************************************
//************************************************************
#pragma interruptlow interruptisrl			//<<< C18 compiler definition to indicate the interrupt low priority function.
void interruptisrl (void)
{


	//<<<<< ADD OTHER LOW PRIORITY INTERRUPTS HERE <<<<<
	
	
}
#pragma code 		//(Return to normal)












