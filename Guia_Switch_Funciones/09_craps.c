/* EJERCICIO 9: CRAPS
 * Funcionamiento: se lanzan dos dados. 7/11 gana, 2/3/12 pierde; otra suma se vuelve punto.
 * Tipos: int para dados y sumas; time_t para inicializar aleatoriedad.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    int a, b, s, p;
    srand((unsigned)time(NULL));
    a = rand() % 6 + 1;
    b = rand() % 6 + 1;
    s = a + b;
    printf("Primera tirada: %d + %d = %d\n", a, b, s);
    if (s == 7 || s == 11) {
        puts("Gana el jugador en la primera tirada!");
        system("pause");
        return 0;
    }
    if (s == 2 || s == 3 || s == 12) {
        puts("Craps! Pierde el jugador (gana la casa).");
        system("pause");
        return 0;
    }
    p = s;
    printf("El punto es: %d\n", p);
    do {
        a = rand() % 6 + 1;
        b = rand() % 6 + 1;
        s = a + b;
        printf("Siguiente tirada: %d + %d = %d\n", a, b, s);
    } while (s != p && s != 7);

    puts(s == p ? "Hizo su punto! Gana el jugador." : "Salio 7! Pierde el jugador (gana la casa).");
    system("pause");
    return 0;
}
