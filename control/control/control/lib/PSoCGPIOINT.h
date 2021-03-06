/******************************************************************************
*  Generated by PSoC Designer 5.4.2946
******************************************************************************/
#include <m8c.h>
// AnalogColumn_InputMUX_1 address and mask defines
#pragma	ioport	AnalogColumn_InputMUX_1_Data_ADDR:	0x0
BYTE			AnalogColumn_InputMUX_1_Data_ADDR;
#pragma	ioport	AnalogColumn_InputMUX_1_DriveMode_0_ADDR:	0x100
BYTE			AnalogColumn_InputMUX_1_DriveMode_0_ADDR;
#pragma	ioport	AnalogColumn_InputMUX_1_DriveMode_1_ADDR:	0x101
BYTE			AnalogColumn_InputMUX_1_DriveMode_1_ADDR;
#pragma	ioport	AnalogColumn_InputMUX_1_DriveMode_2_ADDR:	0x3
BYTE			AnalogColumn_InputMUX_1_DriveMode_2_ADDR;
#pragma	ioport	AnalogColumn_InputMUX_1_GlobalSelect_ADDR:	0x2
BYTE			AnalogColumn_InputMUX_1_GlobalSelect_ADDR;
#pragma	ioport	AnalogColumn_InputMUX_1_IntCtrl_0_ADDR:	0x102
BYTE			AnalogColumn_InputMUX_1_IntCtrl_0_ADDR;
#pragma	ioport	AnalogColumn_InputMUX_1_IntCtrl_1_ADDR:	0x103
BYTE			AnalogColumn_InputMUX_1_IntCtrl_1_ADDR;
#pragma	ioport	AnalogColumn_InputMUX_1_IntEn_ADDR:	0x1
BYTE			AnalogColumn_InputMUX_1_IntEn_ADDR;
#define AnalogColumn_InputMUX_1_MASK 0x1
#pragma	ioport	AnalogColumn_InputMUX_1_MUXBusCtrl_ADDR:	0x1d8
BYTE			AnalogColumn_InputMUX_1_MUXBusCtrl_ADDR;
// AnalogColumn_InputMUX_1 Shadow defines
//   AnalogColumn_InputMUX_1_DataShadow define
extern BYTE Port_0_Data_SHADE;
#define AnalogColumn_InputMUX_1_DataShadow (*(unsigned char*)&Port_0_Data_SHADE)
// GREENPin address and mask defines
#pragma	ioport	GREENPin_Data_ADDR:	0x8
BYTE			GREENPin_Data_ADDR;
#pragma	ioport	GREENPin_DriveMode_0_ADDR:	0x108
BYTE			GREENPin_DriveMode_0_ADDR;
#pragma	ioport	GREENPin_DriveMode_1_ADDR:	0x109
BYTE			GREENPin_DriveMode_1_ADDR;
#pragma	ioport	GREENPin_DriveMode_2_ADDR:	0xb
BYTE			GREENPin_DriveMode_2_ADDR;
#pragma	ioport	GREENPin_GlobalSelect_ADDR:	0xa
BYTE			GREENPin_GlobalSelect_ADDR;
#pragma	ioport	GREENPin_IntCtrl_0_ADDR:	0x10a
BYTE			GREENPin_IntCtrl_0_ADDR;
#pragma	ioport	GREENPin_IntCtrl_1_ADDR:	0x10b
BYTE			GREENPin_IntCtrl_1_ADDR;
#pragma	ioport	GREENPin_IntEn_ADDR:	0x9
BYTE			GREENPin_IntEn_ADDR;
#define GREENPin_MASK 0x4
#pragma	ioport	GREENPin_MUXBusCtrl_ADDR:	0x1da
BYTE			GREENPin_MUXBusCtrl_ADDR;
// GREENPin Shadow defines
//   GREENPin_DataShadow define
extern BYTE Port_2_Data_SHADE;
#define GREENPin_DataShadow (*(unsigned char*)&Port_2_Data_SHADE)
// PWM address and mask defines
#pragma	ioport	PWM_Data_ADDR:	0x8
BYTE			PWM_Data_ADDR;
#pragma	ioport	PWM_DriveMode_0_ADDR:	0x108
BYTE			PWM_DriveMode_0_ADDR;
#pragma	ioport	PWM_DriveMode_1_ADDR:	0x109
BYTE			PWM_DriveMode_1_ADDR;
#pragma	ioport	PWM_DriveMode_2_ADDR:	0xb
BYTE			PWM_DriveMode_2_ADDR;
#pragma	ioport	PWM_GlobalSelect_ADDR:	0xa
BYTE			PWM_GlobalSelect_ADDR;
#pragma	ioport	PWM_IntCtrl_0_ADDR:	0x10a
BYTE			PWM_IntCtrl_0_ADDR;
#pragma	ioport	PWM_IntCtrl_1_ADDR:	0x10b
BYTE			PWM_IntCtrl_1_ADDR;
#pragma	ioport	PWM_IntEn_ADDR:	0x9
BYTE			PWM_IntEn_ADDR;
#define PWM_MASK 0x10
#pragma	ioport	PWM_MUXBusCtrl_ADDR:	0x1da
BYTE			PWM_MUXBusCtrl_ADDR;
// PWM Shadow defines
//   PWM_DataShadow define
extern BYTE Port_2_Data_SHADE;
#define PWM_DataShadow (*(unsigned char*)&Port_2_Data_SHADE)
// PWR_BTN address and mask defines
#pragma	ioport	PWR_BTN_Data_ADDR:	0xc
BYTE			PWR_BTN_Data_ADDR;
#pragma	ioport	PWR_BTN_DriveMode_0_ADDR:	0x10c
BYTE			PWR_BTN_DriveMode_0_ADDR;
#pragma	ioport	PWR_BTN_DriveMode_1_ADDR:	0x10d
BYTE			PWR_BTN_DriveMode_1_ADDR;
#pragma	ioport	PWR_BTN_DriveMode_2_ADDR:	0xf
BYTE			PWR_BTN_DriveMode_2_ADDR;
#pragma	ioport	PWR_BTN_GlobalSelect_ADDR:	0xe
BYTE			PWR_BTN_GlobalSelect_ADDR;
#pragma	ioport	PWR_BTN_IntCtrl_0_ADDR:	0x10e
BYTE			PWR_BTN_IntCtrl_0_ADDR;
#pragma	ioport	PWR_BTN_IntCtrl_1_ADDR:	0x10f
BYTE			PWR_BTN_IntCtrl_1_ADDR;
#pragma	ioport	PWR_BTN_IntEn_ADDR:	0xd
BYTE			PWR_BTN_IntEn_ADDR;
#define PWR_BTN_MASK 0x20
#pragma	ioport	PWR_BTN_MUXBusCtrl_ADDR:	0x1db
BYTE			PWR_BTN_MUXBusCtrl_ADDR;
// PWR_BTN Shadow defines
//   PWR_BTN_DataShadow define
extern BYTE Port_3_Data_SHADE;
#define PWR_BTN_DataShadow (*(unsigned char*)&Port_3_Data_SHADE)
// REDPin address and mask defines
#pragma	ioport	REDPin_Data_ADDR:	0x10
BYTE			REDPin_Data_ADDR;
#pragma	ioport	REDPin_DriveMode_0_ADDR:	0x110
BYTE			REDPin_DriveMode_0_ADDR;
#pragma	ioport	REDPin_DriveMode_1_ADDR:	0x111
BYTE			REDPin_DriveMode_1_ADDR;
#pragma	ioport	REDPin_DriveMode_2_ADDR:	0x13
BYTE			REDPin_DriveMode_2_ADDR;
#pragma	ioport	REDPin_GlobalSelect_ADDR:	0x12
BYTE			REDPin_GlobalSelect_ADDR;
#pragma	ioport	REDPin_IntCtrl_0_ADDR:	0x112
BYTE			REDPin_IntCtrl_0_ADDR;
#pragma	ioport	REDPin_IntCtrl_1_ADDR:	0x113
BYTE			REDPin_IntCtrl_1_ADDR;
#pragma	ioport	REDPin_IntEn_ADDR:	0x11
BYTE			REDPin_IntEn_ADDR;
#define REDPin_MASK 0x4
#pragma	ioport	REDPin_MUXBusCtrl_ADDR:	0x1ec
BYTE			REDPin_MUXBusCtrl_ADDR;
// REDPin Shadow defines
//   REDPin_DataShadow define
extern BYTE Port_4_Data_SHADE;
#define REDPin_DataShadow (*(unsigned char*)&Port_4_Data_SHADE)
// BLUEPin address and mask defines
#pragma	ioport	BLUEPin_Data_ADDR:	0x10
BYTE			BLUEPin_Data_ADDR;
#pragma	ioport	BLUEPin_DriveMode_0_ADDR:	0x110
BYTE			BLUEPin_DriveMode_0_ADDR;
#pragma	ioport	BLUEPin_DriveMode_1_ADDR:	0x111
BYTE			BLUEPin_DriveMode_1_ADDR;
#pragma	ioport	BLUEPin_DriveMode_2_ADDR:	0x13
BYTE			BLUEPin_DriveMode_2_ADDR;
#pragma	ioport	BLUEPin_GlobalSelect_ADDR:	0x12
BYTE			BLUEPin_GlobalSelect_ADDR;
#pragma	ioport	BLUEPin_IntCtrl_0_ADDR:	0x112
BYTE			BLUEPin_IntCtrl_0_ADDR;
#pragma	ioport	BLUEPin_IntCtrl_1_ADDR:	0x113
BYTE			BLUEPin_IntCtrl_1_ADDR;
#pragma	ioport	BLUEPin_IntEn_ADDR:	0x11
BYTE			BLUEPin_IntEn_ADDR;
#define BLUEPin_MASK 0x40
#pragma	ioport	BLUEPin_MUXBusCtrl_ADDR:	0x1ec
BYTE			BLUEPin_MUXBusCtrl_ADDR;
// BLUEPin Shadow defines
//   BLUEPin_DataShadow define
extern BYTE Port_4_Data_SHADE;
#define BLUEPin_DataShadow (*(unsigned char*)&Port_4_Data_SHADE)
