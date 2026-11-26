#define _WIN32_WINNT 0x0600
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

/* AND / OR / XOR de dos ficheros de enteros ordenados de menor a mayor.
 * El fichero resultante queda ordenado de mayor a menor.
 * Incluye el bloqueo de atajos de teclado: se activa al iniciar y se restaura al salir.
 * Compilar con: gcc ficheros_and_or_xor_bloqueo.c -o ficheros.exe -lshell32 -ladvapi32
 */

/* Ctrl+C y Ctrl+Break se ignoran. La X de la consola SI funciona: restaura antes de cerrar */

#define PERMITIR_ALTGR 0    /* 1 = deja pasar AltGr (para @ \ | # en teclado espanol) */
#define POLITICA "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\"
#define RUNONCE  "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"

#define LIMITE_MAX 9
#define TAM_BUF    (LIMITE_MAX + 1)
#define TAM_NOMBRE 260

/* ---------- Estado ---------- */

static HHOOK g_hook;             /* gancho de teclado */
static HANDLE g_th;              /* hilo que mantiene el gancho */
static DWORD g_hilo;             /* id de ese hilo */
static HANDLE g_listo;           /* aviso de que el gancho ya esta puesto */
static HANDLE g_hPrincipal;      /* hilo del menu, por si hay que sacarlo de scanf */
static HANDLE g_guardia;         /* ayudante elevado que aplica las politicas de HKLM */
static HANDLE g_evOk, g_evStop;  /* eventos para hablar con el ayudante */
static BOOL pasada[256];         /* pasada[tecla]: esa pulsacion llego a Windows, asi que su soltado tambien debe llegar */

/* ---------- Utilidades ---------- */

static BOOL EsAdmin(void)
{
    HANDLE token;
    TOKEN_ELEVATION e;
    DWORD n;
    BOOL r = FALSE;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        r = GetTokenInformation(token, TokenElevation, &e, sizeof e, &n) && e.TokenIsElevated;
        CloseHandle(token);
    }
    return r;
}

static void Pausa(const char *msg)
{
    printf("%s\nPulsa Enter.\n", msg);
    getchar();
}

/* Lanza este mismo programa como administrador y oculto. Si proc es NULL, espera a que termine. */
static BOOL Elevar(const char *args, HANDLE *proc)
{
    SHELLEXECUTEINFOA s;
    char exe[MAX_PATH];

    GetModuleFileNameA(NULL, exe, MAX_PATH);
    memset(&s, 0, sizeof s);
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
    if (proc) {
        *proc = s.hProcess;
    } else {
        WaitForSingleObject(s.hProcess, INFINITE);
        CloseHandle(s.hProcess);
    }
    return TRUE;
}

/* ---------- Registro ---------- */

/* Escribe el valor (on = TRUE) o lo borra (on = FALSE) */
static void ValorReg(HKEY raiz, const char *ruta, const char *nombre, DWORD tipo, const void *dato, DWORD tam, BOOL on)
{
    HKEY k;

    if (RegCreateKeyExA(raiz, ruta, 0, NULL, 0, KEY_SET_VALUE, NULL, &k, NULL) != ERROR_SUCCESS)
        return;
    if (on) RegSetValueExA(k, nombre, 0, tipo, (const BYTE *)dato, tam);
    else    RegDeleteValueA(k, nombre);
    RegCloseKey(k);
}

/* Pone o quita una politica DWORD. Si soloUsuario es TRUE, las de HKLM se omiten (sin admin). */
static void Politica(BOOL on, BOOL soloUsuario, HKEY raiz, const char *ruta, const char *nombre, DWORD valor)
{
    if (soloUsuario && raiz == HKEY_LOCAL_MACHINE)
        return;
    ValorReg(raiz, ruta, nombre, REG_DWORD, &valor, sizeof valor, on);
}

/* Aplica (on) o quita (!on) las politicas que vacian Ctrl+Alt+Supr y la pantalla de inicio de sesion.
   soloUsuario = TRUE: solo HKCU y sin tocar RunOnce, asi se puede repetir cada 2 s. */
