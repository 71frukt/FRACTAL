#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "complex_nums.h"
#include "mandelbrot.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "sosal?");
    UserScreen user_screen = {&window, {0, 0}, START_SCALE};

    while (window.isOpen()) 
    {
        while (std::optional<sf::Event> event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) 
                window.close();

            if (const sf::Event::KeyPressed *key_event = event->getIf<sf::Event::KeyPressed>())
                KeyboardHandler(key_event, &user_screen);
        }

        window.clear(sf::Color::Black);
        printf("shift.x = %f\n", user_screen.offset.x);

        ERROR_HANDLER(DrawMandelbrot(&user_screen));

        window.display();

        printf("Narisoval monda\n");
    }

    return 0;
}