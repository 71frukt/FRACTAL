#include <stdio.h>
#include <math.h>
#include <immintrin.h>


#include "mandelbrot.h"
#include "unfold_vector_funcs.h"


void DrawPixel(Pixel pixel, sf::RenderWindow *const window)       // pixel = left-up corner of pixel (PIXEL_SIZE x PIXEL_SIZE)
{
    assert(window);
    sf::VertexArray vert_arr(sf::PrimitiveType::Points, 1);

    vert_arr[0].position = sf::Vector2f(pixel.coord.x, pixel.coord.y);
    vert_arr[0].color    = pixel.color;

    window->draw(vert_arr);
}

FractalError DrawMandelbrot0(const EnvironmentInfo *const env_info)
{
    ENV_INFO_ASSERT(env_info);

    sf::VertexArray vertex_array = env_info->vertex_array;

    const size_t max_calc_iterations_num = env_info->max_calc_iterations_num;
    const int    border_radius_sq        = env_info->border_radius_sq;

    Vector2i window_size = {(int)env_info->window_width , (int)env_info->window_heigh};

    static double inverse_window_size_x = 1 / (double)window_size.x;  // с помощью TYPICAL_WINDOW_SIZE сдвигаем разряд для большей точности
    // static double inverse_window_size_y = 1 / (double)window_size.y; // useless
    
    Vector2i lu_window_corner = {-window_size.x / 2 , -window_size.y / 2};              // ox directed to left, oy - down
    Vector2i rd_window_corner = { window_size.x / 2 ,  window_size.y / 2};
    
    Vector2d offset_on_complex_plane = {env_info->offset.x, env_info->offset.y}; 

    for (int y = lu_window_corner.y; y < rd_window_corner.y; y++)
    {
        for (int x = lu_window_corner.x; x < rd_window_corner.x; x++)
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

            ON_GRAPH_MODE(
            sf::Color pixel_color = DarkTurquoiseColoring(n, max_calc_iterations_num); // sf::Color::White;

            size_t num_in_vertex_array = (x - lu_window_corner.x) + (y - lu_window_corner.y) * window_size.x;

            vertex_array[num_in_vertex_array].position = Vector2f(x - lu_window_corner.x, y - lu_window_corner.y);
            vertex_array[num_in_vertex_array].color    = pixel_color;
            );
        }
    }

    ON_GRAPH_MODE(
    env_info->window->draw(vertex_array);       // vertex_array[pixel_num_x * pixel_num_y].position = Vector2f(pixel_num_x, pixel_num_x);
    );

    ENV_INFO_ASSERT(env_info);
    return SUCCESS_EXIT;
}

