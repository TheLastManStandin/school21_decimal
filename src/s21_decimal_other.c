#include <stddef.h>

#include "s21_decimal.h"
#include "s21_decimal_utils.h"

/*
 Возвращаемое значение:
   0 — OK
   1 — ошибка вычисления
*/

int s21_floor(s21_decimal value, s21_decimal *result) {
  int res = s21_decimal_is_valid(&value);
  if (result == NULL) {
    res = 0;
  }
  if (res) {
    int scale = s21_get_scale(&value);
    if (scale == 0) {
      *result = value;
    } else {
      int sign = s21_get_sign(&value);
      s21_decimal res_time = {0};
      s21_truncate(value, &res_time);
      int flag = s21_is_equal(value, res_time);
      if (sign == 0) {
        *result = res_time;
      } else if (sign == 1 && (flag == 0)) {
        s21_decimal one = {0};
        one.bits[0] = 1;
        s21_sub(res_time, one, result);
      } else if (sign == 1 && (flag != 0)) {
        *result = res_time;
      }
    }
  }
  return !res;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int res = s21_decimal_is_valid(&value);
  if (result == NULL) {
    res = 0;
  }
  if (res) {
    *result = value;
    s21_reduce_scale_with_round(result, 0);
  }
  return !res;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int res = s21_decimal_is_valid(&value);
  if (result == NULL) {
    res = 0;
  }
  if (res) {
    int sign = s21_get_sign(&value);
    int count = s21_get_scale(&value);
    uint32_t del = 10;
    *result = value;
    for (int i = 0; i < count; i++) {
      s21_div_mantissa_by_uint32(result, del);
    }
    result->bits[3] = (result->bits[3] & 0) | (sign << 31);
  }
  return !res;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int res = s21_decimal_is_valid(&value);
  if (result == NULL) {
    res = 0;
  }
  if (res) {
    uint32_t sign = value.bits[3] >> 31;
    uint32_t mask = 1;
    *result = value;
    if (sign == 1) {
      result->bits[3] = result->bits[3] & (~(mask << 31));
    } else {
      result->bits[3] = result->bits[3] | (mask << 31);
    }
  }
  return !res;
}
