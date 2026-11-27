/* ============================================================================
 * GUIA DE EJERCICIOS 17 (PUNTEROS) - EJERCICIO 10:
 * Lista doblemente enlazada con punteros (ant / sig): insertar al inicio y al
 * final, eliminar por valor, recorrer hacia adelante y hacia atras, y liberar.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct NodoD {
    int dato;
    struct NodoD *ant;
    struct NodoD *sig;
} NodoD;

static int leer_int(const char *msg) {
    char buf[32]; long v; char *fin;
    for (;;) {
        printf("%s", msg);
        if (!fgets(buf, sizeof buf, stdin)) exit(0);
        v = strtol(buf, &fin, 10);
        if (fin != buf && (*fin == '\n' || *fin == '\r' || *fin == '\0')) return (int)v;
        printf("  Error: entero invalido.\n");
    }
}

static int insertar_inicio(NodoD **ini, NodoD **fin, int val) {
    NodoD *n = (NodoD *)malloc(sizeof(NodoD));
    if (!n) return 0;
    n->dato = val; n->ant = NULL; n->sig = *ini;
    if (*ini) (*ini)->ant = n; else *fin = n;
    *ini = n;
    return 1;
}

static int insertar_final(NodoD **ini, NodoD **fin, int val) {
    NodoD *n = (NodoD *)malloc(sizeof(NodoD));
    if (!n) return 0;
    n->dato = val; n->sig = NULL; n->ant = *fin;
    if (*fin) (*fin)->sig = n; else *ini = n;
    *fin = n;
    return 1;
}

static int eliminar_valor(NodoD **ini, NodoD **fin, int val) {
    NodoD *p = *ini;
    while (p && p->dato != val) p = p->sig;
    if (!p) return 0;
    if (p->ant) p->ant->sig = p->sig; else *ini = p->sig;
    if (p->sig) p->sig->ant = p->ant; else *fin = p->ant;
    free(p);
    return 1;
}

static void mostrar_adelante(const NodoD *ini) {
    const NodoD *p;
    printf("  Adelante: ");
    if (!ini) printf("(vacia)");
    for (p = ini; p; p = p->sig) printf("[%d]%s", p->dato, p->sig ? " <-> " : "");
    printf("\n");
}

static void mostrar_atras(const NodoD *fin) {
    const NodoD *p;
    printf("  Atras   : ");
    if (!fin) printf("(vacia)");
    for (p = fin; p; p = p->ant) printf("[%d]%s", p->dato, p->ant ? " <-> " : "");
    printf("\n");
}

static void liberar_todo(NodoD **ini, NodoD **fin) {
    while (*ini) { NodoD *t = *ini; *ini = t->sig; free(t); }
    *fin = NULL;
}

int main(void) {
    NodoD *ini = NULL, *fin = NULL;
    int op, val;

    for (;;) {
        printf("\n=== PUNTEROS 10: LISTA DOBLEMENTE ENLAZADA ===\n");
        mostrar_adelante(ini);
        mostrar_atras(fin);
        printf("1. Insertar al inicio  2. Insertar al final  3. Eliminar valor  0. Salir\nOpcion: ");
        do { op = _getch(); if (op == 0 || op == 0xE0) { _getch(); op = 0; } } while (op < '0' || op > '3');
        printf("%c\n", op);
        op -= '0';
        if (op == 0) break;

        if (op == 1) {
            val = leer_int("  Valor: ");
            if (!insertar_inicio(&ini, &fin, val)) printf("  [Error] Sin memoria.\n");
        } else if (op == 2) {
            val = leer_int("  Valor: ");
            if (!insertar_final(&ini, &fin, val)) printf("  [Error] Sin memoria.\n");
        } else if (op == 3) {
            val = leer_int("  Valor a eliminar: ");
            printf(eliminar_valor(&ini, &fin, val) ? "  [OK] Eliminado.\n" : "  [Aviso] Valor no encontrado.\n");
        }
    }

    liberar_todo(&ini, &fin);
    return 0;
}
