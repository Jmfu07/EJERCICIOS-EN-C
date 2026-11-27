/* 
 * JUAN MEDINA V-31708027
 *
 * Ejercicio 2: leer un vector de 10 numeros reales, calcular el promedio
 * e indicar cuantos elementos son mayores que el promedio y cuantos son
 * menores o iguales. Cada numero admite como maximo 6 caracteres al
 * escribirlo. Incluye bloqueo completo de atajos (Win+L, Ctrl+Alt+Supr,
 * Ctrl+C/V, Esc, Alt+F4). Se sale con la opcion 0.
 */
#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N          10
#define MAX_CARACT 6
#define PERMITIR_ALTGR 1
#define DOWN(vk) ((GetAsyncKeyState(vk) & 0x8000) != 0)
#define POL "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\"

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

static double leer_real(void)
{
    char buf[MAX_CARACT + 1];
    int len = 0, hay_punto = 0, hay_digito = 0, i;

    for (;;) {
        int c = _getch();

        if (c == 0 || c == 0xE0) {
            _getch();
            continue;
        }
        if (c == '\r') {
            if (hay_digito) break;
            continue;
        }
        if (c == '\b') {
            if (len > 0) {
                len--;
                if (buf[len] == '.') hay_punto = 0;
                hay_digito = 0;
                for (i = 0; i < len; i++)
                    if (buf[i] >= '0' && buf[i] <= '9') hay_digito = 1;
                printf("\b \b");
            }
            continue;
        }

        if (len >= MAX_CARACT)
            continue;

        if (c >= '0' && c <= '9') {
            hay_digito = 1;
        } else if ((c == '.' || c == ',') && !hay_punto) {
            c = '.';
            hay_punto = 1;
        } else if (c == '-' && len == 0) {
            /* signo negativo solo al inicio */
        } else {
            continue;
        }

        buf[len++] = (char)c;
        putchar(c);
    }

    buf[len] = '\0';
    putchar('\n');
    return strtod(buf, NULL);
}

int main(void)
{
    int opcion, i;
    double v[N], suma, promedio;
    int mayores, menores_iguales, repetir;
    int r;

    Iniciar();

    printf("Bloqueo activo: Win+L, Ctrl+Alt+Supr, Ctrl+C/V, Esc, etc. deshabilitados.\n");

    do {
        printf("\n===== VECTOR DE %d NUMEROS REALES =====\n", N);
        printf("1. Ingresar vector y calcular\n");
        printf("0. Salir\n");
        printf("Opcion: ");

        do {
            r = _getch();
            if (r == 0 || r == 0xE0) { _getch(); r = -1; }
        } while (r != '0' && r != '1');
        printf("%c\n", r);
        opcion = r - '0';

        switch (opcion) {
        case 1:
            do {
                suma = 0.0;
                mayores = 0;
                menores_iguales = 0;

                printf("Escribe %d numeros reales (maximo %d caracteres cada uno):\n", N, MAX_CARACT);
                for (i = 0; i < N; i++) {
                    printf("Elemento %d: ", i + 1);
                    v[i] = leer_real();
                    suma += v[i];
                }

                promedio = suma / N;

                for (i = 0; i < N; i++) {
                    if (v[i] > promedio)
                        mayores++;
                    else
                        menores_iguales++;
                }

                printf("\nPromedio: %.2f\n", promedio);
                printf("Mayores que el promedio: %d\n", mayores);
                printf("Menores o iguales al promedio: %d\n", menores_iguales);

                printf("\nDesea volver a ingresar %d numeros reales? (S/N): ", N);
                do {
                    r = _getch();
                    if (r == 0 || r == 0xE0) { _getch(); r = -1; }
                } while (r != 's' && r != 'S' && r != 'n' && r != 'N');
                printf("%c\n", r);
                repetir = (r == 's' || r == 'S');
            } while (repetir);
            break;
        case 0:
            printf("Saliendo...\n");
            Finalizar();
            break;
        }
    } while (opcion != 0);

    return 0;
}
