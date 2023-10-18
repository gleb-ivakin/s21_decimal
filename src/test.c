#include <check.h>
#include <stdio.h>
#include <string.h>
#include "s21_decimal.h"

#define OK 0
#define NEGATIVE_INF 2
#define INF 1
#define DIVISION_BY_ZERO 3

// get scale

START_TEST(get_scale_1) {
    s21_decimal x = {{0, 0, 0, 0b10000000000011110000000000000000}};
    int res1 = get_pow(x);
    int res2 = 15;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(get_scale_2) {
    s21_decimal x = {{0, 0, 0, 0b10000000000000000000000000000000}};
    int res1 = get_pow(x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(get_scale_3) {
    s21_decimal x = {{0, 0, 0, 0b10000000000011010000000000000000}};
    int res1 = get_pow(x);
    int res2 = 13;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(get_scale_4) {
    s21_decimal x = {{0, 0, 0, 0b00000000000011110000000000000000}};
    int res1 = get_pow(x);
    int res2 = 15;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(get_scale_5) {
    s21_decimal x = {{0, 0, 0, 0b00000000000000000000000000000000}};
    int res1 = get_pow(x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(get_scale_6) {
    s21_decimal x = {{0, 50, 0, 0b00000000000011010000000000000000}};
    int res1 = get_pow(x);
    int res2 = 13;
    ck_assert_int_eq(res1, res2);
}
END_TEST

// set scale

START_TEST(set_scale_1) {
    s21_decimal x = {{0, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 15);
    int res1 = get_pow(x);
    int res2 = 15;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(set_scale_2) {
    s21_decimal x = {{0, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 0);
    int res1 = get_pow(x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(set_scale_3) {
    s21_decimal x = {{0, 0, 0, 0b10000000000010000000000000000000}};
    set_pow(&x, 13);
    int res1 = get_pow(x);
    int res2 = 13;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(set_scale_4) {
    s21_decimal x = {{0, 0, 0, 0b00000000000000110000000000000000}};
    set_pow(&x, 15);
    int res1 = get_pow(x);
    int res2 = 15;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(set_scale_5) {
    s21_decimal x = {{0, 0, 0, 0b00000000000001000000000000000000}};
    set_pow(&x, 0);
    int res1 = get_pow(x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(set_scale_6) {
    s21_decimal x = {{0, 50, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 13);
    int res1 = get_pow(x);
    int res2 = 13;
    ck_assert_int_eq(res1, res2);
}
END_TEST

// equaliser

START_TEST(equal_scale_1) {
    s21_decimal x = {{10, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 1);
    set_pow(&y, 0);
    s21_decimal a, b;
    s21_dec_normal(x, y, &a, &b);
    s21_decimal x_test = {{10, 0, 0, 65536}};
    s21_decimal y_test = {{0, 0, 0, 65536}};
    char res1[1000], res2[1000];
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             a.bits[0], a.bits[1], a.bits[2], a.bits[3], b.bits[0], b.bits[1], b.bits[2], b.bits[3]);
    snprintf(res2, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             x_test.bits[0], x_test.bits[1], x_test.bits[2], x_test.bits[3],
             y_test.bits[0], y_test.bits[1], y_test.bits[2], y_test.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(equal_scale_2) {
    s21_decimal x = {{10, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{1, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 2);
    set_pow(&y, 0);
    s21_decimal a, b;
    s21_dec_normal(x, y, &a, &b);
    s21_decimal x_test = {{10, 0, 0, 131072}};
    s21_decimal y_test = {{100, 0, 0, 131072}};
    char res1[1000], res2[1000];
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             a.bits[0], a.bits[1], a.bits[2], a.bits[3], b.bits[0], b.bits[1], b.bits[2], b.bits[3]);
    snprintf(res2, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             x_test.bits[0], x_test.bits[1], x_test.bits[2], x_test.bits[3],
             y_test.bits[0], y_test.bits[1], y_test.bits[2], y_test.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(equal_scale_3) {
    s21_decimal x = {{10, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{1, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 28);
    set_pow(&y, 0);
    s21_decimal a, b;
    s21_dec_normal(x, y, &a, &b);
    s21_decimal x_test = {{10, 0, 0, 1835008}};
    s21_decimal y_test = {{268435456, 1042612833, 542101086, 1835008}};
    char res1[1000], res2[1000];
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             a.bits[0], a.bits[1], a.bits[2], a.bits[3], b.bits[0], b.bits[1], b.bits[2], b.bits[3]);
    snprintf(res2, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             x_test.bits[0], x_test.bits[1], x_test.bits[2], x_test.bits[3],
             y_test.bits[0], y_test.bits[1], y_test.bits[2], y_test.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(equal_scale_4) {
    s21_decimal x = {{10, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{5, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 28);
    set_pow(&y, 0);
    s21_decimal a, b;
    s21_dec_normal(x, y, &a, &b);
    s21_decimal x_test = {{10, 0, 0, 1835008}};
    s21_decimal y_test = {{1342177280, 918096869, 2710505431, 1835008}};
    char res1[1000], res2[1000];
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             a.bits[0], a.bits[1], a.bits[2], a.bits[3], b.bits[0], b.bits[1], b.bits[2], b.bits[3]);
    snprintf(res2, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             x_test.bits[0], x_test.bits[1], x_test.bits[2], x_test.bits[3],
             y_test.bits[0], y_test.bits[1], y_test.bits[2], y_test.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(equal_scale_7) {
    s21_decimal x = {{10, 0, 100, 0b00000000000000000000000000000000}};
    s21_decimal y = {{5, 0, 1565, 0b00000000000000000000000000000000}};
    set_pow(&x, 7);
    set_pow(&y, 0);
    s21_decimal a, b;
    s21_dec_normal(x, y, &a, &b);
    s21_decimal x_test = {{1, 0, 10, 393216}};
    s21_decimal y_test = {{5000000, 0, 1565000000, 393216}};
    char res1[1000], res2[1000];
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             a.bits[0], a.bits[1], a.bits[2], a.bits[3], b.bits[0], b.bits[1], b.bits[2], b.bits[3]);
    snprintf(res2, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             x_test.bits[0], x_test.bits[1], x_test.bits[2], x_test.bits[3],
             y_test.bits[0], y_test.bits[1], y_test.bits[2], y_test.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(equal_scale_8) {
    s21_decimal x = {{10, 0, 3000000000, 0b10000000000000000000000000000000}};
    s21_decimal y = {{13, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 28);
    set_pow(&y, 0);
    s21_decimal a, b;
    s21_dec_normal(x, y, &a, &b);
    s21_decimal x_test = {{1, 0, 300000000, 2149253120}};
    s21_decimal y_test = {{3355443200, 496403223, 704731412, 2149253120}};
    char res1[1000], res2[1000];
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             a.bits[0], a.bits[1], a.bits[2], a.bits[3], b.bits[0], b.bits[1], b.bits[2], b.bits[3]);
    snprintf(res2, sizeof(char)*1000, "%u %u %u %u %u %u %u %u",
             x_test.bits[0], x_test.bits[1], x_test.bits[2], x_test.bits[3],
             y_test.bits[0], y_test.bits[1], y_test.bits[2], y_test.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

// is less

START_TEST(decimal_is_less_1) {
    s21_decimal x = {{4294967295, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 1, 0, 0b00000000000000000000000000000000}};
    int res1 = s21_is_less(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_2) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b00000000000000000000000000000000}};
    int res1 = s21_is_less(y, x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_3) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_less(y, x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_4) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b10000000000000000000000000000000}};
    int res1 = s21_is_less(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_5) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b10000000000000000000000000000000}};
    int res1 = s21_is_less(y, x);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_6) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_less(y, x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_7) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_less(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_8) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_less(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

// is less or equal

START_TEST(decimal_is_less_or_equal_1) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b00000000000000000000000000000000}};
    int res1 = s21_is_less_or_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_or_equal_2) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b00000000000000000000000000000000}};
    int res1 = s21_is_less_or_equal(y, x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_or_equal_3) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_less_or_equal(y, x);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_or_equal_4) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b10000000000000000000000000000000}};
    int res1 = s21_is_less_or_equal(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_or_equal_5) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b10000000000000000000000000000000}};
    int res1 = s21_is_less_or_equal(y, x);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_or_equal_6) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_less_or_equal(y, x);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_or_equal_7) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_less_or_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_less_or_equal_8) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_less_or_equal(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

// is greater

START_TEST(decimal_is_greater_1) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b00000000000000000000000000000000}};
    int res1 = s21_is_greater(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_2) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b00000000000000000000000000000000}};
    int res1 = s21_is_greater(y, x);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_3) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_greater(y, x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_4) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b10000000000000000000000000000000}};
    int res1 = s21_is_greater(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_5) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b10000000000000000000000000000000}};
    int res1 = s21_is_greater(y, x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_6) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_greater(y, x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_7) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_greater(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_8) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_greater(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

// is greater or equal

START_TEST(decimal_is_greater_or_equal_1) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b00000000000000000000000000000000}};
    int res1 = s21_is_greater_or_equal(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_or_equal_2) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b00000000000000000000000000000000}};
    int res1 = s21_is_greater_or_equal(y, x);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_or_equal_3) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_greater_or_equal(y, x);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_or_equal_4) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b10000000000000000000000000000000}};
    int res1 = s21_is_greater_or_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_or_equal_5) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 2, 0b10000000000000000000000000000000}};
    int res1 = s21_is_greater_or_equal(y, x);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_or_equal_6) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_greater_or_equal(y, x);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_or_equal_7) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_greater_or_equal(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_greater_or_equal_8) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_greater_or_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

