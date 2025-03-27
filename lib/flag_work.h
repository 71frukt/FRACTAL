#ifndef FLAG_WORK_H
#define FLAG_WORK_H

#include "mandelbrot.h"

FractalError GetEnvironmentInfo (EnvironmentInfo *env_info, const int argc, const char *argv[]);
FractalError SaveFinalSettings  (EnvironmentInfo *env_info);

const char *const CUSTOM_SETTINGS_FLAG     = "--use_custom_settings";
const char *const SAVE_FINAL_SETTINGS_FLAG = "--save_final_settings";

#endif