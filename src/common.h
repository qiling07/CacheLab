#ifndef _COMMON_H_
#define _COMMON_H_
#include "cache.h"
void baseConvolution(int dim, int_cached_buffer &src, int_cached_buffer &ker, int_cached_buffer &dst);
void convolution(int dim, int_cached_buffer &src, int_cached_buffer &ker, int_cached_buffer &dst);
void baseMatrixMultiplication(int dim, int_cached_buffer &src, int_cached_buffer &src2, int_cached_buffer &dst);
void matrixMultiplication(int dim, int_cached_buffer &src, int_cached_buffer &src2, int_cached_buffer &dst);

#endif // _COMMON_H_