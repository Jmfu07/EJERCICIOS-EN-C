#define _WIN32_WINNT 0x0601   /* necesario para RegSetKeyValueA y RegDeleteKeyValueA */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>            /* _getch */

/* ============================================================================
 * PLANTILLA MAESTRA UNIFICADA DE VALIDACIONES Y BLOQUEO DE TECLADO (Win32)
 * Compatible con: Dev-C++ (TDM-GCC), Visual Studio Code (MSYS2/MinGW) y MSVC.
 *
 * Incluye:
 *  1. Bloqueo de atajos de Windows (Win+L, Ctrl+Alt+Supr, Alt+Tab, Alt+F4,
 *     Ctrl+C/V/X, F1-F24, Esc, QuickEdit). La X de la consola SI funciona y
 *     restaura todo antes de cerrar. Disenado con tabla compacta sin falsos
 *     positivos del antivirus.
 *  2. Validaciones TECLA POR TECLA (_getch): filtra en vivo mientras escribes
 *     (enteros positivos/negativos/con rango, decimales positivos/negativos/
 *     con rango, solo letras, letras con espacios).
 *  3. Validaciones POR LINEA (fgets): enteros, flotantes, caracteres, cadenas
 *     con rango y mensajes claros.
 *  4. Menus instantaneos sin Enter (por numeros, por letras y S/N).
 * ============================================================================ */

#define PERMITIR_ALTGR 0      /* 1 = deja pasar AltGr (para @ \ | # en teclado espanol) */
#define POL "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\"

/* ---------- 1. BLOQUEO DE TECLADO Y POLITICAS DE WINDOWS ---------- */

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

/* ============================================================================
 * 2. VALIDACIONES TECLA POR TECLA (_getch) - De VALIDACIONES DE TIPO DE DATO
 * ============================================================================
 * Filtra cada tecla en tiempo real:
 *   punto:  0 = sin decimales, 1 = permite un punto o coma decimal
 *   signo:  0 = solo positivo, 1 = '-' opcional, 2 = negativo fijo ('-' inicial)
 *   letras: 0 = numeros, 1 = solo letras (A-Z/a-z), 2 = letras y espacios
 *   max:    maximo de digitos o letras (el '-' y el '.' no cuentan)
 */

#define LIMITE_MAX 64
#define TAM_BUF    (LIMITE_MAX + 4)

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

        if (c == 0 || c == 0xE0) {
            _getch();
            continue;
        }
        if (c == '\r') {
            if (cuenta > 0) break;
            continue;
        }
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

        if (letras == 1) {
            if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) || cuenta >= max)
                continue;
            cuenta++;
        } else if (letras == 2) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                if (cuenta >= max) continue;
                cuenta++;
            } else if (c == ' ') {
                if (len == 0 || buf[len - 1] == ' ' || cuenta >= max) continue;
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
            /* signo negativo opcional al inicio */
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

static long   leer_entero_pos(int max)             { char b[TAM_BUF]; leer_texto(b, 0, 0, 0, max); return strtol(b, NULL, 10); }
static long   leer_entero_neg(int max)             { char b[TAM_BUF]; leer_texto(b, 0, 2, 0, max); return strtol(b, NULL, 10); }
static long   leer_entero_tec(int max)             { char b[TAM_BUF]; leer_texto(b, 0, 1, 0, max); return strtol(b, NULL, 10); }
static double leer_decimal_pos(int max)            { char b[TAM_BUF]; leer_texto(b, 1, 0, 0, max); return strtod(b, NULL); }
static double leer_decimal_neg(int max)            { char b[TAM_BUF]; leer_texto(b, 1, 2, 0, max); return strtod(b, NULL); }
static double leer_decimal_tec(int max)            { char b[TAM_BUF]; leer_texto(b, 1, 1, 0, max); return strtod(b, NULL); }
static void   leer_letras(char *dest, int max)     { leer_texto(dest, 0, 0, 1, max); }
static void   leer_letras_esp(char *dest, int max) { leer_texto(dest, 0, 0, 2, max); }

/* Lectura tecla a tecla con validacion de rango [min, max] */
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
        printf("  Fuera de rango: debe estar entre %ld y %ld.\n", min, max);
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
        printf("  Fuera de rango: debe estar entre %g y %g.\n", min, max);
    }
}

