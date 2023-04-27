#include <avr/io.h>
#include "Arduino.h"
#include "i2c.h"

#define wait_for_completion while(!(TWCR &amp; (1 << TWINT)));

void initi2c(){
 
  PRR0 &= ~(1<<PRTWI);  // wake up I2C module on AT2560 power management register
  
  TWSR &= ~(1 << TWPS0);  // prescaler = 1
  TWSR &= ~(1 << TWPS1); //

  //𝑇𝑊𝐵𝑅=((𝐶𝑃𝑈 𝐶𝑙𝑜𝑐𝑘 𝑓𝑟𝑒𝑞𝑢𝑒𝑛𝑐𝑦)/(𝑆𝐶𝐿 𝑓𝑟𝑒𝑞𝑢𝑒𝑛𝑐𝑦)−16)/(2∗〖(4)〗^𝑇𝑊𝑃𝑆 )
  TWBR = 0xC6; // bit rate generator = 10k  (TWBR = 198)

  TWCR |= (1 << TWINT )| (1 << TWEN); // enable two wire interface
}

void startI2C_Trans(unsigned char SLA){
    //Clear TWINT, initiate start condition, initiate enable
  TWCR = ((1 << TWEN) | (1 << TWINT) | (1 << TWSTA));
  wait_for_completion;

  TWDR = (SLA << 1); // slave address + write bit '0'

  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action:clear flag and enable TWI
  wait_for_completion;
}

void stopI2C_Trans(){
  // this function sends a stop condition to stop I2C transmission

  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //trigger action:  send stop condition

}

int *Read_Acc(unsigned char SLA, unsigned char MEMADDRESS){
  // this function sets up reading from SLA at the SLA MEMADDRESS 
 static signed int accelerometer_axes[3];
 signed int accelerometer_x;
 signed int accelerometer_y;
 signed int accelerometer_z;
  StartI2C_Trans(SLA);
 
  write(MEMADDRESS);
  
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // restart to switch to read mode
  wait_for_completion;
  TWDR = (SLA << 1) | 0x01; // 7 bit address for slave plus read bit
  TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// trigger with master sending ack
  wait_for_completion;
 
  TWCR = (1<< TWINT) | (1 << TWEN)| (1 << TWEA);  // master can send a nack now
  wait_for_completion;
  accelerometer_x = TWDR;
 
  TWCR = (1<< TWINT) | (1 << TWEN)| (1 << TWEA);  // master can send a nack now
  wait_for_completion;
  accelerometer_x = (TWDR<<8);
 
  TWCR = (1<< TWINT) | (1 << TWEN)| (1 << TWEA);  // master can send a nack now
  wait_for_completion;
  accelerometer_y = TWDR;
 
   TWCR = (1<< TWINT) | (1 << TWEN)| (1 << TWEA);  // master can send a nack now
  wait_for_completion;
  accelerometer_y = (TWDR<<8);
 
   TWCR = (1<< TWINT) | (1 << TWEN)| (1 << TWEA);  // master can send a nack now
  wait_for_completion;
  accelerometer_z = TWDR;
 
    TWCR = (1<< TWINT) | (1 << TWEN)| (1 << TWEA);  // master can send a nack now
  wait_for_completion;
  accelerometer_z = (TWDR<<8);
 
 accelerometer_axes[0] = accelerometer_x;
 accelerometer_axes[1] = accelerometer_y;
 accelerometer_axes[2] = accelerometer_z;
 
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Stop condition
// after this function is executed the TWDR register has the data from SLA that Master wants to read
 return accelerometer_axes;
}

//unsigned char Read_data() // Returns the last byte from the data register
//{
//  return TWDR;
//}
