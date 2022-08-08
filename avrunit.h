/*
 * avrunit.h Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com
 *
 * A unit test framework for AVR based microcontroller
 *
 */

#ifndef _AVRUNIT_H
#define _AVRUNIT_H

#include <avr/io.h>
#include <avr/eeprom.h>

#ifndef AU_EEPROM_START
#define AU_EEPROM_START 0
#endif

#define AU_MAX_TEST 10
#define AU_F_PASS 0
#define AU_F_FAIL 1
#define AU_F_BROKEN 2
#define AU_F_IGNORE 3
#define AU_F_SIZE 4

typedef struct {
    uint8_t flag;
    uint8_t id;
} stat_t;

stat_t AU_STAT[AU_MAX_TEST];
uint16_t AU_STAT_SIZE = 0;

#ifdef AU_SERIAL
#include <util/setbaud.h>

#define usart_txReady()       (bit_is_set(UCSR0A, UDRE0))
#define usart_rxReady()       (bit_is_set(UCSR0A, RXC0))
#define usart_txWait()        ({loop_until_bit_is_set(UCSR0A, UDRE0);})
#define usart_rxWait()        ({loop_until_bit_is_set(UCSR0A, RXC0);})
#define usart_txByte(data)    (UDR0 = data)
#define usart_rxByte()        (UDR0)

static void USART_init(void) {
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

static void usart_txString(char data[]) {
  uint8_t i = 0;
  while (data[i]) {
      usart_txWait();
      usart_txByte(data[i]);
      i++;
  }
}

static void au_output_serial(void) {
    USART_init();
}
#endif // AU_SERIAL

static void au_output_eeprom(void) {
    int addr = AU_EEPROM_START;
    eeprom_write_word((void *)addr, AU_STAT_SIZE);
    addr = addr + sizeof(AU_STAT_SIZE);
    eeprom_write_block(&AU_STAT, (void *)addr, sizeof(AU_STAT));
}

static void au_output(void) {
    au_output_eeprom();
#ifdef AU_SERIAL
    au_output_serial();
#endif // AU_SERIAL
}

#define AU_OUTPUT_SETUP()
#define AU_OUTPUT() au_output()
#define AU_ASSERT(TEST) do { if (!(TEST)) { return AU_F_FAIL; } } while (0)
#define AU_BROKEN(TEST) do { return AU_F_BROKEN; } while (0)
#define AU_IGNORED(TEST) do { return AU_F_IGNORE; } while (0)

#define AU_UNIT_START
#define AU_UNIT_END do { return AU_F_PASS; } while(0)

#define AU_RUN_TEST(ID, TEST_CASE, ...) do { \
    AU_STAT[AU_STAT_SIZE].flag = TEST_CASE(__VA_ARGS__); \
    AU_STAT[AU_STAT_SIZE].id = ID; \
    AU_STAT_SIZE += 1; \
} while (0)

#endif // _AVRUNIT_H
