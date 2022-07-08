#include "avrunit.h"
#include "trace.h"
#include "led.h"


//#define LED_PIN PB5
//void initLED(void) {
//    DDRB |= _BV(LED_PIN);
//}
//
//void toggleLED(void) {
//    PORTB ^= _BV(LED_PIN);
//}

static int test_ignore(void) {
    mu_ignored(0);
    mu_end();
}
static int test_led(void) {
    mu_assert(PB5 == LED_PIN);
    mu_end();
}

int main (void) {
    TRACE_init();

    mu_run_test(test_ignore);
    mu_run_test(test_led);
    int r = mu_tests_run;
    int f = mu_stat.fail;
    int b = mu_tests_broken;
    int i = mu_tests_ignore;
    T("Tests #=%d  FAIL=%d BROKEN=%d IGNORED=%d", r, f, b, i);
    return (0);
}
