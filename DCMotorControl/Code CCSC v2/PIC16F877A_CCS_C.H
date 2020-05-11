
// register definitions

#define W 0
#define F 1

// register files
#byte INDF          =0x00
#byte TMR0          =0x01
#byte PCL           =0x02
#byte STATUS        =0x03
#byte FSR           =0x04
#byte PORTA         =0x05
#byte PORTB         =0x06
#byte PORTC         =0x07
#byte PORTD         =0x08
#byte PORTE         =0x09

#byte EEDATA        =0x10C
#byte EEADR         =0x10D
#byte EEDATH        =0x10E
#byte EEADRH         =0x10F



#byte PCLATH        =0x0a

//interrupt

#byte PIR2    =0x0d
#byte PIE2    =0x8d
// INTCON Bits for C
#byte INTCON      =0x0b
#bit  GIE         = 0x0b.7
#bit  PEIE        = 0x0b.6
#bit  tmr0ie      = 0x0b.5
#bit  inte        = 0x0b.4
#bit  rbie        = 0x0b.3
#bit  tmr0if      = 0x0b.2
#bit  INTF        = 0x0b.1
#bit  rbif        = 0x0b.0
// PIR1 for C
#byte PIR1    =0x0c
#bit pspif  = 0x0c.7
#bit adif    = 0x0c.6
#bit RCIF    = 0x0c.5
#bit txif    = 0x0c.4
#bit sspif    = 0x0c.3
#bit ccp1if    = 0x0c.2
#bit tmr2if    = 0x0c.1
#bit tmr1if    = 0x0c.0
// PIE1 for C
#byte PIE1    =0x8c
#bit adie    = 0x8c.6
#bit RCIE    = 0x8c.5
#bit TXIE    = 0x8c.4
#bit sspie    = 0x8c.3
#bit ccp1ie    = 0x8c.2
#bit tmr2ie    = 0x8c.1
#bit tmr1ie    = 0x8c.0
//PIR2 for C
#bit cmif    = 0x0d.6
#bit eeif    = 0x0d.4
#bit bclif  = 0x0d.3
#bit ccp2if    = 0x0d.0
//PIE2 for C
#bit osfie    = 0x8d.7
#bit cmie    = 0x8d.6
#bit eeie   = 0x8d.4
//
#byte OPTION_REG    =0x81
#byte TRISA         =0x85
#byte TRISB         =0x86
#byte TRISC         =0x87
#byte TRISD         =0x88
#byte TRISE         =0x89

//USART
#byte TXSTA  =0x98
#bit TRMT    =0x98.1
#bit BRGH    =0x98.2
#bit SYNC    =0x98.4//USART mode select bit =1: Synchronous. =0 Asynchronous
#bit TXEN    =0x98.5
#bit TX9    =0x98.6//9 bit enable bit
#byte SPBRG  =0x99
#byte RCSTA  =0x18
#bit SPEN =0x18.7
#bit RX9 =0x18.6
#bit CREN =0x18.4
#bit FERR =0x18.2
#bit OERR =0x18.1
#byte TXREG  =0x19
#byte   RCREG =0x1A
//Capture/compare/pwm
#byte CCP1CON =0x17
#bit  CCP1X       =0x17.5
#bit  CCP1Y       =0x17.4
#bit  CCP1M3      =0x17.3
#bit  CCP1M2      =0x17.2
#bit  CCP1M1      =0x17.1
#bit  CCP1M0      =0x17.0
#byte CCPR1H  =0x16
#byte CCPR1L  =0x15
#byte CCP2CON =0x1d
#byte CCPR2H  =0x1c
#byte CCPR2L  =0x1b

//Timer 1
#byte T1CON       =0x10
#bit T1CKPS1      =0x10.5
#bit T1CKPS0      =0x10.4
#bit T1OSCEN      =0x10.3
#bit T1SYNC_NOT   =0x10.2
#bit TMR1CS       =0x10.1
#bit tmr1on       =0x10.0

#byte TMR1L       =0x0e
#byte TMR1H       =0x0f

#byte EECON1        =0x18C
#byte EECON2        =0x18D
//Timer 2
#byte T2CON          =0x12
#bit  TMR2ON         =0x12.2
#bit  T2CKPS1        =0x12.1
#bit  T2CKPS0        =0x12.0
#byte PR2            =0x92
//DINH NGHIA BIT
#bit RA5   =0x05.5
#bit RA4   =0x05.4
#bit RA3   =0x05.3
#bit RA2   =0x05.2
#bit RA1   =0x05.1
#bit RA0   =0x05.0

