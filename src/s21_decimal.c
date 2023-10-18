#include "s21_decimal.h"

#undef abs
#define abs(x) ((x) < 0 ? -(x) : (x))

// ----------------------- Основные: ---------------------------------------------
// Арифметические операторы

int s21_add(s21_decimal a, s21_decimal b, s21_decimal *res) {
    int ret_val = 0;
    int sign_a = get_sign(a);
    int sign_b = get_sign(b);

    s21_decimal c, d;
    s21_dec_normal(a, b, &c, &d);
    set_zero(res);
    res->bits[3] = c.bits[3];

    if (sign_a == 0 && sign_b == 1) {
        if (s21_is_gr_or_eq_bits_only(c, d)) {
            ret_val = s21_sub_bits_only(c, d, res);
        } else {
            ret_val = s21_sub_bits_only(d, c, res);
            set_bit(res, 127);  // сделать знак res минусом
        }
    } else if (sign_a == 1 && sign_b == 0) {
        if (s21_is_gr_or_eq_bits_only(c, d)) {
            ret_val = s21_sub_bits_only(c, d, res);
            set_bit(res, 127);
        } else {
            ret_val = s21_sub_bits_only(d, c, res);
            unset_bit(res, 127);
        }
    } else {
        ret_val = s21_add_bits_only(c, d, res, 0);
    }

    if (ret_val == 1 && sign_a == 1 && sign_b == 1)  // обработчик случая переполнения отрицательных чисел
        ret_val += 1;                                // возвращаем 2
    if (ret_val == 1 && get_pow(*res) != 0)
        ret_val = 0;
    if (s21_is_null(*res)) set_null(res);

    return ret_val;
}

int s21_add_bits_only(s21_decimal a, s21_decimal b, s21_decimal *res, int is_it_sub) {
    int mem = 0, ret_val = 0;
    set_zero(res);

    for (int i = 0; i < 96; i++) {
        int temp = get_bit(a, i) + get_bit(b, i) + mem;
        mem = 0;
        if (temp == 3) {
            mem = 1;
            set_bit(res, i);
        } else if (temp == 2) {
            mem = 1;
        } else if (temp == 1) {
            set_bit(res, i);
        }

        if (i == 95 && mem)
            ret_val = 1;
    }

    return (is_it_sub ? 0 : ret_val);
}

int s21_sub(s21_decimal a, s21_decimal b, s21_decimal *res) {
    int ret_val = 0;
    int sign_a = get_sign(a);
    int sign_b = get_sign(b);

    s21_decimal c, d;
    s21_dec_normal(a, b, &c, &d);
    set_zero(res);
    res->bits[3] = c.bits[3];

    if (sign_a == 0 && sign_b == 1) {
        ret_val = s21_add_bits_only(c, d, res, 0);
    } else if (sign_a == 1 && sign_b == 0) {
        ret_val = s21_add_bits_only(d, c, res, 0);
        set_bit(res, 127);
    } else if (sign_a == 1 && sign_b == 1) {
        if (s21_is_gr_or_eq_bits_only(c, d)) {
            ret_val = s21_sub_bits_only(c, d, res);
        } else {
            ret_val = s21_sub_bits_only(d, c, res);
            unset_bit(res, 127);
        }
    } else if (sign_a == 0 && sign_b == 0) {
        if (s21_is_gr_or_eq_bits_only(c, d)) {
            ret_val = s21_sub_bits_only(c, d, res);
        } else {
            ret_val = s21_sub_bits_only(d, c, res);
            set_bit(res, 127);
        }
    }

    if (ret_val == 1 && sign_a == 1 && sign_b == 0)  // обработчик случая переполнения отрицательных чисел
        ret_val += 1;                                // возвращаем 2

    if (ret_val == 1 && get_pow(*res) != 0)
        ret_val = 0;

    if (s21_is_null(*res)) set_null(res);


    return ret_val;
}

int s21_sub_bits_only(s21_decimal a, s21_decimal b, s21_decimal *res) {
    int ret_val = 0;

    get_twos_complement(&b);
    ret_val = s21_add_bits_only(a, b, res, 1);

    return ret_val;
}

int s21_mul(s21_decimal a, s21_decimal b, s21_decimal *res) {
    set_null(res);
    int ret_val = 0;
    if (s21_is_null(a) || s21_is_null(b)) {
    } else {
        // для отслеживания переполнения:
        s21_decimal moar;
        if (compare_bits(a, b) == -1) {
            copy_decimal(b, &moar);
        } else {
            copy_decimal(a, &moar);
        }

        for (int i = 0; i < 96; i++) {
            if (get_bit(b, i))
                s21_add_bits_only(*res, a, res, 0);

            left_shift(&a, 1);
        }

        if (compare_bits(moar, *res) == 1) {
            if (get_sign(a) == get_sign(b))
                ret_val = 1;
            else
                ret_val = 2;
        }

        change_sign(a, b, res);
        sum_pows(a, b, res);

        int res_pow = get_pow(*res);
        if (res_pow > 28) {
            s21_decimal ten = {{10, 0, 0, 0}};
            for (int i = res_pow - 28; i; i--)
                ret_val = s21_div(*res, ten, res);
            set_pow(res, 28);
        }
        if (ret_val != 0)
            set_null(res);
        else if (s21_is_null(*res))
            set_null(res);
    }

    return ret_val;
}


// функция реализована аналогично функции divide выше.

int s21_div(s21_decimal a, s21_decimal b, s21_decimal *res) {
    set_null(res);
    int ret_val = 0;
    s21_decimal zero = {{0, 0, 0, 0}};
        if (s21_is_gr_or_eq_bits_only(zero, b)) {
            return 3;  // Деление на ноль
        }
    s21_decimal one = {{1, 0, 0, 0}};

        long_decimal dividend = {0};
        long_decimal divisor = {0};
        long_decimal divisor_copy = {0};

        for (int i = 0; i < 3; i++) {
            dividend.bits[i] = a.bits[i];
            divisor.bits[i] = b.bits[i];
        }

        copy_long_decimal(divisor, &divisor_copy);
        set_zero(res);

            s21_decimal one_copy = {{1, 0, 0, 0}};

            for (int i = 95; i >= 0; --i) {
                copy_decimal(one_copy, &one);
                copy_long_decimal(divisor_copy, &divisor);
                left_shift_long(&divisor, i);
                if (compare_bits_long(dividend, divisor) != -1) {
                    long_sub_bits_only(dividend, divisor, &dividend);
                    left_shift(&one, i);
                    s21_add_bits_only(*res, one, res, 0);
                }
            }


        int pow_diff = get_pow(a) - get_pow(b);

        if (pow_diff >= 0) {
            sub_pows(a, b, res);
        }
    change_sign(a, b, res);

    return ret_val;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    set_null(result);
    int flag = 0, pow = 0;
    int sign = get_sign(value_1);
    s21_decimal res = {{0, 0, 0, 0}};
    if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0) {
        flag = 3;
    } else  if (s21_is_less(value_1, value_2)) {
        *result = value_1;
    } else {
        s21_div(value_1, value_2, &res);
        pow = get_pow(value_1);
        s21_mul(res, value_2, &res);
        s21_sub_bits_only(value_1, res, result);
    }
    set_pow(result, pow);
    if (get_pow(*result) > 28 && flag != 3)
        flag = sign + 1;
    return flag;
}


