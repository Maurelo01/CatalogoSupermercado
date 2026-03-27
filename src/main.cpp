
#include <iostream>
#include <string>
#include "../include/GestionInventario.h"

void mostrarMenu()
{
    cout << "-----------------------------------------" << endl;
    cout << "   SISTEMA DE CATALOGO DE SUPERMERCADO   " << endl;
    cout << "-----------------------------------------" << endl;
    cout << "1) Cargar catalogo desde CSV" << endl;
    cout << "2) Buscar producto por nombre" << endl;
    cout << "3) Eliminar producto" << endl;
    cout << "4) Ver errores de carga" << endl;
    cout << "5) Salir" << endl;
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
                cin >> ruta;
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
                Producto* p = inventario.buscarPorNombreSecuencial(cadenaBusqueda);
                if (p)
                {
                    cout << "PRODUCTO ENCONTRADO:" << endl;
                    cout << " - Codigo: " << p->codigo_barra << endl;
                    cout << " - Precio: Q" << p->precio << endl;
                    cout << " - Stock: " << p->stock << endl;
                }
                else
                {
                    cout << "Producto no encontrado." << endl;
                }
                break;
            }
            case 3:
            {
                cout << "Ingrese el codigo de barras a eliminar: ";
                cin >> cadenaBusqueda;
                if (inventario.eliminarProducto(cadenaBusqueda))
                {
                    cout << "Producto eliminado de todas las estructuras." << endl;
                }
                else
                {
                    cout << "No se encontro el producto con ese codigo." << endl;
                }
                break;
            }
            case 4:
            {
                cout << "--- ULTIMOS ERRORES REGISTRADOS ---" << endl;
                system("tail -n 10 errors.log");
                break;
            }
            case 5:
                cout << "Saliendo del sistema..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    }
    while (opcion != 5);
    return 0;
}