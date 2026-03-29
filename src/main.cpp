
#include <iostream>
#include <string>
#include "../include/GestionInventario.h"
using namespace std;

void mostrarMenu()
{
    cout << "-----------------------------------------" << endl;
    cout << "   SISTEMA DE CATALOGO DE SUPERMERCADO   " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "1) Cargar catalogo desde CSV" << endl;
    cout << "2) Buscar por nombre - Lista Enlazada" << endl;
    cout << "3) Buscar por nombre - Arbol AVL" << endl;
    cout << "4) Buscar por codigo - Lista Enlazada" << endl;
    cout << "5) Buscar por rango de fechas - Arbol B" << endl;
    cout << "6) Eliminar producto" << endl;
    cout << "7) Ver errores de carga" << endl;
    cout << "8) Generar grafico del Arbol B .dot" << endl;
    cout << "9) Generar grafico del Arbol AVL .dot" << endl;
    cout << "10) Salir" << endl;
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
                inventario.cargarDesdeCSV(ruta);
                cout << "Proceso finalizado. Revise errors.log para obtener detalles." << endl;
                break;
            }
            case 2:
            {
                cout << "Ingrese el nombre exacto del producto: ";
                cin.ignore();
                getline(cin, cadenaBusqueda);
                Producto* producto = inventario.buscarPorNombreSecuencial(cadenaBusqueda);
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
                break;
            }
            case 3:
            {
                cout << "Ingrese el nombre exacto del producto: ";
                cin.ignore();
                getline(cin, cadenaBusqueda);
                Producto* producto = inventario.buscarPorNombreAVL(cadenaBusqueda);
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
                break;
            }
            case 4:
            {
                cout << "Ingrese el codigo de barras: ";
                cin >> cadenaBusqueda;
                Producto* producto = inventario.buscarPorCodigo(cadenaBusqueda);
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
                    cout << "Producto no encontrado en la Tabla Hash." << endl;
                }
                break;
            }
            case 5:
            {
                string fechaInicio, fechaFin;
                cout << "Ingrese la fecha de inicio (Año/Mes/Día): ";
                cin >> fechaInicio;
                cout << "Ingrese la fecha de fin (Año/Mes/Día): ";
                cin >> fechaFin;
                inventario.buscarPorRangoFechas(fechaInicio, fechaFin);
                break;
            }
            case 6:
                cout << "Ingrese el codigo de barras a eliminar: ";
                cin >> cadenaBusqueda;
                if (inventario.eliminarProducto(cadenaBusqueda))
                {
                    cout << "Producto eliminado de las listas." << endl;
                }
                else
                {
                    cout << "No se encontro el producto con ese codigo." << endl;
                }
                break;
            case 7:
                cout << "ULTIMOS ERRORES REGISTRADOS" << endl;
                system("tail -n 10 errors.log");
                break;
            case 8:
                inventario.generarGraficoB();
                break;
            case 9:
                inventario.generarGraficoAVL();
                break;
            case 10:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    }
    while (opcion != 10);
    return 0;
}