static void Politicas(BOOL on, BOOL soloUsuario)
{
    char exe[MAX_PATH], cmd[MAX_PATH + 16];
    HKEY U = HKEY_CURRENT_USER, M = HKEY_LOCAL_MACHINE;

    if (!soloUsuario) {
        /* Si todo muere, Windows ejecuta "limpiar" en el siguiente inicio de sesion */
        GetModuleFileNameA(NULL, exe, MAX_PATH);
        sprintf(cmd, "\"%s\" limpiar", exe);
        ValorReg(U, RUNONCE, "BloqueoAtajos", REG_SZ, cmd, (DWORD)strlen(cmd) + 1, on);
    }

    Politica(on, soloUsuario, U, POLITICA "System",   "DisableLockWorkstation", 1); /* boton Bloquear y Win+L */
    Politica(on, soloUsuario, U, POLITICA "System",   "DisableTaskMgr", 1);         /* Administrador de tareas */
    Politica(on, soloUsuario, U, POLITICA "System",   "DisableChangePassword", 1);  /* Cambiar contrasena */
    Politica(on, soloUsuario, U, POLITICA "Explorer", "NoLogoff", 1);               /* Cerrar sesion */
    Politica(on, soloUsuario, U, POLITICA "Explorer", "NoClose", 1);                /* apagado en Ctrl+Alt+Supr y menu Inicio */

    Politica(on, soloUsuario, M, POLITICA "System",   "DisableLockWorkstation", 1); /* refuerzo de Win+L (NO verificado) */
    Politica(on, soloUsuario, M, POLITICA "System",   "HideFastUserSwitching", 1);  /* Cambiar de usuario */
    Politica(on, soloUsuario, M, POLITICA "System",   "shutdownwithoutlogon", 0);   /* apagado en la pantalla de inicio de sesion */
    Politica(on, soloUsuario, M, "SOFTWARE\\Policies\\Microsoft\\Windows\\System", "DontDisplayNetworkSelectionUI", 1); /* Wi-Fi / red */
    Politica(on, soloUsuario, M, "SOFTWARE\\Microsoft\\Windows Embedded\\EmbeddedLogon", "BrandingNeutral", 8);        /* accesibilidad (NO verificado) */
}

/* ---------- Ayudante elevado ---------- */

/* Corre como administrador: aplica TODAS las politicas, avisa al programa y espera.
   Cuando el programa termina (o pide parar), las quita. Si el programa muere de golpe, igual se restauran. */
static void Guardia(DWORD pid)
{
    char nombre[64];
    HANDLE proceso, ok, parar, espera[2];

    proceso = OpenProcess(SYNCHRONIZE, FALSE, pid);
    sprintf(nombre, "BloqueoAtajos_ok_%lu", (unsigned long)pid);
    ok = OpenEventA(EVENT_MODIFY_STATE, FALSE, nombre);
    sprintf(nombre, "BloqueoAtajos_stop_%lu", (unsigned long)pid);
    parar = OpenEventA(SYNCHRONIZE, FALSE, nombre);

    Politicas(TRUE, FALSE);
    if (ok) SetEvent(ok);

    espera[0] = proceso;
    espera[1] = parar;
    if (proceso && parar) WaitForMultipleObjects(2, espera, FALSE, INFINITE);
    Politicas(FALSE, FALSE);
}

/* Quita las politicas de HKCU y pide al ayudante (si existe) que quite las de HKLM. Se puede repetir. */
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

/* ---------- Teclado ---------- */

/* Lista blanca: solo se escriben texto, numeros, flechas, Espacio, Supr, Mayus y el teclado numerico.
   Lo demas (F1-F24, Esc, Tab, Inicio/Fin, menu contextual, teclas de correo o navegador) se traga. */
static BOOL Permitida(DWORD v)
{
    if (v == VK_SPACE || v == VK_DELETE || v == VK_CAPITAL) return TRUE;
    if (v >= VK_LEFT && v <= VK_DOWN) return TRUE;
    if (v >= '0' && v <= '9') return TRUE;
    if (v >= 'A' && v <= 'Z') return TRUE;
    if (v >= VK_NUMPAD0 && v <= VK_DIVIDE) return TRUE;
    if (v >= VK_OEM_1 && v <= VK_OEM_8) return TRUE; /* signos: ; = , - . / ` [ \ ] ' */
    return v == VK_OEM_102;                          /* tecla < > del teclado espanol */
}