FractalError DrawMandelbrot_cycles(const EnvironmentInfo *const env_info)
{
    ENV_INFO_ASSERT(env_info);

    sf::VertexArray vertex_array = env_info->vertex_array;

    const size_t max_calc_iterations_num = env_info->max_calc_iterations_num;
    const int    border_radius_sq        = env_info->border_radius_sq;
    
    const size_t window_width = (size_t)env_info->window_width;
    const size_t window_heigh = (size_t)env_info->window_heigh;

    const double dx = env_info->scale / window_width;
    const double dy = env_info->scale / window_heigh;

    double left_corner_x = -env_info->scale / 2 + env_info->offset.x;
    double x0 = left_corner_x;
    double y0 = -env_info->scale / 2 + env_info->offset.y;
    
    for (size_t pixel_num_y = 0; pixel_num_y < window_heigh; pixel_num_y++, y0 += dy)
    {
        x0 = left_corner_x;
        
        for (size_t pixel_num_x = 0; pixel_num_x < window_width; pixel_num_x += 4, x0 += dx * 4)
        {
            double Y0[4] = {y0, y0, y0, y0};
            double Y [4] = {y0, y0, y0, y0};

            double X0[4] = {x0, x0 + dx, x0 + 2 * dx, x0 + 3 * dx};
            double X [4] = {x0, x0 + dx, x0 + 2 * dx, x0 + 3 * dx};
            
            size_t iteration_num_pckd [4] = {0, 0, 0, 0};
            
            for (size_t iteration_num = 0; iteration_num < max_calc_iterations_num; iteration_num++)
            {   
                
                double XX[4] = {}; 
                double YY[4] = {}; 
                double XY[4] = {}; 
                double R2[4] = {}; 
                
                for (size_t i = 0; i < 4; i++)  XX[i] = X[i] * X[i];
                for (size_t i = 0; i < 4; i++)  YY[i] = Y[i] * Y[i];
                for (size_t i = 0; i < 4; i++)  XY[i] = X[i] * Y[i];
                
                for (size_t i = 0; i < 4; i++)  R2[i] = XX[i] + YY[i];                            // расстояние до этой точки в квадрате
                
                int cmp_pckd[4] = {};
                for (size_t i = 0; i < 4; i++) 
                {
                    if (R2[i] <= border_radius_sq)
                    {
                        cmp_pckd[i] = 1;
                    }
                }
                
                bool mask = cmp_pckd[0] || cmp_pckd[1] || cmp_pckd[2] || cmp_pckd[3];
                if (!mask) break;

                for (size_t i = 0; i < 4; i++)  iteration_num_pckd  [i] = iteration_num_pckd  [i] + cmp_pckd[i];
                for (size_t i = 0; i < 4; i++)  X[i] = XX[i] - YY[i] + X0[i];    // следующая точка
                for (size_t i = 0; i < 4; i++)  Y[i] = XY[i] + XY[i] + Y0[i];
            }
            
            // printf("i = %lu\n", iteration_num_pckd[0]);
            for (size_t i = 0; i < 4; i++)
            {
                if (iteration_num_pckd[i] < max_calc_iterations_num)
                {
                    ON_GRAPH_MODE(
                    sf::Color pixel_color = DarkTurquoiseColoring(iteration_num_pckd[i], max_calc_iterations_num); // sf::Color::White;
    
                    size_t num_in_vertex_array = (pixel_num_x + i) + pixel_num_y * window_width;

                    vertex_array[num_in_vertex_array].position = Vector2f(pixel_num_x + i, pixel_num_y);
                    vertex_array[num_in_vertex_array].color    = pixel_color;
                    );
                }
            }
        }    
    }

    ON_GRAPH_MODE(
    env_info->window->draw(vertex_array);       // vertex_array[pixel_num_x * pixel_num_y].position = Vector2f(pixel_num_x, pixel_num_x);
    );

    ENV_INFO_ASSERT(env_info);
    return SUCCESS_EXIT;
}


FractalError DrawMandelbrot_intrinsics(const EnvironmentInfo *const env_info)
{
    ENV_INFO_ASSERT(env_info);

    sf::VertexArray vertex_array = env_info->vertex_array;

    __m256d TEST_R2 = _mm256_set1_pd(env_info->border_radius_sq);
    __m256d ONES = _mm256_set1_pd(1.0);

    const size_t max_calc_iterations_num = env_info->max_calc_iterations_num;
    __m256d TEST_IT_NUM = _mm256_set1_pd(max_calc_iterations_num);
    
    const size_t window_width = (size_t)env_info->window_width;
    const size_t window_heigh = (size_t)env_info->window_heigh;

    const double dx = env_info->scale / window_width;
    const double dy = dx;

    double left_corner_x = -env_info->scale / 2 + env_info->offset.x;
    double x0 = left_corner_x;
    double y0 = -env_info->scale / 2 + env_info->offset.y;
    
    for (size_t pixel_num_y = 0; pixel_num_y < window_heigh; pixel_num_y++, y0 += dy)
    {
        x0 = left_corner_x;
        
        for (size_t pixel_num_x = 0; pixel_num_x < window_width; pixel_num_x += 4, x0 += dx * 4)
        {
            __m256d Y0 = _mm256_set1_pd(y0);
            __m256d Y  = _mm256_set1_pd(y0);

            __m256d X0 = _mm256_set_pd(x0 + 3 * dx, x0 + 2 * dx, x0 + dx, x0);
            __m256d X  = _mm256_set_pd(x0 + 3 * dx, x0 + 2 * dx, x0 + dx, x0);

            __m256d iteration_num_pd = _mm256_set1_pd(0);

            for (size_t iteration_num = 0; iteration_num < max_calc_iterations_num; iteration_num++)
            {   
                
                __m256d XX = _mm256_mul_pd(X, X); 
                __m256d YY = _mm256_mul_pd(Y, Y); 
                __m256d XY = _mm256_mul_pd(X, Y);

                __m256d R2 = _mm256_add_pd(XX, YY);     // расстояние до этой точки в квадрате

                __m256d r_cmp_pd = _mm256_cmp_pd(R2, TEST_R2, _CMP_LE_OQ);
                int r_mask_bits  = _mm256_movemask_pd(r_cmp_pd);      // if r2 <= test_r2
                
                r_cmp_pd = _mm256_and_pd(r_cmp_pd, ONES);
                
                if (!r_mask_bits) break;

                iteration_num_pd = _mm256_add_pd(iteration_num_pd, r_cmp_pd);
                X = _mm256_add_pd(_mm256_sub_pd(XX, YY), X0);
                Y = _mm256_add_pd(_mm256_add_pd(XY, XY), Y0);
            }
            
            __m256d it_cmp_pd = _mm256_cmp_pd(iteration_num_pd, TEST_IT_NUM, _CMP_LT_OQ);
            int it_mask_bits  = _mm256_movemask_pd(it_cmp_pd);
            for (size_t i = 0; i < 4; i++)
            {
                if ((it_mask_bits && 1))
                {
                    ON_GRAPH_MODE(
                    sf::Color pixel_color = DarkTurquoiseColoring(iteration_num_pd[i], max_calc_iterations_num); // sf::Color::White;
    
                    size_t num_in_vertex_array = (pixel_num_x + i) + pixel_num_y * window_width;

                    vertex_array[num_in_vertex_array].position = Vector2f(pixel_num_x + i, pixel_num_y);
                    vertex_array[num_in_vertex_array].color    = pixel_color;
                    );
                }

                it_mask_bits >>= 1;
            }
        }    
    }

    ON_GRAPH_MODE(
    env_info->window->draw(vertex_array);       // vertex_array[pixel_num_x * pixel_num_y].position = Vector2f(pixel_num_x, pixel_num_x);
    );

    ENV_INFO_ASSERT(env_info);
    return SUCCESS_EXIT;
}



