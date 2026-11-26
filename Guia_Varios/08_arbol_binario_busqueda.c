/*
 * ============================================================================
 * GUIA VARIOS - EJERCICIO 8: Arbol Binario de Busqueda (ABB)
 * ============================================================================
 * Algoritmo: Implementa un Arbol Binario de Busqueda (ABB) dinamico utilizando
 *            recursividad y punteros para todas las operaciones:
 *              1. Insertar nodos recursivamente respetando la propiedad ABB
 *                 (menores a la izquierda, mayores a la derecha).
 *              2. Buscar recursivamente un valor en el arbol indicando la
 *                 profundidad a la que fue hallado.
 *              3. Recorrer el arbol recursivamente en:
 *                 - Inorden   (Izquierda - Raiz - Derecha, orden ascendente)
 *                 - Preorden  (Raiz - Izquierda - Derecha)
 *                 - Postorden (Izquierda - Derecha - Raiz)
 *              4. Eliminar un nodo y liberar el arbol recursivamente.
 * Funciones:
 *   - insertar_abb_rec(), buscar_abb_rec(), eliminar_abb_rec()
 *   - inorden_rec(), preorden_rec(), postorden_rec(), liberar_abb_rec()
 * Punteros y Recursividad:
 *   - Estructura recursiva NodoABB con punteros izq y der.
 * Tipos de datos: struct NodoABB, int.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct NodoABB {
    int dato;
    struct NodoABB *izq;
    struct NodoABB *der;
} NodoABB;

void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leer_entero(const char *prompt, int min, int max) {
    int valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &valor) != 1) {
            printf("  [Error] Ingrese un numero entero valido.\n");
            limpiar_buffer();
        } else if (valor < min || valor > max) {
            printf("  [Error] Rango permitido: [%d, %d].\n", min, max);
            limpiar_buffer();
        } else {
            limpiar_buffer();
            return valor;
        }
    }
}

NodoABB *crear_nodo_abb(int valor) {
    NodoABB *n = (NodoABB *)malloc(sizeof(NodoABB));
    if (n) {
        n->dato = valor;
        n->izq = NULL;
        n->der = NULL;
    }
    return n;
}

NodoABB *insertar_abb_rec(NodoABB *raiz, int valor, int *insertado) {
    if (raiz == NULL) {
        *insertado = 1;
        return crear_nodo_abb(valor);
    }
    if (valor < raiz->dato) {
        raiz->izq = insertar_abb_rec(raiz->izq, valor, insertado);
    } else if (valor > raiz->dato) {
        raiz->der = insertar_abb_rec(raiz->der, valor, insertado);
    } else {
        *insertado = 0;
    }
    return raiz;
}

const NodoABB *buscar_abb_rec(const NodoABB *raiz, int valor, int *nivel) {
    if (raiz == NULL) {
        return NULL;
    }
    if (valor == raiz->dato) {
        return raiz;
    }
    (*nivel)++;
    if (valor < raiz->dato) {
        return buscar_abb_rec(raiz->izq, valor, nivel);
    }
    return buscar_abb_rec(raiz->der, valor, nivel);
}

NodoABB *minimo_abb(NodoABB *nodo) {
    NodoABB *actual = nodo;
    while (actual && actual->izq != NULL) {
        actual = actual->izq;
    }
    return actual;
}

NodoABB *eliminar_abb_rec(NodoABB *raiz, int valor, int *eliminado) {
    if (raiz == NULL) return NULL;

    if (valor < raiz->dato) {
        raiz->izq = eliminar_abb_rec(raiz->izq, valor, eliminado);
    } else if (valor > raiz->dato) {
        raiz->der = eliminar_abb_rec(raiz->der, valor, eliminado);
    } else {
        *eliminado = 1;
        if (raiz->izq == NULL) {
            NodoABB *temp = raiz->der;
            free(raiz);
            return temp;
        } else if (raiz->der == NULL) {
            NodoABB *temp = raiz->izq;
            free(raiz);
            return temp;
        }
        NodoABB *sucesor = minimo_abb(raiz->der);
        raiz->dato = sucesor->dato;
        int dummy = 0;
        raiz->der = eliminar_abb_rec(raiz->der, sucesor->dato, &dummy);
    }
    return raiz;
}

void inorden_rec(const NodoABB *raiz) {
    if (!raiz) return;
    inorden_rec(raiz->izq);
    printf("%d ", raiz->dato);
    inorden_rec(raiz->der);
}

void preorden_rec(const NodoABB *raiz) {
    if (!raiz) return;
    printf("%d ", raiz->dato);
    preorden_rec(raiz->izq);
    preorden_rec(raiz->der);
}

void postorden_rec(const NodoABB *raiz) {
    if (!raiz) return;
    postorden_rec(raiz->izq);
    postorden_rec(raiz->der);
    printf("%d ", raiz->dato);
}

void liberar_abb_rec(NodoABB *raiz) {
    if (!raiz) return;
    liberar_abb_rec(raiz->izq);
    liberar_abb_rec(raiz->der);
    free(raiz);
}

int main(void) {
    NodoABB *raiz = NULL;

    printf("=================================================================\n");
    printf("   ARBOL BINARIO DE BUSQUEDA (ABB) RECURSIVO\n");
    printf("=================================================================\n");

    int opcion = 0;
    do {
        printf("\n------------------ MENU ARBOL BINARIO ------------------\n");
        printf("  1. Insertar nodo en el arbol\n");
        printf("  2. Buscar un valor en el arbol (Recursivo)\n");
        printf("  3. Mostrar recorridos (Inorden, Preorden, Postorden)\n");
        printf("  4. Eliminar un nodo del arbol\n");
        printf("  5. Salir\n");
        opcion = leer_entero("Seleccione una opcion (1 - 5): ", 1, 5);

        if (opcion == 1) {
            int v = leer_entero("  Ingrese valor entero a insertar (-100000 a 100000): ", -100000, 100000);
            int ok = 0;
            raiz = insertar_abb_rec(raiz, v, &ok);
            if (ok) {
                printf("  [OK] Valor %d insertado en el ABB.\n", v);
            } else {
                printf("  [Aviso] El valor %d ya existe en el ABB.\n", v);
            }
        } else if (opcion == 2) {
            if (!raiz) {
                printf("  [Aviso] El arbol esta vacio.\n");
            } else {
                int v = leer_entero("  Ingrese valor a buscar: ", -100000, 100000);
                int nivel = 0;
                const NodoABB *hallado = buscar_abb_rec(raiz, v, &nivel);
                if (hallado) {
                    printf("  [ENCONTRADO] El nodo [%d] existe en el nivel/profundidad %d.\n",
                           hallado->dato, nivel);
                } else {
                    printf("  [No encontrado] El valor %d no pertenece al arbol.\n", v);
                }
            }
        } else if (opcion == 3) {
            if (!raiz) {
                printf("  [Aviso] El arbol esta vacio.\n");
            } else {
                printf("\n  * Recorrido INORDEN   (Izq-Raiz-Der): ");
                inorden_rec(raiz);
                printf("\n  * Recorrido PREORDEN  (Raiz-Izq-Der): ");
                preorden_rec(raiz);
                printf("\n  * Recorrido POSTORDEN (Izq-Der-Raiz): ");
                postorden_rec(raiz);
                printf("\n");
            }
        } else if (opcion == 4) {
            if (!raiz) {
                printf("  [Aviso] El arbol esta vacio.\n");
            } else {
                int v = leer_entero("  Ingrese valor a eliminar: ", -100000, 100000);
                int elim = 0;
                raiz = eliminar_abb_rec(raiz, v, &elim);
                if (elim) {
                    printf("  [OK] Nodo %d eliminado del arbol.\n", v);
                } else {
                    printf("  [No encontrado] El valor %d no existe en el arbol.\n", v);
                }
            }
        }
    } while (opcion != 5);

    liberar_abb_rec(raiz);
    printf("\nMemoria del arbol liberada. Programa finalizado.\n");
    system("pause");
    return 0;
}
