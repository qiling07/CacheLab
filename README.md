# This is the Purdue Principles of Computer Architecture Cache Lab

## Contents

* [General Rules](#general-rules)

* [Cache Simulation](#cache-simulation)
  * [Replacement Policy](#replacement-policy)

* [Software Optimization](#software-optimization)

* [Striding Prefetcher](#striding-prefetcher)

  * [Statistics](#statistics)

* [Python Script and Short Report](#python-script-and-short-report)

* [Grading](#grading)

## General Rules

We have several files you will want to modify for this homework. Please make sure you don't modify:

* simulated_ints.cpp
* simulated_ints.h
* common.h

The other files will require modification, for these modifications gentle hints are provided by comments that contain the word "tomato". These points in the code are the ones that need modification, while you can do a restructuring around these points, anything that changes the structure of the simulation is not permitted.

## Cache Simulation

For the cache simulation we want you to fill in functions in the cache.cpp file:

* cacheAddress
* writeCache

cacheAddress implements the access to cache while writeCache implements a write operation. The cache is a 32kB 8-way associative cache with blocks of 64 byte. Make sure your results are similar to the Base results in Results.txt file.

### Replacement Policy

For this part you want to add an argument for the main.cpp, and you want to add an option in cache.h under the enum Policy, register your policy by checking the argument. You will want to modify the parts of the cache functions which care about the replacement policy to respect the registered policy. Implement LRU, one other replacement policy of your chosing and Belady's replacement policy for Belady's policy you may want to take a trace of your cache accesses to a file and open that file to check the future accesses. Record and observe the stats of your different replacement policies (e.g. ResultsLRU.txt).

## Software Optimization

In this part we want you to optimize the cache accesses in the functions.cpp. Do not modify the base functions, the functions you want to modify have a comment with "tomato" in it. The goal is to make the Total time statistic of the cache for your functions is at least half the original value.

## Striding Prefetcher

In this part you want to fill in the fetchFromQueue in cache.cpp and Prefetcher::collect_and_update, Prefetcher::prefetch_mems in cache.h. Also add an argument to enable the prefetcher, when this argument is present in main.cpp you can call enablePrefetcher.

### Statistics

Fill in the statistics from the printStats function in cache.cpp, you might want to hold some data to make it possible.

## Python Script and Short Report

Here we want you to collect the stats with all the options you have (replacement policy, prefetcher enabled). You want to write a python3 script which will invoke the program with all your inputs and save the output to Results${replacement_policy}${prefetcher_enabled}.txt. Now we want you to write the reasons for differences between several results. First for LRU, reason about the difference between ResultsLRU.txt, ResultsLRUPrefetcher.txt. This is to see the difference between having a prefetcher and not, one or two sentences for each point in the statistics is enough. We also want to see the differences between ResultsLRUPrefetcher.txt, ResultsBeladyPrefetcher.txt, and Results\<YourReplacement>Prefetcher.txt, here you want to observe the performance differences accross different policies, summarize the numbers with respect to Belady, and if you have the prefetcher statistics implemented give a reason for the difference you see if any. Name the python3 script run.py, and your report as report.md