/* ============================================================================
 * 3. VALIDACIONES POR LINEA (fgets) Y MENUS INSTANTANEOS (_getch)
 * ============================================================================ */

#define TAM_LINEA   128
#define CUALQUIERA  0
#define SOLO_LETRAS 1

enum { LINEA_OK, LINEA_LARGA, LINEA_EOF };

static void fin_entrada(void)
{
    printf("\n[Entrada cerrada, saliendo del programa]\n");
    exit(1);
}

static int leer_linea(char buf[], int tam)
{
    int len, c, excedio;
    if (fgets(buf, tam, stdin) == NULL) return LINEA_EOF;
    len = (int)strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
        len--;
        if (len > 0 && buf[len - 1] == '\r') buf[len - 1] = '\0';
        return LINEA_OK;
    }
    excedio = 0;
    c = getchar();
    while (c != '\n' && c != EOF) {
        excedio = 1;
        c = getchar();
    }
    return excedio ? LINEA_LARGA : LINEA_OK;
}

static void recortar(char s[])
{
    int ini = 0, fin = (int)strlen(s), i;
    while (s[ini] != '\0' && isspace((unsigned char)s[ini])) ini++;
    while (fin > ini && isspace((unsigned char)s[fin - 1])) fin--;
    for (i = 0; i < fin - ini; i++) s[i] = s[ini + i];
    s[fin - ini] = '\0';
}

static int obtener_linea(char buf[], int tam)
{
    int r = leer_linea(buf, tam);
    if (r == LINEA_EOF) fin_entrada();
    if (r == LINEA_LARGA) {
        printf("  Entrada demasiado larga. Intenta de nuevo.\n");
        return 0;
    }
    recortar(buf);
    if (buf[0] == '\0') {
        printf("  No puede estar vacio.\n");
        return 0;
    }
    return 1;
}

int leer_entero(const char prompt[], int min, int max)
{
    char buf[TAM_LINEA];
    int i, d, signo, ok, desborde;
    long valor;

    for (;;) {
        printf("%s [%d a %d]: ", prompt, min, max);
        if (!obtener_linea(buf, TAM_LINEA)) continue;

        i = 0;
        signo = 1;
        if (buf[0] == '-') { signo = -1; i = 1; }
        else if (buf[0] == '+') { i = 1; }

        ok = (buf[i] != '\0');
        valor = 0;
        desborde = 0;
        while (ok && buf[i] != '\0') {
            if (!isdigit((unsigned char)buf[i])) {
                ok = 0;
            } else {
                d = buf[i] - '0';
                if (!desborde) {
                    if (valor > (2147483647L - d) / 10) desborde = 1;
                    else valor = valor * 10 + d;
                }
                i++;
            }
        }

        if (!ok) {
            printf("  Debe ser un numero entero (sin decimales ni letras).\n");
            continue;
        }
        if (desborde) {
            printf("  Numero demasiado grande. Debe estar entre %d y %d.\n", min, max);
            continue;
        }
        valor = valor * signo;
        if (valor < min || valor > max) {
            printf("  Fuera de rango: debe estar entre %d y %d.\n", min, max);
            continue;
        }
        return (int)valor;
    }
}

