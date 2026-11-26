#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ELABORADO POR JUAN MEDINA 20/08/2026
 * 18. Reserva de asientos. Crear un programa que mediante un menu admita reservar o cancelar
 * asientos de una sala de cine, asi como mostrar que asientos estan ocupados y libres actualmente.
 * El array tendra 25 filas y 4 columnas.
 */

#define PERMITIR_ALTGR 0
#define POL "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\"

static HHOOK  g_hook;
static HANDLE g_th, g_listo, g_hPrincipal;
static HANDLE g_guardia, g_evOk, g_evStop;
static DWORD  g_hilo;
static BOOL   pasada[256];

typedef struct { BOOL hklm; const char *ruta, *nombre; DWORD valor; } Pol;

static const Pol POLS[] = {
    {0, POL "System",   "DisableLockWorkstation", 1},
    {0, POL "System",   "DisableTaskMgr", 1},
    {0, POL "System",   "DisableChangePassword", 1},
    {0, POL "Explorer", "NoLogoff", 1},
    {0, POL "Explorer", "NoClose", 1},
    {1, POL "System",   "DisableLockWorkstation", 1},
    {1, POL "System",   "HideFastUserSwitching", 1},
    {1, POL "System",   "shutdownwithoutlogon", 0},
    {1, "SOFTWARE\\Policies\\Microsoft\\Windows\\System", "DontDisplayNetworkSelectionUI", 1},
    {1, "SOFTWARE\\Microsoft\\Windows Embedded\\EmbeddedLogon", "BrandingNeutral", 8},
};

static BOOL EsAdmin(void)
{
    HANDLE t; TOKEN_ELEVATION e; DWORD n; BOOL r = FALSE;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &t)) {
        r = GetTokenInformation(t, TokenElevation, &e, sizeof e, &n) && e.TokenIsElevated;
        CloseHandle(t);
    }
    return r;
}

static void Pausa(const char *msg) { printf("%s\nPulsa Enter.\n", msg); getchar(); }
static void Cerrar(HANDLE *h) { if (*h) CloseHandle(*h); *h = NULL; }
static char *Nombre(char *b, const char *tipo, DWORD pid) { sprintf(b, "BloqueoAtajos_%s_%lu", tipo, (unsigned long)pid); return b; }

static BOOL Elevar(const char *args, HANDLE *proc)
{
    char exe[MAX_PATH]; SHELLEXECUTEINFOA s; memset(&s, 0, sizeof s);
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    s.cbSize = sizeof s; s.fMask = SEE_MASK_NOCLOSEPROCESS; s.lpVerb = "runas";
    s.lpFile = exe; s.lpParameters = args; s.nShow = SW_HIDE;
    if (!ShellExecuteExA(&s) || !s.hProcess) { Pausa("Se necesita administrador (UAC cancelado)."); return FALSE; }
    if (proc) *proc = s.hProcess;
    else { WaitForSingleObject(s.hProcess, INFINITE); CloseHandle(s.hProcess); }
    return TRUE;
}

static void Valor(HKEY r, const char *ruta, const char *n, DWORD tipo, const void *d, DWORD tam, BOOL on)
{
    if (on) RegSetKeyValueA(r, ruta, n, tipo, d, tam);
    else    RegDeleteKeyValueA(r, ruta, n);
}

static void Politicas(BOOL on, BOOL soloUsuario)
{
    char exe[MAX_PATH], cmd[MAX_PATH + 16]; size_t i;
    if (!soloUsuario) {
        GetModuleFileNameA(NULL, exe, MAX_PATH);
        sprintf(cmd, "\"%s\" limpiar", exe);
        Valor(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
              "BloqueoAtajos", REG_SZ, cmd, (DWORD)strlen(cmd) + 1, on);
    }
    for (i = 0; i < sizeof POLS / sizeof *POLS; i++)
        if (!(soloUsuario && POLS[i].hklm))
            Valor(POLS[i].hklm ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER,
                  POLS[i].ruta, POLS[i].nombre, REG_DWORD, &POLS[i].valor, sizeof(DWORD), on);
}

static void Guardia(const char *arg)
{
    char n[64]; DWORD pid = strtoul(arg, NULL, 10); HANDLE h[2], ok;
    h[0] = OpenProcess(SYNCHRONIZE, FALSE, pid);
    ok   = OpenEventA(EVENT_MODIFY_STATE, FALSE, Nombre(n, "ok", pid));
    h[1] = OpenEventA(SYNCHRONIZE, FALSE, Nombre(n, "stop", pid));
    Politicas(TRUE, FALSE);
    if (ok) SetEvent(ok);
    if (h[0] && h[1]) WaitForMultipleObjects(2, h, FALSE, INFINITE);
    Politicas(FALSE, FALSE);
}

