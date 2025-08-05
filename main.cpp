#include <iostream> 
#include <limits>   
#include <vector>   
#include "menu.h"
#include "optimizacion.h" // Ahora contiene Restriccion y ProgramaDatos
#include "graficos.h"     // Contiene la funcion de dibujo grafico

// Variables globales para almacenar los datos del problema
double precio_mesas = 0.0;
double precio_sillas = 0.0;
std::vector<Restriccion> restricciones; 

// Para la solucion optima
double x1_optimo = 0.0;
double x2_optimo = 0.0;
double ganancia_maxima = 0.0;
bool simplex_calculado = false; // Nueva bandera para saber si el simplex ya se corrio

int main() {
    int opcion;
    do {
        mostrarMenu(); // Muestra el menu al usuario
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        // Validar la entrada para asegurar que sea un numero entero
        if (std::cin.fail()) {
            std::cout << "Entrada invalida. Por favor, ingrese un numero." << std::endl; // Mensaje de error para entrada invalida
            std::cin.clear(); // Limpiar el flag de error
            // Descartar la entrada restante en el buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; // Volver a mostrar el menu
        }

        // Limpiar el buffer de entrada despues de leer la opcion para evitar problemas con futuras entradas
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (opcion) {
        case 1:
            // Opcion 1: Ingreso de precios de venta
            ingresarPrecios(precio_mesas, precio_sillas);
            simplex_calculado = false; // Resetear la bandera si se cambian los precios
            break;
        case 2:
            // Opcion 2: Ingreso de restricciones de produccion
            ingresarRestricciones(restricciones); // Ahora pasa el vector de restricciones
            simplex_calculado = false; // Resetear la bandera si se cambian las restricciones
            break;
        case 3:
            // Opcion 3: Ingreso de la Funcion de Ganancia (Funcion a Maximizar)
            mostrarFuncionGanancia(precio_mesas, precio_sillas);
            break;
        case 4:
            // Opcion 4: Calculo de la solucion optima
            calcularSolucionOptima(precio_mesas, precio_sillas, restricciones, x1_optimo, x2_optimo, ganancia_maxima, simplex_calculado);
            break;
        case 5:
            // Opcion 5: Solucion grafica del problema de optimizacion
            dibujarSolucionGrafica(precio_mesas, precio_sillas, x1_optimo, x2_optimo, ganancia_maxima, simplex_calculado, restricciones);
            break;
        case 6:
            // Opcion 6: Salir del programa
            std::cout << "Gracias por usar el sistema de optimizacion, vuelva pronto." << std::endl;
            break;
        default:
            std::cout << "Opcion no valida. Por favor, intente de nuevo." << std::endl;
            break;
        }
        std::cout << std::endl; 
    } while (opcion != 6);

    return 0;
}