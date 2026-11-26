#define _WIN32_WINNT 0x0601   /* necesario para RegSetKeyValueA y RegDeleteKeyValueA */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>            /* getch */

/* Ctrl+C y Ctrl+Break se ignoran. La X de la consola SI funciona: restaura antes de cerrar */

#define PERMITIR_ALTGR 0      /* 1 = deja pasar AltGr (para @ \ | # en teclado espanol) */
#define POL "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\"

/* ---------- Estado ---------- */

static HHOOK g_hook;                        /* gancho de teclado */
static HANDLE g_th, g_listo, g_hPrincipal;  /* hilo del gancho, aviso de listo, hilo principal */
static HANDLE g_guardia, g_evOk, g_evStop;  /* ayudante elevado y sus eventos */
static DWORD g_hilo;                        /* id del hilo del gancho */
static BOOL pasada[256];                    /* pulsaciones que llegaron a Windows: su soltado tambien */

/* ---------- Politicas (tabla) ---------- */

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

/* ---------- Utilidades ---------- */

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

/* Lanza este mismo programa como administrador y oculto. Si proc es NULL, espera a que termine. */
static BOOL Elevar(const char *args, HANDLE *proc)
{
    char exe[MAX_PATH];
    SHELLEXECUTEINFOA s = {sizeof s, SEE_MASK_NOCLOSEPROCESS};

    GetModuleFileNameA(NULL, exe, MAX_PATH);
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

/* ---------- Registro ---------- */

/* Escribe el valor (on) o lo borra (!on) */
static void Valor(HKEY r, const char *ruta, const char *n, DWORD tipo, const void *d, DWORD tam, BOOL on)
{
    if (on) RegSetKeyValueA(r, ruta, n, tipo, d, tam);
    else    RegDeleteKeyValueA(r, ruta, n);
}

/* Aplica (on) o quita (!on) las politicas. soloUsuario: solo HKCU y sin RunOnce (se repite cada 2 s). */
static void Politicas(BOOL on, BOOL soloUsuario)
{
    char exe[MAX_PATH], cmd[MAX_PATH + 16];
    size_t i;

    if (!soloUsuario) {   /* Si todo muere, Windows ejecuta "limpiar" en el siguiente inicio de sesion */
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

/* ---------- Modos internos (linea de comandos) ---------- */

/* "guardia <pid>": corre como administrador, aplica TODO, avisa y espera.
   Cuando el programa termina (o pide parar) lo quita; si el programa muere de golpe, tambien. */
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

/* "limpiar": lo lanza RunOnce tras un cierre brusco */
static void Limpiar(void)
{
    if (EsAdmin()) Politicas(FALSE, FALSE);
    else Elevar("limpiar", NULL);
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

/* Lista blanca: texto, numeros, flechas, Espacio, Supr, Mayus y teclado numerico.
   Lo demas (F1-F24, Esc, Tab, Inicio/Fin, menu contextual, teclas de correo o navegador) se traga. */
static BOOL Permitida(DWORD v)
{
    return v == VK_SPACE || v == VK_DELETE || v == VK_CAPITAL || v == VK_OEM_102  /* < > en teclado espanol */
        || (v >= VK_LEFT && v <= VK_DOWN) || (v >= '0' && v <= '9') || (v >= 'A' && v <= 'Z')
        || (v >= VK_NUMPAD0 && v <= VK_DIVIDE) || (v >= VK_OEM_1 && v <= VK_OEM_8);
}

static BOOL Down(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }

/* Tecla fantasma (0xE8, no asignada): al soltar Win o Alt evita que Windows abra un menu */
static void Fantasma(void)
{
    keybd_event(0xE8, 0, 0, 0);
    keybd_event(0xE8, 0, KEYEVENTF_KEYUP, 0);
}

/* Win, Alt, Ctrl y Shift SE PASAN a Windows (su estado es el real, nada queda atascado).
   Se bloquea lo tecleado con Win, Alt o Ctrl pulsadas, y lo que no esta en la lista blanca. */
static LRESULT CALLBACK Hook(int c, WPARAM w, LPARAM l)
{
    KBDLLHOOKSTRUCT *k = (KBDLLHOOKSTRUCT *)l;
    DWORD vk = k->vkCode & 0xFF;
    BOOL abajo = (w == WM_KEYDOWN || w == WM_SYSKEYDOWN), pasa;

    if (c != HC_ACTION || (k->flags & LLKHF_INJECTED))
        return CallNextHookEx(g_hook, c, w, l);
    if (vk == 0xFF) return 1;               /* Fn+4 del Lenovo L490 (apaga la pantalla) */

    switch (vk) {
    case VK_LWIN: case VK_RWIN: case VK_MENU: case VK_LMENU: case VK_RMENU:
        if (!abajo) Fantasma();
        /* fallthrough */
    case VK_SHIFT: case VK_LSHIFT: case VK_RSHIFT: case VK_CONTROL: case VK_LCONTROL: case VK_RCONTROL:
        return CallNextHookEx(g_hook, c, w, l);
    }

    if (!abajo) {                           /* soltado: pasa solo si su pulsacion paso */
        pasa = pasada[vk];
        pasada[vk] = FALSE;
        return pasa ? CallNextHookEx(g_hook, c, w, l) : 1;
    }

    /* Enter y Borrar pasan siempre salvo con Ctrl/Alt/Win (Win+Enter abre el narrador). AltGr = Ctrl "falso". */
    if (Down(VK_LWIN) || Down(VK_RWIN)
        || ((Down(VK_CONTROL) || Down(VK_MENU)) && !(PERMITIR_ALTGR && Down(VK_RMENU)))
        || (vk != VK_RETURN && vk != VK_BACK && !Permitida(vk)))
        return 1;

    pasada[vk] = TRUE;
    return CallNextHookEx(g_hook, c, w, l);
}

static HHOOK Poner(void) { return SetWindowsHookExA(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0); }

/* Temporizadores con callback: DispatchMessage los ejecuta */
static VOID CALLBACK TPol(HWND h, UINT m, UINT_PTR i, DWORD t) { (void)h; (void)m; (void)i; (void)t; Politicas(TRUE, TRUE); }

static VOID CALLBACK TGancho(HWND h, UINT m, UINT_PTR i, DWORD t)
{
    HHOOK nuevo = Poner();
    (void)h; (void)m; (void)i; (void)t;
    if (nuevo) { UnhookWindowsHookEx(g_hook); g_hook = nuevo; }
}

/* Ctrl+C/Break se ignoran. Al cerrar consola (X), cerrar sesion o apagar, restaura antes de morir */
static BOOL WINAPI Cierre(DWORD e)
{
    if (e == CTRL_C_EVENT || e == CTRL_BREAK_EVENT) return TRUE;
    Restaurar();
    return FALSE;
}

/* Hilo propio: gancho y temporizadores viven aqui, asi el menu puede quedarse esperando entrada */
static DWORD WINAPI Hilo(LPVOID p)
{
    MSG m;
    (void)p;

    g_hilo = GetCurrentThreadId();
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL); /* un gancho lento lo descarta Windows */
    SetTimer(NULL, 0, 2000, TPol);       /* reaplica politicas de usuario cada 2 s */
    SetTimer(NULL, 0, 20000, TGancho);   /* renueva el gancho cada 20 s */

    Politicas(TRUE, g_guardia != NULL);  /* antes del gancho: no hay hueco para Win+L */
    g_hook = Poner();
    SetEvent(g_listo);

    while (g_hook && GetMessage(&m, NULL, 0, 0) > 0)
        DispatchMessage(&m);

    if (g_hook) UnhookWindowsHookEx(g_hook);
    Restaurar();
    return 0;
}

/* ---------- Inicio y final ---------- */

/* FINAL: detiene el gancho y restaura todo. Segura de llamar varias veces (menu o atexit). */
static void Finalizar(void)
{
    if (g_th) {
        PostThreadMessage(g_hilo, WM_QUIT, 0, 0);
        WaitForSingleObject(g_th, 8000);
        Cerrar(&g_th);
    }
    Cerrar(&g_hPrincipal);
}

/* Lanza el ayudante elevado y espera su confirmacion */
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

/* Instala el bloqueo. FALSE si no se pudo (sin administrador confirmado o sin gancho). */
static BOOL Bloquear(void)
{
    DWORD modo;
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);

    if (g_th) return TRUE;
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_hPrincipal, THREAD_TERMINATE, FALSE, 0);

    if (!EsAdmin() && !LanzarGuardia()) { Restaurar(); return FALSE; }

    SetConsoleCtrlHandler(Cierre, TRUE);  /* la X de la consola queda activa: restaura y cierra */
    if (GetConsoleMode(in, &modo))        /* un clic en la consola congelaria el gancho */
        SetConsoleMode(in, (modo | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE);

    g_listo = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_th = CreateThread(NULL, 0, Hilo, NULL, 0, NULL);
    WaitForSingleObject(g_listo, 5000);

    if (!g_hook) { Cerrar(&g_th); Restaurar(); return FALSE; }
    atexit(Finalizar);
    return TRUE;
}

/* INICIO: atiende los modos internos o, en uso normal, activa el bloqueo. */
static void Iniciar(void)
{
    if (__argc > 1 && !strcmp(__argv[1], "guardia")) { Guardia(__argc > 2 ? __argv[2] : ""); ExitProcess(0); }
    if (__argc > 1 && !strcmp(__argv[1], "limpiar")) { Limpiar(); ExitProcess(0); }
    if (!Bloquear()) { Pausa("Error al instalar el bloqueo."); ExitProcess(1); }
}

//* Maximo de caracteres que se leen por linea (incluye el '\0'). */
#define TAM_LINEA 128
 
/* Para leer_cadena: que caracteres se aceptan */
#define CUALQUIERA   0   /* cualquier caracter */
#define SOLO_LETRAS  1   /* solo letras (A-Z, a-z) y espacios */
 
enum { LINEA_OK, LINEA_LARGA, LINEA_EOF };
 
/* ===================== UTILIDADES INTERNAS ===================== */
 
static void fin_entrada(void)
{
    printf("\n[Entrada cerrada, saliendo del programa]\n");
    exit(1);
}
 
/* Lee una linea SIN desbordar buf. Si es mas larga que tam-1,
 * descarta el resto de la linea y devuelve LINEA_LARGA. */
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
    /* No hubo '\n': la linea era mas larga que el buffer -> vaciar el resto */
    excedio = 0;
    c = getchar();
    while (c != '\n' && c != EOF) {
        excedio = 1;
        c = getchar();
    }
    return excedio ? LINEA_LARGA : LINEA_OK;
}
 
/* Quita espacios al inicio y al final. */
static void recortar(char s[])
{
    int ini, fin, i;
 
    ini = 0;
    fin = (int)strlen(s);
    while (s[ini] != '\0' && isspace((unsigned char)s[ini])) ini++;
    while (fin > ini && isspace((unsigned char)s[fin - 1])) fin--;
    for (i = 0; i < fin - ini; i++) s[i] = s[ini + i];
    s[fin - ini] = '\0';
}
 
/* Devuelve 1 si se obtuvo una linea valida y no vacia.
 * Devuelve 0 si estaba vacia o era muy larga (ya imprimio el error):
 * quien la llama solo debe volver a pedir el dato. */
static int obtener_linea(char buf[], int tam)
{
    int r;
 
    r = leer_linea(buf, tam);
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
 
/* ===================== ENTEROS ===================== */
/* Uso:  edad = leer_entero("Edad", 0, 120);
 * Muestra solo:  Edad [0 a 120]:
 * Rechaza: letras, decimales (1,5 / 1.5), vacio, fuera de rango, desborde. */
 
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
 
        ok = (buf[i] != '\0');      /* tiene que haber al menos un digito */
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
 
/* ===================== FLOAT ===================== */
/* Uso:  radio = leer_float("Radio", 0.1, 1000.0);
 * Acepta punto o coma decimal (3.14 o 3,14). Sin notacion cientifica. */
 
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
 
        resultado = (float)valor;   /* se compara ya como float */
        if (resultado < min || resultado > max) {
            printf("  Fuera de rango: debe estar entre %g y %g.\n", (double)min, (double)max);
            continue;
        }
        return resultado;
    }
}
 
