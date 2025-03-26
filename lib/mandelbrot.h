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

struct UserScreen
{
    sf::RenderWindow *window;
    Vector2d          offset;
    double            scale;
};

FractalError DrawPixel       (Pixel pixel, sf::RenderWindow *const window);       // pixel = left-up corner of pixel (PIXEL_SIZE x PIXEL_SIZE)
FractalError DrawMandelbrot  (const UserScreen *const user_screen);
FractalError KeyboardHandler (const sf::Event::KeyPressed* key_event, UserScreen *user_screen);

Vector2i GetWindowOffset(const sf::Event::KeyPressed* key_event);

#define DARK_TURQUOISE_COLORING(color_part) (n >= MAX_SEQUENCE_N ? sf::Color::Black : sf::Color {uint8_t(color_part * 2), uint8_t ((color_part * 2) + 26), uint8_t ((color_part * 2) + 32)})

#ifdef GRAPH_DEBUG

#define USER_SCREEN_ASSERT(user_screen)  do                     \
{                                                                \
    CodePlace assert_place = {__FILE__, __func__, __LINE__};      \
                                                                   \
    if (user_screen == NULL)                                        \
        return {USER_SCREEN_PTR_ERR, assert_place};                  \
                                                                      \
    else if (user_screen->window == NULL)                              \
        return {WINDOW_PTR_ERR, assert_place};                          \
                                                                         \
    else if (user_screen->scale < 0)                                      \
        return {SCALE_SIGN_ERR, assert_place};                             \
} while (0)

#define KEYBOARD_ASSERT(key_event)  do                          \
{                                                                \
    CodePlace assert_place = {__FILE__, __func__, __LINE__};      \
                                                                   \
    if (key_event == NULL)                                          \
        return {KEY_EVENT_ERR, assert_place};                        \
} while (0)

#else
#define USER_SCREEN_ASSERT(user_screen)
#define KEYBOARD_ASSERT(key_event)
#endif

#endif