/* Tecla fantasma (0xE8, no asignada). Se inyecta justo antes de soltar Win o Alt para que
   Windows no tome el toque por abrir un menu (Inicio, barra de menus...). */
static void Fantasma(void)
{
    keybd_event(0xE8, 0, 0, 0);
    keybd_event(0xE8, 0, KEYEVENTF_KEYUP, 0);
}

/* Gancho de teclado. Win, Alt, Ctrl y Mayus SE PASAN a Windows: su estado es el real y nada queda atascado.
   Se bloquea lo que se teclea mientras Win, Alt o Ctrl estan pulsadas, y lo que no esta en la lista blanca. */
static LRESULT CALLBACK Hook(int c, WPARAM w, LPARAM l)
{
    KBDLLHOOKSTRUCT *k = (KBDLLHOOKSTRUCT *)l;
    DWORD vk;
    BOOL abajo, ctrl, alt, win, bloquea;

    if (c != HC_ACTION || (k->flags & LLKHF_INJECTED))
        return CallNextHookEx(g_hook, c, w, l);

    vk = k->vkCode & 0xFF;
    abajo = (w == WM_KEYDOWN || w == WM_SYSKEYDOWN);

    /* Fn+4 del Lenovo L490 (apaga la pantalla): llega como vk 0xFF, scan 0x63 */
    if (vk == 0xFF)
        return 1;

    /* Win y Alt pasan a Windows; al soltarlas se manda la tecla fantasma */
    if (vk == VK_LWIN || vk == VK_RWIN || vk == VK_MENU || vk == VK_LMENU || vk == VK_RMENU) {
        if (!abajo) Fantasma();
        return CallNextHookEx(g_hook, c, w, l);
    }

    /* Ctrl y Mayus pasan siempre */
    if (vk == VK_SHIFT || vk == VK_LSHIFT || vk == VK_RSHIFT || vk == VK_CONTROL || vk == VK_LCONTROL || vk == VK_RCONTROL)
        return CallNextHookEx(g_hook, c, w, l);

    ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
    alt  = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
    win  = ((GetAsyncKeyState(VK_LWIN) | GetAsyncKeyState(VK_RWIN)) & 0x8000) != 0;
    if (PERMITIR_ALTGR && GetAsyncKeyState(VK_RMENU) < 0)
        ctrl = alt = FALSE; /* AltGr: Windows manda un Ctrl "falso" */

    /* Soltado: si su pulsacion paso, pasa; si fue bloqueada, se traga */
    if (!abajo) {
        if (pasada[vk]) {
            pasada[vk] = FALSE;
            return CallNextHookEx(g_hook, c, w, l);
        }
        return 1;
    }

    /* Enter y Borrar pasan siempre, salvo con Ctrl, Alt o Win (Win+Enter abre el narrador) */
    if (vk == VK_RETURN || vk == VK_BACK)
        bloquea = ctrl || alt || win;
    else
        bloquea = ctrl || alt || win || !Permitida(vk);

    if (bloquea)
        return 1;

    pasada[vk] = TRUE;
    return CallNextHookEx(g_hook, c, w, l);
}

/* Ctrl+C y Ctrl+Break se ignoran. Al cerrar la consola (X), cerrar sesion o apagar, restaura antes de morir */
static BOOL WINAPI Cierre(DWORD e)
{
    if (e == CTRL_C_EVENT || e == CTRL_BREAK_EVENT)
        return TRUE;
    Restaurar();
    return FALSE;
}

