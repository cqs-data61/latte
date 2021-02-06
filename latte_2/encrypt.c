/* ****************************** *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * Encryption                     *
 * ****************************** */
 
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt.h"
#include "param.h"
#include "sample_z.h"
#include "poly.h"
#include "ntt.h"
#include "encode.h"
#include "fastrandombytes.h"
#include "red.h"

#include <libXKCP.a.headers/SimpleFIPS202.h>

void encrypt(unsigned char *z, POLY_64 *c, const unsigned char *mu, const POLY_64 *a, const POLY_64 *b, const uint64_t l, const unsigned char *seed)
{
	unsigned char seed_in[64];
	unsigned char seed_kdf[32];
	
	static POLY_64 e, e_l[L + 2];
	static POLY_64 m;
	
	uint64_t i, p;
	
	SHAKE256(seed_kdf, 32, seed, 32);
	
	/* Z = \mu xor KDF(seed) */
	for (i = 0; i < 32; i++)
	{
		z[i] = mu[i] ^ seed_kdf[i];
	}
	
	memcpy(seed_in, seed, 32);
	memcpy(seed_in + 32, z, 32);
	
	/* seed for DGS is KDF(seed || Z) */
	SHAKE256(seed_kdf, 32, seed_in, 64);

	fastrandombytes_setseed(seed_kdf);
	
	/* e, e_h, e_1,..., e_l, e_b <-- D_{\sigma_e}^N */
	sample_e(&e);
	
	ntt(&e);
	
	for (i = 0; i < l + 2; i++)
	{
		sample_e(e_l + i);
		
		ntt(e_l + i);
	}
	
	/* C_i = A_i * e + e_i */
	for (i = 0; i < l + 1; i++)
	{
		for (p = 0; p < N; p++)
		{
			c[i].poly[p] = con_sub(montgomery(a[i].poly[p], e.poly[p]) + e_l[i].poly[p], Q);
		}
	}
	
	/* m = Encode(seed) */
	encode(&m, seed);
	
	ntt(&m);
	
	/* C_b = b * e + e_b + m */
	for (p = 0; p < N; p++)
	{
		c[l + 1].poly[p] = con_sub(montgomery(b->poly[p], e.poly[p]) + e_l[l + 1].poly[p], Q);
		c[l + 1].poly[p] = con_sub(c[l + 1].poly[p] + m.poly[p], Q);
	}
}
