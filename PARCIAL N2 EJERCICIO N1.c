/*

EJERCICIO N1 - PARCIAL N2 
JUAN MEDINA V-31708027
ELABORADO 11-11-2026

a) Cargar simultaneamente  los nombres de los vendedores en un vector y la cantidad de articulos vendidos por vendedor en una matriZ
b) Sumar la cantidad de articulos por vendedor y almacenarlo en un vector 
c) Cual fue el vendedor que obtuvo la menor venta de articulos 
d) Copiar la cantidad de los articulos en un vector 
c) Utilizar el cilo while con respuesta al operador 
f) validar todas las variables imprimir los requerimientos anteriores 

*/

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARACT 6                 /* <-- limite de digitos o letras de TODAS las lecturas */
#define TAM_BUF (MAX_CARACT + 3)     /* digitos + '-' + '.' + fin de cadena */
#define N_VEC 5                      /* tamano del vector de ejemplo */
 
#define MAX_VEND 10000               /* tope de vendedores (n cantidad) */
#define N_ART 3                      /* 3 articulos por vendedor */

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
 * Solo los digitos (o letras) cuentan para el limite MAX_CARACT; el '-' y el '.' no.
 * Enter confirma (al menos un digito o una letra). Backspace borra.
 * buf debe tener TAM_BUF espacios. */
static void leer_texto(char *buf, int punto, int signo, int letras)
{
    int len = 0, hay_punto = 0, cuenta = 0;     /* cuenta = digitos o letras escritos */
    int minimo = (signo == 2) ? 1 : 0;          /* el '-' fijo no se puede borrar */

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
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) || cuenta >= MAX_CARACT)
                continue;
            cuenta++;
        } else if (c >= '0' && c <= '9') {
            if (cuenta >= MAX_CARACT)       /* limite de digitos */
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

/* ---- Una funcion por cada validacion ---- */

static long leer_entero_pos(void)   { char b[TAM_BUF]; leer_texto(b, 0, 0, 0); return strtol(b, NULL, 10); }
static long leer_entero_neg(void)   { char b[TAM_BUF]; leer_texto(b, 0, 2, 0); return strtol(b, NULL, 10); }
static long leer_entero(void)       { char b[TAM_BUF]; leer_texto(b, 0, 1, 0); return strtol(b, NULL, 10); }

static double leer_decimal_pos(void) { char b[TAM_BUF]; leer_texto(b, 1, 0, 0); return strtod(b, NULL); }
static double leer_decimal_neg(void) { char b[TAM_BUF]; leer_texto(b, 1, 2, 0); return strtod(b, NULL); }
static double leer_decimal(void)     { char b[TAM_BUF]; leer_texto(b, 1, 1, 0); return strtod(b, NULL); }
static void leer_letras(char *destino) { leer_texto(destino, 0, 0, 1); }  

int leer_sn()
{
    for (;;) {
        int c = _getch();
 
        if (c == 0 || c == 0xE0) {          /* tecla especial: se ignora */
            _getch();
            continue;
        }
 
        if (c == 's' || c == 'S') {
            putchar('S');
            putchar('\n');
            return 1;
        }
        if (c == 'n' || c == 'N') {
            putchar('N');
            putchar('\n');
            return 0;
        }
    }
}


main ()
{
	SetConsoleCtrlHandler(NULL, TRUE);   /* Ctrl + C no cierra el programa */

    HANDLE hilo = CreateThread(NULL, 0, HiloHook, NULL, 0, &g_hilo_hook);

    char nombres[MAX_VEND][TAM_BUF];     /* vector de nombres */
    int m[MAX_VEND][N_ART];             /* matriz: articulos vendidos por vendedor */
    int tot[MAX_VEND];                  /* vector de totales por vendedor */
    int copia[MAX_VEND * N_ART];        /* vector copia de las cantidades */

    int n = 0, i, j, k, menor;
    int otro = 1;
    
	 /* e) while con respuesta del operador: un vendedor por vuelta */
    while (otro) {
    	
    	system("cls");
    	printf ("SOLO 6 CARACTERES MAXiMO PARA INGRESAR LETRAS Y NUMEROS POSITIVOS");
 
        /* a) carga simultanea: nombre en el vector, articulos en la matriz */
        printf("\nVendedor %ld (solo letras): ", n + 1);
        leer_letras(nombres[n]);
        for (j = 0; j < N_ART; j++) {
            printf("  Articulo %ld (cantidad vendida): ", j + 1);
            m[n][j] = leer_entero_pos();
        }
        n++;
 
        if (n == MAX_VEND) {
            printf("\nSe alcanzo el maximo de %d vendedores.\n", MAX_VEND);
            otro = 0;
        } else {
            printf("\nDesea agregar otro vendedor? (S/N): ");
            otro = leer_sn();
        }
    }
 
    /* b) suma de articulos por vendedor */
    for (i = 0; i < n; i++) {
        tot[i] = 0;
        for (j = 0; j < N_ART; j++) tot[i] += m[i][j];
    }
 
    /* c) vendedor con la menor venta (si hay empate, el primero) */
    menor = 0;
    for (i = 1; i < n; i++)
        if (tot[i] < tot[menor]) menor = i;
 
    /* d) copiar las cantidades de la matriz a un vector */
    k = 0;
    for (i = 0; i < n; i++)
        for (j = 0; j < N_ART; j++) copia[k++] = m[i][j];
 
    /* f) imprimir todos los requerimientos */
    system("cls");
    printf("\n--- DATOS CARGADOS ---\n");
    for (i = 0; i < n; i++) {
        printf("%-8s", nombres[i]);
        for (j = 0; j < N_ART; j++) printf(" %7ld", m[i][j]);
        printf("   | Total: %ld\n", tot[i]);
    }
    printf("\nMenor venta: %s con %ld articulos\n", nombres[menor], tot[menor]);
    printf("Vector copia:");
    for (k = 0; k < n * N_ART; k++) printf(" %ld", copia[k]);
    printf("\n");
 
    salir();
    WaitForSingleObject(hilo, INFINITE);
    CloseHandle(hilo);
    }
