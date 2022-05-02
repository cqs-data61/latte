/* ****************************** *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * Delegation                     *
 * ****************************** */

#ifndef _DELEGATE_H
#define _DELEGATE_H

#include <stdint.h>
#include "poly.h"
#include "mat.h"

void delegate(MAT_FFT *s_fft, MAT_FFT *s_tree_root, POLY_FFT *s_tree_dim2, const MAT_FFT *fft_basis, const POLY_64 *a, const uint64_t l, const MAT_FFT *tree_root, const POLY_FFT *tree_dim2);

#endif
