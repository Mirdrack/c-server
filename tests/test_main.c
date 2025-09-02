#include <check.h>
#include <stdlib.h>

Suite *utils_suite(void);
Suite *logger_suite(void);

int main(void)
{
    int number_failed = 0;
    SRunner *suite_runner = srunner_create(utils_suite());
    srunner_add_suite(suite_runner, logger_suite());

    srunner_run_all(suite_runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
