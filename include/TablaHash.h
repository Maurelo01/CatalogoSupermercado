#ifndef TABLAHASH_H
#define TABLAHASH_H
#include "Producto.h"
#include <string>

struct NodoHash
{
    Producto* producto;
    NodoHash* siguiente;
    NodoHash(Producto* prod) : producto(prod), siguiente(nullptr) {}
};

class TablaHash
{
private:
    static const int TAMAÑO = 997;
    NodoHash* tabla[TAMAÑO];

    int funcionHash(std::string clave)
    {
        unsigned long hash = 0;
        for (char c : clave)
        {
            hash = ((hash << 5) - hash) + c;
        }
        return hash % TAMAÑO;
    }

public:
    TablaHash()
    {
        for (int i = 0; i < TAMAÑO; i++)
        {
            tabla[i] = nullptr;
        }
    }

    void insertar(Producto* producto)
    {
        int indice = funcionHash(producto->codigo_barra);
        NodoHash* nuevoNodo = new NodoHash(producto);
        nuevoNodo->siguiente = tabla[indice];
        tabla[indice] = nuevoNodo;
    }

    Producto* buscar(std::string codigo)
    {
        int indice = funcionHash(codigo);
        NodoHash* nodoActual = tabla[indice];
        while (nodoActual)
        {
            if (nodoActual->producto->codigo_barra == codigo)
            {
                return nodoActual->producto;
            }
            nodoActual = nodoActual->siguiente;
        }
        return nullptr;
    }

    ~TablaHash()
    {
        for (int i = 0; i < TAMAÑO; i++)
        {
            NodoHash* nodoActual = tabla[i];
            while (nodoActual)
            {
                NodoHash* temp = nodoActual;
                nodoActual = nodoActual->siguiente;
                delete temp;
            }
        }
    }
};

#endif