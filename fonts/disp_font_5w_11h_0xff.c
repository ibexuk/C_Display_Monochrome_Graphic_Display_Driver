
#ifdef CONSTANT			//This is defined in main.h and as all source files include this file it will be defined already for any source code file using include on this file.
#define	FONT			//So define this.  By doing this we acheive providing the external declaration for files including this font file, and providing the actual functions
						//and tables when the compiler gets to adding this file to the project.
#endif

#include "..\main.h"					//(Include the project global defines file)



//----- FOR A NEW FONT -----
//	Add an include of this file to the top of any .c files you will be calling display__string fucntions from using this font.
//	Adjust the 8 defines / names shown below with <<< <<<
//	You may need to adjust the 'const' of the BitFontCreator Pro output tables to deal with compiler requirements (for instance if additional qualifiers
//	are needed or if special attributes are needed to deal with memory space allocation).  Do this on the BitFontCreator Pro C Output File contents:
//		FIND 					REPLACE WITH
//		const unsigned char		FONT_CONSTANT_BYTE
//		const unsigned int		FONT_CONSTANT_WORD
//	and adjust the defines of these below.
//	Paste the contents of the BitFontCreator Pro C Output File between the markers at the end of the file (or overwrite if updating the font)



#ifdef FONT
//---------------------------------------------------------------
//----- EXTERNAL DECLARATIONS WHEN INCLUDING THIS FONT FILE -----
//---------------------------------------------------------------
extern CONSTANT BYTE *disp_font_5w_11h_0xff (WORD character);				//<<< SET NAME FOR A NEW FONT <<<< (Needs to be different to other font's)


#else
//---------------------------------------------
//----- INTERNAL DECLARATIONS & FUNCTIONS -----
//---------------------------------------------

#define FONT_HEIGHT					11										//<<< SET CORRECTLY FOR A NEW FONT <<<< (See 'Font height:' at the start of the BitFontCreator C output file)
#define INTER_CHARACTER_SPACE		1										//<<< SET AS REQUIRED FOR THIS FONT


//#define	FONT_CONSTANT_BYTE	CONSTANT BYTE	//Adjust if necessary for your compiler
#define	FONT_CONSTANT_BYTE 		CONSTANT BYTE __attribute__((space(psv)))		

//#define	FONT_CONSTANT_WORD 	CONSTANT WORD	//Adjust if necessary for your compiler
#define	FONT_CONSTANT_WORD 		CONSTANT WORD __attribute__((space(psv)))


//Deal with ensuring all names are unique
#ifdef data_table
#undef data_table
#endif
#ifdef offset_table
#undef offset_table
#endif
#ifdef index_table
#undef index_table
#endif
#ifdef width_table
#undef width_table
#endif
#define	data_table		dt_disp_font_5w_11h_0xff						//<<< MAKE A UNIQUE NAME FOR THIS FONT (e.b. 'dt_' followed by the font name - required to deal with compilers that error if names are re-used globally)
#define	offset_table	ot_disp_font_5w_11h_0xff						//<<< MAKE A UNIQUE NAME FOR THIS FONT (e.b. 'ot_' followed by the font name - required to deal with compilers that error if names are re-used globally)
#define	index_table 	it_disp_font_5w_11h_0xff						//<<< MAKE A UNIQUE NAME FOR THIS FONT (e.b. 'it_' followed by the font name - required to deal with compilers that error if names are re-used globally)
#define	width_table 	wt_disp_font_5w_11h_0xff						//<<< MAKE A UNIQUE NAME FOR THIS FONT (e.b. 'wt_' followed by the font name - required to deal with compilers that error if names are re-used globally)


//Declarations of the tables below
FONT_CONSTANT_BYTE data_table[];
FONT_CONSTANT_WORD offset_table[];
FONT_CONSTANT_BYTE index_table[];
FONT_CONSTANT_BYTE width_table[];

//Global display driver variables we will load
extern WORD display_bitmap_height;
extern WORD display_bitmap_width;
extern BYTE display_font_spacing;



//*****************************
//***** GET FONT FUNCTION *****
//*****************************
//Call with:
//	character = code for the character
//Returns:
//	Start address of the character data in memory
//	display_bitmap_width = character width (pixels)
//	display_bitmap_height = character height (pixels)
//	display_font_spacing = the spacing you have defined to occur between each character (pixels)

CONSTANT BYTE *disp_font_5w_11h_0xff (WORD character)							//<<< SET NAME FOR A NEW FONT <<<< (to match external name above)
{
	WORD index;
	CONSTANT BYTE *p_character_data;
	
	//SET THE CHARACTER HEIGHT
	display_bitmap_height = FONT_HEIGHT;			//Load the global variable
	
	//SET THE INTER CHARACTER SPACE
	display_font_spacing = INTER_CHARACTER_SPACE;	//Load the global variable
	
	//GET INDEX OF THE CHARACTER
	index = index_table[character];					//For many uses if you want to save space you could remove this, use 'index = character - 0x20' instead, and delete the index_table to save code space. 
	
	//GET THE CHARACTER WIDTH
	display_bitmap_width = width_table[index];		//Load the global variable
	
	//SET THE CHARACTER DATA START ADDRESSS
	p_character_data = &data_table[0];
	p_character_data += offset_table[index];

	//GET THE CHARACTER WIDTH AND EXIT
	return(p_character_data);
}	




//<<<< The BitFontCreator Pro C File Contents Goes Here >>>>
//<<<< START OF THE BIT FONT CREATOR PRO C FILE CONTENTS >>>>




/*******************************************************************************************
  Font name: Ericsson GA628
  Font width: varialbe (proportional font)
  Font height: 11
  Encode: ANSI+

  Data length: 8 bits
  Data format: Big Endian, Row based, Row preferred, Unpacked

  Create time: 08:37 06-03-2010  by BitFontCreator Pro (e-mail: support@iseatech.com)
 *******************************************************************************************/

/*******************************************************************************************
  Data table provides the bitmap data of each character.

  To get the starting data offset of character 'A', you can use the following expression:

     FONT_CONSTANT_BYTE index = index_table['A'];
     FONT_CONSTANT_WORD offset = offset_table[index];
     FONT_CONSTANT_BYTE *pData = data_table[offset];

 *******************************************************************************************/
