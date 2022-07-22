#ifndef _AVRUNIT_H
#define _AVRUNIT_H

#include <avr/io.h>

#define AU_F_PASS 0
#define AU_F_FAIL 1
#define AU_F_BROKEN 2
#define AU_F_IGNORE 3

typedef struct {
    uint16_t run;
    uint16_t fail;
    uint16_t broken;
    uint16_t ignore;
    uint16_t failID[10];
    uint16_t brokenID[10];
    uint16_t ignoreID[10];
} stat_t;

#define AU_SETUP stat_t AU_STAT = {0};

#define AU_OUTPUT_SETUP au_output_setup();
#define AU_OUTPUT au_output();
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