float leer_float(const char prompt[], float min, float max)
{
    char buf[TAM_LINEA];
    int i, ok, ndig, hay_sep, decimales, k, signo;
    char c;
    double mantisa, divisor, valor;
    float resultado;

    for (;;) {
        printf("%s [%g a %g]: ", prompt, (double)min, (double)max);
        if (!obtener_linea(buf, TAM_LINEA)) continue;

        i = 0;
        signo = 1;
        if (buf[0] == '-') { signo = -1; i = 1; }
        else if (buf[0] == '+') { i = 1; }

        ok = 1;
        ndig = 0;
        hay_sep = 0;
        decimales = 0;
        mantisa = 0.0;
        while (ok && buf[i] != '\0') {
            c = buf[i];
            if (isdigit((unsigned char)c)) {
                ndig++;
                mantisa = mantisa * 10.0 + (c - '0');
                if (hay_sep) decimales++;
            } else if ((c == '.' || c == ',') && !hay_sep) {
                hay_sep = 1;
            } else {
                ok = 0;
            }
            i++;
        }

        if (!ok || ndig == 0) {
            printf("  Debe ser un numero (ej: 3.14 o 3,14).\n");
            continue;
        }
        if (ndig > 15) {
            printf("  Demasiados digitos (maximo 15).\n");
            continue;
        }

        divisor = 1.0;
        for (k = 0; k < decimales; k++) divisor = divisor * 10.0;
        valor = (mantisa / divisor) * signo;

        resultado = (float)valor;
        if (resultado < min || resultado > max) {
            printf("  Fuera de rango: debe estar entre %g y %g.\n", (double)min, (double)max);
            continue;
        }
        return resultado;
    }
}

char leer_caracter(const char prompt[], const char permitidos[])
{
    char buf[TAM_LINEA];
    int i, n = (int)strlen(permitidos);

    for (;;) {
        printf("%s ", prompt);
        if (n > 0) {
            printf("[");
            for (i = 0; i < n; i++) {
                if (i > 0) printf("/");
                printf("%c", permitidos[i]);
            }
            printf("]: ");
        } else {
            printf("[1 caracter]: ");
        }
        if (!obtener_linea(buf, TAM_LINEA)) continue;

        if (strlen(buf) != 1) {
            printf("  Escribe un unico caracter.\n");
            continue;
        }
        if (n == 0) return buf[0];

        for (i = 0; i < n; i++) {
            if (toupper((unsigned char)permitidos[i]) == toupper((unsigned char)buf[0]))
                return buf[0];
        }
        printf("  Caracter no permitido.\n");
    }
}

void leer_cadena(const char prompt[], char dest[], int min, int max, int tipo)
{
    char buf[TAM_LINEA];
    int i, len, ok;

    if (max > TAM_LINEA - 1) max = TAM_LINEA - 1;

    for (;;) {
        printf("%s [%d a %d caracteres]: ", prompt, min, max);
        if (!obtener_linea(buf, TAM_LINEA)) continue;

        len = (int)strlen(buf);
        if (len < min) {
            printf("  Minimo %d caracteres.\n", min);
            continue;
        }
        if (len > max) {
            printf("  Maximo %d caracteres.\n", max);
            continue;
        }
        if (tipo == SOLO_LETRAS) {
            ok = 1;
            for (i = 0; i < len; i++) {
                if (!(isalpha((unsigned char)buf[i]) || buf[i] == ' ')) ok = 0;
            }
            if (!ok) {
                printf("  Solo se permiten letras y espacios.\n");
                continue;
            }
        }
        for (i = 0; i <= len; i++) dest[i] = buf[i];
        return;
    }
}

static int leer_tecla(void)
{
    int c = _getch();
    if (c == 0 || c == 224) {
        _getch();
        return 0;
    }
    return c;
}

int leer_opcion_numerica(char min, char max)
{
    int c;
    for (;;) {
        c = leer_tecla();
        if (c >= min && c <= max) {
            printf("%c\n", c);
            return c - '0';
        }
    }
}

char leer_opcion_letras(const char validas[])
{
    int c, i, n = (int)strlen(validas);
    for (;;) {
        c = leer_tecla();
        if (c == 0) continue;
        c = toupper(c);
        for (i = 0; i < n; i++) {
            if (toupper((unsigned char)validas[i]) == c) {
                printf("%c\n", c);
                return (char)c;
            }
        }
    }
}

int desea_repetir(const char pregunta[])
{
    printf("%s (S/N): ", pregunta);
    return leer_opcion_letras("SN") == 'S';
}

/* ============================================================
 *  ZONA DE CONFIGURACION DEL PROGRAMA DE EJEMPLO
 * ============================================================ */