/* ===================== CHAR ===================== */
/* Uso:  letra = leer_caracter("Continuar?", "SN");
 *       -> muestra:  Continuar? [S/N]:
 * permitidos = "" (cadena vacia) acepta cualquier caracter.
 * No distingue mayusculas de minusculas. */
 
char leer_caracter(const char prompt[], const char permitidos[])
{
    char buf[TAM_LINEA];
    int i, n;
 
    n = (int)strlen(permitidos);
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
 
/* ===================== CADENA ===================== */
/* Uso:
 *     char nombre[20 + 1];      <- el arreglo mide (maximo + 1) por el '\0'
 *     leer_cadena("Nombre", nombre, 2, 20, SOLO_LETRAS);
 *
 * REGLA DE ORO: el numero del arreglo debe ser (max + 1).
 * Lo mas facil es usar un #define para que ambos coincidan siempre:
 *     #define NOMBRE_MAX 20
 *     char nombre[NOMBRE_MAX + 1];
 *     leer_cadena("Nombre", nombre, 2, NOMBRE_MAX, SOLO_LETRAS);
 */
 
void leer_cadena(const char prompt[], char dest[], int min, int max, int tipo)
{
    char buf[TAM_LINEA];
    int i, len, ok;
 
    if (max > TAM_LINEA - 1) max = TAM_LINEA - 1;   /* seguridad */
 
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
        for (i = 0; i <= len; i++) dest[i] = buf[i];   /* copia incluyendo '\0' */
        return;
    }
}
 
