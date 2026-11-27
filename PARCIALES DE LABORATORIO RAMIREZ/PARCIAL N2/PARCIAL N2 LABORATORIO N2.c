/*

EJERCICIO N2 LABORATORIO - PARCIAL N2
JUAN MEDINA V-31708027
ELABORADO 12-11-2026

- Rutas multi tramo con retrasos de por zona (recursividad sobre arreglos)
- Calculo recursivo de tiempos de transito multi etapa

Una compania de transporte de carga divide sus envios nacionales en M tramos consecutivos (almacenados en un arreglo unidimensional 
int tramos[], donde cada posicion indica cuantos dias habiles toma cruzar esa region). La empresa no realiza movimientos de carga los sabados 
ni los domingos, por lo que cualquier paquete que llegue a un punto de control en fin de semana queda retenido hasta el lunes para iniciar el
siguiente tramo

Se pide: Programe en lenguaje C una funcion  puramente recursiva que reciba el arreglo de tramos, la cantidad total de tramo (M), el indice
 del tramo actual, el dia de la semana que inicia el envio (1= lunes ... 7= domingo) y retorne la cantidad total de dias/ calendario que tomara en 
 completar toda la ruta desde el origen hasta el destino final 
 
 
(RESTRICCION: NO SE PERMITE EL USO DE CICLOS ITERATIVOS EN NINGUNA PARTE DEL CALCULO DE RUTAS, NI EL USO DE ESTRUCTURAS DE DATOS DINAMICAS O STRUCT)

*/

#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMITE_MAX 20                 /* tope absoluto: ninguna lectura puede pasar de aqui */
#define TAM_BUF    (LIMITE_MAX + 3)   /* digitos + '-' + '.' + fin de cadena */
#define MAX_TRAMOS   20                 /* maximo de tramos permitidos */
#define MAX_FERIADOS 30                 /* maximo de feriados permitidos */

#define PERMITIR_ALTGR 0   /* 1 = deja pasar AltGr (para @ \ | # en teclado espanol) */

static HHOOK g_hook;
static HANDLE g_th, g_listo;
static DWORD g_hilo;
static BOOL pasada[256];   /* pasada[tecla]: su pulsacion llego a Windows, asi que su soltado tambien debe llegar */

/* Lista blanca: texto, numeros, flechas, Espacio, Supr, Mayus y teclado numerico.
   Lo demas (F1-F24, Esc, Tab, Inicio/Fin, menu contextual, multimedia) se traga. */
static BOOL Permitida(DWORD v)
{
    if (v == VK_SPACE || v == VK_DELETE || v == VK_CAPITAL) return TRUE;
    if (v >= VK_LEFT && v <= VK_DOWN) return TRUE;
    if (v >= '0' && v <= '9') return TRUE;
    if (v >= 'A' && v <= 'Z') return TRUE;
    if (v >= VK_NUMPAD0 && v <= VK_DIVIDE) return TRUE;
    if (v >= VK_OEM_1 && v <= VK_OEM_8) return TRUE;
    return v == VK_OEM_102;
}

/* Tecla fantasma: se inyecta al soltar Win o Alt para que Windows no tome el toque por abrir un menu */
static void Fantasma(void)
{
    keybd_event(0xE8, 0, 0, 0);
    keybd_event(0xE8, 0, KEYEVENTF_KEYUP, 0);
}

