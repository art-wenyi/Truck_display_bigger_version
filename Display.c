#include "I2C.h"
#include "Display.h"
#include "Screen_Image.h"
#include <stdio.h>

/* Define the I/O Display pin control */
#define 	Display_Start_Reset			P3OUT &= ~BIT2
#define 	Display_Stop_Reset			P3OUT |= BIT2
#define     Display_Reset_Pin_Enable	P3DIR |= BIT2

void Display_Initial(void){
	//WDTCTL = WDTPW | WDTHOLD;

	_delay_cycles(240000);
    // Initialize I2C peripheral as master
    I2C_Master_Display_Init(I2C_SOURCE_CLOCK_SMCLK,I2C_SOURCE_CLOCK_FREQ,I2C_CLOCK_DISPLAY_FREQ);
	_delay_cycles(240000);

	Display_Reset_Pin_Enable;                   //   don't forget to pull this this P3.2 pin

	Display_Stop_Reset;
	_delay_cycles(3000);
	Display_Start_Reset;
	_delay_cycles(120000);
	Display_Stop_Reset;

	_delay_cycles(3000);

	Set_Display_On_Off(0xAE);										// Display Off (0xAE/0xAF)
	Set_Display_Clock(0x80);										// Set Clock as 350 Frames/Sec
	Set_Multiplex_Ratio(0x1F);										// 1/16 Duty (0x0F~0x3F)
	Set_Display_Offset(0x00);										// Shift Mapping RAM Counter (0x00~0x3F)
	Set_Start_Line(0x00);											// Set Mapping RAM Display Start Line (0x00~0x3F)
	Set_Charge_Pump(0x14);											// Enable Built-in DC/DC Converter (0x10/0x14)
	Set_Addressing_Mode(0x01);										// Set Page Addressing Mode (0x00/0x01/0x02)
	Set_Segment_Remap(0xA1);										// Set SEG/Column Mapping (0xA0/0xA1)
	Set_Common_Remap(0xC8);											// Set COM/Row Scan Direction (0xC0/0xC8)
	Set_Common_Config(0x12);										// Set Sequential Configuration (0x02/0x12)
	Set_Contrast_Control(0xFF);										// Set SEG Output Current
	Set_Precharge_Period(0xF1);										// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	Set_VCOMH(0x20);												// Set VCOM Deselect Level
	Set_Entire_Display(0xA4);										// Disable Entire Display On (0xA4/0xA5)
	Set_Inverse_Display(0xA6);										// Disable Inverse Display On (0xA6/0xA7)

	Clear_RAM();													// Initialize Memory to all 0x00's

	Set_Display_On_Off(0xAF);										// Display On (0xAE/0xAF)
	Turn_Display_On();

//	Set_Display_On_Off(0xAE);										// Turn it off
}

void Set_Contrast_Control_Register(unsigned char mod){
	I2C_Write_Packet_To_Display(0x00,0x01,0x81);
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,mod);
	_delay_cycles(3000);
	return;
}

void Clear_RAM(void){
    unsigned char i,j;

    for(i=0;i<0x80;i++){
    	Set_Page_Address(0x00);
    	Set_Column_Address(i);
    	  for(j=0;j<4;j++){
    		  I2C_Write_Packet_To_Display(0x40,0x01,0x00);
    		  _delay_cycles(500);
    	  }
		}
    return;
}

void Display_Picture(unsigned char pic_up[],unsigned char pic_down[]){
    unsigned char i,j;

    for(i=0;i<0x40;i++){
    	Set_Page_Address(0x00);
    	Set_Column_Address(i+32);
    	  for(j=0;j<2;j++){
    		 I2C_Write_Packet_To_Display(0x40,0x01,pic_up[(i*2) + j]);
    		 __delay_cycles(800);
    	  	 }
    	  for(j=0;j<2;j++){
    		 I2C_Write_Packet_To_Display(0x40,0x01,pic_down[(i*2) + j]);
    		 __delay_cycles(800);
    	  	 }
		}
    return;
}

void Set_Start_Column(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,0x00+(d%32));
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,0x20+(d/32));
	_delay_cycles(3000);
}

void Set_Addressing_Mode(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,0x20);
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Page_Address(unsigned char page){
    page = (0xb0 | page);											// Set Page Start Address (B0-B7h)
    I2C_Write_Packet_To_Display(0x00,0x01,page);
    _delay_cycles(3000);
	return;
}

void Set_Column_Address(unsigned char column){
	I2C_Write_Packet_To_Display(0x00,0x01,( 0x10 | (column>>4)));
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,(0x0f & column));
	_delay_cycles(3000);
	return;
}

void Set_Start_Line(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,0x40|d);
	_delay_cycles(3000);
}

