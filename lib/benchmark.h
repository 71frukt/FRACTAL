#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdio.h>

#include "mandelbrot.h"

const char *const REF_ENV_INFO_VALUES_FILE_NAME = "benchmark_ref_values.txt";
const char *const RES_BENCHMARK_FILE_NAME       = "res_benchmark.md";

const size_t NUM_OF_EXPS_IN_SERIES = 20;
const size_t BENCHMARK_TEST_FRAME_NUM = 50;

inline uint64_t rdtsc();
void TestBenchmark();



#endif