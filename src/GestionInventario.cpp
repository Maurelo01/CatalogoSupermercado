#include "../include/GestionInventario.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <chrono>
using namespace std;

static void imprimirDouble(double valor, int decimales)
{
    long long entero = (long long)valor;
    double fraccion  = valor - (double)entero;
    long long escala = 1;
    for (int i = 0; i < decimales; i++) escala *= 10;
    long long parteDecimal = (long long)(fraccion * escala + 0.5);
    if (parteDecimal >= escala)
    {
        entero++;
        parteDecimal -= escala;
    }
    cout << entero << ".";
    long long temp = escala / 10;
    while (temp > parteDecimal && temp > 1)
    {
        cout << "0";
        temp /= 10;
    }
    if (parteDecimal > 0) cout << parteDecimal;
    else if (decimales > 0) cout << "0";
}

static void separador(int n, char c = '-')
{
    for (int i = 0; i < n; i++) 
    {
        cout << c;
    }
    cout << endl;
    cout << endl;
}

GestionInventario::GestionInventario() : listaNoOrdenada(false, true), listaOrdenada(true, false) {}

void GestionInventario::cargarDesdeCSV(string ruta)
{
    ifstream archivo(ruta);
    ofstream log("errors.log", ios::trunc);
    string linea;
    if (!archivo.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo de datos." << endl;
        log  << "Error: La ruta no existe o el archivo no se puede leer: " << ruta << endl;
        return;
    }
    if (!getline(archivo, linea)) return;
    int cargados = 0, omitidos = 0;
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
                int    stock  = stoi(stockStr);
                Producto* producto   = new Producto(nombre, codigo, categoria, fecha, marca, precio, stock);
                if (agregarProducto(producto))
                {
                    cargados++;
                }
                else
                {
                    log << "Linea ignorada, Codigo duplicado: " << linea << endl;
                    omitidos++;
                    delete producto;
                }
            }
            catch (const exception& e)
            {
                log << "Linea malformada (formato numerico): " << linea << " > " << e.what() << endl;
                omitidos++;
            }
        }
        else
        {
            log << "Linea mal definida (faltan columnas): " << linea << endl;
            omitidos++;
        }
    }
    archivo.close();
    log.close();
    cout << "Carga finalizada: " << cargados << " insertados, " << omitidos << " omitidos." << endl;
}

bool GestionInventario::agregarProducto(Producto* nuevo)
{
    if (listaNoOrdenada.buscarPorCodigo(nuevo->codigo_barra) != nullptr)
    {
        return false; 
    }
    listaNoOrdenada.insertar(nuevo);
    listaOrdenada.insertar(nuevo);
    avlNombres.insertar(nuevo);
    arbolFechas.insertar(nuevo);
    arbolCategoria.insertar(nuevo);
    return true;
}

bool GestionInventario::eliminarProducto(string codigo)
{
    Producto* productoAEliminar = listaNoOrdenada.buscarPorCodigo(codigo);
    if (productoAEliminar == nullptr)
    {
        return false; 
    }
    string nombreAEliminar = productoAEliminar->nombre;
    string categoriaAEliminar = productoAEliminar->categoria;
    avlNombres.eliminar(nombreAEliminar);
    arbolFechas.eliminar(productoAEliminar);
    arbolCategoria.eliminarProducto(categoriaAEliminar, codigo);
    bool e1 = listaNoOrdenada.eliminar(codigo);
    bool e2 = listaOrdenada.eliminar(codigo);
    return e1 && e2;
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
    return listaNoOrdenada.buscarPorCodigo(codigo);
}

void GestionInventario::buscarPorRangoFechas(string fechaInicio, string fechaFin)
{
    arbolFechas.imprimirEnRango(fechaInicio, fechaFin);
}

void GestionInventario::buscarPorCategoria(string categoria)
{
    arbolCategoria.buscarPorCategoria(categoria);
}

void GestionInventario::listarPorNombre()
{
    avlNombres.listarEnOrden();
}

