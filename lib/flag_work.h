#ifndef FLAG_WORK_H
#define FLAG_WORK_H

#include "mandelbrot.h"

void GetEnvInfoFromCmd          (EnvironmentInfo *env_info, const int argc, const char *argv[]);
void GetEnvInfoFromFile         (EnvironmentInfo *env_info, const char *const file_name);

void LoadCustomSettings         (EnvironmentInfo *env_info, int *cur_arg_num, const char *argv[]);
void NeedToSaveFinalSettings    (EnvironmentInfo *env_info, int *arg_counter, const char *argv[]);   // information is stored that you will need to save the settings at the end and where
void SetDrawFunc                (EnvironmentInfo *env_info, int *arg_counter, const char *argv[]);

FractalError SaveFinalSettings  (EnvironmentInfo *env_info);

const size_t FLAGS_NUM = 4;

struct Flag
{
    const char *const name;
    void (*action)(EnvironmentInfo *env_info, int *arg_counter, const char *argv[]);        // arg counter points on next arg after flag name
};

const Flag FLAGS[FLAGS_NUM] = 
{
    { "--use_custom_settings", LoadCustomSettings      },
    { "--save_final_settings", NeedToSaveFinalSettings },
    { "--set_rendering_func" , SetDrawFunc             }        // 0-3 to increase productivity
};

#endif