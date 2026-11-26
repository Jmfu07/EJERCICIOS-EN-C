#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*  
 * ELABORADO 8/10/2026 -- JUAN MEDINA CI:31708027
 *
 * Una persona tiene una cuenta corriente en un banco nacional con un monto inicial de 520000,00 Bs soberanos.
 * La persona ingresa por teclado su clave de acceso compuesta de cuatro digitos y luego una opcion numerica
 * para realizar las siguientes operaciones:
 *   1- Consulta de saldo
 *   2- Deposito
 *   3- Retiro
 *   4- Salir
 *
 * EFECTUAR UN PROGRAMA EN LENGUAJE C PARA:
 * A- Ingresar la clave de acceso y validar.
 * B- Ingresar una opcion del usuario y realizar la transaccion solicitada (imprimir resultado).
 * C- Utilizar el ciclo while con respuesta del operador.
 * D- Validar todas las opciones e incluso cuando esta sea incorrecta.
 */

#define PERMITIR_ALTGR 0
#define POL "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\"
#define LIMITE_MAX 12
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

static int leer_clave_4_digitos(void)
{
    char buf[5];
    int len = 0;

    for (;;) {
        int c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') {
            if (len == 4) break;
            continue;
        }
        if (c == '\b') {
            if (len > 0) { len--; printf("\b \b"); }
            continue;
        }
        if (c >= '0' && c <= '9' && len < 4) {
            buf[len++] = (char)c;
            putchar('*');
        }
    }
    buf[4] = '\0';
    putchar('\n');
    return (int)strtol(buf, NULL, 10);
}

static double leer_monto_pos(int max_dig)
{
    char buf[TAM_BUF];
    int len = 0, hay_punto = 0, cuenta = 0, i;

    for (;;) {
        int c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (cuenta > 0) break; continue; }
        if (c == '\b') {
            if (len > 0) {
                len--;
                if (buf[len] == '.') hay_punto = 0;
                cuenta = 0;
                for (i = 0; i < len; i++)
                    if (buf[i] != '.') cuenta++;
                printf("\b \b");
            }
            continue;
        }
        if (c >= '0' && c <= '9') {
            if (cuenta >= max_dig) continue;
            cuenta++;
        } else if ((c == '.' || c == ',') && !hay_punto) {
            c = '.';
            hay_punto = 1;
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

static int leer_sn(void)
{
    for (;;) {
        int c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == 's' || c == 'S') { putchar('S'); putchar('\n'); return 1; }
        if (c == 'n' || c == 'N') { putchar('N'); putchar('\n'); return 0; }
    }
}

int main(void)
{
    int op1 = 0, clave = 0, continuar = 1, r;
    double saldo = 520000.00, deposito = 0.0, retiro = 0.0;

    Iniciar();

    do {
        printf("\nIngrese su clave secreta de 4 digitos: ");
        clave = leer_clave_4_digitos();
        if (clave == 1234)
            printf("\nCLAVE CORRECTA\n");
        else
            printf("\n[ERROR] CLAVE INCORRECTA. Intente nuevamente.\n");
    } while (clave != 1234);

    while (continuar) {
        printf("\n--- BIENVENIDO AL BFC ---\n");
        printf("1. Consulta de saldo\n");
        printf("2. Deposito\n");
        printf("3. Retiro\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion [1-4]: ");

        do {
            r = _getch();
            if (r == 0 || r == 0xE0) { _getch(); r = 0; }
        } while (r < '1' || r > '4');
        printf("%c\n", r);
        op1 = r - '0';

        if (op1 == 1) {
            printf("\nSu saldo actual es de: %.2f Bs\n", saldo);
        } else if (op1 == 2) {
            do {
                printf("\nIngrese el monto a depositar (max %.2f Bs): ", saldo);
                deposito = leer_monto_pos(9);
                if (deposito <= 0.0 || deposito > saldo)
                    printf("[Error] El monto debe ser mayor a 0 y no exceder su saldo actual (%.2f Bs).\n", saldo);
            } while (deposito <= 0.0 || deposito > saldo);

            saldo += deposito;
            printf("Deposito exitoso. Nuevo saldo: %.2f Bs\n", saldo);
        } else if (op1 == 3) {
            if (saldo <= 0.0) {
                printf("\n[Error] No dispone de saldo para retirar.\n");
            } else {
                do {
                    printf("\nIngrese el monto a retirar (disponible %.2f Bs): ", saldo);
                    retiro = leer_monto_pos(9);
                    if (retiro <= 0.0 || retiro > saldo)
                        printf("[Error] El retiro debe ser mayor a 0 y no superar su saldo actual (%.2f Bs).\n", saldo);
                } while (retiro <= 0.0 || retiro > saldo);

                saldo -= retiro;
                printf("Retiro exitoso. Nuevo saldo: %.2f Bs\n", saldo);
            }
        } else if (op1 == 4) {
            break;
        }

        printf("\nDesea realizar otra operacion? (S/N): ");
        continuar = leer_sn();
    }

    printf("\nGRACIAS POR USAR EL CAJERO. Saldo final: %.2f Bs\n", saldo);
    Finalizar();
    return 0;
}
