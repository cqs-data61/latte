/* ****************************** *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * Modular arithmetic             *
 * ****************************** */
 
#ifndef _RED_H
#define _RED_H

#include <stdint.h>
#include "param.h"

/* x - q if x >= q */
static inline uint64_t con_sub(const uint64_t x, const uint64_t q)
{
	return x - ((-(1 ^ ((x - q) >> 63))) & q);
}

/* x + q if x <= q */
static inline uint64_t con_add(const uint64_t x, const uint64_t q)
{
	return x + ((-(x >> 63)) & q);
}

#define RED_SHORT_SHIFT_1 36
#define RED_SHORT_SHIFT_2 20
#define RED_SHORT_MASK ((1LL << RED_SHORT_SHIFT_1) - 1)

/* low hamming weight reduction
 * for q=2^(k_1) +/- 2^(k_2) + 1, k_1>k_2, and input x
 * x=u*2^(k_1)+v mod q=u*(-/+ 2^(k_2)-1)+v mod q */
static inline uint64_t red_short(uint64_t t)
{
	uint64_t x, y;
	
	x = t >> RED_SHORT_SHIFT_1;
	y = t & RED_SHORT_MASK;
	
	return con_sub(y + (x << RED_SHORT_SHIFT_2) - x, Q);
}

#define MONTGOMERY_FACTOR 1048577
#define MONTGOMERY_SHIFT 28
#define MONTGOMERY_MASK ((1LL << MONTGOMERY_SHIFT) - 1)
#define MONTGOMERY_CONVERT_FACTOR 15728624LL
#define MONTGOMERY_INV_FACTOR 60129480704LL

#define MONTGOMERY_SEP_SHIFT 31
#define MONTGOMERY_SEP_MASK ((1LL << MONTGOMERY_SEP_SHIFT) - 1)
#define MONTGOMERY_SEP_HI 17179869184LL
/* Montgomery reduction
 * Input: x < Q*R, where R=2^k
 * Output: m = x*R^{-1} % Q
 * 
 * b = Q^{-1} % R
 * t = ((x % R)*b) % R
 * m = x / R - t * Q / R */
static inline uint64_t montgomery_32(uint64_t a, uint64_t b)
{
	uint64_t t, x, y;
	
	t = a * b;
	x = t & MONTGOMERY_MASK;
	y = (x * MONTGOMERY_FACTOR) & MONTGOMERY_MASK;

	return con_add((t >> MONTGOMERY_SHIFT) - ((y * Q) >> MONTGOMERY_SHIFT), Q);
}

static inline uint64_t montgomery(uint64_t a, uint64_t b)
{
	uint64_t a0, a1, b0, b1;
	uint64_t x0, x1, x2;
	
	a0 = a & MONTGOMERY_SEP_MASK;
	a1 = a >> MONTGOMERY_SEP_SHIFT;
	b0 = b & MONTGOMERY_SEP_MASK;
	b1 = b >> MONTGOMERY_SEP_SHIFT;
	
	x0 = montgomery_32(a0, b0);
	x1 = a0 * b1 + a1 * b0;
	x2 = (a1 * b1 + (x1 >> MONTGOMERY_SEP_SHIFT)) * MONTGOMERY_SEP_HI;
	x1 = montgomery_32(x1 & MONTGOMERY_SEP_MASK, 1LL << MONTGOMERY_SEP_SHIFT);
	
	return red_short(x0 + x1 + x2);
}

/* Modular inverse mod q */
static inline int64_t inverse(int64_t a)
{
	int64_t t = 0;
	int64_t newt = 1;
	int64_t r = Q;
	int64_t newr = a;
	int64_t q;
	int64_t tmp;

	while (newr)
	{
		q = r / newr;
		tmp = newt;
		newt = t - q * newt;
		t = tmp;
		tmp = newr;
		newr = r - q * newr;
		r = tmp;
	}
	
	return con_add(t, Q);
}

#endif
