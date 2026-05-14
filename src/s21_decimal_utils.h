#ifndef S21_DECIMAL_UTILS_H
#define S21_DECIMAL_UTILS_H

#include <stdbool.h>
#include <stdint.h>

#include "s21_decimal.h"

#define DECIMAL_WORDS 4           // 4 32-х битных слов
#define DECIMAL_TOTAL_BITS 128    // всего бит
#define DECIMAL_MANTISSA_WORDS 3  // 3 32-х битных слов
#define DECIMAL_MANTISSA_BITS 96  // биты мантиссы

#define BIG_WORDS 8            // 8 32-х битных слов
#define BIG_TOTAL_BITS 256     // всего бит
#define BIG_MANTISSA_WORDS 7   // 7 32-х битных слов
#define BIG_MANTISSA_BITS 224  // биты мантиссы

#define WORD_BITS 32      // кал-во битов в int
#define SIGN_BIT 31       // сдвиг bit в bits[3]
#define SCALE_SHIFT 16    // сдвиг scale в bits[3]
#define SCALE_MASK 0xFFu  // 8 бит под scale
#define MAX_SCALE 28      // максимальное значение scale
#define TEN 10            // десятка для сдвига
// маска зарезервированных бит, при сдвиге bits[3 || 7] на маску должны
// равняться 0
#define ALLOWED_META_MASK ((1u << SIGN_BIT) | (SCALE_MASK << SCALE_SHIFT))
#define RESERVED_BIT_MASK (~ALLOWED_META_MASK)

// Базовые функции
void s21_zero_decimal(s21_decimal *d);            // обнулить число
bool s21_decimal_is_zero(const s21_decimal *d);   // проверка на 0
bool s21_decimal_is_valid(const s21_decimal *d);  // проверка корректности
// копирование decimal d в dcopy, 0 - OK, 1 - false
int s21_copy_decimal(const s21_decimal *src, s21_decimal *dst);

// Работа с любыми битами (0..127)
int s21_get_bit_raw(const s21_decimal *d, int bit);      // получить бит
void s21_set_bit_raw(s21_decimal *d, int bit, int val);  // установить бит

// Знак
int s21_get_sign(const s21_decimal *d);       // 0 или 1
void s21_set_sign(s21_decimal *d, int sign);  // установить знак

// Scale
int s21_get_scale(const s21_decimal *d);           // получить scale
int s21_set_scale(s21_decimal *d, uint8_t scale);  // установить scale

// Сравнение мантисс
int s21_cmp_mantissa(const s21_decimal *a,
                     const s21_decimal *b);  // -1,0,1

// Арифметика мантиссы (96 бит)
int s21_add_one_for_round(
    s21_decimal *d);  // добавить к мантиссе 1 и проверить переполнение

int s21_add_mantissa(const s21_decimal *a, const s21_decimal *b,
                     s21_decimal *dst);  // сложение

int s21_sub_mantissa(const s21_decimal *a, const s21_decimal *b,
                     s21_decimal *dst);  // вычитание (a>=b)

int s21_shift_left_mantissa(s21_decimal *d, int shift);   // сдвиг влево
int s21_shift_right_mantissa(s21_decimal *d, int shift);  // сдвиг вправо

int s21_mul_mantissa_by_uint32(
    s21_decimal *d, uint32_t m);  // возвращает 0 - ОК || 1 - Переполнение
uint32_t s21_div_mantissa_by_uint32(s21_decimal *d,
                                    uint32_t dvr);  // возвращает остаток

// Выравнивание scale
int s21_normalize_scales(s21_decimal *a, s21_decimal *b);

// Округление
int s21_reduce_scale_with_round(s21_decimal *d, uint8_t target_scale);

// BIG DECIMAL --------------------------------------------------------------
typedef struct {
  uint32_t bits[8];
} s21_big_decimal;

// Базовые функции
void s21_zero_big_decimal(s21_big_decimal *d);           // обнулить число
bool s21_big_decimal_is_zero(const s21_big_decimal *d);  // проверка на 0
bool s21_big_decimal_is_valid(
    const s21_big_decimal *d);  // проверка корректности

// Работа с любыми битами
int s21_get_bit_raw_big_decimal(const s21_big_decimal *d,
                                int bit);  // получить бит
void s21_set_bit_raw_big_decimal(s21_big_decimal *d, int bit,
                                 int val);  // установить бит

// Знак
int s21_get_sign_big_decimal(const s21_big_decimal *d);       // 0 или 1
void s21_set_sign_big_decimal(s21_big_decimal *d, int sign);  // установить знак

// Scale
int s21_get_scale_big_decimal(const s21_big_decimal *d);  // получить scale
int s21_set_scale_big_decimal(s21_big_decimal *d, uint8_t scale);

// переводим из decimal в big_decimal
s21_big_decimal s21_to_big(const s21_decimal *d);
// из big в обычный decimal
int s21_big_to_decimal(const s21_big_decimal *bd, s21_decimal *d);

// сравнение только мантисы
int s21_big_cmp(const s21_big_decimal *a, const s21_big_decimal *b);

// сдвиги
void s21_big_shift_left(s21_big_decimal *bd, int shift);

void s21_big_shift_right(s21_big_decimal *bd, int shift);

// арифметика мантисы
int s21_big_add_mantissa(const s21_big_decimal *a, const s21_big_decimal *b,
                         s21_big_decimal *dst);  // сложение

int s21_big_sub_mantissa(const s21_big_decimal *a, const s21_big_decimal *b,
                         s21_big_decimal *dst);  // вычитание (a>=b)

int s21_big_mul_mantissa_by_uint32(
    s21_big_decimal *d, uint32_t m);  // возвращает 0 - ОК || 1 - Переполнение
uint32_t s21_big_div_mantissa_by_uint32(s21_big_decimal *d,
                                        uint32_t dvr);  // возвращает остаток

int s21_big_mul_mantissa(const s21_big_decimal *a, const s21_big_decimal *b,
                         s21_big_decimal *dst);

int s21_big_div_mantissa(const s21_big_decimal *a, const s21_big_decimal *b,
                         s21_big_decimal *quotient, s21_big_decimal *remainder);

void print_decimal(s21_decimal d);

#endif  // S21_DECIMAL_UTILS_H