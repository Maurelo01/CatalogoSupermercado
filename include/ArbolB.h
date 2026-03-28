#ifndef ARBOLB_H
#define ARBOLB_H

#include "Producto.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

struct ListaFechaIgual
{
    Producto* producto;
    ListaFechaIgual* siguiente; 
    ListaFechaIgual(Producto* p) : producto(p), siguiente(nullptr) {}
};

struct ElementoArbolB
{
    std::string fecha;
    ListaFechaIgual* lista;
    ElementoArbolB() : fecha(""), lista(nullptr) {}
    void agregarProducto(Producto* producto)
    {
        ListaFechaIgual* nuevo = new ListaFechaIgual(producto);
        nuevo->siguiente = lista;
        lista = nuevo;
    }
};

class NodoB
{
    public:
        ElementoArbolB* claves;
        int grado;
        NodoB** hijos;
        int numClaves;
        bool esHoja;
        NodoB(int nGrado, bool nEsHoja)
        {
            grado = nGrado;
            esHoja = nEsHoja;
            claves = new ElementoArbolB[2 * grado - 1];
            hijos = new NodoB*[2 * grado];
            numClaves = 0;
            for (int i = 0; i < 2 * grado; i++)
            {
                hijos[i] = nullptr;
            }
        }

        void insertarNoLleno(Producto* producto, const std::string& fecha)
        {
            int i = numClaves - 1;
            if (esHoja)
            {
                while (i >= 0 && claves[i].fecha > fecha)
                {
                    claves[i + 1] = claves[i];
                    i--;
                }
                claves[i + 1].fecha = fecha;
                claves[i + 1].lista = new ListaFechaIgual(producto);
                numClaves++;
            }
            else
            {
                while (i >= 0 && claves[i].fecha > fecha)
                {
                    i--;
                }
                if (hijos[i + 1]->numClaves == 2 * grado - 1)
                {
                    dividirHijo(i + 1, hijos[i + 1]);
                    if (claves[i + 1].fecha < fecha)
                    {
                        i++;
                    }
                }
                hijos[i+1]->insertarNoLleno(producto, fecha);
            }
        }

        void dividirHijo(int i, NodoB* y)
        {
            NodoB* z = new NodoB(y->grado, y->esHoja);
            z->numClaves = grado - 1;
            for (int j = 0; j < grado - 1; j++)
            {
                z->claves[j] = y->claves[j + grado];
                y->claves[j + grado].fecha = "";
                y->claves[j + grado].lista = nullptr; 
            }
            if (!y->esHoja)
            {
                for (int j = 0; j < grado; j++)
                {
                    z->hijos[j] = y->hijos[j + grado];
                }
            }
            y->numClaves = grado - 1;
            for (int j = numClaves; j >= i + 1; j--)
            {
                hijos[j + 1] = hijos[j];
            }
            hijos[i + 1] = z;
            for (int j = numClaves - 1; j >= i; j--)
            {
                claves[j + 1] = claves[j];
            }
            claves[i] = y->claves[grado - 1];
            y->claves[grado - 1].fecha = "";
            y->claves[grado - 1].lista = nullptr;
            numClaves++;
        }
};

class ArbolB
{
    private:
        NodoB* raiz;
        int grado;

        NodoB* buscar(NodoB* nodo, const std::string& fecha)
        {
            if (!nodo)
            {
                return nullptr;
            }
            int i = 0;
            while (i < nodo->numClaves && fecha > nodo->claves[i].fecha)
            {
                i++;
            }
            if (i < nodo->numClaves && fecha == nodo->claves[i].fecha)
            {
                return nodo;
            }
            if (nodo->esHoja)
            {
                return nullptr;
            }
            return buscar(nodo->hijos[i], fecha);
        }

        void buscarEnRango(NodoB* nodo, const std::string& fechaInicio, const std::string& fechaFin)
        {
            if (!nodo)
            {
                return;
            }
            int i = 0;
            while (i < nodo->numClaves && nodo->claves[i].fecha < fechaInicio)
            {
                i++;
            }
            if (!nodo->esHoja)
            {    
                buscarEnRango(nodo->hijos[i], fechaInicio, fechaFin);
            }
            while (i < nodo->numClaves && nodo->claves[i].fecha <= fechaFin)
            {
                ListaFechaIgual* actual = nodo->claves[i].lista;
                while (actual)
                {
                    cout << " - [" << actual->producto->fecha_caducidad << "] " << actual->producto->nombre << " (Código: " << actual->producto->codigo_barra << ")\n";
                    actual = actual->siguiente;
                }
                i++;
                if (!nodo->esHoja)
                {
                    buscarEnRango(nodo->hijos[i], fechaInicio, fechaFin);
                }
            }
        }

        void generarDot(NodoB* nodo, std::ofstream& archivo, int& contador, int idActual)
        {
            archivo << "  node" << idActual << " [label=\"";
            for (int i = 0; i < nodo->numClaves; i++)
            {
                archivo << nodo->claves[i].fecha;
                if (i < nodo->numClaves - 1)
                {
                    archivo << " | ";  
                }
            }
            archivo << "\"];\n";
            if (!nodo->esHoja)
            {
                for (int i = 0; i <= nodo->numClaves; i++)
                {
                    int idHijo = ++contador;
                    archivo << "  node" << idActual << " -> node" << idHijo << ";\n";
                    generarDot(nodo->hijos[i], archivo, contador, idHijo);
                }
            }
        }
    public:
        ArbolB(int g = 3) : raiz(nullptr), grado(g) {}

        void insertar(Producto* producto)
        {
            std::string fecha = producto->fecha_caducidad;
            NodoB* existente = buscar(raiz, fecha);
            if (existente)
            {
                for (int i = 0; i < existente->numClaves; i++)
                {
                    if (existente->claves[i].fecha == fecha)
                    {
                        existente->claves[i].agregarProducto(producto);
                        return;
                    }
                }
            }

            if (!raiz)
            {
                raiz = new NodoB(grado, true);
                raiz->claves[0].fecha = fecha;
                raiz->claves[0].lista = new ListaFechaIgual(producto);
                raiz->numClaves = 1;
            }
            else
            {
                if (raiz->numClaves == 2 * grado - 1)
                {
                    NodoB* nuevaRaiz = new NodoB(grado, false);
                    nuevaRaiz->hijos[0] = raiz;
                    nuevaRaiz->dividirHijo(0, raiz);
                    int i = 0;
                    if (nuevaRaiz->claves[0].fecha < fecha)
                    {
                        i++;
                    }
                    nuevaRaiz->hijos[i]->insertarNoLleno(producto, fecha);
                    raiz = nuevaRaiz;
                }
                else
                {
                    raiz->insertarNoLleno(producto, fecha);
                }
            }
        }
        
        void imprimirEnRango(const std::string& fechaInicio, const std::string& fechaFin)
        {
            cout << "Productos con fecha de caducidad entre " << fechaInicio << " y " << fechaFin << ":\n";
            buscarEnRango(raiz, fechaInicio, fechaFin);
        }

        void crearGrafico(const std::string& nombreArchivo)
        {
            std::ofstream archivo(nombreArchivo);
            archivo << "digraph ArbolB {\n";
            archivo << "  node [shape=record];\n";
            if (raiz)
            {
                int contador = 0;
                generarDot(raiz, archivo, contador, contador);
            }
            archivo << "}\n";
            archivo.close();
            cout << "Gráfico de arbol B generado correctamente: " << nombreArchivo << endl;
        }
};

#endif


