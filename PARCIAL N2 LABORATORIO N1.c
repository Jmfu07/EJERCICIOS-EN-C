/*
 * EJERCICIO N1 LABORATORIO - PARCIAL N2
 * JUAN MEDINA V-31708027
 * ELABORADO 12-11-2026
 *
 * - Elaborar el programa en lenguaje C que pinte un tablero de ajedrez, los peones con la letra P,
 *   las torres con T, los caballos con C, los alfiles con A, el rey con R y la reina con M.
 *
 * Se pide:
 * 1- Mover los peones centrales del teclado
 * 2- Que los peones coman
 * 3- Aplicar el ciclo while con respuesta al operador
 * 4- Validar todas las variables
 * 5- Validar cada una de las opciones e imprimir el resultado de cada actividad
 * 6- Validar en caso que la opcion sea incorrecta
 *
 * Solo los peones del centro se van a mover, comer o ser comidos (D y E).
 * Empieza el blanco. Los peones de C y F solo pueden ser comidos.
 */

#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* =====================================================================
 *              AJEDREZ (SOLO PEONES CENTRALES) - SIN PUNTEROS
 * ===================================================================== */

#define N      8
#define BLANCO 1
#define NEGRO  2
#define META   2

char tab[N][N];
int  dueno[N][N];
int  movil[N][N];
int  turno, jugadas, comidas;
int  fila_leida, col_leida;
int  msg_tipo, m_num, m_turno, m_fo, m_co, m_fd, m_cd, m_comida;

void imprimir_bando(int c)
{
    if (c == BLANCO) printf("BLANCAS");
    else printf("NEGRAS");
}

void mostrar_mensaje(void)
{
    if (msg_tipo == 1) {
        printf("  >> Jugada %d (", m_num);
        imprimir_bando(m_turno);
        printf("): peon %c%d -> %c%d", 'A' + m_co, N - m_fo, 'A' + m_cd, N - m_fd);
        if (m_comida != ' ')
            printf("  [COME la pieza %c]", m_comida);
        printf("\n");
    } else if (msg_tipo == 2) {
        printf("  Las ");
        imprimir_bando(m_turno);
        printf(" no tienen jugadas. Pasan el turno.\n");
    }
    msg_tipo = 0;
}

void inicializar(void)
{
    const char fila[N] = { 'T', 'C', 'A', 'M', 'R', 'A', 'C', 'T' };
    int f, c;

    for (f = 0; f < N; f++)
        for (c = 0; c < N; c++) {
            tab[f][c] = ' ';
            dueno[f][c] = 0;
            movil[f][c] = 0;
        }
    for (c = 0; c < N; c++) {
        tab[0][c] = fila[c]; dueno[0][c] = NEGRO;
        tab[1][c] = 'P';     dueno[1][c] = NEGRO;
        tab[6][c] = 'P';     dueno[6][c] = BLANCO;
        tab[7][c] = fila[c]; dueno[7][c] = BLANCO;
    }

    movil[1][3] = movil[1][4] = movil[6][3] = movil[6][4] = 1;
    turno = BLANCO;
    jugadas = 0;
    comidas = 0;
    msg_tipo = 0;
}

void dibujar(void)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    int f, c;

    printf("\n      A  B  C  D  E  F  G  H\n");
    for (f = 0; f < N; f++) {
        printf("  %d  ", N - f);
        for (c = 0; c < N; c++) {
            SetConsoleTextAttribute(h, ((f + c) % 2 == 0) ? 0xF0 : 0x0F);
            printf(" %c%c", tab[f][c], dueno[f][c] == NEGRO ? '*' : ' ');
        }
        SetConsoleTextAttribute(h, 0x07);
        printf("  %d\n", N - f);
    }
    printf("      A  B  C  D  E  F  G  H\n\n");
    printf("  Casilla blanca = fondo blanco, negra = fondo negro.\n");
    printf("  Pieza blanca = letra sola (P), pieza negra = con asterisco (P*).\n");
    printf("  Piezas comidas: %d de %d\n", comidas, META);
}

int leer_casilla(int destino)
{
    int letra = 0, numero = 0, len = 0, c;

    if (destino) printf("  Casilla de destino: ");
    else         printf("  Casilla de origen : ");

    for (;;) {
        c = _getch();

        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '0' && len == 0) { putchar('\n'); return 0; }
        if (c == '\r') { if (len == 2) break; continue; }
        if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
        if (len == 0 && ((c >= 'A' && c <= 'H') || (c >= 'a' && c <= 'h'))) {
            if (c >= 'a') c -= 32;
            letra = c;
            len = 1;
            putchar(c);
        } else if (len == 1 && c >= '1' && c <= '8') {
            numero = c;
            len = 2;
            putchar(c);
        }
    }
    putchar('\n');
    col_leida  = letra - 'A';
    fila_leida = N - (numero - '0');
    return 1;
}

