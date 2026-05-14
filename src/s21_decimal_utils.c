#include "s21_decimal_utils.h"

// Базовые функции
/*
  Обнуляет весь decimal: мантиссу, scale и sign.
  После реализации должна делать d->bits[0..3] = 0.
*/
void s21_zero_decimal(s21_decimal *d) {
  for (int i = 0; i < DECIMAL_WORDS; i++) {
    d->bits[i] = 0;
  }
}

/*
  Возвращает true, если значение decimal равно 0.
  Проверяется мантисса (bits[0..2]); sign/scale не важны для нуля.
*/
bool s21_decimal_is_zero(const s21_decimal *d) {
  return (d->bits[0] == 0 && d->bits[1] == 0 && d->bits[2] == 0);
}

/*
  Проверяет корректность внутреннего представления:
   - scale в допустимом диапазоне (обычно 0..28)
   - зарезервированные биты в bits[3] равны 0
   - формат знака/scale корректный
  Возвращает true, если decimal валиден.
*/
bool s21_decimal_is_valid(const s21_decimal *d) {
  if (!d) return false;

  bool res = true;

  uint32_t scale = (d->bits[DECIMAL_WORDS - 1] >> SCALE_SHIFT) & SCALE_MASK;
  if (scale > MAX_SCALE) res = false;

  if ((d->bits[DECIMAL_WORDS - 1] & RESERVED_BIT_MASK) != 0) res = false;

  return res;
}

// копирование decimal d в dcopy, 0 - OK, 1 - false
int s21_copy_decimal(const s21_decimal *src, s21_decimal *dst) {
  if (!src || !dst) return 1;

  for (int i = 0; i < DECIMAL_WORDS; ++i) {
    dst->bits[i] = src->bits[i];
  }

  return 0;
}

// Работа с любыми битами (0..127)
/*
  Возвращает значение бита по глобальному индексу 0..127.
  Биты 0..95 — мантисса, bits[3] содержит знак/scale и служебные.
*/
int s21_get_bit_raw(const s21_decimal *d, int bit) {
  int res = -1;

  if (bit >= 0 && bit < DECIMAL_TOTAL_BITS) {
    int word = bit / WORD_BITS;
    int shift = bit % WORD_BITS;
    res = (d->bits[word] >> shift) & 1u;
  }

  return res;
}

/*
  Устанавливает/сбрасывает бит по глобальному индексу 0..127.
  val 0 или 1.
*/
void s21_set_bit_raw(s21_decimal *d, int bit, int val) {
  if (bit >= 0 && bit < DECIMAL_TOTAL_BITS) {
    int word = bit / WORD_BITS;
    int shift = bit % WORD_BITS;

    if (val == 1) {
      d->bits[word] |= (1u << shift);
    } else if (val == 0) {
      d->bits[word] &= ~(1u << shift);
    }
  }
}

// Знак
/*
  Возвращает знак числа: 0 — плюс, 1 — минус.
  Обычно хранится в бите S21_DEC_SIGN_BIT (127).
*/
int s21_get_sign(const s21_decimal *d) {
  return (d->bits[DECIMAL_WORDS - 1] >> SIGN_BIT) & 1u;
}

/*
  Устанавливает знак числа (0/1).
  Должна корректно выставлять sign-bit и не трогать мантиссу/scale.
*/
void s21_set_sign(s21_decimal *d, int sign) {
  if (sign == 1) {
    d->bits[DECIMAL_WORDS - 1] |= (1u << SIGN_BIT);
  } else if (sign == 0) {
    d->bits[DECIMAL_WORDS - 1] &= ~(1u << SIGN_BIT);
  }
}

// Scale
/*
  Возвращает scale (количество цифр после запятой).
  Обычно scale хранится в bits[3] на позиции S21_DEC_SCALE_SHIFT и маске
  S21_DEC_SCALE_MASK.
*/
int s21_get_scale(const s21_decimal *d) {
  return (d->bits[DECIMAL_WORDS - 1] >> SCALE_SHIFT) & SCALE_MASK;
}

