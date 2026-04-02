#ifndef ArBOLBMAS_H
#define ArBOLBMAS_H

#include "Producto.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

struct ListaCategoria
{
    Producto* producto;
    ListaCategoria* siguiente; 
    ListaCategoria(Producto* producto) : producto(producto), siguiente(nullptr) {}
};

class NodoBMas
{
    public:
        bool esHoja;
        int numClaves;
        int grado;
        string* claves;
        NodoBMas** hijos;
        ListaCategoria** listas;
        NodoBMas* siguiente;
        NodoBMas* padre;

        NodoBMas(int nGrado, bool nEsHoja)
        {
            grado = nGrado;
            esHoja = nEsHoja;
            numClaves = 0;
            siguiente = nullptr;
            padre = nullptr;
            claves = new string[2 * grado];
            hijos = new NodoBMas*[2 * grado + 1];
            listas = new ListaCategoria*[2 * grado];
            for (int i = 0; i <= 2 * grado; i++)
            {
                hijos[i] = nullptr;
            }
            for (int i = 0; i < 2 * grado; i++)
            {
                listas[i] = nullptr;
            }
        }

        void agregarProductoAClave(int indice, Producto* producto)
        {
            ListaCategoria* nuevo = new ListaCategoria(producto);
            nuevo->siguiente = listas[indice];
            listas[indice] = nuevo;
        }
};

struct ParNodoId
{
    NodoBMas* nodo;
    int id;
};

class ArbolBMas
{
    private:
        NodoBMas* raiz;
        int grado;
        
        NodoBMas* buscarHoja(NodoBMas* nodo, const string& categoria)
        {
            if (nodo->esHoja)
            {
                return nodo;
            }
            int i = 0;
            while (i < nodo->numClaves && categoria >= nodo->claves[i])
            {
                i++;
            }
            if (!nodo->hijos[i])
            {
                return nodo; 
            }

            return buscarHoja(nodo->hijos[i], categoria);
        }

        void dividirNodo(NodoBMas* nodo)
        {
            if (nodo->esHoja)
            {
                NodoBMas* nuevoNodo = new NodoBMas(grado, true);
                int mitad = grado;
                nuevoNodo->numClaves = nodo->numClaves - mitad;
                for (int i = 0; i < nuevoNodo->numClaves; i++)
                {
                    nuevoNodo->claves[i] = nodo->claves[mitad + i];
                    nuevoNodo->listas[i] = nodo->listas[mitad + i];
                }
                nodo->numClaves = mitad;
                nuevoNodo->siguiente = nodo->siguiente;
                nodo->siguiente = nuevoNodo;
                insertarEnPadre(nodo, nuevoNodo->claves[0], nuevoNodo);
            }
            else
            {
                NodoBMas* nuevoNodo = new NodoBMas(grado, false);
                int mitad = grado;
                string clavePromedio = nodo->claves[mitad];
                nuevoNodo->numClaves = nodo->numClaves - mitad - 1;
                for (int i = 0; i < nuevoNodo->numClaves; i++)
                {
                    nuevoNodo->claves[i] = nodo->claves[mitad + 1 + i];
                }
                for (int i = 0; i <= nuevoNodo->numClaves; i++)
                {
                    nuevoNodo->hijos[i] = nodo->hijos[mitad + 1 + i];
                    if (nuevoNodo->hijos[i])
                    {
                        nuevoNodo->hijos[i]->padre = nuevoNodo;
                    }
                }
                nodo->numClaves = mitad;
                insertarEnPadre(nodo, nodo->claves[mitad], nuevoNodo);
            }
        }

        void insertarEnPadre(NodoBMas* izq, const string& clave, NodoBMas* der)
        {
            if (izq == raiz)
            {
                NodoBMas* nuevaRaiz = new NodoBMas(grado, false);
                nuevaRaiz->claves[0] = clave;
                nuevaRaiz->hijos[0] = izq;
                nuevaRaiz->hijos[1] = der;
                nuevaRaiz->numClaves = 1;
                izq->padre = nuevaRaiz;
                der->padre = nuevaRaiz;
                raiz = nuevaRaiz;
                return;
            }
            NodoBMas* padre = izq->padre;
            int i = padre->numClaves - 1;
            while (i >= 0 && padre->claves[i] > clave)
            {
                padre->claves[i + 1] = padre->claves[i];
                padre->hijos[i + 2] = padre->hijos[i + 1];
                i--;
            }
            padre->claves[i + 1] = clave;
            padre->hijos[i + 2] = der;
            padre->numClaves++;
            der->padre = padre;
            if (padre->numClaves == 2 * grado)
            {
                dividirNodo(padre);
            }
        }

        void generarDot(NodoBMas* nodo, ofstream& archivo, int& contador, int idActual, ParNodoId* hojas, int& numHojas)
        {
            archivo << "  node" << idActual << " [label=<<TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\"><TR>";
            for (int i = 0; i < nodo->numClaves; i++)
            {
                archivo << "<TD>" << nodo->claves[i] << "</TD>";
            }
            archivo << "</TR></TABLE>>];\n";
            if (!nodo->esHoja)
            {
                for (int i = 0; i <= nodo->numClaves; i++)
                {
                    if (nodo->hijos[i])
                    {
                        int idHijo = ++contador;
                        archivo << "  node" << idActual << " -> node" << idHijo << ";\n";
                        generarDot(nodo->hijos[i], archivo, contador, idHijo, hojas, numHojas);
                    }
                }
            }
            else
            {
                hojas[numHojas++] = {nodo, idActual};
            }
        }

