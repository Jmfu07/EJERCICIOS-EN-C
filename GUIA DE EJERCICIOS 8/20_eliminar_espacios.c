/* EJERCICIO 20: ELIMINAR ESPACIOS
 * Funcionamiento: cada caracter no blanco se copia al destino; los espacios solo se saltan.
 * Tipos: char[] para cadenas e int para las posiciones de lectura/escritura.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quitar(char o[], char d[], int i, int j)
{
    if (!o[i]) {
        d[j] = 0;
        return;
    }
    if (o[i] != ' ' && o[i] != '\t' && o[i] != '\r' && o[i] != '\n') {
        d[j] = o[i];
        quitar(o, d, i + 1, j + 1);
    } else {
        quitar(o, d, i + 1, j);
    }
}

int main(void)
{
    char o[100], d[100];
    do {
        printf("Ingrese una cadena con espacios: ");
        if (!fgets(o, 100, stdin)) return 0;
        o[strcspn(o, "\r\n")] = 0;
    } while (o[0] == '\0');

    quitar(o, d, 0, 0);
    printf("Cadena sin espacios: %s\n", d);
    system("pause");
    return 0;
}