FractalError DrawMandelbrot_deployment(const EnvironmentInfo *const env_info)
{
    ENV_INFO_ASSERT(env_info);

    
    sf::VertexArray vertex_array = env_info->vertex_array;
    
    __m256d TEST_R2 = _mm256_set1_pd(env_info->border_radius_sq);
    __m256d ONES = _mm256_set1_pd(1.0);    
    
    
    const size_t max_calc_iterations_num = env_info->max_calc_iterations_num;
    __m256d TEST_IT_NUM = _mm256_set1_pd(max_calc_iterations_num);
    
    const size_t window_width = (size_t)env_info->window_width;
    const size_t window_heigh = (size_t)env_info->window_heigh;
    
    const double dx = env_info->scale / window_width;
    const double dy = dx;
    
    double left_corner_x = -env_info->scale / 2 + env_info->offset.x;
    double x0 = left_corner_x;
    double y0 = -env_info->scale / 2 + env_info->offset.y;
    
    size_t k = 4;   // коэффициент развёртывания
    for (size_t pixel_num_y = 0; pixel_num_y < window_heigh; pixel_num_y++, y0 += dy)
    {
        x0 = left_corner_x;
        
        for (size_t pixel_num_x = 0; pixel_num_x < window_width; pixel_num_x += 4 * k, x0 += dx * 4 * k)
        {
            SET1_UNFOLD_8(__m256d Y0, y0);
                // __m256d Y0 = _mm256_set1_pd(y0);
            SET1_UNFOLD_8(__m256d Y , y0);
                // __m256d Y  = _mm256_set1_pd(y0);


            __m256d X0_a = _mm256_set_pd(x0 + 3 * dx, x0 + 2 * dx, x0 + dx, x0);
            __m256d X_a  = _mm256_set_pd(x0 + 3 * dx, x0 + 2 * dx, x0 + dx, x0);
            
            __m256d X0_b = _mm256_set_pd(x0 + 7 * dx, x0 + 6 * dx, x0 + 5 * dx, x0 + 4 * dx);
            __m256d X_b  = _mm256_set_pd(x0 + 7 * dx, x0 + 6 * dx, x0 + 5 * dx, x0 + 4 * dx);

            __m256d X0_c = _mm256_set_pd(x0 + 11 * dx, x0 + 10 * dx, x0 + 9 * dx, x0 + 8 * dx);
            __m256d X_c  = _mm256_set_pd(x0 + 11 * dx, x0 + 10 * dx, x0 + 9 * dx, x0 + 8 * dx);

            __m256d X0_d = _mm256_set_pd(x0 + 15 * dx, x0 + 14 * dx, x0 + 13 * dx, x0 + 12 * dx);
            __m256d X_d  = _mm256_set_pd(x0 + 15 * dx, x0 + 14 * dx, x0 + 13 * dx, x0 + 12 * dx);

            // __m256d X0_e = _mm256_set_pd(x0 + 19 * dx, x0 + 18 * dx, x0 + 17 * dx, x0 + 16 * dx);
            // __m256d X_e  = _mm256_set_pd(x0 + 19 * dx, x0 + 18 * dx, x0 + 17 * dx, x0 + 16 * dx);

            // __m256d X0_f = _mm256_set_pd(x0 + 23 * dx, x0 + 22 * dx, x0 + 21 * dx, x0 + 20 * dx);
            // __m256d X_f  = _mm256_set_pd(x0 + 23 * dx, x0 + 22 * dx, x0 + 21 * dx, x0 + 20 * dx);

            // __m256d X0_g = _mm256_set_pd(x0 + 27 * dx, x0 + 26 * dx, x0 + 25 * dx, x0 + 24 * dx);
            // __m256d X_g  = _mm256_set_pd(x0 + 27 * dx, x0 + 26 * dx, x0 + 25 * dx, x0 + 24 * dx);
            
            // __m256d X0_h = _mm256_set_pd(x0 + 31 * dx, x0 + 30 * dx, x0 + 29 * dx, x0 + 28 * dx);
            // __m256d X_h  = _mm256_set_pd(x0 + 31 * dx, x0 + 30 * dx, x0 + 29 * dx, x0 + 28 * dx);

            SET1_UNFOLD_8(__m256d iteration_num_pd, 0);
                // __m256d iteration_num_pd = _mm256_set1_pd(0);


            for (size_t iteration_num = 0; iteration_num < max_calc_iterations_num; iteration_num++)
            {   
                MUL_UNFOLD_8(__m256d XX, X, X);
                    // __m256d XX = _mm256_mul_pd(X, X); 
                MUL_UNFOLD_8(__m256d YY, Y, Y);
                    // __m256d YY = _mm256_mul_pd(Y, Y); 
                MUL_UNFOLD_8(__m256d XY, X, Y);
                    // __m256d XY = _mm256_mul_pd(X, Y);

                ADD_UNFOLD_8(__m256d R2, XX, YY);
                    // __m256d R2 = _mm256_add_pd(XX, YY);     // расстояние до этой точки в квадрате

                CMP_UNFOLD_8(__m256d r_cmp_pd, R2, TEST_R2, _CMP_LE_OQ);
                    // __m256d r_cmp_pd = _mm256_cmp_pd(R2, TEST_R2, _CMP_LE_OQ);
                MOVEMASK_UNFOLD_8(int r_mask_bits, r_cmp_pd);
                    // int r_mask_bits  = _mm256_movemask_pd(r_cmp_pd);      // if r2 <= test_r2
                
                    GET_NORMAL_CMP_MASK(r_cmp_pd, r_cmp_pd, ONES);
                    // r_cmp_pd = _mm256_and_pd(r_cmp_pd, ONES);
                
                if (MASK_AND(!r_mask_bits)) break;
                    // if (!r_mask_bits) break;

                ADD_UNFOLD_8(iteration_num_pd, iteration_num_pd, r_cmp_pd);
                    // iteration_num_pd = _mm256_add_pd(iteration_num_pd, r_cmp_pd);

                SUB_UNFOLD_8(__m256d XXsubYY, XX, YY);
                ADD_UNFOLD_8(__m256d XYaddXY, XY, XY);

                ADD_UNFOLD_8(X, XXsubYY, X0);
                    // X = _mm256_add_pd(_mm256_sub_pd(XX, YY), X0);
                ADD_UNFOLD_8(Y, XYaddXY, Y0);
                    // Y = _mm256_add_pd(_mm256_add_pd(XY, XY), Y0);

            }
            
            CMP_UNFOLD_8(__m256d it_cmp_pd, iteration_num_pd, TEST_IT_NUM, _CMP_LT_OQ);
                // __m256d it_cmp_pd = _mm256_cmp_pd(iteration_num_pd, TEST_IT_NUM, _CMP_LT_OQ);

            MOVEMASK_UNFOLD_8(int it_mask_bits, it_cmp_pd);
                // int it_mask_bits  = _mm256_movemask_pd(it_cmp_pd);

            DRAW_VEC(it_mask_bits_a, iteration_num_pd_a, 0 );
            DRAW_VEC(it_mask_bits_b, iteration_num_pd_b, 4 );
            DRAW_VEC(it_mask_bits_c, iteration_num_pd_c, 8 );
            DRAW_VEC(it_mask_bits_d, iteration_num_pd_d, 12);
            // DRAW_VEC(it_mask_bits_e, iteration_num_pd_e, 16);
            // DRAW_VEC(it_mask_bits_f, iteration_num_pd_f, 20);
            // DRAW_VEC(it_mask_bits_g, iteration_num_pd_g, 24);
            // DRAW_VEC(it_mask_bits_h, iteration_num_pd_h, 28);

            // for (size_t i = 0; i < 4; i++)
            // {
                // sf::Color pixel_color = DarkTurquoiseColoring(iteration_num_pd[i], max_calc_iterations_num * (it_mask_bits && 1)); // sf::Color::White;      // if (it_mask_bits && 1) == 0 -> Black color

                // size_t num_in_vertex_array = (pixel_num_x + i) + pixel_num_y * window_width;

                // vertex_array[num_in_vertex_array].position = Vector2f(pixel_num_x + i, pixel_num_y);
                // vertex_array[num_in_vertex_array].color    = pixel_color;

            //     it_mask_bits >>= 1;
            // }
        }    
    }

    env_info->window->draw(vertex_array);       // vertex_array[pixel_num_x * pixel_num_y].position = Vector2f(pixel_num_x, pixel_num_x);

    ENV_INFO_ASSERT(env_info);
    return SUCCESS_EXIT;
}



