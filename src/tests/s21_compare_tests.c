#include "../s21_decimal_utils.h"
#include "s21_test.h"

/////////////////////////////////////
/////   IS LESS
/////////////////////////////////////

START_TEST(s21_is_less_basic_true) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 5;
  b.bits[0] = 10;

  ck_assert_int_eq(s21_is_less(a, b), 1);
}
END_TEST

START_TEST(s21_is_less_basic_false) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 10;
  b.bits[0] = 5;

  ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(s21_is_less_equal_false) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 7;
  b.bits[0] = 7;

  ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(s21_is_less_negative_true) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 10;
  b.bits[0] = 5;
  a.bits[3] = 2147483648;
  b.bits[3] = 2147483648;

  ck_assert_int_eq(s21_is_less(a, b), 1);
}
END_TEST

START_TEST(s21_is_less_diff_scale_true) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 10;
  b.bits[0] = 11;
  a.bits[3] = 65536;
  b.bits[3] = 65536;

  ck_assert_int_eq(s21_is_less(a, b), 1);
}
END_TEST

/////////////////////////////////////
/////   IS LESS OR EQUAL
/////////////////////////////////////

START_TEST(s21_is_less_or_equal_less) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 3;
  b.bits[0] = 7;

  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(s21_is_less_or_equal_equal) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 8;
  b.bits[0] = 8;

  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(s21_is_less_or_equal_false) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 9;
  b.bits[0] = 4;

  ck_assert_int_eq(s21_is_less_or_equal(a, b), 0);
}
END_TEST

START_TEST(s21_is_less_or_equal_negative_equal) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 15;
  b.bits[0] = 15;
  a.bits[3] = 2147483648;
  b.bits[3] = 2147483648;

  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(s21_is_less_or_equal_zeroes) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[3] = 0;
  b.bits[3] = 2147483648;

  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

/////////////////////////////////////
/////   IS GREATER
/////////////////////////////////////

START_TEST(s21_is_greater_basic_true) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 12;
  b.bits[0] = 5;

  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(s21_is_greater_basic_false) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 2;
  b.bits[0] = 8;

  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(s21_is_greater_equal_false) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 8;
  b.bits[0] = 8;

  ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(s21_is_greater_negative_true) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 2;
  b.bits[0] = 9;
  a.bits[3] = 2147483648;
  b.bits[3] = 2147483648;

  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(s21_is_greater_diff_scale_true) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 150;
  b.bits[0] = 149;
  a.bits[3] = 131072;
  b.bits[3] = 131072;

  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

/////////////////////////////////////
/////   IS GREATER OR EQUAL
/////////////////////////////////////

START_TEST(s21_is_greater_or_equal_greater) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 20;
  b.bits[0] = 10;

  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(s21_is_greater_or_equal_equal) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 10;
  b.bits[0] = 10;

  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(s21_is_greater_or_equal_false) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 1;
  b.bits[0] = 3;

  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 0);
}
END_TEST

START_TEST(s21_is_greater_or_equal_negative_equal) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 25;
  b.bits[0] = 25;
  a.bits[3] = 2147483648;
  b.bits[3] = 2147483648;

  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(s21_is_greater_or_equal_zeroes) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[3] = 2147483648;
  b.bits[3] = 0;

  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

/////////////////////////////////////
/////   IS EQUAL
/////////////////////////////////////

START_TEST(s21_is_equal_basic_true) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 17;
  b.bits[0] = 17;

  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(s21_is_equal_basic_false) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 17;
  b.bits[0] = 18;

  ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(s21_is_equal_diff_scale_true) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 10;
  b.bits[0] = 100;
  a.bits[3] = 65536;
  b.bits[3] = 131072;

  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(s21_is_equal_zero_sign_true) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[3] = 0;
  b.bits[3] = 2147483648;

  ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(s21_is_equal_sign_false) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 5;
  b.bits[0] = 5;
  b.bits[3] = 2147483648;

  ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

/////////////////////////////////////
/////   IS NOT EQUAL
/////////////////////////////////////

START_TEST(s21_is_not_equal_basic_true) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 4;
  b.bits[0] = 9;

  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(s21_is_not_equal_basic_false) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 11;
  b.bits[0] = 11;

  ck_assert_int_eq(s21_is_not_equal(a, b), 0);
}
END_TEST