        void eliminar(NodoBMas* nodo)
        {
            if (!nodo) return;
            if (!nodo->esHoja)
            {
                for (int i = 0; i <= nodo->numClaves; i++)
                {
                    eliminar(nodo->hijos[i]);
                }
            }
            else
            {
                for (int i = 0; i < nodo->numClaves; i++)
                {
                    ListaCategoria* actual = nodo->listas[i];
                    while (actual)
                    {
                        ListaCategoria* temp = actual;
                        actual = actual->siguiente;
                        delete temp;
                    }
                }
            }
            delete[] nodo->claves;
            delete[] nodo->hijos;
            delete[] nodo->listas;
            delete nodo;
        }

        public:
            ArbolBMas(int g = 3) : raiz(nullptr), grado(g) {}

            void insertar(Producto* producto)
            {
                string categoria = producto->categoria;
                if (!raiz)
                {
                    raiz = new NodoBMas(grado, true);
                    raiz->claves[0] = categoria;
                    raiz->agregarProductoAClave(0, producto);
                    raiz->numClaves = 1;
                    return;
                }
                NodoBMas* hoja = buscarHoja(raiz, categoria);
                for (int i = 0; i < hoja->numClaves; i++)
                {
                    if (hoja->claves[i] == categoria)
                    {
                        hoja->agregarProductoAClave(i, producto);
                        return;
                    }
                }
                int i = hoja->numClaves - 1;
                while (i >= 0 && hoja->claves[i] > categoria)
                {
                    hoja->claves[i + 1] = hoja->claves[i];
                    hoja->listas[i + 1] = hoja->listas[i];
                    i--;
                }
                hoja->claves[i + 1] = categoria;
                hoja->listas[i + 1] = new ListaCategoria(producto);
                hoja->numClaves++;
                if (hoja->numClaves == 2 * grado)
                {
                    dividirNodo(hoja);
                }
            }

            void buscarPorCategoria(string categoria)
            {
                if(!raiz)
                {
                    return;
                }
                NodoBMas* hoja = buscarHoja(raiz, categoria);
                for (int i = 0; i < hoja->numClaves; i++)
                {
                    if (hoja->claves[i] == categoria)
                    {
                        cout << " PRODUCTOS EN LA CATEGORIA " << categoria << ":\n";
                        ListaCategoria* actual = hoja->listas[i];
                        while (actual)
                        {
                            cout << " - " << actual->producto->nombre << " (Código de barra: " << actual->producto->codigo_barra << ", Precio: Q" << actual->producto->precio << ")\n";
                            actual = actual->siguiente;
                        }
                        return;
                    }
                }
                cout << "No se encontraron productos en la categoría " << categoria << ".\n";
            }

            void crearGrafico(const string& nombreArchivo)
            {
                ofstream archivo(nombreArchivo);
                archivo << "digraph ArbolBMas {\n";
                archivo << "  rankdir=TB;\n";
                archivo << "  node [shape=plaintext];\n";
                if (raiz)
                {
                    int contador = 0;
                    ParNodoId hojas[1000];
                    int numHojas = 0;
                    generarDot(raiz, archivo, contador, contador, hojas, numHojas);
                    NodoBMas* hoja = raiz;
                    while (!hoja->esHoja) hoja = hoja->hijos[0];
                    while (hoja && hoja->siguiente)
                    {
                        int idActual = -1, idSig = -1;
                        for (int i = 0; i < numHojas; i++)
                        {
                            if (hojas[i].nodo == hoja)            idActual = hojas[i].id;
                            if (hojas[i].nodo == hoja->siguiente) idSig    = hojas[i].id;
                        }
                        if (idActual != -1 && idSig != -1)
                        {
                            archivo << "  node" << idActual << " -> node" << idSig << " [color=red, constraint=false];\n";
                        }
                        hoja = hoja->siguiente;
                    }
                }
                archivo << "}\n";
                archivo.close();
                cout << "Gráfico del árbol B+ generado correctamente: " << nombreArchivo << endl;
            }

            void eliminarProducto(const string& categoria, const string& codigoBarra)
            {
                if (!raiz) return;
                NodoBMas* hoja = buscarHoja(raiz, categoria);
                for (int i = 0; i < hoja->numClaves; i++)
                {
                    if (hoja->claves[i] == categoria)
                    {
                        ListaCategoria* actual = hoja->listas[i];
                        ListaCategoria* anterior = nullptr;
                        while (actual)
                        {
                            if (actual->producto->codigo_barra == codigoBarra)
                            {
                                if (anterior) anterior->siguiente = actual->siguiente;
                                else hoja->listas[i] = actual->siguiente;
                                delete actual;
                                return;
                            }
                            anterior = actual;
                            actual = actual->siguiente;
                        }
                    }
                }
            }

            ~ArbolBMas()
            {
                eliminar(raiz);
            }
};
#endif