#bit RB7   =0x06.7
#bit RB6   =0x06.6
#bit RB5   =0x06.5
#bit RB4   =0x06.4
#bit RB3   =0x06.3
#bit RB2   =0x06.2
#bit RB1   =0x06.1
#bit RB0   =0x06.0

#bit RC7   =0x07.7
#bit RC6   =0x07.6
#bit RC5   =0x07.5
#bit RC4   =0x07.4
#bit RC3   =0x07.3
#bit RC2   =0x07.2
#bit RC1   =0x07.1
#bit RC0   =0x07.0

#bit RD7   =0x08.7
#bit RD6   =0x08.6
#bit RD5   =0x08.5
#bit RD4   =0x08.4
#bit RD3   =0x08.3
#bit RD2   =0x08.2
#bit RD1   =0x08.1
#bit RD0   =0x08.0

#bit RE2   =0x09.2
#bit RE1   =0x09.1
#bit RE0   =0x09.0


#bit TRISA5   =0x85.5
#bit TRISA4   =0x85.4
#bit TRISA3   =0x85.3
#bit TRISA2   =0x85.2
#bit TRISA1   =0x85.1
#bit TRISA0   =0x85.0

#bit TRISB7   =0x86.7
#bit TRISB6   =0x86.6
#bit TRISB5   =0x86.5
#bit TRISB4   =0x86.4
#bit TRISB3   =0x86.3
#bit TRISB2   =0x86.2
#bit TRISB1   =0x86.1
#bit TRISB0   =0x86.0

#bit TRISC7   =0x87.7
#bit TRISC6   =0x87.6
#bit TRISC5   =0x87.5
#bit TRISC4   =0x87.4
#bit TRISC3   =0x87.3
#bit TRISC2   =0x87.2
#bit TRISC1   =0x87.1
#bit TRISC0   =0x87.0

#bit TRISD7   =0x88.7
#bit TRISD6   =0x88.6
#bit TRISD5   =0x88.5
#bit TRISD4   =0x88.4
#bit TRISD3   =0x88.3
#bit TRISD2   =0x88.2
#bit TRISD1   =0x88.1
#bit TRISD0   =0x88.0

#bit TRISE2   =0x89.2
#bit TRISE1   =0x89.1
#bit TRISE0   =0x89.0
// OPTION Bits
#bit RBPU_NOT   = 0x81.7
#bit intedg     = 0x81.6
#bit T0CS       = 0x81.5
#bit t0se       = 0x81.4
#bit PSA        = 0x81.3
#bit PS2        = 0x81.2
#bit PS1        = 0x81.1
#bit PS0        = 0x81.0

// EECON1 Bits
#bit eepgd   = 0x18c.7
#bit free      = 0x18C.4
#bit wrerr     = 0x18C.3
#bit wren       = 0x18C.2
#bit wr         = 0x18C.1
#bit rd         = 0x18C.0

//ADCON0
#byte ADCON0      =0x1F
#bit  ADON        =0x1F.0 
#bit  ADC_GO      =0x1F.2
#bit  CHS0        =0x1F.3
#bit  CHS1        =0x1F.4
#bit  CHS2        =0x1F.5
#bit  ADCS0       =0x1F.6
#bit  ADCS1       =0x1F.7

//ADCON1
#byte ADCON1      =0x9F
#bit  ADFM        =0x9F.7
#bit  ADCS2       =0x9F.6
#bit  PCFG3       =0x9F.3
#bit  PCFG2       =0x9F.2
#bit  PCFG1       =0x9F.1
#bit  PCFG0       =0x9F.0
#byte ADRESH      =0x1E
#byte ADRESL      =0x9E

//SPI register
#byte SSPSTAT =0x94
#bit SMP=0x94.7
#bit CKE=0x94.6
#bit DA_SPI=0x94.5
#bit P_SPI=0x94.4
#bit S_SPI=0x94.3
#bit RW_SPI=0x94.2
#bit UA_SPI=0x94.1
#bit BF__SPI=0x94.0

#byte SSPCON1=0x14
#bit WCOL=0x14.7
#bit SSPOV=0x14.6
#bit SSPEN=0x14.5
#bit CKP=0x14.4
#bit SSPM3=0x14.3
#bit SSPM2=0x14.2
#bit SSPM1=0x14.1
#bit SSPM0=0x14.0

#byte SSPBUF=0x13
//Comparator modul
#byte CMCON    = 0x9C
#bit C2OUT     = 0x9C.7
#bit C1OUT     = 0x9C.6
#bit C2INV     =0x9C.5
#bit C1INV     =0x9C.4
#bit CM2       = 0x9C.2
#bit CM1       = 0x9C.1
#bit CM0       = 0x9C.0

