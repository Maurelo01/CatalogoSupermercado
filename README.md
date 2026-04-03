# CatalogoSupermercado
Aplicación de consola en C++ que gestiona un catálogo de productos usando cinco estructuras de datos implementadas desde cero: Lista Enlazada (no ordenada y ordenada), Árbol AVL, Árbol B y Árbol B+. El sistema soporta inserción, búsqueda y eliminación coherente en todas las estructuras, carga desde CSV (+1000 productos), comparación de tiempos de búsqueda y generación de gráficos en formato Graphviz.

# Requisitos
Para compilar y ejecutar este proyecto, asegúrate de contar con lo siguiente en tu sistema:
- Compilador de C++ (`g++`) con soporte para C++17.
- Herramienta `make` o `cmake` (versión mínima sugerida: 3.0).
- Graphviz instalado (`dot`) para generar las imágenes PNG de los árboles.

Para instalar Graphviz en Linux (Ubuntu/Debian):
```
sudo apt install graphviz
```

# Instrucciones de Compilación

Este proyecto ofrece dos métodos para compilar: usando el `Makefile` o mediante `CMake`.

# Opción 1: Compilar usando Makefile

1. Abre una terminal y navega hasta la carpeta raíz del proyecto (donde se encuentra el archivo `Makefile`).
2. Para compilar en Linux:
   ```
   make
   ```
3. Una vez finalizado, ejecuta el programa con:
   ```
   ./supermercado
   ```
4. Para limpiar los archivos compilados:
   ```
   make clean
   ```

# Opción 2: Compilar usando CMake

1. Desde la carpeta raíz del proyecto, crea la carpeta de build y entra en ella:
   ```
   mkdir build && cd build
   ```
2. Genera los archivos de compilación:
   ```
   cmake ..
   ```
3. Compila el proyecto:
   ```
   make
   ```
4. Ejecuta el programa desde la carpeta `build`:
   ```
   ./supermercado
   ```

# Uso del Programa

Al ejecutar el programa se muestra el siguiente menú principal:

```
-----------------------------------------
   SISTEMA DE CATALOGO DE SUPERMERCADO
-----------------------------------------
1)  Cargar catálogo desde CSV
2)  Agregar producto manualmente
3)  Buscar por nombre - Lista Enlazada
4)  Buscar por nombre - Árbol AVL
5)  Buscar por código - Lista Enlazada
6)  Buscar por rango de fechas - Árbol B
7)  Buscar por categoría - Árbol B+
8)  Eliminar producto
9)  Listar productos en orden alfabético (AVL in-order)
10) Comparar búsquedas: Lista vs AVL
11) Ver errores de carga
12) Generar gráfico del Árbol B (.dot y .png)
13) Generar gráfico del Árbol B+ (.dot y .png)
14) Generar gráfico del Árbol AVL (.dot y .png)
15) Salir
```

### Carga desde CSV (opción 1)

El archivo CSV debe seguir el formato:

```
Nombre,CodigoBarra,Categoria,FechaCaducidad,Marca,Precio,Stock
Leche Entera,7500001,Lacteos,2025-10-02,SuperMax,104.48,309
```

El programa valida automáticamente cada línea, omite duplicados y líneas malformadas, y registra todos los errores en el archivo `errors.log`.

Se incluye el archivo `catalogo.csv` con más de 1,000 productos de muestra listos para usar.

### Comparación de búsquedas (opción 10)

El sistema solicita dos parámetros:
- `m`: número de búsquedas por repetición.
- `j`: número de repeticiones.

Mide y compara los tiempos en microsegundos entre la búsqueda secuencial en lista enlazada y la búsqueda binaria en el árbol AVL, tanto para búsquedas exitosas como fallidas.

### Visualización de árboles (opciones 12, 13, 14)

Genera archivos `.dot` y los convierte automáticamente a imágenes `.png` usando Graphviz. Los archivos se crean en el directorio de ejecución:

- `arbolB.dot` / `arbolB.png`
- `arbolBMas.dot` / `arbolBMas.png`
- `arbol_avl.dot` / `arbol_avl.png`

# Estructuras de Datos Implementadas

| Estructura | Clave | Propósito |
| Lista Enlazada (no ordenada) | Código de barra | Registro maestro, búsqueda secuencial, validación de duplicados |
| Lista Enlazada (ordenada) | Nombre | Pruebas comparativas de búsqueda |
| Árbol AVL | Nombre | Búsqueda binaria por nombre, listado alfabético |
| Árbol B | Fecha de caducidad | Consultas por rango de fechas |
| Árbol B+ | Categoría | Agrupación y recorrido eficiente por categoría |

# Autor
Mauricio Joel Gómez Barrios — 202031478
Estructura de Datos — USAC CUNOC — 2026