FONT_CONSTANT_BYTE data_table[] = {

/* character 0x0020 (' '): (width=5, offset=0) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x0021 ('!'): (width=1, offset=11) */
0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 
0x80, 0x00, 0x00, 

/* character 0x0022 ('"'): (width=3, offset=22) */
0x00, 0x00, 0xA0, 0xA0, 0xA0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x0023 ('#'): (width=5, offset=33) */
0x00, 0x00, 0x50, 0x50, 0xF8, 0x50, 0xF8, 0x50, 
0x50, 0x00, 0x00, 

/* character 0x0024 ('$'): (width=5, offset=44) */
0x00, 0x00, 0x20, 0x78, 0xA0, 0x70, 0x28, 0xF0, 
0x20, 0x00, 0x00, 

/* character 0x0025 ('%'): (width=5, offset=55) */
0x00, 0x00, 0xC0, 0xC8, 0x10, 0x20, 0x40, 0x98, 
0x18, 0x00, 0x00, 

/* character 0x0026 ('&'): (width=5, offset=66) */
0x00, 0x00, 0x60, 0x90, 0xA0, 0x40, 0xA8, 0x90, 
0x68, 0x00, 0x00, 

/* character 0x0027 ('''): (width=2, offset=77) */
0x00, 0x00, 0xC0, 0x40, 0x80, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x0028 ('('): (width=3, offset=88) */
0x00, 0x00, 0x20, 0x40, 0x80, 0x80, 0x80, 0x80, 
0x40, 0x20, 0x00, 

/* character 0x0029 (')'): (width=3, offset=99) */
0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x20, 
0x40, 0x80, 0x00, 

/* character 0x002A ('*'): (width=5, offset=110) */
0x00, 0x00, 0x00, 0x20, 0xA8, 0x70, 0xA8, 0x20, 
0x00, 0x00, 0x00, 

/* character 0x002B ('+'): (width=5, offset=121) */
0x00, 0x00, 0x00, 0x20, 0x20, 0xF8, 0x20, 0x20, 
0x00, 0x00, 0x00, 

/* character 0x002C (','): (width=2, offset=132) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x40, 
0x80, 0x00, 0x00, 

/* character 0x002D ('-'): (width=5, offset=143) */
0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x002E ('.'): (width=2, offset=154) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 
0xC0, 0x00, 0x00, 

/* character 0x002F ('/'): (width=5, offset=165) */
0x00, 0x00, 0x00, 0x08, 0x10, 0x20, 0x40, 0x80, 
0x00, 0x00, 0x00, 

/* character 0x0030 ('0'): (width=5, offset=176) */
0x00, 0x00, 0x70, 0x88, 0x98, 0xA8, 0xC8, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x0031 ('1'): (width=3, offset=187) */
0x00, 0x00, 0x40, 0xC0, 0x40, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x0032 ('2'): (width=5, offset=198) */
0x00, 0x00, 0x70, 0x88, 0x08, 0x10, 0x20, 0x40, 
0xF8, 0x00, 0x00, 

/* character 0x0033 ('3'): (width=5, offset=209) */
0x00, 0x00, 0xF8, 0x10, 0x20, 0x70, 0x08, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x0034 ('4'): (width=5, offset=220) */
0x00, 0x00, 0x10, 0x30, 0x50, 0x90, 0xF8, 0x10, 
0x10, 0x00, 0x00, 

/* character 0x0035 ('5'): (width=5, offset=231) */
0x00, 0x00, 0xF8, 0x80, 0xF0, 0x08, 0x08, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x0036 ('6'): (width=5, offset=242) */
0x00, 0x00, 0x30, 0x40, 0x80, 0xF0, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x0037 ('7'): (width=5, offset=253) */
0x00, 0x00, 0xF8, 0x08, 0x10, 0x20, 0x40, 0x40, 
0x40, 0x00, 0x00, 

/* character 0x0038 ('8'): (width=5, offset=264) */
0x00, 0x00, 0x70, 0x88, 0x88, 0x70, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x0039 ('9'): (width=5, offset=275) */
0x00, 0x00, 0x70, 0x88, 0x88, 0x78, 0x08, 0x10, 
0x60, 0x00, 0x00, 

/* character 0x003A (':'): (width=2, offset=286) */
0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0xC0, 0xC0, 
0x00, 0x00, 0x00, 

/* character 0x003B (';'): (width=2, offset=297) */
0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0xC0, 0x40, 
0x80, 0x00, 0x00, 

/* character 0x003C ('<'): (width=4, offset=308) */
0x00, 0x00, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 
0x10, 0x00, 0x00, 

/* character 0x003D ('='): (width=5, offset=319) */
0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0xF8, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x003E ('>'): (width=4, offset=330) */
0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x20, 0x40, 
0x80, 0x00, 0x00, 

/* character 0x003F ('?'): (width=5, offset=341) */
0x00, 0x00, 0x70, 0x88, 0x08, 0x10, 0x20, 0x00, 
0x20, 0x00, 0x00, 

/* character 0x0040 ('@'): (width=5, offset=352) */
0x00, 0x00, 0x70, 0x88, 0xB8, 0xA8, 0xB0, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x0041 ('A'): (width=5, offset=363) */
0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0xF8, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x0042 ('B'): (width=5, offset=374) */
0x00, 0x00, 0xF0, 0x88, 0x88, 0xF0, 0x88, 0x88, 
0xF0, 0x00, 0x00, 

/* character 0x0043 ('C'): (width=5, offset=385) */
0x00, 0x00, 0x70, 0x88, 0x80, 0x80, 0x80, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x0044 ('D'): (width=5, offset=396) */
0x00, 0x00, 0xE0, 0x90, 0x88, 0x88, 0x88, 0x90, 
0xE0, 0x00, 0x00, 

/* character 0x0045 ('E'): (width=5, offset=407) */
0x00, 0x00, 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 
0xF8, 0x00, 0x00, 

/* character 0x0046 ('F'): (width=5, offset=418) */
0x00, 0x00, 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 
0x80, 0x00, 0x00, 

/* character 0x0047 ('G'): (width=5, offset=429) */
0x00, 0x00, 0x70, 0x88, 0x80, 0xB8, 0x88, 0x88, 
0x78, 0x00, 0x00, 

/* character 0x0048 ('H'): (width=5, offset=440) */
0x00, 0x00, 0x88, 0x88, 0x88, 0xF8, 0x88, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x0049 ('I'): (width=3, offset=451) */
0x00, 0x00, 0xE0, 0x40, 0x40, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x004A ('J'): (width=5, offset=462) */
0x00, 0x00, 0x38, 0x10, 0x10, 0x10, 0x10, 0x90, 
0x60, 0x00, 0x00, 

/* character 0x004B ('K'): (width=5, offset=473) */
0x00, 0x00, 0x88, 0x90, 0xA0, 0xC0, 0xA0, 0x90, 
0x88, 0x00, 0x00, 

/* character 0x004C ('L'): (width=5, offset=484) */
0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 
0xF8, 0x00, 0x00, 

/* character 0x004D ('M'): (width=5, offset=495) */
0x00, 0x00, 0x88, 0xD8, 0xA8, 0xA8, 0x88, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x004E ('N'): (width=5, offset=506) */
0x00, 0x00, 0x88, 0x88, 0xC8, 0xA8, 0x98, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x004F ('O'): (width=5, offset=517) */
0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x0050 ('P'): (width=5, offset=528) */
0x00, 0x00, 0xF0, 0x88, 0x88, 0xF0, 0x80, 0x80, 
0x80, 0x00, 0x00, 

/* character 0x0051 ('Q'): (width=5, offset=539) */
0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0xA8, 0x90, 
0x68, 0x00, 0x00, 

/* character 0x0052 ('R'): (width=5, offset=550) */
0x00, 0x00, 0xF0, 0x88, 0x88, 0xF0, 0xA0, 0x90, 
0x88, 0x00, 0x00, 

/* character 0x0053 ('S'): (width=5, offset=561) */
0x00, 0x00, 0x78, 0x80, 0x80, 0x70, 0x08, 0x08, 
0xF0, 0x00, 0x00, 

/* character 0x0054 ('T'): (width=5, offset=572) */
0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 
0x20, 0x00, 0x00, 

/* character 0x0055 ('U'): (width=5, offset=583) */
0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x0056 ('V'): (width=5, offset=594) */
0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 0x88, 0x50, 
0x20, 0x00, 0x00, 

/* character 0x0057 ('W'): (width=5, offset=605) */
0x00, 0x00, 0x88, 0x88, 0x88, 0xA8, 0xA8, 0xA8, 
0x50, 0x00, 0x00, 

/* character 0x0058 ('X'): (width=5, offset=616) */
0x00, 0x00, 0x88, 0x88, 0x50, 0x20, 0x50, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x0059 ('Y'): (width=5, offset=627) */
0x00, 0x00, 0x88, 0x88, 0x88, 0x50, 0x20, 0x20, 
0x20, 0x00, 0x00, 

/* character 0x005A ('Z'): (width=5, offset=638) */
0x00, 0x00, 0xF8, 0x08, 0x10, 0x20, 0x40, 0x80, 
0xF8, 0x00, 0x00, 

/* character 0x005B ('['): (width=3, offset=649) */
0x00, 0x00, 0xE0, 0x80, 0x80, 0x80, 0x80, 0x80, 
0xE0, 0x00, 0x00, 

/* character 0x005C ('\'): (width=5, offset=660) */
0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 
0x00, 0x00, 0x00, 

/* character 0x005D (']'): (width=3, offset=671) */
0x00, 0x00, 0xE0, 0x20, 0x20, 0x20, 0x20, 0x20, 
0xE0, 0x00, 0x00, 

/* character 0x005E ('^'): (width=5, offset=682) */
0x00, 0x20, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x005F ('_'): (width=5, offset=693) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xF8, 0x00, 0x00, 

/* character 0x0060 ('`'): (width=2, offset=704) */
0x00, 0x00, 0x80, 0x80, 0x40, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x0061 ('a'): (width=5, offset=715) */
0x00, 0x00, 0x00, 0x00, 0x70, 0x08, 0x78, 0x88, 
0x78, 0x00, 0x00, 

/* character 0x0062 ('b'): (width=5, offset=726) */
0x00, 0x00, 0x80, 0x80, 0xB0, 0xC8, 0x88, 0x88, 
0xF0, 0x00, 0x00, 

/* character 0x0063 ('c'): (width=5, offset=737) */
0x00, 0x00, 0x00, 0x00, 0x70, 0x80, 0x80, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x0064 ('d'): (width=5, offset=748) */
0x00, 0x00, 0x08, 0x08, 0x68, 0x98, 0x88, 0x88, 
0x78, 0x00, 0x00, 

/* character 0x0065 ('e'): (width=5, offset=759) */
0x00, 0x00, 0x00, 0x00, 0x70, 0x88, 0xF8, 0x80, 
0x70, 0x00, 0x00, 

/* character 0x0066 ('f'): (width=5, offset=770) */
0x00, 0x00, 0x30, 0x48, 0x40, 0xF0, 0x40, 0x40, 
0x40, 0x00, 0x00, 

/* character 0x0067 ('g'): (width=5, offset=781) */
0x00, 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 
0x78, 0x08, 0x70, 

/* character 0x0068 ('h'): (width=5, offset=792) */
0x00, 0x00, 0x80, 0x80, 0xB0, 0xC8, 0x88, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x0069 ('i'): (width=3, offset=803) */
0x00, 0x00, 0x40, 0x00, 0xC0, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x006A ('j'): (width=4, offset=814) */
0x00, 0x00, 0x10, 0x00, 0x30, 0x10, 0x10, 0x10, 
0x90, 0x60, 0x00, 

/* character 0x006B ('k'): (width=4, offset=825) */
0x00, 0x00, 0x80, 0x80, 0x90, 0xA0, 0xC0, 0xA0, 
0x90, 0x00, 0x00, 

/* character 0x006C ('l'): (width=3, offset=836) */
0x00, 0x00, 0xC0, 0x40, 0x40, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x006D ('m'): (width=5, offset=847) */
0x00, 0x00, 0x00, 0x00, 0xD0, 0xA8, 0xA8, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x006E ('n'): (width=5, offset=858) */
0x00, 0x00, 0x00, 0x00, 0xB0, 0xC8, 0x88, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x006F ('o'): (width=5, offset=869) */
0x00, 0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x0070 ('p'): (width=5, offset=880) */
0x00, 0x00, 0x00, 0x00, 0xF0, 0x88, 0x88, 0xC8, 
0xB0, 0x80, 0x80, 

/* character 0x0071 ('q'): (width=5, offset=891) */
0x00, 0x00, 0x00, 0x00, 0x78, 0x88, 0x88, 0x98, 
0x68, 0x08, 0x08, 

/* character 0x0072 ('r'): (width=5, offset=902) */
0x00, 0x00, 0x00, 0x00, 0xB0, 0xC8, 0x80, 0x80, 
0x80, 0x00, 0x00, 

/* character 0x0073 ('s'): (width=5, offset=913) */
0x00, 0x00, 0x00, 0x00, 0x70, 0x80, 0x70, 0x08, 
0xF0, 0x00, 0x00, 

/* character 0x0074 ('t'): (width=5, offset=924) */
0x00, 0x00, 0x40, 0xE0, 0x40, 0x40, 0x40, 0x48, 
0x30, 0x00, 0x00, 

/* character 0x0075 ('u'): (width=5, offset=935) */
0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0x98, 
0x68, 0x00, 0x00, 

/* character 0x0076 ('v'): (width=5, offset=946) */
0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0x50, 
0x20, 0x00, 0x00, 

/* character 0x0077 ('w'): (width=5, offset=957) */
0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0xA8, 0xA8, 
0x50, 0x00, 0x00, 

/* character 0x0078 ('x'): (width=5, offset=968) */
0x00, 0x00, 0x00, 0x00, 0x88, 0x50, 0x20, 0x50, 
0x88, 0x00, 0x00, 

/* character 0x0079 ('y'): (width=5, offset=979) */
0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 
0x78, 0x08, 0x70, 

/* character 0x007A ('z'): (width=5, offset=990) */
0x00, 0x00, 0x00, 0x00, 0xF8, 0x10, 0x20, 0x40, 
0xF8, 0x00, 0x00, 

/* character 0x007B ('{'): (width=4, offset=1001) */
0x00, 0x00, 0x30, 0x40, 0x40, 0x80, 0x40, 0x40, 
0x30, 0x00, 0x00, 

/* character 0x007C ('|'): (width=1, offset=1012) */
0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 
0x80, 0x00, 0x00, 

/* character 0x007D ('}'): (width=4, offset=1023) */
0x00, 0x00, 0xC0, 0x20, 0x20, 0x10, 0x20, 0x20, 
0xC0, 0x00, 0x00, 

/* character 0x007E ('~'): (width=5, offset=1034) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0xB0, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x007F (''): (width=2, offset=1045) */
0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 
0x00, 0x00, 0x00, 

/* character 0x0080 ('Ä'): (width=4, offset=1056) */
0x00, 0x00, 0x70, 0x80, 0xE0, 0x80, 0xE0, 0x80, 
0x70, 0x00, 0x00, 

/* character 0x0081 ('Å'): (width=5, offset=1067) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x0082 ('Ç'): (width=4, offset=1078) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x0083 ('É'): (width=4, offset=1089) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x0084 ('Ñ'): (width=4, offset=1100) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x0085 ('Ö'): (width=4, offset=1111) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x0086 ('Ü'): (width=3, offset=1122) */
0x00, 0x00, 0x00, 0x00, 0x40, 0xE0, 0x40, 0x40, 
0x40, 0x00, 0x00, 

/* character 0x0087 ('á'): (width=3, offset=1133) */
0x00, 0x00, 0x00, 0x00, 0x40, 0xE0, 0x40, 0xE0, 
0x40, 0x00, 0x00, 

/* character 0x0088 ('à'): (width=2, offset=1144) */
0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x0089 ('â'): (width=4, offset=1155) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x008A ('ä'): (width=4, offset=1166) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x008B ('ã'): (width=4, offset=1177) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x008C ('å'): (width=4, offset=1188) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x008D ('ç'): (width=5, offset=1199) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x008E ('é'): (width=4, offset=1210) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x008F ('è'): (width=5, offset=1221) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x0090 ('ê'): (width=5, offset=1232) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x0091 ('ë'): (width=4, offset=1243) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x0092 ('í'): (width=4, offset=1254) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x0093 ('ì'): (width=4, offset=1265) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x0094 ('î'): (width=4, offset=1276) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x0095 ('ï'): (width=4, offset=1287) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x0096 ('ñ'): (width=4, offset=1298) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x0097 ('ó'): (width=4, offset=1309) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x0098 ('ò'): (width=2, offset=1320) */
0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x0099 ('ô'): (width=4, offset=1331) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x009A ('ö'): (width=4, offset=1342) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x009B ('õ'): (width=4, offset=1353) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x009C ('ú'): (width=5, offset=1364) */
0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
0x78, 0x00, 0x00, 

/* character 0x009D ('ù'): (width=5, offset=1375) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x009E ('û'): (width=4, offset=1386) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x009F ('ü'): (width=4, offset=1397) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x00A0 ('†'): (width=4, offset=1408) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x00A1 ('°'): (width=1, offset=1419) */
0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x80, 0x80, 
0x80, 0x00, 0x00, 

/* character 0x00A2 ('¢'): (width=5, offset=1430) */
0x00, 0x00, 0x20, 0x70, 0xA0, 0xA0, 0xA8, 0x70, 
0x20, 0x00, 0x00, 

/* character 0x00A3 ('£'): (width=5, offset=1441) */
0x00, 0x00, 0x20, 0x50, 0x40, 0xF0, 0x40, 0x40, 
0x78, 0x00, 0x00, 

/* character 0x00A4 ('§'): (width=5, offset=1452) */
0x00, 0x00, 0x88, 0x70, 0x88, 0x88, 0x88, 0x70, 
0x88, 0x00, 0x00, 

/* character 0x00A5 ('•'): (width=5, offset=1463) */
0x00, 0x00, 0x88, 0x50, 0x20, 0xF8, 0x20, 0xF8, 
0x20, 0x00, 0x00, 

/* character 0x00A6 ('¶'): (width=1, offset=1474) */
0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 
0x80, 0x00, 0x00, 

/* character 0x00A7 ('ß'): (width=5, offset=1485) */
0x00, 0x00, 0x70, 0x80, 0xE0, 0x50, 0x38, 0x08, 
0x70, 0x00, 0x00, 

/* character 0x00A8 ('®'): (width=5, offset=1496) */
0x00, 0x00, 0x00, 0xD8, 0xF8, 0xF8, 0x70, 0x20, 
0x00, 0x00, 0x00, 

/* character 0x00A9 ('©'): (width=5, offset=1507) */
0x00, 0x00, 0x70, 0x88, 0xA8, 0xC8, 0xA8, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00AA ('™'): (width=4, offset=1518) */
0x00, 0x00, 0x40, 0x40, 0xF0, 0x90, 0x90, 0x90, 
0xF0, 0x00, 0x00, 

/* character 0x00AB ('´'): (width=5, offset=1529) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x90, 0x48, 
0x00, 0x00, 0x00, 

/* character 0x00AC ('¨'): (width=5, offset=1540) */
0x00, 0x00, 0x70, 0xA8, 0xF8, 0xA8, 0xF8, 0x50, 
0x50, 0x00, 0x00, 

/* character 0x00AD ('≠'): (width=4, offset=1551) */
0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0xF0, 0x00, 0x00, 

/* character 0x00AE ('Æ'): (width=5, offset=1562) */
0x00, 0x00, 0x70, 0x88, 0xE8, 0xF8, 0xD8, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00AF ('Ø'): (width=5, offset=1573) */
0x00, 0x00, 0x18, 0x20, 0x40, 0x40, 0x80, 0x80, 
0x80, 0x00, 0x00, 

/* character 0x00B0 ('∞'): (width=5, offset=1584) */
0x00, 0x00, 0xC0, 0x20, 0x10, 0x10, 0x08, 0x08, 
0x08, 0x00, 0x00, 

/* character 0x00B1 ('±'): (width=5, offset=1595) */
0x00, 0x00, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x00, 
0xF8, 0x00, 0x00, 

/* character 0x00B2 ('≤'): (width=5, offset=1606) */
0x00, 0x00, 0x10, 0x28, 0x08, 0x10, 0x38, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x00B3 ('≥'): (width=5, offset=1617) */
0x00, 0x00, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x00B4 ('¥'): (width=5, offset=1628) */
0x00, 0x00, 0x00, 0x50, 0x50, 0x00, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00B5 ('µ'): (width=5, offset=1639) */
0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0xF0, 
0x80, 0x80, 0x00, 

/* character 0x00B6 ('∂'): (width=5, offset=1650) */
0x00, 0x00, 0x78, 0xE8, 0xE8, 0x68, 0x28, 0x28, 
0x28, 0x00, 0x00, 

/* character 0x00B7 ('∑'): (width=5, offset=1661) */
0x00, 0x00, 0x00, 0x50, 0x50, 0x00, 0x70, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x00B8 ('∏'): (width=5, offset=1672) */
0x00, 0x00, 0xF8, 0x88, 0x88, 0x88, 0x88, 0x88, 
0xF8, 0x00, 0x00, 

/* character 0x00B9 ('π'): (width=2, offset=1683) */
0x00, 0x00, 0x40, 0xC0, 0x40, 0x40, 0x40, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x00BA ('∫'): (width=5, offset=1694) */
0x00, 0x00, 0x20, 0x50, 0x20, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 

/* character 0x00BB ('ª'): (width=5, offset=1705) */
0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x48, 0x90, 
0x00, 0x00, 0x00, 

/* character 0x00BC ('º'): (width=5, offset=1716) */
0x00, 0x00, 0xF8, 0xA8, 0xA8, 0xA8, 0xF8, 0x20, 
0x20, 0x00, 0x00, 

/* character 0x00BD ('Ω'): (width=5, offset=1727) */
0x00, 0x00, 0x20, 0x50, 0x50, 0x88, 0x88, 0x88, 
0xF8, 0x00, 0x00, 

/* character 0x00BE ('æ'): (width=5, offset=1738) */
0x00, 0x00, 0x20, 0x50, 0x50, 0x88, 0x88, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x00BF ('ø'): (width=5, offset=1749) */
0x00, 0x00, 0x20, 0x00, 0x20, 0x40, 0x80, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00C0 ('¿'): (width=5, offset=1760) */
0x40, 0x20, 0x70, 0x88, 0x88, 0x88, 0xF8, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x00C1 ('¡'): (width=5, offset=1771) */
0x10, 0x20, 0x70, 0x88, 0x88, 0x88, 0xF8, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x00C2 ('¬'): (width=5, offset=1782) */
0x20, 0x50, 0x70, 0x88, 0x88, 0x88, 0xF8, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x00C3 ('√'): (width=5, offset=1793) */
0x00, 0x00, 0x50, 0xA0, 0x70, 0x88, 0xF8, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x00C4 ('ƒ'): (width=5, offset=1804) */
0x50, 0x00, 0x70, 0x88, 0x88, 0x88, 0xF8, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x00C5 ('≈'): (width=5, offset=1815) */
0x70, 0x50, 0x70, 0x70, 0x88, 0x88, 0xF8, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x00C6 ('∆'): (width=5, offset=1826) */
0x00, 0x00, 0x78, 0xA0, 0xA0, 0xF8, 0xA0, 0xA0, 
0xB8, 0x00, 0x00, 

/* character 0x00C7 ('«'): (width=5, offset=1837) */
0x00, 0x00, 0x00, 0x70, 0x88, 0x80, 0x80, 0x88, 
0x70, 0x20, 0x60, 

/* character 0x00C8 ('»'): (width=5, offset=1848) */
0x40, 0x20, 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 
0xF8, 0x00, 0x00, 

/* character 0x00C9 ('…'): (width=5, offset=1859) */
0x10, 0x20, 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 
0xF8, 0x00, 0x00, 

/* character 0x00CA (' '): (width=5, offset=1870) */
0x20, 0x50, 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 
0xF8, 0x00, 0x00, 

/* character 0x00CB ('À'): (width=5, offset=1881) */
0x50, 0x00, 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 
0xF8, 0x00, 0x00, 

/* character 0x00CC ('Ã'): (width=3, offset=1892) */
0x80, 0x40, 0xE0, 0x40, 0x40, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x00CD ('Õ'): (width=3, offset=1903) */
0x20, 0x40, 0xE0, 0x40, 0x40, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x00CE ('Œ'): (width=3, offset=1914) */
0x40, 0xA0, 0xE0, 0x40, 0x40, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x00CF ('œ'): (width=3, offset=1925) */
0xA0, 0x00, 0xE0, 0x40, 0x40, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x00D0 ('–'): (width=5, offset=1936) */
0x00, 0x00, 0x70, 0x48, 0x48, 0xE8, 0x48, 0x48, 
0x70, 0x00, 0x00, 

/* character 0x00D1 ('—'): (width=5, offset=1947) */
0x70, 0x00, 0x88, 0x88, 0xC8, 0xA8, 0x98, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x00D2 ('“'): (width=5, offset=1958) */
0x40, 0x20, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00D3 ('”'): (width=5, offset=1969) */
0x10, 0x20, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00D4 ('‘'): (width=5, offset=1980) */
0x20, 0x50, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00D5 ('’'): (width=5, offset=1991) */
0x50, 0xA0, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00D6 ('÷'): (width=5, offset=2002) */
0x50, 0x00, 0x70, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00D7 ('◊'): (width=5, offset=2013) */
0x00, 0x00, 0x00, 0x88, 0x50, 0x20, 0x50, 0x88, 
0x00, 0x00, 0x00, 

/* character 0x00D8 ('ÿ'): (width=5, offset=2024) */
0x00, 0x00, 0x68, 0x90, 0xA8, 0xA8, 0xA8, 0x48, 
0xB0, 0x00, 0x00, 

/* character 0x00D9 ('Ÿ'): (width=5, offset=2035) */
0x40, 0x20, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00DA ('⁄'): (width=5, offset=2046) */
0x10, 0x20, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00DB ('€'): (width=5, offset=2057) */
0x20, 0x50, 0x00, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00DC ('‹'): (width=5, offset=2068) */
0x50, 0x00, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00DD ('›'): (width=5, offset=2079) */
0x10, 0x20, 0x88, 0x88, 0x88, 0x50, 0x20, 0x20, 
0x20, 0x00, 0x00, 

/* character 0x00DE ('ﬁ'): (width=4, offset=2090) */
0x00, 0x00, 0x80, 0x80, 0xE0, 0x90, 0xE0, 0x80, 
0x80, 0x00, 0x00, 

/* character 0x00DF ('ﬂ'): (width=5, offset=2101) */
0x00, 0x00, 0x00, 0x70, 0x88, 0xF0, 0x88, 0xF0, 
0x80, 0x00, 0x00, 

/* character 0x00E0 ('‡'): (width=5, offset=2112) */
0x00, 0x40, 0x20, 0x00, 0x70, 0x08, 0x78, 0x88, 
0x78, 0x00, 0x00, 

/* character 0x00E1 ('·'): (width=5, offset=2123) */
0x00, 0x10, 0x20, 0x00, 0x70, 0x08, 0x78, 0x88, 
0x78, 0x00, 0x00, 

/* character 0x00E2 ('‚'): (width=5, offset=2134) */
0x00, 0x20, 0x50, 0x00, 0x70, 0x08, 0x78, 0x88, 
0x78, 0x00, 0x00, 

/* character 0x00E3 ('„'): (width=5, offset=2145) */
0x00, 0x50, 0xA0, 0x00, 0x70, 0x08, 0x78, 0x88, 
0x78, 0x00, 0x00, 

/* character 0x00E4 ('‰'): (width=5, offset=2156) */
0x00, 0x00, 0x50, 0x00, 0x70, 0x08, 0x78, 0x88, 
0x78, 0x00, 0x00, 

/* character 0x00E5 ('Â'): (width=5, offset=2167) */
0x70, 0x50, 0x70, 0x00, 0x70, 0x08, 0x78, 0x88, 
0x78, 0x00, 0x00, 

/* character 0x00E6 ('Ê'): (width=5, offset=2178) */
0x00, 0x00, 0x00, 0x00, 0xD0, 0x28, 0xF8, 0xA0, 
0xF8, 0x00, 0x00, 

/* character 0x00E7 ('Á'): (width=5, offset=2189) */
0x00, 0x00, 0x00, 0x00, 0x70, 0x80, 0x80, 0x88, 
0x70, 0x20, 0x60, 

/* character 0x00E8 ('Ë'): (width=5, offset=2200) */
0x00, 0x40, 0x20, 0x00, 0x70, 0x88, 0xF8, 0x80, 
0x70, 0x00, 0x00, 

/* character 0x00E9 ('È'): (width=5, offset=2211) */
0x00, 0x10, 0x20, 0x00, 0x70, 0x88, 0xF8, 0x80, 
0x70, 0x00, 0x00, 

/* character 0x00EA ('Í'): (width=5, offset=2222) */
0x00, 0x20, 0x50, 0x00, 0x70, 0x88, 0xF8, 0x80, 
0x70, 0x00, 0x00, 

/* character 0x00EB ('Î'): (width=5, offset=2233) */
0x00, 0x00, 0x50, 0x00, 0x70, 0x88, 0xF8, 0x80, 
0x70, 0x00, 0x00, 

/* character 0x00EC ('Ï'): (width=3, offset=2244) */
0x00, 0x80, 0x40, 0x00, 0xC0, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x00ED ('Ì'): (width=3, offset=2255) */
0x00, 0x20, 0x40, 0x00, 0xC0, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x00EE ('Ó'): (width=3, offset=2266) */
0x00, 0x40, 0xA0, 0x00, 0xC0, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x00EF ('Ô'): (width=3, offset=2277) */
0x00, 0x00, 0xA0, 0x00, 0xC0, 0x40, 0x40, 0x40, 
0xE0, 0x00, 0x00, 

/* character 0x00F0 (''): (width=5, offset=2288) */
0x00, 0x00, 0x10, 0x38, 0x10, 0x50, 0xB0, 0x90, 
0x70, 0x00, 0x00, 

/* character 0x00F1 ('Ò'): (width=5, offset=2299) */
0x00, 0x00, 0x70, 0x00, 0xB0, 0xC8, 0x88, 0x88, 
0x88, 0x00, 0x00, 

/* character 0x00F2 ('Ú'): (width=5, offset=2310) */
0x00, 0x40, 0x20, 0x00, 0x70, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00F3 ('Û'): (width=5, offset=2321) */
0x00, 0x10, 0x20, 0x00, 0x70, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00F4 ('Ù'): (width=5, offset=2332) */
0x00, 0x20, 0x50, 0x00, 0x70, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00F5 ('ı'): (width=5, offset=2343) */
0x00, 0x50, 0xA0, 0x00, 0x70, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00F6 ('ˆ'): (width=5, offset=2354) */
0x00, 0x00, 0x50, 0x00, 0x70, 0x88, 0x88, 0x88, 
0x70, 0x00, 0x00, 

/* character 0x00F7 ('˜'): (width=5, offset=2365) */
0x00, 0x00, 0x00, 0x20, 0x00, 0xF8, 0x00, 0x20, 
0x00, 0x00, 0x00, 

/* character 0x00F8 ('¯'): (width=5, offset=2376) */
0x00, 0x00, 0x00, 0x00, 0x68, 0x90, 0xA8, 0x48, 
0xB0, 0x00, 0x00, 

/* character 0x00F9 ('˘'): (width=5, offset=2387) */
0x00, 0x00, 0x40, 0x20, 0x88, 0x88, 0x88, 0x98, 
0x68, 0x00, 0x00, 

/* character 0x00FA ('˙'): (width=5, offset=2398) */
0x00, 0x00, 0x10, 0x20, 0x88, 0x88, 0x88, 0x98, 
0x68, 0x00, 0x00, 

/* character 0x00FB ('˚'): (width=5, offset=2409) */
0x00, 0x20, 0x50, 0x00, 0x88, 0x88, 0x88, 0x98, 
0x68, 0x00, 0x00, 

/* character 0x00FC ('¸'): (width=5, offset=2420) */
0x00, 0x00, 0x50, 0x00, 0x88, 0x88, 0x88, 0x98, 
0x68, 0x00, 0x00, 

/* character 0x00FD ('˝'): (width=5, offset=2431) */
0x00, 0x10, 0x20, 0x00, 0x88, 0x88, 0x88, 0x88, 
0x78, 0x08, 0x70, 

/* character 0x00FE ('˛'): (width=4, offset=2442) */
0x00, 0x00, 0x80, 0xE0, 0x90, 0x90, 0x90, 0xE0, 
0x80, 0x00, 0x00, 

/* character 0x00FF ('ˇ'): (width=5, offset=2453) */
0x00, 0x00, 0x50, 0x00, 0x88, 0x88, 0x88, 0x88, 
0x78, 0x08, 0x70, 

};