/* Hilo propio: el gancho y los temporizadores viven aqui, asi el menu puede quedarse esperando teclas */
static DWORD WINAPI Hilo(LPVOID p)
{
    MSG m;
    UINT_PTR tPol, tHook;
    (void)p;

    g_hilo = GetCurrentThreadId();
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL); /* un gancho lento lo descarta Windows */
    tPol  = SetTimer(NULL, 0, 2000, NULL);  /* reaplica politicas de usuario cada 2 s */
    tHook = SetTimer(NULL, 0, 20000, NULL); /* renueva el gancho cada 20 s */

    Politicas(TRUE, g_guardia != NULL);     /* antes del gancho: no hay hueco para Win+L */
    g_hook = SetWindowsHookExA(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0);
    SetEvent(g_listo);

    while (g_hook && GetMessage(&m, NULL, 0, 0) > 0) {
        if (m.message != WM_TIMER) {
            DispatchMessage(&m);
        } else if (m.wParam == tPol) {
            Politicas(TRUE, TRUE);
        } else if (m.wParam == tHook) {
            HHOOK nuevo = SetWindowsHookExA(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0);
            if (nuevo) {
                UnhookWindowsHookEx(g_hook);
                g_hook = nuevo;
            }
        }
    }

    if (g_hook) UnhookWindowsHookEx(g_hook);
    Restaurar();
    return 0;
}

/* ---------- Inicio y final del bloqueo ---------- */

/* FINAL: detiene el gancho y restaura todo. Es segura de llamar mas de una vez.
   atexit la ejecuta al terminar main. */
static void Finalizar(void)
{
    if (g_th) {
        PostThreadMessage(g_hilo, WM_QUIT, 0, 0);
        WaitForSingleObject(g_th, 8000);
        CloseHandle(g_th);
        g_th = NULL;
    }
    if (g_hPrincipal) {
        CloseHandle(g_hPrincipal);
        g_hPrincipal = NULL;
    }
}

/* Instala el bloqueo. FALSE si no se pudo (sin administrador confirmado o sin gancho). */
static BOOL Bloquear(void)
{
    DWORD modo;
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);

    if (g_th) return TRUE;

    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_hPrincipal, THREAD_TERMINATE, FALSE, 0);

    if (!EsAdmin()) {
        char nombre[64], args[48];
        DWORD pid = GetCurrentProcessId();
        HANDLE espera[2];
        BOOL ok = FALSE;

        sprintf(nombre, "BloqueoAtajos_ok_%lu", (unsigned long)pid);
        g_evOk = CreateEventA(NULL, TRUE, FALSE, nombre);
        sprintf(nombre, "BloqueoAtajos_stop_%lu", (unsigned long)pid);
        g_evStop = CreateEventA(NULL, TRUE, FALSE, nombre);
        sprintf(args, "guardia %lu", (unsigned long)pid);

        if (Elevar(args, &g_guardia)) {
            espera[0] = g_evOk;
            espera[1] = g_guardia;
            ok = WaitForMultipleObjects(2, espera, FALSE, 120000) == WAIT_OBJECT_0;
            if (!ok) Pausa("La ayuda con permisos de administrador no confirmo a tiempo.");
        }
        if (!ok) {
            Restaurar();
            return FALSE;
        }
    }

    SetConsoleCtrlHandler(Cierre, TRUE);  /* la X de la consola queda activa: restaura y cierra */
    if (GetConsoleMode(in, &modo))        /* un clic en la consola congelaria el gancho */
        SetConsoleMode(in, (modo | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE);

    g_listo = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_th = CreateThread(NULL, 0, Hilo, NULL, 0, NULL);
    WaitForSingleObject(g_listo, 5000);

    if (!g_hook) {
        CloseHandle(g_th);
        g_th = NULL;
        Restaurar();
        return FALSE;
    }
    atexit(Finalizar);
    return TRUE;
}

/* INICIO: atiende los modos internos (ayudante elevado y limpieza) o, en uso normal, activa el bloqueo.
   Lee los argumentos de la linea de comandos, por eso main no necesita argc ni argv. */
