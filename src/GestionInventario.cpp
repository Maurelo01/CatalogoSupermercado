#include "../include/GestionInventario.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

GestionInventario::GestionInventario() : listaNoOrdenada(false, true), listaOrdenada(true, false) {}

void GestionInventario::cargarDesdeCSV(string ruta)
{
    ifstream archivo(ruta);
    ofstream log("errors.log", ios::app);
    string linea;
    if (!archivo.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo de datos." << endl;
        log << "Error: La ruta no existe o el archivo no se puede leer: " << ruta << endl;
        return;
    }
    if (!getline(archivo, linea)) return;
    while (getline(archivo, linea))
    {
        if (linea.empty()) continue;
        stringstream ss(linea);
        string nombre, codigo, categoria, fecha, marca, precioStr, stockStr;
        if (getline(ss, nombre, ',') && getline(ss, codigo, ',') && getline(ss, categoria, ',') && getline(ss, fecha, ',') && getline(ss, marca, ',') && getline(ss, precioStr, ',') && getline(ss, stockStr, ','))
        {
            try
            {
                double precio = stod(precioStr);
                int stock = stoi(stockStr);
                Producto* nuevoProducto = new Producto(nombre, codigo, categoria, fecha, marca, precio, stock);
                if (agregarProducto(nuevoProducto))
                {
                }
                else
                {
                    log << "Línea ignorada, Código de barra duplicada: " << linea << endl;
                    delete nuevoProducto;
                }

            }
            catch (const exception& e)
            {
                log << "Línea malformada (Error de formato numérico): " << linea << " > " << e.what() << endl;
            }
        }
        else 
        {
            log << "Línea mal definida (Faltan columnas): " << linea << endl;
        }
    }
    archivo.close();
    log.close();
}

bool GestionInventario::agregarProducto(Producto* nuevo)
{
    if (hashBarras.buscar(nuevo->codigo_barra) != nullptr)
    {
        return false;
    }
    listaNoOrdenada.insertar(nuevo);
    listaOrdenada.insertar(nuevo);
    hashBarras.insertar(nuevo);
    avlNombres.insertar(nuevo);
    return true;
}

Producto* GestionInventario::buscarPorNombreSecuencial(string nombre)
{
    return listaNoOrdenada.buscarSecuencia(nombre);
}

Producto* GestionInventario::buscarPorNombreAVL(string nombre)
{
    return avlNombres.buscarPorNombre(nombre);
}

Producto* GestionInventario::buscarPorCodigo(string codigo)
{
    return hashBarras.buscar(codigo);
}

void GestionInventario::generarGraficoAVL()
{
    avlNombres.crearGrafico("arbol.dot");
    cout << "Archivo 'arbol.dot' generado exitosamente, para verlo usa Graphviz." << endl;
}

bool GestionInventario::eliminarProducto(string codigo)
{
    bool e1 = listaNoOrdenada.eliminar(codigo);
    bool e2 = listaOrdenada.eliminar(codigo);
    return e1 && e2;
}

GestionInventario::~GestionInventario()
{
}