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

static int test_broken(void) {
    mu_broken(0);
    mu_end();
}

static int test_fail(void) {
    mu_assert(1 == 0);
    mu_end();
}

static int test_led(void) {
    mu_assert(PB5 == LED_PIN);
    mu_end();
}

int main (void) {
    TRACE_init();

    mu_run_test(test_ignore);
    mu_run_test(test_broken);
    mu_run_test(test_fail);
    mu_run_test(test_led);

    int r = mu_stat.run;
    int f = mu_stat.fail;
    int b = mu_stat.broken;
    int i = mu_stat.ignore;
    int p = r-f-b-i;
    T("Tests #=%d  P=%d F=%d B=%d I=%d", r, p, f, b, i);
    return (0);
}
