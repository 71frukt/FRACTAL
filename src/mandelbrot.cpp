#include <stdio.h>
#include <math.h>

#include "mandelbrot.h"

FractalError DrawPixel(Pixel pixel, sf::RenderWindow *const window)       // pixel = left-up corner of pixel (PIXEL_SIZE x PIXEL_SIZE)
{
    if (window == NULL) return FRACTAL_ERROR(WINDOW_PTR_ERR);

    sf::VertexArray vert_arr(sf::PrimitiveType::Points, 1);

    vert_arr[0].position = sf::Vector2f(pixel.coord.x, pixel.coord.y);
    vert_arr[0].color    = pixel.color;

    window->draw(vert_arr);

    return SUCCESS_EXIT;
}

FractalError DrawMandelbrot(const EnvironmentInfo *const env_info)
{
    ENV_INFO_ASSERT(env_info);

    const size_t max_calc_iterations_num = env_info->max_calc_iterations_num;
    const int    border_radius_sq = env_info->border_radius_sq;

    Vector2i window_size = {(int)env_info->window->getSize().x , (int)env_info->window->getSize().y};

    static double inverse_window_size_x = 1 / (double)window_size.x;  // с помощью TYPICAL_WINDOW_SIZE сдвигаем разряд для большей точности
    // static double inverse_window_size_y = 1 / (double)window_size.y; // useless
    
    Vector2i lu_window_corner = {-window_size.x / 2 , -window_size.y / 2};              // ox directed to left, oy - down
    Vector2i rd_window_corner = { window_size.x / 2 ,  window_size.y / 2};
    
    Vector2d offset_on_complex_plane = {env_info->offset.x / (double)window_size.x, env_info->offset.y / (double)window_size.y}; 

    for (int y = lu_window_corner.y; y <= rd_window_corner.y; y++)
    {
        for (int x = lu_window_corner.x; x <= rd_window_corner.x; x++)
        {
            Vector2d const_complex = {(double)(x * env_info->scale * inverse_window_size_x) + offset_on_complex_plane.x ,
                                      (double)(y * env_info->scale * inverse_window_size_x) + offset_on_complex_plane.y};
            Vector2d cur_complex   = {0, 0};
            Vector2d prev_complex  = {0, 0};
            
            size_t n = 0;
            for ( ; n <= max_calc_iterations_num; n++)
            {
                cur_complex  = SquareComplex(prev_complex) + const_complex;
                prev_complex = cur_complex;
                
                int cur_radius_sq = cur_complex.x * cur_complex.x + cur_complex.y * cur_complex.y;
                
                if (cur_radius_sq > border_radius_sq)
                    break;
            }
            
            sf::Color pixel_color = DarkTurquoiseColoring(n, max_calc_iterations_num);

            Vector2i cur_point = Vector2i {x, y} + rd_window_corner;
            Pixel pixel = {(cur_point), pixel_color};

            ERROR_HANDLER(DrawPixel(pixel, env_info->window));
        }
    }

    ENV_INFO_ASSERT(env_info);

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

FractalError KeyboardHandler(const sf::Event::KeyPressed *const key_event, EnvironmentInfo *const env_info)
{
    ENV_INFO_ASSERT(env_info);
    KEYBOARD_ASSERT(key_event);

    switch (key_event->code) 
    {
        case sf::Keyboard::Key::D:
            env_info->offset.x += OFFSET_VELOCITY / START_SCALE * env_info->scale;
            break;

        case sf::Keyboard::Key::A:
            env_info->offset.x -= OFFSET_VELOCITY / START_SCALE * env_info->scale;
            break;

        case sf::Keyboard::Key::W:
            env_info->offset.y -= OFFSET_VELOCITY / START_SCALE * env_info->scale;
            break;

        case sf::Keyboard::Key::S:
            env_info->offset.y += OFFSET_VELOCITY / START_SCALE * env_info->scale;
            break;

        case sf::Keyboard::Key::Add:
            env_info->scale *= 0.9;
            break;

        case sf::Keyboard::Key::Subtract:
            env_info->scale *= 1.1;
            break;

        default:
            break;
    }

    ENV_INFO_ASSERT(env_info);
    KEYBOARD_ASSERT(key_event);

    return SUCCESS_EXIT;
}

FractalError PrintFPS(EnvironmentInfo *const env_info, sf::Text *fps_text)
{
    ENV_INFO_ASSERT(env_info);

    static size_t    frame_count = 0;
    static sf::Clock fps_clock;

    
    if (++frame_count >= FPS_RATIO)
    {
        float fps = FPS_RATIO / fps_clock.restart().asSeconds();
        frame_count = 0;

        char fps_buffer[10] = {};
        sprintf(fps_buffer, "%.2f", fps);
        
        fps_text->setString(fps_buffer);
    }

    env_info->window->draw(*fps_text);

    ENV_INFO_ASSERT(env_info);
    return SUCCESS_EXIT;
}

sf::Color TricolorColoring(const size_t iterations_num, const size_t max_iterations_num)
{
    uint8_t color_part = (double) iterations_num / (double) MAX_SEQUENCE_N * 255;

    if (iterations_num >= max_iterations_num)
        return sf::Color {255, 255, 255};

    else if (iterations_num < max_iterations_num / 10)
        return sf::Color {uint8_t (color_part * 8), 0, 0};
    
    else
        return sf::Color {0, 0, uint8_t (color_part * color_part)};
}

sf::Color DarkTurquoiseColoring(const size_t iterations_num, const size_t max_iterations_num)
{
    uint8_t color_part = (double) iterations_num / (double) MAX_SEQUENCE_N * 255;

    if (iterations_num >= max_iterations_num)
        return sf::Color::Black;

    else
        return sf::Color {uint8_t(color_part * 2), uint8_t ((color_part * 2) + 26), uint8_t ((color_part * 2) + 32)};
}