static void Limpiar(void) { if (EsAdmin()) Politicas(FALSE, FALSE); else Elevar("limpiar", NULL); }
static void Restaurar(void)
{
    HANDLE g = (HANDLE)InterlockedExchangePointer((PVOID volatile *)&g_guardia, NULL);
    Politicas(FALSE, g != NULL);
    if (g) { SetEvent(g_evStop); WaitForSingleObject(g, 5000); CloseHandle(g); }
}

static BOOL Permitida(DWORD v)
{
    return v == VK_SPACE || v == VK_DELETE || v == VK_CAPITAL || v == VK_OEM_102
        || (v >= VK_LEFT && v <= VK_DOWN) || (v >= '0' && v <= '9') || (v >= 'A' && v <= 'Z')
        || (v >= VK_NUMPAD0 && v <= VK_DIVIDE) || (v >= VK_OEM_1 && v <= VK_OEM_8);
}

static BOOL Down(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }
static void Fantasma(void) { keybd_event(0xE8, 0, 0, 0); keybd_event(0xE8, 0, KEYEVENTF_KEYUP, 0); }

static LRESULT CALLBACK Hook(int c, WPARAM w, LPARAM l)
{
    KBDLLHOOKSTRUCT *k = (KBDLLHOOKSTRUCT *)l; DWORD vk = k->vkCode & 0xFF;
    BOOL abajo = (w == WM_KEYDOWN || w == WM_SYSKEYDOWN), pasa;
    if (c != HC_ACTION || (k->flags & LLKHF_INJECTED)) return CallNextHookEx(g_hook, c, w, l);
    if (vk == 0xFF) return 1;
    switch (vk) {
    case VK_LWIN: case VK_RWIN: case VK_MENU: case VK_LMENU: case VK_RMENU:
        if (!abajo) Fantasma();
        /* fallthrough */
    case VK_SHIFT: case VK_LSHIFT: case VK_RSHIFT: case VK_CONTROL: case VK_LCONTROL: case VK_RCONTROL:
        return CallNextHookEx(g_hook, c, w, l);
    }
    if (!abajo) { pasa = pasada[vk]; pasada[vk] = FALSE; return pasa ? CallNextHookEx(g_hook, c, w, l) : 1; }
    if (Down(VK_LWIN) || Down(VK_RWIN)
        || ((Down(VK_CONTROL) || Down(VK_MENU)) && !(PERMITIR_ALTGR && Down(VK_RMENU)))
        || (vk != VK_RETURN && vk != VK_BACK && !Permitida(vk)))
        return 1;
    pasada[vk] = TRUE;
    return CallNextHookEx(g_hook, c, w, l);
}

static HHOOK Poner(void) { return SetWindowsHookExA(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0); }
static VOID CALLBACK TPol(HWND h, UINT m, UINT_PTR i, DWORD t) { (void)h; (void)m; (void)i; (void)t; Politicas(TRUE, TRUE); }
static VOID CALLBACK TGancho(HWND h, UINT m, UINT_PTR i, DWORD t) { HHOOK nuevo = Poner(); (void)h; (void)m; (void)i; (void)t; if (nuevo) { UnhookWindowsHookEx(g_hook); g_hook = nuevo; } }
static BOOL WINAPI Cierre(DWORD e) { if (e == CTRL_C_EVENT || e == CTRL_BREAK_EVENT) return TRUE; Restaurar(); return FALSE; }

static DWORD WINAPI Hilo(LPVOID p)
{
    MSG m; (void)p;
    g_hilo = GetCurrentThreadId();
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    SetTimer(NULL, 0, 2000, TPol);
    SetTimer(NULL, 0, 20000, TGancho);
    Politicas(TRUE, g_guardia != NULL);
    g_hook = Poner();
    SetEvent(g_listo);
    while (g_hook && GetMessage(&m, NULL, 0, 0) > 0) DispatchMessage(&m);
    if (g_hook) UnhookWindowsHookEx(g_hook);
    Restaurar();
    return 0;
}

static void Finalizar(void)
{
    if (g_th) { PostThreadMessage(g_hilo, WM_QUIT, 0, 0); WaitForSingleObject(g_th, 8000); Cerrar(&g_th); }
    Cerrar(&g_hPrincipal);
}

static BOOL LanzarGuardia(void)
{
    char n[64], args[48]; DWORD pid = GetCurrentProcessId(); HANDLE h[2];
    h[0] = g_evOk = CreateEventA(NULL, TRUE, FALSE, Nombre(n, "ok", pid));
    g_evStop      = CreateEventA(NULL, TRUE, FALSE, Nombre(n, "stop", pid));
    sprintf(args, "guardia %lu", (unsigned long)pid);
    if (!Elevar(args, &g_guardia)) return FALSE;
    h[1] = g_guardia;
    if (WaitForMultipleObjects(2, h, FALSE, 120000) == WAIT_OBJECT_0) return TRUE;
    Pausa("La ayuda con permisos de administrador no confirmo a tiempo.");
    return FALSE;
}

