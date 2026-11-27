/* bloqueo_ligero.c - C puro (Win32).
 * Bloquea atajos con un gancho de teclado. NO toca el registro ni pide administrador:
 * Ctrl+Alt+Supr y Win+L siguen funcionando. Una sola ventana de consola.
 * Salida: Ctrl+F12 (al instante) o escribe 0 y Enter.
 * Compilar: gcc bloqueo_ligero.c -o bloqueo_ligero.exe -luser32
 */

#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <stdio.h>

#define TECLA_SALIDA VK_F12
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

    if (abajo && ctrl && vk == TECLA_SALIDA)
        ExitProcess(0);   /* salida rapida: no hay nada que restaurar */

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
    if (w) EnableMenuItem(GetSystemMenu(w, FALSE), SC_CLOSE, MF_GRAYED); /* X de la consola desactivada */
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

int main(void)
{
    char linea[16];

    if (!Iniciar())
        return 1;

    /* Aqui va tu programa. Ctrl+F12 sale en cualquier momento */
    printf("Atajos bloqueados. Salida: Ctrl+F12 o escribe 0 y Enter.\n");
    while (fgets(linea, sizeof linea, stdin)) {
        if (linea[0] == '0')
            break;
        printf("Escribiste: %s", linea);
    }

    Finalizar();
    return 0;
}