/* ===================== TECLA / MENUS ===================== */
 
/* Devuelve la tecla presionada, o 0 si no sirve (teclas especiales, etc.). */
static int leer_tecla(void)
{
    int c;
 
    c = getch();
    if (c == 0 || c == 224) {   /* flechas / teclas de funcion mandan 2 codigos */
        getch();
        return 0;
    }
    return c;
}
 
/* Menu numerico: acepta solo teclas entre min y max (ej: '0','4').
 * Con conio basta presionar la tecla; Enter y las demas se ignoran.
 * Devuelve el numero (0 a 9). */
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
 
/* Menu por letras: validas = "ABV" etc. Devuelve la letra en MAYUSCULA. */
char leer_opcion_letras(const char validas[])
{
    int c, i, n;
 
    n = (int)strlen(validas);
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
 
/* Pregunta S/N. Devuelve 1 si S, 0 si N. */
int desea_repetir(const char pregunta[])
{
    printf("%s (S/N): ", pregunta);
    return leer_opcion_letras("SN") == 'S';
}

 
/* ============================================================
 *  ZONA DE CONFIGURACION DEL PROGRAMA  (EDITA SOLO ESTA PARTE)
 *  Cambia los valores y el programa se adapta solo.
 * ============================================================ */
 
/* --- Menu principal: teclas validas (de '0' a '4') --- */
#define OPC_MIN '0'
#define OPC_MAX '4'
 
/* --- Caso 1: suma de enteros (rango permitido) --- */
#define SUMA_MIN (-1000)
#define SUMA_MAX 1000
 
/* --- Caso 2: radio del circulo (float, rango permitido) --- */
#define RADIO_MIN 0.1
#define RADIO_MAX 1000.0
 
/* --- Caso 3: nombre (cantidad de caracteres que necesitas) --- */
#define NOMBRE_MIN 2
#define NOMBRE_MAX 20      /* <- aqui pones cuantos caracteres quieres como maximo */
 
/* --- Caso 4: submenu por letras y char permitidos --- */
#define SUB_LETRAS     "ABV"   /* letras validas del submenu */
#define SUB_CHARS      "SN"    /* caracteres validos para leer_caracter */
#define SUB_ENTERO_MIN 1
#define SUB_ENTERO_MAX 10
 
/* ===================== PROGRAMA DE EJEMPLO ===================== */

static void mostrar_menu(void)
{
    printf("\n===== MENU PRINCIPAL =====\n");
    printf(" 1. Sumar dos enteros\n");
    printf(" 2. Area de un circulo (float)\n");
    printf(" 3. Saludo (cadena)\n");
    printf(" 4. Submenu por letras\n");
    printf(" 0. Salir\n");
    printf("Elige una opcion [%c a %c]: ", OPC_MIN, OPC_MAX);
}

int main()
{
	Iniciar();
    int opcion;

    do {
        mostrar_menu();
        opcion = leer_opcion_numerica(OPC_MIN, OPC_MAX);

        switch (opcion) {

        case 1: {
            int a, b;
            do {    /* repite SOLO este caso, sin volver al menu */
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
            char nombre[NOMBRE_MAX + 1];   /* tamano = maximo + 1 (por el '\0') */
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

        case 0:
            printf("Hasta luego.\n");
            Finalizar();   /* opcion de salida: restaura todo */
            exit(0);
            break;
        }

    } while (opcion != 0);

    return 0;
}
