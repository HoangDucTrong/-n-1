#include <18f4550.h>
#fuses HSPLL, NOWDT, NOPROTECT, NOLVP, PLL5
#use delay(clock=20M)
#use i2c(Master,Fast=100000, sda=PIN_B0, scl=PIN_B1, STREAM=DS1307_STREAM)
#include "i2c_lcd.c"
#include <ds1307.c>
#define DATA_PIN   PIN_E2 
#define CLOCK_PIN  PIN_E0  
#define LATCH_PIN  PIN_D6  
#define DHT22_PIN  PIN_D5
const unsigned char seven_seg_digits[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
unsigned int8 second, minute, hour,  date,  month, year;
RTC_Time *mytime;
unsigned char digits[6];
void shiftOut(unsigned char data) {
    for (int i = 0; i < 8; i++) {
        output_bit(DATA_PIN, (data >> (7 - i)) & 0x01);
        output_high(CLOCK_PIN);
        delay_us(1);
        output_low(CLOCK_PIN);
    }
}
void updateDisplay() {
    output_low(LATCH_PIN);
    for (int i = 0; i < 6; i++) shiftOut(seven_seg_digits[digits[i]]);
    output_high(LATCH_PIN);
}
void updateTime() {
    hour = mytime->hours;
    minute = mytime->minutes;
    second = mytime->seconds;
    date = mytime->day;
    month = mytime->month;
    year = mytime->year;
    digits[0] = year % 10;
    digits[1] = year / 10;
    digits[2] = month % 10;
    digits[3] = month / 10;
    digits[4] = date % 10;
    digits[5] = date / 10;
    updateDisplay();
}
char Temperature[] = " 00.0 C";
char Humidity[]    = " 00.0 %";
int8 T_Byte1, T_Byte2, RH_Byte1, RH_Byte2, CheckSum;
int16 Temp, RH;
void Start_Signal(void) {
  output_drive(DHT22_PIN);                            
  output_low(DHT22_PIN);                              
  delay_ms(25);                                       
  output_high(DHT22_PIN);                             
  delay_us(50);                                       
  output_float(DHT22_PIN);                           
}
int1 Check_Response(void) {
  set_timer1(0);                                      
  while(!input(DHT22_PIN) && get_timer1() < 150);     
  if(get_timer1() >= 150)                             
    return 0;                                       
  else {
    set_timer1(0);                                   
    while(input(DHT22_PIN) && get_timer1() < 100);    
    if(get_timer1() >= 100)                          
      return 0;                                       
    else
      return 1;                                      
  }
}
int1 Read_Data(int8 *dht_data) {
  int8 j;
  *dht_data = 0;
  for(j = 0; j < 8; j++){
    set_timer1(0);                                    
    while(!input(DHT22_PIN))                          
      if(get_timer1() >= 100) {                       
        return 1;
      }
    set_timer1(0);                                    
    while(input(DHT22_PIN))                           
      if(get_timer1() > 100) {                        
        return 1;                                     
      }
     if(get_timer1() > 50)                            
       bit_set(*dht_data, (7 - j));                   
  }

  return 0;                                          
}
void readDHT22() {
    Start_Signal();               
    if(Check_Response()) {         
        Read_Data(&RH_Byte1);      
        Read_Data(&RH_Byte2);      
        Read_Data(&T_Byte1);       
        Read_Data(&T_Byte2);       
        Read_Data(&CheckSum);      
        if(CheckSum == ((RH_Byte1 + RH_Byte2 + T_Byte1 + T_Byte2) & 0xFF)) {

          RH = RH_Byte1;
          RH = (RH << 8) | RH_Byte2;
          Temp = T_Byte1;
          Temp = (Temp << 8) | T_Byte2;

          if( RH >= 1000)
            Humidity[0]  = '1';
          else
            Humidity[0]  = ' ';

          if(Temp > 0x8000) {
            Temperature[0] = '-';
            Temp = Temp & 0x7FFF;
          }
          else
          Temperature[0] = ' ';
          Temperature[1]  = (Temp / 100) % 10  + 48;
          Temperature[2]  = (Temp / 10) % 10  + 48;
          Temperature[4]  = Temp % 10  + 48;
          Humidity[1]     = (RH / 100) % 10 + 48;
          Humidity[2]     = (RH / 10) % 10 + 48;
          Humidity[4]     = RH % 10 + 48;
        }
        else {
          Temperature[0] = Temperature[1] = Temperature[2] = Temperature[4] = 'E';
          Humidity[0]    = Humidity[1]    = Humidity[2]    = Humidity[4]    = 'E';
        }
    }
    else {
      Temperature[0] = Temperature[1] = Temperature[2] = Temperature[4] = 'E';
      Humidity[0]    = Humidity[1]    = Humidity[2]    = Humidity[4]    = 'E';
    }
}
int mode;
int gio=0,phut=0,giay=0;
#int_rb
void setup()
{
      delay_ms(20);
      if(input(pin_B7)==0)
      {
         mode++;
         if(mode==6)  mode = 0;
         while(input(pin_B7)==0);
      }
      if(mode==3)
      {
         if(input(pin_B5)==0)
         {
            gio++;
            if(gio==24)  gio = 0;
            while(input(pin_B5)==0);
         }
         if(input(pin_B6)==0)
         {
            gio--;
            if(gio==-1)  gio = 23;
            while(input(pin_B6)==0);
         }
      }
      if(mode==4)
      {
         if(input(pin_B5)==0)
         {
            phut++;
            if(phut==60)  gio = 0;
            while(input(pin_B5)==0);
         }
         if(input(pin_B6)==0)
         {
            phut--;
            if(gio==-1)  gio = 59;
            while(input(pin_B6)==0);
         }
      }
}


void hengio()
{
   
   lcd_clear();
   lcd_gotoxy(1, 1);
   printf(lcd_putc, "Mode: 3");
   lcd_gotoxy(1, 2);
   printf(lcd_putc, "THOI GIAN BAO THUC");
   lcd_gotoxy(1, 3);
   printf(lcd_putc, "Time: %u : %u :%u    ", hour,minute,second);
   if(mode == 3)
   {
       lcd_gotoxy(1, 4);
       printf(lcd_putc,"Set: %d : %d : %d",gio,phut,giay);
       delay_ms(100);
       lcd_gotoxy(5, 4);
       printf(lcd_putc,"  ");
       delay_ms(100);
       
   }
   else if(mode == 4)
   {
      lcd_gotoxy(10, 4);
       printf(lcd_putc,"%d ",phut);
       delay_ms(100);
       lcd_gotoxy(10, 4);
       printf(lcd_putc,"  ");
       delay_ms(100);
   }
   else if(mode == 5)
   {
       lcd_gotoxy(1, 4);
       printf(lcd_putc,"Set: %d : %d : %d",gio,phut,giay);
       if(gio== hour && phut == minute) output_high(pin_c0); else output_low(pin_c0);
   }
   
}
void main() {
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_4);
    set_tris_b(0xFF);
    enable_interrupts(INT_RB);
    enable_interrupts(GLOBAL);
    lcd_init(0x70, 20, 4);
    Temperature[5] = 223;
    while (TRUE) {
    readDHT22();
    mytime = RTC_Get();
    if (mytime->hours > 23 || mytime->minutes > 59 || mytime->seconds > 59) {
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "RTC Err!");
      } 
    else {
            updateTime();   
      }
    if(mode==0)
    {
      
      lcd_gotoxy(1, 1);
      printf(lcd_putc, "Mode: %d   ", mode);
      lcd_gotoxy(1, 2);
      printf(lcd_putc, "Time: %u : %u :%u    ", hour,minute,second);
      lcd_gotoxy(1, 3);
      printf(lcd_putc, "    Le Ky Tuan");
      lcd_gotoxy(1, 4);
      printf(lcd_putc, "  Hoang Duc Trong");
      
     
    }
    else if(mode==1)
    {
      lcd_gotoxy(1, 1);
      printf(lcd_putc, "Mode: %d   ", mode);
      lcd_gotoxy(1, 2);
      printf(lcd_putc, "Nhiet Do: %s", Temperature);
      lcd_gotoxy(1, 3);
      printf(lcd_putc, "Do Am: %s", Humidity);
      lcd_gotoxy(1, 4);
      printf(lcd_putc, "                                         ");
      mytime = RTC_Get();
    }
    else
    {
      hengio();
    }
    }

}
