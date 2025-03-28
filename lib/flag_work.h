#ifndef FLAG_WORK_H
#define FLAG_WORK_H

#include "mandelbrot.h"

void GetEnvironmentInfo         (EnvironmentInfo *env_info, const int argc, const char *argv[]);

void LoadCustomSettings         (EnvironmentInfo *env_info, int *cur_arg_num, const char *argv[]);
void NeedToSaveFinalSettings    (EnvironmentInfo *env_info, int *arg_counter, const char *argv[]);   // information is stored that you will need to save the settings at the end and where

FractalError SaveFinalSettings  (EnvironmentInfo *env_info);

const char *const CUSTOM_SETTINGS_FLAG     = "--use_custom_settings";
const char *const SAVE_FINAL_SETTINGS_FLAG = "--save_final_settings";

const size_t FLAGS_NUM = 2;

struct Flag
{
    const char *const name;
    void (*action)(EnvironmentInfo *env_info, int *arg_counter, const char *argv[]);        // arg counter points on next arg after flag name
};

const Flag FLAGS[FLAGS_NUM] = 
{
    {"--use_custom_settings", LoadCustomSettings},
    {"--save_final_settings", NeedToSaveFinalSettings}
};

#endif