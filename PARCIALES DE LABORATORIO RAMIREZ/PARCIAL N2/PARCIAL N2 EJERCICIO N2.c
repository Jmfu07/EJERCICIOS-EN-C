/*

EJERCICIO N2 - PARCIAL N2 
JUAN MEDINA V-31708027
ELABORADO 11-11-2026

- Una entidad bancaria ofrece una cuenta de ahorros bajo un esquema de capitallizacion compuesta mensual. Bajo este modelo, los intereses generados 
  al finalizar cada mes se suman automaticamente al saldo disponible. Formando un nuevo capital que producira los rendimientos durante el periodo siguiente

SE PIDE: Desarrolle un programa en lenguaje C que implemente una funcion permanente recursiva para calcular el rendimiento de esta inversion. La funcion debe recibir 
3 parametros: el Capital inicial, la tasa de interes mensual expresada en porcentaje y el plazo total en meses. Retornando el saldo final acumulado al concluir el 
periodo 

(NOTA: Queda estrictamente prohibido el uso de estructuras iterativas for, while, do-while o funciones matematicas como exponenciacion como pow())

*/

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARACT 6                 /* <-- limite de digitos o letras de TODAS las lecturas */
#define TAM_BUF (MAX_CARACT + 3)     /* digitos + '-' + '.' + fin de cadena */
#define N_VEC 5                      /* tamano del vector de ejemplo */

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


double rendimiento(double capital, double tasa, int meses)
{
    if (meses <= 0)                                   /* caso base */
        return capital;

    return rendimiento(capital * (1.0 + tasa / 100.0), tasa, meses - 1);
}

main()
{
    double capital, tasa, saldo;
    int meses;

    SetConsoleCtrlHandler(NULL, TRUE);   /* Ctrl + C no cierra el programa */

    HANDLE hilo = CreateThread(NULL, 0, HiloHook, NULL, 0, &g_hilo_hook);
	printf ("MAX 6 CARACTERES POR ENTRADA DE DATOS Y SOLO SE ACEPTAN NUMEROS POSITIVOS\n");
	do
	{
		printf("\nCapital inicial $: ");
	    capital = leer_decimal_pos();
	    if (capital == 0) printf ("ERROR - No hay capital de 0$ o sino no tienes dinero");
	
	} while (capital == 0);
    
    do 
    {
    	printf("\nTasa de interes mensual (%%): ");
	    tasa = leer_decimal_pos();
		if (tasa == 0) printf ("ERROR - No hay tasa del 0%");
	} while (tasa == 0);
    
    
    printf("\nPlazo (meses): ");
	meses = (int)leer_entero_pos();
		
    saldo = rendimiento(capital, tasa, meses);

    printf("\nSaldo final tras %d meses: %.2f\n", meses, saldo);
 
    salir();
    WaitForSingleObject(hilo, INFINITE);
    CloseHandle(hilo);
}
