#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <stdint.h>

typedef struct {
  uint32_t bits[4];
} s21_decimal;

/*
Арифметические операторы

Функции возвращают код ошибки:

    0 — OK;
    1 — число слишком велико или равно бесконечности;
    2 — число слишком мало или равно отрицательной бесконечности;
    3 — деление на 0.

    Уточнение про числа, не вмещающиеся в мантиссу:
    При получении чисел, не вмещающихся в мантиссу при арифметических операциях,
    используй банковское округление
    (например, 79,228,162,514,264,337,593,543,950,335 — 0.6 =
79,228,162,514,264,337,593,543,950,334).
*/
int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // +
int s21_sub(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // -
int s21_mul(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // *
int s21_div(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // /

/*
Операторы сравнения

Возвращаемое значение:

    0 — FALSE;
    1 — TRUE.
*/
int s21_is_less(s21_decimal, s21_decimal);              // <
int s21_is_less_or_equal(s21_decimal, s21_decimal);     // <=
int s21_is_greater(s21_decimal, s21_decimal);           // >
int s21_is_greater_or_equal(s21_decimal, s21_decimal);  // >=
int s21_is_equal(s21_decimal, s21_decimal);             // ==
int s21_is_not_equal(s21_decimal, s21_decimal);         // !=

/*
Преобразователи

Возвращаемое значение — код ошибки:
    0 — OK;
    1 — ошибка конвертации.

Уточнение про преобразование числа типа float:
    Если числа слишком малы (0 < |x| < 1e-28), возвращай ошибку и значение,
равное 0. Если числа слишком велики (|x| >
79,228,162,514,264,337,593,543,950,335) или равны бесконечности, возвращай
ошибку. При обработке числа с типом float преобразовывай все содержащиеся в нём
значимые десятичные цифры. Если таких цифр больше 7, то значение числа должно
округляться с использованием банковского округления к числу, у которого не
больше 7 значимых цифр.

Уточнение про преобразование из числа типа decimal в тип int:
    Если в числе типа decimal есть дробная часть, то её следует отбросить
(например, 0.9 преобразуется 0).

*/
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

/*
Другие функции

Возвращаемое значение — код ошибки:
    0 — OK;
    1 — ошибка вычисления.
*/
// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result);
// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result);
// Записывает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result);
// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result);

#endif  // S21_DECIMAL_H