// Преобразователи

int s21_truncate(s21_decimal value, s21_decimal *result) {
    int ret_val = 0;
    s21_decimal ten = {{10, 0, 0, 0}};
    int pow = get_pow(value);

    if (pow) {
        s21_div(value, ten, result);

        for (int i = 0; i < pow-1; i++)
            s21_div(*result, ten, result);
    }
    set_pow(result, 0);
    return ret_val;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
    *dst = 0;
    int ret_val = s21_truncate(src, &src);
    if (ret_val != 0 || src.bits[1] || src.bits[2] ||
        src.bits[0] > 2147483647u) {
        ret_val = 1;
    } else {
        *dst = src.bits[0];
        if (get_sign(src)) *dst *= -1;
    }
    return ret_val;
}

// Другие функции


int s21_negate(s21_decimal value, s21_decimal *result) {
    for (int i = 0; i < 4; i++)
        result->bits[i] = value.bits[i];
    change_bit(result, 127);
    return 0;
}

// --------------------- Вспомогательные:------------------------------------------

// Битовые операции


int get_bit(s21_decimal num, int pos) {
    return (num.bits[pos/32] >> (pos%32)) & 1;
}

void set_bit(s21_decimal* num, int pos) {
    num->bits[pos/32] |= (1 << pos%32);
}

void unset_bit(s21_decimal* num, int pos) {
    num->bits[pos/32] &= ~(1 << pos%32);
}

void change_bit(s21_decimal* num, int pos) {
    num->bits[pos/32] ^= (1 << pos%32);
}


// Операции с числами s21_decimal

void copy_decimal(s21_decimal a, s21_decimal* res) {
    for (int i = 0; i < 4; i++)
        res->bits[i] = a.bits[i];
}

void normalise(s21_decimal* a, s21_decimal* b) {
    int pow_a = get_pow(*a);
    int pow_b = get_pow(*b);
    int pow_diff = pow_a - pow_b;
    s21_decimal* larger_pow;
    s21_decimal* lesser_pow;
    if (pow_b > pow_a) {
        larger_pow = b;
        lesser_pow = a;
    } else {
        larger_pow = a;
        lesser_pow = b;
    }

    s21_decimal ten = {{10, 0, 0, 0x00010000}};
    s21_decimal temp = {{0, 0, 0, 0}};

    int overflow = 0;

    // умножаем число с меньшим показателем степени делителя, пока оно не переполнится:
    while (pow_diff && !overflow) {
        overflow = s21_mul(*lesser_pow, ten, &temp);
        if (!overflow)
            copy_decimal(temp, lesser_pow);
        pow_diff = get_pow(*a) - get_pow(*b);
    }

    // затем делим число с большим показателем степени делителя, пока не выровняем числа:
    while (pow_diff) {
        s21_div(*larger_pow, ten, larger_pow);
        pow_diff = get_pow(*a) - get_pow(*b);
    }
}

void sum_pows(s21_decimal a, s21_decimal b, s21_decimal *res) {
    int res_sign = get_sign(*res);

    res->bits[3] = (get_pow(a) + get_pow(b)) << 16;

    if (res_sign)
        set_bit(res, 127);
}

void sub_pows(s21_decimal a, s21_decimal b, s21_decimal *res) {
    int res_sign = get_sign(*res);

    res->bits[3] = (get_pow(a) - get_pow(b)) << 16;

    if (res_sign)
        set_bit(res, 127);
}

int get_pow(s21_decimal a) {
    return (a.bits[3] >> 16) & 0x000000ff;
}

int get_sign(s21_decimal a) {
    return ((a.bits[3] & 0x80000000) == 0) ? 0 : 1;
}

void change_sign(s21_decimal a, s21_decimal b, s21_decimal *res) {
    int a_sign = get_sign(a);
    int b_sign = get_sign(b);

    if ((!a_sign && b_sign) || (a_sign && !b_sign))
        set_bit(res, 127);
}

void left_shift(s21_decimal* num, int shift) {
    for (int i = 95-shift; i >= 0; i--) {
        if (get_bit(*num, i))
            set_bit(num, i+shift);
        else
            unset_bit(num, i+shift);
    }

    for (int i = shift-1; i >= 0; i--)
        unset_bit(num, i);
}

int s21_is_null(s21_decimal num) {
    int ret_val = 1;

    for (int i = 0; i < 3; i++)
        if (num.bits[i])
            ret_val = 0;

    return ret_val;
}

void get_twos_complement(s21_decimal* num) {
    for (int i = 0; i < 3; i++)
        num->bits[i] = ~(num->bits[i]);
    s21_decimal one = {{1, 0, 0, 0}};
    s21_add_bits_only(*num, one, num, 0);
}

void set_zero(s21_decimal* num) {
    for (int i = 0; i < 3; i++)
        num->bits[i] = 0;
}

void set_null(s21_decimal* num) {
    for (int i = 0; i < 4; i++)
        num->bits[i] = 0;
}

// функция возвращает:
// 1, если a больше b;
// -1, если а меньше b;
// 0, если а равно b.
int compare_bits(s21_decimal a, s21_decimal b) {
    int res = 0;

    int no_break = 1;
    for (int i = 2; i >= 0 && no_break; i--) {
        if (a.bits[i] > b.bits[i]) {
            res = 1;
            no_break = 0;
        }
        if (a.bits[i] < b.bits[i]) {
            res = -1;
            no_break = 0;
        }
    }

    return res;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
    if (!dst) {
            return 1;
    }
    *dst = 0;

    for (int i = 0; i < 96; i++)
        if (get_bit(src, i))
            *dst += pow(2, i);

    long int tmp;
    tmp = 1;
    for (int src_pow = get_pow(src); src_pow > 0; src_pow--) {
        tmp *= 10;
    }
    *dst /= tmp;

    if (get_sign(src) == 1)
        *dst = -(*dst);

    return 0;
}


// Прочее:

