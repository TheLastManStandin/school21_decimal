#include <math.h>
#include <stdio.h>
#include <string.h>

#include "s21_decimal.h"
#include "s21_decimal_utils.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_zero_decimal(dst);
  s21_set_scale(dst, 0);
  s21_set_sign(dst, src < 0 ? 1 : 0);
  dst->bits[0] = src < 0 ? -src : src;
  return 0;
}

int s21_from_float_to_decimal_parse(float src, char *mantissa,
                                    int *exponent_final, int *sign) {
  if (isnan(src) || isinf(src)) return 1;

  *sign = (src < 0) ? 1 : 0;
  char buf[14] = {0};
  sprintf(buf, "%e", src);

  char *ptr = buf;
  int m_idx = 0;
  if (*ptr == '-') ptr++;

  while (*ptr && *ptr != '.') mantissa[m_idx++] = *ptr++;
  if (*ptr == '.') ptr++;

  int last_nonzero = m_idx;
  while (*ptr && *ptr != 'e') {
    if (*ptr != '0') last_nonzero = m_idx + 1;
    mantissa[m_idx++] = *ptr++;
  }
  mantissa[last_nonzero] = '\0';

  int exp_sign = 1, exp_value = 0;
  while (*ptr && *ptr != 'e') ptr++;
  if (*ptr == 'e') {
    ptr++;
    if (*ptr == '-') {
      exp_sign = -1;
      ptr++;
    } else if (*ptr == '+')
      ptr++;
    while (*ptr >= '0' && *ptr <= '9')
      exp_value = exp_value * 10 + (*ptr++ - '0');
  }

  *exponent_final = exp_sign * exp_value;

  int res = 0;
  if (*exponent_final > 28 || *exponent_final < -28) res = 1;

  return res;
}

int s21_from_float_to_decimal_convert(char *mantissa, int exponent_final,
                                      int sign, s21_decimal *dst) {
  int scale = (int)strlen(mantissa) - 1 - exponent_final;

  if (scale < 0) {
    scale = -scale;
    for (size_t i = 0; i < (size_t)scale; i++) {
      strcat(mantissa, "0");
    }
    scale = 0;
  }

  unsigned int value = 0;
  sscanf(mantissa, "%u", &value);

  dst->bits[0] = value;
  s21_set_scale(dst, scale);
  s21_set_sign(dst, sign);

  return 0;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  s21_zero_decimal(dst);

  char mantissa[32] = {0};
  int exponent_final = 0;
  int sign = 0;

  int err =
      s21_from_float_to_decimal_parse(src, mantissa, &exponent_final, &sign);
  if (!err) {
    err =
        s21_from_float_to_decimal_convert(mantissa, exponent_final, sign, dst);
  }

  return err;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (!s21_decimal_is_valid(&src)) {
    return 1;
  }

  s21_decimal dec;
  s21_truncate(src, &dec);
  int err = 0;

  if (dec.bits[1] != 0 || dec.bits[2] != 0 || dec.bits[0] > INT32_MAX) {
    err = 1;
  }

  if (!err) {
    *dst = dec.bits[0];

    if (s21_get_sign(&dec)) {
      *dst *= -1;
    }
  }

  return err;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!s21_decimal_is_valid(&src)) {
    return 1;
  }

  int err = 0;
  long double value = 0;

  value += src.bits[0];
  value += (long double)src.bits[1] * 4294967296.0;
  value += (long double)src.bits[2] * 18446744073709551616.0;

  int scale = (src.bits[3] >> SCALE_SHIFT) & SCALE_MASK;

  for (int i = 0; i < scale; i++) {
    value /= 10.0f;
  }

  value *= src.bits[3] >> SIGN_BIT ? -1 : 1;

  *dst = (float)value;

  return err;
}
