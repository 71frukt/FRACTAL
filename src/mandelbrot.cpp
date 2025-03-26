#include <stdio.h>

#include "mandelbrot.h"

void DrawPixel(Pixel pixel, sf::RenderWindow *const window)       // pixel = left-up corner of pixel (PIXEL_SIZE x PIXEL_SIZE)
{
    sf::VertexArray vert_arr(sf::PrimitiveType::Points, 1);

    vert_arr[0].position = sf::Vector2f(pixel.coord.x, pixel.coord.y);
    vert_arr[0].color    = pixel.color;

    window->draw(vert_arr);
}


FractalError DrawMandelbrot(const UserScreen *const user_screen)
{
    
    Vector2i window_size = {(int)user_screen->window->getSize().x , (int)user_screen->window->getSize().y};

    Vector2i lu_window_corner = {-window_size.x / 2 , -window_size.y / 2};         // ox directed to left, oy - down
    Vector2i rd_window_corner = { window_size.x / 2 ,  window_size.y / 2};

    for (int y = lu_window_corner.y; y <= rd_window_corner.y; y++)
    {
        for (int x = lu_window_corner.x; x <= rd_window_corner.x; x++)
        {
            Vector2d const_complex = {(double)(x + user_screen->offset.x) * user_screen->scale / window_size.x , (double)(y + user_screen->offset.y) * user_screen->scale / window_size.x};
            Vector2d cur_complex   = {0, 0};
            Vector2d prev_complex  = {0, 0};
            
            bool limited_sequence = true;
            
            for (size_t n = 0; n <= MAX_SEQUENCE_N; n++)
            {
                cur_complex  = SquareComplex(prev_complex) + const_complex;
                prev_complex = cur_complex;
                
                int cur_radius_sq = cur_complex.x * cur_complex.x + cur_complex.y * cur_complex.y;
                
                if (cur_radius_sq > BORDER_RADIUS_SQ)
                {
                    limited_sequence = false;
                    break;
                }
            }
            
            if (0);


            if (limited_sequence)
            {
                Vector2i cur_point = {x, y};
                Pixel pixel = {cur_point + rd_window_corner, sf::Color::Red};

                DrawPixel(pixel, user_screen->window);
            }
        }
    }

    return {};
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

    switch (key_event->code) 
    {
        case sf::Keyboard::Key::D:
            user_screen->offset += {OFFSET_VELOCITY, 0};
            break;

        case sf::Keyboard::Key::A:
            user_screen->offset += {-OFFSET_VELOCITY, 0};
            break;

        case sf::Keyboard::Key::W:
            user_screen->offset += {0, -OFFSET_VELOCITY};
            break;

        case sf::Keyboard::Key::S:
            user_screen->offset += {0, OFFSET_VELOCITY};
            break;

        default:
            break;
    }

    USER_SCREEN_ASSERT(user_screen);
    KEYBOARD_ASSERT(key_event);

    return SUCCESS_EXIT;
}