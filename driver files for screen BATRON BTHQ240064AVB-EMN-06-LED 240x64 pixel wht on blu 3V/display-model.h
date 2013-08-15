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
//---------- SCREEN:	BATRON BTHQ240064AVB-EMN-06-LED White-COG		----------
//---------- Resolution:	240 x 64 pixel								----------
//---------- Colour:		White on Blue								----------
//---------- Driver IC:		S1D10605D04B (x2)							----------
//---------- Interface:		Serial (SPI)								----------
//---------- Voltage:		3.0V										----------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------


/*
###############################################
##### TO USE THIS DRIVER IN A NEW PROJECT #####
###############################################
- Check this header file and the display.h header file for defines to setup and any usage notes
- Configure your SPI serial port as follows:-
  Display reads data on the rising edge of the clock (output from PIC on the falling edge).  Max bus speed 4MHz.
  Data is sent from bit7 to bit 0.  Data is converted into parallel data on the rising edge of the 8th clock.
  
  For C18 compiler add this to the main application initialise:-
	//SETUP SPI BUS FOR COMMS TO SCREEN
	SSPSTATbits.SMP = 1;
	SSPSTATbits.CKE = 1;		//Data is valid on the rising edge of the clock (Transmit occurs on transition from active to Idle clock state)
	SSPCON1bits.CKP = 0;		//Clock low in idle bus state
	SSPCON1bits.SSPM3 = 0;		//SPI in master mode.  Set clock to Fosc/16 (Max frequency screen will accept = 4MHz)
	SSPCON1bits.SSPM2 = 0;
	SSPCON1bits.SSPM1 = 0;
	SSPCON1bits.SSPM0 = 1;
	SSPCON1bits.SSPEN = 1;		//Enable SSP Port


  For C30 compiler add this to the main application initialise:-
	//SETUP SPI BUS FOR COMMS TO SCREEN
	w_temp = SPI1BUF;
	SPI1STAT = 0;
	SPI1CON1 = 0b0000001100111110;	//SPI in master mode (SPI1STATbits.SPIEN must be 0 to write to this register)
									//Data is valid on the rising edge of the clock (Transmit occurs on transition from active to Idle clock state)
									//Clock low in idle bus state
									//Prescallers 4:1 1:1 = 4MHz(Max frequency screen will accept = 4MHz)
	SPI1STATbits.SPIEN = 1;			//Enable the port

  For C32 compiler add this to the main application initialise:-
	SpiChnOpen(1,						//SPI Channel
				(SPICON_ON | SPICON_MSTEN | SPICON_CKE | SPICON_SMP | SPICON_FRZ),	 //SPICON_CKP = idle high, SPICON_CKE = output changes on active to idle clock, SPICON_SMP = Input data sampled at end of data output time
				20);			//Fpb divisor to extract the baud rate: BR = Fpb / fpbDiv.  A value between 2 and 1024
								//Baud = 20 = 4MHz @ 80MHz bus speed (Max frequency screen will accept = 4MHz)


USING THE SCREEN IN LANDSCAPE WITH THE RIBBON CABLE UPPERMOST:
Bitmap converter rotation setting: 270
Screen coordinates when calling driver functions:
  63,0               63,239

  0,0                 0,239
*/




//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef DISPLAY_MODEL_C_INIT				//(Include this section only once for each source file that includes this header file)
#define	DISPLAY_MODEL_C_INIT


//----------------------------------------------
//----- DEFINE TARGET COMPILER & PROCESSOR -----
//----------------------------------------------
//(ONLY 1 SHOULD BE INCLUDED, COMMENT OUT OTHERS)
//#define	DISPLAY_USING_MICROCHIP_C18_COMPILER
#define	DISPLAY_USING_MICROCHIP_C30_COMPILER
//#define	DISPLAY_USING_MICROCHIP_C32_COMPILER
//<< add other compiler types here


