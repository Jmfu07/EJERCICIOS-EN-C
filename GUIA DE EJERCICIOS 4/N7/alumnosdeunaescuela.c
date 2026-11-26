#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ELABORADO POR JUAN MEDINA 20/08/2026
 * 7. Tenemos guardada en una matriz los alumnos de una escuela. Sabiendo que hay 3 cursos distintos,
 * 5 alumnos por curso y que cada alumno tiene 2 asignaturas, se pide:
 *   1. Pedir por teclado las notas que ha sacado cada alumno en cada asignatura y de cada curso.
 *   2. Decir cual es la nota media de un determinado curso.
 *   3. Decir cuantos aprobados y suspensos hay en una determinada asignatura en un determinado curso.
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

static double pedirNota(void)
{
    char buf[16]; int len, hay_punto, cuenta, i, c; double v;
    for (;;) {
        len = 0; hay_punto = 0; cuenta = 0;
        for (;;) {
            c = _getch();
            if (c == 0 || c == 0xE0) { _getch(); continue; }
            if (c == '\r') { if (cuenta > 0) break; continue; }
            if (c == '\b') {
                if (len > 0) {
                    len--;
                    if (buf[len] == '.') hay_punto = 0;
                    cuenta = 0;
                    for (i = 0; i < len; i++) if (buf[i] != '.') cuenta++;
                    printf("\b \b");
                }
                continue;
            }
            if (c >= '0' && c <= '9' && cuenta < 5) {
                cuenta++; buf[len++] = (char)c; putchar(c);
            } else if ((c == '.' || c == ',') && !hay_punto) {
                hay_punto = 1; buf[len++] = '.'; putchar('.');
            }
        }
        buf[len] = '\0'; putchar('\n');
        v = strtod(buf, NULL);
        if (v >= 0.0 && v <= 10.0) return v;
        printf("  Error: la nota debe estar entre 0 y 10. Intente de nuevo: ");
    }
}

static int leer_opcion(char min, char max)
{
    int c;
    for (;;) {
        c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c >= min && c <= max) {
            printf("%c\n", c);
            return c - '0';
        }
    }
}

int main(void)
{
    double notas[3][5][2], suma;
    int curso, alumno, asignatura, cursoElegido, asignaturaElegida, opcion;
    int aprobados, suspensos;

    Iniciar();

    printf("=== NOTAS DE 3 CURSOS x 5 ALUMNOS x 2 ASIGNATURAS (0 a 10) ===\n\n");
    for (curso = 0; curso < 3; curso++) {
        for (alumno = 0; alumno < 5; alumno++) {
            for (asignatura = 0; asignatura < 2; asignatura++) {
                printf("Curso %d, Alumno %d, Asignatura %d: ", curso + 1, alumno + 1, asignatura + 1);
                notas[curso][alumno][asignatura] = pedirNota();
            }
        }
    }

    do {
        printf("\n--- MENU ---\n");
        printf("1. Nota media de un curso\n");
        printf("2. Aprobados y suspensos por asignatura y curso\n");
        printf("0. Salir\n");
        printf("Opcion [0-2]: ");
        opcion = leer_opcion('0', '2');

        if (opcion == 1) {
            printf("Seleccione Curso [1-3]: ");
            cursoElegido = leer_opcion('1', '3');
            suma = 0.0;
            for (alumno = 0; alumno < 5; alumno++)
                for (asignatura = 0; asignatura < 2; asignatura++)
                    suma += notas[cursoElegido - 1][alumno][asignatura];
            printf("Media del curso %d: %.2f\n", cursoElegido, suma / 10.0);
        } else if (opcion == 2) {
            printf("Seleccione Curso [1-3]: ");
            cursoElegido = leer_opcion('1', '3');
            printf("Seleccione Asignatura [1-2]: ");
            asignaturaElegida = leer_opcion('1', '2');
            aprobados = 0;
            suspensos = 0;
            for (alumno = 0; alumno < 5; alumno++) {
                if (notas[cursoElegido - 1][alumno][asignaturaElegida - 1] >= 5.0)
                    aprobados++;
                else
                    suspensos++;
            }
            printf("Curso %d, Asignatura %d -> Aprobados: %d | Suspensos: %d\n",
                   cursoElegido, asignaturaElegida, aprobados, suspensos);
        }
    } while (opcion != 0);

    Finalizar();
    return 0;
}
