#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ELABORADO POR JUAN MEDINA 13/08/2026
 * 2.- Una persona tiene una cuenta corriente en un Banco Nacional con un monto inicial de 520 BsF.
 * Ingresa por teclado su clave de acceso de 4 digitos y una opcion numerica:
 *   1.- Consulta saldo.
 *   2.- Retiro (el monto se ingresa por teclado).
 *   3.- Deposito (el monto se ingresa por teclado).
 *   4.- Salir.
 * (Version 1: valida formato de clave de 4 digitos y todas las opciones, incluyendo invalidas)
 */

#define PERMITIR_ALTGR 1
#define MAX_MONTO_DIG  8
#define DOWN(vk)       ((GetAsyncKeyState(vk) & 0x8000) != 0)
#define POL            "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\"

/* ==================== BLOQUEO DE TECLADO Y POLITICAS ===================== */

static HHOOK  g_hook;
static HANDLE g_th, g_listo, g_guardia, g_evStop;
static DWORD  g_hilo;
static BOOL   g_pasada[256];

static const BOOL  POL_HKLM[] = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 };
static const char *POL_RUTA[] = {
    POL "System", POL "System", POL "System", POL "Explorer", POL "Explorer",
    POL "System", POL "System", POL "System",
    "SOFTWARE\\Policies\\Microsoft\\Windows\\System",
    "SOFTWARE\\Microsoft\\Windows Embedded\\EmbeddedLogon"
};
static const char *POL_NOM[]  = {
    "DisableLockWorkstation", "DisableTaskMgr", "DisableChangePassword", "NoLogoff", "NoClose",
    "DisableLockWorkstation", "HideFastUserSwitching", "shutdownwithoutlogon",
    "DontDisplayNetworkSelectionUI", "BrandingNeutral"
};
static const DWORD POL_VAL[]  = { 1, 1, 1, 1, 1, 1, 1, 0, 1, 8 };

static char *EvNom(char *b, const char *t, DWORD p) { sprintf(b, "Bloq_%s_%lu", t, (unsigned long)p); return b; }

static BOOL EsAdmin(void) {
    HANDLE t = NULL; TOKEN_ELEVATION e; DWORD n;
    BOOL r = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &t)
          && GetTokenInformation(t, TokenElevation, &e, sizeof e, &n) && e.TokenIsElevated;
    if (t) CloseHandle(t);
    return r;
}

static BOOL Elevar(const char *args, HANDLE *proc) {
    char exe[MAX_PATH]; SHELLEXECUTEINFOA s;
    memset(&s, 0, sizeof s); GetModuleFileNameA(NULL, exe, MAX_PATH);
    s.cbSize = sizeof s; s.fMask = SEE_MASK_NOCLOSEPROCESS;
    s.lpVerb = "runas"; s.lpFile = exe; s.lpParameters = args; s.nShow = SW_HIDE;
    if (!ShellExecuteExA(&s) || !s.hProcess) { printf("Se requiere administrador (UAC cancelado).\nPulsa Enter.\n"); getchar(); return FALSE; }
    if (proc) *proc = s.hProcess; else { WaitForSingleObject(s.hProcess, INFINITE); CloseHandle(s.hProcess); }
    return TRUE;
}

static void RegVal(HKEY r, const char *k, const char *n, DWORD t, const void *d, DWORD s, BOOL on) {
    if (on) RegSetKeyValueA(r, k, n, t, d, s); else RegDeleteKeyValueA(r, k, n);
}

static void Politicas(BOOL on, BOOL soloUsr) {
    char exe[MAX_PATH], cmd[MAX_PATH + 16]; size_t i;
    if (!soloUsr) {
        GetModuleFileNameA(NULL, exe, MAX_PATH);
        sprintf(cmd, "\"%s\" limpiar", exe);
        RegVal(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
               "BloqueoAtajos", REG_SZ, cmd, (DWORD)strlen(cmd) + 1, on);
    }
    for (i = 0; i < sizeof POL_VAL / sizeof *POL_VAL; i++)
        if (!(soloUsr && POL_HKLM[i]))
            RegVal(POL_HKLM[i] ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER,
                   POL_RUTA[i], POL_NOM[i], REG_DWORD, &POL_VAL[i], sizeof(DWORD), on);
}

static void Guardia(const char *arg) {
    char n[64]; DWORD pid = strtoul(arg, NULL, 10);
    HANDLE h[2], ok = OpenEventA(EVENT_MODIFY_STATE, FALSE, EvNom(n, "ok", pid));
    h[0] = OpenProcess(SYNCHRONIZE, FALSE, pid);
    h[1] = OpenEventA(SYNCHRONIZE, FALSE, EvNom(n, "stop", pid));
    Politicas(TRUE, FALSE);
    if (ok) { SetEvent(ok); CloseHandle(ok); }
    if (h[0] && h[1]) WaitForMultipleObjects(2, h, FALSE, INFINITE);
    if (h[0]) CloseHandle(h[0]);
    if (h[1]) CloseHandle(h[1]);
    Politicas(FALSE, FALSE);
}