//-------------------------
//----- DISPLAY SETUP -----
//-------------------------
#define	DISPLAY_WIDTH_PIXELS		240				//Width is the X coordinate
#define	DISPLAY_HEIGHT_PIXELS		64				//Height is the Y coordinate and is used for the page address (divide this value by 8 to give the number of pages)
#define	DISPLAY_WIDTH_START_OFFSET	0				//Offset to first actual display position (0 if not applicable)
#define	DISPLAY_DEFAULT_CONTRAST	36


//------------------------
//----- USER OPTIONS -----
//------------------------
#define	DISPLAY_USE_LOCAL_RAM_BUFFER				//Include to optionally use a ram array to hold a copy of the display bitmap output to speed up display and for displays
													//where you cannot read back display data (e.g. many serial displays).  Comment out if memory not available

//#define	ORIENTATION_IS_0							//Only one of these should be defined
//#define	ORIENTATION_IS_90							//Only one of these should be defined
//#define	ORIENTATION_IS_180							//Only one of these should be defined
#define	ORIENTATION_IS_270							//Only one of these should be defined

#ifdef ORIENTATION_IS_180
	#define	INVERT_Y_AXIS_COORDINATES				//Comment out normally, include if you need to reverse the direction of display on the Y axis.
#endif



#ifdef DISPLAY_USING_MICROCHIP_C18_COMPILER
//########################################
//########################################
//##### USING MICROCHIP C18 COMPILER #####
//########################################
//########################################

//----------------------
//----- IO DEFINES -----
//----------------------
//SPI BUS CONTROL REGISTERS:-
//#define	DISPLAY_SPI_BUF_FULL			SSP1STATbits.BF			//>0 when the SPI receive buffer contains a received byte, also signifying that transmit is complete
//#define	DISPLAY_SPI_TX_BYTE(data)		SSP1BUF = data			//Macro to write a byte and start transmission over the SPI bus
//#define	DISPLAY_SPI_RX_BYTE_BUFFER		SSP1BUF					//Register to read last received byte from
//##### Due to silicon bugs in the PIC18F4620 the following alternative defines are needed as workarounds #####
#define	DISPLAY_SPI_BUF_FULL			PIR1bits.SSPIF
#define	DISPLAY_SPI_TX_BYTE(data)		PIR1bits.SSPIF = 0; SSPBUF = data
#define	DISPLAY_SPI_RX_BYTE_BUFFER		SSPBUF
//(Although we don't read from the display some SPI bus peripherals require reading rx before starting a new transmission)

//CONTROL PINS:-
#define DISPLAY_A0(state)		LATEbits.LATE0 = state	//0 = Command, 1=Data
#define DISPLAY_CS1(state)		LATEbits.LATE1 = state	//0 = select controller 1
#define DISPLAY_CS2(state)		LATDbits.LATD7 = state	//0 = select controller 2
#define	DISPLAY_RESET(state)	LATEbits.LATE2 = state	//0 = reset controller

//###############################################
//###############################################
//##### END OF USING MICROCHIP C18 COMPILER #####
//###############################################
//###############################################
#endif		//#ifdef DISPLAY_USING_MICROCHIP_C18_COMPILER


#ifdef DISPLAY_USING_MICROCHIP_C30_COMPILER
//########################################
//########################################
//##### USING MICROCHIP C30 COMPILER #####
//########################################
//########################################

//----------------------
//----- IO DEFINES -----
//----------------------
//SPI BUS CONTROL REGISTERS:-
#define	DISPLAY_SPI_BUF_FULL			IFS0bits.SPI1IF 		//>0 when the SPI receive buffer contains a received byte, also signifying that transmit is complete
#define	DISPLAY_SPI_TX_BYTE(data)		IFS0bits.SPI1IF = 0; SPI1BUF = data			//Macro to write a byte and start transmission over the SPI bus
#define	DISPLAY_SPI_RX_BYTE_BUFFER		SPI1BUF					//Register to read last received byte from
//(Although we don't read from the display some SPI bus peripherals require reading rx before starting a new transmission)

//CONTROL PINS:-
#define DISPLAY_A0(state)		_LATD0 = state			//0 = Command, 1=Data
#define DISPLAY_CS1(state)		_LATD1 = state			//0 = select controller 1
#define DISPLAY_CS2(state)		_LATD2 = state			//0 = select controller 2
#define	DISPLAY_RESET(state)	_LATD3 = state			//0 = reset controller


