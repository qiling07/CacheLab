
#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

void baseConvolution(int dim, int_cached_buffer &src, int_cached_buffer &ker, int_cached_buffer &dst)
{
    int i, j, k, l;

    for (i = 0; i < dim - 8 + 1; i++)
        for (j = 0; j < dim - 8 + 1; j++)
        {
            dst[j * dim + i] = 0;
            for (k = 0; k < 8; k++)
                for (l = 0; l < 8; l++)
                {
                    dst[j * dim + i] = dst[j * dim + i] + src[(j + l) * dim + (i + k)] * ker[l * 8 + k];
                }
        }
}

void convolution(int dim, int_cached_buffer &src, int_cached_buffer &ker, int_cached_buffer &dst)
{
    // rule: offsets, indices and sizes can be integers but all the integers that are used as temporary
    // values are int_cache_dependent, you can have at most 8 of these otherwise initialize a buffer.
    // int i, j, initIndex = dim - 7;
    // int_cache_dependent res;
    // example: you can initialize another buffer like this.
    // int_cached_buffer kernValues;
    // initBuffer(kernValues, 64);
    // tomato: optimize convolution for more cache hits.

    // int i, j, k, l, m, n;

    // for (j = 0; j < dim - 8 + 1; j++)
    //     for (i = 0; i < dim - 8 + 1; i++)
    //     {
    //         dst[j * dim + i] = 0;
    //         for (l = 0; l < 8; l++)
    //             for (k = 0; k < 8; k++)
    //             {
    //                 dst[j * dim + i] += src[(j + l) * dim + (i + k)] * ker[l * 8 + k];
    //             }
    //     }

    int i, j, k, l, m, n;
    int bi, bj, tilesize = 8;
    for (bj = 0; bj < dim - 8 + 1; bj += tilesize)
    for (bi = 0; bi < dim - 8 + 1; bi += tilesize)
        for (j = bj; j < bi + tilesize && j < dim - 8 + 1; j++)
        for (i = bi; i < bj + tilesize && i < dim - 8 + 1; i++) {
            dst[j * dim + i] = 0;
            for (l = 0; l < 8; l++)
            for (k = 0; k < 8; k++)
                dst[j * dim + i] += src[(j + l) * dim + (i + k)] * ker[l * 8 + k];
        }
}

void baseMatrixMultiplication(int dim, int_cached_buffer &src, int_cached_buffer &src2, int_cached_buffer &dst)
{
    int i, j, k;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
        {
            dst[j * dim + i] = 0;
            for (k = 0; k < dim; k++)
                dst[j * dim + i] = dst[j * dim + i] + src[j * dim + k] * src2[i + k * dim];
        }
}

void matrixMultiplication(int dim, int_cached_buffer &src, int_cached_buffer &src2, int_cached_buffer &dst)
{
    int i, j, k;
    int bi, bj, bk, tilesize = 16;

    for (bj = 0; bj < dim; bj += tilesize)
    for (bi = 0; bi < dim; bi += tilesize)
    for (bk = 0; bk < dim; bk += tilesize)
        for (j = bj; j < bj + tilesize && j < dim; j++)
        for (k = bk; k < bk + tilesize && k < dim; k++)
        for (i = bi; i < bi + tilesize && i < dim; i++)
        {
            if (k == 0) dst[j * dim + i] = 0;
            dst[j * dim + i] += src[j * dim + k] * src2[k * dim + i];
        }
}
