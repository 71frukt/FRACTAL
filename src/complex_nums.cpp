#include <stdio.h>

#include "complex_nums.h"

Vector2d AddComplex(Vector2d pixel_1, Vector2d pixel_2)
{
    Vector2d res_num = {pixel_1.x + pixel_2.x, pixel_1.y + pixel_2.y};
    return res_num;
}

Vector2d MulComplex(Vector2d pixel_1, Vector2d pixel_2)
{
    float res_x = pixel_1.x * pixel_2.x - pixel_1.y * pixel_2.y;
    float res_y = pixel_1.x * pixel_2.y + pixel_2.x * pixel_1.y;

    Vector2d res_pixel = {res_x, res_y};
    return res_pixel;
}

Vector2d SquareComplex(Vector2d pixel)
{
    float res_x = pixel.x * pixel.x - pixel.y * pixel.y;
    float res_y = 2 * pixel.x * pixel.y;

    Vector2d res_pixel = {res_x, res_y};
    return res_pixel;
}