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

const unsigned int DEFAULT_WINDOW_WIDTH = 800;
const unsigned int DEFAULT_WINDOW_HEIGH = 800;

const int BORDER_RADIUS_SQ   = 3 * 3;        // the boundary, after crossing which the sequence of pixels in the N-th degree is considered infinitely large
const int MAX_SEQUENCE_N     = 64;

const double START_SCALE     = 2;            // the length of the segment that fits horizontally on the screen = 4
const double SCALE_VELOCITY  = 0.2;          // scale(t0 + 1 second) = scale(t0) * (1 - SCALE_VELOCITY)  --  if pressed "+"
const double OFFSET_VELOCITY = 0.1;          // units per second on complex plane when scale = 1
const double ACCELERATE_COEFF_SPEED = 1;     // every second of continuous movement its speed increases by 100%

const size_t FPS_RATIO = 10;                 // env_info = FPS_RATIO / elapsed_time
const char *const FPS_FONT_NAME = "build/fonts/fps_font.ttf";

const size_t ENV_INFO_BUFFER_LEN = 30;



struct Pixel
{
    Vector2i coord;
    Color    color;
};

struct EnvironmentInfo
{
    FractalError    (*DrawFunc)(const EnvironmentInfo *const env_info);

    sf::RenderWindow *window;
    unsigned int      window_width;
    unsigned int      window_heigh;

    sf::VertexArray   vertex_array;     // array of screen pixels

    sf::Text         *screen_text;
    float             cur_fps;

    Vector2d          offset;   // in pixels
    double            scale;

    size_t            max_calc_iterations_num;
    double            border_radius_sq;

    bool              use_custom_settings;
    const char       *source_settings_file;
    
    bool              save_final_settings;
    const char       *dest_settings_file;
};

void DrawPixel                  (Pixel pixel, sf::RenderWindow *const window);       // pixel = left-up corner of pixel (PIXEL_SIZE x PIXEL_SIZE)

FractalError DrawMandelbrot0            (const EnvironmentInfo *const env_info);            // the first version
FractalError DrawMandelbrot_cycles      (const EnvironmentInfo *const env_info);            // cycles of 4
FractalError DrawMandelbrot_intrinsics  (const EnvironmentInfo *const env_info);
FractalError DrawMandelbrot_deployment  (const EnvironmentInfo *const env_info);


FractalError MovementHandler    (EnvironmentInfo *const env_info);

FractalError CalculateFps       (EnvironmentInfo *const env_info);
FractalError PrintScreenText    (EnvironmentInfo *const env_info);                  // FPS, scale and mb other info

Vector2d     GetWindowOffset    (const sf::Event::KeyPressed *const key_event);


sf::Color TricolorColoring      (const size_t iterations_num, const size_t max_iterations_num);
sf::Color DarkTurquoiseColoring (const size_t iterations_num, const size_t max_iterations_num);


#ifndef NO_GRAPHICS 
#define ON_GRAPH_MODE(_VA_ARGS_)  _VA_ARGS_
#else
#define ON_GRAPH_MODE(...)
#endif



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