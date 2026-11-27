/* ============================================================================
 * GUIA DE EJERCICIOS 17 (PUNTEROS) - EJERCICIO 09:
 * Pila (LIFO) y Cola (FIFO) dinamicas implementadas desde cero con nodos
 * enlazados y punteros dobles (Nodo **).
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct Nodo {
    int dato;
    struct Nodo *sig;
} Nodo;

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

/* --- Operaciones de PILA (LIFO) --- */
static int push(Nodo **tope, int val) {
    Nodo *n = (Nodo *)malloc(sizeof(Nodo));
    if (!n) return 0;
    n->dato = val;
    n->sig = *tope;
    *tope = n;
    return 1;
}

static int pop(Nodo **tope, int *salida) {
    Nodo *tmp;
    if (!*tope) return 0;
    tmp = *tope;
    *salida = tmp->dato;
    *tope = tmp->sig;
    free(tmp);
    return 1;
}

/* --- Operaciones de COLA (FIFO) --- */
static int enqueue(Nodo **frente, Nodo **fin, int val) {
    Nodo *n = (Nodo *)malloc(sizeof(Nodo));
    if (!n) return 0;
    n->dato = val;
    n->sig = NULL;
    if (!*fin) *frente = *fin = n;
    else { (*fin)->sig = n; *fin = n; }
    return 1;
}

static int dequeue(Nodo **frente, Nodo **fin, int *salida) {
    Nodo *tmp;
    if (!*frente) return 0;
    tmp = *frente;
    *salida = tmp->dato;
    *frente = tmp->sig;
    if (!*frente) *fin = NULL;
    free(tmp);
    return 1;
}

static void mostrar_nodos(const Nodo *p, const char *titulo) {
    printf("%s: ", titulo);
    if (!p) printf("(vacia)");
    for (; p; p = p->sig) printf("[%d]%s", p->dato, p->sig ? " -> " : "");
    printf("\n");
}

static void liberar_nodos(Nodo **cabeza) {
    while (*cabeza) { Nodo *t = *cabeza; *cabeza = t->sig; free(t); }
}

int main(void) {
    Nodo *pila = NULL, *cola_ini = NULL, *cola_fin = NULL;
    int op, val;

    for (;;) {
        printf("\n=== PUNTEROS 09: PILA (LIFO) Y COLA (FIFO) DINAMICAS ===\n");
        mostrar_nodos(pila,     "  PILA");
        mostrar_nodos(cola_ini, "  COLA");
        printf("1. Push (apilar)   2. Pop (desapilar)\n");
        printf("3. Enqueue (encolar) 4. Dequeue (desencolar)\n0. Salir\nOpcion: ");
        do { op = _getch(); if (op == 0 || op == 0xE0) { _getch(); op = 0; } } while (op < '0' || op > '4');
        printf("%c\n", op);
        op -= '0';
        if (op == 0) break;

        if (op == 1) {
            val = leer_int("  Valor a apilar: ");
            if (!push(&pila, val)) printf("  [Error] Sin memoria.\n");
        } else if (op == 2) {
            if (pop(&pila, &val)) printf("  -> Desapilado: %d\n", val);
            else printf("  Pila vacia.\n");
        } else if (op == 3) {
            val = leer_int("  Valor a encolar: ");
            if (!enqueue(&cola_ini, &cola_fin, val)) printf("  [Error] Sin memoria.\n");
        } else if (op == 4) {
            if (dequeue(&cola_ini, &cola_fin, &val)) printf("  -> Desencolado: %d\n", val);
            else printf("  Cola vacia.\n");
        }
    }

    liberar_nodos(&pila);
    liberar_nodos(&cola_ini);
    return 0;
}
