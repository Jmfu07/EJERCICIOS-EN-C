#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/*
 * ELABORADO POR JUAN MEDINA V-31708027
 *
 * INGRESAR UNA OPCION NUMERICA PARA REALIZAR LAS SIGUIENTES ACTIVIDADES EN LENGUAJE C:
 * 1- INGRESE POR TECLADO UN VALOR NUMERICO DE A Y CALCULE EL VALOR DE K CUANDO K ES = A LA RAIZ CUADRADA DE A
 * 2- INGRESE POR TECLADO 3 VALORES A, B, C, INTERCAMBIE EL VALOR DE B EN A, DE C EN B, Y A EN C
 * 3- INGRESE POR TECLADO NOMBRE, EDAD, SEXO Y PROMEDIO DE UN ALUMNO. IMPRIMA EL NOMBRE Y EL SEXO SI EL ALUMNO
 *    ESTA APROBADO, EN CASO CONTRARIO IMPRIMA UN MENSAJE DE REPROBADO
 * 4- VALIDAR TODAS LAS OPCIONES E INCLUSO EN CASO INCORRECTO
 * 5- IMPRIMIR LOS RESULTADOS DE CADA ACTIVIDAD
 */

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
#define LIMITE_MAX 32
#define TAM_BUF    (LIMITE_MAX + 4)


/* ---------- Validaciones ---------- */

static void leer_texto(char *buf, int punto, int signo, int letras, int max)
{
    int len = 0, hay_punto = 0, cuenta = 0, i;
    int minimo = (signo == 2) ? 1 : 0;

    if (max > LIMITE_MAX) max = LIMITE_MAX;
    if (max < 1) max = 1;

    if (signo == 2) {
        buf[len++] = '-';
        putchar('-');
    }

    for (;;) {
        int c = _getch();

        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (cuenta > 0) break; continue; }
        if (c == '\b') {
            if (len > minimo) {
                len--;
                if (buf[len] == '.') hay_punto = 0;
                cuenta = 0;
                for (i = 0; i < len; i++)
                    if (buf[i] != '-' && buf[i] != '.') cuenta++;
                printf("\b \b");
            }
            continue;
        }

        if (letras) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                if (cuenta >= max) continue;
                cuenta++;
            } else if (c == ' ' && letras == 2 && len > 0 && buf[len - 1] != ' ' && cuenta < max) {
                cuenta++;
            } else {
                continue;
            }
        } else if (c >= '0' && c <= '9') {
            if (cuenta >= max) continue;
            cuenta++;
        } else if ((c == '.' || c == ',') && punto && !hay_punto) {
            c = '.';
            hay_punto = 1;
        } else if (c == '-' && signo == 1 && len == 0) {
            /* signo negativo opcional */
        } else {
            continue;
        }

        buf[len++] = (char)c;
        putchar(c);
    }

    while (len > 0 && buf[len - 1] == ' ') len--;
    buf[len] = '\0';
    putchar('\n');
}

static long leer_entero_rango(const char *prompt, long min, long max, int max_dig)
{
    long v;
    int signo = (min < 0 && max < 0) ? 2 : (min < 0 ? 1 : 0);
    char b[TAM_BUF];
    for (;;) {
        printf("%s [%ld a %ld]: ", prompt, min, max);
        leer_texto(b, 0, signo, 0, max_dig);
        v = strtol(b, NULL, 10);
        if (v >= min && v <= max) return v;
        printf("  [ERROR] Fuera de rango (%ld a %ld).\n", min, max);
    }
}

static double leer_decimal_rango(const char *prompt, double min, double max, int max_dig)
{
    double v;
    int signo = (min < 0.0 && max < 0.0) ? 2 : (min < 0.0 ? 1 : 0);
    char b[TAM_BUF];
    for (;;) {
        printf("%s [%g a %g]: ", prompt, min, max);
        leer_texto(b, 1, signo, 0, max_dig);
        v = strtod(b, NULL);
        if (v >= min && v <= max) return v;
        printf("  [ERROR] Fuera de rango (%g a %g).\n", min, max);
    }
}

static char leer_opcion_letras(const char *validas)
{
    int c, i, n = (int)strlen(validas);
    for (;;) {
        c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        c = toupper(c);
        for (i = 0; i < n; i++) {
            if (toupper((unsigned char)validas[i]) == c) {
                printf("%c\n", c);
                return (char)c;
            }
        }
    }
}

int main(void)
{
    int op, edad, r;
    double a, b, c, k, prom, aux;
    char nombre[30], sexo;

    Iniciar();

    do {
        printf("\n----- MENU LABORATORIO DE LENGUAJES -----\n");
        printf("1- CALCULO DE RAIZ CUADRADA\n");
        printf("2- INTERCAMBIO DE A, B Y C\n");
        printf("3- APROBADOS Y REPROBADOS\n");
        printf("4- SALIR\n");
        printf("----- Seleccione una opcion [1-4]: ");

        do {
            r = _getch();
            if (r == 0 || r == 0xE0) { _getch(); r = -1; }
        } while (r < '1' || r > '4');
        printf("%c\n", r);
        op = r - '0';

        if (op == 1) {
            a = leer_decimal_rango("INGRESE EL NUMERO A PARA CALCULAR SU RAIZ CUADRADA", 0.0, 1000000.0, 8);
            k = sqrt(a);
            system("cls");
            printf("\nLa raiz cuadrada de %.2f es %.2f\n", a, k);
        } else if (op == 2) {
            a = leer_decimal_rango("INGRESE UN VALOR PARA A", -100000.0, 100000.0, 8);
            b = leer_decimal_rango("INGRESE UN VALOR PARA B", -100000.0, 100000.0, 8);
            c = leer_decimal_rango("INGRESE UN VALOR PARA C", -100000.0, 100000.0, 8);

            system("cls");
            printf("A VALIA EN EL INICIO: %.2f\n", a);
            printf("B VALIA EN EL INICIO: %.2f\n", b);
            printf("C VALIA EN EL INICIO: %.2f\n", c);

            aux = a;
            a = b;
            b = c;
            c = aux;

            printf("\nDESPUES DEL INTERCAMBIO:\n");
            printf("A AHORA VALE = %.2f\n", a);
            printf("B AHORA VALE = %.2f\n", b);
            printf("C AHORA VALE = %.2f\n", c);
        } else if (op == 3) {
            printf("INGRESE EL NOMBRE DEL ALUMNO (solo letras): ");
            leer_texto(nombre, 0, 0, 2, 25);
            printf("Nombre guardado exitosamente: %s\n", nombre);

            edad = (int)leer_entero_rango("INGRESE LA EDAD DEL ALUMNO", 1, 99, 2);

            printf("INGRESE EL SEXO DEL ALUMNO (M = Masculino / F = Femenino): ");
            sexo = leer_opcion_letras("MF");

            prom = leer_decimal_rango("INGRESE EL PROMEDIO DEL ALUMNO", 0.0, 20.0, 5);

            system("cls");
            printf("\n================ RESULTADO ================\n");
            if (prom >= 9.5) {
                printf("ALUMNO APROBADO\n");
                printf("Nombre:   %s\n", nombre);
                printf("Edad:     %d\n", edad);
                printf("Sexo:     %s\n", (sexo == 'M') ? "Masculino" : "Femenino");
                printf("Promedio: %.2f\n", prom);
            } else {
                printf("EL ALUMNO ESTA REPROBADO (Promedio insuficiente: %.2f)\n", prom);
            }
            printf("===========================================\n");
        }
    } while (op != 4);

    printf("Saliendo del programa...\n");
    Finalizar();
    return 0;
}