int s21_dec_normal(s21_decimal x, s21_decimal y, s21_decimal *x1, s21_decimal *y1) {
    /*
        функция осуществляет "выравнивание" показателей степени двух чисел x, y и возвращает измененные числа
        через переменные x1, y1.
        для сохранения максимальной точности выравнивания, всегда выравнивание начинается с числа с мЕньшим
        показателем степени путем умножения на 10 и увеличения показатели степени на 1.
        Только в том случае, если дальнейшее увеличение степени числа х невозможно из-за переполнения мантиссы
        числа, производится уменьшение степени числа у делением на 10 и уменьшением степени на 1.
        !!!! Однако, это может привести к тому, что младшие биты мантиссы будут утеряны !!!
        Функция возвращает:
        0 - если получилось выравнять числа по порядку относительно друг друга
        1 - если не получилось точно выравнять относительно друг друга
    */
    char return_code = 0;
    s21_decimal x1_aux, y1_aux;     //  вспомогательные переменные
    set_null(&x1_aux);
    set_null(&y1_aux);

    int sign_x = get_sign(x);
    int sign_y = get_sign(y);

    // убираем знаки
    x.bits[3] &= 0x7fffffff;
    y.bits[3] &= 0x7fffffff;

    char ex = (unsigned char)(x.bits[3] >> 16);     //  порядок числа x
    char ey = (unsigned char)(y.bits[3] >> 16);     //  порядок числа y
    char ediff = ex - ey;

    if (ediff != 0) {   //  если степени одинаковые, то функция пропускает свою работу и ничего не делает
        //  для удобства всегда определяю x1 числом с меньшим порядком, а y1 - с большим
        if (ediff < 0) {
            copy_decimal(x, x1);
            copy_decimal(y, y1);
        } else {
            copy_decimal(y, x1);
            copy_decimal(x, y1);
        }

        char ex1 = (unsigned char)(x1->bits[3] >> 16);     //  порядок числа x1
        char ey1 = (unsigned char)(y1->bits[3] >> 16);     //  порядок числа y1

        for (int i = 0; i < abs(ediff); i++) {
            if ( !s21_dec_mant_mul_10(*x1, &x1_aux) ) {
                ex1++;
                x1_aux.bits[3] &= ~0x00ff0000;  //  обнуление показателей степени
                x1_aux.bits[3] |= (ex1 << 16);          //  запись нового значения
                copy_decimal(x1_aux, x1);
            } else {
                break;  //  попадаем сюда, если происходит переполнение при попытке умножить мантиссу на 10
            }
        }

        char exdiff = ex1 - ey1;    //  проверяем - проверяем новую разность порядков
        if ((exdiff) != 0) {
            //  выравниваем порядок числа y1 осуществляя деление и уменьшая величину порядка
            for (int i = 0; i < abs(exdiff); i++) {
                if ( !s21_dec_mant_div_10(*y1, &y1_aux) ) {
                    ey1--;
                    y1_aux.bits[3] &= ~0x00ff0000;
                    y1_aux.bits[3] |= (ey1 << 16);
                    copy_decimal(y1_aux, y1);
                } else {
                            //  порядок числа ставится как у числа x1
                    //  по знаку можно не беспокоится - к этому моменту сравниваем числа с одинаковым знаком
                    return_code = 1;        //  произошло нарушение точности при сравнении
                    y1->bits[3] = x1->bits[3];
                    y1->bits[2] = y1->bits[1] = y1->bits[0] = 0;
                }
            }
        }
        //  теперь нужно вернуть соответствие между x/x1 и y/y1
        if (ediff < 0) {
            //  ничего не делаем - не было подмены числа х на у и наоборот
        } else {
            copy_decimal(*x1, &y1_aux);  //  сохраняем x1 во вспомогательной переменной
            copy_decimal(*y1, x1);
            copy_decimal(y1_aux, y1);
        }
    } else {    //  выравнивания порядков не было - просто копируем входные значения
        copy_decimal(x, x1);
        copy_decimal(y, y1);
    }    //  end of if (!ediff)

    if (sign_x)
        set_sign(x1, 1);
    if (sign_y)
        set_sign(y1, 1);
    return (return_code);
}

int s21_dec_mant_mul_10(s21_decimal source, s21_decimal *dst) {
    /* Функция умножения мантиссы на 10 числа decimal
        Алгоритм основан на том, что умножение числа х на 10 можно представить как сумму 8х+2х, что тождественно
        - сдвигу числа х на три разряда влево
        - сдвигу числа х на один разряд влево
        - сложения получившихся произведений.
        При сдвиге первого слагаемого используется контроль переполнения - если это произошло, функция возвращает 1.
        При сдвиге второго слагаемого контроль переполнения не выполняется, так как очевидно, что если первое слагаемое
        не переполнится, то второе тоже нет.
    */

    unsigned int result = 0;
    s21_decimal dec_num_aux; set_zero(&dec_num_aux);
    unsigned int overflow_flag_2 = 0;

    //  first part of the results (x*2*2*2 is done below)
    //  if high-order bit could be multiplied by 2^3, it is ok to proceed
    if (__builtin_mul_overflow(source.bits[2], 8, &result)) {
        //  decimal number if too big to be multiplied - overflow
        overflow_flag_2 = 1;
    } else {
        /*
            possible to multiply by 8. Код работает, но не гарантирует очередность исполнения в другой системе/
            лучше писать так:
            dst->bits[2] = (source.bits[2] << 3)
            dst->bits[2] |= (source.bits[1] >> 29)
            ...
        */

        unsigned int overflow_flag_1 = 0;
        unsigned int overflow_flag_0 = 0;   //  flag to show that sum of bits[0] cause overflow

        dst->bits[2] = (source.bits[2] << 3) | (source.bits[1] >> 29);
        dst->bits[1] = (source.bits[1] << 3) | (source.bits[0] >> 29);
        dst->bits[0] = (source.bits[0] << 3);

        //  second part of the results (2*x is done below)
        dec_num_aux.bits[2] = (source.bits[2] << 1) | source.bits[1] >> 31;
        dec_num_aux.bits[1] = (source.bits[1] << 1) | source.bits[0] >> 31;
        dec_num_aux.bits[0] = (source.bits[0] << 1);


        /*
            at this stage all components of the multiplication are available:
            x << 3 is stored in the dst->bits
            2x is stored in the dec_num_aux components.
            Суммируем с контролем переполнения - если переполнение произошло, то переносим единичку на следующий разряд.
            В теории, блок else не нужен, так как все переменные overflow .. обнулены при входе в функцию, но с
            ним спокойнее
        */
        if ( __builtin_uadd_overflow(dst->bits[0], dec_num_aux.bits[0], &result) ) {
            overflow_flag_0 = 1;
        } else {
            overflow_flag_0 = 0;
        }
        dst->bits[0] += dec_num_aux.bits[0];

        /*
            требуется дважды контролировать переполнение:
            в первом случае - убедиться, что если есть единицв с младших разрядов, то это не приведет к переполнению
            более старшего разряда. Если придет, то сразу ставим флаг и втоое сравнение делать не надо, так как после
            переполнения единицей станет равным 0 и второе слагаемое его точно не переполнит.
            во втором случае, убедиться, что сумма первого слагаемого + 1 м второго не приведет к переполнению.
            Если приведет, то ставим флаг переполнения равный 1.
        */
        if (__builtin_uadd_overflow(dst->bits[1], overflow_flag_0, &result)) {
            overflow_flag_1 = 1;
        } else {
            if (__builtin_uadd_overflow(dst->bits[1] + overflow_flag_0, dec_num_aux.bits[1], &result)) {
                overflow_flag_1 = 1;
            } else {
                overflow_flag_1 = 0;
            }
        }
        dst->bits[1] += (dec_num_aux.bits[1] + overflow_flag_0);

        if (__builtin_uadd_overflow(dst->bits[2], overflow_flag_1, &result)) {
            overflow_flag_2 = 1;
        } else {
            if (__builtin_uadd_overflow(dst->bits[2] + overflow_flag_1, dec_num_aux.bits[2], &result)) {
                overflow_flag_2 = 1;
            } else {
                overflow_flag_2 = 0;
            }
        }
        dst->bits[2] += (dec_num_aux.bits[2] + overflow_flag_1);
    }

    return (overflow_flag_2);
}

