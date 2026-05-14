#include "../s21_decimal_utils.h"
#include "s21_test.h"

static s21_decimal make_decimal(uint32_t lo, uint32_t mid, uint32_t hi,
                                int scale, int sign) {
  s21_decimal d = {0};
  d.bits[0] = lo;
  d.bits[1] = mid;
  d.bits[2] = hi;
  s21_set_scale(&d, scale);
  s21_set_sign(&d, sign);
  return d;
}

/////////////////////////////////////
/////           ADD
/////////////////////////////////////

START_TEST(s21_add_simple) {
  s21_decimal a = make_decimal(10, 0, 0, 0, 0);
  s21_decimal b = make_decimal(20, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_add(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 30);
  ck_assert_int_eq(r.bits[1], 0);
  ck_assert_int_eq(r.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(&r), 0);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_add_zero_left) {
  s21_decimal a = make_decimal(0, 0, 0, 0, 0);
  s21_decimal b = make_decimal(55, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_add(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 55);
  ck_assert_int_eq(s21_get_scale(&r), 0);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_add_zero_right) {
  s21_decimal a = make_decimal(55, 0, 0, 0, 0);
  s21_decimal b = make_decimal(0, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_add(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 55);
  ck_assert_int_eq(s21_get_scale(&r), 0);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_add_with_scale_same) {
  s21_decimal a = make_decimal(123, 0, 0, 1, 0);  // 12.3
  s21_decimal b = make_decimal(77, 0, 0, 1, 0);   // 7.7
  s21_decimal r = {0};

  int code = s21_add(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 200);  // 20.0
  ck_assert_int_eq(s21_get_scale(&r), 1);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_add_with_scale_diff) {
  s21_decimal a = make_decimal(15, 0, 0, 1, 0);  // 1.5
  s21_decimal b = make_decimal(25, 0, 0, 2, 0);  // 0.25
  s21_decimal r = {0};

  int code = s21_add(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 175);  // 1.75
  ck_assert_int_eq(s21_get_scale(&r), 2);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_add_negative_positive) {
  s21_decimal a = make_decimal(100, 0, 0, 0, 1);  // -100
  s21_decimal b = make_decimal(30, 0, 0, 0, 0);   // 30
  s21_decimal r = {0};

  int code = s21_add(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 70);
  ck_assert_int_eq(s21_get_sign(&r), 1);
  ck_assert_int_eq(s21_get_scale(&r), 0);
}
END_TEST

START_TEST(s21_add_positive_negative) {
  s21_decimal a = make_decimal(100, 0, 0, 0, 0);  // 100
  s21_decimal b = make_decimal(30, 0, 0, 0, 1);   // -30
  s21_decimal r = {0};

  int code = s21_add(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 70);
  ck_assert_int_eq(s21_get_sign(&r), 0);
  ck_assert_int_eq(s21_get_scale(&r), 0);
}
END_TEST

START_TEST(s21_add_two_negative) {
  s21_decimal a = make_decimal(40, 0, 0, 0, 1);
  s21_decimal b = make_decimal(2, 0, 0, 0, 1);
  s21_decimal r = {0};

  int code = s21_add(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 42);
  ck_assert_int_eq(s21_get_sign(&r), 1);
  ck_assert_int_eq(s21_get_scale(&r), 0);
}
END_TEST

START_TEST(s21_add_to_zero) {
  s21_decimal a = make_decimal(500, 0, 0, 1, 0);  // 50.0
  s21_decimal b = make_decimal(500, 0, 0, 1, 1);  // -50.0
  s21_decimal r = {0};

  int code = s21_add(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 0);
  ck_assert_int_eq(r.bits[1], 0);
  ck_assert_int_eq(r.bits[2], 0);
}
END_TEST

START_TEST(s21_add_overflow_positive) {
  s21_decimal a = make_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
  s21_decimal b = make_decimal(1, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_add(a, b, &r);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(s21_add_overflow_negative) {
  s21_decimal a = make_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 1);
  s21_decimal b = make_decimal(1, 0, 0, 0, 1);
  s21_decimal r = {0};

  int code = s21_add(a, b, &r);

  ck_assert_int_eq(code, 2);
}
END_TEST

/////////////////////////////////////
/////           SUB
/////////////////////////////////////

START_TEST(s21_sub_simple) {
  s21_decimal a = make_decimal(50, 0, 0, 0, 0);
  s21_decimal b = make_decimal(20, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_sub(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 30);
  ck_assert_int_eq(s21_get_sign(&r), 0);
  ck_assert_int_eq(s21_get_scale(&r), 0);
}
END_TEST

START_TEST(s21_sub_negative_result) {
  s21_decimal a = make_decimal(20, 0, 0, 0, 0);
  s21_decimal b = make_decimal(50, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_sub(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 30);
  ck_assert_int_eq(s21_get_sign(&r), 1);
  ck_assert_int_eq(s21_get_scale(&r), 0);
}
END_TEST

START_TEST(s21_sub_same_values) {
  s21_decimal a = make_decimal(777, 0, 0, 0, 0);
  s21_decimal b = make_decimal(777, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_sub(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 0);
  ck_assert_int_eq(r.bits[1], 0);
  ck_assert_int_eq(r.bits[2], 0);
}
END_TEST

START_TEST(s21_sub_with_scale_same) {
  s21_decimal a = make_decimal(155, 0, 0, 1, 0);  // 15.5
  s21_decimal b = make_decimal(22, 0, 0, 1, 0);   // 2.2
  s21_decimal r = {0};

  int code = s21_sub(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 133);  // 13.3
  ck_assert_int_eq(s21_get_scale(&r), 1);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_sub_with_scale_diff) {
  s21_decimal a = make_decimal(150, 0, 0, 2, 0);  // 1.50
  s21_decimal b = make_decimal(5, 0, 0, 1, 0);    // 0.5
  s21_decimal r = {0};

  int code = s21_sub(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 100);  // 1.00
  ck_assert_int_eq(s21_get_scale(&r), 2);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_sub_minus_minus) {
  s21_decimal a = make_decimal(100, 0, 0, 0, 1);  // -100
  s21_decimal b = make_decimal(40, 0, 0, 0, 1);   // -40
  s21_decimal r = {0};

  int code = s21_sub(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 60);
  ck_assert_int_eq(s21_get_sign(&r), 1);  // -60
}
END_TEST

START_TEST(s21_sub_plus_minus) {
  s21_decimal a = make_decimal(100, 0, 0, 0, 0);  // 100
  s21_decimal b = make_decimal(40, 0, 0, 0, 1);   // -40
  s21_decimal r = {0};

  int code = s21_sub(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 140);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_sub_minus_plus) {
  s21_decimal a = make_decimal(100, 0, 0, 0, 1);  // -100
  s21_decimal b = make_decimal(40, 0, 0, 0, 0);   // 40
  s21_decimal r = {0};

  int code = s21_sub(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 140);
  ck_assert_int_eq(s21_get_sign(&r), 1);
}
END_TEST

START_TEST(s21_sub_zero) {
  s21_decimal a = make_decimal(0, 0, 0, 0, 0);
  s21_decimal b = make_decimal(25, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_sub(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 25);
  ck_assert_int_eq(s21_get_sign(&r), 1);
}
END_TEST

/////////////////////////////////////
/////           MUL
/////////////////////////////////////

START_TEST(s21_mul_simple) {
  s21_decimal a = make_decimal(6, 0, 0, 0, 0);
  s21_decimal b = make_decimal(7, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_mul(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 42);
  ck_assert_int_eq(s21_get_scale(&r), 0);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_mul_by_zero) {
  s21_decimal a = make_decimal(12345, 0, 0, 3, 0);
  s21_decimal b = make_decimal(0, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_mul(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 0);
  ck_assert_int_eq(r.bits[1], 0);
  ck_assert_int_eq(r.bits[2], 0);
}
END_TEST

START_TEST(s21_mul_negative) {
  s21_decimal a = make_decimal(6, 0, 0, 0, 1);
  s21_decimal b = make_decimal(7, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_mul(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 42);
  ck_assert_int_eq(s21_get_sign(&r), 1);
  ck_assert_int_eq(s21_get_scale(&r), 0);
}
END_TEST

START_TEST(s21_mul_two_negative) {
  s21_decimal a = make_decimal(6, 0, 0, 0, 1);
  s21_decimal b = make_decimal(7, 0, 0, 0, 1);
  s21_decimal r = {0};

  int code = s21_mul(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 42);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_mul_with_scale) {
  s21_decimal a = make_decimal(12, 0, 0, 1, 0);  // 1.2
  s21_decimal b = make_decimal(25, 0, 0, 1, 0);  // 2.5
  s21_decimal r = {0};

  int code = s21_mul(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 300);  // 3.00
  ck_assert_int_eq(s21_get_scale(&r), 2);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_mul_scale_sum) {
  s21_decimal a = make_decimal(123, 0, 0, 2, 0);  // 1.23
  s21_decimal b = make_decimal(10, 0, 0, 1, 0);   // 1.0
  s21_decimal r = {0};

  int code = s21_mul(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 1230);
  ck_assert_int_eq(s21_get_scale(&r), 3);
}
END_TEST

START_TEST(s21_mul_one) {
  s21_decimal a = make_decimal(999, 0, 0, 1, 0);  // 99.9
  s21_decimal b = make_decimal(1, 0, 0, 0, 0);    // 1
  s21_decimal r = {0};

  int code = s21_mul(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 999);
  ck_assert_int_eq(s21_get_scale(&r), 1);
}
END_TEST

START_TEST(s21_mul_overflow_positive) {
  s21_decimal a = make_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
  s21_decimal b = make_decimal(2, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_mul(a, b, &r);

  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(s21_mul_overflow_negative) {
  s21_decimal a = make_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 1);
  s21_decimal b = make_decimal(2, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_mul(a, b, &r);

  ck_assert_int_eq(code, 2);
}
END_TEST

/////////////////////////////////////
/////           DIV
/////////////////////////////////////

START_TEST(s21_div_simple) {
  s21_decimal a = make_decimal(84, 0, 0, 0, 0);
  s21_decimal b = make_decimal(2, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_div(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 42);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_div_negative) {
  s21_decimal a = make_decimal(84, 0, 0, 0, 1);
  s21_decimal b = make_decimal(2, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_div(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 42);
  ck_assert_int_eq(s21_get_sign(&r), 1);
}
END_TEST

START_TEST(s21_div_two_negative) {
  s21_decimal a = make_decimal(84, 0, 0, 0, 1);
  s21_decimal b = make_decimal(2, 0, 0, 0, 1);
  s21_decimal r = {0};

  int code = s21_div(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 42);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_div_by_zero) {
  s21_decimal a = make_decimal(84, 0, 0, 0, 0);
  s21_decimal b = make_decimal(0, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_div(a, b, &r);

  ck_assert_int_eq(code, 3);
}
END_TEST

START_TEST(s21_div_zero_by_number) {
  s21_decimal a = make_decimal(0, 0, 0, 0, 0);
  s21_decimal b = make_decimal(5, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_div(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 0);
  ck_assert_int_eq(r.bits[1], 0);
  ck_assert_int_eq(r.bits[2], 0);
}
END_TEST

START_TEST(s21_div_exact_with_scale) {
  s21_decimal a = make_decimal(100, 0, 0, 1, 0);  // 10.0
  s21_decimal b = make_decimal(20, 0, 0, 1, 0);   // 2.0
  s21_decimal r = {0};

  int code = s21_div(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 5);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_div_fraction_half) {
  s21_decimal a = make_decimal(1, 0, 0, 0, 0);
  s21_decimal b = make_decimal(2, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_div(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 5);
  ck_assert_int_eq(s21_get_scale(&r), 1);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_div_fraction_quarter) {
  s21_decimal a = make_decimal(1, 0, 0, 0, 0);
  s21_decimal b = make_decimal(4, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_div(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 25);
  ck_assert_int_eq(s21_get_scale(&r), 2);
  ck_assert_int_eq(s21_get_sign(&r), 0);
}
END_TEST

START_TEST(s21_div_by_one) {
  s21_decimal a = make_decimal(12345, 0, 0, 2, 0);  // 123.45
  s21_decimal b = make_decimal(1, 0, 0, 0, 0);
  s21_decimal r = {0};

  int code = s21_div(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 12345);
  ck_assert_int_eq(s21_get_scale(&r), 2);
}
END_TEST

START_TEST(s21_div_same_numbers) {
  s21_decimal a = make_decimal(999, 0, 0, 3, 0);
  s21_decimal b = make_decimal(999, 0, 0, 3, 0);
  s21_decimal r = {0};

  int code = s21_div(a, b, &r);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(r.bits[0], 1);
  ck_assert_int_eq(s21_get_scale(&r), 0);
}
END_TEST

/////////////////////////////////////
/////         SUITE
/////////////////////////////////////

Suite *arithmetic_tests_suite(void) {
  Suite *s = suite_create("ArithmeticFuncs");

  TCase *tc_add = tcase_create("Add");
  tcase_add_test(tc_add, s21_add_simple);
  tcase_add_test(tc_add, s21_add_zero_left);
  tcase_add_test(tc_add, s21_add_zero_right);
  tcase_add_test(tc_add, s21_add_with_scale_same);
  tcase_add_test(tc_add, s21_add_with_scale_diff);
  tcase_add_test(tc_add, s21_add_negative_positive);
  tcase_add_test(tc_add, s21_add_positive_negative);
  tcase_add_test(tc_add, s21_add_two_negative);
  tcase_add_test(tc_add, s21_add_to_zero);
  tcase_add_test(tc_add, s21_add_overflow_positive);
  tcase_add_test(tc_add, s21_add_overflow_negative);
  suite_add_tcase(s, tc_add);

  TCase *tc_sub = tcase_create("Sub");
  tcase_add_test(tc_sub, s21_sub_simple);
  tcase_add_test(tc_sub, s21_sub_negative_result);
  tcase_add_test(tc_sub, s21_sub_same_values);
  tcase_add_test(tc_sub, s21_sub_with_scale_same);
  tcase_add_test(tc_sub, s21_sub_with_scale_diff);
  tcase_add_test(tc_sub, s21_sub_minus_minus);
  tcase_add_test(tc_sub, s21_sub_plus_minus);
  tcase_add_test(tc_sub, s21_sub_minus_plus);
  tcase_add_test(tc_sub, s21_sub_zero);
  suite_add_tcase(s, tc_sub);

  TCase *tc_mul = tcase_create("Mul");
  tcase_add_test(tc_mul, s21_mul_simple);
  tcase_add_test(tc_mul, s21_mul_by_zero);
  tcase_add_test(tc_mul, s21_mul_negative);
  tcase_add_test(tc_mul, s21_mul_two_negative);
  tcase_add_test(tc_mul, s21_mul_with_scale);
  tcase_add_test(tc_mul, s21_mul_scale_sum);
  tcase_add_test(tc_mul, s21_mul_one);
  tcase_add_test(tc_mul, s21_mul_overflow_positive);
  tcase_add_test(tc_mul, s21_mul_overflow_negative);
  suite_add_tcase(s, tc_mul);

  TCase *tc_div = tcase_create("Div");
  tcase_add_test(tc_div, s21_div_simple);
  tcase_add_test(tc_div, s21_div_negative);
  tcase_add_test(tc_div, s21_div_two_negative);
  tcase_add_test(tc_div, s21_div_by_zero);
  tcase_add_test(tc_div, s21_div_zero_by_number);
  tcase_add_test(tc_div, s21_div_exact_with_scale);
  tcase_add_test(tc_div, s21_div_fraction_half);
  tcase_add_test(tc_div, s21_div_fraction_quarter);
  tcase_add_test(tc_div, s21_div_by_one);
  tcase_add_test(tc_div, s21_div_same_numbers);
  suite_add_tcase(s, tc_div);

  return s;
}