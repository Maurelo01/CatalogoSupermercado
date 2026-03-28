#ifndef GESTIONINVENTARIO_H
#define GESTIONINVENTARIO_H

#include <string>
#include "Producto.h"
#include "ListaEnlazada.h"
#include "TablaHash.h"
#include "ArbolAVL.h"

class GestionInventario
{
    private:
        ListaEnlazada listaNoOrdenada; 
        ListaEnlazada listaOrdenada;
        TablaHash hashBarras;
        ArbolAVL avlNombres;
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
        void generarGraficoAVL();
        ~GestionInventario();
};

#endif