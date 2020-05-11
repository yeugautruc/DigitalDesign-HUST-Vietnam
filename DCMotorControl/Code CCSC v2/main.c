/* Code project 1 Dieu khien van toc DC
Code viet bang CCSC ver4.105
Chinh sua lan cuoi vao 21/10/2013
//---------------------------------------
system source mangagement:dung de quan ly tai nguyen
system tick Timer0=1.6ms;
Stage 1: Default
   Hien thi toc do do duoc.Quet nut bam. Nhap nhay Led.
   Tu dong vao stage 1 sau khi khoi tao. Vao stage 1 khi bam button run tu stage 4, khi so vong quay yeu cau =0.
   Bam button dat toc do de chuyen sang stage 2. Bam button dat so vong de chuyen sang stage 3.
//-----------------------------------------
task 1: nhap nhay led
   pin RA4;timer0
   piority 3
task 2: Quet 7seg 4digit
   PORTB;RD4-RD7;
   piority 3
task 3: quet nut bam
   pin RA1,RA2,RA3;
   piority 3
task 4:do toc do dong co
   dem so xung trong 163,8ms de tinh van toc
   timer0;counter1;;pin RC0(Counter), 
   piority 4
   
*/
#include "main.h"
#include "PIC16F877A_CCS_C.H"
#include "pic16f877a_function.h"
#include "pic16f877a_function.c"
#include <system.c>
//------------------------------------------------------------------------------
struct Flag1{
   unsigned char Run:1;
   unsigned char Speed_Set:1;
   unsigned char Rev_Set:1;
   unsigned char temp:5;
}Button_Flag={0,0,0,0};

