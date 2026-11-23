/* Compilar: gcc validaciones.c -o validaciones.exe
 *
 * Validaciones de entrada tecla por tecla, con el hook de teclado activo y
 * un menu con switch para probar cada una (1 a 7) y ver ejemplos de uso
 * con operaciones (8) y con un vector (9). Se sale con la opcion 0.
 *
 * LIMITE: cada validacion recibe su propio maximo de digitos o letras. El '-'
 * y el '.' NO cuentan. Ejemplo con maximo 6: 123456, -123456 (7 caracteres),
 * 1234.56 (7 caracteres) o -1234.56 (8 caracteres).
 * Los maximos de cada opcion se cambian en los #define MAX_... de abajo.
 * Para enteros usa 9 digitos o menos (un long de Windows es de 32 bits).
 */
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Maximo de digitos o letras de cada validacion */
#define MAX_ENTERO_POS 6
#define MAX_ENTERO_NEG 5
#define MAX_ENTERO     4
#define MAX_DEC_POS    6
#define MAX_DEC_NEG    5
#define MAX_DEC        3
#define MAX_LETRAS     8
#define MAX_OPERACION  4     /* numeros de la opcion 8 */
#define MAX_VECTOR     3     /* numeros de la opcion 9 */

#define LIMITE_MAX 20                 /* tope absoluto: ninguna lectura puede pasar de aqui */
#define TAM_BUF    (LIMITE_MAX + 3)   /* digitos + '-' + '.' + fin de cadena */
#define N_VEC      5                  /* tamano del vector de ejemplo */

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

/* Lectura base, tecla por tecla.
 *   punto:  1 = permite un punto decimal
 *   signo:  0 = sin signo (positivo), 1 = '-' opcional, 2 = negativo (el '-' se pone solo)
 *   letras: 1 = solo letras A-Z / a-z (ignora punto y signo)
 *   max:    maximo de digitos o letras (el '-' y el '.' no cuentan)
 * Enter confirma (al menos un digito o una letra). Backspace borra.
 * buf debe tener TAM_BUF espacios. */
static void leer_texto(char *buf, int punto, int signo, int letras, int max)
{
    int len = 0, hay_punto = 0, cuenta = 0;     /* cuenta = digitos o letras escritos */
    int minimo = (signo == 2) ? 1 : 0;          /* el '-' fijo no se puede borrar */

    if (max > LIMITE_MAX) max = LIMITE_MAX;
    if (max < 1) max = 1;

    if (signo == 2) {
        buf[len++] = '-';
        putchar('-');
    }

    for (;;) {
        int c = _getch();

        if (c == 0 || c == 0xE0) {          /* tecla especial: se ignora */
            _getch();
            continue;
        }

        if (c == '\r') {                    /* Enter */
            if (cuenta > 0)
                break;
            continue;
        }

        if (c == '\b') {                    /* Backspace */
            if (len > minimo) {
                len--;
                if (buf[len] == '.') hay_punto = 0;
                cuenta = 0;
                for (int i = 0; i < len; i++)
                    if (buf[i] != '-' && buf[i] != '.') cuenta++;
                printf("\b \b");
            }
            continue;
        }

        if (letras) {
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) || cuenta >= max)
                continue;
            cuenta++;
        } else if (c >= '0' && c <= '9') {
            if (cuenta >= max)              /* limite de digitos de esta validacion */
                continue;
            cuenta++;
        } else if (c == '.' && punto && !hay_punto) {
            hay_punto = 1;
        } else if (c == '-' && signo == 1 && len == 0) {
            /* signo negativo opcional, solo al inicio */
        } else {
            continue;
        }

        buf[len++] = (char)c;
        putchar(c);
    }

    buf[len] = '\0';
    putchar('\n');
}

/* ---- Una funcion por cada validacion (cada una recibe su maximo) ---- */

static long leer_entero_pos(int max)   { char b[TAM_BUF]; leer_texto(b, 0, 0, 0, max); return strtol(b, NULL, 10); }
static long leer_entero_neg(int max)   { char b[TAM_BUF]; leer_texto(b, 0, 2, 0, max); return strtol(b, NULL, 10); }
static long leer_entero(int max)       { char b[TAM_BUF]; leer_texto(b, 0, 1, 0, max); return strtol(b, NULL, 10); }
23222

static double leer_decimal_pos(int max) { char b[TAM_BUF]; leer_texto(b, 1, 0, 0, max); return strtod(b, NULL); }
static double leer_decimal_neg(int max) { char b[TAM_BUF]; leer_texto(b, 1, 2, 0, max); return strtod(b, NULL); }
static double leer_decimal(int max)     { char b[TAM_BUF]; leer_texto(b, 1, 1, 0, max); return strtod(b, NULL); }

