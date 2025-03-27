#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "complex_nums.h"
#include "mandelbrot.h"
#include "flag_work.h"

int main(const int argc, const char *argv[])
{            
    sf::RenderWindow window(sf::VideoMode({800, 800}), "sosal?");
    EnvironmentInfo env_info = {};
    env_info.window = &window;
    ERROR_HANDLER(GetEnvironmentInfo(&env_info, argc, argv));

    while (window.isOpen())
    {
        while (std::optional<sf::Event> event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) 
                window.close();

            if (const sf::Event::KeyPressed *key_event = event->getIf<sf::Event::KeyPressed>())
                KeyboardHandler(key_event, &env_info);
        }

        window.clear(sf::Color::Black);

        ERROR_HANDLER(DrawMandelbrot(&env_info));

        window.display();

        printf("Narisoval monda\n");
    }

    if (env_info.save_final_settings == true)
        ERROR_HANDLER(SaveFinalSettings(&env_info));

    return 0;
}