static void Iniciar(void)
{
    const char *cl = GetCommandLineA();
    char cmd[32] = "";

    cl += (*cl == '"') ? strcspn(cl + 1, "\"") + 2 : strcspn(cl, " "); /* salta la ruta del .exe */
    cl += strspn(cl, " ");
    sscanf(cl, "%31s", cmd);

    if (strcmp(cmd, "guardia") == 0) {          /* ayudante elevado (lo lanza Bloquear) */
        Guardia(strtoul(cl + 8, NULL, 10));
        ExitProcess(0);
    }
    if (strcmp(cmd, "limpiar") == 0) {          /* lo lanza RunOnce tras un cierre brusco */
        if (EsAdmin()) Politicas(FALSE, FALSE);
        else Elevar("limpiar", NULL);
        ExitProcess(0);
    }
    if (!Bloquear()) {
        Pausa("Error al instalar el bloqueo.");
        ExitProcess(1);
    }
}

/* ---------- Entrada por teclado (validacion) ---------- */

/* Lectura base, tecla por tecla: solo digitos (sin signo ni punto).
 * max = maximo de digitos. Enter confirma (al menos un digito). Backspace borra.
 * buf debe tener TAM_BUF espacios. */
static void leer_digitos(char buf[], int max)
{
    int len = 0;

    if (max > LIMITE_MAX) max = LIMITE_MAX;
    if (max < 1) max = 1;

    for (;;) {
        int c = _getch();

        if (c == 0 || c == 0xE0) {          /* tecla especial: se ignora */
            _getch();
            continue;
        }
        if (c == '\r') {                    /* Enter */
            if (len > 0) break;
            continue;
        }
        if (c == '\b') {                    /* Backspace */
            if (len > 0) { len--; printf("\b \b"); }
            continue;
        }
        if (c >= '0' && c <= '9' && len < max) {
            buf[len++] = (char)c;
            putchar(c);
        }
    }
    buf[len] = '\0';
    putchar('\n');
}

static long leer_entero_pos(int max) { char b[TAM_BUF]; leer_digitos(b, max); return strtol(b, NULL, 10); }

/* Lee un nombre de fichero tecla a tecla. Rechaza los caracteres que Windows
 * no admite en un nombre (< > : " / \ | ? *) y los de control. */
static void leer_nombre(char buf[], int max)
{
    int len = 0;

    for (;;) {
        int c = _getch();

        if (c == 0 || c == 0xE0) {          /* tecla especial: se ignora */
            _getch();
            continue;
        }
        if (c == '\r') {                    /* Enter */
            if (len > 0) break;
            continue;
        }
        if (c == '\b') {                    /* Backspace */
            if (len > 0) { len--; printf("\b \b"); }
            continue;
        }
        if (c >= 32 && c < 127 && !strchr("<>:\"/\\|?*", c) && len < max - 1) {
            buf[len++] = (char)c;
            putchar(c);
        }
    }
    buf[len] = '\0';
    putchar('\n');
}

static void pedir_nombre(const char *texto, char buf[])
{
    printf("%s: ", texto);
    leer_nombre(buf, TAM_NOMBRE);
}

/* ---------- Ficheros ---------- */

enum { OK = 0, E_ABRIR = -1, E_ORDEN = -2, E_MEM = -3 };

/* Carga un fichero en un vector dinamico. Exige orden ascendente; descarta repetidos. */
static int cargar(const char *nombre, int **v, int *n)
{
    FILE *f = fopen(nombre, "r");
    int *a = NULL, *t;
    int cap = 0, cnt = 0, x;

    *v = NULL;
    *n = 0;
    if (!f) return E_ABRIR;

    while (fscanf(f, "%d", &x) == 1) {
        if (cnt > 0 && x == a[cnt - 1]) continue;       /* repetido */
        if (cnt > 0 && x < a[cnt - 1]) {                /* no esta ordenado */
            free(a);
            fclose(f);
            return E_ORDEN;
        }
        if (cnt == cap) {
            cap = cap ? cap * 2 : 64;
            t = (int *)realloc(a, (size_t)cap * sizeof *a);
            if (!t) { free(a); fclose(f); return E_MEM; }
            a = t;
        }
        a[cnt++] = x;
    }
    fclose(f);
    *v = a;
    *n = cnt;
    return OK;
}

static int guardar(const char *nombre, const int *v, int n)
{
    FILE *f = fopen(nombre, "w");
    int i;

    if (!f) return E_ABRIR;
    for (i = 0; i < n; i++) fprintf(f, "%d\n", v[i]);
    fclose(f);
    return OK;
}

