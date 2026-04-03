#include <iostream>
#include <string>
#include <chrono>
#include "../include/GestionInventario.h"
using namespace std;

void mostrarMenu()
{
    cout << "-----------------------------------------" << endl;
    cout << "   SISTEMA DE CATALOGO DE SUPERMERCADO   " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "1) Cargar catalogo desde CSV" << endl;
    cout << "2) Agregar producto manualmente" << endl;
    cout << "3) Buscar por nombre - Lista Enlazada" << endl;
    cout << "4) Buscar por nombre - Arbol AVL" << endl;
    cout << "5) Buscar por codigo - Lista Enlazada" << endl;
    cout << "6) Buscar por rango de fechas - Arbol B" << endl;
    cout << "7) Buscar por categoria - Arbol B+" << endl;
    cout << "8) Eliminar producto" << endl;
    cout << "9) Listar productos en orden alfabetico (AVL in-order)" << endl;
    cout << "10) Comparar busquedas: Lista - AVL" << endl;
    cout << "11) Ver errores de carga" << endl;
    cout << "12) Generar grafico del Arbol B .dot" << endl;
    cout << "13) Generar grafico del Arbol B+ .dot" << endl;
    cout << "14) Generar grafico del Arbol AVL .dot" << endl;
    cout << "15) Salir" << endl;
    cout << "Seleccione una opcion: ";
}

