/* 
 JUAN MEDINA V-31708027
 
 Ejercicio 2: leer un vector de 10 numeros reales, calcular el promedio
 e indicar cuantos elementos son mayores que el promedio y cuantos son
 menores o iguales. Cada numero admite como maximo 6 caracteres al
 escribirlo. Mientras corre, el hook bloquea las combinaciones de teclas
 de la lista. Se sale con la opcion 0.
 
 */
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N          10   /* tamano del vector */
#define MAX_CARACT 6    /* maximo de caracteres por numero */

static HHOOK g_hook;

static DWORD g_hilo_hook;

/* Funcion de salida: detiene el hook sin usar ninguna tecla */
static void salir(void)
{
    PostThreadMessage(g_hilo_hook, WM_QUIT, 0, 0);
}

/* Devuelve 1 si la tecla pulsada es una de las combinaciones de la lista */
static int validar(DWORD vk, int ctrl, int shift, int alt)
{
    /* Ctrl + letra */
    if (ctrl && !shift && !alt && vk != 0 && strchr("CVXZYAFBPSGNTWM", (int)vk))
        return 1;

    if (vk == VK_ESCAPE)                                /* Esc y sus combinaciones */
        return (ctrl && shift && !alt) ||               /* Ctrl + Shift + Esc */
               (ctrl && !shift && !alt) ||              /* Ctrl + Esc */
               (alt && !ctrl && !shift) ||              /* Alt + Esc */
               (shift && !ctrl && !alt) ||              /* Shift + Esc */
               (!ctrl && !shift && !alt);               /* Esc */

    if (vk == VK_F4 && alt && !ctrl && !shift)          /* Alt + F4 */
        return 1;

    return 0;
}

static LRESULT CALLBACK Hook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT *kb = (KBDLLHOOKSTRUCT *)lParam;

        int ctrl  = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
        int shift = (GetAsyncKeyState(VK_SHIFT)   & 0x8000) != 0;
        int alt   = (kb->flags & LLKHF_ALTDOWN) != 0;

        if (validar(kb->vkCode, ctrl, shift, alt))
            return 1;   /* la tecla no llega a ninguna aplicacion */
    }
    return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

/* El hook corre en su propio hilo para que la lectura no lo bloquee */
static DWORD WINAPI HiloHook(LPVOID p)
{
    g_hook = SetWindowsHookEx(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(g_hook);
    return 0;
}

/* Lee un numero real tecla por tecla. Solo deja escribir digitos, un punto
 * decimal y un '-' al inicio, con un maximo de MAX_CARACT caracteres.
 * Enter confirma (el numero debe tener al menos un digito); Backspace borra. */
static double leer_real(void)
{
    char buf[MAX_CARACT + 1];
    int len = 0, hay_punto = 0, hay_digito = 0;

    for (;;) {
        int c = _getch();

        if (c == 0 || c == 0xE0) {          /* tecla especial (flechas, F1...): se ignora */
            _getch();
            continue;
        }

        if (c == '\r') {                    /* Enter */
            if (hay_digito)
                break;
            continue;
        }

        if (c == '\b') {                    /* Backspace */
            if (len > 0) {
                len--;
                if (buf[len] == '.') hay_punto = 0;
                hay_digito = 0;
                for (int i = 0; i < len; i++)
                    if (buf[i] >= '0' && buf[i] <= '9') hay_digito = 1;
                printf("\b \b");
            }
            continue;
        }

        if (len >= MAX_CARACT)              /* limite de caracteres alcanzado */
            continue;

        if (c >= '0' && c <= '9') {
            hay_digito = 1;
        } else if (c == '.' && !hay_punto) {
            hay_punto = 1;
        } else if (c == '-' && len == 0) {
            /* signo negativo solo al inicio */
        } else {
            continue;                       /* cualquier otro caracter se rechaza */
        }

        buf[len++] = (char)c;
        putchar(c);
    }

    buf[len] = '\0';
    putchar('\n');
    return strtod(buf, NULL);
}

int main()
{
    SetConsoleCtrlHandler(NULL, TRUE);   /* Ctrl + C no cierra el programa */

    HANDLE hilo = CreateThread(NULL, 0, HiloHook, NULL, 0, &g_hilo_hook);

    int opcion;
    double v[N], suma, promedio;
    int mayores, menores_iguales, repetir;
    int r;

    printf("Hook activo: prueba Ctrl + C, Ctrl + V, Esc, etc. (no deben funcionar)\n");

    do {
        printf("\n===== VECTOR DE %d NUMEROS REALES =====\n", N);
        printf("1. Ingresar vector y calcular\n");
        printf("0. Salir\n");
        printf("Opcion: ");

        /* Lee una sola tecla, sin necesidad de Enter */
        do {
            r = _getch();                     /* sin Enter: solo acepta '0' o '1' */
        } while (r != '0' && r != '1');
        printf("%c\n", r);
        opcion = r - '0';

        switch (opcion) {
        case 1:
            do {
                suma = 0;
                mayores = 0;
                menores_iguales = 0;

                printf("Escribe %d numeros reales (maximo %d caracteres cada uno):\n", N, MAX_CARACT);
                for (int i = 0; i < N; i++) {
                    printf("Elemento %d: ", i + 1);
                    v[i] = leer_real();
                    suma += v[i];
                }

                promedio = suma / N;

                for (int i = 0; i < N; i++) {
                    if (v[i] > promedio)
                        mayores++;
                    else
                        menores_iguales++;
                }

                printf("\nPromedio: %.2f\n", promedio);
                printf("Mayores que el promedio: %d\n", mayores);
                printf("Menores o iguales al promedio: %d\n", menores_iguales);

                /* Pregunta si desea volver a ingresar 10 numeros */
                printf("\nDesea volver a ingresar %d numeros reales? (S/N): ", N);
                do {
                    r = _getch();
                } while (r != 's' && r != 'S' && r != 'n' && r != 'N');
                printf("%c\n", r);
                repetir = (r == 's' || r == 'S');
            } while (repetir);
            break;
        case 0:
            printf("Saliendo...\n");
            salir();
            break;
        default:
            printf("Opcion no valida.\n");
        }
    } while (opcion != 0);

    WaitForSingleObject(hilo, INFINITE);
    CloseHandle(hilo);
    return 0;
}
