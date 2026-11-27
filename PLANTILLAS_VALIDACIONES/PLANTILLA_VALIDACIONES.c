/* ============================================================================
 * PLANTILLA UNIFICADA: BLOQUEO DE TECLADO + VALIDACIONES TECLA POR TECLA
 * Compilar: gcc PLANTILLA_VALIDACIONES.c -o PLANTILLA_VALIDACIONES.exe
 * Compatible con: Dev-C++ (TDM-GCC), VS Code (MinGW/MSYS2) y MSVC.
 * ============================================================================ */

#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <conio.h>

/* ==================== CONFIGURACION (edita estos #define) ===================
 * En numeros, '-' y '.' / ',' NO cuentan como digito (ej: 6 -> maximo 999999) */
#define PERMITIR_ALTGR 1  /* 1 = permite AltGr (@ \ | # { } [ ] ~ en espanol) */

#define MAX_ENT_POS    6  /* entero positivo: hasta 999999 */
#define MAX_ENT_NEG    6  /* entero negativo fijo: hasta -999999 */
#define MAX_ENT        6  /* entero con signo opcional: -999999 a 999999 */
#define MAX_DEC_POS    6  /* decimal positivo (6 digitos + '.' o ',' aparte) */
#define MAX_DEC_NEG    6  /* decimal negativo fijo (6 digitos + '-' y '.') */
#define MAX_DEC        6  /* decimal con signo opcional (6 digitos + '-' y '.') */
#define MAX_LETRAS     15 /* solo letras (A-Z / a-z) */
#define MAX_TEXTO      30 /* letras y espacios */
#define MAX_CUALQ      30 /* letras, numeros y simbolos imprimibles */

#define LIM_MAX        64
#define TAM_BUF        (LIM_MAX + 4)
#define ES_LETRA(c)    (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define DOWN(vk)       ((GetAsyncKeyState(vk) & 0x8000) != 0)
#define POL            "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\"

enum { M_ENT_POS, M_ENT_NEG, M_ENT, M_DEC_POS, M_DEC_NEG, M_DEC, M_LETRAS, M_TEXTO, M_CUALQ };

/* ==================== 1. BLOQUEO DE TECLADO Y POLITICAS ===================== */

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

void Finalizar(void) {
    if (!g_th) return;
    PostThreadMessage(g_hilo, WM_QUIT, 0, 0);
    WaitForSingleObject(g_th, 8000);
    CloseHandle(g_th); g_th = NULL;
}

