#include <iostream>
#include "menu.h"

void mostrarMenu() {
    std::cout << "--- Menu de Optimizacion de Produccion ---" << std::endl;
    std::cout << "1. Ingreso de precios de venta (mesas y sillas)" << std::endl;
    std::cout << "2. Ingreso de restricciones de produccion" << std::endl;
    std::cout << "3. Ingreso de la Funcion de Ganancia (Funcion a Maximizar)" << std::endl;
    std::cout << "4. Calculo de la solucion optima" << std::endl;
    std::cout << "5. Solucion grafica del problema de optimizacion" << std::endl;
    std::cout << "6. Salir del programa" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
}