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

/* x + q if x <= 0 */
static inline uint64_t con_add(const uint64_t x, const uint64_t q)
{
	return x + ((-(x >> 63)) & q);
}

static const __uint128_t RED_PLANTARD_R = (((__uint128_t)18427605699237269502ULL)<<64)|4503324816572417ULL;
#define RED_PLANTARD_SHIFT 64
static const __uint128_t RED_PLANTARD_CONVERT_FACTOR = (((__uint128_t)36038694771490876ULL)<<64)|126105184658915024ULL; 
static const __uint128_t RED_PLANTARD_INV_FACTOR = (((__uint128_t)1171142641370202111ULL)<<64)|16428431980493007152ULL; 

/* Plantard's multiplication modular reduction from:
 * Thomas Plantard. Efficient Word Size Modular Arithmetic. IEEE Trans. Emerg. Top. Comput. 9(3): 1506-1518 (2021)
 * https://ieeexplore.ieee.org/document/9416314 */
static inline uint64_t red_plantard(__uint128_t a, __uint128_t b)
{
	__uint128_t c;
	c = a * b * RED_PLANTARD_R;
	return (((c >> RED_PLANTARD_SHIFT) + 1) * Q) >> RED_PLANTARD_SHIFT;
}

static inline uint64_t red_plantard_const(__uint128_t a, __uint128_t b)
{
	__uint128_t c;
	c = a * b;
	return (((c >> RED_PLANTARD_SHIFT) + 1) * Q) >> RED_PLANTARD_SHIFT;
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
