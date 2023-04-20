// Authors: Nawaf ALmutairi, Vaishnavi Narayanan
// Date:04/20/2023
// Assignment: Lab 5
//
#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPB3(){

DDRB &= ~(1 << DDB3);
PORTB |= (1 << PORTB3);


PCICR |= (1 << PCIE0); // enable PCINT 0 - 7
PCMSK0 |= (1 << PCINT3); // enable PCINT3
}
