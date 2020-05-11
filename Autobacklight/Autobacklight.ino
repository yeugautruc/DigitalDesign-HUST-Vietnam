
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
int val;
int temp,i;
int back = 13;
void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("AUTO BACKLIGHT");
  pinMode(back, OUTPUT);  
}
void loop() {
  temp = 0;
 for(i=1;i<10;i++)
  {
  temp= analogRead(0);
  temp=+temp;
  val= temp/9;
  }
  
  //val= analogRead(0);
  delay(10);
  if ( val >= 1){digitalWrite(back, LOW);}
  else digitalWrite(back,HIGH);
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print("Light value:");
  lcd.print(val);
}


