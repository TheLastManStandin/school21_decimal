#include "../s21_decimal_utils.h"
#include "s21_test.h"

START_TEST(s21_from_int_basic) {
  int a = 15;
  s21_decimal b = {0};
  s21_from_int_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 15);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq(b.bits[3], 0);
}
END_TEST

START_TEST(s21_from_int_big) {
  int a = 2147483647;
  s21_decimal b = {0};
  s21_from_int_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 2147483647);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq(b.bits[3], 0);
}
END_TEST

START_TEST(s21_from_int_negative) {
  int a = -15;
  s21_decimal b = {0};
  s21_from_int_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 15);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq(b.bits[3], 2147483648);
}
END_TEST

START_TEST(s21_from_int_big_negative) {
  int a = -2147483647;
  s21_decimal b = {0};
  s21_from_int_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 2147483647);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq(b.bits[3], 2147483648);
}
END_TEST

START_TEST(s21_from_int_zero) {
  int a = 0;
  s21_decimal b = {0};
  s21_from_int_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 0);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq(b.bits[3], 0);
}
END_TEST

/////////////////////////////////////
/////   FROM FLOAT TO DECIMAL
/////////////////////////////////////

START_TEST(s21_from_float_default) {
  float a = 35.1f;
  s21_decimal b = {0};
  s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 351);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq((b.bits[3] >> SCALE_SHIFT) & SCALE_MASK, 1);
}
END_TEST

START_TEST(s21_from_float_default2) {
  float a = 35.123f;
  s21_decimal b = {0};
  s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 35123);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq((b.bits[3] >> SCALE_SHIFT) & SCALE_MASK, 3);
}
END_TEST

START_TEST(s21_from_float_default3) {
  float a = 3512.345f;
  s21_decimal b = {0};
  s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 3512345);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq((b.bits[3] >> SCALE_SHIFT) & SCALE_MASK, 3);
}
END_TEST

START_TEST(s21_from_float_default4) {
  float a = 0.0000035123f;
  s21_decimal b = {0};
  s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 35123);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq((b.bits[3] >> SCALE_SHIFT) & SCALE_MASK, 10);
}
END_TEST

START_TEST(s21_from_float_default5) {
  float a = 32345.184299428;
  s21_decimal b = {0};
  s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 3234518);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq((b.bits[3] >> SCALE_SHIFT) & SCALE_MASK, 2);
}
END_TEST

START_TEST(s21_from_float_negative) {
  float a = -35.1f;
  s21_decimal b = {0};
  s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 351);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq((b.bits[3] >> SCALE_SHIFT) & SCALE_MASK, 1);
  ck_assert_int_eq((b.bits[3] >> SIGN_BIT) & 1, 1);
}
END_TEST

START_TEST(s21_from_float_zero) {
  float a = 0.0f;
  s21_decimal b = {0};
  s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 0);
  ck_assert_int_eq(b.bits[1], 0);
  ck_assert_int_eq(b.bits[2], 0);
  ck_assert_int_eq(b.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_trailing_zeros) {
  float a = 3.500f;
  s21_decimal b = {0};
  s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 35);
  ck_assert_int_eq((b.bits[3] >> SCALE_SHIFT) & SCALE_MASK, 1);
}
END_TEST

START_TEST(s21_from_float_integer) {
  float a = 100.0f;
  s21_decimal b = {0};
  s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 100);
  ck_assert_int_eq((b.bits[3] >> SCALE_SHIFT) & SCALE_MASK, 0);
}
END_TEST

START_TEST(s21_from_float_integer2) {
  float a = 12300.0f;
  s21_decimal b = {0};
  s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(b.bits[0], 12300);
  ck_assert_int_eq((b.bits[3] >> SCALE_SHIFT) & SCALE_MASK, 0);
}
END_TEST

START_TEST(s21_from_float_small) {
  float a = 1e-30f;
  s21_decimal b = {0};
  int result = s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_from_float_inf) {
  float a = INFINITY;
  s21_decimal b = {0};
  int result = s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_from_float_nan) {
  float a = NAN;
  s21_decimal b = {0};
  int result = s21_from_float_to_decimal(a, &b);

  ck_assert_int_eq(result, 1);
}
END_TEST

/////////////////////////////////////
/////   FROM DECIMAL TO INT
/////////////////////////////////////