/*******************************************************************************************
  Offset table provides the starting offset of each character in the data table.

  To get the starting offset of character 'A', you can use the following expression:

     FONT_CONSTANT_BYTE index = index_table['A'];
     FONT_CONSTANT_WORD offset = offset_table[index]

 *******************************************************************************************/
FONT_CONSTANT_WORD offset_table[] = {
/*		offset    char    hexcode */
/*		======    ====    ======= */
  		     0, /*          0020    */
  		    11, /*   !      0021    */
  		    22, /*   "      0022    */
  		    33, /*   #      0023    */
  		    44, /*   $      0024    */
  		    55, /*   %      0025    */
  		    66, /*   &      0026    */
  		    77, /*   '      0027    */
  		    88, /*   (      0028    */
  		    99, /*   )      0029    */
  		   110, /*   *      002A    */
  		   121, /*   +      002B    */
  		   132, /*   ,      002C    */
  		   143, /*   -      002D    */
  		   154, /*   .      002E    */
  		   165, /*   /      002F    */
  		   176, /*   0      0030    */
  		   187, /*   1      0031    */
  		   198, /*   2      0032    */
  		   209, /*   3      0033    */
  		   220, /*   4      0034    */
  		   231, /*   5      0035    */
  		   242, /*   6      0036    */
  		   253, /*   7      0037    */
  		   264, /*   8      0038    */
  		   275, /*   9      0039    */
  		   286, /*   :      003A    */
  		   297, /*   ;      003B    */
  		   308, /*   <      003C    */
  		   319, /*   =      003D    */
  		   330, /*   >      003E    */
  		   341, /*   ?      003F    */
  		   352, /*   @      0040    */
  		   363, /*   A      0041    */
  		   374, /*   B      0042    */
  		   385, /*   C      0043    */
  		   396, /*   D      0044    */
  		   407, /*   E      0045    */
  		   418, /*   F      0046    */
  		   429, /*   G      0047    */
  		   440, /*   H      0048    */
  		   451, /*   I      0049    */
  		   462, /*   J      004A    */
  		   473, /*   K      004B    */
  		   484, /*   L      004C    */
  		   495, /*   M      004D    */
  		   506, /*   N      004E    */
  		   517, /*   O      004F    */
  		   528, /*   P      0050    */
  		   539, /*   Q      0051    */
  		   550, /*   R      0052    */
  		   561, /*   S      0053    */
  		   572, /*   T      0054    */
  		   583, /*   U      0055    */
  		   594, /*   V      0056    */
  		   605, /*   W      0057    */
  		   616, /*   X      0058    */
  		   627, /*   Y      0059    */
  		   638, /*   Z      005A    */
  		   649, /*   [      005B    */
  		   660, /*   \      005C    */
  		   671, /*   ]      005D    */
  		   682, /*   ^      005E    */
  		   693, /*   _      005F    */
  		   704, /*   `      0060    */
  		   715, /*   a      0061    */
  		   726, /*   b      0062    */
  		   737, /*   c      0063    */
  		   748, /*   d      0064    */
  		   759, /*   e      0065    */
  		   770, /*   f      0066    */
  		   781, /*   g      0067    */
  		   792, /*   h      0068    */
  		   803, /*   i      0069    */
  		   814, /*   j      006A    */
  		   825, /*   k      006B    */
  		   836, /*   l      006C    */
  		   847, /*   m      006D    */
  		   858, /*   n      006E    */
  		   869, /*   o      006F    */
  		   880, /*   p      0070    */
  		   891, /*   q      0071    */
  		   902, /*   r      0072    */
  		   913, /*   s      0073    */
  		   924, /*   t      0074    */
  		   935, /*   u      0075    */
  		   946, /*   v      0076    */
  		   957, /*   w      0077    */
  		   968, /*   x      0078    */
  		   979, /*   y      0079    */
  		   990, /*   z      007A    */
  		  1001, /*   {      007B    */
  		  1012, /*   |      007C    */
  		  1023, /*   }      007D    */
  		  1034, /*   ~      007E    */
  		  1045, /*         007F    */
  		  1056, /*   Ä      0080    */
  		  1067, /*   Å      0081    */
  		  1078, /*   Ç      0082    */
  		  1089, /*   É      0083    */
  		  1100, /*   Ñ      0084    */
  		  1111, /*   Ö      0085    */
  		  1122, /*   Ü      0086    */
  		  1133, /*   á      0087    */
  		  1144, /*   à      0088    */
  		  1155, /*   â      0089    */
  		  1166, /*   ä      008A    */
  		  1177, /*   ã      008B    */
  		  1188, /*   å      008C    */
  		  1199, /*   ç      008D    */
  		  1210, /*   é      008E    */
  		  1221, /*   è      008F    */
  		  1232, /*   ê      0090    */
  		  1243, /*   ë      0091    */
  		  1254, /*   í      0092    */
  		  1265, /*   ì      0093    */
  		  1276, /*   î      0094    */
  		  1287, /*   ï      0095    */
  		  1298, /*   ñ      0096    */
  		  1309, /*   ó      0097    */
  		  1320, /*   ò      0098    */
  		  1331, /*   ô      0099    */
  		  1342, /*   ö      009A    */
  		  1353, /*   õ      009B    */
  		  1364, /*   ú      009C    */
  		  1375, /*   ù      009D    */
  		  1386, /*   û      009E    */
  		  1397, /*   ü      009F    */
  		  1408, /*   †      00A0    */
  		  1419, /*   °      00A1    */
  		  1430, /*   ¢      00A2    */
  		  1441, /*   £      00A3    */
  		  1452, /*   §      00A4    */
  		  1463, /*   •      00A5    */
  		  1474, /*   ¶      00A6    */
  		  1485, /*   ß      00A7    */
  		  1496, /*   ®      00A8    */
  		  1507, /*   ©      00A9    */
  		  1518, /*   ™      00AA    */
  		  1529, /*   ´      00AB    */
  		  1540, /*   ¨      00AC    */
  		  1551, /*   ≠      00AD    */
  		  1562, /*   Æ      00AE    */
  		  1573, /*   Ø      00AF    */
  		  1584, /*   ∞      00B0    */
  		  1595, /*   ±      00B1    */
  		  1606, /*   ≤      00B2    */
  		  1617, /*   ≥      00B3    */
  		  1628, /*   ¥      00B4    */
  		  1639, /*   µ      00B5    */
  		  1650, /*   ∂      00B6    */
  		  1661, /*   ∑      00B7    */
  		  1672, /*   ∏      00B8    */
  		  1683, /*   π      00B9    */
  		  1694, /*   ∫      00BA    */
  		  1705, /*   ª      00BB    */
  		  1716, /*   º      00BC    */
  		  1727, /*   Ω      00BD    */
  		  1738, /*   æ      00BE    */
  		  1749, /*   ø      00BF    */
  		  1760, /*   ¿      00C0    */
  		  1771, /*   ¡      00C1    */
  		  1782, /*   ¬      00C2    */
  		  1793, /*   √      00C3    */
  		  1804, /*   ƒ      00C4    */
  		  1815, /*   ≈      00C5    */
  		  1826, /*   ∆      00C6    */
  		  1837, /*   «      00C7    */
  		  1848, /*   »      00C8    */
  		  1859, /*   …      00C9    */
  		  1870, /*          00CA    */
  		  1881, /*   À      00CB    */
  		  1892, /*   Ã      00CC    */
  		  1903, /*   Õ      00CD    */
  		  1914, /*   Œ      00CE    */
  		  1925, /*   œ      00CF    */
  		  1936, /*   –      00D0    */
  		  1947, /*   —      00D1    */
  		  1958, /*   “      00D2    */
  		  1969, /*   ”      00D3    */
  		  1980, /*   ‘      00D4    */
  		  1991, /*   ’      00D5    */
  		  2002, /*   ÷      00D6    */
  		  2013, /*   ◊      00D7    */
  		  2024, /*   ÿ      00D8    */
  		  2035, /*   Ÿ      00D9    */
  		  2046, /*   ⁄      00DA    */
  		  2057, /*   €      00DB    */
  		  2068, /*   ‹      00DC    */
  		  2079, /*   ›      00DD    */
  		  2090, /*   ﬁ      00DE    */
  		  2101, /*   ﬂ      00DF    */
  		  2112, /*   ‡      00E0    */
  		  2123, /*   ·      00E1    */
  		  2134, /*   ‚      00E2    */
  		  2145, /*   „      00E3    */
  		  2156, /*   ‰      00E4    */
  		  2167, /*   Â      00E5    */
  		  2178, /*   Ê      00E6    */
  		  2189, /*   Á      00E7    */
  		  2200, /*   Ë      00E8    */
  		  2211, /*   È      00E9    */
  		  2222, /*   Í      00EA    */
  		  2233, /*   Î      00EB    */
  		  2244, /*   Ï      00EC    */
  		  2255, /*   Ì      00ED    */
  		  2266, /*   Ó      00EE    */
  		  2277, /*   Ô      00EF    */
  		  2288, /*         00F0    */
  		  2299, /*   Ò      00F1    */
  		  2310, /*   Ú      00F2    */
  		  2321, /*   Û      00F3    */
  		  2332, /*   Ù      00F4    */
  		  2343, /*   ı      00F5    */
  		  2354, /*   ˆ      00F6    */
  		  2365, /*   ˜      00F7    */
  		  2376, /*   ¯      00F8    */
  		  2387, /*   ˘      00F9    */
  		  2398, /*   ˙      00FA    */
  		  2409, /*   ˚      00FB    */
  		  2420, /*   ¸      00FC    */
  		  2431, /*   ˝      00FD    */
  		  2442, /*   ˛      00FE    */
  		  2453, /*   ˇ      00FF    */
};

