#ifndef SRC_S21_DECIMAL_H_
    #define SRC_S21_DECIMAL_H_

#include <stdio.h>
#include <math.h>

typedef struct {
    unsigned int bits[4];
} s21_decimal;

typedef struct {
    unsigned int bits[6];
} long_decimal;

typedef union {
    float a;
    unsigned int b;
} float_transf;


// ----------------------- Основные: ---------------------------------------------
// Арифметические операторы
int     s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int     s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int     s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int     s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int     s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// Операторы сравнения
int     s21_is_less(s21_decimal x, s21_decimal y);
int     s21_is_less_or_equal(s21_decimal x, s21_decimal y);
int     s21_is_greater(s21_decimal, s21_decimal);
int     s21_is_greater_or_equal(s21_decimal, s21_decimal);
int     s21_is_equal(s21_decimal x, s21_decimal y);
int     s21_is_not_equal(s21_decimal x, s21_decimal y);

// Преобразователи
int     s21_from_decimal_to_int(s21_decimal src, int *dst);
int     s21_from_decimal_to_float(s21_decimal src, float *dst);
int     s21_from_float_to_decimal(float f, s21_decimal *dst);
int     s21_from_int_to_decimal(int src, s21_decimal *dst);



// Другие функции
int     s21_truncate(s21_decimal value, s21_decimal *result);
int     s21_negate(s21_decimal value, s21_decimal *result);
int     s21_round(s21_decimal src, s21_decimal *dst);
int     s21_floor(s21_decimal src, s21_decimal *dst);



// ---------------------Вспомогательные:------------------------------------------




// Битовые операции
int     get_bit(s21_decimal num, int pos);
void    set_bit(s21_decimal* num, int pos);
void    unset_bit(s21_decimal* num, int pos);
void    change_bit(s21_decimal* num, int pos);

// Операции с числами s21_decimal
void    copy_decimal(s21_decimal a, s21_decimal* res);
void    change_sign(s21_decimal a, s21_decimal b, s21_decimal *res);
void    left_shift(s21_decimal* num, int shift);
void    get_twos_complement(s21_decimal* num);
void    sum_pows(s21_decimal a, s21_decimal b, s21_decimal *res);
void    sub_pows(s21_decimal a, s21_decimal b, s21_decimal *res);
void    set_zero(s21_decimal* num);
int     s21_is_null(s21_decimal num);
int     get_pow(s21_decimal a);
void    set_pow(s21_decimal *a, int pow);
void    set_sign(s21_decimal *d, int s);
int     get_sign(s21_decimal a);
int     compare_bits(s21_decimal a, s21_decimal b);
int     s21_dec_mant_div_10(s21_decimal source, s21_decimal *dst);
int     s21_dec_normal(s21_decimal x, s21_decimal y, s21_decimal *x1, s21_decimal *y1);
void    set_null(s21_decimal* num);
void    reverse(s21_decimal *a);
int     s21_is_gr_or_eq_bits_only(s21_decimal a, s21_decimal b);
int     s21_add_bits_only(s21_decimal a, s21_decimal b, s21_decimal *res, int is_it_sub);
int     s21_sub_bits_only(s21_decimal a, s21_decimal b, s21_decimal *res);
int     s21_dec_mant_mul_10(s21_decimal source, s21_decimal *dst);
int     s21_dec_normal(s21_decimal x, s21_decimal y, s21_decimal *x1, s21_decimal *y1);
int     s21_dec_mant_div_10(s21_decimal source, s21_decimal *dst);
int     s21_new(unsigned int x, int e, int sign, s21_decimal *dst);
int     s21_is_greater_aux(s21_decimal x, s21_decimal y);

// Функции для деления с использованием типа long_decimal:
void    left_shift_long(long_decimal* long_x, int shift);
int     get_bit_long(long_decimal long_x, int pos);
void    set_bit_long(long_decimal* long_x, int pos);
int     compare_bits_long(long_decimal long_x, long_decimal long_y);
void    long_set_zero(long_decimal* num);
int     long_add_bits_only(long_decimal a, long_decimal b, long_decimal *res);
void    long_get_twos_complement(long_decimal* num);
int     long_sub_bits_only(long_decimal a, long_decimal b, long_decimal *res);
void    copy_long_decimal(long_decimal a, long_decimal* res);

#endif  // SRC_S21_DECIMAL_H_
