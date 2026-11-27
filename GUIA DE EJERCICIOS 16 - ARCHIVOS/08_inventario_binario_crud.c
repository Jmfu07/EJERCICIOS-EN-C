/* ============================================================================
 * GUIA DE EJERCICIOS 16 (ARCHIVOS) - EJERCICIO 08:
 * Sistema de inventario con ARCHIVOS BINARIOS (fwrite, fread, fseek, ftell):
 * alta, listado, busqueda por codigo, modificacion directa in-place y baja.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define ARCH_BIN "inventario.dat"
#define ARCH_TMP "inventario_tmp.dat"

typedef struct {
    int codigo;
    char nombre[40];
    int stock;
    double precio;
} Producto;

static int leer_int_rango(const char *msg, int min, int max) {
    char buf[32]; long v; char *fin;
    for (;;) {
        printf("%s [%d a %d]: ", msg, min, max);
        if (!fgets(buf, sizeof buf, stdin)) exit(0);
        v = strtol(buf, &fin, 10);
        if (fin != buf && (*fin == '\n' || *fin == '\r' || *fin == '\0') && v >= min && v <= max)
            return (int)v;
        printf("  Error: valor invalido.\n");
    }
}

static double leer_double_pos(const char *msg) {
    char buf[32]; double v; char *fin;
    for (;;) {
        printf("%s: ", msg);
        if (!fgets(buf, sizeof buf, stdin)) exit(0);
        v = strtod(buf, &fin);
        if (fin != buf && (*fin == '\n' || *fin == '\r' || *fin == '\0') && v > 0.0) return v;
        printf("  Error: ingrese un precio mayor a 0.\n");
    }
}

static void agregar_producto(void) {
    Producto p;
    FILE *f = fopen(ARCH_BIN, "ab");
    if (!f) { printf("  [Error] No se pudo abrir '%s'.\n", ARCH_BIN); return; }
    p.codigo = leer_int_rango("  Codigo", 1, 99999);
    printf("  Nombre del producto: ");
    if (!fgets(p.nombre, sizeof p.nombre, stdin)) { fclose(f); return; }
    p.nombre[strcspn(p.nombre, "\r\n")] = '\0';
    if (p.nombre[0] == '\0') strcpy(p.nombre, "SinNombre");
    p.stock  = leer_int_rango("  Stock inicial", 0, 100000);
    p.precio = leer_double_pos("  Precio unitario ($)");
    fwrite(&p, sizeof p, 1, f);
    fclose(f);
    printf("  [OK] Producto guardado en binario.\n");
}

static void listar_productos(void) {
    FILE *f = fopen(ARCH_BIN, "rb");
    Producto p;
    long total_bytes;
    int n = 0;
    if (!f) { printf("  (Inventario vacio)\n"); return; }
    fseek(f, 0, SEEK_END);
    total_bytes = ftell(f);
    rewind(f);
    printf("\n%-6s | %-28s | %-6s | %-10s\n", "COD", "PRODUCTO", "STOCK", "PRECIO");
    printf("-------+------------------------------+--------+-----------\n");
    while (fread(&p, sizeof p, 1, f) == 1) {
        printf("%-6d | %-28s | %-6d | $%9.2f\n", p.codigo, p.nombre, p.stock, p.precio);
        n++;
    }
    printf("Total registros: %d (%ld bytes)\n", n, total_bytes);
    fclose(f);
}

static void modificar_producto(void) {
    FILE *f = fopen(ARCH_BIN, "r+b");
    Producto p;
    int cod, hallado = 0;
    if (!f) { printf("  (Inventario vacio)\n"); return; }
    cod = leer_int_rango("  Codigo a modificar", 1, 99999);
    while (fread(&p, sizeof p, 1, f) == 1) {
        if (p.codigo == cod) {
            printf("  Actual: %s | Stock: %d | Precio: $%.2f\n", p.nombre, p.stock, p.precio);
            p.stock  = leer_int_rango("  Nuevo stock", 0, 100000);
            p.precio = leer_double_pos("  Nuevo precio ($)");
            fseek(f, -(long)sizeof p, SEEK_CUR);
            fwrite(&p, sizeof p, 1, f);
            hallado = 1;
            break;
        }
    }
    fclose(f);
    printf(hallado ? "  [OK] Producto actualizado in-place con fseek.\n" : "  [Aviso] Codigo no encontrado.\n");
}

static void eliminar_producto(void) {
    FILE *fin = fopen(ARCH_BIN, "rb"), *fout;
    Producto p;
    int cod, hallado = 0;
    if (!fin) { printf("  (Inventario vacio)\n"); return; }
    fout = fopen(ARCH_TMP, "wb");
    if (!fout) { fclose(fin); return; }
    cod = leer_int_rango("  Codigo a eliminar", 1, 99999);
    while (fread(&p, sizeof p, 1, fin) == 1) {
        if (p.codigo == cod) { hallado = 1; continue; }
        fwrite(&p, sizeof p, 1, fout);
    }
    fclose(fin); fclose(fout);
    remove(ARCH_BIN); rename(ARCH_TMP, ARCH_BIN);
    printf(hallado ? "  [OK] Producto eliminado.\n" : "  [Aviso] Codigo no encontrado.\n");
}

int main(void) {
    int op;
    for (;;) {
        printf("\n=== ARCHIVOS 08: INVENTARIO BINARIO CRUD (%s) ===\n", ARCH_BIN);
        printf("1. Agregar producto\n2. Listar inventario\n3. Modificar (fseek)\n4. Eliminar\n0. Salir\nOpcion: ");
        do { op = _getch(); if (op == 0 || op == 0xE0) { _getch(); op = 0; } } while (op < '0' || op > '4');
        printf("%c\n", op);
        op -= '0';
        if (op == 0) break;
        if (op == 1) agregar_producto();
        else if (op == 2) listar_productos();
        else if (op == 3) modificar_producto();
        else if (op == 4) eliminar_producto();
    }
    return 0;
}