/*******************************************************************************************
  Index table is used to find the mapping index of a character.

  If you can find a simple mathematical expression for index mapping, you can use that.
  If you do not have such a mathematical expression, this index table is just for you.

  To get the index of character 'A', you can use the following expression:

     FONT_CONSTANT_BYTE index = index_table['A'];

 *******************************************************************************************/
FONT_CONSTANT_BYTE index_table[] = {
/*		index   hexcode   decimal  char */
/*		=====   =======   =======  ==== */
  		  0, /*   00          0     .   */
  		  0, /*   01          1     .   */
  		  0, /*   02          2     .   */
  		  0, /*   03          3     .   */
  		  0, /*   04          4     .   */
  		  0, /*   05          5     .   */
  		  0, /*   06          6     .   */
  		  0, /*   07          7     .   */
  		  0, /*   08          8     .   */
  		  0, /*   09          9     .   */
  		  0, /*   0A         10     .   */
  		  0, /*   0B         11     .   */
  		  0, /*   0C         12     .   */
  		  0, /*   0D         13     .   */
  		  0, /*   0E         14     .   */
  		  0, /*   0F         15     .   */
  		  0, /*   10         16     .   */
  		  0, /*   11         17     .   */
  		  0, /*   12         18     .   */
  		  0, /*   13         19     .   */
  		  0, /*   14         20     .   */
  		  0, /*   15         21     .   */
  		  0, /*   16         22     .   */
  		  0, /*   17         23     .   */
  		  0, /*   18         24     .   */
  		  0, /*   19         25     .   */
  		  0, /*   1A         26     .   */
  		  0, /*   1B         27     .   */
  		  0, /*   1C         28     .   */
  		  0, /*   1D         29     .   */
  		  0, /*   1E         30     .   */
  		  0, /*   1F         31     .   */
  		  0, /*   20         32         */
  		  1, /*   21         33     !   */
  		  2, /*   22         34     "   */
  		  3, /*   23         35     #   */
  		  4, /*   24         36     $   */
  		  5, /*   25         37     %   */
  		  6, /*   26         38     &   */
  		  7, /*   27         39     '   */
  		  8, /*   28         40     (   */
  		  9, /*   29         41     )   */
  		 10, /*   2A         42     *   */
  		 11, /*   2B         43     +   */
  		 12, /*   2C         44     ,   */
  		 13, /*   2D         45     -   */
  		 14, /*   2E         46     .   */
  		 15, /*   2F         47     /   */
  		 16, /*   30         48     0   */
  		 17, /*   31         49     1   */
  		 18, /*   32         50     2   */
  		 19, /*   33         51     3   */
  		 20, /*   34         52     4   */
  		 21, /*   35         53     5   */
  		 22, /*   36         54     6   */
  		 23, /*   37         55     7   */
  		 24, /*   38         56     8   */
  		 25, /*   39         57     9   */
  		 26, /*   3A         58     :   */
  		 27, /*   3B         59     ;   */
  		 28, /*   3C         60     <   */
  		 29, /*   3D         61     =   */
  		 30, /*   3E         62     >   */
  		 31, /*   3F         63     ?   */
  		 32, /*   40         64     @   */
  		 33, /*   41         65     A   */
  		 34, /*   42         66     B   */
  		 35, /*   43         67     C   */
  		 36, /*   44         68     D   */
  		 37, /*   45         69     E   */
  		 38, /*   46         70     F   */
  		 39, /*   47         71     G   */
  		 40, /*   48         72     H   */
  		 41, /*   49         73     I   */
  		 42, /*   4A         74     J   */
  		 43, /*   4B         75     K   */
  		 44, /*   4C         76     L   */
  		 45, /*   4D         77     M   */
  		 46, /*   4E         78     N   */
  		 47, /*   4F         79     O   */
  		 48, /*   50         80     P   */
  		 49, /*   51         81     Q   */
  		 50, /*   52         82     R   */
  		 51, /*   53         83     S   */
  		 52, /*   54         84     T   */
  		 53, /*   55         85     U   */
  		 54, /*   56         86     V   */
  		 55, /*   57         87     W   */
  		 56, /*   58         88     X   */
  		 57, /*   59         89     Y   */
  		 58, /*   5A         90     Z   */
  		 59, /*   5B         91     [   */
  		 60, /*   5C         92     \   */
  		 61, /*   5D         93     ]   */
  		 62, /*   5E         94     ^   */
  		 63, /*   5F         95     _   */
  		 64, /*   60         96     `   */
  		 65, /*   61         97     a   */
  		 66, /*   62         98     b   */
  		 67, /*   63         99     c   */
  		 68, /*   64        100     d   */
  		 69, /*   65        101     e   */
  		 70, /*   66        102     f   */
  		 71, /*   67        103     g   */
  		 72, /*   68        104     h   */
  		 73, /*   69        105     i   */
  		 74, /*   6A        106     j   */
  		 75, /*   6B        107     k   */
  		 76, /*   6C        108     l   */
  		 77, /*   6D        109     m   */
  		 78, /*   6E        110     n   */
  		 79, /*   6F        111     o   */
  		 80, /*   70        112     p   */
  		 81, /*   71        113     q   */
  		 82, /*   72        114     r   */
  		 83, /*   73        115     s   */
  		 84, /*   74        116     t   */
  		 85, /*   75        117     u   */
  		 86, /*   76        118     v   */
  		 87, /*   77        119     w   */
  		 88, /*   78        120     x   */
  		 89, /*   79        121     y   */
  		 90, /*   7A        122     z   */
  		 91, /*   7B        123     {   */
  		 92, /*   7C        124     |   */
  		 93, /*   7D        125     }   */
  		 94, /*   7E        126     ~   */
  		 95, /*   7F        127        */
  		 96, /*   80        128     Ä   */
  		 97, /*   81        129     Å   */
  		 98, /*   82        130     Ç   */
  		 99, /*   83        131     É   */
  		100, /*   84        132     Ñ   */
  		101, /*   85        133     Ö   */
  		102, /*   86        134     Ü   */
  		103, /*   87        135     á   */
  		104, /*   88        136     à   */
  		105, /*   89        137     â   */
  		106, /*   8A        138     ä   */
  		107, /*   8B        139     ã   */
  		108, /*   8C        140     å   */
  		109, /*   8D        141     ç   */
  		110, /*   8E        142     é   */
  		111, /*   8F        143     è   */
  		112, /*   90        144     ê   */
  		113, /*   91        145     ë   */
  		114, /*   92        146     í   */
  		115, /*   93        147     ì   */
  		116, /*   94        148     î   */
  		117, /*   95        149     ï   */
  		118, /*   96        150     ñ   */
  		119, /*   97        151     ó   */
  		120, /*   98        152     ò   */
  		121, /*   99        153     ô   */
  		122, /*   9A        154     ö   */
  		123, /*   9B        155     õ   */
  		124, /*   9C        156     ú   */
  		125, /*   9D        157     ù   */
  		126, /*   9E        158     û   */
  		127, /*   9F        159     ü   */
  		128, /*   A0        160     †   */
  		129, /*   A1        161     °   */
  		130, /*   A2        162     ¢   */
  		131, /*   A3        163     £   */
  		132, /*   A4        164     §   */
  		133, /*   A5        165     •   */
  		134, /*   A6        166     ¶   */
  		135, /*   A7        167     ß   */
  		136, /*   A8        168     ®   */
  		137, /*   A9        169     ©   */
  		138, /*   AA        170     ™   */
  		139, /*   AB        171     ´   */
  		140, /*   AC        172     ¨   */
  		141, /*   AD        173     ≠   */
  		142, /*   AE        174     Æ   */
  		143, /*   AF        175     Ø   */
  		144, /*   B0        176     ∞   */
  		145, /*   B1        177     ±   */
  		146, /*   B2        178     ≤   */
  		147, /*   B3        179     ≥   */
  		148, /*   B4        180     ¥   */
  		149, /*   B5        181     µ   */
  		150, /*   B6        182     ∂   */
  		151, /*   B7        183     ∑   */
  		152, /*   B8        184     ∏   */
  		153, /*   B9        185     π   */
  		154, /*   BA        186     ∫   */
  		155, /*   BB        187     ª   */
  		156, /*   BC        188     º   */
  		157, /*   BD        189     Ω   */
  		158, /*   BE        190     æ   */
  		159, /*   BF        191     ø   */
  		160, /*   C0        192     ¿   */
  		161, /*   C1        193     ¡   */
  		162, /*   C2        194     ¬   */
  		163, /*   C3        195     √   */
  		164, /*   C4        196     ƒ   */
  		165, /*   C5        197     ≈   */
  		166, /*   C6        198     ∆   */
  		167, /*   C7        199     «   */
  		168, /*   C8        200     »   */
  		169, /*   C9        201     …   */
  		170, /*   CA        202         */
  		171, /*   CB        203     À   */
  		172, /*   CC        204     Ã   */
  		173, /*   CD        205     Õ   */
  		174, /*   CE        206     Œ   */
  		175, /*   CF        207     œ   */
  		176, /*   D0        208     –   */
  		177, /*   D1        209     —   */
  		178, /*   D2        210     “   */
  		179, /*   D3        211     ”   */
  		180, /*   D4        212     ‘   */
  		181, /*   D5        213     ’   */
  		182, /*   D6        214     ÷   */
  		183, /*   D7        215     ◊   */
  		184, /*   D8        216     ÿ   */
  		185, /*   D9        217     Ÿ   */
  		186, /*   DA        218     ⁄   */
  		187, /*   DB        219     €   */
  		188, /*   DC        220     ‹   */
  		189, /*   DD        221     ›   */
  		190, /*   DE        222     ﬁ   */
  		191, /*   DF        223     ﬂ   */
  		192, /*   E0        224     ‡   */
  		193, /*   E1        225     ·   */
  		194, /*   E2        226     ‚   */
  		195, /*   E3        227     „   */
  		196, /*   E4        228     ‰   */
  		197, /*   E5        229     Â   */
  		198, /*   E6        230     Ê   */
  		199, /*   E7        231     Á   */
  		200, /*   E8        232     Ë   */
  		201, /*   E9        233     È   */
  		202, /*   EA        234     Í   */
  		203, /*   EB        235     Î   */
  		204, /*   EC        236     Ï   */
  		205, /*   ED        237     Ì   */
  		206, /*   EE        238     Ó   */
  		207, /*   EF        239     Ô   */
  		208, /*   F0        240        */
  		209, /*   F1        241     Ò   */
  		210, /*   F2        242     Ú   */
  		211, /*   F3        243     Û   */
  		212, /*   F4        244     Ù   */
  		213, /*   F5        245     ı   */
  		214, /*   F6        246     ˆ   */
  		215, /*   F7        247     ˜   */
  		216, /*   F8        248     ¯   */
  		217, /*   F9        249     ˘   */
  		218, /*   FA        250     ˙   */
  		219, /*   FB        251     ˚   */
  		220, /*   FC        252     ¸   */
  		221, /*   FD        253     ˝   */
  		222, /*   FE        254     ˛   */
  		223, /*   FF        255     ˇ   */
};

