#include "avrunit.h"

static int test_fail(void) {
    AU_UNIT_START;

    AU_ASSERT(1 == 0);

    AU_UNIT_END;
}

int main (void) {

    AU_RUN_TEST(0x0c, test_fail);

    AU_OUTPUT();

    return (0);
}