Vector2d GetWindowOffset(const sf::Event::KeyPressed *const key_event)
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


FractalError MovementHandler(EnvironmentInfo *const env_info)
{
    ENV_INFO_ASSERT(env_info);

    static double accelerate_coeff = 1;
    bool is_movement = false;

    double movement_coeff = accelerate_coeff * env_info->scale / env_info->cur_fps;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        env_info->offset.y -= OFFSET_VELOCITY * movement_coeff;
        is_movement = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        env_info->offset.x -= OFFSET_VELOCITY * movement_coeff;
        is_movement = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        env_info->offset.y += OFFSET_VELOCITY * movement_coeff;
        is_movement = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        env_info->offset.x += OFFSET_VELOCITY * movement_coeff;
        is_movement = true;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add))
    {
        env_info->scale *= 1 - SCALE_VELOCITY * accelerate_coeff / env_info->cur_fps;
        is_movement = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract))
    {
        env_info->scale *= 1 + SCALE_VELOCITY * accelerate_coeff / env_info->cur_fps;
        is_movement = true;
    }

    if (is_movement == false)
        accelerate_coeff = 1;
    else
        accelerate_coeff += 1 / env_info->cur_fps;

    ENV_INFO_ASSERT(env_info);
    return SUCCESS_EXIT;
}