void GestionInventario::compararBusquedas(int m, int j)
{
    int totalProductos = avlNombres.contarNodos();
    if (totalProductos == 0)
    {
        cout << "No hay productos cargados para realizar la comparación." << endl;
        return;
    }
    if (m > totalProductos) m = totalProductos;
    string* nombres = new string[m];
    int recolectados = 0;
    avlNombres.obtenerNombres(nombres, recolectados, m);
    const string nombreInexistente = "PRODUCTO NO EXISTE";
    long long tiempoListaExitosa = 0;
    long long tiempoListaFallida = 0;
    for (int rep = 0; rep < j; rep++)
    {
        auto t0 = chrono::high_resolution_clock::now();
        for (int i = 0; i < m; i++)
        {
            listaNoOrdenada.buscarSecuencia(nombres[i]);
        }
        auto t1 = chrono::high_resolution_clock::now();
        tiempoListaExitosa += chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
        auto t2 = chrono::high_resolution_clock::now();
        listaNoOrdenada.buscarSecuencia(nombreInexistente);
        auto t3 = chrono::high_resolution_clock::now();
        tiempoListaFallida += chrono::duration_cast<chrono::microseconds>(t3 - t2).count();
    }
    long long tiempoAVLExitosa = 0;
    long long tiempoAVLFallida = 0;
    for (int rep = 0; rep < j; rep++)
    {
        auto t0 = chrono::high_resolution_clock::now();
        for (int i = 0; i < m; i++)
        {
            avlNombres.buscarPorNombre(nombres[i]);
        }
        auto t1 = chrono::high_resolution_clock::now();
        tiempoAVLExitosa += chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
        auto t2 = chrono::high_resolution_clock::now();
        avlNombres.buscarPorNombre(nombreInexistente);
        auto t3 = chrono::high_resolution_clock::now();
        tiempoAVLFallida += chrono::duration_cast<chrono::microseconds>(t3 - t2).count();
    }
    double denominador = (double)j * m;
    double promListaExito = (double)tiempoListaExitosa / denominador;
    double promAVLExito = (double)tiempoAVLExitosa / denominador;
    double promListaFallido = (double)tiempoListaFallida / (double)j;
    double promAVLFallido = (double)tiempoAVLFallida / (double)j;
    long long totalLista = tiempoListaExitosa + tiempoListaFallida;
    long long totalAVL = tiempoAVLExitosa + tiempoAVLFallida;
    cout << endl;
    separador(58);
    cout << "--- Comparacion Lista Enlazada - Arbol AVL ---" << endl;
    cout << " m = " << m << " busquedas por repeticion" << " | j = " << j << " repeticiones" << endl << endl;
    separador(58);
    
    cout << "--- Busquedas exitosas - " << m << " nombres reales ---" << endl;
    separador(58);
    cout << "  Tiempo total Lista (us): " << tiempoListaExitosa << endl;
    cout << "  Tiempo total AVL (us): " << tiempoAVLExitosa   << endl;
    cout << "  Promedio por busqueda Lista: "; imprimirDouble(promListaExito, 4); cout << " us" << endl;
    cout << "  Promedio por busqueda AVL: "; imprimirDouble(promAVLExito,   4); cout << " us" << endl << endl;
 
    cout << "--- Busquedas fallidas - nombre inexistente ---" << endl;
    separador(58);
    cout << "  Tiempo total Lista (us): " << tiempoListaFallida << endl;
    cout << "  Tiempo total AVL (us): " << tiempoAVLFallida   << endl;
    cout << "  Promedio por busqueda Lista: "; imprimirDouble(promListaFallido, 4); cout << " us" << endl;
    cout << "  Promedio por busqueda AVL: "; imprimirDouble(promAVLFallido,   4); cout << " us" << endl << endl;
    
    cout << "--- Totales --- " << endl;
    separador(58);
    cout << "  Lista Enlazada (us): " << totalLista << endl;
    cout << "  Arbol AVL (us): " << totalAVL   << endl;
    cout << "  Resultado: " << (totalLista > totalAVL ? "AVL es mas rapido" : "Lista es mas rapida") << endl;
    if (promAVLExito > 0.0)
    {
        double factor = promListaExito / promAVLExito;
        cout << "Factor de mejora AVL - Lista exitosa: x";
        imprimirDouble(factor, 2);
        cout << endl << endl;
    }
   
    cout << " --- Complejidades teoricas --- " << endl;
    separador(58);
    cout << "  Lista Enlazada: O(n) " << endl;
    cout << "  Arbol AVL: O(log n) " << endl;
    separador(58);
    delete[] nombres;
}

void GestionInventario::generarGraficoAVL()
{
    avlNombres.crearGrafico("arbol.dot");
    cout << "Archivo 'arbol.dot' generado exitosamente, para verlo usa Graphviz." << endl;
}

void GestionInventario::generarGraficoB()
{
    arbolFechas.crearGrafico("arbolB.dot");
    cout << "Archivo arbolB.dot generado exitosamente, para verlo usa Graphviz." << endl;
}

void GestionInventario::generarGraficoBMas()
{
    arbolCategoria.crearGrafico("arbolBMas.dot");
    cout << "Archivo arbolBMas.dot generado exitosamente, para verlo usa Graphviz." << endl;
}

GestionInventario::~GestionInventario()
{
}