/*
  Устанавливает scale.
  Должна:
   - проверить диапазон scale (обычно 0..28)
   - записать scale в bits[3]
   - сохранить знак и не портить резервные биты
  Возвращает 0 при успехе, иначе код ошибки.
*/
int s21_set_scale(s21_decimal *d, uint8_t scale) {
  if (scale > MAX_SCALE) return 1;

  d->bits[DECIMAL_WORDS - 1] &= ~(SCALE_MASK << SCALE_SHIFT);

  d->bits[DECIMAL_WORDS - 1] |= ((uint32_t)scale << SCALE_SHIFT);

  return 0;
}

// Сравнение мантисс
/*
  Сравнивает только мантиссы (bits[0..2]) без учёта знака и scale.
  Возвращает:
    -1 если a<b,
     0 если a==b,
     1 если a>b.
*/
int s21_cmp_mantissa(const s21_decimal *a, const s21_decimal *b) {
  int res = 0;

  if (a && b) {
    for (int i = DECIMAL_MANTISSA_WORDS - 1; i >= 0 && res == 0; i--) {
      if (a->bits[i] > b->bits[i])
        res = 1;
      else if (a->bits[i] < b->bits[i])
        res = -1;
    }
  }

  return res;
}

// Арифметика мантиссы (96 бит)
/*
  Складывает мантиссы a и b (96 бит) и кладёт результат в dst.
  Не трогает sign/scale (обычно dst должен быть заранее подготовлен).
  Возвращает код переполнения (например, 1) если результат не помещается в 96
  бит.
*/
int s21_add_mantissa(const s21_decimal *a, const s21_decimal *b,
                     s21_decimal *dst) {
  uint64_t sum = 0;
  uint64_t carry = 0;
  bool overflow = false;

  for (int i = 0; i < DECIMAL_MANTISSA_WORDS; i++) {
    sum = (uint64_t)a->bits[i] + (uint64_t)b->bits[i] + carry;

    dst->bits[i] = (uint32_t)sum;
    carry = sum >> WORD_BITS;
  }

  if (carry) overflow = true;

  return overflow;
}

/*
  Вычитает мантиссу b из мантиссы a (предполагается a>=b) и кладёт результат в
  dst. Возвращает код ошибки, если a<b или при других нарушениях предположений.
*/
int s21_sub_mantissa(const s21_decimal *a, const s21_decimal *b,
                     s21_decimal *dst) {
  int status = 0;
  uint32_t borrow = 0;

  for (int i = 0; i < DECIMAL_MANTISSA_WORDS; i++) {
    uint32_t av = a->bits[i];
    uint32_t bv = b->bits[i];

    if (av >= bv + borrow) {
      dst->bits[i] = av - bv - borrow;
      borrow = 0;
    } else {
      dst->bits[i] = (uint32_t)(av + (1ULL << WORD_BITS) - bv - borrow);
      borrow = 1;
    }
  }

  if (borrow) status = 1;

  return status;
}

/*
  Умножает мантиссу на m (uint32_t).
  Возвращает код переполнения, если результат не помещается в 96 бит.
*/
int s21_mul_mantissa_by_uint32(s21_decimal *d, uint32_t m) {
  int overflow = 0;

  if (m == 0) {
    d->bits[0] = 0;
    d->bits[1] = 0;
    d->bits[2] = 0;
  } else if (m > 1) {
    uint64_t carry = 0;

    for (int i = 0; i < DECIMAL_MANTISSA_WORDS; i++) {
      uint64_t prod = (uint64_t)d->bits[i] * (uint64_t)m + carry;
      d->bits[i] = (uint32_t)prod;
      carry = prod >> WORD_BITS;
    }

    if (carry != 0) overflow = 1;
  }

  return overflow;
}

/*
  Делит мантиссу на dvr (uint32_t).
  Возвращает остаток от деления (remainder), который полезен для округления.
*/
uint32_t s21_div_mantissa_by_uint32(s21_decimal *d, uint32_t dvr) {
  uint32_t rem = 0;

  for (int i = DECIMAL_MANTISSA_WORDS - 1; i >= 0 && dvr > 1; --i) {
    uint64_t cur = ((uint64_t)rem << WORD_BITS) | (uint64_t)d->bits[i];
    uint32_t q = (uint32_t)(cur / dvr);
    rem = (uint32_t)(cur % dvr);
    d->bits[i] = q;
  }

  return rem;
}