static LRESULT CALLBACK Hook(int c, WPARAM w, LPARAM l)
{
    KBDLLHOOKSTRUCT *k = (KBDLLHOOKSTRUCT *)l;
    DWORD vk;
    BOOL abajo, ctrl, alt, win, bloquea;

    if (c != HC_ACTION || (k->flags & LLKHF_INJECTED))
        return CallNextHookEx(g_hook, c, w, l);

    vk = k->vkCode & 0xFF;
    abajo = (w == WM_KEYDOWN || w == WM_SYSKEYDOWN);

    /* Win y Alt pasan a Windows; al soltarlas se manda la tecla fantasma */
    if (vk == VK_LWIN || vk == VK_RWIN || vk == VK_MENU || vk == VK_LMENU || vk == VK_RMENU) {
        if (!abajo) Fantasma();
        return CallNextHookEx(g_hook, c, w, l);
    }
    /* Ctrl y Mayus pasan siempre */
    if (vk == VK_SHIFT || vk == VK_LSHIFT || vk == VK_RSHIFT || vk == VK_CONTROL || vk == VK_LCONTROL || vk == VK_RCONTROL)
        return CallNextHookEx(g_hook, c, w, l);

    ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
    alt  = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
    win  = ((GetAsyncKeyState(VK_LWIN) | GetAsyncKeyState(VK_RWIN)) & 0x8000) != 0;
    if (PERMITIR_ALTGR && GetAsyncKeyState(VK_RMENU) < 0)
        ctrl = alt = FALSE;

    /* Soltado: pasa solo si su pulsacion paso */
    if (!abajo) {
        if (pasada[vk]) {
            pasada[vk] = FALSE;
            return CallNextHookEx(g_hook, c, w, l);
        }
        return 1;
    }

    /* Enter y Borrar pasan siempre, salvo con Ctrl, Alt o Win */
    if (vk == VK_RETURN || vk == VK_BACK)
        bloquea = ctrl || alt || win;
    else
        bloquea = ctrl || alt || win || !Permitida(vk);

    if (bloquea)
        return 1;

    pasada[vk] = TRUE;
    return CallNextHookEx(g_hook, c, w, l);
}

/* Hilo propio del gancho. Renueva el gancho cada 20 s para que Windows no lo descarte */
static DWORD WINAPI Hilo(LPVOID p)
{
    MSG m;
    (void)p;

    g_hilo = GetCurrentThreadId();
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    g_hook = SetWindowsHookExA(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0);
    SetTimer(NULL, 0, 20000, NULL);
    SetEvent(g_listo);

    while (GetMessage(&m, NULL, 0, 0) > 0) {
        if (m.message == WM_TIMER) {
            HHOOK nuevo = SetWindowsHookExA(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0);
            if (nuevo) {
                UnhookWindowsHookEx(g_hook);
                g_hook = nuevo;
            }
        } else {
            DispatchMessage(&m);
        }
    }
    if (g_hook) UnhookWindowsHookEx(g_hook);
    return 0;
}

/* Ctrl+C y Ctrl+Break se ignoran; el resto de cierres siguen su curso normal */
static BOOL WINAPI Cierre(DWORD e)
{
    return e == CTRL_C_EVENT || e == CTRL_BREAK_EVENT;
}

static void Finalizar(void);

