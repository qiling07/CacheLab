#!/usr/bin/env bash

mkdir -p results
./cache lru > results/Results-LRU-NoPF.txt
./cache rand > results/Results-Rand-NoPF.txt
./cache tree > results/Results-Tree-NoPF.txt
./cache lru pf > results/Results-LRU-PF.txt
./cache rand pf > results/Results-Rand-PF.txt
./cache tree pf > results/Results-Tree-PF.txt
