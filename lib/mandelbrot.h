#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <SFML/Graphics.hpp>

#include "fractal_debug.h"
#include "complex_nums.h"

using Vector2i = sf::Vector2i;
using Vector2u = sf::Vector2u;
using Vector2f = sf::Vector2f;
using Vector2d = sf::Vector2<double>;
using Color    = sf::Color;

const int BORDER_RADIUS_SQ = 3 * 3;         // the boundary, after crossing which the sequence of pixels in the N-th degree is considered infinitely large
const int MAX_SEQUENCE_N   = 64;

const double START_SCALE  = 2;              // the length of the segment that fits horizontally on the screen = 4
const int OFFSET_VELOCITY = 30;             // pixels per key press

struct Pixel
{
    Vector2i coord;
    Color    color;
};

struct EnvironmentInfo
{
    sf::RenderWindow *window;
    Vector2d          offset;
    double            scale;

    size_t            max_calc_iterations_num;
    int               border_radius_sq;

    bool              use_custom_settings;
    const char       *source_settings_file;
    
    bool              save_final_settings;
    const char       *dest_settings_file;
};

FractalError DrawPixel          (Pixel pixel, sf::RenderWindow *const window);       // pixel = left-up corner of pixel (PIXEL_SIZE x PIXEL_SIZE)
FractalError DrawMandelbrot     (const EnvironmentInfo *const env_info);
FractalError KeyboardHandler    (const sf::Event::KeyPressed* key_event, EnvironmentInfo *env_info);

Vector2i GetWindowOffset(const sf::Event::KeyPressed* key_event);

sf::Color TricolorColoring(size_t iterations_num, size_t max_iterations_num);

#define DARK_TURQUOISE_COLORING(color_part) (n >= MAX_SEQUENCE_N ? sf::Color::Black : sf::Color {uint8_t(color_part * 2), uint8_t ((color_part * 2) + 26), uint8_t ((color_part * 2) + 32)})

#ifdef GRAPH_DEBUG

#define ENV_INFO_ASSERT(env_info)  do                                                \
{                                                                                     \
    CodePlace assert_place = {__FILE__, __func__, __LINE__};                           \
                                                                                        \
    if (env_info == NULL)                                                                \
        return {ENV_INFO_PTR_ERR, assert_place};                                          \
                                                                                           \
    else if (env_info->window == NULL)                                                      \
        return {WINDOW_PTR_ERR, assert_place};                                               \
                                                                                              \
    else if (env_info->scale < 0)                                                              \
        return {SCALE_SIGN_ERR, assert_place};                                                  \
                                                                                                 \
    else if (env_info->use_custom_settings == true && env_info->source_settings_file == NULL)     \
        return {SOURCE_SETTINGS_FILE_ERR, assert_place};                                           \
                                                                                                    \
    else if (env_info->save_final_settings == true && env_info->dest_settings_file == NULL)          \
        return {DEST_SETTINGS_FILE_ERR, assert_place};                                                \
} while (0)

#define KEYBOARD_ASSERT(key_event)  do                          \
{                                                                \
    CodePlace assert_place = {__FILE__, __func__, __LINE__};      \
                                                                   \
    if (key_event == NULL)                                          \
        return {KEY_EVENT_ERR, assert_place};                        \
} while (0)

#else
#define ENV_INFO_ASSERT(env_info)
#define KEYBOARD_ASSERT(key_event)
#endif

#endif