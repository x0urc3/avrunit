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

#define AU_MAX_TEST 20
#define AU_F_PASS 0
#define AU_F_FAIL 1
#define AU_F_BROKEN 2
#define AU_F_IGNORE 3
#define AU_F_SIZE 4

typedef struct {
    uint8_t flag;
    uint8_t id;
} record_t;

typedef struct {
    uint16_t size;
    record_t record[AU_MAX_TEST];
} stat_t;

stat_t AU_STAT;

#ifdef AU_SERIAL
#include <util/setbaud.h>

#define USART_txReady()       (bit_is_set(UCSR0A, UDRE0))
#define USART_rxReady()       (bit_is_set(UCSR0A, RXC0))
#define USART_txWait()        ({loop_until_bit_is_set(UCSR0A, UDRE0);})
#define USART_rxWait()        ({loop_until_bit_is_set(UCSR0A, RXC0);})
#define USART_txByte(d)       (UDR0 = d)
#define USART_rxByte()        (UDR0)

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

static void au_wait_serial(uint8_t cmd) {
    uint8_t c;
    while (c != cmd) {
        USART_rxWait();
        c = USART_rxByte();
    }
}

static void au_send_serial(uint8_t *data, uint16_t size) {
    uint8_t *byte;
    for (byte = data; size > 0 ; size--, ++byte) {
        USART_txWait();
        USART_txByte(*byte);
    }
}

static void au_output_serial(void) {
    USART_init();
    au_wait_serial('r');
    au_send_serial((uint8_t *)&AU_STAT, sizeof(AU_STAT));

}
#endif // AU_SERIAL

#ifndef AU_SERIAL
static void au_output_eeprom(void) {
    int addr = AU_EEPROM_START;
    eeprom_write_block(&AU_STAT, (void *)addr, sizeof(AU_STAT));
}
#endif

static void au_output(void) {

#ifndef AU_SERIAL
    au_output_eeprom();
#endif

#ifdef AU_SERIAL
    au_output_serial();
#endif

}

#define AU_OUTPUT_SETUP()
#define AU_OUTPUT() au_output()
#define AU_ASSERT(TEST) do { if (!(TEST)) { return AU_F_FAIL; } } while (0)
#define AU_BROKEN(TEST) do { return AU_F_BROKEN; } while (0)
#define AU_IGNORED(TEST) do { return AU_F_IGNORE; } while (0)

#define AU_UNIT_START
#define AU_UNIT_END do { return AU_F_PASS; } while(0)

#define AU_RUN_TEST(ID, TEST_CASE, ...) do { \
    int index = AU_STAT.size; \
    AU_STAT.record[index].flag = TEST_CASE(__VA_ARGS__); \
    AU_STAT.record[index].id = ID; \
    AU_STAT.size += 1; \
} while (0)

#endif // _AVRUNIT_H