/* ---------- Operaciones ----------
 * Se recorren los dos vectores desde el final (el mayor) hacia el principio,
 * asi vout queda directamente de mayor a menor. Devuelven cuantos elementos escribieron. */

static int op_and(const int *a, int na, const int *b, int nb, int *out)
{
    int i = na - 1, j = nb - 1, k = 0;

    while (i >= 0 && j >= 0) {
        if (a[i] == b[j])      { out[k++] = a[i]; i--; j--; }
        else if (a[i] > b[j])  i--;
        else                   j--;
    }
    return k;
}

static int op_or(const int *a, int na, const int *b, int nb, int *out)
{
    int i = na - 1, j = nb - 1, k = 0;

    while (i >= 0 && j >= 0) {
        if (a[i] == b[j])      { out[k++] = a[i]; i--; j--; }
        else if (a[i] > b[j])  out[k++] = a[i--];
        else                   out[k++] = b[j--];
    }
    while (i >= 0) out[k++] = a[i--];
    while (j >= 0) out[k++] = b[j--];
    return k;
}

static int op_xor(const int *a, int na, const int *b, int nb, int *out)
{
    int i = na - 1, j = nb - 1, k = 0;

    while (i >= 0 && j >= 0) {
        if (a[i] == b[j])      { i--; j--; }
        else if (a[i] > b[j])  out[k++] = a[i--];
        else                   out[k++] = b[j--];
    }
    while (i >= 0) out[k++] = a[i--];
    while (j >= 0) out[k++] = b[j--];
    return k;
}

/* ---------- Programa ---------- */

static int menu(void)
{
    long op;

    printf("\n1) AND (en ambos ficheros)\n"
           "2) OR  (en cualquiera, sin repetidos)\n"
           "3) XOR (en uno solo)\n"
           "0) Salir\n"
           "Opcion: ");
    for (;;) {
        op = leer_entero_pos(1);
        if (op >= 0 && op <= 3) return (int)op;
        printf("Opcion no valida (0-3): ");
    }
}

static int error_carga(const char *nombre, int r)
{
    if (r == E_ABRIR)      printf("No se pudo abrir '%s'.\n", nombre);
    else if (r == E_ORDEN) printf("'%s' no esta ordenado de menor a mayor.\n", nombre);
    else if (r == E_MEM)   printf("Memoria insuficiente leyendo '%s'.\n", nombre);
    return r != OK;
}

int main(void)
{
    char f1[TAM_NOMBRE], f2[TAM_NOMBRE], fs[TAM_NOMBRE];
    int *a = NULL, *b = NULL, *out = NULL;
    int na = 0, nb = 0, nout, op;

    Iniciar();   /* activa el bloqueo de atajos (o atiende los modos internos guardia / limpiar) */

    pedir_nombre("Primer fichero de entrada", f1);
    pedir_nombre("Segundo fichero de entrada", f2);
    pedir_nombre("Fichero de salida", fs);

    if (_stricmp(fs, f1) == 0 || _stricmp(fs, f2) == 0) {
        printf("El fichero de salida no puede ser uno de los de entrada.\n");
        return 1;
    }
    if (error_carga(f1, cargar(f1, &a, &na)) || error_carga(f2, cargar(f2, &b, &nb))) {
        free(a);
        free(b);
        return 1;
    }

    out = (int *)malloc((size_t)(na + nb + 1) * sizeof *out);
    if (!out) {
        printf("Memoria insuficiente.\n");
        free(a);
        free(b);
        return 1;
    }

    while ((op = menu()) != 0) {
        if (op == 1)      nout = op_and(a, na, b, nb, out);
        else if (op == 2) nout = op_or(a, na, b, nb, out);
        else              nout = op_xor(a, na, b, nb, out);

        if (guardar(fs, out, nout) == OK)
            printf("Hecho: %d numeros escritos en '%s' (de mayor a menor).\n", nout, fs);
        else
            printf("No se pudo escribir '%s'.\n", fs);
    }

    free(a);
    free(b);
    free(out);
    return 0;   /* atexit(Finalizar) detiene el gancho y restaura las politicas */
}
