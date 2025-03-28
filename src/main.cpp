#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "complex_nums.h"
#include "mandelbrot.h"
#include "flag_work.h"

int main(const int argc, const char *argv[])
{            
    EnvironmentInfo env_info = {};
    GetEnvironmentInfo(&env_info, argc, argv);

    sf::RenderWindow window(sf::VideoMode({env_info.window_width, env_info.window_heigh}), "sosal?");
    env_info.window = &window;
    
    sf::Font env_info_font;
    if (!env_info_font.openFromFile(FPS_FONT_NAME))
        fprintf(stderr, "Error when opening '%s'\n", FPS_FONT_NAME);
    sf::Text env_info_text(env_info_font);
    env_info_text.setCharacterSize(30);
    env_info_text.setFillColor(sf::Color::Green);
    env_info_text.setPosition({50, 50});
    env_info.screen_text = &env_info_text;

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

        ERROR_HANDLER(DrawMandelbrot2(&env_info));
        ERROR_HANDLER(PrintScreenText(&env_info));
        window.display();
    }

    if (env_info.save_final_settings == true)
        ERROR_HANDLER(SaveFinalSettings(&env_info));

    return 0;
}