#define OPC_MIN        '0'
#define OPC_MAX        '5'
#define SUMA_MIN       (-1000)
#define SUMA_MAX       1000
#define RADIO_MIN      0.1f
#define RADIO_MAX      1000.0f
#define NOMBRE_MIN     2
#define NOMBRE_MAX     20
#define SUB_LETRAS     "ABV"
#define SUB_CHARS      "SN"
#define SUB_ENTERO_MIN 1
#define SUB_ENTERO_MAX 10

static void mostrar_menu(void)
{
    printf("\n===== MENU PRINCIPAL (PLANTILLA UNIFICADA) =====\n");
    printf(" 1. Sumar dos enteros (por linea con rango)\n");
    printf(" 2. Area de un circulo (float con rango)\n");
    printf(" 3. Saludo (cadena solo letras)\n");
    printf(" 4. Submenu por letras\n");
    printf(" 5. Prueba de lectura tecla a tecla (_getch)\n");
    printf(" 0. Salir\n");
    printf("Elige una opcion [%c a %c]: ", OPC_MIN, OPC_MAX);
}

int main(void)
{
    int opcion;

    Iniciar();

    /* Evita advertencias por funciones de la plantilla no llamadas en el ejemplo */
    (void)leer_entero_pos;
    (void)leer_entero_neg;
    (void)leer_entero_tec;
    (void)leer_decimal_pos;
    (void)leer_decimal_neg;
    (void)leer_decimal_tec;
    (void)leer_letras;
    (void)leer_letras_esp;
    (void)leer_entero_rango;
    (void)leer_decimal_rango;

    do {
        mostrar_menu();
        opcion = leer_opcion_numerica(OPC_MIN, OPC_MAX);

        switch (opcion) {
        case 1: {
            int a, b;
            do {
                a = leer_entero("Primer numero", SUMA_MIN, SUMA_MAX);
                b = leer_entero("Segundo numero", SUMA_MIN, SUMA_MAX);
                printf("Resultado: %d\n", a + b);
            } while (desea_repetir("Desea volver a calcular?"));
            break;
        }
        case 2: {
            float r;
            do {
                r = leer_float("Radio", RADIO_MIN, RADIO_MAX);
                printf("Area: %.2f\n", 3.14159265f * r * r);
            } while (desea_repetir("Desea volver a calcular?"));
            break;
        }
        case 3: {
            char nombre[NOMBRE_MAX + 1];
            do {
                leer_cadena("Tu nombre", nombre, NOMBRE_MIN, NOMBRE_MAX, SOLO_LETRAS);
                printf("Hola, %s!\n", nombre);
            } while (desea_repetir("Desea ingresar otro nombre?"));
            break;
        }
        case 4: {
            char sub, c;
            int n;
            do {
                printf("\n-- Submenu --\n");
                printf(" A) Probar un caracter\n");
                printf(" B) Probar un entero\n");
                printf(" V) Volver\n");
                printf("Opcion [A/B/V]: ");
                sub = leer_opcion_letras(SUB_LETRAS);
                switch (sub) {
                case 'A':
                    c = leer_caracter("Escribe una letra", SUB_CHARS);
                    printf("Elegiste: %c\n", c);
                    break;
                case 'B':
                    n = leer_entero("Entero", SUB_ENTERO_MIN, SUB_ENTERO_MAX);
                    printf("Elegiste: %d\n", n);
                    break;
                default:
                    break;
                }
            } while (sub != 'V');
            break;
        }
        case 5: {
            char palabra[TAM_BUF];
            long ent;
            double dec;
            do {
                ent = leer_entero_rango("Entero tecla a tecla", -500, 500, 4);
                dec = leer_decimal_rango("Decimal tecla a tecla", 0.1, 999.9, 6);
                printf("Palabra (solo letras, max 12): ");
                leer_letras_esp(palabra, 12);
                printf("Leido -> Entero: %ld | Decimal: %g | Texto: %s\n", ent, dec, palabra);
            } while (desea_repetir("Desea probar otra vez?"));
            break;
        }
        case 0:
            printf("Hasta luego.\n");
            Finalizar();
            exit(0);
            break;
        }
    } while (opcion != 0);

    return 0;
}
