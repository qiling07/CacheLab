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
    } else {
        printf("Invalid argument. Please use 'lru', 'rand', 'tree', or 'pf'.\n");
        return 1;
    }
    if (args < 3) {
        printf("Prefetcher disabled.\n");
    }
    else if (strcmp(argv[2], "pf") == 0) {
        printf("Prefetcher enabled.\n");
        enablePrefetcher();
    } else {
        printf("Invalid argument. Please use 'pf'.\n");
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