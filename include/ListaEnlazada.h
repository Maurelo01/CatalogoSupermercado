#ifndef LISTAENLAZADA_H
#define LISTAENLAZADA_H

#include "Producto.h"
#include <iostream>
#include <string>

struct Nodo
{
    Producto* dato;
    Nodo* siguiente;
    Nodo(Producto* p) : dato(p), siguiente(nullptr) {}
};

class ListaEnlazada
{
    private:
        Nodo* cabeza;
        bool ordenada;
        bool limpio;

    public:
        ListaEnlazada(bool esOrdenada, bool limpia = false) : cabeza(nullptr), ordenada(esOrdenada), limpio(limpia) {}
        void insertar(Producto* p)
        {
            Nodo* nuevo = new Nodo(p);
            if (!cabeza || (!ordenada))
            {
                nuevo->siguiente = cabeza;
                cabeza = nuevo;
            }
            else
            {
                Nodo* actual = cabeza;
                Nodo* anterior = nullptr;
                while (actual && actual->dato->nombre < p->nombre)
                {
                    anterior = actual;
                    actual = actual->siguiente;
                }
                if (!anterior)
                {
                    nuevo->siguiente = cabeza;
                    cabeza = nuevo;
                }
                else
                {
                    anterior->siguiente = nuevo;
                    nuevo->siguiente = actual;
                }
            }
        }

        Producto* buscarSecuencia(std::string nombreBuscado)
        {
            Nodo* actual = cabeza;
            while (actual != nullptr)
            {
                if (actual->dato->nombre == nombreBuscado)
                {
                    return actual->dato;
                }
                actual = actual->siguiente;
            }
            return nullptr;
        }

        Producto* buscarPorCodigo(std::string codigo)
        {
            Nodo* actual = cabeza;
            while (actual != nullptr)
            {
                if (actual->dato->codigo_barra == codigo)
                {
                    return actual->dato;
                }
                actual = actual->siguiente;
            }
            return nullptr;
        }
        
        bool eliminar(std::string codigo)
        {
            Nodo* actual = cabeza;
            Nodo* anterior = nullptr;
            while (actual != nullptr)
            {
                if (actual->dato->codigo_barra == codigo)
                {
                    if (anterior == nullptr)
                    {
                        cabeza = actual->siguiente;
                    }
                    else
                    {
                        anterior->siguiente = actual->siguiente;
                    }
                    if (limpio && actual->dato != nullptr)
                    {
                        delete actual->dato;
                        actual->dato = nullptr;
                    }
                    delete actual;
                    return true;
                }
                anterior = actual;
                actual = actual->siguiente;
            }
            return false;
        }

        ~ListaEnlazada()
        {
            Nodo* actual = cabeza;
            while (actual != nullptr)
            {
                Nodo* siguiente = actual->siguiente;
                if (limpio && actual->dato != nullptr)
                {
                    delete actual->dato;
                    actual->dato = nullptr;
                }
                delete actual; 
                actual = siguiente;
            }
        }
};

#endif