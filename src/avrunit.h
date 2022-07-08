#ifndef _AVRUNIT_H
#define _AVRUNIT_H

#include <avr/io.h>
#include <avr/eeprom.h>

#define MU_PASS 0
#define MU_FAIL 1
#define MU_BROKEN 2
#define MU_IGNORE 3

typedef struct {
    uint16_t run;
    uint16_t fail;
    uint16_t broken;
    uint16_t ignore;
    uint16_t failID[10];
    uint16_t brokenID[10];
    uint16_t ignoreID[10];
} stat_t;

/*
#define mu_setup do { \
    stat_t EEMEM mu_ee_stat = {0}; \
    stat_t mu_stat = {0}; \
    int mu_tests_run = 0; \
    int mu_tests_fail = 0; \
    int mu_tests_broken = 0; \
    int mu_tests_ignore = 0; \
} while (0)
*/
stat_t EEMEM mu_ee_stat = {0};
stat_t mu_stat = {0};

#define mu_assert(test) do { if (!(test)) { return MU_FAIL; } } while (0)
#define mu_broken(test) do { return MU_BROKEN; } while (0)
#define mu_ignored(test) do { return MU_IGNORE; } while (0)

#define mu_end() do { return MU_PASS; } while(0)

#define mu_run_test_named(test, ...) do { \
    int result; \
    result = test(__VA_ARGS__); \
    mu_stat.run++; \
    switch (result) { \
        case MU_FAIL: \
            mu_stat.fail += 1; \
            break; \
        case MU_BROKEN: \
            mu_stat.broken += 1; \
            break; \
        case MU_IGNORE: \
            mu_stat.ignore += 1; \
            break; \
    } \
} while (0)

#define mu_run_test(test, ...) mu_run_test_named (test, __VA_ARGS__)


#endif // _AVRUNIT_H
