    using namespace std;
    #ifndef PRODUCTO_H
    #define PRODUCTO_H
    #include <string>

    struct Producto
    {
        string nombre;
        string codigo_barra;
        string categoria;
        string fecha_caducidad;
        string marca;
        double precio;
        int stock;

        Producto(string n, string cb, string categ, string fc, string m, double p, int s) : nombre(n), codigo_barra(cb), categoria(categ), fecha_caducidad(fc), marca(m), precio(p), stock(s) {}
    };

    #endif