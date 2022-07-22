#ifndef _LED_H
#define _LED_H

#include <avr/io.h>

#define LED_PIN PB5

void initLED(void);
void toggleLED(void);

#endif //_LED_H
