#ifndef GRAFICOS_H
#define GRAFICOS_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "optimizacion.h"

// Declaraciones de funciones auxiliares de dibujo
void dibujarTexto(sf::RenderWindow& window, const std::string& texto, float x, float y, const sf::Font& font, unsigned int size, const sf::Color& color);
void dibujarLinea(sf::RenderWindow& window, float x1, float y1, float x2, float y2, const sf::Color& color, float thickness = 2.0f);

// Funcion principal para dibujar la solucion grafica
void dibujarSolucionGrafica(double precio_mesas, double precio_sillas, double x1_optimo, double x2_optimo, double ganancia_maxima, bool simplex_calculado, const std::vector<Restriccion>& restricciones);

#endif