// Добавление еденицы
/*
  Добавлет еденицу при банковском округлении
*/
int s21_add_one_for_round(s21_decimal *d) {
  int overflow = 0;
  uint64_t carry = 1;

  for (int i = 0; i < DECIMAL_MANTISSA_WORDS && carry != 0; ++i) {
    uint64_t sum = (uint64_t)d->bits[i] + carry;
    d->bits[i] = (uint32_t)sum;
    carry = sum >> WORD_BITS;
  }

  if (carry != 0) overflow = 1;

  return overflow;
}

// Выравнивание scale
/*
  Делает scale у a и b одинаковыми.
  Обычно:
   - увеличивает scale меньшего числа, умножая мантиссу на 10,
   - при невозможности (переполнение) уменьшает scale большего числа,
     деля мантиссу на 10 с банковским округлением.
  Возвращает 0 при успехе или код ошибки (если выровнять нельзя).
*/
int s21_normalize_scales(s21_decimal *a, s21_decimal *b) {
  int a_scale = s21_get_scale(a);
  int b_scale = s21_get_scale(b);
  int flag = 0;
  while (a_scale != b_scale && flag == 0) {
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t w = 0;
    if (a_scale > b_scale) {
      i = s21_mul_mantissa_by_uint32(b, TEN);
      if (i != 1) {
        b_scale++;
        s21_set_scale(b, b_scale);
      } else {
        j = s21_div_mantissa_by_uint32(a, TEN);
        if (j > 5 || (j == 5 && (a->bits[0] & 1))) w = s21_add_one_for_round(a);

        a_scale--;
        s21_set_scale(a, a_scale);
      }
      if (i == 1 && w == 1) {
        flag = 1;
      }
    } else if (a_scale < b_scale) {
      i = s21_mul_mantissa_by_uint32(a, TEN);
      if (i != 1) {
        a_scale++;
        s21_set_scale(a, a_scale);
      } else {
        j = s21_div_mantissa_by_uint32(b, TEN);
        if (j > 5 || (j == 5 && (b->bits[0] & 1))) w = s21_add_one_for_round(b);

        b_scale--;
        s21_set_scale(b, b_scale);
      }
      if (i == 1 && w == 1) {
        flag = 1;
      }
    }
  }
  return flag;
}

// Округление
/*
  Уменьшает scale числа d до target_scale с банковским округлением.
  Например, если target_scale=0 — округление до целого.
  Возвращает 0 при успехе или код ошибки при невозможности.
*/
int s21_reduce_scale_with_round(s21_decimal *d, uint8_t target_scale) {
  if (target_scale > MAX_SCALE) {
    return 1;
  }

  uint8_t scale = s21_get_scale(d);
  int ok = 1;

  if (scale <= target_scale) {
    ok = 0;
  }

  unsigned short scale_dif = scale - target_scale;
  int modulo = 0;
  int overflow = 0;

  while (scale > target_scale && ok) {
    modulo = s21_div_mantissa_by_uint32(d, 10);

    if (target_scale + 1 < scale) {  // Если не предпоследяя цифра
      if (modulo > 0 && !overflow) {
        overflow = 1;
      }
    } else {
      if ((modulo == 5 && !overflow) && (d->bits[0] % 2 == 1)) {
        s21_decimal one_mantissa_decimal = {0};
        s21_set_bit_raw(&one_mantissa_decimal, 0, 1);

        s21_add_mantissa(d, &one_mantissa_decimal, d);
      } else if ((modulo == 5 && overflow) || modulo > 5) {
        s21_decimal one_mantissa_decimal = {0};
        s21_set_bit_raw(&one_mantissa_decimal, 0, 1);

        s21_add_mantissa(d, &one_mantissa_decimal, d);
      }
    }

    scale_dif--;
    scale--;
    s21_set_scale(d, target_scale + scale_dif);
  }

  return 0;
}

