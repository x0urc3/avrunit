#include "avrunit.h"

static int test_ignore(void) {
    AU_UNIT_START;

    AU_IGNORED(1 == 1);

    AU_UNIT_END;
}

static int test_broken(void) {
    AU_UNIT_START;

    AU_BROKEN(1 == 1);

    AU_UNIT_END;
}

static int test_pass(void) {
    AU_UNIT_START;

    AU_ASSERT(1 == 1);

    AU_UNIT_END;
}

int main (void) {

    AU_RUN_TEST(0x0a, test_ignore);
    AU_RUN_TEST(0x0b, test_broken);
    AU_RUN_TEST(0x0d, test_pass);

    AU_OUTPUT();

    return (0);
}