int validar(int fo, int co, int fd, int cd, int quien)
{
    int dir = (quien == BLANCO) ? -1 : 1;
    int ini = (quien == BLANCO) ? 6 : 1;
    int df = fd - fo, dc = cd - co;

    if (fd < 0 || fd >= N || cd < 0 || cd >= N) return 1;
    if (dueno[fo][co] == 0) return 2;
    if (dueno[fo][co] != quien) return 3;
    if (tab[fo][co] != 'P' || !movil[fo][co]) return 4;

    if (dc == 0) {
        if (df == dir)
            return dueno[fd][cd] == 0 ? 0 : 5;
        if (df == 2 * dir && fo == ini)
            return (dueno[fo + dir][co] == 0 && dueno[fd][cd] == 0) ? 0 : 5;
        return 5;
    }
    if ((dc == 1 || dc == -1) && df == dir) {
        if (cd < 2 || cd > 5) return 7;
        return dueno[fd][cd] == 3 - quien ? 0 : 6;
    }
    return 5;
}

int hay_movimientos(int quien)
{
    int f, c, dir = (quien == BLANCO) ? -1 : 1;

    for (f = 0; f < N; f++)
        for (c = 0; c < N; c++)
            if (dueno[f][c] == quien && movil[f][c])
                if (validar(f, c, f + dir, c, quien) == 0 || validar(f, c, f + 2 * dir, c, quien) == 0 ||
                    validar(f, c, f + dir, c - 1, quien) == 0 || validar(f, c, f + dir, c + 1, quien) == 0)
                    return 1;
    return 0;
}

void jugar(void)
{
    int fo, co, fd, cd, pasados = 0, salio = 0, codigo, comida;

    inicializar();
    while (comidas < META && pasados < 2 && !salio) {
        system("cls");
        printf("  === AJEDREZ: PEONES CENTRALES ===");
        dibujar();
        printf("\n");
        mostrar_mensaje();
        printf("\n");

        if (!hay_movimientos(turno)) {
            msg_tipo = 2;
            m_turno = turno;
            turno = 3 - turno;
            pasados++;
            continue;
        }
        pasados = 0;

        printf("  Turno de las ");
        imprimir_bando(turno);
        printf("   (0 = salir)\n");

        if (!leer_casilla(0)) { salio = 1; break; }
        fo = fila_leida;
        co = col_leida;

        if (!leer_casilla(1)) { salio = 1; break; }
        fd = fila_leida;
        cd = col_leida;

        codigo = validar(fo, co, fd, cd, turno);
        if (codigo != 0) {
            switch (codigo) {
            case 1: printf(">> JUGADA INVALIDA: Fuera del tablero.\n"); break;
            case 2: printf(">> JUGADA INVALIDA: La casilla de origen esta vacia.\n"); break;
            case 3: printf(">> JUGADA INVALIDA: Esa pieza no es del turno actual.\n"); break;
            case 4: printf(">> JUGADA INVALIDA: Solo se mueven los peones centrales (D y E).\n"); break;
            case 5: printf(">> JUGADA INVALIDA: Ese movimiento no es valido para el peon.\n"); break;
            case 6: printf(">> JUGADA INVALIDA: En diagonal solo se puede comer una pieza rival.\n"); break;
            case 7: printf(">> JUGADA INVALIDA: Solo se puede comer en las columnas C a F.\n"); break;
            }
            printf("  Pulsa una tecla para continuar...");
            _getch();
            continue;
        }

        comida = tab[fd][cd];
        tab[fd][cd] = tab[fo][co];
        dueno[fd][cd] = dueno[fo][co];
        movil[fd][cd] = movil[fo][co];
        tab[fo][co] = ' ';
        dueno[fo][co] = 0;
        movil[fo][co] = 0;
        jugadas++;
        if (comida != ' ') comidas++;

        msg_tipo = 1;
        m_num = jugadas;
        m_turno = turno;
        m_fo = fo; m_co = co;
        m_fd = fd; m_cd = cd;
        m_comida = comida;

        turno = 3 - turno;
    }

    system("cls");
    printf("  === AJEDREZ: PEONES CENTRALES ===");
    dibujar();
    printf("\n");
    mostrar_mensaje();
    printf("\n");
    if (comidas >= META)
        printf("  >> FIN DE LA PARTIDA: se comieron %d piezas en %d jugadas.\n", comidas, jugadas);
    else if (salio)
        printf("  >> Partida abandonada.\n");
    else
        printf("  >> FIN: ningun bando puede mover. Tablas.\n");
}

int leer_opcion(void)
{
    int c;
    for (;;) {
        c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '1' || c == '0') {
            putchar(c);
            putchar('\n');
            return c - '0';
        }
    }
}

int main(void)
{
    int salir = 0, op;

    Iniciar();

    while (!salir) {
        printf("\n  === AJEDREZ: PEONES CENTRALES ===\n");
        printf("  1. Jugar\n");
        printf("  0. Salir\n\n");
        printf("  Opcion: ");
        op = leer_opcion();

        if (op == 1)
            jugar();
        else
            salir = 1;
    }

    printf("\n  >> Saliendo del programa.\n");
    Finalizar();
    return 0;
}