//###############################################
//###############################################
//##### END OF USING MICROCHIP C30 COMPILER #####
//###############################################
//###############################################
#endif		//#ifdef DISPLAY_USING_MICROCHIP_C30_COMPILER


#ifdef DISPLAY_USING_MICROCHIP_C32_COMPILER
//########################################
//########################################
//##### USING MICROCHIP C32 COMPILER #####
//########################################
//########################################

//----------------------
//----- IO DEFINES -----
//----------------------
//SPI BUS CONTROL REGISTERS:-
#define	DISPLAY_SPI_BUF_FULL			SpiChnGetRxIntFlag(1) 		//>0 when the SPI receive buffer contains a received byte, also signifying that transmit is complete
#define	DISPLAY_SPI_TX_BYTE(data)		SpiChnClrRxIntFlag(1); SpiChnPutC(1, data)		//Macro to write a byte and start transmission over the SPI bus
#define	DISPLAY_SPI_RX_BYTE_BUFFER		SpiChnGetC(1)				//Macro to read last received byte from
//(Although we don't read from the display some SPI bus peripherals require reading rx before starting a new transmission)

//CONTROL PINS:-
#define DISPLAY_A0(state)		(state ? mPORTDSetBits(BIT_0) : mPORTDClearBits(BIT_0))		//0 = Command, 1=Data
#define DISPLAY_CS1(state)		(state ? mPORTDSetBits(BIT_1) : mPORTDClearBits(BIT_1))		//0 = select controller 1
#define DISPLAY_CS2(state)		(state ? mPORTDSetBits(BIT_2) : mPORTDClearBits(BIT_2))		//0 = select controller 2
#define	DISPLAY_RESET(state) 	(state ? mPORTDSetBits(BIT_3) : mPORTDClearBits(BIT_3))		//0 = reset controller

//###############################################
//###############################################
//##### END OF USING MICROCHIP C32 COMPILER #####
//###############################################
//###############################################
#endif		//#ifdef DISPLAY_USING_MICROCHIP_C32_COMPILER







#endif



//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef DISPLAY_MODEL_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------
void display_set_address (WORD x_coord, WORD y_coord_page);
void display_write_command (BYTE data);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
void display_model_initialise(void);
void display_model_set_contrast(BYTE contrast_value);
void display_write_bitmap_byte (BYTE bitmap_mask, BYTE bitmap_data, WORD x_byte_coord, WORD y_byte_coord);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern void display_model_initialise(void);
extern void display_model_set_contrast(BYTE contrast_value);
extern void display_write_bitmap_byte (BYTE bitmap_mask, BYTE bitmap_data, WORD x_byte_coord, WORD y_byte_coord);


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef DISPLAY_MODEL_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------




//----- OPTIONAL BUFFER FOR LOCAL COPY OF SCREEN OUTPUT DATA -----
#ifdef DISPLAY_USE_LOCAL_RAM_BUFFER
	#ifdef DISPLAY_USING_MICROCHIP_C18_COMPILER
		#pragma udata display_local_buffer_ram_section	//This is the PIC C18 compiler command to use the specially defined section in the linker script (PIC18 project uses a modified linker script for this)
	#endif											//(C18 large array requirement to use a special big section of ram defined in the linker script)

	#if (DISPLAY_HEIGHT_PIXELS & 0x0007)
		BYTE display_copy_buffer[DISPLAY_WIDTH_PIXELS][(DISPLAY_HEIGHT_PIXELS >> 3) + 1)];		//DISPLAY_WIDTH_PIXELS is not a multiple of 8 so need to add 1 for remainder
	#else
		BYTE display_copy_buffer[DISPLAY_WIDTH_PIXELS][(DISPLAY_HEIGHT_PIXELS >> 3)];			//DISPLAY_WIDTH_PIXELS is a multiple of 8
	#endif

	#ifdef DISPLAY_USING_MICROCHIP_C18_COMPILER
		#pragma udata
	#endif
#endif


//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------


#endif