//******************************************************************************
//  Global variables
//******************************************************************************
unsigned char const LED_MAP[11] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x79};
struct Task_ID_Type Task_Led={1,3,1};
unsigned long Task_Led_Timer=0;// dung khi delay task_led. 16bit
struct Task_ID_Type Task_7seg={2,3,1};
unsigned long Task_7seg_Timer=0;// dung khi delay task_Button
struct Task_ID_Type Task_Button={3,3,1};
unsigned long Task_Button_Timer=0;// dung khi delay task_Button
struct Task_ID_Type Task_Get_Speed={4,4,1};//Counter do van toc
unsigned long Task_Get_Speed_Timer=0;// dung khi delay task_get speed
//------------------------------------------------------------------------------
unsigned char Timer0_Counter=0;
unsigned char Stage=0;
unsigned char _7seg_Buff[4];
unsigned long RPM_Value=0;//van toc do dc
unsigned long RPM_Set=100;//van toc cai dat
unsigned long RPM_Max=520;//max speed per minute
unsigned long Rev_Gonna_Run=9999;//so vong se chay
float Pulse_Gonna_Run=0;
unsigned long Pulse_Gonna_Run_temp=0;
unsigned long Pulse_Count=0;//dem xung xem da chay duoc bao nhieu vong
unsigned long RPM_Value_temp=0;
unsigned char i=1;
//------------------------------------------------------------------------------
float PID_MAX=1023;
float PID_MIN=0;
//******************************************************************************
//function prototype
void Call_Task_Led(void);//nhap nhay led
void Call_Task_7seg (void);//hien thi 7seg 4digit
void Call_Task_Button (void);//quet nut bam
void Call_Task_Get_Speed (void);//Counter
void Put_Duty(unsigned int16 Process_Value,unsigned long Set_Point);//update gia tri PID vao PID value
unsigned long ADC_Read(void);
//******************************************************************************
//******************************************************************************
//  Interrupt routine
//******************************************************************************
#INT_TIMER0
void Timer0_interrupt(void){
   TMR0IF=0;
   Timer0_Counter=1;
   if(Task_Get_Speed_Timer>0)   Task_Get_Speed_Timer--;
   else TMR1ON=0;//stop timer1;
   
}
#INT_AD
void AD_interrupt(void){
   ADIF=0;
   ADIE=0;   
}
#INT_TIMER2 
void Timer2(void){
   TMR2IF=0;//CCP1R1L=200;
}
//--------------------------------------------
//******************************************************************************
//  main
//******************************************************************************
void main()
{  
   init();
   CCP_Init();
   delay_ms(1000);
   CCP_Init();
   Timer1_Init();
   //Timer2_Init();
   ADC_init();
   Stage=2;
   
   while(1){
      //quan ly cac task
      if(Timer0_Counter==1){
         Timer0_Counter=0;
         if(Task_Led_Timer>0)   Task_Led_Timer--;
         if(Task_7seg_Timer>0)   Task_7seg_Timer--;
         if(Task_Button_Timer>0)   Task_Button_Timer--;
         //if(Task_Get_Speed_Timer>0)   Task_Get_Speed_Timer--;
      }
      if(Task_Led_Timer==0)   Task_Led.Piority=3;
      if(Task_7seg_Timer==0)   Task_7seg.Piority=3;
      if(Task_Button_Timer==0)   Task_Button.Piority=3;
      if(Task_Get_Speed_Timer==0)   Task_Get_Speed.Piority=4;
      //select stage----------------------------------------------------------
      //stage 1: cai dat RPM_max
      if(Stage==1){
         Task_Array[0]=Task_OS_Temp;
         Task_Array[1]=Task_Led;
         Task_Array[2]=Task_7seg;
         Task_Array[3]=Task_Button;
         Task_Array[4]=Task_Get_Speed;
         switch(select_task(5)){
            case 0:Call_Task_OS_Temp();break;
            case 1:Call_Task_Led();break;
            case 2:{
               unsigned long temp=RPM_Value_temp/i;
               if(i>50) RPM_Max=temp;
               _7seg_Buff[0] = LED_MAP[temp/1000];
               _7seg_Buff[1] = LED_MAP[(temp %1000)/100];
               _7seg_Buff[2] = LED_MAP[(temp % 100)/10];
               _7seg_Buff[3] = LED_MAP[temp % 10];
               Call_Task_7seg();break;
            }
            case 3:{Call_Task_Button();
               if((Button_Flag.Run==0)&&(Button_Flag.Speed_Set==1)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Speed_Set=0;Stage=2;//chuyen stage
               }
               if((Button_Flag.Run==0)&&(Button_Flag.Speed_Set==0)&&(Button_Flag.Rev_Set==1)){  
                  Button_Flag.Rev_Set=0;Stage=3;
               }
               if((Button_Flag.Run==1)&&(Button_Flag.Speed_Set==0)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Run=0;Stage=4;
               }
               if((Button_Flag.Run==1)&&(Button_Flag.Speed_Set==1)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Run=0;Button_Flag.Speed_Set=0;Stage=1;
               }
               
               break;
            }
            case 4:Call_Task_Get_Speed();
               Put_Duty(RPM_Value,850);
               
               break;
         }
      
      Led_Run=0;Led_Speed_Set=0;Led_Rev_Set=0;
      }
      //------------------------------------------------------------------------
      //hien thi toc do dat. doc adc->toc do dat
      if(Stage==2){
         Task_Array[0]=Task_OS_Temp;
         Task_Array[1]=Task_Led;
         Task_Array[2]=Task_7seg;
         Task_Array[3]=Task_Button;
         switch(select_task(4)){
            case 0:Call_Task_OS_Temp();break;
            case 1:Call_Task_Led();break;
            case 2:{
               RPM_Set=ADC_Read();RPM_Set=(unsigned long)(RPM_Set/1023.0*RPM_Max);
               _7seg_Buff[0] = LED_MAP[RPM_Set/1000];
               _7seg_Buff[1] = LED_MAP[(RPM_Set %1000)/100];
               _7seg_Buff[2] = LED_MAP[(RPM_Set % 100)/10];
               _7seg_Buff[3] = LED_MAP[RPM_Set % 10];
               Call_Task_7seg();break;
            }
            case 3:{Call_Task_Button();
               if((Button_Flag.Run==0)&&(Button_Flag.Speed_Set==1)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Speed_Set=0;Stage=2;//chuyen stage
               }
               if((Button_Flag.Run==0)&&(Button_Flag.Speed_Set==0)&&(Button_Flag.Rev_Set==1)){  
                  Button_Flag.Rev_Set=0;Stage=3;
               }
               if((Button_Flag.Run==1)&&(Button_Flag.Speed_Set==0)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Run=0;Stage=4;
               }
               if((Button_Flag.Run==1)&&(Button_Flag.Speed_Set==1)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Run=0;Button_Flag.Speed_Set=0;Stage=1;
               }
               break;
            }          
         }
      Disable_PWM_Modul();
      Led_Speed_Set=1;Led_Rev_Set=0;Led_Run=0;//Led dat toc do sang
         
      }
      //-----------------------------------------------------------------------
      //stage 3: dat so vong se chay, hien thi so vong se chay
      if(Stage==3){
         Task_Array[0]=Task_OS_Temp;
         Task_Array[1]=Task_Led;
         Task_Array[2]=Task_7seg;
         Task_Array[3]=Task_Button;
         switch(select_task(4)){
            case 0:Call_Task_OS_Temp();break;
            case 1:Call_Task_Led();break;
            case 2:{
               Rev_Gonna_Run=ADC_Read();
               Rev_Gonna_Run=Rev_Gonna_Run>>2;
               //Rev_Gonna_Run=Rev_Gonna_Run*4;
               if(Rev_Gonna_Run>150) Rev_Gonna_Run=9999;
               else{
                  Pulse_Gonna_Run=Rev_Gonna_Run*131.7647;
                  Pulse_Gonna_Run_temp=(unsigned long)Pulse_Gonna_Run;
               }
               Pulse_Count=0;
               _7seg_Buff[0] = LED_MAP[Rev_Gonna_Run/1000];
               _7seg_Buff[1] = LED_MAP[(Rev_Gonna_Run %1000)/100];
               _7seg_Buff[2] = LED_MAP[(Rev_Gonna_Run % 100)/10];
               _7seg_Buff[3] = LED_MAP[Rev_Gonna_Run % 10];
               Call_Task_7seg();break;
            }
            case 3:{Call_Task_Button();
               if((Button_Flag.Run==0)&&(Button_Flag.Speed_Set==1)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Speed_Set=0;Stage=2;//chuyen stage
               }
               if((Button_Flag.Run==0)&&(Button_Flag.Speed_Set==0)&&(Button_Flag.Rev_Set==1)){  
                  Button_Flag.Rev_Set=0;Stage=3;
               }
               if((Button_Flag.Run==1)&&(Button_Flag.Speed_Set==0)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Run=0;Stage=4;
               }
               if((Button_Flag.Run==1)&&(Button_Flag.Speed_Set==1)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Run=0;Button_Flag.Speed_Set=0;Stage=1;
               }
               break;
            }          
      }
      Disable_PWM_Modul();
      Led_Speed_Set=0;Led_Rev_Set=1;Led_Run=0;//Led dat so vong sang
      }
      //------------------------------------------------------------------------
      //stage 4: Run hien thi toc do do dc,PWM,dem so vong con lai
      if(stage==4){
         Task_Array[0]=Task_OS_Temp;
         Task_Array[1]=Task_Led;
         Task_Array[2]=Task_7seg;
         Task_Array[3]=Task_Button;
         Task_Array[4]=Task_Get_Speed;
         switch(select_task(5)){
            case 0:Call_Task_OS_Temp();break;
            case 1:Call_Task_Led();break;
            case 2:{unsigned long temp=RPM_Value_temp/i;
               _7seg_Buff[0] = LED_MAP[temp/1000];
               _7seg_Buff[1] = LED_MAP[(temp %1000)/100];
               _7seg_Buff[2] = LED_MAP[(temp % 100)/10];
               _7seg_Buff[3] = LED_MAP[temp % 10];
               Call_Task_7seg();break;
            }
            case 3:{Call_Task_Button();
               if((Button_Flag.Run==0)&&(Button_Flag.Speed_Set==1)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Speed_Set=0;Stage=2;//chuyen stage
               }
               if((Button_Flag.Run==0)&&(Button_Flag.Speed_Set==0)&&(Button_Flag.Rev_Set==1)){  
                  Button_Flag.Rev_Set=0;Stage=3;
               }
               if((Button_Flag.Run==1)&&(Button_Flag.Speed_Set==0)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Run=0;Stage=4;
               }
               if((Button_Flag.Run==1)&&(Button_Flag.Speed_Set==1)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Run=0;Button_Flag.Speed_Set=0;Stage=1;
               }
               if((Button_Flag.Run==0)&&(Button_Flag.Speed_Set==1)&&(Button_Flag.Rev_Set==1)){  
                  Button_Flag.Speed_Set=0;Button_Flag.Rev_Set=0;Stage=5;
               }
               break;
            }
            case 4:{Call_Task_Get_Speed();
                  Put_Duty(RPM_Value,RPM_Set);
                  if(Rev_Gonna_Run<=400){
                     if(Pulse_Count>Pulse_Gonna_Run_temp){/*Pulse_Count=0;*/ Button_Flag.Rev_Set=1;}
                  }
                  
                  break;
            }
         }
         
         Led_Speed_Set=0;Led_Rev_Set=0;Led_Run=1;//Led dat toc do sang
      }
      //---------------------------------------------------------------------------
     //stage 5: show adc
     if(Stage==5){
         Task_Array[0]=Task_OS_Temp;
         Task_Array[1]=Task_Led;
         Task_Array[2]=Task_7seg;
         Task_Array[3]=Task_Button;
         Task_Array[4]=Task_Get_Speed;
         switch(select_task(5)){
            case 0:Call_Task_OS_Temp();break;
            case 1:Call_Task_Led();break;
            case 2:{
               unsigned long temp=ADC_Read();
               
               _7seg_Buff[0] = LED_MAP[temp/1000];
               _7seg_Buff[1] = LED_MAP[(temp %1000)/100];
               _7seg_Buff[2] = LED_MAP[(temp % 100)/10];
               _7seg_Buff[3] = LED_MAP[temp % 10];
               Call_Task_7seg();break;
            }
            case 3:{Call_Task_Button();
               if((Button_Flag.Run==0)&&(Button_Flag.Speed_Set==1)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Speed_Set=0;Stage=2;//chuyen stage
               }
               if((Button_Flag.Run==0)&&(Button_Flag.Speed_Set==0)&&(Button_Flag.Rev_Set==1)){  
                  Button_Flag.Rev_Set=0;Stage=3;
               }
               if((Button_Flag.Run==1)&&(Button_Flag.Speed_Set==0)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Run=0;Stage=4;
               }
               if((Button_Flag.Run==1)&&(Button_Flag.Speed_Set==1)&&(Button_Flag.Rev_Set==0)){  
                  Button_Flag.Run=0;Button_Flag.Speed_Set=0;Stage=1;
               }
               break;
            }
            case 4:{Call_Task_Get_Speed();
                  Put_Duty(RPM_Value,RPM_Set);
                  if(Rev_Gonna_Run<=400){
                     float temp=Rev_Gonna_Run*131.7647;
                     if(Pulse_Count>(unsigned long)temp){Pulse_Count=0; stage=2;}
                  }
                  break;
            }
         }
      
      Led_Run=1;Led_Speed_Set=1;Led_Rev_Set=0;
      }
      //------------------------------------------------------------------------
      
   
   
   }
}
//******************************************************************************
//  Tasks
//******************************************************************************
/*Task 1:nhap nhay led*/
void Call_Task_Led (void)
{  //static unsigned char Count_Line=0;
    
    for (;;)
    {   
        Led=~Led;
        Task_Led_Timer=500;Task_Led.Piority=0;
        break;
    }
}
/*Task 2:Hien thi 7seg*/
void Call_Task_7seg (void){
   //step 1 la hien thi ky tu so 1
   for(;;){
      digit1=0;digit2=0;digit3=0;digit4=0;
      if(Task_7seg.Step<=1){
         PORTB=_7seg_Buff[0];digit1=1;
         Task_7seg_Timer=1;
         Task_7seg.Step=2;Task_7seg.Piority=0;break;
      }
      if(Task_7seg.Step==2){
         PORTB=_7seg_Buff[1];digit2=1;
         Task_7seg_Timer=1;
         Task_7seg.Step=3;Task_7seg.Piority=0;break;
      }
      if(Task_7seg.Step==3){
         PORTB=_7seg_Buff[2];digit3=1;
         Task_7seg_Timer=1;
         Task_7seg.Step=4;Task_7seg.Piority=0;break;
      }
      if(Task_7seg.Step>=4){
         PORTB=_7seg_Buff[3];digit4=1;
         Task_7seg_Timer=1;
         Task_7seg.Step=1;Task_7seg.Piority=0;break;
      }
   }

}
/*Task 3: Quet Button
   nut nao dc bam thi set flag cua button do =1
*/
void Call_Task_Button (void){
   static struct Flag1 temp;
   for(;;){
      if(Task_Button.Step==1){
         if((RA1==1)||(RA2==1)||(RA3==1)){
            Task_Button_Timer=20;
            Task_Button.Step=2;Task_Button.Piority=0;break;//step 2 la khi nguoi su dung giu button
         }else{
            Task_Button_Timer=1;
            Task_Button.Piority=0;break; 
         }
      }
      if(Task_Button.Step==2){
         if(RA1==1) temp.Run=1;
         if(RA2==1) temp.Speed_Set=1;
         if(RA3==1) temp.Rev_Set=1;
         if((RA1==0)&&(RA2==0)&&(RA3==0)){//neu ko giu nut bam nua thi sang step3 la khi nguoi su dung nha nut bam
            Task_Button_Timer=20;
            Task_Button.Step=3;Task_Button.Piority=0;break;
         }else{
            Task_Button_Timer=1;//delay 1tick
            Task_Button.Piority=0;break;
         }
      }
      if(Task_Button.Step==3){
         //reset co tam, set flag
         if(temp.Run==1){  Button_Flag.Run=1;temp.Run=0;}
         if(temp.Speed_Set==1){ Button_Flag.Speed_Set=1;temp.Speed_Set=0;}
         if(temp.Rev_Set==1){Button_Flag.Rev_Set=1;temp.Rev_Set=0;}
         //quay lai step 1
         Task_Button_Timer=1;//delay 1tick
         Task_Button.Step=1;Task_Button.Piority=0;break;
      }
   }
}
/*Task 4: Dieu khien dong co. Dung RC0(Counter), RC2(PWM). Dem xung trong 100tick=162,8ms
   Neu Run==1 thi cho dieu xung PWM. Run==0 thi chi dem toc do quay
*/
void Call_Task_Get_Speed (void){
   for(;;){
      //step 1: bat dau qua trinh counter
      if(Task_Get_Speed.Step==1){
         TMR1H=0;TMR1L=0;TMR1ON=1;//run timer1;
         Task_Get_Speed_Timer=10;
         Task_Get_Speed.Step=2;Task_Get_Speed.Piority=0;break;
      }
      //step 2: doc ra gia tri Counter va tinh toan
      if(Task_Get_Speed.Step==2){
         TMR1ON=0;//stop timer1;
         if(Rev_Gonna_Run<=400)  Pulse_Count+=TMR1H*256+TMR1L;
         else Pulse_Count=0;
         //RPM_Value=(unsigned long)((TMR1H*256+TMR1L)*1.047/PPR*850);//doc toc do RPM=Count*60000/16.38*850/3500/PPR
         RPM_Value=(unsigned long)((TMR1H*256+TMR1L)*1.047/PPR*850);
         Task_Get_Speed.Step=1;
         //hien thi gia tri trung binh cua 60 lan do
         i++;RPM_Value_temp+=RPM_Value;
         if(i>=60){i=1;RPM_Value_temp=RPM_Value;}
      }
   
   }
}
/*Tinh toan gia tri PWM su dung de bam xung PWM
*/
void Put_Duty(unsigned int16 Process_Value,unsigned long Set_Point){
   static float Error;
   static float Old_Error_G=0;
   static float Sum_G=0;
   static float Control_New;
   unsigned int16 PID_Value;
   float temp=(float)Process_Value;
   float temp2=(float)Set_Point;
   static float I_Max=50000;
   static float I_Min=-50000;   
   Error=(float)(temp2-temp);
   RPM_Value=(unsigned int16)temp;
   Control_New += 0.9*(Error);
   Sum_G += Error;
   Control_New += (Sum_G/50.0);
   Control_New += ((Error - Old_Error_G)*1.3);
   if(Sum_G>I_MAX) Sum_G=I_Max;
   else if(Sum_G<I_MIN) Sum_G=I_Min;
   if (Control_New>=PID_MAX){
      Control_New = PID_MAX;
   }
   else{
      if(Control_New<=PID_MIN){
      Control_New=PID_MIN;
      }
   }
   Old_Error_G = Error;
   PID_Value=(unsigned int16)(Control_New);
   PID_Value=1023-PID_Value;
   
   //set PWM duty
   //lay 2 bit thap cua PWM
   //CCP1Y=PID_Value&&0x01;CCP1X=PID_Value&&0x02;
   //lay 8 bit cao cua PWM
   //PID_Value=PID_Value>>2;CCPR1L=PID_Value;
   // CCPR1L=0x01;CCP1Y=0;CCP1X=0;
   set_pwm1_duty(PID_Value);
   Enable_PWM_Modul();
}
unsigned long ADC_Read(void){
   ADC_GO=1;
   while(ADC_GO==1){
      unsigned char temp2;
      unsigned char temp=0;
      for(temp2=0;temp2<50;temp2++);//delay 10us
      temp++;
      if(temp>10) break;//delay toi da 100us
   }
   return (ADRESH*256+ADRESL);
      
}