int s21_dec_mant_div_10(s21_decimal source, s21_decimal *dst) {
    /* Функция используется для деления на 10 мантиссы числа decimal.
        принципы деления двоичных чисел прописаны здесь:
        https://www.wikihow.com/Divide-Binary-Numbers
        https://ru.wikihow.com/%D0%B4%D0%B5%D0%BB%D0%B8%D1%82%D1%8C-%D0%B4%D0%B2%D0%BE%D0%B8%D1%87%D0%BD%D1%8B%D0%B5-%D1%87%D0%B8%D1%81%D0%BB%D0%B0
        Функция возвращает только целую часть от деления!
        0 - деление прошло успешно
        1 - неуспешное деление (на выходе нет целой части от деления)
    */

    int             return_code = 0;
    unsigned int    buff_for_div = 0;     //  буфер, где будут накапливаться числа для деления
    unsigned int    buff_after_div = 0;   //  буфер, где промежуточного результата деления
    (void)buff_after_div;
    unsigned int const mask[] = {     //  маски для битов с 32-го по 0-ой
        0x80000000,
        0x40000000,
        0x20000000,
        0x10000000,
        0x8000000,
        0x4000000,
        0x2000000,
        0x1000000,
        0x800000,
        0x400000,
        0x200000,
        0x100000,
        0x80000,
        0x40000,
        0x20000,
        0x10000,
        0x8000,
        0x4000,
        0x2000,
        0x1000,
        0x800,
        0x400,
        0x200,
        0x100,
        0x80,
        0x40,
        0x20,
        0x10,
        0x8,
        0x4,
        0x2,
        0x1
    };

    set_zero(dst);          //  обнуляем выходное число

    for (int j = 2; j >= 0; j--) {
        for (int i = 0; i < 32; i++) {
            buff_for_div |= (source.bits[j] & mask[i]) >> (31 - i);
            if (buff_for_div < 10) {
                buff_for_div <<= 1;
                continue;
            } else {
                buff_after_div = buff_for_div / 10;
                dst->bits[j] |= (1 << (31 - i));
                buff_for_div = buff_for_div % 10;
                //  готовим числа к следующей итерации
                buff_for_div <<= 1;
            }
        }
    }
    //  в результатет деления нет больше целой части
    if ( dst->bits[2] == 0 && dst->bits[1] == 0 && dst->bits[0] == 0 ) {
        return_code = 1;
    }
    return (return_code);
}

int s21_is_less_aux(s21_decimal x, s21_decimal y) {
    /*  вспомогательная функция для сравнения двух decimal _с одинаковым порядком и знаком_ !!!
        возвращает 1 - если х больше у
        возвращает 0 - если х не больше у
        возвращает -1 - если сравниваются числа с разными порядками или знаками
    */
    char compare_completed = 0;
    char return_code = 0;
    char sign_x = (x.bits[3] >> 31);
    char sign_y = (y.bits[3] >> 31);
    char bit_x;
    char bit_y;

    unsigned int const mask[] = {     //  маски для битов с 32-го по 0-ой
        0x80000000,
        0x40000000,
        0x20000000,
        0x10000000,
        0x8000000,
        0x4000000,
        0x2000000,
        0x1000000,
        0x800000,
        0x400000,
        0x200000,
        0x100000,
        0x80000,
        0x40000,
        0x20000,
        0x10000,
        0x8000,
        0x4000,
        0x2000,
        0x1000,
        0x800,
        0x400,
        0x200,
        0x100,
        0x80,
        0x40,
        0x20,
        0x10,
        0x8,
        0x4,
        0x2,
        0x1
    };

    if (sign_x == 0 && sign_y == 0) {
        for (int j = 2; j >= 0; j--) {
            for (int i = 0; i < 32; i++) {
                bit_x = (x.bits[j] & mask[i]) >> (31 - i);
                bit_y = (y.bits[j] & mask[i]) >> (31 - i);
                if (bit_x < bit_y) {
                    return_code = 1;
                    compare_completed = 1;
                    break;
                }
                if (bit_x > bit_y) {
                    return_code = 0;
                    compare_completed = 1;
                    break;
                }
                if (compare_completed) break;
            }
        }
    }
    //  если два числа отрицательны, то результат надо инвертировать
    if (sign_x == 1 && sign_y == 1) {
        for (int j = 2; j >= 0; j--) {
            for (int i = 0; i < 32; i++) {
                bit_x = (x.bits[j] & mask[i]) >> (31 - i);
                bit_y = (y.bits[j] & mask[i]) >> (31 - i);
                if (bit_x < bit_y) {
                    return_code = 0;
                    compare_completed = 1;
                    break;
                }
                if (bit_x > bit_y) {
                    return_code = 1;
                    compare_completed = 1;
                    break;
                }
                if (compare_completed) break;
            }
        }
    }

    if ( (sign_x - sign_y) != 0 ) {
        return_code = -1;
    }
    return (return_code);
}

int s21_is_gr_or_eq_bits_only(s21_decimal a, s21_decimal b) {
    int res = 1;

    int no_break = 1;
    for (int i = 2; i >= 0 && no_break; i--) {
        if (a.bits[i] < b.bits[i]) {
            res = 0;
            no_break = 0;
        }
        if (a.bits[i] > b.bits[i]) {
            no_break = 0;
        }
    }

    return res;
}

int long_sub_bits_only(long_decimal a, long_decimal b, long_decimal *res) {
    int ret_val = 0;

    long_get_twos_complement(&b);
    ret_val = long_add_bits_only(a, b, res);

    return ret_val;
}

