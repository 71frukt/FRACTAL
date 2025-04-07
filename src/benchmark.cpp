#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "mandelbrot.h"
#include "flag_work.h"
#include "benchmark.h"


inline uint64_t rdtsc()
{
    uint32_t lo, hi;
    asm volatile (
        "rdtsc"
        : "=a" (lo), "=d" (hi)
    );
    return ((uint64_t)hi << 32) | lo;
}


void TestBenchmark()
{
    printf("Start benchmark\n");

    FILE *dest_file = fopen(RES_BENCHMARK_FILE_NAME, "w");
    if (dest_file == NULL)
        fprintf(stderr, "Error opening dest benchmark file\n");

    EnvironmentInfo env_info = {};
    
    GetEnvInfoFromFile(&env_info, REF_ENV_INFO_VALUES_FILE_NAME);

    // res info:   | № | BENCHMARK_TEST_FRAME_NUM | window sizes | iteration sequence len | border_radius || tacts count |

    fprintf(dest_file, "# The results of measuring the performance of calculating the Mondelbrot set\n\n");

    fprintf(dest_file, "### Configuration information\n\n");
    fprintf(dest_file, "|                      |         |\n"
                       "|----------------------|---------|\n");
    fprintf(dest_file, "| num of exp in series | %lu     |\n",   NUM_OF_EXPS_IN_SERIES                       );
    fprintf(dest_file, "| Frames count         | %lu     |\n",   BENCHMARK_TEST_FRAME_NUM                    );
    fprintf(dest_file, "| window sizes         | %u x %u |\n",   env_info.window_width, env_info.window_heigh);
    fprintf(dest_file, "| border radius        | %.2e   |\n\n", sqrtf(env_info.border_radius_sq)            );


    printf("Benchmark: DrawMandelbrot0 calculating...\n");


    fprintf(dest_file, "### Series 1 - `DrawMandelbrot0`\n\n");
    fprintf(dest_file, "| № | tacts count | tacts per frame |\n");
    fprintf(dest_file, "|---|-------------|-----------------|\n");

    for (size_t exp_num = 0; exp_num < NUM_OF_EXPS_IN_SERIES; exp_num++)
    {
        const size_t start_tact = rdtsc();

        FractalError (*volatile unopt_wrapper)(const EnvironmentInfo *env_info) = DrawMandelbrot0;

        for (size_t frame_count = 0; frame_count < BENCHMARK_TEST_FRAME_NUM; frame_count++)
        {
            ERROR_HANDLER(unopt_wrapper(&env_info));
        }
    
        const size_t end_tact = rdtsc();
    
        size_t tacts_count = end_tact - start_tact;
        fprintf(dest_file, "| %lu | %lu | %.2e |\n", exp_num + 1, tacts_count, (double) tacts_count / BENCHMARK_TEST_FRAME_NUM);
    }

    printf("Benchmark: DrawMandelbrot0 calculated\n");
    printf("Benchmark: DrawMandelbrot_cycles calculating...\n");


    fprintf(dest_file, "### Series 2 - `DrawMandelbrot_cycles`\n\n");
    fprintf(dest_file, "| № | tacts count | tacts per frame |\n");
    fprintf(dest_file, "|---|-------------|-----------------|\n");

    for (size_t exp_num = 0; exp_num < NUM_OF_EXPS_IN_SERIES; exp_num++)
    {
        const size_t start_tact = rdtsc();

        FractalError (*volatile unopt_wrapper)(const EnvironmentInfo *env_info) = DrawMandelbrot_cycles;

        for (size_t frame_count = 0; frame_count < BENCHMARK_TEST_FRAME_NUM; frame_count++)
        {
            ERROR_HANDLER(unopt_wrapper(&env_info));
        }
    
        const size_t end_tact = rdtsc();
    
        size_t tacts_count = end_tact - start_tact;
        fprintf(dest_file, "| %lu | %lu | %.2e |\n", exp_num + 1, tacts_count, (double) tacts_count / BENCHMARK_TEST_FRAME_NUM);
    }

    printf("Benchmark: DrawMandelbrot_cycles calculated\n");
    printf("Benchmark: DrawMandelbrot_intrinsics calculating...\n");


    fprintf(dest_file, "### Series 3 - `DrawMandelbrot_intrinsics`\n\n");
    fprintf(dest_file, "| № | tacts count | tacts per frame |\n");
    fprintf(dest_file, "|---|-------------|-----------------|\n");

    for (size_t exp_num = 0; exp_num < NUM_OF_EXPS_IN_SERIES; exp_num++)
    {
        const size_t start_tact = rdtsc();

        FractalError (*volatile unopt_wrapper)(const EnvironmentInfo *const env_info) = DrawMandelbrot_intrinsics;

        for (size_t frame_count = 0; frame_count < BENCHMARK_TEST_FRAME_NUM; frame_count++)
        {
            ERROR_HANDLER(unopt_wrapper(&env_info));
        }
    
        const size_t end_tact = rdtsc();
    
        size_t tacts_count = end_tact - start_tact;
        fprintf(dest_file, "| %lu | %lu | %.2e |\n", exp_num + 1, tacts_count, (double) tacts_count / BENCHMARK_TEST_FRAME_NUM);
    }


    printf("Benchmark: DrawMandelbrot_intrinsics calculated\n");
    printf("Benchmark: DrawMandelbrot_deployment calculating...\n");


    fprintf(dest_file, "### Series 4 - `DrawMandelbrot_deployment`\n\n");
    fprintf(dest_file, "| № | tacts count | tacts per frame |\n");
    fprintf(dest_file, "|---|-------------|-----------------|\n");

    for (size_t exp_num = 0; exp_num < NUM_OF_EXPS_IN_SERIES; exp_num++)
    {
        const size_t start_tact = rdtsc();

        FractalError (*volatile unopt_wrapper)(const EnvironmentInfo *env_info) = DrawMandelbrot_deployment;

        for (size_t frame_count = 0; frame_count < BENCHMARK_TEST_FRAME_NUM; frame_count++)
        {
            ERROR_HANDLER(unopt_wrapper(&env_info));
        }
    
        const size_t end_tact = rdtsc();
    
        size_t tacts_count = end_tact - start_tact;
        fprintf(dest_file, "| %lu | %lu | %.2e |\n", exp_num + 1, tacts_count, (double) tacts_count / BENCHMARK_TEST_FRAME_NUM);
    }

    printf("Benchmark: DrawMandelbrot_deployment calculated\n");


    fclose(dest_file);
}