#include "s21_test.h"

int main(void) {
  int number_failed = 0;

  Suite *convert_suite;
  Suite *other_suite;
  Suite *arithmetic_suite;
  Suite *compare_suite;
  SRunner *sr;

  convert_suite = convert_tests_suite();
  other_suite = other_tests_suite();
  arithmetic_suite = arithmetic_tests_suite();
  compare_suite = compare_tests_suite();

  sr = srunner_create(convert_suite);
  srunner_add_suite(sr, other_suite);
  srunner_add_suite(sr, arithmetic_suite);
  srunner_add_suite(sr, compare_suite);

  srunner_set_fork_status(sr, CK_NOFORK);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);

  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}
