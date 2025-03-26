#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "complex_nums.h"
#include "mandelbrot.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({1800, 1800}), "sosal?");
    UserScreen user_screen = {&window, {0, 0}, START_SCALE};

    while (window.isOpen()) 
    {
        while (std::optional<sf::Event> event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) 
                window.close();

            if (const sf::Event::KeyPressed *key_event = event->getIf<sf::Event::KeyPressed>())
                user_screen.offset += GetWindowOffset(key_event);            
        }

        window.clear(sf::Color::Black);
        printf("shift.x = %d\n", user_screen.offset.x);

        DrawMandelbrot(&user_screen);

        window.display();

        user_screen.scale -= 0.5;

        printf("Narisoval monda\n");
    }

    return 0;
}