// BIG DECIMAL --------------------------------------------------------------
// Обнуление
void s21_zero_big_decimal(s21_big_decimal *d) {
  for (int i = 0; i < BIG_WORDS; i++) {
    d->bits[i] = 0;
  }
}
// проверка нулевой мантисы
bool s21_big_decimal_is_zero(const s21_big_decimal *d) {
  bool result = true;

  for (int i = 0; i < BIG_MANTISSA_WORDS; i++) {
    if (d->bits[i] != 0) {
      result = false;
    }
  }

  return result;
}

// Знак
int s21_get_sign_big_decimal(const s21_big_decimal *d) {
  return (d->bits[BIG_WORDS - 1] >> SIGN_BIT) & 1u;
}

void s21_set_sign_big_decimal(s21_big_decimal *d, int sign) {
  if (sign == 1) {
    d->bits[BIG_WORDS - 1] |= (1u << SIGN_BIT);
  } else {
    d->bits[BIG_WORDS - 1] &= ~(1u << SIGN_BIT);
  }
}

// Scale
int s21_get_scale_big_decimal(const s21_big_decimal *d) {
  return (d->bits[BIG_WORDS - 1] >> SCALE_SHIFT) & SCALE_MASK;
}

int s21_set_scale_big_decimal(s21_big_decimal *d, uint8_t scale) {
  if (scale > MAX_SCALE) return 1;

  d->bits[BIG_WORDS - 1] &= ~(SCALE_MASK << SCALE_SHIFT);
  d->bits[BIG_WORDS - 1] |= ((uint32_t)scale << SCALE_SHIFT);

  return 0;
}

// переводим из decimal в big_decimal
s21_big_decimal s21_to_big(const s21_decimal *d) {
  s21_big_decimal big;
  s21_zero_big_decimal(&big);

  for (int i = 0; i < DECIMAL_MANTISSA_WORDS; i++) {
    big.bits[i] = d->bits[i];
  }

  big.bits[BIG_WORDS - 1] = d->bits[DECIMAL_WORDS - 1];

  return big;
}

// из big в обычный decimal
int s21_big_to_decimal(const s21_big_decimal *bd, s21_decimal *d) {
  int res = 0;
  s21_zero_decimal(d);

  for (int i = DECIMAL_MANTISSA_WORDS; i < BIG_MANTISSA_WORDS; i++) {
    if (bd->bits[i] != 0) {
      res = 1;
    }
  }

  if (!res) {
    for (int i = 0; i < DECIMAL_MANTISSA_WORDS; i++) {
      d->bits[i] = bd->bits[i];
    }
    d->bits[DECIMAL_WORDS - 1] = bd->bits[BIG_WORDS - 1];
  }

  return res;
}

// сравнение только мантисы
int s21_big_cmp(const s21_big_decimal *a, const s21_big_decimal *b) {
  int res = 0;
  int flag = 0;
  for (int i = BIG_WORDS - 2; i >= 0; i--) {
    if (!flag) {
      if (a->bits[i] > b->bits[i]) {
        res = 1;
        flag = 1;
      } else if (a->bits[i] < b->bits[i]) {
        res = -1;
        flag = 1;
      }
    }
  }
  return res;
}

// арифметика мантисы
// сложение
int s21_big_add_mantissa(const s21_big_decimal *a, const s21_big_decimal *b,
                         s21_big_decimal *dst) {
  uint64_t sum = 0;
  uint64_t carry = 0;
  bool overflow = false;

  for (int i = 0; i < BIG_MANTISSA_WORDS; i++) {
    sum = (uint64_t)a->bits[i] + (uint64_t)b->bits[i] + carry;

    dst->bits[i] = (uint32_t)sum;
    carry = sum >> WORD_BITS;
  }

  if (carry) overflow = true;

  return overflow;
}

