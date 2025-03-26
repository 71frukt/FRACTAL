#ifndef COMPLEX_NUMS_H
#define COMPLEX_NUMS_H

#include <SFML/Graphics.hpp>
using Vector2f = sf::Vector2f;
using Vector2d = sf::Vector2<double>;

Vector2d SquareComplex (Vector2d pixel);
Vector2d MulComplex    (Vector2d pixel_1, Vector2d pixel_2);
Vector2d AddComplex    (Vector2d pixel_1, Vector2d pixel_2);

#endif