void long_get_twos_complement(long_decimal* num) {
    for (int i = 0; i < 6; i++)
        num->bits[i] = ~(num->bits[i]);
    long_decimal one = {{1, 0, 0, 0, 0, 0}};
    long_add_bits_only(*num, one, num);
}

int long_add_bits_only(long_decimal a, long_decimal b, long_decimal *res) {
    int mem = 0, ret_val = 0;
    long_set_zero(res);

    for (int i = 0; i < 192; i++) {
        int temp = get_bit_long(a, i) + get_bit_long(b, i) + mem;
        mem = 0;
        if (temp == 3) {
            mem = 1;
            set_bit_long(res, i);
        } else if (temp == 2) {
            mem = 1;
        } else if (temp == 1) {
            set_bit_long(res, i);
        }

        if (i == 95 && mem)
            ret_val = 1;
    }

    return ret_val;
}

void long_set_zero(long_decimal* num) {
    for (int i = 0; i < 6; i++)
        num->bits[i] = 0;
}

int get_bit_long(long_decimal long_x, int pos) {
    return (long_x.bits[pos/32] >> (pos%32)) & 1;
}

void set_bit_long(long_decimal* long_x, int pos) {
    long_x->bits[pos/32] |= (1 << pos%32);
}

void unset_bit_long(long_decimal* long_x, int pos) {
    long_x->bits[pos/32] &= ~(1 << pos%32);
}

void left_shift_long(long_decimal* long_x, int shift) {
    for (int i = 191-shift; i >= 0; i--) {
        if (get_bit_long(*long_x, i))
            set_bit_long(long_x, i+shift);
        else
            unset_bit_long(long_x, i+shift);
    }

    for (int i = shift-1; i >= 0; i--)
        unset_bit_long(long_x, i);
}

int compare_bits_long(long_decimal long_x, long_decimal long_y) {
    int res = 0;

    int no_break = 1;
    for (int i = 5; i >= 0 && no_break; i--) {
        if (long_x.bits[i] > long_y.bits[i]) {
            res = 1;
            no_break = 0;
        } else if (long_x.bits[i] < long_y.bits[i]) {
            res = -1;
            no_break = 0;
        }
    }

    return res;
}

void copy_long_decimal(long_decimal a, long_decimal* res) {
    for (int i = 0; i < 6; i++)
        res->bits[i] = a.bits[i];
}

//  ================== comparision operators =================
int s21_is_less(s21_decimal x, s21_decimal y) {
    /* Less than
        принцип работы функции:
        - определяем знаки чисел и пытаемся сделать сравнение, если знаки разные
        - если знаки одинаковые, то выравниваем порядки
        - числа с выровненными порядками сравниваются между собой
        Return value:
        0 - FALSE
        1 - TRUE
    */
    char return_code = 0;
    char compare_completed = 0;
    s21_decimal x1, y1;             //  вспомогательные переменные
    set_null(&x1);          //  инициализация вспомогательных переменных
    set_null(&y1);
    char ex = (unsigned char)(x.bits[3] >> 16);     //  порядок числа x
    char ey = (unsigned char)(y.bits[3] >> 16);     //  порядок числа y
    (void)ex;
    (void)ey;
    char sign_x = (x.bits[3] >> 31);
    char sign_y = (y.bits[3] >> 31);

    //  проверим знаки - если у "х" минус, а у "у" плюс, то ответ сразу очевиден
    if (sign_x - sign_y > 0) {
        return_code = 1;
        compare_completed = 1;
    } else if (sign_x - sign_y < 0) {   //  меньше 0, если х положительно, а у отрицательно
        return_code = 0;
        compare_completed = 1;
    }

    //  если compare_completed == 0 в этом месте, то это означает, что знаки одинаковы и надо сравнивать
    //  числа выравнивая их по порядку
    if ( compare_completed == 0 ) {
        //  уравниваем числа по порядку - выровненные храняться в переменных x1/y1
        s21_dec_normal(x, y, &x1, &y1);
        return_code = s21_is_less_aux(x1, y1);
        if (return_code == -1) printf("s21_is_less: ошибка сравнения чисел\n");
    }
    return (return_code);
}
int s21_is_less_or_equal(s21_decimal x, s21_decimal y) {
    char return_code = s21_is_less(x, y);
    if ( (return_code != -1) && (return_code != 1) )
        return_code = s21_is_equal(x, y);
    return (return_code);
}
int s21_is_greater(s21_decimal x, s21_decimal y) {
    /* Greater than
        принцип работы функции:
        - определяем знаки чисел и пытаемся сделать сравнение, если знаки разные
        - если знаки одинаковые, то выравниваем порядки
        - числа с выровненными порядками сравниваются между собой
        Return value:
        0 - FALSE
        1 - TRUE
    */
    char return_code = 0;
    char compare_completed = 0;
    s21_decimal x1, y1;             //  вспомогательные переменные
    set_null(&x1);          //  инициализация вспомогательных переменных
    set_null(&y1);
    char ex = (unsigned char)(x.bits[3] >> 16);     //  порядок числа x
    char ey = (unsigned char)(y.bits[3] >> 16);     //  порядок числа y
    (void)ex;
    (void)ey;

    char sign_x = (x.bits[3] >> 31);
    char sign_y = (y.bits[3] >> 31);

    //  проверим знаки - если у "х" минус, а у "у" плюс, то ответ сразу очевиден
    if (sign_x - sign_y > 0) {
        return_code = 0;
        compare_completed = 1;
    } else if (sign_x - sign_y < 0) {   //  меньше 0, если х положительно, а у отрицательно
        return_code = 1;
        compare_completed = 1;
    }

    //  если compare_completed == 0 в этом месте, то это означает, что знаки одинаковы и надо сравнивать
    //  числа выравнивая их по порядку
    if ( compare_completed == 0 ) {
        //  уравниваем числа по порядку - выровненные храняться в переменных x1/y1
        s21_dec_normal(x, y, &x1, &y1);
        return_code = s21_is_greater_aux(x1, y1);
        if (return_code == -1) printf("s21_is_greater: ошибка сравнения чисел\n");
    }
    return (return_code);
}
int s21_is_greater_or_equal(s21_decimal x, s21_decimal y) {
    char return_code = s21_is_greater(x, y);
    if ( (return_code != -1) && (return_code != 1) )
        return_code = s21_is_equal(x, y);
    return (return_code);
}

int s21_is_not_equal(s21_decimal x, s21_decimal y) {
    char return_code = s21_is_equal(x, y);
    return (return_code == 1 ? 0 : 1);
}

