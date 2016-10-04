

	//DO DISPLAY IO PIN TEST
/*
#include "display-model.h"
	BYTE display_test_state = 0;
	DISPLAY_DATA_BUS_TRIS(0x00);
	while (1)
	{
		ClearWDT();
		switch (display_test_state++)
		{
		case 0:
			DISPLAY_RW(1);
			DISPLAY_RD(0);
			DISPLAY_A0(0);
			DISPLAY_CS1(0);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x00);
			break;
		case 1:
			DISPLAY_RW(0);
			DISPLAY_RD(1);
			DISPLAY_A0(0);
			DISPLAY_CS1(0);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x00);
			break;
		case 2:
			DISPLAY_RW(0);
			DISPLAY_RD(0);
			DISPLAY_A0(1);
			DISPLAY_CS1(0);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x00);
			break;
		case 3:
			DISPLAY_RW(0);
			DISPLAY_RD(0);
			DISPLAY_A0(0);
			DISPLAY_CS1(1);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x00);
			break;
		case 4:
			DISPLAY_RW(0);
			DISPLAY_RD(0);
			DISPLAY_A0(0);
			DISPLAY_CS1(0);
			DISPLAY_RESET(1);
			DISPLAY_DATA_BUS_OP(0x00);
			break;
		case 5:
			DISPLAY_RW(0);
			DISPLAY_RD(0);
			DISPLAY_A0(0);
			DISPLAY_CS1(0);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x01);
			break;
		case 6:
			DISPLAY_RW(0);
			DISPLAY_RD(0);
			DISPLAY_A0(0);
			DISPLAY_CS1(0);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x02);
			break;
		case 7:
			DISPLAY_RW(0);
			DISPLAY_RD(0);
			DISPLAY_A0(0);
			DISPLAY_CS1(0);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x04);
			break;
		case 8:
			DISPLAY_RW(0);
			DISPLAY_RD(0);
			DISPLAY_A0(0);
			DISPLAY_CS1(0);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x08);
			break;
		case 9:
			DISPLAY_RW(0);
			DISPLAY_RD(0);
			DISPLAY_A0(0);
			DISPLAY_CS1(0);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x10);
			break;
		case 10:
			DISPLAY_RW(0);
			DISPLAY_RD(0);
			DISPLAY_A0(0);
			DISPLAY_CS1(0);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x20);
			break;
		case 11:
			DISPLAY_RW(0);
			DISPLAY_RD(0);
			DISPLAY_A0(0);
			DISPLAY_CS1(0);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x40);
			break;
		case 12:
			DISPLAY_RW(0);
			DISPLAY_RD(0);
			DISPLAY_A0(0);
			DISPLAY_CS1(0);
			DISPLAY_RESET(0);
			DISPLAY_DATA_BUS_OP(0x80);
			break;
		default:
			display_test_state = 0;
			break;

		}
		display_delay_ms(1);
	}
*/
