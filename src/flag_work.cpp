#include <stdio.h>
#include <string.h>

#include "flag_work.h"

FractalError GetEnvironmentInfo(EnvironmentInfo *env_info, const int argc, const char *argv[])
{
    assert(argv);
    assert(argc > 0);
    assert(env_info);

    for (int i = 1; i < (1 + 3) && i < argc; i += 2)   // два флага, два имени файла, при каждом i рассматриваем argv[i] и argv[i+1]
    {
        if (strcmp(argv[i], CUSTOM_SETTINGS_FLAG) == 0)
        {
            env_info->use_custom_settings = true;
            env_info->source_settings_file = argv[i + 1];
            
            // fprintf(stderr, "file = %s\n", env_info->source_settings_file);
            FILE *settings_file = fopen(env_info->source_settings_file, "r");
            assert(settings_file);

            fscanf(settings_file, "%lf%*[^\n]%*c%lf%*[^\n]%*c%lu%*[^\n]%*c%lf%*[^\n]%*c%d",     // TODO check %*[^\n] error
                  &env_info->offset.x, &env_info->offset.y, &env_info->max_calc_iterations_num, &env_info->scale, &env_info->border_radius_sq);

            fclose(settings_file);
        }

        else if (strcmp(argv[i], SAVE_FINAL_SETTINGS_FLAG) == 0)
        {
            env_info->save_final_settings = true;
            env_info->dest_settings_file = argv[i + 1];
        }
    }

    if (env_info->use_custom_settings == false)
    {
        env_info->offset                  = {0, 0};
        env_info->max_calc_iterations_num = MAX_SEQUENCE_N;
        env_info->scale                   = START_SCALE;
        env_info->border_radius_sq        = BORDER_RADIUS_SQ;
    }

    ENV_INFO_ASSERT(env_info);
    return SUCCESS_EXIT;
}

FractalError SaveFinalSettings(EnvironmentInfo *env_info)
{
    ENV_INFO_ASSERT(env_info);

    FILE *dest_file = fopen(env_info->dest_settings_file, "w");

    fprintf(dest_file, 
            "%lf \t\t // offset.x                   \n"
            "%lf \t\t // offset.y                   \n"
            "%lu \t\t // max_calc_iterations_num    \n"
            "%lf \t\t // scale                      \n"
            "%d  \t\t // border_radius_sq           \n",
            env_info->offset.x, env_info->offset.y, env_info->max_calc_iterations_num, env_info->scale, env_info->border_radius_sq);

    fclose(dest_file);

    ENV_INFO_ASSERT(env_info);
    return SUCCESS_EXIT;
}