int s21_is_greater_aux(s21_decimal x, s21_decimal y) {
    /*  вспомогательная функция для сравнения двух decimal _с одинаковым порядком и знаком_ !!!
        возвращает 1 - если х больше у
        возвращает 0 - если х не больше у
        возвращает -1 - если сравниваются числа с разными порядками или знаками
    */
    char compare_completed = 0;
    char return_code = 0;
    char sign_x = (x.bits[3] >> 31);
    char sign_y = (y.bits[3] >> 31);
    char bit_x;
    char bit_y;

    unsigned int const mask[] = {     //  маски для битов с 32-го по 0-ой
        0x80000000,
        0x40000000,
        0x20000000,
        0x10000000,
        0x8000000,
        0x4000000,
        0x2000000,
        0x1000000,
        0x800000,
        0x400000,
        0x200000,
        0x100000,
        0x80000,
        0x40000,
        0x20000,
        0x10000,
        0x8000,
        0x4000,
        0x2000,
        0x1000,
        0x800,
        0x400,
        0x200,
        0x100,
        0x80,
        0x40,
        0x20,
        0x10,
        0x8,
        0x4,
        0x2,
        0x1
    };

    if (sign_x == 0 && sign_y == 0) {
        for (int j = 2; j >= 0; j--) {
            for (int i = 0; i < 32; i++) {
                bit_x = (x.bits[j] & mask[i]) >> (31 - i);
                bit_y = (y.bits[j] & mask[i]) >> (31 - i);
                if (bit_x < bit_y) {
                    return_code = 0;
                    compare_completed = 1;
                    break;
                }
                if (bit_x > bit_y) {
                    return_code = 1;
                    compare_completed = 1;
                    break;
                }
                if (compare_completed) break;
            }
        }
    }
    //  если два числа отрицательны
    if (sign_x == 1 && sign_y == 1) {
        for (int j = 2; j >= 0; j--) {
            for (int i = 0; i < 32; i++) {
                bit_x = (x.bits[j] & mask[i]) >> (31 - i);
                bit_y = (y.bits[j] & mask[i]) >> (31 - i);
                if (bit_x < bit_y) {
                    return_code = 1;
                    compare_completed = 1;
                    break;
                }
                if (bit_x > bit_y) {
                    return_code = 0;
                    compare_completed = 1;
                    break;
                }
                if (compare_completed) break;
            }
        }
    }
    if ( (sign_x - sign_y) != 0 ) {
        return_code = -1;
    }
    return (return_code);
}

// ==========Keenacla==========

//  ========================== aux  ==========================
int s21_floor(s21_decimal src, s21_decimal *dst) {
    /* функция для "округления" числа decimal в мЕньшую сторону
        Принцип работы:
        1. Определяется показатель экспоненты, тем самым, определяется сколько цифр после запятой которые надо убрать
        2. Так как мантисса целое число, то она делиться на 10^(exp-1);
        3. К этому моменту число decimal выглядит как xxx.y (так как деление на 10 производилось не exp раз, а exp-1)
            теперь необходимо определить какое число в y. Если это 0, то округление производить не нужно.
            Если y != 0, то надо определить как округлять число decimal:
            - если положительное, то после окончательного деления на 10 ничего делать не надо
            - если отрицательное, то после окончательного деления надо прибавить 1.
    */

    char return_code = 0;
    char exp = (unsigned char)(src.bits[3] >> 16);
    char sign = (src.bits[3] >> 31);
    unsigned int res;               //  для контроля переполнения

    s21_decimal s21_dec_aux;            //  вспомогательная переменная
    set_null(&s21_dec_aux);

    s21_decimal s21_dec_xy, src_initial;
    copy_decimal(src, &src_initial);     //  сохраняем исходное значение

    //  преобразовываем мантиссу, чтобы ней в ней осталась одна десятичная цифра
    if ( exp != 0 ) {
        char round_flag = 0;

        //  деление производиться только (exp-1) раз! Потом надо будет еще раз поделить
        for (int i = 1; i < exp; i++) {
            //  print_dec2bin(src);
            s21_dec_mant_div_10(src, &s21_dec_aux);
            //  print_dec2bin(s21_dec_aux);
            src.bits[2] = s21_dec_aux.bits[2];
            src.bits[1] = s21_dec_aux.bits[1];
            src.bits[0] = s21_dec_aux.bits[0];
        }
        //  в src храниться величина x.y (исходное число разделенное на 10^(exp-1)
        copy_decimal(src, &s21_dec_xy);
        s21_dec_mant_div_10(src, &s21_dec_aux);
        s21_dec_mant_mul_10(s21_dec_aux, &src);

        //  после деления и умножения сравниваем с s21_dec_xy и src
        //  если дробная часть src была равна 0, то мантиссы чисел src и s21_dec_xy
        //  должны быть равными
        if ( (src.bits[2] != s21_dec_xy.bits[2]) ||
             (src.bits[1] != s21_dec_xy.bits[1]) ||
             (src.bits[0] != s21_dec_xy.bits[0]) ) {
                round_flag = 1;     //  фиксация факта того, что дробная часть не была равна 0
        }

        //  еще раз делим на 10, чтобы отбросить дробную часть
        s21_dec_mant_div_10(src, &s21_dec_aux);
        src.bits[2] = s21_dec_aux.bits[2];
        src.bits[1] = s21_dec_aux.bits[1];
        src.bits[0] = s21_dec_aux.bits[0];

        //  если число отрицательное, то "отнимаем" от него единицу (так как числа
        //  храняться как абс величины, то единицу прибавляем с контролем переполнения)
        //  если положительное, то оно уже не содержит дробной части
        if ( (round_flag) && (sign) ) {
            if ( !__builtin_uadd_overflow(src.bits[0], 1, &res) ) {
                src.bits[0]++;
            } else if ( !__builtin_uadd_overflow(src.bits[1], 1, &res) ) {
                src.bits[1]++;
            } else if ( !__builtin_uadd_overflow(src.bits[2], 1, &res) ) {
                src.bits[2]++;
            } else {    //  не удалось уменьшить из-за переполнения - ошибка округления
                return_code = 1;
            }
        }
        dst->bits[3] = src.bits[3];
        dst->bits[3] &= ~0xff0000;     //  cleaning exponenta value _without impact on sign value_
        dst->bits[2] = src.bits[2];
        dst->bits[1] = src.bits[1];
        dst->bits[0] = src.bits[0];
    } else {    //  возвращаем исходное число
        dst->bits[3] = src_initial.bits[3];
        dst->bits[2] = src_initial.bits[2];
        dst->bits[1] = src_initial.bits[1];
        dst->bits[0] = src_initial.bits[0];
    }  //  end of if (exp != 0)
    return (return_code);
}

