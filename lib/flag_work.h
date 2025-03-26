#ifndef FLAG_WORK_H
#define FLAG_WORK_H

#include "mandelbrot.h"

EnvironmentInfo GetEnvironmentInfo(const int argc, const char *argv[]);

const char *const CUSTOM_SETTINGS_FLAG     = "--use_custom_settings";
const char *const SAVE_FINAL_SETTINGS_FLAG = "--save_final_settings";

#endif