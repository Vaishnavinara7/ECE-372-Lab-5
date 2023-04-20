#include "timer.h"
//You many use any timer you wish for the microsecond delay and the millisecond delay


/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
    TCCR1A &= ~( (1 << WGM10) | ( 1<< WGM11));
    TCCR1B |= ( 1 << WGM12);
    TCCR1B &= ~ ( 1 << WGM13); 
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayMs(unsigned int delay){
TCNT1 = 0;

// set outout compare value
// we used the formula OCR0A = [Td * fclk]/ PS
// using a PS = 8, Td = 1us fclk = 16MHz.
// OCR1A = 2  - 1 to acccount for starting at bit 0.
// if we want to pass a value called delay then we can set OCR0A = 1 * delay
// andthat should allow values in us to be delayed by the right time up to at least 250us.
OCR1A = 62 * delay;

// set output compare flag  TIFR0 down by writing a logic 1
TIFR1 |= (1 << OCF1A);

// turn on clock with the CS bits and start counting
// Use Prescaleer of 8 (2 counts is  1 us)

TCCR1B |= (1 << CS12);
TCCR1B &= ~((1 << CS11)| (1 << CS10));

// poll the flag OCF1A bit to see when it is raised
// while the flag bit OCF1A is down , do nothing
while (( TIFR1 & ( 1 << OCF1A)) == 0;

// exit out we have our delay required
// turn off clock
TCCR1B &= ~( ( 1 << CS12) | ( 1 << CS11) | (1 << CS10));
}