static BOOL Bloquear(void)
{
    DWORD modo; HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    if (g_th) return TRUE;
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_hPrincipal, THREAD_TERMINATE, FALSE, 0);
    if (!EsAdmin() && !LanzarGuardia()) { Restaurar(); return FALSE; }
    SetConsoleCtrlHandler(Cierre, TRUE);
    if (GetConsoleMode(in, &modo)) SetConsoleMode(in, (modo | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE);
    g_listo = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_th = CreateThread(NULL, 0, Hilo, NULL, 0, NULL);
    WaitForSingleObject(g_listo, 5000);
    if (!g_hook) { Cerrar(&g_th); Restaurar(); return FALSE; }
    atexit(Finalizar);
    return TRUE;
}

static void Iniciar(void)
{
    if (__argc > 1 && !strcmp(__argv[1], "guardia")) { Guardia(__argc > 2 ? __argv[2] : ""); ExitProcess(0); }
    if (__argc > 1 && !strcmp(__argv[1], "limpiar")) { Limpiar(); ExitProcess(0); }
    if (!Bloquear()) { Pausa("Error al instalar el bloqueo."); ExitProcess(1); }
}

static int leer_entero_rango(const char *prompt, int min, int max)
{
    char buf[8]; int len, c, v;
    for (;;) {
        len = 0;
        printf("%s [%d a %d]: ", prompt, min, max);
        for (;;) {
            c = _getch();
            if (c == 0 || c == 0xE0) { _getch(); continue; }
            if (c == '\r') { if (len > 0) break; continue; }
            if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
            if (c >= '0' && c <= '9' && len < 2) {
                buf[len++] = (char)c; putchar(c);
            }
        }
        buf[len] = '\0'; putchar('\n');
        v = (int)strtol(buf, NULL, 10);
        if (v >= min && v <= max) return v;
        printf("  Error: valor fuera de rango (%d a %d).\n", min, max);
    }
}

int main(void)
{
    int asientos[25][4] = {{0}};
    int fila, columna, opcion, i, j, r, libres, ocupados;

    Iniciar();

    do {
        printf("\n--- RESERVA DE ASIENTOS DE CINE (25 FILAS x 4 COLUMNAS) ---\n");
        printf("1. Reservar asiento\n");
        printf("2. Cancelar reserva\n");
        printf("3. Mostrar estado de la sala\n");
        printf("0. Salir\n");
        printf("Opcion [0-3]: ");

        do {
            r = _getch();
            if (r == 0 || r == 0xE0) { _getch(); r = -1; }
        } while (r < '0' || r > '3');
        printf("%c\n", r);
        opcion = r - '0';

        switch (opcion) {
        case 1:
            fila    = leer_entero_rango("Fila", 1, 25);
            columna = leer_entero_rango("Columna", 1, 4);
            if (asientos[fila - 1][columna - 1] == 1) {
                printf("El asiento (Fila %d, Columna %d) ya esta OCUPADO.\n", fila, columna);
            } else {
                asientos[fila - 1][columna - 1] = 1;
                printf("Asiento (Fila %d, Columna %d) RESERVADO con exito.\n", fila, columna);
            }
            break;
        case 2:
            fila    = leer_entero_rango("Fila", 1, 25);
            columna = leer_entero_rango("Columna", 1, 4);
            if (asientos[fila - 1][columna - 1] == 0) {
                printf("El asiento (Fila %d, Columna %d) ya estaba LIBRE.\n", fila, columna);
            } else {
                asientos[fila - 1][columna - 1] = 0;
                printf("Reserva en (Fila %d, Columna %d) CANCELADA con exito.\n", fila, columna);
            }
            break;
        case 3:
            libres = 0;
            ocupados = 0;
            printf("\nEstado de la sala (O = Ocupado, L = Libre):\n");
            printf("         C1 C2 C3 C4\n");
            for (i = 0; i < 25; i++) {
                printf("Fila %2d:  ", i + 1);
                for (j = 0; j < 4; j++) {
                    if (asientos[i][j] == 1) { printf("O  "); ocupados++; }
                    else                     { printf("L  "); libres++; }
                }
                printf("\n");
            }
            printf("Total Libres: %d | Total Ocupados: %d\n", libres, ocupados);
            break;
        case 0:
            printf("Saliendo...\n");
            break;
        }
    } while (opcion != 0);

    Finalizar();
    return 0;
}
