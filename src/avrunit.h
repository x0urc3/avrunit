#include <avr/io.h>
#include <avr/eeprom.h>

#define MU_PASS 0
#define MU_FAIL 1
#define MU_BROKEN 2
#define MU_IGNORE 3

typedef uint8_t ut8;
typedef uint16_t size_t;

struct eeDataStruct {
    uint16_t run;
    uint16_t fail;
    uint16_t broken;
    uint16_t ignore;
    uint16_t failID[10];
    uint16_t brokenID[10];
    uint16_t ignoreID[10];
};

struct eeDataStruct EEMEM mu_ee_Stat = {0};     // Required to be defined
                                                //
//struct muDataStruct {
typedef struct {
    uint16_t run;
    uint16_t fail;
    uint16_t broken;
    uint16_t ignore;
    uint16_t failID[10];
    uint16_t brokenID[10];
    uint16_t ignoreID[10];
} muDataStruct ;

//struct muDataStruct mu_stat;
struct muDataStruct mu_stat;

#define mu_assert(test) do { if (!(test)) { return MU_FAIL; } } while (0)
#define mu_broken(test) do { return MU_BROKEN; } while (0)
#define mu_ignored(test) do { return MU_IGNORE; } while (0)

#define mu_end() do { return MU_PASS; } while(0)

#define mu_run_test_named(test, ...) do { \
    int result; \
    result = test(__VA_ARGS__); \
    mu_tests_run++; \
    switch (result) { \
        case MU_FAIL: \
            mu_stat.fail += 1; \
            break; \
        case MU_BROKEN: \
            mu_tests_broken += 1; \
            break; \
        case MU_IGNORE: \
            mu_tests_ignore += 1; \
            break; \
    }\
} while (0)

#define mu_run_test(test, ...) mu_run_test_named (test, __VA_ARGS__)

mu_stat.fail = 0;
int mu_tests_run = 0;
int mu_tests_fail = 0;
int mu_tests_broken = 0;
int mu_tests_ignore = 0;
