#include "avrunit.h"
//#include "trace.h"
#include "led.h"
#ifndef BAUD
#define BAUD  9600              // Set a safe default baud rate
#endif
#include <util/setbaud.h>


//#define LED_PIN PB5
//void initLED(void) {
//    DDRB |= _BV(LED_PIN);
//}
//
//void toggleLED(void) {
//    PORTB ^= _BV(LED_PIN);
//}

static int test_ignore(void) {
    AU_UNIT_START;

    AU_IGNORED(0);

    AU_UNIT_END;
}

static int test_broken(void) {
    AU_UNIT_START;

    AU_BROKEN(0);

    AU_UNIT_END;
}

static int test_fail(void) {
    AU_UNIT_START;

    AU_ASSERT(1 == 0);

    AU_UNIT_END;
}

static int test_led(void) {
    AU_UNIT_START;

    AU_ASSERT(PB5 == LED_PIN);

    AU_UNIT_END;
}

AU_SETUP;

static void mu_output_setup(void) {
  UBRR0H = UBRRH_VALUE;         // defined in setbaud.h & requires BAUD
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~_BV(U2X0);
#endif

  UCSR0B = _BV(TXEN0) | _BV(RXEN0);     // Enable USART transmitter/receiver
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);   // 8 data bits, 1 stop bit
}

#define usart_txWait() (loop_until_bit_is_set(UCSR0A, UDRE0);)
#define usart_txByte(data) (UDR0 = data)

static void usart_txString(char data[]) {
  uint8_t i = 0;
  while (data[i]) {
      usart_txWait();
      usart_txByte(data[i]);
      i++;
  }
}

static void mu_output(void) {
    int r = AU_STAT.run;
    int f = AU_STAT.fail;
    int b = AU_STAT.broken;
    int i = AU_STAT.ignore;
    int p = r-f-b-i;
//    T("Tests #=%d  P=%d F=%d B=%d I=%d\n", r, p, f, b, i);
}

int main (void) {
    AU_OUTPUT_SETUP;

    AU_RUN_TEST(0x00, test_ignore);
    AU_RUN_TEST(0x01, test_broken);
    AU_RUN_TEST(0x02, test_fail);
    AU_RUN_TEST(0xa0, test_led);

    AU_OUTPUT;

    return (0);
}