static void leer_letras(char *destino, int max) { leer_texto(destino, 0, 0, 1, max); }   /* destino: TAM_BUF espacios */

int main(void)
{
    SetConsoleCtrlHandler(NULL, TRUE);   /* Ctrl + C no cierra el programa */

    HANDLE hilo = CreateThread(NULL, 0, HiloHook, NULL, 0, &g_hilo_hook);

    int opcion, r, op;
    char texto[TAM_BUF];
    double a, b;
    long vec[N_VEC], suma, mayor;

    printf("Hook activo: prueba Ctrl + C, Ctrl + V, Esc, etc. (no deben funcionar)\n");

    do {
        printf("\n===== VALIDACIONES =====\n");
        printf("(el maximo no cuenta el '-' ni el '.')\n");
        printf("1. Entero positivo            (max %d)\n", MAX_ENTERO_POS);
        printf("2. Entero negativo            (max %d)\n", MAX_ENTERO_NEG);
        printf("3. Entero positivo o negativo (max %d)\n", MAX_ENTERO);
        printf("4. Decimal positivo           (max %d)\n", MAX_DEC_POS);
        printf("5. Decimal negativo           (max %d)\n", MAX_DEC_NEG);
        printf("6. Decimal positivo o negativo (max %d)\n", MAX_DEC);
        printf("7. Solo letras                (max %d)\n", MAX_LETRAS);
        printf("8. Ejemplo: operaciones       (max %d)\n", MAX_OPERACION);
        printf("9. Ejemplo: vector de %d enteros (max %d)\n", N_VEC, MAX_VECTOR);
        printf("0. Salir\n");
        printf("Opcion: ");

        do {
            r = _getch();                     /* sin Enter: solo acepta '0' a '9' */
        } while (r < '0' || r > '9');
        printf("%c\n", r);
        opcion = r - '0';

        switch (opcion) {
        case 1:
            printf("Entero positivo: ");
            printf("Leido: %ld\n", leer_entero_pos(MAX_ENTERO_POS));
            break;
        case 2:
            printf("Entero negativo: ");
            printf("Leido: %ld\n", leer_entero_neg(MAX_ENTERO_NEG));
            break;
        case 3:
            printf("Entero: ");
            printf("Leido: %ld\n", leer_entero(MAX_ENTERO));
            break;
        case 4:
            printf("Decimal positivo: ");
            printf("Leido: %g\n", leer_decimal_pos(MAX_DEC_POS));
            break;
        case 5:
            printf("Decimal negativo: ");
            printf("Leido: %g\n", leer_decimal_neg(MAX_DEC_NEG));
            break;
        case 6:
            printf("Decimal: ");
            printf("Leido: %g\n", leer_decimal(MAX_DEC));
            break;
        case 7:
            printf("Letras: ");
            leer_letras(texto, MAX_LETRAS);
            printf("Leido: %s\n", texto);
            break;
        case 8:     /* operaciones con dos decimales */
            printf("Primer numero: ");
            a = leer_decimal(MAX_OPERACION);
            printf("Segundo numero: ");
            b = leer_decimal(MAX_OPERACION);
            printf("Operacion (+ - * /): ");
            do {
                op = _getch();
            } while (op != '+' && op != '-' && op != '*' && op != '/');
            printf("%c\n", op);

            switch (op) {
            case '+': printf("Resultado: %g\n", a + b); break;
            case '-': printf("Resultado: %g\n", a - b); break;
            case '*': printf("Resultado: %g\n", a * b); break;
            case '/':
                if (b != 0)
                    printf("Resultado: %g\n", a / b);
                else
                    printf("Error: no se puede dividir entre cero.\n");
                break;
            }
            break;
        case 9:     /* vector de enteros */
            suma = 0;
            printf("Escribe %d numeros enteros:\n", N_VEC);
            for (int i = 0; i < N_VEC; i++) {
                printf("Elemento %d: ", i + 1);
                vec[i] = leer_entero(MAX_VECTOR);
                suma += vec[i];
                if (i == 0 || vec[i] > mayor)
                    mayor = vec[i];
            }
            printf("Suma: %ld\n", suma);
            printf("Promedio: %.2f\n", (double)suma / N_VEC);
            printf("Mayor: %ld\n", mayor);
            break;
        case 0:
            printf("Saliendo...\n");
            salir();
            break;
        }
    } while (opcion != 0);

    WaitForSingleObject(hilo, INFINITE);
    CloseHandle(hilo);
    return 0;
}
