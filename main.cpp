// Example code using SPI transmission to write to 8x8 LED array through MAxim 7219 chip
// The 8x8 LED array is part number 1038AS and it is interfaced with the MAX7219CNG chip

#include <Arduino.h>
#include "SPI.h"
#include "i2c.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "timer.h"
#include "pwm.h"
#include "switch.h"

#define SLA 0x68  // MPU_6050 address with PIN AD0 grounded
#define PWR_MGMT  0x6B
#define WAKEUP 0x00
#define SL_MEMA_XAX_HIGH  0x3B
#define SL_MEMA_XAX_LOW   0x3C
#define SL_MEMA_YAX_HIGH  0x3D
#define SL_MEMA_YAX_LOW   0x3E
#define SL_MEMA_ZAX_HIGH  0x3F
#define SL_MEMA_ZAX_LOW   0x40
#define SL_TEMP_HIGH      0x41
#define SL_TEMP_LOW       0x42
#define ACCEL_CONFIG      0x1C

/*
 * Define a set of states that can be used in the state machine using an enum.
 */
typedef enum stateEnum{
    wait_press,
    bounce_low,
    wait_release,
    bounce_high
  } stateType;

// Initialize states.  Remember to use volatile 
volatile stateType state = wait_press;

typedef enum Alarm_reaction{
    NotB,
    NotBToB,
    B,
    BToNotB
  } AlarmType;

// Initialize states.  Remember to use volatile 
volatile AlarmType Alarm = NotB;

typedef enum face_reaction{
    Smiley,
    SmileyToFrowny,
    Frowny,
    FrownyToSmiley
  } FaceType;

// Initialize states.  Remember to use volatile 
volatile FaceType Face = Smiley;

int main () {
//initTimer1();
  Serial.begin(9600);  // use serial port for print in debug
  sei();
  SPI_MASTER_Init(); // initialize SPI module and set the data rate
  initI2C();  // initialize I2C and set bit rate
  int freq_val = 0;
  //freq_val = ADCL;
   //freq_val += ((unsigned int)ADCH)<<8;
 change_frequency(freq_val);
 initPWMTimer4();
  float X_C = 0;	
  float Y_C = 0;
  float Z_C = 0;
  signed int X_val = 0;
  signed int Y_val = 0;
  signed int Z_val = 0;
  signed int Ph = 2;
  StartI2C_Trans(SLA);
  write(PWR_MGMT);// address on SLA for Power Management
  write(WAKEUP); // send data to Wake up from sleep mode
  StopI2C_Trans();
// initialize 8 x 8 LED array (info from MAX7219 datasheet)
write_execute(0x0A, 0x08);  // brightness control
write_execute(0x0B, 0x07); // scanning all rows and columns
write_execute(0x0C,0x01); // set shutdown register to normal operation (0x01)
write_execute(0x0F, 0x00); // display test register - set to normal operation (0x01)


  while(1){
    Read_from(SLA, ACCEL_CONFIG);
     Ph = Read_data();
    Serial.println(Ph);
    //change_frequency(0);
     Read_from(SLA,SL_MEMA_XAX_HIGH);
    X_val= Read_data(); // read upper value
    Read_from(SLA,SL_MEMA_XAX_LOW);
    X_val = (X_val << 8 )| Read_data(); // append lower value
    X_C = (X_val / 16000.0)-0.05;
     Serial.print(" X =  ");
    Serial.print(X_C);

    Read_from(SLA,SL_MEMA_YAX_HIGH);
    Y_val= Read_data(); 
    Read_from(SLA,SL_MEMA_YAX_LOW);
    Y_val = (Y_val << 8 )| Read_data(); 
    Y_C = (Y_val / 16000.0)+0.0055;
   Serial.print("        Y = ");
    Serial.print(Y_C);

    Read_from(SLA,SL_MEMA_ZAX_HIGH);
    Z_val= Read_data(); 
    Read_from(SLA,SL_MEMA_ZAX_LOW);
    Z_val = (Z_val << 8 )| Read_data(); // append lower value
    Z_C = (Z_val / 16000.0)-0.743;
     Serial.print("        Z = ");
    Serial.println(Z_C);
   
   
    StopI2C_Trans();

  
if (Face == Smiley)
 {
  if (X_C > 0.8) {
    Face = SmileyToFrowny;
  //Alarm = NotBToB;
                  }
  }
else if (Face ==  Frowny)
  {
  if (X_C <= 0.8) {
    Face =FrownyToSmiley;
    //Alarm = BToNotB;
    
                 }
  }
    
if (Alarm == NotB)
 {
  if (X_C > 0.8) {
    
  Alarm = NotBToB;
                  }
  }
else if (Alarm ==  B)
  {
  if (X_C <= 0.8) {
  
    Alarm = BToNotB;
    
                 }
  }
  switch (Face) {
   case Frowny:
    break;
      case FrownyToSmiley:
        Face = Smiley;
  
          // write "HI" to LED array.  Must address all 8 rows.  
//Each row has data for individual LED:   "1" LED is on, "0" LED is off
write_execute(0x01, 0b00000000); // all LEDS in Row 1 are off
write_execute(0x02, 0b00010100); // row 2 LEDS 
write_execute(0x03, 0b00000000); // row 3 LEDS
write_execute(0x04, 0b00100010); // row 4 LEDS
write_execute(0x05, 0b00100010); // row 5 LEDS
write_execute(0x06, 0b00011100); // row 6 LEDS
write_execute(0x07, 0b00000000); // row 7 LEDS
write_execute(0x08, 0b00000000); // row 8 LEDS
//delayMs(1000);  // delay for 2 s to display "HI"

case Smiley:
 break;
case SmileyToFrowny:
  Face = Frowny;

write_execute(0x01, 0b00000000);
write_execute(0x02, 0b00010100);
write_execute(0x03, 0b00000000);
write_execute(0x04, 0b00011100);
write_execute(0x05, 0b00100010);
write_execute(0x06, 0b00100010);
write_execute(0x07, 0b00000000);
write_execute(0x08, 0b00000000);
//delayMs(1000); // delay for 2 sec to display "ALL"
  
  }
   switch (Alarm) {
   case B:
    break;
      case BToNotB:
        Alarm = NotB;
        
        
         change_frequency(0);
        //delayMs(1000);
        case NotB:
 break;
case NotBToB:
  Alarm = B;
   change_frequency(1);
   //delayMs(1000);
  }}
return 0;
  }