FractalError CalculateFps(EnvironmentInfo *const env_info)
{
    ENV_INFO_ASSERT(env_info);
    
    static size_t    frame_count = 0;
    static sf::Clock env_info_clock;

    if (++frame_count >= FPS_RATIO)
    {
        env_info->cur_fps = FPS_RATIO / env_info_clock.restart().asSeconds();
        frame_count = 0;
    }    

    ENV_INFO_ASSERT(env_info);
    return SUCCESS_EXIT;
}


FractalError PrintScreenText(EnvironmentInfo *const env_info)
{
    ENV_INFO_ASSERT(env_info);

    char env_info_buffer[ENV_INFO_BUFFER_LEN] = {};

    double scale = env_info->scale;

    ERROR_HANDLER(CalculateFps(env_info));

    if (scale > 1)
        snprintf(env_info_buffer, ENV_INFO_BUFFER_LEN, "%.2f\nscale = %lu : 1", env_info->cur_fps, (size_t) scale);
    
    else
        snprintf(env_info_buffer, ENV_INFO_BUFFER_LEN, "%.2f\nscale = 1 : %lu", env_info->cur_fps, (size_t) (1 / scale));

    env_info->screen_text->setString(env_info_buffer);
    env_info->window->draw(*env_info->screen_text);

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