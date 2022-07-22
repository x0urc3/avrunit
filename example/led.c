#include "led.h"

void initLED(void) {
    DDRB |= _BV(LED_PIN);
}

void toggleLED(void) {
    PORTB ^= _BV(LED_PIN);
}