START_TEST(s21_to_int_default) {
  int a = 0;
  s21_decimal b = {0};

  s21_set_bit_raw(&b, 23, 1);

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, pow(2, 23));
}
END_TEST

START_TEST(s21_to_int_zero) {
  int a = -1;
  s21_decimal b = {0};

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, 0);
}
END_TEST

START_TEST(s21_to_int_positive) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 42;

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, 42);
}
END_TEST

START_TEST(s21_to_int_negative) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 42;
  s21_set_sign(&b, 1);

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, -42);
}
END_TEST

START_TEST(s21_to_int_max) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 2147483647;

  int result = s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(a, 2147483647);
}
END_TEST

START_TEST(s21_to_int_min) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 2147483647;
  s21_set_sign(&b, 1);

  int result = s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(a, -2147483647);
}
END_TEST

START_TEST(s21_to_int_min_minus_one) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 2147483648U;

  int result = s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_to_int_with_scale_1) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 123;
  s21_set_scale(&b, 1);

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, 12);
}
END_TEST

START_TEST(s21_to_int_with_scale_2) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 999;
  s21_set_scale(&b, 2);

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, 9);
}
END_TEST

START_TEST(s21_to_int_truncate_09) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 9;
  s21_set_scale(&b, 1);

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, 0);
}
END_TEST

START_TEST(s21_to_int_truncate_099) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 99;
  s21_set_scale(&b, 2);

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, 0);
}
END_TEST

START_TEST(s21_to_int_negative_with_scale) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 123;
  s21_set_scale(&b, 1);
  s21_set_sign(&b, 1);

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, -12);
}
END_TEST

START_TEST(s21_to_int_too_big) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 1;
  b.bits[1] = 1;

  int result = s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_to_int_bits_2_nonzero) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 1;
  b.bits[2] = 1;

  int result = s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_to_int_max_scale) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 12345678;
  s21_set_scale(&b, 28);

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, 0);
}
END_TEST

START_TEST(s21_to_int_overflow) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 3000000000U;

  int result = s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_to_int_negative_overflow) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 3000000000U;
  s21_set_sign(&b, 1);

  int result = s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_to_int_100_with_scale_2) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 10000;
  s21_set_scale(&b, 2);

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, 100);
}
END_TEST

START_TEST(s21_to_int_negative_truncate) {
  int a = 0;
  s21_decimal b = {0};
  b.bits[0] = 15;
  s21_set_scale(&b, 1);
  s21_set_sign(&b, 1);

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, -1);
}
END_TEST

START_TEST(s21_to_int_one) {
  int a = -1;
  s21_decimal b = {0};
  b.bits[0] = 1;

  s21_from_decimal_to_int(b, &a);

  ck_assert_int_eq(a, 1);
}
END_TEST

///////////////////////////////////////
/////   FROM DECIMAL TO FLOAT
///////////////////////////////////////

START_TEST(s21_to_float_zero) {
  float a = -1.0f;
  s21_decimal b = {0};

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 0.0f);
}
END_TEST

START_TEST(s21_to_float_positive) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 42;

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 42.0f);
}
END_TEST

START_TEST(s21_to_float_negative) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 42;
  s21_set_sign(&b, 1);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, -42.0f);
}
END_TEST

START_TEST(s21_to_float_scale_1) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 123;
  s21_set_scale(&b, 1);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 12.3f);
}
END_TEST

START_TEST(s21_to_float_scale_2) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 1234;
  s21_set_scale(&b, 2);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 12.34f);
}
END_TEST

START_TEST(s21_to_float_scale_3) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 12345;
  s21_set_scale(&b, 3);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 12.345f);
}
END_TEST

START_TEST(s21_to_float_max_scale) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 123;
  s21_set_scale(&b, 28);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 1.23e-26f);
}
END_TEST

START_TEST(s21_to_float_negative_with_scale) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 123;
  s21_set_scale(&b, 2);
  s21_set_sign(&b, 1);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, -1.23f);
}
END_TEST

START_TEST(s21_to_float_01) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 1;
  s21_set_scale(&b, 1);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 0.1f);
}
END_TEST

START_TEST(s21_to_float_001) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 1;
  s21_set_scale(&b, 3);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 0.001f);
}
END_TEST

