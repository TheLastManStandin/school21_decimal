#include <stddef.h>

#include "../s21_decimal_utils.h"
#include "s21_test.h"

START_TEST(s21_floor1) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 37;
  value.bits[3] |= (1u << 31);
  value.bits[3] |= (1u << 16);

  int code = s21_floor(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 4);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 1);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_floor2) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 1234;
  value.bits[3] |= (2u << 16);

  int code = s21_floor(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 12);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_floor3) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 1234;
  value.bits[3] |= (1u << 31);
  value.bits[3] |= (2u << 16);

  int code = s21_floor(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 13);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 1);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_floor4) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 5;

  int code = s21_floor(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(s21_floor5) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 30;
  value.bits[3] |= (1u << 31);
  value.bits[3] |= (1u << 16);

  int code = s21_floor(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 3);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 1);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_floor6) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  int code = s21_floor(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_floor7) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 30;
  value.bits[3] |= (1u << 16);

  int code = s21_floor(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 3);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_floor8) {
  s21_decimal value = {0};

  value.bits[0] = 10;

  int code = s21_floor(value, NULL);

  ck_assert_int_ne(code, 0);
}
END_TEST
///////////////////////////////////////////
///////////////////////// ROUND
///////////////////////////////////////////

START_TEST(s21_round1) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 1249;
  value.bits[3] |= (2u << 16);

  int code = s21_round(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 12);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_round2) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 1251;
  value.bits[3] |= (2u << 16);

  int code = s21_round(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 13);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_round3) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 25;
  value.bits[3] |= (1u << 16);

  int code = s21_round(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_round4) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 35;
  value.bits[3] |= (1u << 16);

  int code = s21_round(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 4);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_round5) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 14;
  value.bits[3] |= (1u << 16);
  value.bits[3] |= (1u << 31);

  int code = s21_round(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 1);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_round6) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 16;
  value.bits[3] |= (1u << 16);
  value.bits[3] |= (1u << 31);

  int code = s21_round(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 1);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_round7) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 25;
  value.bits[3] |= (1u << 16);
  value.bits[3] |= (1u << 31);

  int code = s21_round(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 1);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_round8) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 35;
  value.bits[3] |= (1u << 16);
  value.bits[3] |= (1u << 31);

  int code = s21_round(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 4);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 1);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_round9) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 30;
  value.bits[3] |= (1u << 16);

  int code = s21_round(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 3);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_round10) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 5;

  int code = s21_round(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] >> 31, 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);
}
END_TEST

START_TEST(s21_round11) {
  s21_decimal value = {0};

  value.bits[0] = 10;

  int code = s21_round(value, NULL);

  ck_assert_int_ne(code, 0);
}
END_TEST

///////////////////////////////////////////
///////////////////////// TRUNCATE
///////////////////////////////////////////
START_TEST(s21_truncate1) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 1234;
  value.bits[3] |= (2u << 16);

  int code = s21_truncate(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 12);
  ck_assert_int_eq(result.bits[3] >> 31, 0);
}
END_TEST

START_TEST(s21_truncate2) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 1234;
  value.bits[3] |= (2u << 16);
  value.bits[3] |= (1u << 31);

  int code = s21_truncate(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 12);
  ck_assert_int_eq(result.bits[3] >> 31, 1);
}
END_TEST

START_TEST(s21_truncate3) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 5;

  int code = s21_truncate(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
}
END_TEST

START_TEST(s21_truncate4) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 5;
  value.bits[3] |= (1u << 31);

  int code = s21_truncate(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[3] >> 31, 1);
}
END_TEST

START_TEST(s21_truncate5) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 30;
  value.bits[3] |= (1u << 16);

  int code = s21_truncate(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 3);
}
END_TEST

START_TEST(s21_truncate6) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  int code = s21_truncate(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(s21_truncate7) {
  s21_decimal value = {0};

  int code = s21_truncate(value, NULL);

  ck_assert_int_ne(code, 0);
}
END_TEST
///////////////////////////////////////////
///////////////////////// NEGATE
///////////////////////////////////////////
START_TEST(s21_negate1) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 5;

  int code = s21_negate(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[3] >> 31, 1);
}
END_TEST

START_TEST(s21_negate2) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 5;
  value.bits[3] |= (1u << 31);

  int code = s21_negate(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[3] >> 31, 0);
}
END_TEST

START_TEST(s21_negate3) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  value.bits[0] = 37;
  value.bits[3] |= (1u << 16);

  int code = s21_negate(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 37);
  ck_assert_int_eq(result.bits[3] >> 31, 1);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 1);
}
END_TEST

START_TEST(s21_negate4) {
  s21_decimal value = {0};
  s21_decimal result = {0};

  int code = s21_negate(value, &result);

  ck_assert_int_eq(code, 0);
  ck_assert_int_eq(result.bits[0], 0);
}
END_TEST

START_TEST(s21_negate5) {
  s21_decimal value = {0};

  int code = s21_negate(value, NULL);

  ck_assert_int_ne(code, 0);
}
END_TEST

////END////
Suite *other_tests_suite(void) {
  Suite *s = suite_create("OtherFuncs");

  TCase *tc_floor = tcase_create("Floor");
  TCase *tc_round = tcase_create("Round");
  TCase *tc_truncate = tcase_create("Truncate");
  TCase *tc_negate = tcase_create("Negate");

  // floor tests
  tcase_add_test(tc_floor, s21_floor1);
  tcase_add_test(tc_floor, s21_floor2);
  tcase_add_test(tc_floor, s21_floor3);
  tcase_add_test(tc_floor, s21_floor4);
  tcase_add_test(tc_floor, s21_floor5);
  tcase_add_test(tc_floor, s21_floor6);
  tcase_add_test(tc_floor, s21_floor7);
  tcase_add_test(tc_floor, s21_floor8);

  // round tests
  tcase_add_test(tc_round, s21_round1);
  tcase_add_test(tc_round, s21_round2);
  tcase_add_test(tc_round, s21_round3);
  tcase_add_test(tc_round, s21_round4);
  tcase_add_test(tc_round, s21_round5);
  tcase_add_test(tc_round, s21_round6);
  tcase_add_test(tc_round, s21_round7);
  tcase_add_test(tc_round, s21_round8);
  tcase_add_test(tc_round, s21_round9);
  tcase_add_test(tc_round, s21_round10);
  tcase_add_test(tc_round, s21_round11);

  // truncate tests
  tcase_add_test(tc_truncate, s21_truncate1);
  tcase_add_test(tc_truncate, s21_truncate2);
  tcase_add_test(tc_truncate, s21_truncate3);
  tcase_add_test(tc_truncate, s21_truncate4);
  tcase_add_test(tc_truncate, s21_truncate5);
  tcase_add_test(tc_truncate, s21_truncate6);
  tcase_add_test(tc_truncate, s21_truncate7);

  // negate tests
  tcase_add_test(tc_negate, s21_negate1);
  tcase_add_test(tc_negate, s21_negate2);
  tcase_add_test(tc_negate, s21_negate3);
  tcase_add_test(tc_negate, s21_negate4);
  tcase_add_test(tc_negate, s21_negate5);

  /////////
  suite_add_tcase(s, tc_floor);
  suite_add_tcase(s, tc_round);
  suite_add_tcase(s, tc_truncate);
  suite_add_tcase(s, tc_negate);

  return s;
}