// is equal

START_TEST(decimal_is_equal_1) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_equal_2) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_equal_3) {
    s21_decimal x = {{0, 0, 1, 0b0000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_equal(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_equal_4) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_equal(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_equal_5) {
    s21_decimal x = {{0, 52, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_equal(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_equal_6) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{1747, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_equal(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_equal_7) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{1747, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_equal(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_equal_8) {
    s21_decimal x = {{0, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 0, 0b00000000000000000000000000000000}};
    int res1 = s21_is_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

// is not equal

START_TEST(decimal_is_not_equal_1) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_not_equal(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_not_equal_2) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_not_equal(x, y);
    int res2 = 0;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_not_equal_3) {
    s21_decimal x = {{0, 0, 1, 0b0000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b10000000000000000000000000000000}};
    int res1 = s21_is_not_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_not_equal_4) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_not_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_not_equal_5) {
    s21_decimal x = {{0, 52, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_not_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_not_equal_6) {
    s21_decimal x = {{0, 0, 1, 0b00000000000000000000000000000000}};
    s21_decimal y = {{1747, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_not_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(decimal_is_not_equal_7) {
    s21_decimal x = {{0, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{1747, 0, 1, 0b00000000000000000000000000000000}};
    int res1 = s21_is_not_equal(x, y);
    int res2 = 1;
    ck_assert_int_eq(res1, res2);
}
END_TEST


// floor

START_TEST(s21_floor_1) {
    s21_decimal x = {{15008, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 3);
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    s21_floor(x, &z);
    char res1[1000], res2[1000] = "15 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_floor_2) {
    s21_decimal x = {{15008, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 3);
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    s21_floor(x, &z);
    char res1[1000], res2[1000] = "15 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_floor_3) {
    s21_decimal x = {{15008, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 10);
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    s21_floor(x, &z);
    char res1[1000], res2[1000] = "0 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

// round

START_TEST(s21_round_1) {
    s21_decimal x = {{95008, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 0);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "95008 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_round_2) {
    s21_decimal x = {{95008, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 1);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "9501 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_round_3) {
    s21_decimal x = {{95008, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 2);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "950 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_round_4) {
    s21_decimal x = {{95008, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 4);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "10 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_round_5) {
    s21_decimal x = {{95008, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 5);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "1 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_round_6) {
    s21_decimal x = {{95008, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 6);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_round_7) {
    s21_decimal x = {{95008, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 0);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "95008 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_round_8) {
    s21_decimal x = {{95008, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 1);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "9501 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_round_9) {
    s21_decimal x = {{95008, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 2);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "950 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_round_10) {
    s21_decimal x = {{95008, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 4);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "10 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_round_11) {
    s21_decimal x = {{95008, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 5);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "1 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_round_12) {
    s21_decimal x = {{95008, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 6);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_round(x, &z);
    char res1[1000], res2[1000] = "0 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

// negate

START_TEST(s21_negate_1) {
    s21_decimal x = {{100, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    s21_negate(x, &z);
    char res1[1000], res2[1000] = "100 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_negate_2) {
    s21_decimal x = {{100, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    s21_negate(x, &z);
    char res1[1000], res2[1000] = "100 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

// from int to decimal

START_TEST(from_int_to_decimal_1) {
    s21_decimal x = {{0, 0, 0, 0}};
    s21_decimal y = {{15, 0, 0, 0}};
    int z = 15;
    s21_from_int_to_decimal(z, &x);
    char res1[1000], res2[1000];
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", x.bits[0], x.bits[1], x.bits[2], x.bits[3]);
    snprintf(res2, sizeof(char)*1000, "%u %u %u %u", y.bits[0], y.bits[1], y.bits[2], y.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(from_int_to_decimal_2) {
    s21_decimal x = {{0, 0, 0, 0}};
    s21_decimal y = {{15, 0, 0, 2147483648}};
    int z = -15;
    s21_from_int_to_decimal(z, &x);
    char res1[1000], res2[1000];
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", x.bits[0], x.bits[1], x.bits[2], x.bits[3]);
    snprintf(res2, sizeof(char)*1000, "%u %u %u %u", y.bits[0], y.bits[1], y.bits[2], y.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(from_int_to_decimal_3) {
    s21_decimal x = {{0, 0, 0, 0}};
    s21_decimal y = {{0, 0, 0, 0}};
    int z = 0;
    s21_from_int_to_decimal(z, &x);
    char res1[1000], res2[1000];
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", x.bits[0], x.bits[1], x.bits[2], x.bits[3]);
    snprintf(res2, sizeof(char)*1000, "%u %u %u %u", y.bits[0], y.bits[1], y.bits[2], y.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(from_int_to_decimal_4) {
    int z = s21_from_int_to_decimal(15, 0);
    ck_assert_int_eq(z, 1);
}
END_TEST

// from decimal to int

START_TEST(from_decimal_to_int_1) {
    s21_decimal x = {{150, 13658, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 10);
    int y;
    s21_from_decimal_to_int(x, &y);
    ck_assert_int_eq(y, -5866);
}
END_TEST

START_TEST(from_decimal_to_int_2) {
    s21_decimal x = {{150, 13658, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 10);
    int y;
    s21_from_decimal_to_int(x, &y);
    ck_assert_int_eq(y, 5866);
}
END_TEST

START_TEST(from_decimal_to_int_3) {
    s21_decimal x = {{150, 13658, 155, 0b00000000000000000000000000000000}};
    set_pow(&x, 10);
    int y;
    s21_from_decimal_to_int(x, &y);
    ck_assert_int_eq(y, 0);
}
END_TEST

START_TEST(from_decimal_to_int_4) {
    s21_decimal x = {{2100000000, 0, 0, 0b00000000000000000000000000000000}};
//    set_pow(&x, 0);
    int y;
    s21_from_decimal_to_int(x, &y);
    ck_assert_int_eq(y, 2100000000);
}
END_TEST

START_TEST(from_decimal_to_int_5) {
    s21_decimal x = {{2100000000, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 0);
    int y;
    s21_from_decimal_to_int(x, &y);
    ck_assert_int_eq(y, -2100000000);
}
END_TEST

START_TEST(from_decimal_to_int_6) {
    s21_decimal x = {{3100000000, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 0);
    int y;
    s21_from_decimal_to_int(x, &y);
    ck_assert_int_eq(y, 0);
}
END_TEST

START_TEST(from_decimal_to_int_7) {
    s21_decimal x = {{9, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 1);
    int y;
    s21_from_decimal_to_int(x, &y);
    ck_assert_int_eq(y, 0);
}
END_TEST

// from float to decimal

START_TEST(from_float_to_decimal_1) {
    float src = 56.986;
    s21_decimal z = {{0, 0, 0, 0}};
    int n1 = s21_from_float_to_decimal(src, &z);
    char res1[1000], res2[1000] = "56986 0 0 196608";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
    ck_assert_int_eq(n1, 0);
}
END_TEST

START_TEST(from_float_to_decimal_2) {
    float src = 1.0 / 0.0;
    s21_decimal z = {{0, 0, 0, 0}};
    int n1 = s21_from_float_to_decimal(src, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
    ck_assert_int_eq(n1, 1);
}
END_TEST

START_TEST(from_float_to_decimal_3) {
    float src = -1.596;
    s21_decimal z = {{0, 0, 0, 0}};
    int n1 = s21_from_float_to_decimal(src, &z);
    char res1[1000], res2[1000] = "1596 0 0 2147680256";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
    ck_assert_int_eq(n1, 0);
}
END_TEST

// s21_from_decimal_to_float;

START_TEST(from_decimal_to_float_1) {
    s21_decimal x = {{15008, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 3);
    float res1 = 0;
    int n1 = s21_from_decimal_to_float(x, &res1);
    float res2 = 15.008;
    ck_assert_float_eq(res1, res2);
    ck_assert_int_eq(n1, 0);
}
END_TEST

START_TEST(from_decimal_to_float_2) {
    s21_decimal x = {{15008, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 3);
    float res1 = 0;
    int n1 = s21_from_decimal_to_float(x, &res1);
    float res2 = -15.008;
    ck_assert_float_eq(res1, res2);
    ck_assert_int_eq(n1, 0);
}
END_TEST

START_TEST(from_decimal_to_float_3) {
    s21_decimal x = {{15008, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 3);
    float res1 = 0;
    int n1 = s21_from_decimal_to_float(x, NULL);
    float res2 = 0;
    ck_assert_float_eq(res1, res2);
    ck_assert_int_eq(n1, 1);
}
END_TEST

// s21_from_float_to_decimal

START_TEST(s21_from_float_to_decimal_2) {
    s21_decimal val = {{0, 0, 0, 0}};
    s21_from_float_to_decimal(-128.023, &val);
    ck_assert_int_eq(val.bits[0], 12802299);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(get_bit(val, 127), 1);
}
END_TEST

START_TEST(s21_from_float_to_decimal_3) {
    s21_decimal val = {{0, 0, 0, 0}};
    s21_from_float_to_decimal(-2.1474836E+09, &val);
    ck_assert_int_eq(val.bits[0], 2147483600);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(get_bit(val, 127), 1);
}
END_TEST

START_TEST(s21_from_float_to_decimal_5) {
    s21_decimal val = {{0, 0, 0, 0}};
    s21_from_float_to_decimal(1.02E+09, &val);
    ck_assert_int_eq(val.bits[0], 1020000000);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_6) {
    s21_decimal val = {{0, 0, 0, 0}};
    s21_from_float_to_decimal(-333.2222, &val);
    ck_assert_int_eq(val.bits[0], 3332222);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(get_bit(val, 127), 1);
}
END_TEST

START_TEST(s21_from_float_to_decimal_7) {
    s21_decimal val = {{0, 0, 0, 0}};
    float a = 1.0 / 0.0;
    s21_from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_8) {
    s21_decimal val = {{0, 0, 0, 0}};
    float a = -1.0 / 0.0;
    s21_from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_9) {
    s21_decimal val = {{0, 0, 0, 0}};
    float a = 0 / 0.0;
    s21_from_float_to_decimal(a, &val);
    ck_assert_int_eq(val.bits[0], 0);
    ck_assert_int_eq(val.bits[1], 0);
    ck_assert_int_eq(val.bits[2], 0);
    ck_assert_int_eq(val.bits[3], 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_1) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    src.bits[0] = 18122;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147680256;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq_tol(number, -18.122, 0.001);
    ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_2) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    src.bits[0] = 1812;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147483648;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq_tol(number, -1812, 0.001);
    ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_3) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    src.bits[0] = 0XFFFFFF;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq_tol(number, 16777215, 0.001);
    ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_4) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    src.bits[0] = 23450985;
    src.bits[1] = 0;
    src.bits[2] = 0;
    src.bits[3] = 2147745792;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq_tol(number, -2345.0987, 0.001);
    ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_5) {
    s21_decimal src;
    int result = 0;
    float number = 0.0;
    src.bits[0] = 4294967295;
    src.bits[1] = 4294967295;
    src.bits[2] = 0;
    src.bits[3] = 0;
    result = s21_from_decimal_to_float(src, &number);
    ck_assert_float_eq(number, 0xFFFFFFFFFFFFFFFF);
    ck_assert_int_eq(result, 0);
}
END_TEST

// add

START_TEST(decimal_add) {
    s21_decimal x = {{4294967295, 4294967295, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{1, 0, 0,                   0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_add(x, y, &z);
    char res1[1000], res2[1000] = "0 0 1 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_add_2) {
    s21_decimal x = {{20, 1, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{21, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_add(x, y, &z);
    char res1[1000], res2[1000] = "4294967295 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_add_3) {
    s21_decimal x = {{20, 1, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{21, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_add(x, y, &z);
    char res1[1000], res2[1000] = "41 1 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_add_4) {
    s21_decimal x = {{20, 1, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{21, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_add(x, y, &z);
    char res1[1000], res2[1000] = "4294967295 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_add_5) {
    s21_decimal x = {{20, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{21, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_add(x, y, &z);
    char res1[1000], res2[1000] = "1 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_add_6) {
    s21_decimal x = {{25, 1, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{21, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_add(x, y, &z);
    char res1[1000], res2[1000] = "4 1 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_add_8) {
    s21_decimal x = {{99, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{100, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_add(x, y, &z);
    char res1[1000], res2[1000] = "1 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_add_9) {
    s21_decimal x = {{0, 0, 3000000000, 1835008}};
    s21_decimal y = {{0, 0, 3000000000, 1835008}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_add(x, y, &z);
    s21_decimal f = {{0, 0, 600000000, 1769472}};
    ck_assert_int_eq(s21_is_equal(f, z), 1);
}
END_TEST

START_TEST(decimal_add_10) {
    s21_decimal x = {{0, 0, 3000000000, 0}};
    s21_decimal y = {{0, 0, 3000000000, 0}};
    s21_decimal z = {{0, 0, 0, 0}};
    ck_assert_int_eq(s21_add(x, y, &z), 1);
}
END_TEST

START_TEST(decimal_add_11) {
    s21_decimal x = {{1, 0, 0, 1835008}};
    s21_decimal y = {{1, 0, 0, 2149318656}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_add(x, y, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

// sub

START_TEST(decimal_sub_1) {
    s21_decimal x = {{1, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{1, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_sub(x, y, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_sub_2) {
    s21_decimal x = {{2, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{1, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_sub(x, y, &z);
    char res1[1000], res2[1000] = "1 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_sub_3) {
    s21_decimal x = {{1, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{2, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_sub(x, y, &z);
    char res1[1000], res2[1000] = "1 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_sub_4) {
    s21_decimal x = {{2, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{2, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_sub(x, y, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_sub_5) {
    s21_decimal x = {{1, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{2, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_sub(x, y, &z);
    char res1[1000], res2[1000] = "1 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_sub_6) {
    s21_decimal x = {{2, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{1, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_sub(x, y, &z);
    char res1[1000], res2[1000] = "1 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_sub_7) {
    s21_decimal x = {{2, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{1, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_sub(x, y, &z);
    char res1[1000], res2[1000] = "3 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_sub_8) {
    s21_decimal x = {{2, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{1, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_sub(x, y, &z);
    char res1[1000], res2[1000] = "3 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_sub_11) {
    s21_decimal x = {{1, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{1, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 28);
    set_pow(&y, 28);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_sub(x, y, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(s21_mul_1) {
    s21_decimal value_1 = {{5, 0, 0, 0}};
    s21_decimal value_2 = {{7, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal check = {{35, 0, 0, 0}};
    int return_value = s21_mul(value_1, value_2, &result);
    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(return_value, OK);
}
END_TEST

START_TEST(s21_mul_2) {
    s21_decimal value_1 = {{0xFFFFFFFF, 0, 0, 0}};
    s21_decimal value_2 = {{0xFFFFFFFF, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal check = {{1, 0xFFFFFFFE, 0, 0}};
    int return_value = s21_mul(value_1, value_2, &result);
    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(return_value, OK);
}
END_TEST

START_TEST(s21_mul_3) {
    s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal value_2 = {{2, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal check = {{0, 0, 0, 0}};
    int return_value = s21_mul(value_1, value_2, &result);
    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(return_value, INF);
}
END_TEST

START_TEST(s21_mul_4) {
    s21_decimal value_1 = {{123456u, 123u, 0, 0}};
    s21_decimal value_2 = {{654321u, 654u, 0, 0}};
    set_sign(&value_2, 1);
    set_pow(&value_1, 2);
    set_pow(&value_2, 3);
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal check = {{0xcedabe40, 0x99c0c5d, 0x13a3a, 0x80050000}};
    int return_value = s21_mul(value_1, value_2, &result);
    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(return_value, OK);
}
END_TEST

START_TEST(s21_mul_5) {
    s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal value_2 = {{2, 0, 0, 0}};
    set_sign(&value_2, 1);
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal check = {{0, 0, 0, 0}};
    int return_value = s21_mul(value_1, value_2, &result);
    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(return_value, NEGATIVE_INF);
}
END_TEST

START_TEST(s21_mul_6) {
    s21_decimal value_1 = {{17, 0, 0, 0}};
    set_sign(&value_1, 1);
    s21_decimal value_2 = {{0, 0, 0, 0}};
    s21_decimal result;
    s21_decimal check = {{0, 0, 0, 0}};
    int return_value = s21_mul(value_1, value_2, &result);
    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(return_value, OK);
}
END_TEST

START_TEST(s21_mul_8) {
    s21_decimal src1, src2;
    int a = -32768;
    int b = 32768;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = -1073741824;
    s21_decimal res_od;
    s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
}
END_TEST

START_TEST(s21_div_2) {
    s21_decimal value_1 = {{0x88888888, 0x88888888, 0x88888888, 0}};
    s21_decimal value_2 = {{0x2, 0, 0, 0}};
    set_sign(&value_2, 1);
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal check = {{0x44444444, 0x44444444, 0x44444444, 0}};
    set_sign(&check, 1);
    int return_value = s21_div(value_1, value_2, &result);
    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(return_value, OK);
}
END_TEST

START_TEST(s21_div_4) {
    s21_decimal value_1 = {{15, 0, 0, 0}};
    set_sign(&value_1, 1);
    s21_decimal value_2 = {{0, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal check = {{0, 0, 0, 0}};
    int return_value = s21_div(value_1, value_2, &result);
    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(return_value, DIVISION_BY_ZERO);
}
END_TEST

START_TEST(s21_mod_2) {
    s21_decimal value_1 = {{46, 0, 0, 0}};
    set_pow(&value_1, 1);
    s21_decimal value_2 = {{0, 0, 0, 0}};
    set_pow(&value_2, 2);
    s21_decimal check = {{0, 0, 0, 0}};
    s21_decimal result;
    int return_value = s21_mod(value_1, value_2, &result);
    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(return_value, DIVISION_BY_ZERO);
}
END_TEST

START_TEST(s21_truncate_1) {
    s21_decimal value_1 = {{35, 0, 0, 0}};
    s21_decimal check = {{3, 0, 0, 0}};
    set_pow(&value_1, 1);
    s21_truncate(value_1, &value_1);
    ck_assert_int_eq(s21_is_equal(value_1, check), 1);
}
END_TEST

START_TEST(s21_truncate_2) {
    s21_decimal value_1 = {{123456, 0, 0, 0}};
    set_pow(&value_1, 3);
    set_sign(&value_1, 1);
    s21_decimal check = {{123, 0, 0, 0}};
    set_sign(&check, 1);
    s21_truncate(value_1, &value_1);
    ck_assert_int_eq(s21_is_equal(value_1, check), 1);
}
END_TEST

START_TEST(s21_truncate_3) {
    s21_decimal src1;
    src1.bits[3] = 0x000A0000;
    src1.bits[2] = 0x0;
    src1.bits[1] = 0xFFFFFFFF;
    src1.bits[0] = 0xFFFFFFFF;
    s21_decimal result;
    result.bits[3] = 0x0;
    result.bits[2] = 0x0;
    result.bits[1] = 0x0;
    result.bits[0] = 0x6DF37F67;
    s21_decimal res_od;
    s21_truncate(src1, &res_od);
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST

START_TEST(s21_truncate_4) {
    s21_decimal src1;
    src1.bits[0] = 0b00000000000000000000000001101101;
    src1.bits[1] = 0b00000000000011000110010101011011;
    src1.bits[2] = 0b00000000000000000011000000111001;
    src1.bits[3] = 0b00000000000011100000000000000000;
    s21_decimal result;
    result.bits[0] = 0b10000111101111000001011000011110;
    result.bits[1] = 0b00000000000000000000000000000000;
    result.bits[2] = 0b00000000000000000000000000000000;
    result.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal res_od;
    s21_truncate(src1, &res_od);
    ck_assert_float_eq(res_od.bits[0], result.bits[0]);
    ck_assert_float_eq(res_od.bits[1], result.bits[1]);
    ck_assert_float_eq(res_od.bits[2], result.bits[2]);
    ck_assert_float_eq(res_od.bits[3], result.bits[3]);
}
END_TEST

//==============================================================

START_TEST(decimal_mul_1) {
    s21_decimal x = {{0, 0, 0, 0b00000000000011110000000000000000}};
    s21_decimal y = {{0, 0, 0, 0b00000000000000100000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_2) {
    s21_decimal x = {{2, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{5, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "10 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_3) {
    s21_decimal x = {{2, 1, 2, 0b00000000000000000000000000000000}};
    s21_decimal y = {{5, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "10 5 10 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_4) {
    s21_decimal x = {{2, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 10, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "0 0 20 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_5) {
    s21_decimal x = {{1000000000, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{6, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "1705032704 1 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_6) {
    s21_decimal x = {{1000000000, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{6, 0, 0,          0b10000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "1705032704 1 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_7) {
    s21_decimal x = {{1000000000, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{6, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "1705032704 1 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_8) {
    s21_decimal x = {{1000000000, 0, 0, 0b00000000000011110000000000000000}};
    s21_decimal y = {{6, 0, 0,          0b10000000000000010000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "1705032704 1 0 2148532224";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_9) {
    s21_decimal x = {{1, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{165, 22, 6587456, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "165 22 6587456 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_10) {
    s21_decimal x = {{400, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{500, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 14);
    set_pow(&y, 14);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "200000 0 0 1835008";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_11) {
    s21_decimal x = {{400, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{500, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 15);
    set_pow(&y, 14);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "20000 0 0 1835008";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_12) {
    s21_decimal x = {{400, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{500, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 15);
    set_pow(&y, 16);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "200 0 0 1835008";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_13) {
    s21_decimal x = {{400, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{500, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 17);
    set_pow(&y, 16);
    s21_decimal z = {{0, 0, 0, 0}};
    s21_mul(x, y, &z);
    char res1[1000], res2[1000] = "2 0 0 1835008";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(decimal_mul_16) {
    s21_decimal src1, src2;
    int a = 9403;
    int b = 202;
    int res_our_dec = 0;
    s21_from_int_to_decimal(a, &src1);
    s21_from_int_to_decimal(b, &src2);
    int res_origin = 1899406;
    s21_decimal res_od;
    s21_mul(src1, src2, &res_od);
    s21_from_decimal_to_int(res_od, &res_our_dec);
    ck_assert_int_eq(res_our_dec, res_origin);
}
END_TEST

START_TEST(div_1) {
    s21_decimal x = {{3100000000, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{1, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    s21_div(x, y, &z);
    char res1[1000], res2[1000] = "3100000000 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(div_3) {
    s21_decimal x = {{52525252, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    int n1 = 0;
    n1 = s21_div(x, y, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
    ck_assert_int_eq(n1, 3);
}
END_TEST

START_TEST(div_4) {
    s21_decimal x = {{52525252, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    int n1 = 0;
    n1 = s21_div(x, y, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
    ck_assert_int_eq(n1, 3);
}
END_TEST

START_TEST(div_5) {
    s21_decimal x = {{52525252, 0, 0, 0b0000000000000000000000000000000}};
    s21_decimal y = {{0, 0, 0, 0b10000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    int n1 = 0;
    n1 = s21_div(x, y, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
    ck_assert_int_eq(n1, 3);
}
END_TEST

START_TEST(div_6) {
    s21_decimal x = {{52525252, 0, 0, 8845}};
    s21_decimal y = {{0, 0, 0, 10}};
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    int n1 = 0;
    n1 = s21_div(x, y, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
    ck_assert_int_eq(n1, 3);
}
END_TEST

START_TEST(div_7) {
    s21_decimal x = {{52525252, 0, 0, 65465}};
    s21_decimal y = {{0, 0, 0, 9598595}};
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    int n1 = 0;
    n1 = s21_div(x, y, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
    ck_assert_int_eq(n1, 3);
}
END_TEST

START_TEST(div_10) {
    s21_decimal x = {{4, 0, 1, 0b10000000000000000000000000000000}};
    s21_decimal y = {{2, 0, 0, 0b00000000000000000000000000000000}};
    s21_decimal z = {{0, 0, 0, 0}};
    set_pow(&x, 0);
    set_pow(&y, 0);
    int n1 = 0;
    n1 = s21_div(x, y, &z);
    char res1[1000], res2[1000] = "2 2147483648 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
    ck_assert_int_eq(n1, 0);
}
END_TEST

// mod

START_TEST(mod_4) {
    s21_decimal x = {{91, 0, 0, 0}};
    s21_decimal y = {{95, 0, 0, 0}};
    s21_decimal z = {{0, 0, 0, 0}};
    int n1 = 0;
    n1 = s21_mod(x, y, &z);
    char res1[1000], res2[1000] = "91 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
    ck_assert_int_eq(n1, 0);
}
END_TEST

START_TEST(mod_5) {
    s21_decimal x = {{91, 0, 0, 0}};
    s21_decimal y = {{0, 0, 0, 0}};
    s21_decimal z = {{0, 0, 0, 0}};
    int n1 = 0;
    n1 = s21_mod(x, y, &z);
    char res1[1000], res2[1000] = "0 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
    ck_assert_int_eq(n1, 3);
}
END_TEST

START_TEST(truncate_1) {
    s21_decimal x = {{15008, 0, 0, 0b10000000000000000000000000000000}};
    set_pow(&x, 3);
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    s21_truncate(x, &z);
    char res1[1000], res2[1000] = "15 0 0 2147483648";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

START_TEST(truncate_2) {
    s21_decimal x = {{15008, 0, 0, 0b00000000000000000000000000000000}};
    set_pow(&x, 2);
    s21_decimal z = {{0, 0, 0, 0b00000000000000000000000000000000}};
    s21_truncate(x, &z);
    char res1[1000], res2[1000] = "150 0 0 0";
    snprintf(res1, sizeof(char)*1000, "%u %u %u %u", z.bits[0], z.bits[1], z.bits[2], z.bits[3]);
    ck_assert_str_eq(res1, res2);
}
END_TEST

int main(void) {
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, decimal_add);
    tcase_add_test(tc1_1, decimal_add_2);
    tcase_add_test(tc1_1, decimal_add_3);
    tcase_add_test(tc1_1, decimal_add_4);
    tcase_add_test(tc1_1, decimal_add_5);
    tcase_add_test(tc1_1, decimal_add_6);
    tcase_add_test(tc1_1, decimal_add_8);
    tcase_add_test(tc1_1, decimal_add_9);
    tcase_add_test(tc1_1, decimal_add_10);
    tcase_add_test(tc1_1, decimal_add_11);
    tcase_add_test(tc1_1, decimal_sub_1);
    tcase_add_test(tc1_1, decimal_sub_2);
    tcase_add_test(tc1_1, decimal_sub_3);
    tcase_add_test(tc1_1, decimal_sub_4);
    tcase_add_test(tc1_1, decimal_sub_5);
    tcase_add_test(tc1_1, decimal_sub_6);
    tcase_add_test(tc1_1, decimal_sub_7);
    tcase_add_test(tc1_1, decimal_sub_8);
    tcase_add_test(tc1_1, decimal_sub_11);

    tcase_add_test(tc1_1, get_scale_1);
    tcase_add_test(tc1_1, get_scale_2);
    tcase_add_test(tc1_1, get_scale_3);
    tcase_add_test(tc1_1, get_scale_4);
    tcase_add_test(tc1_1, get_scale_5);
    tcase_add_test(tc1_1, get_scale_6);
    tcase_add_test(tc1_1, set_scale_1);
    tcase_add_test(tc1_1, set_scale_2);
    tcase_add_test(tc1_1, set_scale_3);
    tcase_add_test(tc1_1, set_scale_4);
    tcase_add_test(tc1_1, set_scale_5);
    tcase_add_test(tc1_1, set_scale_6);
    tcase_add_test(tc1_1, equal_scale_1);
    tcase_add_test(tc1_1, equal_scale_2);
    tcase_add_test(tc1_1, equal_scale_3);
    tcase_add_test(tc1_1, equal_scale_4);
    tcase_add_test(tc1_1, equal_scale_7);
    tcase_add_test(tc1_1, equal_scale_8);
    tcase_add_test(tc1_1, decimal_is_less_1);
    tcase_add_test(tc1_1, decimal_is_less_2);
    tcase_add_test(tc1_1, decimal_is_less_3);
    tcase_add_test(tc1_1, decimal_is_less_4);
    tcase_add_test(tc1_1, decimal_is_less_5);
    tcase_add_test(tc1_1, decimal_is_less_6);
    tcase_add_test(tc1_1, decimal_is_less_7);
    tcase_add_test(tc1_1, decimal_is_less_8);
    tcase_add_test(tc1_1, decimal_is_less_or_equal_1);
    tcase_add_test(tc1_1, decimal_is_less_or_equal_2);
    tcase_add_test(tc1_1, decimal_is_less_or_equal_3);
    tcase_add_test(tc1_1, decimal_is_less_or_equal_4);
    tcase_add_test(tc1_1, decimal_is_less_or_equal_5);
    tcase_add_test(tc1_1, decimal_is_less_or_equal_6);
    tcase_add_test(tc1_1, decimal_is_less_or_equal_7);
    tcase_add_test(tc1_1, decimal_is_less_or_equal_8);
    tcase_add_test(tc1_1, decimal_is_greater_1);
    tcase_add_test(tc1_1, decimal_is_greater_2);
    tcase_add_test(tc1_1, decimal_is_greater_3);
    tcase_add_test(tc1_1, decimal_is_greater_4);
    tcase_add_test(tc1_1, decimal_is_greater_5);
    tcase_add_test(tc1_1, decimal_is_greater_6);
    tcase_add_test(tc1_1, decimal_is_greater_7);
    tcase_add_test(tc1_1, decimal_is_greater_8);
    tcase_add_test(tc1_1, decimal_is_greater_or_equal_1);
    tcase_add_test(tc1_1, decimal_is_greater_or_equal_2);
    tcase_add_test(tc1_1, decimal_is_greater_or_equal_3);
    tcase_add_test(tc1_1, decimal_is_greater_or_equal_4);
    tcase_add_test(tc1_1, decimal_is_greater_or_equal_5);
    tcase_add_test(tc1_1, decimal_is_greater_or_equal_6);
    tcase_add_test(tc1_1, decimal_is_greater_or_equal_7);
    tcase_add_test(tc1_1, decimal_is_greater_or_equal_8);
    tcase_add_test(tc1_1, decimal_is_equal_1);
    tcase_add_test(tc1_1, decimal_is_equal_2);
    tcase_add_test(tc1_1, decimal_is_equal_3);
    tcase_add_test(tc1_1, decimal_is_equal_4);
    tcase_add_test(tc1_1, decimal_is_equal_5);
    tcase_add_test(tc1_1, decimal_is_equal_6);
    tcase_add_test(tc1_1, decimal_is_equal_7);
    tcase_add_test(tc1_1, decimal_is_equal_8);

    tcase_add_test(tc1_1, decimal_is_not_equal_1);
    tcase_add_test(tc1_1, decimal_is_not_equal_2);
    tcase_add_test(tc1_1, decimal_is_not_equal_3);
    tcase_add_test(tc1_1, decimal_is_not_equal_4);
    tcase_add_test(tc1_1, decimal_is_not_equal_5);
    tcase_add_test(tc1_1, decimal_is_not_equal_6);
    tcase_add_test(tc1_1, decimal_is_not_equal_7);

    tcase_add_test(tc1_1, s21_floor_1);
    tcase_add_test(tc1_1, s21_floor_2);
    tcase_add_test(tc1_1, s21_floor_3);
    tcase_add_test(tc1_1, s21_round_1);
    tcase_add_test(tc1_1, s21_round_2);
    tcase_add_test(tc1_1, s21_round_3);
    tcase_add_test(tc1_1, s21_round_4);
    tcase_add_test(tc1_1, s21_round_5);
    tcase_add_test(tc1_1, s21_round_6);
    tcase_add_test(tc1_1, s21_round_7);
    tcase_add_test(tc1_1, s21_round_8);
    tcase_add_test(tc1_1, s21_round_9);
    tcase_add_test(tc1_1, s21_round_10);
    tcase_add_test(tc1_1, s21_round_11);
    tcase_add_test(tc1_1, s21_round_12);
    tcase_add_test(tc1_1, s21_negate_1);
    tcase_add_test(tc1_1, s21_negate_2);

    tcase_add_test(tc1_1, from_int_to_decimal_1);
    tcase_add_test(tc1_1, from_int_to_decimal_2);
    tcase_add_test(tc1_1, from_int_to_decimal_3);
    tcase_add_test(tc1_1, from_int_to_decimal_4);
    tcase_add_test(tc1_1, from_decimal_to_int_1);
    tcase_add_test(tc1_1, from_decimal_to_int_2);
    tcase_add_test(tc1_1, from_decimal_to_int_3);
    tcase_add_test(tc1_1, from_decimal_to_int_4);
    tcase_add_test(tc1_1, from_decimal_to_int_5);
    tcase_add_test(tc1_1, from_decimal_to_int_6);
    tcase_add_test(tc1_1, from_decimal_to_int_7);
    tcase_add_test(tc1_1, from_float_to_decimal_1);
    tcase_add_test(tc1_1, from_float_to_decimal_2);
    tcase_add_test(tc1_1, from_float_to_decimal_3);
    tcase_add_test(tc1_1, from_decimal_to_float_1);
    tcase_add_test(tc1_1, from_decimal_to_float_2);
    tcase_add_test(tc1_1, from_decimal_to_float_3);

    tcase_add_test(tc1_1, s21_from_float_to_decimal_2);
    tcase_add_test(tc1_1, s21_from_float_to_decimal_3);
    tcase_add_test(tc1_1, s21_from_float_to_decimal_5);
    tcase_add_test(tc1_1, s21_from_float_to_decimal_6);
    tcase_add_test(tc1_1, s21_from_float_to_decimal_7);
    tcase_add_test(tc1_1, s21_from_float_to_decimal_8);
    tcase_add_test(tc1_1, s21_from_float_to_decimal_9);

    tcase_add_test(tc1_1, s21_from_decimal_to_float_1);
    tcase_add_test(tc1_1, s21_from_decimal_to_float_2);
    tcase_add_test(tc1_1, s21_from_decimal_to_float_3);
    tcase_add_test(tc1_1, s21_from_decimal_to_float_4);
    tcase_add_test(tc1_1, s21_from_decimal_to_float_5);

    tcase_add_test(tc1_1, s21_mul_1);
    tcase_add_test(tc1_1, s21_mul_2);
    tcase_add_test(tc1_1, s21_mul_3);
    tcase_add_test(tc1_1, s21_mul_4);
    tcase_add_test(tc1_1, s21_mul_5);
    tcase_add_test(tc1_1, s21_mul_6);
    tcase_add_test(tc1_1, s21_mul_8);
    tcase_add_test(tc1_1, s21_div_2);
    tcase_add_test(tc1_1, s21_div_4);
    tcase_add_test(tc1_1, s21_mod_2);
    tcase_add_test(tc1_1, s21_truncate_1);
    tcase_add_test(tc1_1, s21_truncate_2);
    tcase_add_test(tc1_1, s21_truncate_3);
    tcase_add_test(tc1_1, s21_truncate_4);
    tcase_add_test(tc1_1, decimal_mul_1);
    tcase_add_test(tc1_1, decimal_mul_2);
    tcase_add_test(tc1_1, decimal_mul_3);
    tcase_add_test(tc1_1, decimal_mul_4);
    tcase_add_test(tc1_1, decimal_mul_5);
    tcase_add_test(tc1_1, decimal_mul_6);
    tcase_add_test(tc1_1, decimal_mul_7);
    tcase_add_test(tc1_1, decimal_mul_8);
    tcase_add_test(tc1_1, decimal_mul_9);
    tcase_add_test(tc1_1, decimal_mul_10);
    tcase_add_test(tc1_1, decimal_mul_11);
    tcase_add_test(tc1_1, decimal_mul_12);
    tcase_add_test(tc1_1, decimal_mul_13);
    tcase_add_test(tc1_1, decimal_mul_16);
    tcase_add_test(tc1_1, div_1);
    tcase_add_test(tc1_1, div_3);
    tcase_add_test(tc1_1, div_4);
    tcase_add_test(tc1_1, div_5);
    tcase_add_test(tc1_1, div_6);
    tcase_add_test(tc1_1, div_7);
    tcase_add_test(tc1_1, div_10);
    tcase_add_test(tc1_1, mod_4);
    tcase_add_test(tc1_1, mod_5);
    tcase_add_test(tc1_1, truncate_1);
    tcase_add_test(tc1_1, truncate_2);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
