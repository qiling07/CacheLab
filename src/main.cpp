#include "cache.h"
#include "common.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

int main(int args, char **argv)
{
    initCache();
    matrix_mult_args ma;
    convolution_args ca;
    // tomato: add an argument to specify your replacement policy
    if (args < 2) {
        printf("Default to 'lru'.\n");
        registerPolicy(CacheSimulation::Policy::LRU);
    }
    else if (strcmp(argv[1], "lru") == 0) {
        printf("You selected 'lru'.\n");
        registerPolicy(CacheSimulation::Policy::LRU);
    } 
    else if (strcmp(argv[1], "rand") == 0) {
        printf("You selected 'rand'.\n");
        registerPolicy(CacheSimulation::Policy::Random);
    } 
    else if (strcmp(argv[1], "tree") == 0) {
        printf("You selected 'tree'.\n");
        registerPolicy(CacheSimulation::Policy::Tree);
    } 
    else {
        printf("Invalid argument. Please use 'lru', 'rand', or 'tree'.\n");
        return 1;
    }

    initMatrixMultiplicationWithDimensions(256, ma);
    initConvolutionWithDimensions(256, ca);
    baseConvolution(ca.dim, ca.src, ca.ker, ca.dst);
    copyBaseDst(ca);
    std::cout << "Base Convolution Stats:" << std::endl;
    printStats();
    std::cout << "=======================" << std::endl;
    resetCacheStats();
    convolution(ca.dim, ca.src, ca.ker, ca.dst);
    checkBuffers(ca);
    std::cout << "Your Convolution Stats:" << std::endl;
    printStats();
    std::cout << "=======================" << std::endl;
    resetCacheStats();
    baseMatrixMultiplication(ma.dim, ma.src, ma.src2, ma.dst);
    copyBaseDst(ma);
    std::cout << "Base Matrix Multiplication Stats:" << std::endl;
    printStats();
    std::cout << "=================================" << std::endl;
    resetCacheStats();
    matrixMultiplication(ma.dim, ma.src, ma.src2, ma.dst);
    checkBuffers(ma);
    std::cout << "Your Matrix Multiplication Stats:" << std::endl;
    printStats();
    std::cout << "=================================" << std::endl;
    resetCacheStats();
}