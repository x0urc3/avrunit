#include "led.h"
#include <util/delay.h>

int main(void) {

    initLED();

    while (1) {
        toggleLED();
        _delay_ms(1000);
    }

    return(0);
}