int main()
{
    GestionInventario inventario;
    int opcion;
    string cadenaBusqueda;
    do
    {
        mostrarMenu();
        if (!(cin >> opcion))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        switch (opcion)
        {
            case 1:
            {
                string ruta;
                cout << "Ingrese la ruta del archivo CSV: ";
                cin.ignore();
                getline(cin, ruta);
                cout << "Cargando datos... " << endl;
                auto inicio = chrono::high_resolution_clock::now();
                inventario.cargarDesdeCSV(ruta);
                auto fin = chrono::high_resolution_clock::now();
                auto duracion = chrono::duration_cast<chrono::milliseconds>(fin - inicio).count();
                cout << "Carga completada en " << duracion << " ms." << endl;
                cout << "Revise errors.log para obtener detalles." << endl;
                break;
            }
            case 2:
            {
                string nombre, codigo, categoria, fecha, marca;
                double precio;
                int stock;
                cout << " NUEVO PRODUCTO " << endl;
                cin.ignore();
                cout << "Nombre: "; getline(cin, nombre);
                cout << "Codigo de barras: "; getline(cin, codigo);
                cout << "Categoria: "; getline(cin, categoria);
                cout << "Fecha de caducidad (Año-Mes-Día): "; getline(cin, fecha);
                cout << "Marca: "; getline(cin, marca);
                cout << "Precio: Q"; cin >> precio;
                cout << "Stock: "; cin >> stock;
                Producto* nuevo = new Producto(nombre, codigo, categoria, fecha, marca, precio, stock);
                auto inicio = chrono::high_resolution_clock::now();
                bool exito = inventario.agregarProducto(nuevo);
                auto fin = chrono::high_resolution_clock::now();
                auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                if(exito)
                {
                    cout << "Producto agregado correctamente a todas las estructuras" << endl;
                }
                else
                {
                    cout << "Error: El producto con codigo " << codigo << " ya existe." << endl;
                    delete nuevo;
                }
                cout << "Tiempo de inserción: " << duracion << " microsegundos." << endl;
                break;
            }
            case 3:
            {
                cout << "Ingrese el nombre exacto del producto: ";
                cin.ignore();
                getline(cin, cadenaBusqueda);
                auto inicio = chrono::high_resolution_clock::now();
                Producto* producto = inventario.buscarPorNombreSecuencial(cadenaBusqueda);
                auto fin = chrono::high_resolution_clock::now();
                auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                if (producto)
                {
                    cout << " - Lista Enlazada - " << endl;
                    cout << "PRODUCTO ENCONTRADO:" << endl;
                    cout << " - Codigo: " << producto->codigo_barra << endl;
                    cout << " - Precio: Q" << producto->precio << endl;
                    cout << " - Stock: " << producto->stock << endl;
                }
                else
                {
                    cout << "Producto no encontrado." << endl;
                }
                cout << "Tiempo de búsqueda secuencial: " << duracion << " us." << endl;
                break;
            }
            case 4:
            {
                cout << "Ingrese el nombre exacto del producto: ";
                cin.ignore();
                getline(cin, cadenaBusqueda);
                auto inicio = chrono::high_resolution_clock::now();
                Producto* producto = inventario.buscarPorNombreAVL(cadenaBusqueda);
                auto fin = chrono::high_resolution_clock::now();
                auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                if (producto)
                {
                    cout << " - Arbol AVL - " << endl;
                    cout << "PRODUCTO ENCONTRADO" << endl;
                    cout << " - Codigo: " << producto->codigo_barra << endl;
                    cout << " - Precio: Q" << producto->precio << endl;
                    cout << " - Stock: " << producto->stock << endl;
                }
                else
                {
                    cout << "Producto no encontrado." << endl;
                }
                cout << "Tiempo de búsqueda binaria en AVL: " << duracion << " us." << endl;
                break;
            }
            case 5:
            {
                cout << "Ingrese el codigo de barras: ";
                cin >> cadenaBusqueda;
                auto inicio = chrono::high_resolution_clock::now();
                Producto* producto = inventario.buscarPorCodigo(cadenaBusqueda);
                auto fin = chrono::high_resolution_clock::now();
                auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                if (producto)
                {
                    cout << " - Lista Enlazada - " << endl;
                    cout << "PRODUCTO ENCONTRADO:" << endl;
                    cout << " - Nombre: " << producto->nombre << endl;
                    cout << " - Categoria: " << producto->categoria << endl;
                    cout << " - Precio: Q" << producto->precio << endl;
                    cout << " - Stock: " << producto->stock << endl;
                }
                else
                {
                    cout << "Producto no encontrado en la Lista Enlazada." << endl;
                }
                cout << "Tiempo de búsqueda por codigo: " << duracion << " us." << endl;
                break;
            }
            case 6:
            {
                string fechaInicio, fechaFin;
                cout << "Ingrese la fecha de inicio (Año-Mes-Día): ";
                cin >> fechaInicio;
                cout << "Ingrese la fecha de fin (Año-Mes-Día): ";
                cin >> fechaFin;
                auto inicio = chrono::high_resolution_clock::now();
                inventario.buscarPorRangoFechas(fechaInicio, fechaFin);
                auto fin = chrono::high_resolution_clock::now();
                auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                cout << "Tiempo de búsqueda en arbol B: " << duracion << " us." << endl;
                break;
            }
            case 7:
            {
                cout << "Ingrese la categoria a buscar: ";
                cin.ignore();
                getline(cin, cadenaBusqueda);
                auto inicio = chrono::high_resolution_clock::now();
                inventario.buscarPorCategoria(cadenaBusqueda);
                auto fin = chrono::high_resolution_clock::now();
                auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                cout << "Tiempo de búsqueda en arbol B+: " << duracion << " us." << endl;
                break;
            }
            case 8:
            {
                cout << "Ingrese el codigo de barras a eliminar: ";
                cin >> cadenaBusqueda;
                auto inicio = chrono::high_resolution_clock::now();
                bool eliminado = inventario.eliminarProducto(cadenaBusqueda);
                auto fin = chrono::high_resolution_clock::now();
                auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                if (eliminado)
                {
                    cout << "Producto eliminado de las listas." << endl;
                }
                else
                {
                    cout << "No se encontro el producto con ese codigo." << endl;
                }
                cout << "Tiempo de eliminación: " << duracion << " us." << endl;
                break;
            }
            case 9:
            {
                auto inicio   = chrono::high_resolution_clock::now();
                inventario.listarPorNombre();
                auto fin      = chrono::high_resolution_clock::now();
                auto duracion = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                cout << "Tiempo de recorrido in-order: " << duracion << " us." << endl;
                break;
            }
            case 10:
            {
                int m, j;
                cout << "Numero de busquedas por repeticion (m): ";
                cin >> m;
                cout << "Numero de repeticiones (j): ";
                cin >> j;
                if (m <= 0 || j <= 0)
                {
                    cout << "m y j deben ser positivos." << endl;
                    break;
                }
                inventario.compararBusquedas(m, j);
                break;
            }
            case 11:
                cout << "ULTIMOS ERRORES REGISTRADOS" << endl;
                system("tail -n 10 errors.log");
                break;
            case 12:
                inventario.generarGraficoB();
                break;
            case 13:
                inventario.generarGraficoBMas();
                break;
            case 14:
                inventario.generarGraficoAVL();
                break;
            case 15:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    }
    while (opcion != 15);
    return 0;
}