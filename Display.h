
#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "I2C.h"
#include "msp430f5659.h"

void Display_Initial(void);
void Display_Write_Instruction(unsigned char command);
void Display_Write_Data(unsigned char data);
void Set_Contrast_Control_Register(unsigned char mod);
void Display_Picture(unsigned char pic_up[],unsigned char pic_down[]);
extern void Set_Start_Column(unsigned char d);
void Set_Addressing_Mode(unsigned char d);
void Set_Page_Address(unsigned char page);
void Set_Column_Address(unsigned char column);
void Set_Start_Line(unsigned char d);
void Set_Contrast_Control(unsigned char d);
void Set_Charge_Pump(unsigned char d);
void Set_Segment_Remap(unsigned char d);
void Set_Entire_Display(unsigned char d);
void Set_Inverse_Display(unsigned char d);
void Set_Multiplex_Ratio(unsigned char d);
void Set_Display_On_Off(unsigned char d);
void Set_Start_Page(unsigned char d);
void Set_Common_Remap(unsigned char d);
void Set_Display_Offset(unsigned char d);
void Set_Display_Clock(unsigned char d);
void Set_Precharge_Period(unsigned char d);
void Set_Common_Config(unsigned char d);
void Set_VCOMH(unsigned char d);
void Set_NOP();
void Fill_RAM(unsigned char Data);
void Clear_RAM(void);
void Turn_Display_On(void);
void Turn_Display_Off(void);

//=== below are functoins add by wenyi, 06-02/2016 =======================================
//=== use for the trunk display project ===================================
unsigned char* numberMapping(int digit);
void display1Byte(unsigned char data);


void displayBlankLine(int col);
void displayCar(int col);
void displayPeople(int col);
void displayLeftSide();
void displayRightSide();
void displayVelocity(int velocity);

void displayCarOnly();
void displayCarWithLeftSide();
void displayCarWithRightSide();
void displayPeopleOnly();
void displayPeopleWithLeftSide();
void displayPeopleWithRightSide();
void displayLeftSideOnly();
void displayRightSideOnly();
void displayProcess(int leftside, int rightside, int car_flag, int people_flag, int velocity_flag, int velocity);

#endif /* DISPLAY_H_ */