/*******************************************************************************************
  Width table provides the width of each character. It's useful for proportional font only.
  For monospaced font, the width of each character is the same.

  To get the width of character 'A', you can use the following expression:

     FONT_CONSTANT_BYTE index = index_table['A'];
     FONT_CONSTANT_BYTE width = width_table[index];

 *******************************************************************************************/
FONT_CONSTANT_BYTE width_table[] = {
/*		width    char    hexcode */
/*		=====    ====    ======= */
  		  5, /*          0020    */
  		  1, /*   !      0021    */
  		  3, /*   "      0022    */
  		  5, /*   #      0023    */
  		  5, /*   $      0024    */
  		  5, /*   %      0025    */
  		  5, /*   &      0026    */
  		  2, /*   '      0027    */
  		  3, /*   (      0028    */
  		  3, /*   )      0029    */
  		  5, /*   *      002A    */
  		  5, /*   +      002B    */
  		  2, /*   ,      002C    */
  		  5, /*   -      002D    */
  		  2, /*   .      002E    */
  		  5, /*   /      002F    */
  		  5, /*   0      0030    */
  		  3, /*   1      0031    */
  		  5, /*   2      0032    */
  		  5, /*   3      0033    */
  		  5, /*   4      0034    */
  		  5, /*   5      0035    */
  		  5, /*   6      0036    */
  		  5, /*   7      0037    */
  		  5, /*   8      0038    */
  		  5, /*   9      0039    */
  		  2, /*   :      003A    */
  		  2, /*   ;      003B    */
  		  4, /*   <      003C    */
  		  5, /*   =      003D    */
  		  4, /*   >      003E    */
  		  5, /*   ?      003F    */
  		  5, /*   @      0040    */
  		  5, /*   A      0041    */
  		  5, /*   B      0042    */
  		  5, /*   C      0043    */
  		  5, /*   D      0044    */
  		  5, /*   E      0045    */
  		  5, /*   F      0046    */
  		  5, /*   G      0047    */
  		  5, /*   H      0048    */
  		  3, /*   I      0049    */
  		  5, /*   J      004A    */
  		  5, /*   K      004B    */
  		  5, /*   L      004C    */
  		  5, /*   M      004D    */
  		  5, /*   N      004E    */
  		  5, /*   O      004F    */
  		  5, /*   P      0050    */
  		  5, /*   Q      0051    */
  		  5, /*   R      0052    */
  		  5, /*   S      0053    */
  		  5, /*   T      0054    */
  		  5, /*   U      0055    */
  		  5, /*   V      0056    */
  		  5, /*   W      0057    */
  		  5, /*   X      0058    */
  		  5, /*   Y      0059    */
  		  5, /*   Z      005A    */
  		  3, /*   [      005B    */
  		  5, /*   \      005C    */
  		  3, /*   ]      005D    */
  		  5, /*   ^      005E    */
  		  5, /*   _      005F    */
  		  2, /*   `      0060    */
  		  5, /*   a      0061    */
  		  5, /*   b      0062    */
  		  5, /*   c      0063    */
  		  5, /*   d      0064    */
  		  5, /*   e      0065    */
  		  5, /*   f      0066    */
  		  5, /*   g      0067    */
  		  5, /*   h      0068    */
  		  3, /*   i      0069    */
  		  4, /*   j      006A    */
  		  4, /*   k      006B    */
  		  3, /*   l      006C    */
  		  5, /*   m      006D    */
  		  5, /*   n      006E    */
  		  5, /*   o      006F    */
  		  5, /*   p      0070    */
  		  5, /*   q      0071    */
  		  5, /*   r      0072    */
  		  5, /*   s      0073    */
  		  5, /*   t      0074    */
  		  5, /*   u      0075    */
  		  5, /*   v      0076    */
  		  5, /*   w      0077    */
  		  5, /*   x      0078    */
  		  5, /*   y      0079    */
  		  5, /*   z      007A    */
  		  4, /*   {      007B    */
  		  1, /*   |      007C    */
  		  4, /*   }      007D    */
  		  5, /*   ~      007E    */
  		  2, /*         007F    */
  		  4, /*   Ä      0080    */
  		  5, /*   Å      0081    */
  		  4, /*   Ç      0082    */
  		  4, /*   É      0083    */
  		  4, /*   Ñ      0084    */
  		  4, /*   Ö      0085    */
  		  3, /*   Ü      0086    */
  		  3, /*   á      0087    */
  		  2, /*   à      0088    */
  		  4, /*   â      0089    */
  		  4, /*   ä      008A    */
  		  4, /*   ã      008B    */
  		  4, /*   å      008C    */
  		  5, /*   ç      008D    */
  		  4, /*   é      008E    */
  		  5, /*   è      008F    */
  		  5, /*   ê      0090    */
  		  4, /*   ë      0091    */
  		  4, /*   í      0092    */
  		  4, /*   ì      0093    */
  		  4, /*   î      0094    */
  		  4, /*   ï      0095    */
  		  4, /*   ñ      0096    */
  		  4, /*   ó      0097    */
  		  2, /*   ò      0098    */
  		  4, /*   ô      0099    */
  		  4, /*   ö      009A    */
  		  4, /*   õ      009B    */
  		  5, /*   ú      009C    */
  		  5, /*   ù      009D    */
  		  4, /*   û      009E    */
  		  4, /*   ü      009F    */
  		  4, /*   †      00A0    */
  		  1, /*   °      00A1    */
  		  5, /*   ¢      00A2    */
  		  5, /*   £      00A3    */
  		  5, /*   §      00A4    */
  		  5, /*   •      00A5    */
  		  1, /*   ¶      00A6    */
  		  5, /*   ß      00A7    */
  		  5, /*   ®      00A8    */
  		  5, /*   ©      00A9    */
  		  4, /*   ™      00AA    */
  		  5, /*   ´      00AB    */
  		  5, /*   ¨      00AC    */
  		  4, /*   ≠      00AD    */
  		  5, /*   Æ      00AE    */
  		  5, /*   Ø      00AF    */
  		  5, /*   ∞      00B0    */
  		  5, /*   ±      00B1    */
  		  5, /*   ≤      00B2    */
  		  5, /*   ≥      00B3    */
  		  5, /*   ¥      00B4    */
  		  5, /*   µ      00B5    */
  		  5, /*   ∂      00B6    */
  		  5, /*   ∑      00B7    */
  		  5, /*   ∏      00B8    */
  		  2, /*   π      00B9    */
  		  5, /*   ∫      00BA    */
  		  5, /*   ª      00BB    */
  		  5, /*   º      00BC    */
  		  5, /*   Ω      00BD    */
  		  5, /*   æ      00BE    */
  		  5, /*   ø      00BF    */
  		  5, /*   ¿      00C0    */
  		  5, /*   ¡      00C1    */
  		  5, /*   ¬      00C2    */
  		  5, /*   √      00C3    */
  		  5, /*   ƒ      00C4    */
  		  5, /*   ≈      00C5    */
  		  5, /*   ∆      00C6    */
  		  5, /*   «      00C7    */
  		  5, /*   »      00C8    */
  		  5, /*   …      00C9    */
  		  5, /*          00CA    */
  		  5, /*   À      00CB    */
  		  3, /*   Ã      00CC    */
  		  3, /*   Õ      00CD    */
  		  3, /*   Œ      00CE    */
  		  3, /*   œ      00CF    */
  		  5, /*   –      00D0    */
  		  5, /*   —      00D1    */
  		  5, /*   “      00D2    */
  		  5, /*   ”      00D3    */
  		  5, /*   ‘      00D4    */
  		  5, /*   ’      00D5    */
  		  5, /*   ÷      00D6    */
  		  5, /*   ◊      00D7    */
  		  5, /*   ÿ      00D8    */
  		  5, /*   Ÿ      00D9    */
  		  5, /*   ⁄      00DA    */
  		  5, /*   €      00DB    */
  		  5, /*   ‹      00DC    */
  		  5, /*   ›      00DD    */
  		  4, /*   ﬁ      00DE    */
  		  5, /*   ﬂ      00DF    */
  		  5, /*   ‡      00E0    */
  		  5, /*   ·      00E1    */
  		  5, /*   ‚      00E2    */
  		  5, /*   „      00E3    */
  		  5, /*   ‰      00E4    */
  		  5, /*   Â      00E5    */
  		  5, /*   Ê      00E6    */
  		  5, /*   Á      00E7    */
  		  5, /*   Ë      00E8    */
  		  5, /*   È      00E9    */
  		  5, /*   Í      00EA    */
  		  5, /*   Î      00EB    */
  		  3, /*   Ï      00EC    */
  		  3, /*   Ì      00ED    */
  		  3, /*   Ó      00EE    */
  		  3, /*   Ô      00EF    */
  		  5, /*         00F0    */
  		  5, /*   Ò      00F1    */
  		  5, /*   Ú      00F2    */
  		  5, /*   Û      00F3    */
  		  5, /*   Ù      00F4    */
  		  5, /*   ı      00F5    */
  		  5, /*   ˆ      00F6    */
  		  5, /*   ˜      00F7    */
  		  5, /*   ¯      00F8    */
  		  5, /*   ˘      00F9    */
  		  5, /*   ˙      00FA    */
  		  5, /*   ˚      00FB    */
  		  5, /*   ¸      00FC    */
  		  5, /*   ˝      00FD    */
  		  4, /*   ˛      00FE    */
  		  5, /*   ˇ      00FF    */
};










//<<<< END OF THE BIT FONT CREATOR PRO C FILE CONTENTS >>>>
#endif //#ifdef DISPLAY_C