START_TEST(s21_to_float_big_value) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 1000000;

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 1000000.0f);
}
END_TEST

START_TEST(s21_to_float_big_with_scale) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 12345678;
  s21_set_scale(&b, 4);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 1234.5678f);
}
END_TEST

START_TEST(s21_to_float_multiple_bits) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 1;
  b.bits[1] = 1;
  b.bits[2] = 0;

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 4294967297.0f);
}
END_TEST

START_TEST(s21_to_float_precision) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 1234567;
  s21_set_scale(&b, 6);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 1.234567f);
}
END_TEST

START_TEST(s21_to_float_large_number) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 34000000;
  s21_set_scale(&b, 1);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 3400000.0f);
}
END_TEST

START_TEST(s21_to_float_large_number_high_bit) {
  float a = 0.0f;
  s21_decimal b = {0};

  b.bits[2] = 0x80000000;  // установлен самый старший бит мантиссы

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 3.961408125713217e28f);
}
END_TEST

START_TEST(s21_to_float_large_number_all_high) {
  float a = 0.0f;
  s21_decimal b = {0};

  b.bits[2] = 0xFFFFFFFF;
  b.bits[1] = 0xFFFFFFFF;
  b.bits[0] = 0xFFFFFFFF;

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 7.922816251426433e28f);
}
END_TEST

START_TEST(s21_to_float_large_number_scaled) {
  float a = 0.0f;
  s21_decimal b = {0};

  b.bits[2] = 0x80000000;
  s21_set_scale(&b, 5);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 3.961408125713217e23f);
}
END_TEST

START_TEST(s21_to_float_small_number) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 1;
  s21_set_scale(&b, 10);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 1e-10f);
}
END_TEST

START_TEST(s21_to_float_one) {
  float a = -1.0f;
  s21_decimal b = {0};
  b.bits[0] = 1;

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 1.0f);
}
END_TEST

START_TEST(s21_to_float_max_value) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 2147483647;
  b.bits[1] = 0;
  b.bits[2] = 0;

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 2147483647.0f);
}
END_TEST

START_TEST(s21_to_float_truncate_decimal) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 123456789;
  s21_set_scale(&b, 8);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 1.23456789f);
}
END_TEST

START_TEST(s21_to_float_minus_one) {
  float a = 0.0f;
  s21_decimal b = {0};
  b.bits[0] = 1;
  s21_set_sign(&b, 1);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, -1.0f);
}
END_TEST

START_TEST(s21_to_float_small_number_min_scale) {
  float a = 0.0f;
  s21_decimal b = {0};

  b.bits[0] = 1;
  s21_set_scale(&b, 28);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 1e-28f);
}
END_TEST

START_TEST(s21_to_float_small_number_large_mantissa) {
  float a = 0.0f;
  s21_decimal b = {0};

  b.bits[2] = 0x80000000;  // старший бит мантиссы
  s21_set_scale(&b, 28);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 3.9614081f);
}
END_TEST

START_TEST(s21_to_float_small_fraction) {
  float a = 0.0f;
  s21_decimal b = {0};

  b.bits[0] = 1234567;
  s21_set_scale(&b, 13);

  s21_from_decimal_to_float(b, &a);

  ck_assert_float_eq(a, 1.234567e-7f);
}
END_TEST

///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////

