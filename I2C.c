#include "I2C.h"
#include "msp430f5659.h"

// Display Local Variables
unsigned char TXByteCtr;
unsigned char TxDataBuff[2];
unsigned char *PTxData;
unsigned char TXDataBuff_Count = 0;

unsigned char tx_reg_name_display;
// tx_length is how many bytes of data will be written
unsigned char tx_length_display;
// *tx_data will point to the data to be written
unsigned char tx_data_display;
// rx_reg_name is the register the MSP430 will read data from
unsigned char rx_reg_name_display;
// rx_length is how many bytes of data will be read
unsigned char rx_length_display;
// *rx_data will point to the data to be read
unsigned char *rx_data_display;
// receive flag is used for the ISR to know the MSP430 will be reading from the
// Accelerometer
unsigned char i2c_receive_flag_display;
// transmit flag is used for the ISR to know the MSP430 will be writing to the
// Accelerometer
unsigned char i2c_transmit_flag_display;
// Used to keep track how many bytes have been received
unsigned char rx_byte_ctr_display;
// Used to keep track how many bytes have been transmitted
unsigned char tx_byte_ctr_display;


///////////////////////////////////////////////////////////////////////////// Display begins/////////////////////////////////////////////////////////////////////////////
void I2C_Master_Display_Init(unsigned char selectClockSource,
				     unsigned long clockSourceFrequency,
				 	 unsigned long desiredI2CClock){
  // Set-up I2C SDA and SCL pins
	 P8SEL |= BIT5;             // use UCB1SDA, P8.5
     P8SEL |= BIT6;				// use UCB1SCL, P8.6
     // Disable the USCI Module, for edit settings to USCI register
	 I2C_Display_Disable();

	 UCB1CTL1 |= UCSSEL_2 + UCSWRST; 						//Select SMCLK			 											// Select ACLK
	 UCB1BR0 = (unsigned short) (clockSourceFrequency/desiredI2CClock);
	 UCB1BR1 = (unsigned short) ( (clockSourceFrequency/desiredI2CClock) >> 8 );
	 UCB1I2CSA = I2C_Display_Write_ADDR;
	 UCB1CTL0 |= UCMST + UCMODE_3 + UCSYNC;

	 I2C_Display_Enable();
	 UCB1IE |= UCTXIE;
}


void I2C_Display_Set_Slave_Address(unsigned char slaveAddress)
{
  // Set the address of the slave with which the master will communicate.
  UCB1I2CSA = slaveAddress;
}

void I2C_Display_Enable(void)
{
	UCB1CTL1 &= ~(UCSWRST);
}

void I2C_Display_Disable(void)
{
	UCB1CTL1 |= (UCSWRST);     // must do this before change settings
}

void I2C_Display_Set_Mode(unsigned short receive){
	  UCB1CTL1 |= UCSWRST;
	  if (receive){
	    // Configure in receive mode
	    UCB1CTL1 &= ~(UCTR);
	  }
	  else{
	    // Configure in transmit mode
	    UCB1CTL1 |= UCTR;
	  }
}

unsigned short I2C_Display_Bus_Busy(void){
  // Return the bus busy status.
  if(UCB1STAT & UCBBUSY){
      return(0x00);
  }
  else{
      return(0x01);
  }
}

unsigned short I2C_Display_Busy(void){
  // Return the busy status.
  if((UCB1IFG & UCTXIFG) || (UCB1IFG & UCRXIFG)){
      return(0x00);
  }
  else{
      return(0x01);
  }
}

void I2C_Display_Interrupt_Enable(unsigned char interruptFlags){
  // Enable the interrupt masked bit
  UCB1IE |= interruptFlags;
}

void I2C_Display_Interrupt_Disable(unsigned char interruptFlags){
  // Disable the interrupt masked bit
  UCB1IE &= ~(interruptFlags);
}

void I2C_Display_Interrupt_Clear(unsigned char interruptFlags){
  // Clear the I2C interrupt source.
  UCB1IFG &= ~(interruptFlags);
}

unsigned char I2C_Display_Interrupt_Status(unsigned char mask){
  // Return the interrupt status of the request masked bit.
  return (UCB1IFG & mask);
}

void I2C_Write_Packet_To_Display(unsigned char displayRegName,
						    unsigned char dataLength,
						    unsigned char writeData){

	  // Assign values to global variables
	  TxDataBuff[0] = displayRegName;
	  TxDataBuff[1] = writeData;
	  TXByteCtr = 2;
	  TXDataBuff_Count = 0;

	  // Set Slave Address to [0x3C + UCTR] (Transmit Bit)
	  UCB1I2CSA = I2C_Display_Write_ADDR;
	  I2C_Display_Interrupt_Enable(UCTXIE);
	  I2C_Display_Interrupt_Enable(UCNACKIE);
	  UCB1CTL1 |= UCTR + UCTXSTT;            							// Send I2C start condition
      __bis_SR_register(GIE);       									// CPU off, enable interrupts
      _no_operation();
	   while(UCB1CTL1 & UCTXSTP);          								// Ensure stop condition got sent

}
///////////////////////////////////////////////////////////////////////////// Display ends/////////////////////////////////////////////////////////////////////////////

/* Display ISR */
#pragma vector = USCI_B1_VECTOR
__interrupt void USCI_I2C_Display_ISR(void){
  switch(__even_in_range(UCB1IV,12)){
  case  0: break;                           // Vector  0: No interrupts
  case  2: break;                           // Vector  2: ALIFG
  case  4: 		      						// Vector  4: NACKIFG
    _no_operation();
	  break;
  case  6: break;                           // Vector  6: STTIFG
  case  8: break;                           // Vector  8: STPIFG
  case 10: 									// Vector 10: RXIFG
  	break;
  case 12:         // Vector 12: TXIFG
  if(TXByteCtr--){
	   UCB1TXBUF = TxDataBuff[TXDataBuff_Count++];
   	   }

  else{
       UCB1CTL1 |= UCTXSTP;                  					// Send I2C stop condition
       UCB1IFG &~ UCTXIFG;
//       if(sleep_exit == FALSE)
//       __bic_SR_register_on_exit(LPM4_bits); 					 // Exit active CPU
   	   }
   break;
  default: break;
  }
}