// вычитание (a>=b)
int s21_big_sub_mantissa(const s21_big_decimal *a, const s21_big_decimal *b,
                         s21_big_decimal *dst) {
  int status = 0;
  uint32_t borrow = 0;

  for (int i = 0; i < BIG_MANTISSA_WORDS; i++) {
    uint32_t av = a->bits[i];
    uint32_t bv = b->bits[i];

    if (av >= bv + borrow) {
      dst->bits[i] = av - bv - borrow;
      borrow = 0;
    } else {
      dst->bits[i] = (uint32_t)(av + (1ULL << WORD_BITS) - bv - borrow);
      borrow = 1;
    }
  }

  if (borrow) status = 1;

  return status;
}
// Умножение. Возвращает 0 - ОК || 1 - Переполнение
int s21_big_mul_mantissa_by_uint32(s21_big_decimal *d, uint32_t m) {
  int overflow = 0;

  if (m == 0) {
    for (int i = 0; i < BIG_MANTISSA_WORDS; i++) {
      d->bits[i] = 0;
    }
  } else if (m > 1) {
    uint64_t carry = 0;

    for (int i = 0; i < BIG_MANTISSA_WORDS; ++i) {
      uint64_t prod = (uint64_t)d->bits[i] * (uint64_t)m + carry;
      d->bits[i] = (uint32_t)prod;
      carry = prod >> WORD_BITS;
    }

    if (carry != 0) overflow = 1;
  }

  return overflow;
}

// Деление. Возвращает остаток
uint32_t s21_big_div_mantissa_by_uint32(s21_big_decimal *d, uint32_t dvr) {
  uint32_t rem = 0;

  for (int i = BIG_MANTISSA_WORDS - 1; i >= 0 && dvr > 1; --i) {
    uint64_t cur = ((uint64_t)rem << WORD_BITS) | (uint64_t)d->bits[i];
    uint32_t q = (uint32_t)(cur / dvr);
    rem = (uint32_t)(cur % dvr);
    d->bits[i] = q;
  }

  return rem;
}

// умножение мантисс
int s21_big_mul_mantissa(const s21_big_decimal *a, const s21_big_decimal *b,
                         s21_big_decimal *dst) {
  if (!a || !b || !dst) {
    return 1;
  }

  int overflow = 0;

  s21_zero_big_decimal(dst);

  for (int i = 0; i < BIG_MANTISSA_WORDS; i++) {
    uint64_t carry = 0;

    for (int j = 0; j < BIG_MANTISSA_WORDS; j++) {
      if (i + j < BIG_MANTISSA_WORDS) {
        uint64_t cur = (uint64_t)dst->bits[i + j] +
                       (uint64_t)a->bits[i] * (uint64_t)b->bits[j] + carry;

        dst->bits[i + j] = (uint32_t)cur;
        carry = cur >> WORD_BITS;
      } else if (a->bits[i] != 0 && b->bits[j] != 0) {
        overflow = 1;
      }
    }

    if (carry != 0) {
      overflow = 1;
    }
  }

  return overflow;
}

int s21_big_add_one_for_round(s21_big_decimal *d) {
  int overflow = 0;
  uint64_t carry = 1;

  for (int i = 0; i < BIG_MANTISSA_WORDS && carry != 0; ++i) {
    uint64_t sum = (uint64_t)d->bits[i] + carry;
    d->bits[i] = (uint32_t)sum;
    carry = sum >> WORD_BITS;
  }

  if (carry != 0) overflow = 1;

  return overflow;
}

// деление
int s21_big_div_mantissa(const s21_big_decimal *a, const s21_big_decimal *b,
                         s21_big_decimal *quotient,
                         s21_big_decimal *remainder) {
  if (!a || !b || !quotient || !remainder) return 1;

  int res = 0;

  s21_zero_big_decimal(quotient);
  s21_zero_big_decimal(remainder);

  if (s21_big_decimal_is_zero(b)) {
    res = 1;
  } else {
    s21_big_decimal current = *a;

    while (s21_big_cmp(&current, b) >= 0) {
      s21_big_sub_mantissa(&current, b, &current);
      s21_big_add_one_for_round(quotient);
    }

    *remainder = current;
  }

  return res;
}
