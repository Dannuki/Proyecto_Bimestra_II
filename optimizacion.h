#ifndef OPTIMIZACION_H
#define OPTIMIZACION_H

#include <vector>

struct Restriccion {
    double coef_x1;
    double coef_x2;
    double lado_derecho;
};

struct ProgramaDatos {
    double precioMesas;
    double precioSillas;
    double mesasOptimas;
    double sillasOptimas;
    double gananciaMaxima;
    bool simplexCalculado;
    std::vector<Restriccion> restricciones; // Vector de restricciones
};

// Opcion 1: Ingreso de precios de venta
void ingresarPrecios(double& p1, double& p2);

// Opcion 2: Ingreso de restricciones de produccion
void ingresarRestricciones(std::vector<Restriccion>& restricciones); 

// Opcion 3: Muestra la funcion de ganancia
void mostrarFuncionGanancia(double p1, double p2);

// Opcion 4: Calcula la solucion optima
void calcularSolucionOptima(double p1, double p2, const std::vector<Restriccion>& restricciones, double& x1_optimo, double& x2_optimo, double& ganancia_maxima, bool& simplex_calculado); // Ahora pasa el vector y la bandera
#endif