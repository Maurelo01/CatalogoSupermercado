    #ifndef PRODUCTO_H
    #define PRODUCTO_H
    #include <string>

    struct Producto
    {
        std::string nombre;
        std::string codigo_barra;
        std::string categoria;
        std::string fecha_caducidad;
        std::string marca;
        double precio;
        int stock;

        Producto(std::string n, std::string cb, std::string categ, std::string fc, std::string m, double p, int s) : nombre(n), codigo_barra(cb), categoria(categ), fecha_caducidad(fc), marca(m), precio(p), stock(s) {}
    };

    #endif