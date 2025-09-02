#include "../include/utils.h"
#include <check.h>

START_TEST(test_simple_extension)
{
    ck_assert_str_eq(get_file_extention("file.txt"), "txt");
}
END_TEST

START_TEST(test_multiple_dots)
{
    ck_assert_str_eq(get_file_extention("archive.tar.gz"), "gz");
    ck_assert_str_eq(get_file_extention("a.b.c.d"), "d");
}
END_TEST

START_TEST(test_no_extension)
{
    ck_assert_str_eq(get_file_extention("file"), "");
}
END_TEST

START_TEST(test_hidden_file)
{
    ck_assert_str_eq(get_file_extention(".hiddenfile"), "");
}
END_TEST

START_TEST(test_trailing_dot)
{
    ck_assert_str_eq(get_file_extention("file."), "");
}
END_TEST

START_TEST(test_only_dots)
{
    ck_assert_str_eq(get_file_extention(".."), "");
}
END_TEST

START_TEST(test_empty_string)
{
    ck_assert_str_eq(get_file_extention(""), "");
}
END_TEST

Suite *utils_suite(void)
{
    Suite *suite = suite_create("FileUtils");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_simple_extension);
    tcase_add_test(tc_core, test_multiple_dots);
    tcase_add_test(tc_core, test_no_extension);
    tcase_add_test(tc_core, test_hidden_file);
    tcase_add_test(tc_core, test_trailing_dot);
    tcase_add_test(tc_core, test_only_dots);
    tcase_add_test(tc_core, test_empty_string);

    suite_add_tcase(suite, tc_core);
    return suite;
}
