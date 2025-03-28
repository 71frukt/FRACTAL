#include <stdio.h>
#include <string.h>

#include "flag_work.h"

void GetEnvironmentInfo(EnvironmentInfo *env_info, const int argc, const char *argv[])
{
    assert(env_info);
    assert(argv);
    assert(argc > 0);
    assert(env_info);

    for (int arg_counter = 1; arg_counter < argc; )   // два флага, два имени файла, при каждом i рассматриваем argv[i] и argv[i+1]
    {
        for (size_t flag_counter = 0; flag_counter < FLAGS_NUM; flag_counter++)
        {
            if (strcmp(argv[arg_counter], FLAGS[flag_counter].name) == 0)
            {
                arg_counter++;
                FLAGS[flag_counter].action(env_info, &arg_counter, argv);

                break;
            }
        }
    }

    if (env_info->use_custom_settings == false)
    {
        env_info->window_width            = DEFAULT_WINDOW_WIDTH;
        env_info->window_heigh            = DEFAULT_WINDOW_HEIGH;
        env_info->offset                  = {0, 0};
        env_info->max_calc_iterations_num = MAX_SEQUENCE_N;
        env_info->scale                   = START_SCALE;
        env_info->border_radius_sq        = BORDER_RADIUS_SQ;
    }
}

void LoadCustomSettings(EnvironmentInfo *env_info, int *arg_counter, const char *argv[])
{
    assert(env_info);
    assert(arg_counter);
    assert(argv);

    env_info->use_custom_settings = true;
    env_info->source_settings_file = argv[(*arg_counter)++];
    
    FILE *settings_file = fopen(env_info->source_settings_file, "r");
    assert(settings_file);
    
    fscanf(settings_file, "%lf" "%*[^\n]%*c" "%lf" "%*[^\n]%*c" "%lu" "%*[^\n]%*c" "%lf" "%*[^\n]%*c" "%lf" "%*[^\n]%*c" "%u" "%*[^\n]%*c" "%u" ,     // TODO check %*[^\n] error
        &env_info->offset.x, &env_info->offset.y, &env_info->max_calc_iterations_num, &env_info->scale, &env_info->border_radius_sq, &env_info->window_width, &env_info->window_heigh);
        
    fclose(settings_file);
    
    printf("%lf" "%lf" "%lu" "%lf" "%lf" "%u" "%u" ,     // TODO check %*[^\n] error
        env_info->offset.x, env_info->offset.y, env_info->max_calc_iterations_num, env_info->scale, env_info->border_radius_sq, env_info->window_width, env_info->window_heigh);
}

void NeedToSaveFinalSettings(EnvironmentInfo *env_info, int *arg_counter, const char *argv[])   // information is stored that you will need to save the settings at the end and where
{
    assert(env_info);
    assert(arg_counter);
    assert(argv);

    env_info->save_final_settings = true;
    env_info->dest_settings_file = argv[(*arg_counter)++];

    fprintf(stderr, "filename = '%s'\n", env_info->dest_settings_file);
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
            "%f  \t\t // border_radius_sq           \n"
            "%u  \t\t // window_width               \n"
            "%u  \t\t // window_heigh               \n",            
            env_info->offset.x, env_info->offset.y, env_info->max_calc_iterations_num, env_info->scale, env_info->border_radius_sq, env_info->window_width, env_info->window_heigh);

    fclose(dest_file);

    ENV_INFO_ASSERT(env_info);
    return SUCCESS_EXIT;
}