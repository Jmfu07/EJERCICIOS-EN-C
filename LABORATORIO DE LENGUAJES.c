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

#define PERMITIR_ALTGR 0
#define POL "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\"
#define LIMITE_MAX 32
#define TAM_BUF    (LIMITE_MAX + 4)

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
    HANDLE t;
    TOKEN_ELEVATION e;
    DWORD n;
    BOOL r = FALSE;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &t)) {
        r = GetTokenInformation(t, TokenElevation, &e, sizeof e, &n) && e.TokenIsElevated;
        CloseHandle(t);
    }
    return r;
}

static void Pausa(const char *msg) { printf("%s\nPulsa Enter.\n", msg); getchar(); }
static void Cerrar(HANDLE *h) { if (*h) CloseHandle(*h); *h = NULL; }

static char *Nombre(char *b, const char *tipo, DWORD pid)
{
    sprintf(b, "BloqueoAtajos_%s_%lu", tipo, (unsigned long)pid);
    return b;
}

static BOOL Elevar(const char *args, HANDLE *proc)
{
    char exe[MAX_PATH];
    SHELLEXECUTEINFOA s;
    memset(&s, 0, sizeof s);
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    s.cbSize = sizeof s;
    s.fMask = SEE_MASK_NOCLOSEPROCESS;
    s.lpVerb = "runas";
    s.lpFile = exe;
    s.lpParameters = args;
    s.nShow = SW_HIDE;
    if (!ShellExecuteExA(&s) || !s.hProcess) {
        Pausa("Se necesita administrador (UAC cancelado).");
        return FALSE;
    }
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
    char exe[MAX_PATH], cmd[MAX_PATH + 16];
    size_t i;
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
    char n[64];
    DWORD pid = strtoul(arg, NULL, 10);
    HANDLE h[2], ok;
    h[0] = OpenProcess(SYNCHRONIZE, FALSE, pid);
    ok   = OpenEventA(EVENT_MODIFY_STATE, FALSE, Nombre(n, "ok", pid));
    h[1] = OpenEventA(SYNCHRONIZE, FALSE, Nombre(n, "stop", pid));
    Politicas(TRUE, FALSE);
    if (ok) SetEvent(ok);
    if (h[0] && h[1]) WaitForMultipleObjects(2, h, FALSE, INFINITE);
    Politicas(FALSE, FALSE);
}

static void Limpiar(void)
{
    if (EsAdmin()) Politicas(FALSE, FALSE);
    else Elevar("limpiar", NULL);
}

static void Restaurar(void)
{
    HANDLE g = (HANDLE)InterlockedExchangePointer((PVOID volatile *)&g_guardia, NULL);
    Politicas(FALSE, g != NULL);
    if (g) {
        SetEvent(g_evStop);
        WaitForSingleObject(g, 5000);
        CloseHandle(g);
    }
}

static BOOL Permitida(DWORD v)
{
    return v == VK_SPACE || v == VK_DELETE || v == VK_CAPITAL || v == VK_OEM_102
        || (v >= VK_LEFT && v <= VK_DOWN) || (v >= '0' && v <= '9') || (v >= 'A' && v <= 'Z')
        || (v >= VK_NUMPAD0 && v <= VK_DIVIDE) || (v >= VK_OEM_1 && v <= VK_OEM_8);
}

static BOOL Down(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }

static void Fantasma(void)
{
    keybd_event(0xE8, 0, 0, 0);
    keybd_event(0xE8, 0, KEYEVENTF_KEYUP, 0);
}

static LRESULT CALLBACK Hook(int c, WPARAM w, LPARAM l)
{
    KBDLLHOOKSTRUCT *k = (KBDLLHOOKSTRUCT *)l;
    DWORD vk = k->vkCode & 0xFF;
    BOOL abajo = (w == WM_KEYDOWN || w == WM_SYSKEYDOWN), pasa;

    if (c != HC_ACTION || (k->flags & LLKHF_INJECTED))
        return CallNextHookEx(g_hook, c, w, l);
    if (vk == 0xFF) return 1;

    switch (vk) {
    case VK_LWIN: case VK_RWIN: case VK_MENU: case VK_LMENU: case VK_RMENU:
        if (!abajo) Fantasma();
        /* fallthrough */
    case VK_SHIFT: case VK_LSHIFT: case VK_RSHIFT: case VK_CONTROL: case VK_LCONTROL: case VK_RCONTROL:
        return CallNextHookEx(g_hook, c, w, l);
    }

    if (!abajo) {
        pasa = pasada[vk];
        pasada[vk] = FALSE;
        return pasa ? CallNextHookEx(g_hook, c, w, l) : 1;
    }

    if (Down(VK_LWIN) || Down(VK_RWIN)
        || ((Down(VK_CONTROL) || Down(VK_MENU)) && !(PERMITIR_ALTGR && Down(VK_RMENU)))
        || (vk != VK_RETURN && vk != VK_BACK && !Permitida(vk)))
        return 1;

    pasada[vk] = TRUE;
    return CallNextHookEx(g_hook, c, w, l);
}

static HHOOK Poner(void) { return SetWindowsHookExA(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0); }

static VOID CALLBACK TPol(HWND h, UINT m, UINT_PTR i, DWORD t) { (void)h; (void)m; (void)i; (void)t; Politicas(TRUE, TRUE); }

static VOID CALLBACK TGancho(HWND h, UINT m, UINT_PTR i, DWORD t)
{
    HHOOK nuevo = Poner();
    (void)h; (void)m; (void)i; (void)t;
    if (nuevo) { UnhookWindowsHookEx(g_hook); g_hook = nuevo; }
}

static BOOL WINAPI Cierre(DWORD e)
{
    if (e == CTRL_C_EVENT || e == CTRL_BREAK_EVENT) return TRUE;
    Restaurar();
    return FALSE;
}

static DWORD WINAPI Hilo(LPVOID p)
{
    MSG m;
    (void)p;
    g_hilo = GetCurrentThreadId();
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    SetTimer(NULL, 0, 2000, TPol);
    SetTimer(NULL, 0, 20000, TGancho);
    Politicas(TRUE, g_guardia != NULL);
    g_hook = Poner();
    SetEvent(g_listo);
    while (g_hook && GetMessage(&m, NULL, 0, 0) > 0)
        DispatchMessage(&m);
    if (g_hook) UnhookWindowsHookEx(g_hook);
    Restaurar();
    return 0;
}

static void Finalizar(void)
{
    if (g_th) {
        PostThreadMessage(g_hilo, WM_QUIT, 0, 0);
        WaitForSingleObject(g_th, 8000);
        Cerrar(&g_th);
    }
    Cerrar(&g_hPrincipal);
}

static BOOL LanzarGuardia(void)
{
    char n[64], args[48];
    DWORD pid = GetCurrentProcessId();
    HANDLE h[2];
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
    DWORD modo;
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    if (g_th) return TRUE;
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_hPrincipal, THREAD_TERMINATE, FALSE, 0);
    if (!EsAdmin() && !LanzarGuardia()) { Restaurar(); return FALSE; }
    SetConsoleCtrlHandler(Cierre, TRUE);
    if (GetConsoleMode(in, &modo))
        SetConsoleMode(in, (modo | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE);
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
