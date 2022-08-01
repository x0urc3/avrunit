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

#define AU_MAX_TEST 10
#define AU_F_PASS 0
#define AU_F_FAIL 1
#define AU_F_BROKEN 2
#define AU_F_IGNORE 3
#define AU_F_SIZE 4

typedef struct {
    uint8_t index;
    uint8_t id[AU_MAX_TEST];
} stat_t;

stat_t AU_STAT[AU_F_SIZE];

typedef struct {
    char label_index[2];
    char label_id[2];
} stat_fmt_t;

stat_fmt_t AU_STAT_FMT[AU_F_SIZE] = {
    {"P ", "ID"},
    {"F ", "ID"},
    {"B ", "ID"},
    {"I ", "ID"}
};

#define AU_OUTPUT_SETUP()
#define AU_OUTPUT() do { \
    int addr = 0; \
    eeprom_write_block(&AU_STAT, (void *)addr, sizeof(AU_STAT)); \
    addr = sizeof(AU_STAT); \
    eeprom_write_block(&AU_STAT_FMT, (void *)addr, sizeof(AU_STAT_FMT)); \
} while (0)
#define AU_ASSERT(TEST) do { if (!(TEST)) { return AU_F_FAIL; } } while (0)
#define AU_BROKEN(TEST) do { return AU_F_BROKEN; } while (0)
#define AU_IGNORED(TEST) do { return AU_F_IGNORE; } while (0)

#define AU_UNIT_START
#define AU_UNIT_END do { return AU_F_PASS; } while(0)

#define AU_RUN_TEST(ID, TEST_CASE, ...) do { \
    int flag, index; \
    flag = TEST_CASE(__VA_ARGS__); \
    index = AU_STAT[flag].index; \
    AU_STAT[flag].id[index] = ID; \
    AU_STAT[flag].index = index + 1; \
} while (0)

#endif // _AVRUNIT_H