Suite *convert_tests_suite(void) {
  Suite *s = suite_create("ConvertFuncs");

  TCase *tc_from_int = tcase_create("FromIntToDecimal");
  tcase_add_test(tc_from_int, s21_from_int_basic);
  tcase_add_test(tc_from_int, s21_from_int_big);
  tcase_add_test(tc_from_int, s21_from_int_negative);
  tcase_add_test(tc_from_int, s21_from_int_big_negative);
  tcase_add_test(tc_from_int, s21_from_int_zero);
  suite_add_tcase(s, tc_from_int);

  TCase *tc_from_float = tcase_create("FromFloatToDecimal");
  tcase_add_test(tc_from_float, s21_from_float_default);
  tcase_add_test(tc_from_float, s21_from_float_default2);
  tcase_add_test(tc_from_float, s21_from_float_default3);
  tcase_add_test(tc_from_float, s21_from_float_default4);
  tcase_add_test(tc_from_float, s21_from_float_default5);
  tcase_add_test(tc_from_float, s21_from_float_negative);
  tcase_add_test(tc_from_float, s21_from_float_zero);
  tcase_add_test(tc_from_float, s21_from_float_trailing_zeros);
  tcase_add_test(tc_from_float, s21_from_float_integer);
  tcase_add_test(tc_from_float, s21_from_float_integer2);
  tcase_add_test(tc_from_float, s21_from_float_small);
  tcase_add_test(tc_from_float, s21_from_float_inf);
  tcase_add_test(tc_from_float, s21_from_float_nan);
  suite_add_tcase(s, tc_from_float);

  TCase *tc_to_int = tcase_create("FromDecimalToInt");
  tcase_add_test(tc_to_int, s21_to_int_default);
  tcase_add_test(tc_to_int, s21_to_int_zero);
  tcase_add_test(tc_to_int, s21_to_int_positive);
  tcase_add_test(tc_to_int, s21_to_int_negative);
  tcase_add_test(tc_to_int, s21_to_int_max);
  tcase_add_test(tc_to_int, s21_to_int_min);
  tcase_add_test(tc_to_int, s21_to_int_min_minus_one);
  tcase_add_test(tc_to_int, s21_to_int_with_scale_1);
  tcase_add_test(tc_to_int, s21_to_int_with_scale_2);
  tcase_add_test(tc_to_int, s21_to_int_truncate_09);
  tcase_add_test(tc_to_int, s21_to_int_truncate_099);
  tcase_add_test(tc_to_int, s21_to_int_negative_with_scale);
  tcase_add_test(tc_to_int, s21_to_int_too_big);
  tcase_add_test(tc_to_int, s21_to_int_bits_2_nonzero);
  tcase_add_test(tc_to_int, s21_to_int_max_scale);
  tcase_add_test(tc_to_int, s21_to_int_overflow);
  tcase_add_test(tc_to_int, s21_to_int_negative_overflow);
  tcase_add_test(tc_to_int, s21_to_int_100_with_scale_2);
  tcase_add_test(tc_to_int, s21_to_int_negative_truncate);
  tcase_add_test(tc_to_int, s21_to_int_one);
  suite_add_tcase(s, tc_to_int);

  TCase *tc_to_float = tcase_create("FromDecimalToFloat");
  tcase_add_test(tc_to_float, s21_to_float_zero);
  tcase_add_test(tc_to_float, s21_to_float_positive);
  tcase_add_test(tc_to_float, s21_to_float_negative);
  tcase_add_test(tc_to_float, s21_to_float_scale_1);
  tcase_add_test(tc_to_float, s21_to_float_scale_2);
  tcase_add_test(tc_to_float, s21_to_float_scale_3);
  tcase_add_test(tc_to_float, s21_to_float_max_scale);
  tcase_add_test(tc_to_float, s21_to_float_negative_with_scale);
  tcase_add_test(tc_to_float, s21_to_float_01);
  tcase_add_test(tc_to_float, s21_to_float_001);
  tcase_add_test(tc_to_float, s21_to_float_big_value);
  tcase_add_test(tc_to_float, s21_to_float_big_with_scale);
  tcase_add_test(tc_to_float, s21_to_float_multiple_bits);
  tcase_add_test(tc_to_float, s21_to_float_precision);
  tcase_add_test(tc_to_float, s21_to_float_large_number);
  tcase_add_test(tc_to_float, s21_to_float_small_number);
  tcase_add_test(tc_to_float, s21_to_float_one);
  tcase_add_test(tc_to_float, s21_to_float_max_value);
  tcase_add_test(tc_to_float, s21_to_float_truncate_decimal);
  tcase_add_test(tc_to_float, s21_to_float_minus_one);
  tcase_add_test(tc_to_float, s21_to_float_large_number_high_bit);
  tcase_add_test(tc_to_float, s21_to_float_large_number_all_high);
  tcase_add_test(tc_to_float, s21_to_float_large_number_scaled);
  tcase_add_test(tc_to_float, s21_to_float_small_number_min_scale);
  tcase_add_test(tc_to_float, s21_to_float_small_number_large_mantissa);
  tcase_add_test(tc_to_float, s21_to_float_small_fraction);
  suite_add_tcase(s, tc_to_float);

  return s;
}
