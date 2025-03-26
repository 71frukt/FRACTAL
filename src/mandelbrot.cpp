#include <stdio.h>
#include <math.h>

#include "mandelbrot.h"


FractalError SetWindowSettings(UserScreen *const user_screen, sf::RenderWindow *window, EnvironmentInfo *const env_settings)
{
    user_screen->window = window;
    user_screen->offset = env_settings->start_offset;
    user_screen->scale  = env_settings->start_scale;

    USER_SCREEN_ASSERT(user_screen);
    return SUCCESS_EXIT;
}

FractalError DrawPixel(Pixel pixel, sf::RenderWindow *const window)       // pixel = left-up corner of pixel (PIXEL_SIZE x PIXEL_SIZE)
{
    ON_GRAPH_DEBUG(if (window == NULL) return {WINDOW_PTR_ERR, {__FILE__, __func__, __LINE__}});

    sf::VertexArray vert_arr(sf::PrimitiveType::Points, 1);

    vert_arr[0].position = sf::Vector2f(pixel.coord.x, pixel.coord.y);
    vert_arr[0].color    = pixel.color;

    window->draw(vert_arr);

    return SUCCESS_EXIT;
}


FractalError DrawMandelbrot(const UserScreen *const user_screen, const EnvironmentInfo *const env_settings)
{
    USER_SCREEN_ASSERT(user_screen);

    const size_t max_calc_iterations_num = env_settings->max_calc_iterations_num;

    Vector2i window_size = {(int)user_screen->window->getSize().x , (int)user_screen->window->getSize().y};

    static double inverse_window_size_x = 1 / (double)window_size.x;  // с помощью TYPICAL_WINDOW_SIZE сдвигаем разряд для большей точности
    // static double inverse_window_size_y = 1 / (double)window_size.y; // useless
    
    Vector2i lu_window_corner = {-window_size.x / 2 , -window_size.y / 2};              // ox directed to left, oy - down
    Vector2i rd_window_corner = { window_size.x / 2 ,  window_size.y / 2};
    
    Vector2d offset_on_complex_plane = {user_screen->offset.x / (double)window_size.x, user_screen->offset.y / (double)window_size.y}; 

    for (int y = lu_window_corner.y; y <= rd_window_corner.y; y++)
    {
        for (int x = lu_window_corner.x; x <= rd_window_corner.x; x++)
        {
            Vector2d const_complex = {(double)(x * user_screen->scale * inverse_window_size_x) + offset_on_complex_plane.x ,
                                      (double)(y * user_screen->scale * inverse_window_size_x) + offset_on_complex_plane.y};
            Vector2d cur_complex   = {0, 0};
            Vector2d prev_complex  = {0, 0};
            
            size_t n = 0;
            for ( ; n <= max_calc_iterations_num; n++)
            {
                cur_complex  = SquareComplex(prev_complex) + const_complex;
                prev_complex = cur_complex;
                
                int cur_radius_sq = cur_complex.x * cur_complex.x + cur_complex.y * cur_complex.y;
                
                if (cur_radius_sq > BORDER_RADIUS_SQ)
                    break;
            }
            

            uint8_t color_part = (double) n / (double) MAX_SEQUENCE_N * 255;
            sf::Color pixel_color = DARK_TURQUOISE_COLORING(color_part);

            Vector2i cur_point = Vector2i {x, y} + rd_window_corner;
            Pixel pixel = {(cur_point), pixel_color};

            ERROR_HANDLER(DrawPixel(pixel, user_screen->window));
        }
    }

    USER_SCREEN_ASSERT(user_screen);

    return SUCCESS_EXIT;
}


Vector2i GetWindowOffset(const sf::Event::KeyPressed *const key_event)
{
    switch (key_event->code) 
    {
        case sf::Keyboard::Key::D:
            return {OFFSET_VELOCITY, 0};

        case sf::Keyboard::Key::A:
            return {-OFFSET_VELOCITY, 0};

        case sf::Keyboard::Key::W:
            return {0, -OFFSET_VELOCITY};

        case sf::Keyboard::Key::S:
            return {0, OFFSET_VELOCITY};

        default:
            return {0, 0}; 
            break;
    }
}

FractalError KeyboardHandler(const sf::Event::KeyPressed* key_event, UserScreen *user_screen)
{
    USER_SCREEN_ASSERT(user_screen);
    KEYBOARD_ASSERT(key_event);

    printf("offset.x = %f, offset.y = %f\n", user_screen->offset.x, user_screen->offset.y);

    switch (key_event->code) 
    {
        case sf::Keyboard::Key::D:
            user_screen->offset.x += OFFSET_VELOCITY / START_SCALE * user_screen->scale;
            break;

        case sf::Keyboard::Key::A:
            user_screen->offset.x -= OFFSET_VELOCITY / START_SCALE * user_screen->scale;
            break;

        case sf::Keyboard::Key::W:
            user_screen->offset.y -= OFFSET_VELOCITY / START_SCALE * user_screen->scale;
            break;

        case sf::Keyboard::Key::S:
            user_screen->offset.y += OFFSET_VELOCITY / START_SCALE * user_screen->scale;
            break;

        case sf::Keyboard::Key::Add:
            user_screen->scale *= 0.9;
            break;

        case sf::Keyboard::Key::Subtract:
            user_screen->scale *= 1.1;
            break;

        default:
            break;
    }

    USER_SCREEN_ASSERT(user_screen);
    KEYBOARD_ASSERT(key_event);

    return SUCCESS_EXIT;
}