/* EJERCICIO 4: INVERSION DE CADENA
 * Funcionamiento: se intercambian el primer y ultimo caracter y se repite hacia el centro.
 * Tipos: char[] almacena la cadena; int representa posiciones.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void invertir(char s[], int i, int j)
{
    char t;
    if (i >= j) return;
    t = s[i];
    s[i] = s[j];
    s[j] = t;
    invertir(s, i + 1, j - 1);
}

int main(void)
{
    char s[100], original[100];
    do {
        printf("Ingrese una cadena (no vacia): ");
        if (!fgets(s, sizeof(s), stdin)) return 0;
        s[strcspn(s, "\r\n")] = '\0';
    } while (s[0] == '\0');

    strcpy(original, s);
    invertir(s, 0, (int)strlen(s) - 1);
    printf("Cadena original:  %s\n", original);
    printf("Cadena invertida: %s\n", s);
    system("pause");
    return 0;
}