int s21_round(s21_decimal src, s21_decimal *dst) {
    /*
        Rounds a decimal value to the nearest integer.
        Принцип работы функции:
        Фактически повторяет алгоритм работы s21_floor, с той лишь разницей, что
        в конце сравниваем остаток отделения не с 0, а по остатку определяем в
        какую сторону округлять
        Return value - code error:
        0 - OK
        1 - calculation error
    */
    char return_code = 0;
    char exp = (unsigned char)(src.bits[3] >> 16);
    char sign = (src.bits[3] >> 31);
    (void)sign;
    unsigned int res;               //  для контроля переполнения

    s21_decimal s21_dec_aux;            //  вспомогательная переменная
    set_null(&s21_dec_aux);

    s21_decimal s21_dec_xy, src_initial;
    copy_decimal(src, &src_initial);     //  сохраняем исходное значение

    //  преобразовываем мантиссу, чтобы ней в ней осталась одна десятичная цифра
    if ( exp != 0 ) {
        char ost = 0;
        for (int i = 1; i < exp; i++) {
            s21_dec_mant_div_10(src, &s21_dec_aux);
            src.bits[2] = s21_dec_aux.bits[2];
            src.bits[1] = s21_dec_aux.bits[1];
            src.bits[0] = s21_dec_aux.bits[0];
        }
        //  в src храниться величина x.y (исходное число разделенное на 10^(exp-1)
        copy_decimal(src, &s21_dec_xy);
        s21_dec_mant_div_10(src, &s21_dec_aux);
        s21_dec_mant_mul_10(s21_dec_aux, &src);

        //  после деления и умножения сравниваем с s21_dec_xy и src
        //  если дробная часть src была равна 0, то мантиссы чисел src и s21_dec_xy
        //  должны быть равными
        if ( (src.bits[2] - s21_dec_xy.bits[2]) != 0 ||
             (src.bits[1] - s21_dec_xy.bits[1]) != 0 ) {
            //  фиксация факта того, что остаток от деления значительный
            //  по идее, такого не должно быть - printf оставлен для автоматических проверок
            //  чтобы зафиксировать такой факт
            printf("s21_round: значительный остаток от деления\n");
        } else {
                if (s21_dec_xy.bits[0] - src.bits[0] >= 5) {
                //  если остаток от деления больше 5, то надо к s21_dec_aux
                //  прибавить 1, которая будет округлением после удаления остатка
                ost = 1;
            }
        }

        //  еще раз делим на 10, чтобы отбросить последюю дробную часть
        s21_dec_mant_div_10(src, &s21_dec_aux);
        src.bits[2] = s21_dec_aux.bits[2];
        src.bits[1] = s21_dec_aux.bits[1];
        src.bits[0] = s21_dec_aux.bits[0];

        //  теперь надо к src добавить результат округления с контролем переполнения
        //  в отличие от floor, знак числа нам не важен - мы округляем к ближайшему
        //  целому модуль числа
        if ( !__builtin_uadd_overflow(src.bits[0], ost, &res) ) {
                src.bits[0] += ost;
            } else if ( !__builtin_uadd_overflow(src.bits[1], 1, &res) ) {
                src.bits[1]++;
            } else if ( !__builtin_uadd_overflow(src.bits[2], 1, &res) ) {
                src.bits[2]++;
            } else {    //  не удалось округлить из-за переполнения - ошибка округления
                return_code = 1;
            }

        dst->bits[3] = src.bits[3];
        dst->bits[3] &= ~0xff0000;     //  cleaning exponenta value _without impact on sign value_
        dst->bits[2] = src.bits[2];
        dst->bits[1] = src.bits[1];
        dst->bits[0] = src.bits[0];
    } else {    //  возвращаем исходное число
        dst->bits[3] = src_initial.bits[3];
        dst->bits[2] = src_initial.bits[2];
        dst->bits[1] = src_initial.bits[1];
        dst->bits[0] = src_initial.bits[0];
    }  //  end of if (exp != 0)
    return (return_code);
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
    if (!dst) {
            return 1;
        }
    set_null(dst);      //  обнуляем целевую переменную, чтобы убедиться, что в ней нет мусора
    char sign = ((long int)src >> 31);      //  переменная для хранения флага знака
    if (sign != 0) {
        src = (src ^ sign);
        src++;
    }

    //  вызываем функцию для преобразования
    return (!s21_new((unsigned int)src, 0, sign, dst) ? 0 : 1);
}

