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

    sf::Font fps_font;
    if (!fps_font.openFromFile(FPS_FONT_NAME))
        fprintf(stderr, "Error when opening '%s'\n", FPS_FONT_NAME);
    sf::Text fps_text(fps_font);
    fps_text.setCharacterSize(30);
    fps_text.setFillColor(sf::Color::Green);
    fps_text.setPosition({50, 50});

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
        ERROR_HANDLER(PrintFPS(&env_info, &fps_text));
        window.display();
    }

    if (env_info.save_final_settings == true)
        ERROR_HANDLER(SaveFinalSettings(&env_info));

    return 0;
}