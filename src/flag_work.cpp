#include <stdio.h>
#include <string.h>

#include "flag_work.h"

void GetEnvInfoFromCmd(EnvironmentInfo *env_info, const int argc, const char *argv[])
{
    assert(env_info);
    assert(argv);
    assert(argc > 0);
    assert(env_info);

    env_info->DrawFunc = DrawMandelbrot_deployment;     // по умолчанию самая быстрая
    env_info->cur_fps  = 1;                             // хотя бы какое-то ненулевое начальное значение, чтобы избежать деления на 0 при расчёте перемещения

    for (int arg_counter = 1; arg_counter < argc; )     // два флага, два имени файла, при каждом i рассматриваем argv[i] и argv[i+1]
    {
        int prev_arg_counter = arg_counter;

        for (size_t flag_counter = 0; flag_counter < FLAGS_NUM; flag_counter++)
        {
            if (strcmp(argv[arg_counter], FLAGS[flag_counter].name) == 0)
            {
                arg_counter++;
                FLAGS[flag_counter].action(env_info, &arg_counter, argv);

                break;
            }
        }

        if (prev_arg_counter == arg_counter)
        {
            fprintf(stderr, "Error reading arg[%d] = '%s'\n", arg_counter, argv[arg_counter]);
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
    
    GetEnvInfoFromFile(env_info, env_info->source_settings_file);

    // FILE *settings_file = fopen(env_info->source_settings_file, "r");
    // assert(settings_file);
    
    // fscanf(settings_file, "%lf" "%*[^\n]%*c" "%lf" "%*[^\n]%*c" "%lu" "%*[^\n]%*c" "%lf" "%*[^\n]%*c" "%lf" "%*[^\n]%*c" "%u" "%*[^\n]%*c" "%u" ,     // TODO check %*[^\n] error
    //     &env_info->offset.x, &env_info->offset.y, &env_info->max_calc_iterations_num, &env_info->scale, &env_info->border_radius_sq, &env_info->window_width, &env_info->window_heigh);
        
    // fclose(settings_file);
}

void GetEnvInfoFromFile(EnvironmentInfo *env_info, const char *const file_name)
{
    FILE *settings_file = fopen(file_name, "r");
    if (settings_file == NULL)
        fprintf(stderr, "Error opening configuration file\n");
    
    fscanf(settings_file, "%lf" "%*[^\n]%*c" "%lf" "%*[^\n]%*c" "%lu" "%*[^\n]%*c" "%lf" "%*[^\n]%*c" "%lf" "%*[^\n]%*c" "%u" "%*[^\n]%*c" "%u" ,     // TODO check %*[^\n] error
        &env_info->offset.x, &env_info->offset.y, &env_info->max_calc_iterations_num, &env_info->scale, &env_info->border_radius_sq, &env_info->window_width, &env_info->window_heigh);
        
    fclose(settings_file);
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

void SetDrawFunc(EnvironmentInfo *env_info, int *arg_counter, const char *argv[])
{
    assert(env_info);
    assert(arg_counter);
    assert(argv);

    switch (atoi(argv[*arg_counter]))
    {    
    case 0:
        env_info->DrawFunc = DrawMandelbrot0;
        break;
    
    case 1:
        env_info->DrawFunc = DrawMandelbrot_cycles;
        break;

    case 2:
        env_info->DrawFunc = DrawMandelbrot_intrinsics;
        break;

    case 3:
        env_info->DrawFunc = DrawMandelbrot_intrinsics;
        break;

    default:
        break;
    }

    (*arg_counter)++;
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

