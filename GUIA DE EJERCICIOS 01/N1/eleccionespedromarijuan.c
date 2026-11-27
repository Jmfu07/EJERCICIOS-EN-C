#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ELABORADO POR JUAN MEDINA 07/08/2026
 * 1.- En una eleccion realizada en un municipio, se presentaron tres candidatos: Pedro, Maria y Juan.
 * Por cada votante se elaboro un registro asi: Cedula de Identidad y el valor del voto.
 *   - 1 = Pedro, 2 = Maria, 3 = Juan, cualquier otro digito = Voto nulo.
 * Se pide: a) Votos de cada candidato, b) Votos nulos, c) Quien gano las elecciones.
 * Utilizar el ciclo Haga-Mientras con respuesta al operador.
 */

#define PERMITIR_ALTGR 1
#define MAX_VOTANTES   1000
#define MAX_CED_MIN    6
#define MAX_CED_MAX    8
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

static long leer_digitos_pos(int min_dig, int max_dig) {
    char buf[16]; int len = 0;
    for (;;) {
        int c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (len >= min_dig) break; continue; }
        if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
        if (c >= '0' && c <= '9' && len < max_dig) { buf[len++] = (char)c; putchar(c); }
    }
    buf[len] = '\0'; putchar('\n');
    return strtol(buf, NULL, 10);
}

static int leer_sn(void) {
    for (;;) {
        int c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == 's' || c == 'S') { printf("S\n"); return 1; }
        if (c == 'n' || c == 'N') { printf("N\n"); return 0; }
    }
}

static int cedula_repetida(long cedula, const long *cedulas, int n) {
    int i;
    for (i = 0; i < n; i++)
        if (cedulas[i] == cedula) return 1;
    return 0;
}

static void mostrar_resultados(int pedro, int maria, int juan, int nulo) {
    int maxv = pedro;
    if (maria > maxv) maxv = maria;
    if (juan > maxv)  maxv = juan;

    printf("--------------------------------------------------------\n");
    printf("            RESULTADOS DE LAS ELECCIONES                \n");
    printf("--------------------------------------------------------\n");
    printf("- Votos para Pedro: %d\n", pedro);
    printf("- Votos para Maria: %d\n", maria);
    printf("- Votos para Juan:  %d\n", juan);
    printf("- Votos nulos:      %d\n", nulo);
    printf("--------------------------------------------------------\n");

    if (maxv == 0) {
        printf("- No hubo votos validos (todos fueron nulos).\n");
    } else if (pedro == maxv && maria == maxv && juan == maxv) {
        printf("- Triple empate entre Pedro, Maria y Juan (%d votos).\n", maxv);
    } else if (pedro == maxv && maria == maxv) {
        printf("- Empate entre Pedro y Maria (%d votos).\n", maxv);
    } else if (pedro == maxv && juan == maxv) {
        printf("- Empate entre Pedro y Juan (%d votos).\n", maxv);
    } else if (maria == maxv && juan == maxv) {
        printf("- Empate entre Maria y Juan (%d votos).\n", maxv);
    } else if (pedro == maxv) {
        printf("- Ganador: Pedro (%d votos).\n", pedro);
    } else if (maria == maxv) {
        printf("- Ganador: Maria (%d votos).\n", maria);
    } else {
        printf("- Ganador: Juan (%d votos).\n", juan);
    }
    printf("--------------------------------------------------------\n");
}

int main(void) {
    long cedula, cedulas[MAX_VOTANTES];
    int voto, pedro = 0, maria = 0, juan = 0, nulo = 0, num_votantes = 0, continuar;

    Iniciar();

    do {
        system("cls");
        printf("--------------------------------------------------------\n");
        printf("          SISTEMA DE VOTACION MUNICIPAL                 \n");
        printf("--------------------------------------------------------\n");

        for (;;) {
            printf("Ingrese su cedula (100000 a 39999999): ");
            cedula = leer_digitos_pos(MAX_CED_MIN, MAX_CED_MAX);
            if (cedula < 100000L || cedula >= 40000000L) {
                printf("  Error: Cedula fuera de rango (debe ser entre 100000 y 39999999).\n");
                continue;
            }
            if (cedula_repetida(cedula, cedulas, num_votantes)) {
                printf("  Error: La cedula %ld ya voto.\n", cedula);
                continue;
            }
            break;
        }
        cedulas[num_votantes++] = cedula;

        printf("\nPOR QUE CANDIDATO VOTA?\n");
        printf("  1. Pedro\n  2. Maria\n  3. Juan\n  Otro digito (0, 4..9) = Voto Nulo\n");
        printf("Ingrese su voto (1 digito): ");
        voto = (int)leer_digitos_pos(1, 1);

        switch (voto) {
        case 1: pedro++; printf(">> Voto registrado para Pedro.\n"); break;
        case 2: maria++; printf(">> Voto registrado para Maria.\n"); break;
        case 3: juan++;  printf(">> Voto registrado para Juan.\n");  break;
        default: nulo++; printf(">> Voto NULO registrado.\n");       break;
        }

        if (num_votantes >= MAX_VOTANTES) {
            printf("\nLimite maximo de %d votantes alcanzado.\n", MAX_VOTANTES);
            continuar = 0;
        } else {
            printf("\nDesea registrar otro votante? (S/N): ");
            continuar = leer_sn();
        }
    } while (continuar);

    system("cls");
    mostrar_resultados(pedro, maria, juan, nulo);
    printf("\nPresione cualquier tecla para salir...");
    _getch();

    Finalizar();
    return 0;
}
