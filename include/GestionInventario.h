using namespace std;
#ifndef GESTIONINVENTARIO_H
#define GESTIONINVENTARIO_H

#include <string>
#include "Producto.h"
#include "ListaEnlazada.h"

class GestionInventario
{
    private:
        ListaEnlazada listaNoOrdenada; 
        ListaEnlazada listaOrdenada;
        void registrarError(const string& mensaje);
        bool validarDuplicado(const string& codigo);

    public:
        GestionInventario();
        void cargarDesdeCSV(string ruta);
        bool agregarProducto(Producto* nuevo);
        bool eliminarProducto(string codigo);
        Producto* buscarPorNombreSecuencial(string nombre);
        ~GestionInventario();
};

#endif