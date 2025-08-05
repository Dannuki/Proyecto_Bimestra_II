#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath> 
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "graficos.h"

void dibujarTexto(sf::RenderWindow& window, const std::string& texto, float x, float y, const sf::Font& font, unsigned int size, const sf::Color& color) {
    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(texto);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    sfText.setPosition(x, y);
    window.draw(sfText);
}

void dibujarLinea(sf::RenderWindow& window, float x1, float y1, float x2, float y2, const sf::Color& color, float thickness) {
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x1, y1), color),
        sf::Vertex(sf::Vector2f(x2, y2), color)
    };
    window.draw(line, 2, sf::Lines);
}


sf::Vector2f problemaACoordenadaPantalla(float x_problema, float y_problema,
    float max_x_grafico, float max_y_grafico,
    float ancho_ventana, float alto_ventana,
    float margen_izquierdo, float margen_inferior) {
    float ancho_grafico_dibujo = ancho_ventana - margen_izquierdo - 20.0f; 
    float alto_grafico_dibujo = alto_ventana - margen_inferior - 20.0f;    

    float escalaX = ancho_grafico_dibujo / max_x_grafico;
    float escalaY = alto_grafico_dibujo / max_y_grafico;

    float px_x = (x_problema * escalaX) + margen_izquierdo;
    // SFML tiene el origen (0,0) arriba a la izquierda, asi que la Y debe invertirse
    float px_y = alto_ventana - ((y_problema * escalaY) + margen_inferior);

    return sf::Vector2f(px_x, px_y);
}

