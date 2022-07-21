#include <stdint.h>
#ifndef THREAD_FIXED_POINT_H
#define THREAD_FIXED_POINT_H
/** 在kernel 中没有浮点数运算，因此这里使用定点数代替浮点数
 * p.q
 * 这里取p = 17 , q = 14
 * x , y 为定点数
 * n 为integer
 */
#define P_FIXED_POINT 17
#define Q_FIXED_POINT 14
#define F_FIXED_POINT 1 << Q_FIXED_POINT

/**
 * @brief Convert n to fixed point
 *
 */
int32_t convert_to_fixed_point(int32_t n)
{
    return n * F_FIXED_POINT;
}

/**
 * @brief Convert fixed point x to integer
 *        round to zero
 * @param x
 * @return int32_t
 */
int32_t convert_to_integer_zero(int32_t x)
{
    return x / F_FIXED_POINT;
}

/**
 * @brief Convert fixed point x to integer
 *        round to nearest integer
 * @param x
 * @return int32_t
 */
int32_t convert_to_integer_near(int32_t x)
{
    if (x >= 0)
    {
        return (x + F_FIXED_POINT / 2) / F_FIXED_POINT;
    }
    else
    {
        return (x - F_FIXED_POINT / 2) / F_FIXED_POINT;
    }
}
/**
 * @brief add two fixed point number x y
 *
 * @param x
 * @param y
 * @return int32_t
 */
int32_t add_2_fixed_num(int32_t x, int32_t y)
{
    return x + y;
}

/**
 * @brief subtract y from x
 *
 * @param x
 * @param y
 * @return int32_t
 */
int32_t subtract_y_from_x(int32_t x, int32_t y)
{
    return x - y;
}
/**
 * @brief x + n*f
 *
 * @param x
 * @param n
 * @return int32_t
 */
int32_t add_integer_to_fixed(int32_t x, int32_t n)
{
    return x + n * F_FIXED_POINT;
}

/**
 * @brief x + n*f
 *
 * @param x
 * @param n
 * @return int32_t
 */
int32_t sub_fixed_from_integer(int32_t x, int32_t n)
{
    return x - n * F_FIXED_POINT;
}

/**
 * @brief return x * y
 *
 * @param x
 * @param y
 * @return int32_t
 */
int32_t mul_2_fixed(int32_t x, int32_t y)
{
    return ((int64_t)x) * y / F_FIXED_POINT;
}
/**
 * @brief return x *n
 *
 * @param x
 * @param n
 * @return int32_t
 */
int32_t mul_fixed_by_integer(int32_t x, int32_t n)
{
    return x * n;
}
/**
 * @brief return x * y
 *
 * @param x
 * @param y
 * @return int32_t
 */

int32_t div_2_fixed(int32_t x, int32_t y)
{
    return ((int64_t)x) * F_FIXED_POINT / y;
}
/**
 * @brief return x *n
 *
 * @param x
 * @param n
 * @return int32_t
 */
int32_t div_fixed_by_integer(int32_t x, int32_t n)
{
    return x / n;
}

#endif /**< threads/fixed-point.h */
