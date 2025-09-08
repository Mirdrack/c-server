#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"

#define TMP_STDOUT "test_stdout.log"
#define TMP_STDERR "test_stderr.log"

// Helper: reset log files and redirect streams
static void redirect_streams(void)
{
    if (freopen(TMP_STDOUT, "w", stdout) == NULL)
    {
        perror("freopen stdout failed");
    }
    if (freopen(TMP_STDERR, "w", stderr) == NULL)
    {
        perror("freopen stderr failed");
    }
    fflush(stdout);
    fflush(stderr);
}

// Helper: read file content into string
static char *read_file(const char *path)
{
    FILE *file = fopen(path, "r");
    if (!file)
    {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);
    return buffer;
}

// --- Tests ---

START_TEST(test_info_log_goes_to_stdout)
{
    redirect_streams();
    set_log_level(LOG_INFO);

    LOG_INFO("Service started");

    fflush(stdout);
    char *out = read_file(TMP_STDOUT);
    ck_assert_msg(out != NULL, "stdout file missing");
    ck_assert_msg(strstr(out, "\"level\": \"INFO\"") != NULL, "INFO log missing in stdout");
    free(out);

    char *err = read_file(TMP_STDERR);
    ck_assert_msg(err != NULL, "stderr file missing");
    ck_assert_int_eq(strlen(err), 0); // no errors expected
    free(err);
}
END_TEST

START_TEST(test_error_log_goes_to_stderr)
{
    redirect_streams();
    set_log_level(LOG_DEBUG);

    LOG_ERROR("Something failed");

    fflush(stderr);
    char *err = read_file(TMP_STDERR);
    ck_assert_msg(err != NULL, "stderr file missing");
    ck_assert_msg(strstr(err, "\"level\": \"ERROR\"") != NULL, "ERROR log missing in stderr");
    free(err);

    char *out = read_file(TMP_STDOUT);
    ck_assert_msg(out != NULL, "stdout file missing");
    ck_assert_int_eq(strlen(out), 0); // nothing in stdout
    free(out);
}
END_TEST

START_TEST(test_log_level_filtering)
{
    redirect_streams();
    set_log_level(LOG_WARN);

    LOG_DEBUG("Debug msg should be hidden");
    LOG_INFO("Info msg should be hidden");
    LOG_WARN("This is a warning");

    fflush(stdout);
    fflush(stderr);

    char *out = read_file(TMP_STDOUT);
    ck_assert_msg(out != NULL, "stdout file missing");
    ck_assert_int_eq(strlen(out), 0); // no debug/info logs
    free(out);

    char *err = read_file(TMP_STDERR);
    ck_assert_msg(err != NULL, "stderr file missing");
    ck_assert_msg(strstr(err, "\"level\": \"WARN\"") != NULL, "WARN log missing in stderr");
    free(err);
}
END_TEST

Suite *logger_suite(void)
{
    Suite *suite = suite_create("Logger");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_info_log_goes_to_stdout);
    tcase_add_test(tc_core, test_error_log_goes_to_stderr);
    tcase_add_test(tc_core, test_log_level_filtering);

    suite_add_tcase(suite, tc_core);
    return suite;
}
