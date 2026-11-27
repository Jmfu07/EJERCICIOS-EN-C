/*
 * Guia: Estructuras Secuenciales - Ejercicio 20
 * Enunciado: Escribir un programa que solicite del usuario un caracter y que
 * situe ese caracter en el centro de la pantalla. El usuario debe poder a
 * continuacion desplazar el caracter pulsando las letras A (arriba), B (abajo),
 * I (izquierda), D (derecha) y F (fin) para terminar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

char leer_caracter_visible(const char *prompt) {
    char buffer[128];

    while (1) {
        size_t i = 0;
        char c;

        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        while (buffer[i] == ' ' || buffer[i] == '\t') {
            i++;
        }
        c = buffer[i];
        if (c == '\0' || c == '\n' || c == '\r') {
            printf("Debe ingresar un caracter visible.\n");
            continue;
        }
        i++;
        while (buffer[i] == ' ' || buffer[i] == '\t') {
            i++;
        }
        if (buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\0') {
            printf("Ingrese unicamente un solo caracter.\n");
            continue;
        }
        return c;
    }
}

int main(void) {
    char ch;
    int x = 40, y = 12;
    const int min_x = 0, max_x = 79;
    const int min_y = 2, max_y = 23;
    int tecla;

    printf("=== EJERCICIO 20: DESPLAZAR CARACTER EN LA PANTALLA ===\n\n");
    ch = leer_caracter_visible("Ingrese un caracter para situar en el centro de la pantalla: ");

    system("cls");
    gotoxy(0, 0);
    printf("Controles: [A] Arriba | [B] Abajo | [I] Izquierda | [D] Derecha | [F] Fin");
    gotoxy(0, 1);
    printf("--------------------------------------------------------------------------------");

    gotoxy(x, y);
    putchar(ch);
    fflush(stdout);

    while (1) {
        int nuevo_x = x;
        int nuevo_y = y;

        tecla = _getch();
        if (tecla == 0 || tecla == 224) {
            (void)_getch();
            continue;
        }
        tecla = toupper(tecla);

        if (tecla == 'F') {
            break;
        } else if (tecla == 'A') {
            if (nuevo_y > min_y) nuevo_y--;
        } else if (tecla == 'B') {
            if (nuevo_y < max_y) nuevo_y++;
        } else if (tecla == 'I') {
            if (nuevo_x > min_x) nuevo_x--;
        } else if (tecla == 'D') {
            if (nuevo_x < max_x) nuevo_x++;
        } else {
            continue;
        }

        if (nuevo_x != x || nuevo_y != y) {
            gotoxy(x, y);
            putchar(' ');
            x = nuevo_x;
            y = nuevo_y;
            gotoxy(x, y);
            putchar(ch);
            fflush(stdout);
        }
    }

    gotoxy(0, max_y + 1);
    printf("\nPrograma finalizado.\n\n");
    system("pause");
    return 0;
}
