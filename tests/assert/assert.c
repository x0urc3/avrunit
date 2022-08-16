#include "avrunit.h"

static int test_assert1(void) {
    AU_UNIT_START;
    uint8_t result = 1;
    int b = 1;

    AU_ASSERT(result == b);

    AU_UNIT_END;
}

static int test_assert2(void) {
    AU_UNIT_START;

    AU_ASSERT_FAIL(1 == 2);

    AU_UNIT_END;
}

int main (void) {

    AU_RUN_TEST(0x0c, test_assert1);
    AU_RUN_TEST(0x0d, test_assert2);

    AU_OUTPUT();

    return (0);
}
