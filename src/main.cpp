#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "complex_nums.h"
#include "mandelbrot.h"
#include "flag_work.h"

int main(const int argc, const char *argv[])
{
    EnvironmentInfo env_settings = GetEnvironmentInfo(argc, argv);
    
    sf::RenderWindow window(sf::VideoMode({800, 800}), "sosal?");
    UserScreen user_screen = {};
    SetWindowSettings(&user_screen, &window, &env_settings);

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

        ERROR_HANDLER(DrawMandelbrot(&user_screen, &env_settings));

        window.display();

        printf("Narisoval monda\n");
    }

    return 0;
}