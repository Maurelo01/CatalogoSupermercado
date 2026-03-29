#ifndef GESTIONINVENTARIO_H
#define GESTIONINVENTARIO_H

#include <string>
#include "Producto.h"
#include "ListaEnlazada.h"
#include "ArbolAVL.h"
#include "ArbolB.h"
#include "ArbolBMas.h"

class GestionInventario
{
    private:
        ListaEnlazada listaNoOrdenada; 
        ListaEnlazada listaOrdenada;
        ArbolAVL avlNombres;
        ArbolB arbolFechas;
        ArbolBMas arbolCategoria;
        void registrarError(const string& mensaje);
        bool validarDuplicado(const string& codigo);

    public:
        GestionInventario();
        void cargarDesdeCSV(string ruta);
        bool agregarProducto(Producto* nuevo);
        bool eliminarProducto(string codigo);
        Producto* buscarPorNombreSecuencial(string nombre);
        Producto* buscarPorNombreAVL(string nombre);
        Producto* buscarPorCodigo(string codigo);
        void buscarPorCategoria(string categoria);
        void generarGraficoAVL();
        void generarGraficoB();
        void generarGraficoBMas();
        void buscarPorRangoFechas(string fechaInicio, string fechaFin);
        ~GestionInventario();
};

#endif