/* EJERCICIO 11: CONTAR VOCALES
 * Funcionamiento: se revisa un caracter por llamada y se suma 1 si es vocal.
 * Tipos: char[] para texto, char para cada letra e int para contador/posicion.
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int vocales(char s[], int p)
{
    char c;
    if (!s[p]) return 0;
    c = (char)tolower((unsigned char)s[p]);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') + vocales(s, p + 1);
}

int main(void)
{
    char s[100];
    do {
        printf("Ingrese una cadena de texto: ");
        if (!fgets(s, 100, stdin)) return 0;
        s[strcspn(s, "\r\n")] = 0;
    } while (s[0] == '\0');

    printf("Total de vocales: %d\n", vocales(s, 0));
    system("pause");
    return 0;
}