void Iniciar(void) {
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

/* ==================== 2. VALIDACIONES TECLA POR TECLA ======================= */

static int leer_tecla(void) {
    int c = _getch();
    if (c == 0 || c == 0xE0) { _getch(); return 0; }
    return c;
}

static int en_lista(const char *s, int c) {
    for (; s && *s; s++)
        if (toupper((unsigned char)*s) == toupper((unsigned char)c)) return 1;
    return 0;
}

static void leer_tec(char *buf, int modo, int min, int max) {
    int len = 0, cuenta = 0, hay_pto = 0;
    int es_num = (modo <= M_DEC), con_pto = (modo >= M_DEC_POS && modo <= M_DEC);
    int signo = es_num ? (modo % 3) : 0, fijo = (signo == 1);

    if (max > LIM_MAX) max = LIM_MAX;
    if (max < 1) max = 1;
    if (min < 1) min = 1;
    if (min > max) min = max;
    if (fijo) { buf[len++] = '-'; putchar('-'); }

    for (;;) {
        int c = leer_tecla();
        if (c == 0) continue;
        if (c == '\r') {
            int eff = cuenta, t = len;
            if (modo >= M_TEXTO) while (t > 0 && buf[t - 1] == ' ') { t--; eff--; }
            if (eff >= min) break;
            continue;
        }
        if (c == '\b') {
            if (len > fijo) {
                len--;
                if (es_num && buf[len] == '.') hay_pto = 0;
                else if (!(es_num && buf[len] == '-')) cuenta--;
                printf("\b \b");
            }
            continue;
        }

        if (es_num) {
            if (c >= '0' && c <= '9') { if (cuenta >= max) continue; cuenta++; }
            else if ((c == '.' || c == ',') && con_pto && !hay_pto && cuenta < max) { c = '.'; hay_pto = 1; }
            else if (c == '-' && signo == 2 && len == 0) { /* '-' opcional al inicio, no cuenta */ }
            else continue;
        } else if (modo == M_LETRAS) {
            if (!ES_LETRA(c) || cuenta >= max) continue;
            cuenta++;
        } else if (modo == M_TEXTO) {
            if (ES_LETRA(c)) { if (cuenta >= max) continue; }
            else if (c == ' ') { if (len == 0 || buf[len - 1] == ' ' || cuenta >= max) continue; }
            else continue;
            cuenta++;
        } else {
            if (c < 32 || c > 126 || (c == ' ' && len == 0) || cuenta >= max) continue;
            cuenta++;
        }
        buf[len++] = (char)c;
        putchar(c);
    }

    if (modo >= M_TEXTO) while (len > 0 && buf[len - 1] == ' ') len--;
    buf[len] = '\0';
    putchar('\n');
}

long   leer_ent(int modo, int max) { char b[TAM_BUF]; leer_tec(b, modo, 1, max); return strtol(b, NULL, 10); }
double leer_dec(int modo, int max) { char b[TAM_BUF]; leer_tec(b, modo, 1, max); return strtod(b, NULL); }

#define leer_ent_pos(max)       leer_ent(M_ENT_POS, (max))
#define leer_ent_neg(max)       leer_ent(M_ENT_NEG, (max))
#define leer_ent_sig(max)       leer_ent(M_ENT,     (max))
#define leer_dec_pos(max)       leer_dec(M_DEC_POS, (max))
#define leer_dec_neg(max)       leer_dec(M_DEC_NEG, (max))
#define leer_dec_sig(max)       leer_dec(M_DEC,     (max))
#define leer_letras(dest, max)  leer_tec((dest), M_LETRAS, 1, (max))
#define leer_texto(dest, max)   leer_tec((dest), M_TEXTO,  1, (max))

long leer_entero(const char *prompt, long min, long max, int max_dig) {
    char b[TAM_BUF];
    int modo = (min < 0 && max < 0) ? M_ENT_NEG : (min < 0 ? M_ENT : M_ENT_POS);
    for (;;) {
        long v;
        printf("%s [%ld a %ld]: ", prompt, min, max);
        leer_tec(b, modo, 1, max_dig);
        errno = 0; v = strtol(b, NULL, 10);
        if (errno == 0 && v >= min && v <= max) return v;
        printf("  Fuera de rango (%ld a %ld).\n", min, max);
    }
}

double leer_decimal(const char *prompt, double min, double max, int max_dig) {
    char b[TAM_BUF];
    int modo = (min < 0.0 && max < 0.0) ? M_DEC_NEG : (min < 0.0 ? M_DEC : M_DEC_POS);
    for (;;) {
        double v;
        printf("%s [%g a %g]: ", prompt, min, max);
        leer_tec(b, modo, 1, max_dig);
        errno = 0; v = strtod(b, NULL);
        if (errno == 0 && v >= min && v <= max) return v;
        printf("  Fuera de rango (%g a %g).\n", min, max);
    }
}

void leer_cadena(const char *prompt, char *dest, int min, int max, int modo) {
    printf("%s [%d a %d caracteres]: ", prompt, min, max);
    leer_tec(dest, modo, min, max);
}

char leer_caracter(const char *prompt, const char *permitidos) {
    int c = 0, hay = 0; const char *p;
    printf("%s [", prompt);
    if (permitidos && *permitidos)
        for (p = permitidos; *p; p++) printf("%s%c", p == permitidos ? "" : "/", *p);
    else printf("1 caracter");
    printf("]: ");
    for (;;) {
        int k = leer_tecla();
        if (k == '\r' && hay) { putchar('\n'); return (char)c; }
        if (k == '\b' && hay) { hay = 0; printf("\b \b"); continue; }
        if (!hay && k >= 32 && k <= 126 && (!permitidos || !*permitidos || en_lista(permitidos, k))) {
            c = k; hay = 1; putchar(c);
        }
    }
}

int leer_opcion_num(char min, char max) {
    for (;;) {
        int c = leer_tecla();
        if (c >= min && c <= max) { printf("%c\n", c); return c - '0'; }
    }
}

char leer_opcion_letra(const char *validas) {
    for (;;) {
        int c = leer_tecla();
        if (c && en_lista(validas, c)) { c = toupper((unsigned char)c); printf("%c\n", c); return (char)c; }
    }
}

int desea_repetir(const char *pregunta) {
    printf("%s (S/N): ", pregunta);
    return leer_opcion_letra("SN") == 'S';
}

/* ==================== 3. PROGRAMA DE PRUEBA ================================= */

int main(void) {
    char cad[TAM_BUF], ch;
    long ent;
    double dec;
    int opc;

    Iniciar();

    do {
        printf("\n=== MENU DE PRUEBAS (Bloqueo activo, simbolos permitidos) ===\n");
        printf("1) Enteros (pos / neg / con signo / con rango)\n");
        printf("2) Decimales (pos / neg / con signo / con rango)\n");
        printf("3) Cadenas (solo letras / letras+espacios / con simbolos)\n");
        printf("4) Caracter permitido y menu de letras (A/B/V)\n");
        printf("0) Salir y restaurar teclado\n");
        printf("Opcion: ");
        opc = leer_opcion_num('0', '4');

        if (opc == 1) {
            printf("Entero positivo (max %d dig): ", MAX_ENT_POS);
            ent = leer_ent_pos(MAX_ENT_POS); printf("  -> %ld\n", ent);

            printf("Entero negativo fijo (max %d dig): ", MAX_ENT_NEG);
            ent = leer_ent_neg(MAX_ENT_NEG); printf("  -> %ld\n", ent);

            printf("Entero con signo (max %d dig): ", MAX_ENT);
            ent = leer_ent_sig(MAX_ENT); printf("  -> %ld\n", ent);

            ent = leer_entero("Entero con rango", -1000, 1000, MAX_ENT);
            printf("  -> %ld\n", ent);
        } else if (opc == 2) {
            printf("Decimal positivo (max %d dig): ", MAX_DEC_POS);
            dec = leer_dec_pos(MAX_DEC_POS); printf("  -> %g\n", dec);

            printf("Decimal negativo fijo (max %d dig): ", MAX_DEC_NEG);
            dec = leer_dec_neg(MAX_DEC_NEG); printf("  -> %g\n", dec);

            printf("Decimal con signo (max %d dig): ", MAX_DEC);
            dec = leer_dec_sig(MAX_DEC); printf("  -> %g\n", dec);

            dec = leer_decimal("Decimal con rango", -999.99, 999.99, MAX_DEC);
            printf("  -> %g\n", dec);
        } else if (opc == 3) {
            printf("Solo letras (max %d): ", MAX_LETRAS);
            leer_letras(cad, MAX_LETRAS); printf("  -> \"%s\"\n", cad);

            leer_cadena("Letras y espacios", cad, 2, MAX_TEXTO, M_TEXTO);
            printf("  -> \"%s\"\n", cad);

            leer_cadena("Texto libre (prueba simbolos @ # $ %% / + -)", cad, 1, MAX_CUALQ, M_CUALQ);
            printf("  -> \"%s\"\n", cad);
        } else if (opc == 4) {
            ch = leer_caracter("Caracter permitido", "SN");
            printf("  -> '%c'\n", ch);

            printf("Elige letra instantanea (A/B/V): ");
            ch = leer_opcion_letra("ABV");
            printf("  -> '%c'\n", ch);
        }
    } while (opc != 0 && desea_repetir("Volver al menu?"));

    Finalizar();
    return 0;
}
