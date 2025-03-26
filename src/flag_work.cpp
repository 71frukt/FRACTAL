#include <stdio.h>
#include <string.h>

#include "flag_work.h"

EnvironmentInfo GetEnvironmentInfo(const int argc, const char *argv[])
{
    assert(argv);
    assert(argc > 0);

    EnvironmentInfo env_settings = {};
    // env_settings.use_custom_settings = false;
    // env_settings.save_final_settings = false;

    // for (int i = 1; i < (1 + 3) && i < argc; i += 2)   // два флага, два имени файла, при каждом i рассматриваем argv[i] и argv[i+1]
    // {
    //     if (strcmp(argv[i], CUSTOM_SETTINGS_FLAG) == 0)
    //     {
    //         env_settings.use_custom_settings = true;
    //         env_settings.source_settings_file = argv[i + 1];

    //         FILE *settings_file = fopen(env_settings.source_settings_file, "r");
    //         fscanf(settings_file, "%lf %lf %lu %lf %d", 
    //               &env_settings.start_offset.x, &env_settings.start_offset.y, &env_settings.max_calc_iterations_num, &env_settings.start_scale, &env_settings.border_radius);
            
    //         fclose(settings_file);
    //     }

    //     else if (strcmp(argv[i], SAVE_FINAL_SETTINGS_FLAG) == 0)
    //     {
    //         env_settings.save_final_settings = true;
    //         env_settings.dest_settings_file = argv[i + 1];
    //     }
    // }

    if (env_settings.use_custom_settings == false)
    {
        fprintf(stderr, "popka\n");
        env_settings.start_offset            = {0, 0};
        env_settings.max_calc_iterations_num = MAX_SEQUENCE_N;
        env_settings.start_scale             = START_SCALE;
        env_settings.border_radius           = BORDER_RADIUS_SQ;
    }

    return env_settings;
}