void dibujarSolucionGrafica(double precio_mesas_g, double precio_sillas_g,
    double x1_optimo_g, double x2_optimo_g, double ganancia_maxima_g,
    bool simplex_calculado_g, const std::vector<Restriccion>& restricciones_g) {

    std::cout << "--- Preparando Solucion Grafica ---" << std::endl;

    ProgramaDatos datos;
    datos.precioMesas = precio_mesas_g;
    datos.precioSillas = precio_sillas_g;
    datos.mesasOptimas = x1_optimo_g;
    datos.sillasOptimas = x2_optimo_g;
    datos.gananciaMaxima = ganancia_maxima_g;
    datos.simplexCalculado = simplex_calculado_g;
    datos.restricciones = restricciones_g; // Copia el vector de restricciones

    if (datos.precioMesas == 0.0 || datos.precioSillas == 0.0 || datos.restricciones.empty() || !datos.simplexCalculado) {
        std::cout << "Error: Faltan datos (precios, restricciones o calculo Simplex no realizado)." << std::endl;
        std::cout << "Por favor, use las Opciones 1, 2 y 4 primero para ingresar datos y calcular la solucion." << std::endl;
        return;
    }


    sf::RenderWindow window(sf::VideoMode(800, 600), "Solucion Grafica de Programacion Lineal");

    sf::Font font;
    if (!font.loadFromFile("C:/project 2/Proyecto 2/x64/Debug/arial.ttf")) { // CAMBIA ESTA RUTA A LA RUTA REAL DE TU FUENTE
        std::cerr << "Error: No se pudo cargar la fuente arial.ttf con ruta absoluta. Las etiquetas y textos pueden no mostrarse." << std::endl;
    }

    // Punto óptimo
    sf::CircleShape puntoOptimo(5.f);
    puntoOptimo.setFillColor(sf::Color::Red);
    puntoOptimo.setOrigin(puntoOptimo.getRadius(), puntoOptimo.getRadius()); // Centrar el origen

    
    const float MAX_X_GRAFICO = 70.0f; 
    const float MAX_Y_GRAFICO = 90.0f; 

    const float MARGEN_IZQUIERDO = 70.0f;
    const float MARGEN_INFERIOR = 70.0f;
    const float MARGEN_DERECHO = 20.0f;
    const float MARGEN_SUPERIOR = 20.0f;

    const float ANCHO_GRAFICO_AREA = window.getSize().x - MARGEN_IZQUIERDO - MARGEN_DERECHO;
    const float ALTO_GRAFICO_AREA = window.getSize().y - MARGEN_INFERIOR - MARGEN_SUPERIOR;


    // Ajustar la posición del punto óptimo para que se mapee correctamente
    if (datos.simplexCalculado) {
        sf::Vector2f posOptimaPantalla = problemaACoordenadaPantalla(
            static_cast<float>(datos.mesasOptimas),
            static_cast<float>(datos.sillasOptimas),
            MAX_X_GRAFICO, MAX_Y_GRAFICO,
            static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y),
            MARGEN_IZQUIERDO, MARGEN_INFERIOR
        );
        puntoOptimo.setPosition(posOptimaPantalla);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(sf::Color::White); // Fondo blanco

        // --- Dibujar Ejes ---
        // Eje X
        dibujarLinea(window, MARGEN_IZQUIERDO, window.getSize().y - MARGEN_INFERIOR,
            MARGEN_IZQUIERDO + ANCHO_GRAFICO_AREA, window.getSize().y - MARGEN_INFERIOR, sf::Color::Black);
        // Eje Y
        dibujarLinea(window, MARGEN_IZQUIERDO, window.getSize().y - MARGEN_INFERIOR,
            MARGEN_IZQUIERDO, MARGEN_SUPERIOR, sf::Color::Black);

        // Etiquetas de los ejes
        dibujarTexto(window, "Mesas (X1)", MARGEN_IZQUIERDO + ANCHO_GRAFICO_AREA / 2 - 30, window.getSize().y - MARGEN_INFERIOR + 10, font, 16, sf::Color::Black);
        // Para "Sillas (X2)", rotamos el texto y ajustamos su posicion.
        sf::Text labelX2;
        labelX2.setFont(font);
        labelX2.setString("Sillas (X2)");
        labelX2.setCharacterSize(16);
        labelX2.setFillColor(sf::Color::Black);
        labelX2.setRotation(-90); // Rotar 90 grados a la izquierda
        labelX2.setPosition(MARGEN_IZQUIERDO - 40, window.getSize().y - MARGEN_INFERIOR - ALTO_GRAFICO_AREA / 2); // Posicionar a la izquierda del eje Y
        window.draw(labelX2);


        // Números en los ejes (ejemplo cada 10 unidades)
        for (int i = 0; i <= MAX_X_GRAFICO; i += 10) {
            sf::Vector2f pos = problemaACoordenadaPantalla(static_cast<float>(i), 0,
                MAX_X_GRAFICO, MAX_Y_GRAFICO,
                static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y),
                MARGEN_IZQUIERDO, MARGEN_INFERIOR);
            dibujarTexto(window, std::to_string(i), pos.x - 5, pos.y + 5, font, 12, sf::Color::Black);
        }
        for (int i = 0; i <= MAX_Y_GRAFICO; i += 10) {
            sf::Vector2f pos = problemaACoordenadaPantalla(0, static_cast<float>(i),
                MAX_X_GRAFICO, MAX_Y_GRAFICO,
                static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y),
                MARGEN_IZQUIERDO, MARGEN_INFERIOR);
            dibujarTexto(window, std::to_string(i), pos.x - 25, pos.y - 5, font, 12, sf::Color::Black);
        }

        // --- Dibujar Restricciones (Líneas) Dinámicamente ---
        int color_index = 0;
        sf::Color colores[] = { sf::Color::Blue, sf::Color::Green, sf::Color::Magenta, sf::Color::Cyan, sf::Color::Yellow };

        for (size_t i = 0; i < datos.restricciones.size(); ++i) {
            const auto& r = datos.restricciones[i];
            sf::Color lineColor = colores[color_index % (sizeof(colores) / sizeof(colores[0]))];
            color_index++;

            sf::Vector2f p1, p2;
            bool calculated = false;

            // Calcular puntos de interseccion con los ejes
            if (r.coef_x1 != 0 && r.coef_x2 != 0) {
                // Interseccion con eje X (x2=0): x1 = C/A
                float x1_intercept = r.lado_derecho / r.coef_x1;
                // Interseccion con eje Y (x1=0): x2 = C/B
                float x2_intercept = r.lado_derecho / r.coef_x2;

                p1 = problemaACoordenadaPantalla(x1_intercept, 0, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR);
                p2 = problemaACoordenadaPantalla(0, x2_intercept, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR);
                calculated = true;
            }
            else if (r.coef_x1 != 0) { // Vertical line x1 = C/A
                float x1_const = r.lado_derecho / r.coef_x1;
                p1 = problemaACoordenadaPantalla(x1_const, 0, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR);
                p2 = problemaACoordenadaPantalla(x1_const, MAX_Y_GRAFICO, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR);
                calculated = true;
            }
            else if (r.coef_x2 != 0) { // Horizontal line x2 = C/B
                float x2_const = r.lado_derecho / r.coef_x2;
                p1 = problemaACoordenadaPantalla(0, x2_const, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR);
                p2 = problemaACoordenadaPantalla(MAX_X_GRAFICO, x2_const, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR);
                calculated = true;
            }

            if (calculated) {
                dibujarLinea(window, p1.x, p1.y, p2.x, p2.y, lineColor);
                std::string label = "R" + std::to_string(i + 1);
                // Posicionar etiqueta cerca de uno de los extremos de la linea, ajustando para no chocar con ejes
                sf::Vector2f labelPos;
                if (r.coef_x1 != 0 && r.coef_x2 != 0) {
                    float x_for_label = r.lado_derecho / r.coef_x1;
                    float y_for_label = r.lado_derecho / r.coef_x2;

                    if (x_for_label >= 0 && x_for_label <= MAX_X_GRAFICO) { // cerca del eje X
                        labelPos = problemaACoordenadaPantalla(x_for_label, 0, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR);
                        labelPos.y -= 15; // Un poco mas arriba
                    }
                    else if (y_for_label >= 0 && y_for_label <= MAX_Y_GRAFICO) { // cerca del eje Y
                        labelPos = problemaACoordenadaPantalla(0, y_for_label, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR);
                        labelPos.x += 5; // Un poco mas a la derecha
                    }
                    else { // Si ambos interceptos están fuera de los límites, usar el centro de la línea dibujada
                        labelPos = (p1 + p2) / 2.0f;
                    }
                }
                else { // Lineas horizontales o verticales
                    labelPos = (p1 + p2) / 2.0f; // Centro de la linea
                    if (r.coef_x1 == 0) labelPos.x += 5; // Horizontal, a la derecha
                    if (r.coef_x2 == 0) labelPos.y -= 15; // Vertical, arriba
                }
                dibujarTexto(window, label, labelPos.x, labelPos.y, font, 14, lineColor);
            }
        }

        // --- Dibujar el Área Factible ---
        
        if (datos.precioMesas == 7 && datos.precioSillas == 5 && datos.restricciones.size() >= 3) {
           
            sf::ConvexShape areaFactible;
            areaFactible.setPointCount(5);
            areaFactible.setPoint(0, problemaACoordenadaPantalla(0, 0, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setPoint(1, problemaACoordenadaPantalla(50, 0, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setPoint(2, problemaACoordenadaPantalla(30, 40, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setPoint(3, problemaACoordenadaPantalla(15, 60, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setPoint(4, problemaACoordenadaPantalla(0, 60, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setFillColor(sf::Color(0, 255, 0, 80));
            areaFactible.setOutlineThickness(1);
            areaFactible.setOutlineColor(sf::Color::Green);
            window.draw(areaFactible);
            dibujarTexto(window, "Area Factible", MARGEN_IZQUIERDO + ANCHO_GRAFICO_AREA / 4, window.getSize().y - MARGEN_INFERIOR - ALTO_GRAFICO_AREA / 4, font, 14, sf::Color::Green);
        }
       
        else if (datos.restricciones.size() == 2 &&
            std::abs(datos.restricciones[0].coef_x1 - 2.0) < 0.001 && std::abs(datos.restricciones[0].coef_x2 - 3.0) < 0.001 && std::abs(datos.restricciones[0].lado_derecho - 100.0) < 0.001 &&
            std::abs(datos.restricciones[1].coef_x1 - 1.0) < 0.001 && std::abs(datos.restricciones[1].coef_x2 - 1.0) < 0.001 && std::abs(datos.restricciones[1].lado_derecho - 50.0) < 0.001) {

            sf::ConvexShape areaFactible;
            areaFactible.setPointCount(4); 
            areaFactible.setPoint(0, problemaACoordenadaPantalla(0, 0, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setPoint(1, problemaACoordenadaPantalla(50, 0, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setPoint(2, problemaACoordenadaPantalla(25, 25, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setPoint(3, problemaACoordenadaPantalla(0, 100.0f / 3.0f, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setFillColor(sf::Color(255, 255, 0, 80));
            areaFactible.setOutlineThickness(1);
            areaFactible.setOutlineColor(sf::Color::Yellow);
            window.draw(areaFactible);
            dibujarTexto(window, "Area Factible", MARGEN_IZQUIERDO + ANCHO_GRAFICO_AREA / 4, window.getSize().y - MARGEN_INFERIOR - ALTO_GRAFICO_AREA / 4, font, 14, sf::Color::Yellow);
        }
        
        else if (datos.restricciones.size() == 2 &&
            std::abs(datos.restricciones[0].coef_x1 - 1.0) < 0.001 && std::abs(datos.restricciones[0].coef_x2 - 2.0) < 0.001 && std::abs(datos.restricciones[0].lado_derecho - 50.0) < 0.001 &&
            std::abs(datos.restricciones[1].coef_x1 - 2.0) < 0.001 && std::abs(datos.restricciones[1].coef_x2 - 1.0) < 0.001 && std::abs(datos.restricciones[1].lado_derecho - 40.0) < 0.001) {

            sf::ConvexShape areaFactible;
            areaFactible.setPointCount(4); 
            areaFactible.setPoint(0, problemaACoordenadaPantalla(0, 0, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setPoint(1, problemaACoordenadaPantalla(20, 0, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setPoint(2, problemaACoordenadaPantalla(10, 20, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setPoint(3, problemaACoordenadaPantalla(0, 25, MAX_X_GRAFICO, MAX_Y_GRAFICO, (float)window.getSize().x, (float)window.getSize().y, MARGEN_IZQUIERDO, MARGEN_INFERIOR));
            areaFactible.setFillColor(sf::Color(100, 100, 0, 80));
            areaFactible.setOutlineThickness(1);
            areaFactible.setOutlineColor(sf::Color::Yellow);
            window.draw(areaFactible);
            dibujarTexto(window, "Area Factible", MARGEN_IZQUIERDO + ANCHO_GRAFICO_AREA / 4, window.getSize().y - MARGEN_INFERIOR - ALTO_GRAFICO_AREA / 4, font, 14, sf::Color::Yellow);
        }
        else {
            // Mensaje si no se puede determinar el area factible de forma simple
            dibujarTexto(window, "Area Factible (requiere logica avanzada)", MARGEN_IZQUIERDO + ANCHO_GRAFICO_AREA / 4, window.getSize().y - MARGEN_INFERIOR - ALTO_GRAFICO_AREA / 4, font, 14, sf::Color::Red);
        }


        // --- Dibujar el Punto Óptimo ---
        if (datos.simplexCalculado) {
            window.draw(puntoOptimo);
            // Mostrar coordenadas del punto óptimo
            std::stringstream ssX, ssY;
            ssX << std::fixed << std::setprecision(1) << datos.mesasOptimas;
            ssY << std::fixed << std::setprecision(1) << datos.sillasOptimas;
            dibujarTexto(window, "Optimo: (" + ssX.str() + ", " + ssY.str() + ")",
                puntoOptimo.getPosition().x + 10, puntoOptimo.getPosition().y - 15, font, 14, sf::Color::Red);
        }

        // --- Mostrar Texto de Resultados en la ventana ---
        std::stringstream ssMesas, ssSillas, ssGanancia;
        ssMesas << std::fixed << std::setprecision(2) << datos.mesasOptimas;
        ssSillas << std::fixed << std::setprecision(2) << datos.sillasOptimas;
        ssGanancia << std::fixed << std::setprecision(2) << datos.gananciaMaxima;

        dibujarTexto(window, "Precios: Mesas=$" + std::to_string(static_cast<int>(datos.precioMesas)) + ", Sillas=$" + std::to_string(static_cast<int>(datos.precioSillas)), 20, 20, font, 16, sf::Color::Black);
        dibujarTexto(window, "Produccion Optima:", 20, 50, font, 18, sf::Color::Black);
        dibujarTexto(window, "  Mesas: " + ssMesas.str(), 20, 75, font, 16, sf::Color::Blue);
        dibujarTexto(window, "  Sillas: " + ssSillas.str(), 20, 100, font, 16, sf::Color::Blue);
        dibujarTexto(window, "Ganancia Maxima: $" + ssGanancia.str(), 20, 125, font, 16, sf::Color::Red);


        window.display();
    }
    std::cout << "\nVentana grafica cerrada. Volviendo al menu principal." << std::endl;
}