void Set_Contrast_Control(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,0x81);
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Charge_Pump(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,0x8D);
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Segment_Remap(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Entire_Display(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Inverse_Display(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Multiplex_Ratio(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,0xA8);
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Display_On_Off(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Start_Page(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,(0xB0|d));
	_delay_cycles(3000);
}

void Set_Common_Remap(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Display_Offset(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,0xD3);
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Display_Clock(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,0xD5);
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Precharge_Period(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,0xD9);
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_Common_Config(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,0xDA);
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_VCOMH(unsigned char d){
	I2C_Write_Packet_To_Display(0x00,0x01,0xDB);
	_delay_cycles(3000);
	I2C_Write_Packet_To_Display(0x00,0x01,d);
	_delay_cycles(3000);
}

void Set_NOP(){
	I2C_Write_Packet_To_Display(0x00,0x01,0xE3);
	_delay_cycles(3000);
}

void Turn_Display_On(){

	Set_Charge_Pump(0x14);											// Enable Built-in DC/DC Converter (0x10/0x14)

	Set_Display_On_Off(0xAF);										// Display On (0xAE/0xAF)
}

void Turn_Display_Off(){

	Set_Display_On_Off(0xAE);										// Display Off (0xAE/0xAF)

	Set_Charge_Pump(0x10);											// Disable Built-in DC/DC Converter (0x10/0x14)
}


//=== add by wenyi 06-02-2016 =====================================
/*
void displayLeftSide(int display_flag){
	char data=0x00;
	int col=0;
	if(display_flag == 2){            // flag is 2 , show side walk
		for(col=0;col<4;col++){          // 4 rows in total, from top to bottom, each row display 8 bits, total 32 bit
			Set_Page_Address(0x00);			 // always page 0
			Set_Column_Address(col+32);      // +32 because display column start with column address 32 in memory
			int row=0;
			for(row=0;row<4;row++){      // side walk has 4 columns each
				data = ((row + col) + 1) % 4 ==0 ? 0b11111100 : 0x00;
				display1Byte(data);
			}
		}
	}else if(display_flag == 1){		 // flag is 1 , don't show side walk
		for(col=0;col<4;col++){          // 4 rows in total, from top to bottom, each row display 8 bits, total 32 bit
			Set_Page_Address(0x00);			 // always page 0
			Set_Column_Address(col+32);      // +32 because display column start with column address 32 in memory
			int row=0;
			for(row=0;row<4;row++){      // side walk has 4 columns each
				display1Byte(0b00000000);
			}
		}
	}else{                        		// flag is 0 , keep last state
		_nop();
	}

}

void displayRightSide(int display_flag){
	char data=0x00;
	int col=0;
	if(display_flag == 2){            // flag is 2 , show side walk
		for(col=0;col<4;col++){          // 4 rows in total, from top to bottom, each row display 8 bits, total 32 bit
			Set_Page_Address(0x00);
			Set_Column_Address(col+32+28);   // use 28,29,30,31 cols
			int row=0;
			for(row=0;row<4;row++){      // side walk has 4 columns each
				data = ((row - col) + 4) % 4 ==0 ? 0b11111100 : 0x00;
				display1Byte(data);
			}
		}
	}else if(display_flag == 1){
		for(col=0;col<4;col++){          // 4 rows in total, from top to bottom, each row display 8 bits, total 32 bit
			Set_Page_Address(0x00);
			Set_Column_Address(col+32+28);   // use 28,29,30,31 cols
			int row=0;
			for(row=0;row<4;row++){      // side walk has 4 columns each
				display1Byte(0b00000000);
			}
		}
	}else{
		_nop();
	}

}

void displayMiddle(int car_flag, int people_flag, int velocity_flag, int velocity){				// display car, people, velocity

	unsigned char *car_img=NULL, \
		 *people_img=NULL, \
		 *velocity_1_digit=NULL,\
		 *velocity_2_digit=NULL, \
		 *velocity_3_digit=NULL;					   // set pointers, use for point to the actual pic that should be displayed

		car_img = (car_flag == 2)?CAR_YES:CAR_NO;
		people_img = (people_flag == 2)?PED_YES:PED_NO;
		velocity_1_digit = numberMapping((velocity/100)%10);    // hundred's digit
		velocity_2_digit = numberMapping((velocity/10)%10);		// ten's digit
		velocity_3_digit = numberMapping(velocity%10);			// unit's digit

		// first part, display car and first number part
		int col=0;
		for(col=0;col<2;col++){              // display first 2 cols of car
			Set_Page_Address(0x00);
			Set_Column_Address(col+32+6);
			display1Byte(car_img[col*2+0]);
			display1Byte(car_img[col*2+1]);
			display1Byte(BLANK);
			display1Byte(BLANK);
		}

		for(col=2;col<6;col++){				// display first 4 cols of digit 1 and 4 cols of middle of the car
			Set_Page_Address(0x00);
			Set_Column_Address(col+32+6);
			display1Byte(car_img[col*2+0]);
			display1Byte(car_img[col*2+1]);
			display1Byte(velocity_1_digit[(col-2)*2+0]);
			display1Byte(velocity_1_digit[(col-2)*2+1]);
		}

		for(col=6;col<8;col++){				// display last 2 cols of car
			Set_Page_Address(0x00);
			Set_Column_Address(col+32+6);
			display1Byte(car_img[col*2+0]);
			display1Byte(car_img[col*2+1]);
			display1Byte(BLANK);
			display1Byte(BLANK);
		}

		for(col=8;col<12;col++){				// display mid 4 ten's digit
			Set_Page_Address(0x00);
			Set_Column_Address(col+32+6);
			display1Byte(BLANK);
			display1Byte(BLANK);
			display1Byte(velocity_2_digit[(col-8)*2+0]);
			display1Byte(velocity_2_digit[(col-8)*2+1]);
		}

		for(col=12;col<13;col++){				// display 1 blank column
			Set_Page_Address(0x00);
			Set_Column_Address(col+32+6);
			display1Byte(BLANK);
			display1Byte(BLANK);
			display1Byte(BLANK);
			display1Byte(BLANK);
		}

		for(col=13;col<14;col++){				// display first 2 cols of people
			Set_Page_Address(0x00);
			Set_Column_Address(col+32+6);
			display1Byte(people_img[(col-13)*2+0]);
			display1Byte(people_img[(col-13)*2+1]);
			display1Byte(BLANK);
			display1Byte(BLANK);
		}

		for(col=14;col<18;col++){				// display first 4 cols of digit 3 and 4 cols of middle of the people
			Set_Page_Address(0x00);
			Set_Column_Address(col+32+6);
			display1Byte(people_img[(col-13)*2+0]);
			display1Byte(people_img[(col-13)*2+1]);
			display1Byte(velocity_3_digit[(col-14)*2+0]);
			display1Byte(velocity_3_digit[(col-14)*2+1]);
		}

		for(col=18;col<20;col++){              // display last 2 cols of people
			Set_Page_Address(0x00);
			Set_Column_Address(col+32+6);
			display1Byte(people_img[(col-13)*2+0]);
			display1Byte(people_img[(col-13)*2+1]);
			display1Byte(BLANK);
			display1Byte(BLANK);
		}
}

*/
void display1Byte(unsigned char data){                             // just for display 1 byte blank on display
	I2C_Write_Packet_To_Display(0x40,0x01,data);
	__delay_cycles(400);           //8MhzÏÂÃ¿¸öÏÔÊ¾ÑÓ³Ù²»ÄÜµÍÓÚ300£¬·ñÔòÁ¢±À
}

unsigned char* numberMapping(int digit){          // mapping the numbers to picture array
	switch(digit){
	case 0: return NUM_0;
	case 1: return NUM_1;
	case 2: return NUM_2;
	case 3: return NUM_3;
	case 4: return NUM_4;
	case 5: return NUM_5;
	case 6: return NUM_6;
	case 7: return NUM_7;
	case 8: return NUM_8;
	case 9: return NUM_9;
	}
	return NUM_8;
}

void displayBlankLine(int col){               // display one blank line at certain column
	Set_Page_Address(0x00);
	Set_Column_Address(col+32);
	display1Byte(BLANK);
	display1Byte(BLANK);
	display1Byte(BLANK);
	display1Byte(BLANK);
}

void displayCar(int col){                     // display car at certain column
	int i=0;
	for(i=0;i<24;i++){
		Set_Page_Address(0x00);
		Set_Column_Address(col+32+i);
		display1Byte(CAR_YES[i*4+0]);
		display1Byte(CAR_YES[i*4+1]);
		display1Byte(CAR_YES[i*4+2]);
		display1Byte(CAR_YES[i*4+3]);
	}
}

void displayPeople(int col){                  // display people at certain column
	int i=0;
	for(i=0;i<19;i++){
		Set_Page_Address(0x00);
		Set_Column_Address(col+32+i);
		display1Byte(PED_YES[i*4+0]);
		display1Byte(PED_YES[i*4+1]);
		display1Byte(PED_YES[i*4+2]);
		display1Byte(PED_YES[i*4+3]);
	}
}

void displayLeftSide(){                      // display left side at the left of the screen
	int i=0;
	for(i=0;i<5;i++){
		Set_Page_Address(0x00);
		Set_Column_Address(i+32);
		display1Byte(SIDE_LANE_LEFT[i*4+0]);
		display1Byte(SIDE_LANE_LEFT[i*4+1]);
		display1Byte(SIDE_LANE_LEFT[i*4+2]);
		display1Byte(SIDE_LANE_LEFT[i*4+3]);
	}
}

void displayRightSide(){                    // display the right side at the right of the screen
	int i=0;
	for(i=0;i<5;i++){
		Set_Page_Address(0x00);
		Set_Column_Address(i+32+27);
		display1Byte(SIDE_LANE_RIGHT[i*4+0]);
		display1Byte(SIDE_LANE_RIGHT[i*4+1]);
		display1Byte(SIDE_LANE_RIGHT[i*4+2]);
		display1Byte(SIDE_LANE_RIGHT[i*4+3]);
	}
}

void displayVelocity(int velocity){                  // display the velocity
	if(velocity == -1){
		displayVelocity(999);
		return;
	}
	unsigned char    *velocity_1_digit=NULL,\
			 	 	 *velocity_2_digit=NULL, \
			 	 	 *velocity_3_digit=NULL;					   // set pointers, use for point to the actual pic that should be displayed
	velocity_1_digit = numberMapping((velocity/100)%10);    // hundred's digit
	velocity_2_digit = numberMapping((velocity/10)%10);		// ten's digit
	velocity_3_digit = numberMapping(velocity%10);			// unit's digit
	int col=0;
	for(col=0;col<8;col++){
		Set_Page_Address(0x00);
		Set_Column_Address(col+32);
		display1Byte(velocity_1_digit[col*4+0]);
		display1Byte(velocity_1_digit[col*4+1]);
		display1Byte(velocity_1_digit[col*4+2]);
		display1Byte(velocity_1_digit[col*4+3]);
	}
	displayBlankLine(col++);
	displayBlankLine(col++);
	displayBlankLine(col++);
	displayBlankLine(col++);
	for(col=12;col<20;col++){
			Set_Page_Address(0x00);
			Set_Column_Address(col+32);
			display1Byte(velocity_2_digit[(col-12)*4+0]);
			display1Byte(velocity_2_digit[(col-12)*4+1]);
			display1Byte(velocity_2_digit[(col-12)*4+2]);
			display1Byte(velocity_2_digit[(col-12)*4+3]);
		}
	displayBlankLine(col++);
	displayBlankLine(col++);
	displayBlankLine(col++);
	displayBlankLine(col++);
	for(col=24;col<32;col++){
			Set_Page_Address(0x00);
			Set_Column_Address(col+32);
			display1Byte(velocity_3_digit[(col-24)*4+0]);
			display1Byte(velocity_3_digit[(col-24)*4+1]);
			display1Byte(velocity_3_digit[(col-24)*4+2]);
			display1Byte(velocity_3_digit[(col-24)*4+3]);
		}
}

void displayCarOnly(){							// display car only, in the middle of the screen
	displayBlankLine(0);
	displayBlankLine(1);
	displayBlankLine(2);
	displayBlankLine(3);
	displayCar(4);
	displayBlankLine(28);
	displayBlankLine(29);
	displayBlankLine(30);
	displayBlankLine(31);
}

void displayCarWithLeftSide(){					// display the car with left side only
	displayLeftSide();
	displayBlankLine(5);
	displayCar(6);
	displayBlankLine(30);
	displayBlankLine(31);
}

void displayCarWithRightSide(){					// display the car with right side only
	displayBlankLine(0);
	displayBlankLine(1);
	displayCar(2);
	displayBlankLine(26);
	displayRightSide();
}

void displayPeopleOnly(){						// display the people only in the middle
	int i=0;
	for(i=0;i<7;i++){
		displayBlankLine(i);
	}
	displayPeople(7);
	for(i=26;i<32;i++){
		displayBlankLine(i);
	}
}

void displayPeopleWithLeftSide(){				// display the people with left side only
	displayLeftSide();
	int i=5;
	for(i=5;i<8;i++){
		displayBlankLine(i);
	}
	displayPeople(8);
	for(i=27;i<32;i++){
		displayBlankLine(i);
	}
}

void displayPeopleWithRightSide(){				// display the people with right side only
	int i=0;
	for(i=0;i<5;i++){
		displayBlankLine(i);
	}
	displayPeople(5);
	for(i=24;i<27;i++){
		displayBlankLine(i);
	}
	displayRightSide();
}

void displayLeftSideOnly(){						// display left side only
	displayLeftSide();
	int i=0;
	for(i=5;i<32;i++){
		displayBlankLine(i);
	}
}

void displayRightSideOnly(){					// display right side only
	int i=0;
	for(i=0;i<27;i++){
		displayBlankLine(i);
	}
	displayRightSide();
}
