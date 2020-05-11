#define Led             RA4
#define Led_TRIS        TRISA4
#define _7seg_Port      PORTB
#define _7seg_TRIS      TRISB
#define digit1          RD7
#define digit1_TRIS     TRISD7
#define digit2          RD6
#define digit2_TRIS     TRISD6
#define digit3          RD5
#define digit3_TRIS     TRISD5
#define digit4          RD4
#define digit4_TRIS     TRISD4
#define Led_Run               RD3
#define Led_Run_TRIS          TRISD3
#define Led_Speed_Set         RD2
#define Led_Speed_Set_TRIS    TRISD2
#define Led_Rev_Set           RD1
#define Led_Rev_Set_TRIS      TRISD1
#define Counter_Pin           RC0
#define Counter_TRIS          TRISC0
#define PWM_Pin               RC2
#define PWM_TRIS              TRISC2
#define SDI RC4
#define SDO RC5
#define SCK RC3
#define SS  RA5
//----------------------------------------------
#define PPR             32//pulse per revolution
#define Enable_PWM_Modul()   {CCP1M3=1;CCP1M2=1;TMR2ON=1;}
#define Disable_PWM_Modul()   {CCP1M3=0;CCP1M2=0;TMR2ON=0;}
//-------------------------------------------------------------------
void init(void);
void RS232_init(void);
unsigned char RS232_transmit(unsigned char c);
unsigned char RS232_receive(unsigned char *pointer);
void ADC_init(void);
void Comparator_init (void);
void SPI_init(void);
void Timer1_Init(void);
void Timer2_Init(void);
void CCP_Init(void);
