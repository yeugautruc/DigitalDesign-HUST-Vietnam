//#include "PIC16F877A_CCS_C.H"//include if use CCS C
//#include "pic16f877a_function.h"


//-------------------------------
void init(void){
   LED_TRIS=0;
   //TRISA0=1;TRISA1=1;TRISA2=1;TRISA3=1;
   _7seg_TRIS=0;
   digit1_TRIS=0;digit2_TRIS=0;digit3_TRIS=0;digit4_TRIS=0;
   Led_Run_TRIS=0;Led_Speed_Set_TRIS=0;Led_Rev_Set_TRIS=0;
   Counter_TRIS=1;PWM_TRIS=0;PWM_pin=1;
   //RBPU_NOT=0;//port B pull up. auto disable when configure as output
   
   
   //TMR1IE=1;//enable tmier1 interrupt
   //
   TMR0IE=1;//enable timer0 interrupt
   T0CS=0;//timer0 use internal clock
   PSA=0;//prescaler is assigned to the timer0
   PS2=1;
   PS1=0;
   PS0=0;//scale 1:32->1638.4us thi ngat
   
    //INTEDG=1;//RB0 interrupt on rising edge
   //INTE=1;//enable RB0 interrupt
   //TRISB0=1;
   GIE=1;//enable global interrupt
   PEIE=1;//enable peripheral interrupt
}
//-----------------------------------------
void ADC_init(void){
   // delay min 20us when change adc channel
   ADIE=0;//interrupt disable
   ADFM=1;//6 most signification bit of ADRESH=0
   TRISA0=1;
   //TRISA1=1;
   //TRISA3=1;
   ADCS2=1;
   ADCS1=1;
   ADCS0=0;//Tad=64Tosc=3.2us. Have to delay 38.4us to finish adc conversion
   PCFG3=1;
   PCFG2=1;
   PCFG1=1;
   PCFG0=0;//RA0;
   ADON=1;//power up adc modul
   CHS2=0;CHS1=0;CHS0=0;//select channel 0
}
/*name: void RS232_init(void)
  khoi tao thong so cho RS232
*/
void RS232_init(void){
   //
   BRGH=1;//high baud rate generator
   SPBRG=129;//or 25:baud rate 9.6K
    SYNC=0;//asynchronous
   SPEN=1;//serial port enable. Tx<RC6>;Rx<RC7> will be configured auto
   //
   //CREN=1;//enable receiver
   //RCIE=1;//enable ceceiver interrupt
  // RX9=0;//8 bit receive
   //
   TXIE=0;//disable interrupt
   TX9=0;//8 bit trans
   TXEN=1;//enable transmit. tramsmit occur when load TXREG register. TXIF is set
   GIE=1;//enable global interrupt
   PEIE=1;//enable peripheral interrupt
   TRISC6=0;
   TRISC7=1;
}

/*name: unsigned char RS232_transmit(unsigned char c)
  ghi ky tu de truyen RS232. Neu ghi thanh cong tra ve 1. ghi ko thanh cong tra ve 0
  */
unsigned char RS232_transmit(unsigned char c){
   if(TXIF==1){//TXIF only be cleared when TXREG if full
      TXREG=c;//load register. transmit occur. TXIF will be clear.
      return 1;
   }else
      return 0;
}
/*name: unsigned char RS232_receive(unsigned char *pointer)
  tra ve error code. doc ky tu nhan duoc tu rs232 ghi vao bien con tro pointer.
*/
unsigned char RS232_receive(unsigned char *pointer){
   if((OERR==0)&&(FERR==0)){//no error
      *pointer=RCREG;//RCREG can contain 2 byte. RCIF is cleared when RCREG is empty
      return 0;//error code =0: no error
   }
   if((OERR==1)&&(FERR==0)){//over run error: RCREG is full, byte in RSR will be lost
      *pointer=RCREG;//RCREG can contain 2 byte. RCIF is cleared when RCREG is empty
      return 1;//error code =1
   }
   if((OERR==0)&&(FERR==1)){//frame error bit: must be check b4 reading RCREG
      *pointer=RCREG;//RCREG can contain 2 byte. RCIF is cleared when RCREG is empty
      return 2;//error code =2
   }
   if((OERR==1)&&(FERR==1)){//
      *pointer=RCREG;//RCREG can contain 2 byte. RCIF is cleared when RCREG is empty
      return 3;//error code =3
   }
}
//-----------------------------------------------------------------
//
void SPI_init(void){
   TRISC3=1;//SCK
   TRISC4=1;//SDI
   TRISC5=0;//SDO
   TRISA5=0;//SS
   
   CKP=1;//idle state of clock is high level
   CKE=0;//transmit occur when idle to active
   SMP=0;//disable in slave mode
   SSPM3=0;SSPM2=1;SSPM1=0;SSPM0=1;//slave mode, SS disable, SS pin use as IO
   SSPIE=0;//disable interrupt
   SSPEN=1;//enable and configure pin for SPI
   TRISA5=0;//SS
   SS=1;
}
//----------------------------------------------------------------------------

void Comparator_init(void){
   //respond time max 10us
   TRISA0=1;
   TRISA1=1;
   TRISA3=1;
   CM2=1;
   CM1=0;
   CM0=0;
   C1INV=0;
   C2INV=0;//no inversion output
   CMIE=1;//interrupt enable
   
}
//------------------------------------------------------------------------------
void Timer1_Init(void){
   //
   TMR1CS=1;//timer1 external clock (counter).Rasing edge.Must have falling edge fist
   //TMR1CS=0;//timer1 internal clock 
   T1CKPS1=0;
   T1CKPS0=0;//scale 1:1
   T1OSCEN=0;//Timer1 Oscillator Disable. Counter rasing on pin RC0 
   T1SYNC_NOT=0;//Synchronize external clock
   TMR1IE=0;//disable interrupt on timer 1
   TMR1IF=0;//clear interrupt flag
   //TMR1H=0;TMR1L=0;
   //TMR1ON=1;//run timer1;
}
void Timer2_Init(void){
   

}

void CCP_Init(void){
   //CCP1M3=1;CCP1M2=1;//PWM mode. 10bit resolution
   T2CKPS1=0;T2CKPS0=1;//timer 2 prescale 1:1
   PR2=0xFF;//19,53KHz
   Disable_PWM_Modul();
   //TMR2ON=0;//stop timer 2
   //TMR2IE=1;
}
