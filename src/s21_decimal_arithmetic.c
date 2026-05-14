#include "s21_decimal.h"
#include "s21_decimal_utils.h"

/*
 s21_decimal_arithmetic.c

 ВАЖНО:
 - Возвращаемые коды ошибок:
     0 — OK
     1 — слишком велико / +inf
     2 — слишком мало / -inf
     3 — деление на 0
*/

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (s21_normalize_scales(&value_1, &value_2) == 1) {
    return (s21_get_sign(&value_1) == 1 ? 2 : 1);
  }

  int code_error = 0;

  int sign_value_1 = s21_get_sign(&value_1);
  int sign_value_2 = s21_get_sign(&value_2);

  if (sign_value_1 == sign_value_2) {
    s21_big_decimal big_1 = s21_to_big(&value_1);
    s21_big_decimal big_2 = s21_to_big(&value_2);
    s21_big_decimal tmp = {0};

    s21_big_add_mantissa(&big_1, &big_2, &tmp);
    s21_set_scale_big_decimal(&tmp, s21_get_scale(&value_1));
    s21_set_sign_big_decimal(&tmp, sign_value_1);
    code_error = s21_big_to_decimal(&tmp, result);

    if (code_error && sign_value_1 == 1) {
      code_error = 2;
    }
  } else {
    int cmp = s21_cmp_mantissa(&value_1, &value_2);
    if (cmp == 0) {
      s21_zero_decimal(result);
    } else if (cmp == 1) {
      s21_sub_mantissa(&value_1, &value_2, result);
      s21_set_scale(result, s21_get_scale(&value_1));
      s21_set_sign(result, sign_value_1);
    } else if (cmp == -1) {
      s21_sub_mantissa(&value_2, &value_1, result);
      s21_set_scale(result, s21_get_scale(&value_1));
      s21_set_sign(result, sign_value_2);
    }
  }

  if (s21_decimal_is_zero(result)) {
    s21_set_sign(result, 0);
  }

  return code_error;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (s21_normalize_scales(&value_1, &value_2) == 1) {
    return (s21_get_sign(&value_1) == 1 ? 2 : 1);
  }

  s21_zero_decimal(result);

  int code_error = 0;

  int sign_value_1 = s21_get_sign(&value_1);
  int sign_value_2 = s21_get_sign(&value_2);

  int cmp = s21_cmp_mantissa(&value_1, &value_2);

  if (sign_value_1 == 0 && sign_value_2 == 0) {  // +a - +b обычное вычитание
    if (cmp == -1) {
      s21_sub_mantissa(&value_2, &value_1, result);
      s21_set_sign(result, 1);
    } else {
      s21_sub_mantissa(&value_1, &value_2, result);
    }

  } else if (sign_value_1 == 1 &&
             sign_value_2 == 1) {  // -a - -b это сравнение модулей
    if (cmp == 1) {
      s21_sub_mantissa(&value_1, &value_2, result);
      s21_set_sign(result, 1);
    } else {
      s21_sub_mantissa(&value_2, &value_1, result);
    }

  } else if (sign_value_1 == 0 &&
             sign_value_2 == 1) {  // +a - -b превращается в сложение a + b
    s21_set_sign(&value_2, 0);
    code_error = s21_add(value_1, value_2, result);

  } else if (sign_value_1 == 1 &&
             sign_value_2 == 0) {  // -a - +b превращается в -(a + b)
    s21_set_sign(&value_1, 0);
    code_error = s21_add(value_1, value_2, result);
    s21_set_sign(result, 1);
  }

  s21_set_scale(result, s21_get_scale(&value_1));

  return code_error;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code_error = 0;

  int sign_result = (s21_get_sign(&value_1) != s21_get_sign(&value_2));
  int scale_result = (s21_get_scale(&value_1) + s21_get_scale(&value_2));

  s21_big_decimal tmp = {0};
  s21_big_decimal big_1 = s21_to_big(&value_1);
  s21_big_decimal big_2 = s21_to_big(&value_2);

  code_error = s21_big_mul_mantissa(&big_1, &big_2, &tmp);

  if (!code_error) {
    code_error = s21_big_to_decimal(&tmp, result);
    s21_set_sign(result, sign_result);
    s21_set_scale(result, scale_result);
    if (s21_decimal_is_zero(result)) s21_set_sign(result, 0);
  }

  if (s21_decimal_is_zero(result)) s21_set_sign(result, 0);

  if (code_error && sign_result) code_error = 2;

  return code_error;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (s21_decimal_is_zero(&value_2)) return 3;
  int code_error = 0;

  int sign_result = (s21_get_sign(&value_1) != s21_get_sign(&value_2));
  int scale_result = (s21_get_scale(&value_1) - s21_get_scale(&value_2));

  s21_big_decimal quotient = {0};
  s21_big_decimal remainder = {0};
  s21_big_decimal big_1 = s21_to_big(&value_1);
  s21_big_decimal big_2 = s21_to_big(&value_2);

  code_error = s21_big_div_mantissa(&big_1, &big_2, &quotient, &remainder);

  int extra_scale = 0;

  while (!s21_big_decimal_is_zero(&remainder) &&
         scale_result + extra_scale < 28) {
    s21_big_mul_mantissa_by_uint32(&remainder, 10);
    s21_big_mul_mantissa_by_uint32(&quotient, 10);

    s21_big_decimal digit = {0};
    s21_big_decimal new_remainder = {0};

    s21_big_div_mantissa(&remainder, &big_2, &digit, &new_remainder);

    s21_big_add_mantissa(&quotient, &digit, &quotient);

    remainder = new_remainder;
    extra_scale++;
  }

  scale_result += extra_scale;

  if (!code_error) {
    code_error = s21_big_to_decimal(&quotient, result);
    s21_set_sign(result, sign_result);
    s21_set_scale(result, scale_result);
  }

  if (s21_decimal_is_zero(result)) s21_set_sign(result, 0);

  if (code_error && sign_result) code_error = 2;

  return code_error;
}