int s21_new(unsigned int x, int e, int sign, s21_decimal *dst) {
    /*  Функиция создания числа decimal при передаче ему в качестве параметров мантиссы, степени и знака
        The s21_new function will combine the coefficient and exponent into a s21_dec.
        Numbers that are too huge to be contained in this format become nan.
        Numbers that are too tiny to be contained in this format become zero.
        0 - OK
        1 - convertation error
    */
    char status = 0;        //  aux variable for statuses.
    /*
        1 - входное число равно 0 или ему эквивалентно
        2 - слишком большое число
        4 -
    */
    char debug_flag = 0;

    set_null(dst);     //  обнуляем целевую переменную, чтобы убедиться, что в ней нет мусора

    /*  ====================== Отсекаем заведомо маленькие числа
        check from the very begining that input values in the function are not similar to 0
    */
    if (e <= -37) {     //  -37 is based on minimum power 28 and shift 8 from called function + 1 as spare
        status |= 1;
    }

    /*  ====================== Отсекаем избыточные нули, стараясь уменьшить величину отрицательной экспоненты
        При умножении исходного числа на 100000000, которое содержало немного цифр после занятой, могли образоваться
        нули, которые не имеют значения. Необходимо от них избавиться, чтобы сделать экспоненту как можно ближе
        к 0 (для случая, когда исходная степень отрицательная) - отрицательная степень не может храниться в числе decimal.
    */
    while ( (x % 10 == 0) && (e < 0) ) {
            x /= 10;
            e++;
        debug_flag |= 1;    //  для дебаггинга только
    }
    /*
        К этому моменту экспонента все еще может быть положительной или отрицательной, причем, меньше -28 из-за того,
        что было введено число в диапазоне, но с большим количеством цифр после занятой. Необходимо их "отрезать".
        Например, число 2.22e-27 не может быть сохранено в виде 222e-29, так как степень превышает диапазон.
        Число будет обрезано до 22e-28.
        Число 22.2e-27 будет сохранено как 222e-28.
    */
    while ( (e < -28) && (x >= 10) ) {
            x /= 10;
            e++;
            //  printf("x value is %d, e value is %d\n", x, e);
            debug_flag |= 1;    //  флаг сделан только для дебаггинга
    }

    /*  ====================== Отсекаем слишком большие числа
        if exponenta is more than 28, no point to do transformations - number is too big and can't be fitten into dec
        этот шаг нужен только для оптимизации исполнения кода, чтобы не пытаться преобразоввать в decimal числа
        заведомо выходящие за диапазон
    */
    if (e > 22) {
        status |= 2;
    }

    /*  ====================== Преобразование числа
        К этому моменту в программе не должно остаться числе с экспонентой меньше -28. Числа меньше будет приравнены к 0.
        Float число храниться в 32 битах, но так как, с учетом степени может быть больше ~4.29496716e9, то не получится
        сохранить число decimal только в bits[0]. Будут необходимо преобразование в биты bits[1], buts[2] для чисел в
        диапазоне -7.923e28...-7.923e28.
        Числа с большими степенями, например 1e29 не входят рассматриваются по условию !(status & 2).
        Алгоритм ниже можно было бы реализовать только одним циклом с использованием s21_dec_mant_mul_10, но данная функция
        производит операции сразу над 3 наборами бит. Если число меньше чем ~4.29496716e9, то отработает только внешний цикл.
        Внутренний будет работать только для ситуации, когда введенные числа больше ~4.29496716e9.
    */
    unsigned int mul_result = 0;
    char big_value = 0;
    (void)big_value;
    char exp_reduction = 0;
    (void)exp_reduction;
    dst->bits[0] = x;               //  далее x не используется
    while ( !(status & 2) && (e > 0) ) {
        if (!__builtin_mul_overflow(dst->bits[0], 10, &mul_result)) {
            dst->bits[0] *= 10;
            e--;
            exp_reduction = 1;  //  только для дебаггинга
        } else {    //  Ой... нельзя - переполненние. Придется двигать биты ручками
            s21_decimal dec_num_aux;            //  вспомогательная переменная, для промежуточных результатов
            set_null(&dec_num_aux);
            dec_num_aux.bits[0] = dst->bits[0];
            while (e > 0) {
                if ( !s21_dec_mant_mul_10(dec_num_aux, dst) ) {
                    e--;
                    copy_decimal(*dst, &dec_num_aux);
                } else {
                    status |= 2;
                    break;
                }
            }
            break;
        }
    }

    //  В этом месте еще могут быть экспоненты меньше -28, принимаем такие числа равные нулю
    if (e < -28) status |= 1;

    switch (status) {
        case 0:     //  normal transformation
            //  bits[0], bits[1], bits[2] are already in the dst value
            e = -e;     //  inverse e sign to sign off to the decimal number with proper 10^e dividor
            dst->bits[3] |= (e << 16);
            dst->bits[3] |= ((long int)sign << 31);
            break;
        case 1:  //  модуль числа больше 0, но меньше 1e-28 (степень -28 в традиционном понимании).
            dst->bits[0] = 0;
            dst->bits[1] = 0;
            dst->bits[2] = 0;
            dst->bits[3] = 0;   //  e = 0 просто для безопасности
            dst->bits[3] |= ((long int)sign << 31);
            break;
        case 2:  //  слишком большое - мантисса забивается 0, степень единицами, знак - из исходного числа
            dst->bits[0] = 0;
            dst->bits[1] = 0;
            dst->bits[2] = 0;
            dst->bits[3] |= (255 << 16);
            dst->bits[3] |= ((long int)sign << 31);
            break;
        default:
            printf("Abnormal case for transformation - do troubleshooing\n");
            break;
    }

    return ( status == 0 ) ? 0 : 1;
}

int s21_from_float_to_decimal(float f, s21_decimal *dst) {
    /*
        алгоритм работы функции преобразования из float в decimal следующий:
        1. С помощью функции frexp определяется мантисса (M) в десятичной форме + степень 2^E2 (M, E2);
        2. Степень 2^E2 преобразуется в десятичную степень 10^E10, где E10 находится из соотношения
            10^E10=2^E2 => E10 = log(2^E2) = E2 * log(2) = E2 * 0.301.. =
            Полученное значение округляется в большую сторону.
            Умножение мантиссы на степень pow(10., (e10 - e)) позволяет нам получить мантиссу которая была во float и избавиться
            от дробной части степени E10 - после этого у нас остается только целая часть с которой будет работа.
        3. Мантисса умножается на 100000000 для того, чтобы убрать дробную часть (float хранит только 6 значащих цифр после занятой)
            и получить целое число, которая в дальнейшем сохраняется в decimal.
        4. Число float сохраняется в объединении - та кбыло удобно дебажить, а в рабочей программе из целого числа удобнее брать
            знак.
        5. После получения мантиссы, степени, знака они передаются в другую функцию s21_new для создания числа decimal.

        Проверенные граничные условия:
        - результирующая степень больше 7.923e28 - число слишком большое - его невозможно преобразовать. В этом случае
            число преобразуется по аналогии с представлением вещественных чисел:
            степень числа забивается единицами, мантисса нулями, знак берется из исходного числа
            [СДЕЛАНО]
        - результирующая степень меньше -28 - число слишком маленькое - его невозможно преобразовать - принимается равным 0.
            [СДЕЛАНО]
        - если степень больше -28 - число слишном маленькое - неообходимо округлить до ближайшего меньшего числа;
            [СДЕЛАНО]

        Наблюдения:
            На некоторых числах, которые во float храняться с погрешностью, происходит не совсем точное преобразование.
            Например, при попытке перевода числа 4e18 в decimal число преобразуется как 3999999900000000000, так как
            в оригинале храниться как 3999999937226997760.000000.
            Необходимо подумать, оставить так или дальше оптимизировать код, чтобы более точно переводить число.

        Что возвращает функция:
        0 - OK
        1 - convertation error
    */

    if (isnan(f) || isinf(f)) {
            return 1;
        }

    float_transf tmp;
    tmp.a = f;  //  теперь int tmp.b содержит unsigned int представление числа float

    int e2, shift = 8;  //  8 is because mantissa is multiplied by 100000000 below to ensure integer number
    double m = frexp((double)f, &e2);  // m - normalized mantissa value
    double e10 = e2 * 0.3010299956639811952137388947;
    int e = (int)ceil(e10);
    m = abs(m) * pow(10., (e10 - e));

    unsigned int m32 = (unsigned int)round(m*100000000.0);

    //  к этому моменту известна мантисса и экспонента. Надо получить знак и можно упаковывать значение,
    //  в число decimal, что делается в функции s21_new.

    return (!s21_new(m32, e - shift, tmp.b >> 31, dst) ? 0 : 1);
}

void set_pow(s21_decimal *a, int pow) {
    a->bits[3] &= ~(0xFF << 16);
    a->bits[3] |= pow << 16;
}


void set_sign(s21_decimal *d, int s) {
    if (s) {
        d->bits[3]|=0x80000000;
    } else {
        d->bits[3]&=~0x80000000;
    }
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
    int flag = 0;
    if (s21_is_null(value_1)&&s21_is_null(value_2)) {
        flag = 1;
    } else {
    int sign_v1 = get_sign(value_1);
    int sign_v2 = get_sign(value_2);
    if (sign_v1 == sign_v2) {
        normalise(&value_1, &value_2);
        s21_decimal t;
        s21_sub_bits_only(value_1, value_2, &t);
        if (s21_is_null(t))
            flag = 1;
        }
    }
    return flag;
}

