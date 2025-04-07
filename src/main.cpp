#include <stdio.h>
#include <SFML/Graphics.hpp>

#include "complex_nums.h"
#include "mandelbrot.h"
#include "flag_work.h"
#include "benchmark.h"

int main(const int argc, const char *argv[])
{          
    #ifdef TEST_BENCHMARK
        TestBenchmark();
        return 0;
    #endif

    EnvironmentInfo env_info = {};
    GetEnvInfoFromCmd(&env_info, argc, argv);
    
    sf::RenderWindow window(sf::VideoMode({env_info.window_width, env_info.window_heigh}), "mandelbrot");
    env_info.window = &window;

    sf::VertexArray vertex_array(sf::PrimitiveType::Points, env_info.window_width * env_info.window_heigh);
    env_info.vertex_array = vertex_array;

    sf::Font env_info_font;
    if (!env_info_font.openFromFile(FPS_FONT_NAME))
        fprintf(stderr, "Error when opening '%s'\n", FPS_FONT_NAME);
        
    sf::Text env_info_text(env_info_font);
    env_info_text.setCharacterSize(30);
    env_info_text.setFillColor(sf::Color::Green);
    env_info_text.setPosition({50, 50});
    env_info.screen_text = &env_info_text;


    while (window.isOpen())
    // for (int i = 0; i < 10; i++)
    {
        while (std::optional<sf::Event> event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) 
                window.close();
        }

        window.clear(sf::Color::Black);

        ERROR_HANDLER(env_info.DrawFunc(&env_info));
        ERROR_HANDLER(PrintScreenText(&env_info));      // calculate and print fps and print scale
        ERROR_HANDLER(MovementHandler(&env_info));
        window.display();
    }

    if (env_info.save_final_settings == true)
        ERROR_HANDLER(SaveFinalSettings(&env_info));

    return 0;
}