static void Restaurar(void) {
    HANDLE g = (HANDLE)InterlockedExchangePointer((PVOID volatile *)&g_guardia, NULL);
    HANDLE s = (HANDLE)InterlockedExchangePointer((PVOID volatile *)&g_evStop, NULL);
    Politicas(FALSE, g != NULL);
    if (s) { if (g) SetEvent(s); CloseHandle(s); }
    if (g) { WaitForSingleObject(g, 5000); CloseHandle(g); }
}

static BOOL Permitida(DWORD v) {
    return v == VK_RETURN || v == VK_BACK || v == VK_SPACE || v == VK_DELETE || v == VK_CAPITAL || v == VK_OEM_102
        || (v >= VK_LEFT && v <= VK_DOWN) || (v >= '0' && v <= '9') || (v >= 'A' && v <= 'Z')
        || (v >= VK_NUMPAD0 && v <= VK_DIVIDE) || (v >= VK_OEM_1 && v <= VK_OEM_8);
}

static LRESULT CALLBACK Hook(int c, WPARAM w, LPARAM l) {
    KBDLLHOOKSTRUCT *k = (KBDLLHOOKSTRUCT *)l; DWORD vk = k->vkCode & 0xFF;
    BOOL abajo = (w == WM_KEYDOWN || w == WM_SYSKEYDOWN), pasa;
    if (c != HC_ACTION || (k->flags & LLKHF_INJECTED)) return CallNextHookEx(g_hook, c, w, l);
    if (vk == 0xFF) return 1;
    if (vk == VK_LWIN || vk == VK_RWIN || vk == VK_MENU || vk == VK_LMENU || vk == VK_RMENU) {
        if (!abajo) { keybd_event(0xE8, 0, 0, 0); keybd_event(0xE8, 0, KEYEVENTF_KEYUP, 0); }
        return CallNextHookEx(g_hook, c, w, l);
    }
    if (vk == VK_SHIFT || vk == VK_CONTROL || (vk >= VK_LSHIFT && vk <= VK_RCONTROL))
        return CallNextHookEx(g_hook, c, w, l);
    if (!abajo) { pasa = g_pasada[vk]; g_pasada[vk] = FALSE; return pasa ? CallNextHookEx(g_hook, c, w, l) : 1; }
    if (DOWN(VK_LWIN) || DOWN(VK_RWIN) || ((DOWN(VK_CONTROL) || DOWN(VK_MENU)) && !(PERMITIR_ALTGR && DOWN(VK_RMENU))) || !Permitida(vk))
        return 1;
    g_pasada[vk] = TRUE;
    return CallNextHookEx(g_hook, c, w, l);
}

static VOID CALLBACK Tick(HWND h, UINT m, UINT_PTR i, DWORD t) {
    static int n; (void)h; (void)m; (void)i; (void)t;
    Politicas(TRUE, TRUE);
    if (++n % 10 == 0) {
        HHOOK x = SetWindowsHookExA(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0);
        if (x) { UnhookWindowsHookEx(g_hook); g_hook = x; }
    }
}

static BOOL WINAPI Cierre(DWORD e) {
    if (e == CTRL_C_EVENT || e == CTRL_BREAK_EVENT) return TRUE;
    Restaurar(); return FALSE;
}

static DWORD WINAPI Hilo(LPVOID p) {
    MSG m; (void)p; g_hilo = GetCurrentThreadId();
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    SetTimer(NULL, 0, 2000, Tick);
    Politicas(TRUE, g_guardia != NULL);
    g_hook = SetWindowsHookExA(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0);
    SetEvent(g_listo);
    while (g_hook && GetMessage(&m, NULL, 0, 0) > 0) DispatchMessage(&m);
    if (g_hook) { UnhookWindowsHookEx(g_hook); g_hook = NULL; }
    Restaurar(); return 0;
}

static BOOL LanzarGuardia(void) {
    char n[64], args[48]; DWORD pid = GetCurrentProcessId(); BOOL ok;
    HANDLE h[2], evOk = CreateEventA(NULL, TRUE, FALSE, EvNom(n, "ok", pid));
    g_evStop = CreateEventA(NULL, TRUE, FALSE, EvNom(n, "stop", pid));
    sprintf(args, "guardia %lu", (unsigned long)pid);
    if (!Elevar(args, &g_guardia)) { CloseHandle(evOk); return FALSE; }
    h[0] = evOk; h[1] = g_guardia;
    ok = (WaitForMultipleObjects(2, h, FALSE, 120000) == WAIT_OBJECT_0);
    CloseHandle(evOk);
    if (!ok) { printf("La ayuda de administrador no confirmo a tiempo.\nPulsa Enter.\n"); getchar(); }
    return ok;
}

