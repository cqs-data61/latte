/* ****************************** *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * Keygen                         *
 * ****************************** */
 
#ifndef _KEYGEN_H
#define _KEYGEN_H

#include "poly.h"
#include "mat.h"

void keygen(MAT_FFT *fft_basis, MAT_FFT *tree_root, POLY_FFT *tree_dim2, POLY_64 *h, const unsigned char *seed);

#endif
