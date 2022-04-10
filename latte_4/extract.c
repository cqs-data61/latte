/* ****************************** *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * Extraction                     *
 * ****************************** */
 
#include <stdint.h>
#include "extract.h"
#include "param.h"
#include "poly.h"
#include "mat.h"
#include "fft.h"
#include "ntt.h"
#include "sample_fft.h"
#include "sample_z.h"
#include "fastrandombytes.h"
#include "red.h"

#include <libXKCP.a.headers/SimpleFIPS202.h>

void extract(POLY_64 *t, const MAT_FFT *fft_basis, const POLY_64 *a, const uint64_t l, const MAT_FFT *tree_root, const POLY_FFT *tree_dim2)
{
	static POLY_64 c_ntt;
	static POLY_FFT c;
	static POLY_FFT s[L];
	
	unsigned char seed[32];
	
	static unsigned char hash_in[sizeof(int64_t) * N + 1];
	
	uint64_t i, p;
	
	memcpy(hash_in, a->poly, sizeof(int64_t) * N);
	hash_in[sizeof(int64_t) * N] = 'G';
	
	SHAKE256(seed, 32, hash_in, sizeof(int64_t) * N + 1);
	
	fastrandombytes_setseed(seed);
	
	/* c = A_l */
	for (i = 0; i < N; i++)
	{
		c_ntt.poly[i] = a->poly[i];
	}
	
	intt(&c_ntt);
	for (i = 0; i < N; i++)
	{
		c.poly[i] = c_ntt.poly[i];
	}
	
	fft(&c, N);
	
	/* Sample preimage */
	sample_preimage(s, fft_basis, tree_root, tree_dim2, &c, l + 1, 1);
	
	for (i = 0; i < l; i++)
	{
		ifft(s + i, N);
		
		for (p = 0; p < N; p++)
		{
			t[i].poly[p] = roundq(crealq(s[i].poly[p]));
		}
		
		ntt(t + i);
	}
}
