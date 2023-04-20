#include <avr/io.h>

#include "pwm.h"
void initPWMTimer4(){
  
  //table 17-2, we need fast PWM, mode 15 for variable frequency
TCCR4A |= (1<<WGM40) | (1<<WGM41);
TCCR4B |= (1<<WGM42) | (1<<WGM43);
// table 17-4, Fast PWM non-inverting mode
TCCR4A &= ~(1<<COM4C0);
TCCR4A |= (1<<COM4C1);

//table 17-6, set prescalerto 1
TCCR4B |= (1<<CS40);
TCCR4B &= ~(1 << CS41);
TCCR4B &= ~(1 << CS42);

//Set Pin 8 on board to output, which is OC4C, PORTH5, PWM
DDRH |= (1<<DDH5); 

void change_frequency(unsigned int freq_val) {
   // To vary the frequency, we need to to change OCR4A (the TOP value)
   OCR4A = 16000000/freq_val;  // (ignoring the 1 in the formula)
}

void change_duty_cycle(double duty_val) {
    // To vary the duty cycle, we need to to change OCR4C (the COMPARE value)
    OCR4C = OCR4A * duty_val;
}