/* INICIAR: deja la consola lista y activa el gancho. FALSE si no se pudo. */
static BOOL Iniciar(void)
{
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    HWND w;
    DWORD modo;

    SetConsoleCtrlHandler(Cierre, TRUE);
    w = GetConsoleWindow();
    if (GetConsoleMode(in, &modo))  /* un clic en la consola congelaria el gancho */
        SetConsoleMode(in, (modo | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE);

    g_listo = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_th = CreateThread(NULL, 0, Hilo, NULL, 0, NULL);
    WaitForSingleObject(g_listo, 5000);

    if (!g_hook) {
        printf("No se pudo activar el bloqueo. Pulsa Enter.\n");
        getchar();
        Finalizar();
        return FALSE;
    }
    return TRUE;
}

/* FINALIZAR: quita el gancho y espera a que su hilo termine. Segura de llamar dos veces */
static void Finalizar(void)
{
    if (g_th) {
        PostThreadMessage(g_hilo, WM_QUIT, 0, 0);
        WaitForSingleObject(g_th, 3000);
        CloseHandle(g_th);
        g_th = NULL;
    }
}


/* Lectura base, tecla por tecla: solo digitos (sin signo ni punto).
 * max = maximo de digitos. Enter confirma (al menos un digito). Backspace borra.
 * buf debe tener TAM_BUF espacios. */
static void leer_digitos(char buf[], int max)
{
    int len = 0;

    if (max > LIMITE_MAX) max = LIMITE_MAX;
    if (max < 1) max = 1;

    for (;;) {
        int c = _getch();

        if (c == 0 || c == 0xE0) {          /* tecla especial: se ignora */
            _getch();
            continue;
        }
        if (c == '\r') {                    /* Enter */
            if (len > 0) break;
            continue;
        }
        if (c == '\b') {                    /* Backspace */
            if (len > 0) { len--; printf("\b \b"); }
            continue;
        }
        if (c >= '0' && c <= '9' && len < max) {
            buf[len++] = (char)c;
            putchar(c);
        }
    }
    buf[len] = '\0';
    putchar('\n');
}

static long leer_entero_pos(int max) { char b[TAM_BUF]; leer_digitos(b, max); return strtol(b, NULL, 10); }


/* ================= SOLUCION DEL EJERCICIO ================= */

void leer_tramos(int tramos[], int M, int i)
{
    if (i >= M)
        return;
    printf("  Tramo %d (dias habiles, max 99): ", i + 1);
    tramos[i] = (int)leer_entero_pos(2);
    leer_tramos(tramos, M, i + 1);
}

void leer_feriados(int feriados[], int F, int i)
{
    if (i >= F)
        return;
    printf("  Feriado %d (dia numero, 1 = dia de salida): ", i + 1);
    feriados[i] = (int)leer_entero_pos(3);
    if (feriados[i] < 1) {
        printf("  Debe ser un dia mayor o igual a 1.\n");
        leer_feriados(feriados, F, i);
        return;
    }
    leer_feriados(feriados, F, i + 1);
}

int es_feriado(int feriados[], int F, int idx, int n)
{
    if (idx >= F)
        return 0;
    if (feriados[idx] == n)
        return 1;
    return es_feriado(feriados, F, idx + 1, n);
}

int simular(int tramos[], int M, int i, int resta, int dia, int n, int feriados[], int F)
{
    if (i >= M)
        return 0;

    if (resta == 0) {
        if (i + 1 < M)
            return simular(tramos, M, i + 1, tramos[i + 1], dia, n, feriados, F);
        return 0;
    }

    if (dia > 5 || es_feriado(feriados, F, 0, n))
        return 1 + simular(tramos, M, i, resta, dia % 7 + 1, n + 1, feriados, F);

    return 1 + simular(tramos, M, i, resta - 1, dia % 7 + 1, n + 1, feriados, F);
}

int ruta(int tramos[], int M, int i, int dia, int feriados[], int F)
{
    if (i >= M)
        return 0;
    return simular(tramos, M, i, tramos[i], dia, 1, feriados, F);
}

int main()
{
    int tramos[MAX_TRAMOS];
    int feriados[MAX_FERIADOS];
    int M, F, dia, total, c;

    if (!Iniciar())
        return 1;

    do {
    	system("cls");
        printf("=== RUTAS MULTI TRAMO ===\n\n");

        do {
            printf("Cantidad de tramos M (1 a %d): ", MAX_TRAMOS);
            M = (int)leer_entero_pos(2);
        } while (M < 1 || M > MAX_TRAMOS);

        printf("\nDias habiles de cada tramo:\n");
        leer_tramos(tramos, M, 0);

        do {
            printf("\nDia de inicio del envio (1=lunes, 2=martes, 3=miercoles, 4=jueves, 5=viernes, 6= sabado y 7=domingo): ");
            dia = (int)leer_entero_pos(1);
        } while (dia < 1 || dia > 7);

        do {
            printf("\nCantidad de feriados durante la ruta (0 a %d) 0 = No hay feriados: ", MAX_FERIADOS);
            F = (int)leer_entero_pos(2);
        } while (F > MAX_FERIADOS);

        if (F > 0) {
            printf("Indica cada feriado como numero de dia desde la salida:\n");
            leer_feriados(feriados, F, 0);
        }

        total = ruta(tramos, M, 0, dia, feriados, F);

        printf("\nDias calendario totales de la ruta: %d\n", total);

        printf("\nDesea calcular otra ruta? (S/N): ");
        do {
            c = _getch();
            if (c >= 'a' && c <= 'z')
                c = c - 32;
        } while (c != 'S' && c != 'N');
        putchar(c);
        printf("\n\n");
    } while (c == 'S');

    Finalizar();
    return 0;
}
