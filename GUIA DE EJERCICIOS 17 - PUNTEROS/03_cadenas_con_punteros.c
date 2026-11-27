/* ============================================================================
 * GUIA DE EJERCICIOS 17 (PUNTEROS) - EJERCICIO 03:
 * Manipulacion de cadenas usando exclusivamente punteros (char *):
 * mi_strlen, mi_strcpy, mi_strcat, mi_strcmp, mi_strrev y conteo de vocales.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static size_t mi_strlen(const char *s) {
    const char *p = s;
    while (*p != '\0') p++;
    return (size_t)(p - s);
}

static char *mi_strcpy(char *dst, const char *src) {
    char *inicio = dst;
    while ((*dst++ = *src++) != '\0') {}
    return inicio;
}

static char *mi_strcat(char *dst, const char *src) {
    char *p = dst;
    while (*p != '\0') p++;
    while ((*p++ = *src++) != '\0') {}
    return dst;
}

static int mi_strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

static void mi_strrev(char *s) {
    char *izq = s, *der = s + mi_strlen(s);
    if (der == izq) return;
    der--;
    while (izq < der) {
        char t = *izq; *izq = *der; *der = t;
        izq++; der--;
    }
}

static void contar_vc(const char *s, int *vocales, int *consonantes) {
    *vocales = 0; *consonantes = 0;
    for (; *s != '\0'; s++) {
        int c = tolower((unsigned char)*s);
        if (c >= 'a' && c <= 'z') {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') (*vocales)++;
            else (*consonantes)++;
        }
    }
}

static void leer_texto(const char *msg, char *buf, int tam) {
    char *p;
    printf("%s", msg);
    if (!fgets(buf, tam, stdin)) exit(0);
    for (p = buf; *p; p++)
        if (*p == '\r' || *p == '\n') { *p = '\0'; break; }
}

int main(void) {
    char s1[80], s2[80], copia[160];
    int voc, cons, cmp;

    printf("=== PUNTEROS 03: CADENAS CON CHAR * ===\n\n");
    leer_texto("Ingrese cadena 1: ", s1, (int)sizeof s1);
    leer_texto("Ingrese cadena 2: ", s2, (int)sizeof s2);

    printf("\nLongitudes -> s1: %u | s2: %u\n", (unsigned)mi_strlen(s1), (unsigned)mi_strlen(s2));

    cmp = mi_strcmp(s1, s2);
    printf("Comparacion (mi_strcmp): %s\n", cmp == 0 ? "IGUALES" : (cmp < 0 ? "s1 < s2" : "s1 > s2"));

    mi_strcpy(copia, s1);
    mi_strcat(copia, " ");
    mi_strcat(copia, s2);
    printf("Concatenada (mi_strcpy + mi_strcat): \"%s\"\n", copia);

    contar_vc(copia, &voc, &cons);
    printf("Vocales: %d | Consonantes: %d\n", voc, cons);

    mi_strrev(copia);
    printf("Invertida (mi_strrev): \"%s\"\n\n", copia);

    system("pause");
    return 0;
}
