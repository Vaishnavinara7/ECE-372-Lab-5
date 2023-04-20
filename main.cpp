// Header and source files
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include "SPI.h"
#include "i2c.h"

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


int main(void){
Serial.begin(9600);   // Serial (for debugging)
  
  // VARIABLES
  //???

  // HARDWARE INITIALIZATIONS
  initSwitchPD0();      // Initialize the switch
  SPI_MASTER_Init();    //Initialize the SPI module
  initPWMTimer4();      // Initialize timer 4 for PWM mode 15
  initTimer1();         // Initialize timer 1 (for millisecond delay)
  initI2C();            // Initialize I2C bus
  sei();                // Enable global interrupts
  
  // SETUP I2C
  signed int X_val = 0;
  signed int Y_val = 0;
  signed int Z_val = 0;
  StartI2C_Trans(SLA);
  write(PWR_MGMT);// address on SLA for Power Management
  write(WAKEUP);  // send data to Wake up from sleep mode
  StopI2C_Trans();
  
  while(1){
    
    
  Read_from(SLA,SL_MEMA_XAX_HIGH );
  X_val= Read_data(); // read upper value
  Read_from(SLA,SL_MEMA_XAX_LOW);
  X_val = (X_val << 8 )| Read_data(); // append lower value
  StopI2C_Trans();

  Read_from(SLA,SL_MEMA_YAX_HIGH );
  Y_val= Read_data(); // read upper value
  Read_from(SLA,SL_MEMA_YAX_LOW);
  Y_val = (Y_val << 8 )| Read_data(); // append lower value
  StopI2C_Trans();

  Read_from(SLA,SL_MEMA_ZAX_HIGH );
  Z_val= Read_data(); // read upper value
  Read_from(SLA,SL_MEMA_ZAX_LOW);
  Z_val = (Z_val << 8 )| Read_data(); // append lower value
  StopI2C_Trans();
    
  }
  
  
  
}
