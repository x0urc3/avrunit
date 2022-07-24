#ifndef _AVRUNIT_H
#define _AVRUNIT_H

#include <avr/io.h>
#include <avr/eeprom.h>

#define AU_MAX_TEST 10
#define AU_F_PASS 0
#define AU_F_FAIL 1
#define AU_F_BROKEN 2
#define AU_F_IGNORE 3

typedef struct {
    uint8_t fail;
    uint8_t failID[AU_MAX_TEST];
    uint8_t broken;
    uint8_t brokenID[AU_MAX_TEST];
    uint8_t ignore;
    uint8_t ignoreID[AU_MAX_TEST];
    uint8_t run;
} stat_t;

stat_t AU_STAT = {0};
uint8_t AU_STAT_FMT[] = {
    7,              // Number of record
    1, AU_MAX_TEST,
    1, AU_MAX_TEST,
    1, AU_MAX_TEST,
    1
};

#define AU_OUTPUT_SETUP()
#define AU_OUTPUT() do { \
    eeprom_write_block(&AU_STAT_FMT, 0, sizeof(AU_STAT_FMT)); \
    eeprom_write_block(&AU_STAT, sizeof(AU_STAT_FMT), sizeof(AU_STAT)); \
} while (0)
#define AU_ASSERT(test) do { if (!(test)) { return AU_F_FAIL; } } while (0)
#define AU_BROKEN(test) do { return AU_F_BROKEN; } while (0)
#define AU_IGNORED(test) do { return AU_F_IGNORE; } while (0)

#define AU_UNIT_START
#define AU_UNIT_END do { return AU_F_PASS; } while(0)

#define AU_RUN_TEST(id, test, ...) do { \
    int result; \
    result = test(__VA_ARGS__); \
    AU_STAT.run++; \
    switch (result) { \
        case AU_F_FAIL: \
            AU_STAT.failID[AU_STAT.fail] = id; \
            AU_STAT.fail += 1; \
            break; \
        case AU_F_BROKEN: \
            AU_STAT.brokenID[AU_STAT.broken] = id; \
            AU_STAT.broken += 1; \
            break; \
        case AU_F_IGNORE: \
            AU_STAT.ignoreID[AU_STAT.ignore] = id; \
            AU_STAT.ignore += 1; \
            break; \
    } \
} while (0)

#endif // _AVRUNIT_H
