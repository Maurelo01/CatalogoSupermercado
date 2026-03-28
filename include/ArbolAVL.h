#ifndef ARBOLAVL_H
#define ARBOLAVL_H
#include "Producto.h"
#include <algorithm>
#include <fstream>
#include <string>

struct NodoAVL
{
    Producto* producto;
    NodoAVL* izquierda;
    NodoAVL* derecha;
    int altura;
    NodoAVL(Producto* prod) : producto(prod), izquierda(nullptr), derecha(nullptr), altura(1) {}
};

class ArbolAVL
{
    private:
        NodoAVL* raiz;
        int obtenerAltura(NodoAVL* nodo)
        {
            return nodo ? nodo->altura : 0;
        }
        int obtenerBalance(NodoAVL* nodo)
        {
            return nodo ? obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha) : 0;
        }
        NodoAVL* rotarDerecha(NodoAVL* y)
        {
            NodoAVL* x = y->izquierda;
            NodoAVL* T2 = x->derecha;
            x->derecha = y;
            y->izquierda = T2;
            y->altura = 1 + std::max(obtenerAltura(y->izquierda), obtenerAltura(y->derecha));
            x->altura = 1 + std::max(obtenerAltura(x->izquierda), obtenerAltura(x->derecha));
            return x;
        }
        NodoAVL* rotarIzquierda(NodoAVL* x)
        {
            NodoAVL* y = x->derecha;
            NodoAVL* T2 = y->izquierda;
            y->izquierda = x;
            x->derecha = T2;
            x->altura = 1 + std::max(obtenerAltura(x->izquierda), obtenerAltura(x->derecha));
            y->altura = 1 + std::max(obtenerAltura(y->izquierda), obtenerAltura(y->derecha));
            return y;
        }
        NodoAVL* insertar(NodoAVL* nodo, Producto* producto)
        {
            if (!nodo) return new NodoAVL(producto);
            if (producto->nombre < nodo->producto->nombre)
            {
                nodo->izquierda = insertar(nodo->izquierda, producto);
            }
            else if (producto->nombre > nodo->producto->nombre)
            {
                nodo->derecha = insertar(nodo->derecha, producto);
            }
            else
            {
                return nodo;
            }
            nodo->altura = 1 + std::max(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));
            int balance = obtenerBalance(nodo);
            if (balance > 1 && producto->nombre < nodo->izquierda->producto->nombre)
            {
                return rotarDerecha(nodo);
            }
            if (balance < -1 && producto->nombre > nodo->derecha->producto->nombre)
            {
                return rotarIzquierda(nodo);
            }
            if (balance > 1 && producto->nombre > nodo->izquierda->producto->nombre)
            {        
                nodo->izquierda = rotarIzquierda(nodo->izquierda);
                return rotarDerecha(nodo);
            }
            if (balance < -1 && producto->nombre < nodo->derecha->producto->nombre)
            {
                nodo->derecha = rotarDerecha(nodo->derecha);
                return rotarIzquierda(nodo);
            }
            return nodo;
        }

        Producto* buscar(NodoAVL* nodo, std::string& nombre)
        {
            if (!nodo) return nullptr;
            if (nombre == nodo->producto->nombre)
            {
                return nodo->producto;
            }
            if (nombre < nodo->producto->nombre)
            {
                return buscar(nodo->izquierda, nombre);
            }
            return buscar(nodo->derecha, nombre);
        }

        void eliminarNodos(NodoAVL* nodo)
        {
            if (nodo)
            {
                eliminarNodos(nodo->izquierda);
                eliminarNodos(nodo->derecha);
                delete nodo;
            }
        }

        void generarDot(NodoAVL* nodo, std::ofstream& archivo)
        {
            if (nodo)
            {
                archivo << "  \"" << nodo->producto->nombre << "\" [label=\"Nombre: " << nodo->producto->nombre << "\\nCódigo: " << nodo->producto->codigo_barra << "\"];\n";
                if (nodo->izquierda)
                {
                    archivo << "  \"" << nodo->producto->nombre << "\" -> \"" << nodo->izquierda->producto->nombre << "\";\n";
                    generarDot(nodo->izquierda, archivo);
                }
                if (nodo->derecha)
                {
                    archivo << "  \"" << nodo->producto->nombre << "\" -> \"" << nodo->derecha->producto->nombre << "\";\n";
                    generarDot(nodo->derecha, archivo);
                }
            }
        }

    public:
        ArbolAVL() : raiz(nullptr) {}
        void insertar(Producto* producto)
        {
            raiz = insertar(raiz, producto);
        }

        Producto* buscarPorNombre(std::string nombre)
        {
            return buscar(raiz, nombre);
        }

        void crearGrafico(const std::string& nombreArchivo)
        {
            std::ofstream archivo(nombreArchivo);
            archivo << "digraph G {\n";
            archivo << "  node [shape=record];\n";
            generarDot(raiz, archivo);
            archivo << "}\n";
            archivo.close();
        }

        ~ArbolAVL()
        {
            eliminarNodos(raiz);
        }
    };
#endif