static void Finalizar(void) {
    if (!g_th) return;
    PostThreadMessage(g_hilo, WM_QUIT, 0, 0);
    WaitForSingleObject(g_th, 8000);
    CloseHandle(g_th); g_th = NULL;
}

static void Iniciar(void) {
    DWORD modo; HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    if (__argc > 1 && !strcmp(__argv[1], "guardia")) { Guardia(__argc > 2 ? __argv[2] : ""); ExitProcess(0); }
    if (__argc > 1 && !strcmp(__argv[1], "limpiar")) { if (EsAdmin()) Politicas(FALSE, FALSE); else Elevar("limpiar", NULL); ExitProcess(0); }
    if (g_th) return;
    if (!EsAdmin() && !LanzarGuardia()) { Restaurar(); printf("Error al instalar el bloqueo.\n"); getchar(); ExitProcess(1); }
    SetConsoleCtrlHandler(Cierre, TRUE);
    if (GetConsoleMode(in, &modo)) SetConsoleMode(in, (modo | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE);
    g_listo = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_th = CreateThread(NULL, 0, Hilo, NULL, 0, NULL);
    WaitForSingleObject(g_listo, 5000); CloseHandle(g_listo); g_listo = NULL;
    if (!g_hook) { if (g_th) { CloseHandle(g_th); g_th = NULL; } Restaurar(); printf("Error al bloquear.\n"); getchar(); ExitProcess(1); }
    atexit(Finalizar);
}

/* ==================== VALIDACIONES Y LOGICA DEL EJERCICIO ================ */

static int leer_clave_4_digitos(void) {
    char buf[5]; int len = 0;
    for (;;) {
        int c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (len == 4) break; continue; }
        if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
        if (c >= '0' && c <= '9' && len < 4) { buf[len++] = (char)c; putchar('*'); }
    }
    buf[4] = '\0'; putchar('\n');
    return (int)strtol(buf, NULL, 10);
}

static double leer_monto_pos(int max_dig) {
    char buf[20]; int len = 0, hay_pto = 0, cuenta = 0;
    for (;;) {
        int c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (cuenta > 0) break; continue; }
        if (c == '\b') {
            if (len > 0) {
                len--;
                if (buf[len] == '.') hay_pto = 0; else cuenta--;
                printf("\b \b");
            }
            continue;
        }
        if (c >= '0' && c <= '9') { if (cuenta >= max_dig) continue; cuenta++; }
        else if ((c == '.' || c == ',') && !hay_pto && cuenta < max_dig) { c = '.'; hay_pto = 1; }
        else continue;
        buf[len++] = (char)c; putchar(c);
    }
    buf[len] = '\0'; putchar('\n');
    return strtod(buf, NULL);
}

static int leer_opcion_menu(void) {
    for (;;) {
        int c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c >= '1' && c <= '4') { printf("%c\n", c); return c - '0'; }
        if (c >= 32 && c <= 126) printf("\n  Opcion '%c' incorrecta. Elija [1-4]: ", c);
    }
}

int main(void) {
    double saldo = 520.00, monto;
    int op, clave;

    Iniciar();

    system("cls");
    printf("Registre/ingrese su clave de acceso (4 digitos): ");
    clave = leer_clave_4_digitos();
    (void)clave;
    printf("Clave validada correctamente.\n");

    do {
        printf("\n--- BIENVENIDO AL BANCO NACIONAL (BFC) ---\n");
        printf("1. Consulta de saldo\n");
        printf("2. Retiro\n");
        printf("3. Deposito\n");
        printf("4. Salir\n");
        printf("Ingrese su opcion [1-4]: ");
        op = leer_opcion_menu();

        switch (op) {
        case 1:
            printf("\nSaldo actual: %.2f BsF\n", saldo);
            break;
        case 2:
            if (saldo <= 0.0) {
                printf("\nNo tiene fondos disponibles para retirar.\n");
            } else {
                do {
                    printf("\nIngrese el monto a retirar (max %.2f BsF): ", saldo);
                    monto = leer_monto_pos(MAX_MONTO_DIG);
                    if (monto <= 0.0 || monto > saldo)
                        printf("  Error: monto invalido o fondos insuficientes (saldo: %.2f BsF).\n", saldo);
                } while (monto <= 0.0 || monto > saldo);
                saldo -= monto;
                printf("Retiro exitoso. Nuevo saldo: %.2f BsF\n", saldo);
            }
            break;
        case 3:
            do {
                printf("\nIngrese el monto a depositar (mayor a 0): ");
                monto = leer_monto_pos(MAX_MONTO_DIG);
                if (monto <= 0.0) printf("  Error: el monto debe ser mayor a 0.\n");
            } while (monto <= 0.0);
            saldo += monto;
            printf("Deposito exitoso. Nuevo saldo: %.2f BsF\n", saldo);
            break;
        case 4:
            printf("\nGracias por usar los servicios del Banco Nacional. Hasta luego!\n");
            break;
        }
    } while (op != 4);

    Finalizar();
    return 0;
}
