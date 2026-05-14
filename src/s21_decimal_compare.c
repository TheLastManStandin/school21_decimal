#include "s21_decimal.h"
#include "s21_decimal_utils.h"

/*
 s21_decimal_compare.c

 Возвращаемое значение:
   0 — FALSE
   1 — TRUE

 ВАЖНО:
 - Реализаций пока нет — только каркас и комментарии.
*/

int s21_is_less(s21_decimal a, s21_decimal b) {
  int a_is_zero = s21_decimal_is_zero(&a);
  int b_is_zero = s21_decimal_is_zero(&b);
  int a_sign = s21_get_sign(&a);
  int b_sign = s21_get_sign(&b);
  int less = 0;
  if (a_is_zero == true && b_is_zero == true) {
    less = 0;
  } else if (a_sign == 0 && b_sign == 1) {
    less = 0;
  } else if (b_sign == 0 && a_sign == 1) {
    less = 1;
  } else if (a_sign == 0 && b_sign == 0) {
    int norm_a = s21_normalize_scales(&a, &b);
    if (norm_a == 0) {
      int res = s21_cmp_mantissa(&a, &b);
      if (res == -1) {
        less = 1;
      } else {
        less = 0;
      }
    } else {
      less = 0;
    }
  } else if (a_sign == 1 && b_sign == 1) {
    int norm_a = s21_normalize_scales(&a, &b);
    if (norm_a == 0) {
      int res = s21_cmp_mantissa(&a, &b);
      if (res == 1) {
        less = 1;
      } else {
        less = 0;
      }
    } else {
      less = 0;
    }
  }
  return less;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  int a_is_zero = s21_decimal_is_zero(&a);
  int b_is_zero = s21_decimal_is_zero(&b);

  int a_sign = s21_get_sign(&a);
  int b_sign = s21_get_sign(&b);
  int less = 0;
  if (a_is_zero == true && b_is_zero == true) {
    less = 1;
  } else if (a_sign == 0 && b_sign == 1) {
    less = 0;
  } else if (b_sign == 0 && a_sign == 1) {
    less = 1;
  } else if (a_sign == 0 && b_sign == 0) {
    int norm_a = s21_normalize_scales(&a, &b);
    if (norm_a == 0) {
      int res = s21_cmp_mantissa(&a, &b);
      if (res == -1 || res == 0) {
        less = 1;
      } else {
        less = 0;
      }
    } else {
      less = 0;
    }
  } else if (a_sign == 1 && b_sign == 1) {
    int norm_a = s21_normalize_scales(&a, &b);
    if (norm_a == 0) {
      int res = s21_cmp_mantissa(&a, &b);
      if (res == 1 || res == 0) {
        less = 1;
      } else {
        less = 0;
      }
    } else {
      less = 0;
    }
  }
  return less;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  return !s21_is_less_or_equal(a, b);
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  return s21_is_greater(a, b) || s21_is_equal(a, b);
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
  int a_is_zero = s21_decimal_is_zero(&a);
  int b_is_zero = s21_decimal_is_zero(&b);

  int a_sign = s21_get_sign(&a);
  int b_sign = s21_get_sign(&b);

  int equal = 0;
  if (a_is_zero == true && b_is_zero == true) {
    equal = 1;
  } else if (a_sign == b_sign) {
    int norm_a = s21_normalize_scales(&a, &b);
    if (norm_a == 0) {
      int res = s21_cmp_mantissa(&a, &b);
      if (res == 0) {
        equal = 1;
      } else {
        equal = 0;
      }
    } else {
      equal = 0;
    }
  }
  return equal;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return !s21_is_equal(a, b);
}