START_TEST(s21_is_not_equal_diff_scale_false) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 25;
  b.bits[0] = 250;
  a.bits[3] = 65536;
  b.bits[3] = 131072;

  ck_assert_int_eq(s21_is_not_equal(a, b), 0);
}
END_TEST

START_TEST(s21_is_not_equal_zero_sign_false) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[3] = 0;
  b.bits[3] = 2147483648;

  ck_assert_int_eq(s21_is_not_equal(a, b), 0);
}
END_TEST

START_TEST(s21_is_not_equal_sign_true) {
  s21_decimal a = {0};
  s21_decimal b = {0};

  a.bits[0] = 3;
  b.bits[0] = 3;
  b.bits[3] = 2147483648;

  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

Suite *compare_tests_suite(void) {
  Suite *s = suite_create("CompareFuncs");
  TCase *tc_less = tcase_create("IsLess");
  tcase_add_test(tc_less, s21_is_less_basic_true);
  tcase_add_test(tc_less, s21_is_less_basic_false);
  tcase_add_test(tc_less, s21_is_less_equal_false);
  tcase_add_test(tc_less, s21_is_less_negative_true);
  tcase_add_test(tc_less, s21_is_less_diff_scale_true);
  suite_add_tcase(s, tc_less);
  TCase *tc_less_or_equal = tcase_create("IsLessOrEqual");
  tcase_add_test(tc_less_or_equal, s21_is_less_or_equal_less);
  tcase_add_test(tc_less_or_equal, s21_is_less_or_equal_equal);
  tcase_add_test(tc_less_or_equal, s21_is_less_or_equal_false);
  tcase_add_test(tc_less_or_equal, s21_is_less_or_equal_negative_equal);
  tcase_add_test(tc_less_or_equal, s21_is_less_or_equal_zeroes);
  suite_add_tcase(s, tc_less_or_equal);
  TCase *tc_greater = tcase_create("IsGreater");
  tcase_add_test(tc_greater, s21_is_greater_basic_true);
  tcase_add_test(tc_greater, s21_is_greater_basic_false);
  tcase_add_test(tc_greater, s21_is_greater_equal_false);
  tcase_add_test(tc_greater, s21_is_greater_negative_true);
  tcase_add_test(tc_greater, s21_is_greater_diff_scale_true);
  suite_add_tcase(s, tc_greater);
  TCase *tc_greater_or_equal = tcase_create("IsGreaterOrEqual");
  tcase_add_test(tc_greater_or_equal, s21_is_greater_or_equal_greater);
  tcase_add_test(tc_greater_or_equal, s21_is_greater_or_equal_equal);
  tcase_add_test(tc_greater_or_equal, s21_is_greater_or_equal_false);
  tcase_add_test(tc_greater_or_equal, s21_is_greater_or_equal_negative_equal);
  tcase_add_test(tc_greater_or_equal, s21_is_greater_or_equal_zeroes);
  suite_add_tcase(s, tc_greater_or_equal);
  TCase *tc_equal = tcase_create("IsEqual");
  tcase_add_test(tc_equal, s21_is_equal_basic_true);
  tcase_add_test(tc_equal, s21_is_equal_basic_false);
  tcase_add_test(tc_equal, s21_is_equal_diff_scale_true);
  tcase_add_test(tc_equal, s21_is_equal_zero_sign_true);
  tcase_add_test(tc_equal, s21_is_equal_sign_false);
  suite_add_tcase(s, tc_equal);
  TCase *tc_not_equal = tcase_create("IsNotEqual");
  tcase_add_test(tc_not_equal, s21_is_not_equal_basic_true);
  tcase_add_test(tc_not_equal, s21_is_not_equal_basic_false);
  tcase_add_test(tc_not_equal, s21_is_not_equal_diff_scale_false);
  tcase_add_test(tc_not_equal, s21_is_not_equal_zero_sign_false);
  tcase_add_test(tc_not_equal, s21_is_not_equal_sign_true);
  suite_add_tcase(s, tc_not_equal);
  return s;
}