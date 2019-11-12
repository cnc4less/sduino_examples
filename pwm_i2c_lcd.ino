/*
  This program makes a easy to control PWM 970Hz signal.
  It needs a Pontentiometer, LCD with I2C module(PCF8574) and a LED with 1kR.
  
  created 12 Nov. 2019
  by Marcos Vinicius
  
  Sduino Library made by
  tehbaht - https://tenbaht.github.io/sduino/
  All credits to him.

*/
//necessary for use of LCD, change if needed
#include <I2C.h>
#include <LiquidCrystal_I2C.h>
#define LCD_ADDR 0x27
LiquidCrystal_I2C(lcd, 0x27, 16, 2);

// Pins association 
const int Pot_Read_Pin = A0;  // Analog input for Potentiometer
const int Pwm_Out_Pin = 13; // PWM output for LED
//Global Variables
int Pot_Read = 0;        
int Pwm_Out = 0;        
int line1_duty = 0;
int line2_adc = 0;

//Average Function
int average_adc(int avg_number,int analog_pin){
  int buffer_array[128]; //Buffer data array
  int long calculated = 0; //Average variable 
  int i=0;   
  for (i = 0; i<avg_number; i++)   //Reading Loop
    {      
    buffer_array[i] = analogRead(analog_pin); //Save to array
    calculated += buffer_array[i]; //Sums to array
    delay(2);
    }
  calculated = calculated/avg_number; //Average calc
  return (calculated); //return Average value calculated
}
//LCD Clear Function
void update_lcd(int x,int y)
{
  lcd_setCursor(x, y);
  lcd_print_s(" ");  
}
//SETUP
void setup() {  
  //Initialize the LCD
  lcd_begin();
  lcd_backlight();
  lcd_clear();
  //Fixed messages
  lcd_setCursor(4, 1);
  lcd_print_s("% Duty+");
  lcd_setCursor(4, 0); 
  lcd_print_s(" AVG ADC");
}
//LOOP
void loop() {    
  int duty_cycle = 0;
  Pot_Read = average_adc(128,Pot_Read_Pin); //Reads adc and returns its average
  Pwm_Out = map(Pot_Read, 0, 1023, 0, 255); //Range transform from 1023 to 255
  if(Pot_Read <8) //Protection for <1% cicle 
  {
    Pwm_Out = 1;  //Send PWM with close to 1% Duty Cycle
  }
  if(Pot_Read>1020) //Protection for >99% cicle
  {
    Pwm_Out = 254; //Send PWM with close to 99% Duty Cycle
  }  
  analogWrite(Pwm_Out_Pin, Pwm_Out); //PWM Generator
  duty_cycle = (Pwm_Out*100)/255; //Show percentage of duty+  
  if (duty_cycle<10)  //lcd DUTY Clearing
    { 
    update_lcd(1,1);
    }    
  lcd_setCursor(0, 1);
  lcd_print_u(duty_cycle); //Prints DUTY Reads
  if (Pot_Read<1000)  //lcd ADC Clearing
    { 
      update_lcd(3,0);
      if (Pot_Read<100)
        {
          update_lcd(2,0); 
          if (Pot_Read<10)
            {
             update_lcd(1,0); 
            } 
        }      
    }    
  lcd_setCursor(0, 0);
  lcd_print_u(Pot_Read); //Prints ADC Reads
  delay(10);      //optional
}
