#include <Arduino.h>
#include "SPI.h"
#include <avr/interrupt.h>
#include <avr/io.h>



int main () {
int apple;
  Serial.begin(9600);  // use serial port for print in debug
  sei();
   SPI_MASTER_Init(); // initialize SPI module and set the data rate

// initialize 8 x 8 LED array (info from MAX7219 datasheet)
write_execute(0x0A, 0x08);  // brightness control
write_execute(0x0B, 0x07); // scanning all rows and columns
write_execute(0x0C,0x01); // set shutdown register to normal operation (0x01)
write_execute(0x0F, 0x00); // display test register - set to normal operation (0x01)

while(1){

// write "HI" to LED array.  Must address all 8 rows.  
//Each row has data for individual LED:   "1" LED is on, "0" LED is off
if( apple <= 1 ){
write_execute(0x01, 0b00000000); // all LEDS in Row 1 are off
write_execute(0x02, 0b00111000); // row 2 LEDS 
write_execute(0x03, 0b01101100); // row 3 LEDS
write_execute(0x04, 0b11000110); // row 4 LEDS
write_execute(0x05, 0b10111010); // row 5 LEDS
write_execute(0x06, 0b01111100); // row 6 LEDS
write_execute(0x07, 0b00000000); // row 7 LEDS
write_execute(0x08, 0b00000000); // row 8 LEDS
}
_delay_ms(1000);  // delay for 1 s to display "HI"

/// write "ALL" to LED array.  Must address all 8 rows.  
//Each row has data for individual LED:   "1" LED is on, "0" LED is off
else{
write_execute(0x01, 0b00000000); // all LEDS in Row 1 are off
write_execute(0x02, 0b00111000); // row 2 LEDS 
write_execute(0x03, 0b01101100); // row 3 LEDS
write_execute(0x04, 0b10111010); // row 4 LEDS
write_execute(0x05, 0b11000110); // row 5 LEDS
write_execute(0x06, 0b01111100); // row 6 LEDS
write_execute(0x07, 0b00000000); // row 7 LEDS
write_execute(0x08, 0b00000000); // row 8 LEDS
}

_delay_ms(2000); // delay for 2 sec to display "ALL"
}

}
