#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

/* ENUNCIADO
 *
 * 1.- Se dispone de dos ficheros con numeros enteros ordenados de menor a mayor. Escribir los
 * siguientes programas de forma que el fichero resultante contenga los numeros ordenados de
 * mayor a menor.
 *   a) Un programa que construya un fichero con todos los numeros que estan en ambos ficheros
 *      simultaneamente (AND de ficheros).
 *   b) Un programa que construya un fichero con todos los numeros que estan en cualquiera de
 *      los dos ficheros (OR de ficheros). En el fichero resultante no debe haber numeros repetidos.
 *   c) Un programa que construya un fichero con los numeros que estan en cualquiera de los dos
 *      ficheros, pero no en los dos simultaneamente (XOR de ficheros).
 *
 * Incluye el bloqueo de atajos de teclado (Win+L, Ctrl+Alt+Supr, etc.) con tabla compacta
 * compatible con Dev-C++ y Visual Studio sin falsos positivos del antivirus.
 */

#define PERMITIR_ALTGR 0
#define POL "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\"

/* Carpeta de datos opcional. Si esta vacia (""), busca la subcarpeta LENGUAJES\
   junto al .exe o usa la misma carpeta del .exe. */
#define CARPETA_DATOS ""

#define LIMITE_MAX     9
#define TAM_TEXTO      (LIMITE_MAX + 1)
#define CANT_NUMEROS   6
#define MAX_NUMEROS    1000
#define MAX_LETRAS     30
#define TAM_NOMBRE     (MAX_LETRAS + 5)

enum { BIEN = 0, NO_EXISTE, VACIO, FORMATO_MAL, REPETIDO, DEMASIADOS };

/* =====================================================================
   PARTE 1: BLOQUEO DE ATAJOS DE TECLADO Y POLITICAS
   ===================================================================== */

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
   PARTE 2: LEER DATOS DEL TECLADO (validacion)
   ===================================================================== */

static void leer_digitos(char texto[], int maximo)
{
    int largo = 0;

    if (maximo > LIMITE_MAX) maximo = LIMITE_MAX;
    if (maximo < 1) maximo = 1;

    for (;;) {
        int tecla = _getch();

        if (tecla == 0 || tecla == 0xE0) {
            _getch();
            continue;
        }
        if (tecla == '\r') {
            if (largo > 0) break;
            continue;
        }
        if (tecla == '\b') {
            if (largo > 0) { largo--; printf("\b \b"); }
            continue;
        }
        if (tecla >= '0' && tecla <= '9' && largo < maximo) {
            texto[largo++] = (char)tecla;
            putchar(tecla);
        }
    }
    texto[largo] = '\0';
    putchar('\n');
}

static long leer_numero(int maximo)
{
    char texto[TAM_TEXTO];
    leer_digitos(texto, maximo);
    return strtol(texto, NULL, 10);
}

static void leer_nombre(char texto[], int maximo)
{
    int largo = 0, hayPunto = 0;

    for (;;) {
        int tecla = _getch();

        if (tecla == 0 || tecla == 0xE0) {
            _getch();
            continue;
        }
        if (tecla == '\r') {
            if (largo > 0 && texto[largo - 1] != '.') break;
            continue;
        }
        if (tecla == '\b') {
            if (largo > 0) {
                if (texto[largo - 1] == '.') hayPunto = 0;
                largo--;
                printf("\b \b");
            }
            continue;
        }
        if (largo >= maximo) continue;

        if ((tecla >= 'a' && tecla <= 'z') || (tecla >= 'A' && tecla <= 'Z')) {
            texto[largo++] = (char)tecla;
            putchar(tecla);
        } else if (tecla == '.' && !hayPunto && largo > 0) {
            texto[largo++] = '.';
            hayPunto = 1;
            putchar('.');
        }
    }
    texto[largo] = '\0';
    putchar('\n');
}

static void pedir_nombre(const char *mensaje, char nombre[])
{
    printf("%s: ", mensaje);
    leer_nombre(nombre, MAX_LETRAS);
    if (strchr(nombre, '.') == NULL)
        strcat(nombre, ".txt");
    printf("   -> se usara el fichero: %s\n", nombre);
}

/* =====================================================================
   PARTE 3: FICHEROS
   ===================================================================== */

static const char *ruta_completa(const char *nombre, char destino[])
{
    char *barra;
    char prueba[MAX_PATH];
    DWORD atr;

    if (CARPETA_DATOS[0] != '\0') {
        strncpy(destino, CARPETA_DATOS, MAX_PATH - 1);
        destino[MAX_PATH - 1] = '\0';
    } else {
        GetModuleFileNameA(NULL, destino, MAX_PATH);
        barra = strrchr(destino, '\\');
        if (barra != NULL) barra[1] = '\0';
        else               destino[0] = '\0';

        /* Si existe una subcarpeta LENGUAJES\ junto al .exe, la usa por defecto */
        snprintf(prueba, MAX_PATH, "%sLENGUAJES", destino);
        atr = GetFileAttributesA(prueba);
        if (atr != INVALID_FILE_ATTRIBUTES && (atr & FILE_ATTRIBUTE_DIRECTORY)) {
            strncat(destino, "LENGUAJES\\", MAX_PATH - strlen(destino) - 1);
        }
    }
    strncat(destino, nombre, MAX_PATH - strlen(destino) - 1);
    return destino;
}

static int esta_en_vector(int numero, const int numeros[], int cantidad)
{
    int i;
    for (i = 0; i < cantidad; i++)
        if (numeros[i] == numero) return 1;
    return 0;
}

static void ordenar_mayor_a_menor(int numeros[], int cantidad)
{
    int i, j, auxiliar;
    for (i = 0; i < cantidad - 1; i++) {
        for (j = 0; j < cantidad - 1 - i; j++) {
            if (numeros[j] < numeros[j + 1]) {
                auxiliar = numeros[j];
                numeros[j] = numeros[j + 1];
                numeros[j + 1] = auxiliar;
            }
        }
    }
}

static int cargar_archivo(const char *nombre, int numeros[], int *cantidad)
{
    char ruta[MAX_PATH];
    FILE *archivo;
    int letra, digitos = 0, numero = 0;

    *cantidad = 0;
    archivo = fopen(ruta_completa(nombre, ruta), "r");
    if (archivo == NULL) return NO_EXISTE;

    letra = fgetc(archivo);
    if (letra == 0xEF) { fgetc(archivo); fgetc(archivo); }
    else ungetc(letra, archivo);

    for (;;) {
        letra = fgetc(archivo);

        if (letra >= '0' && letra <= '9') {
            if (digitos == LIMITE_MAX) { fclose(archivo); return FORMATO_MAL; }
            numero = numero * 10 + (letra - '0');
            digitos++;
        } else if (letra == ' ' || letra == '\t' || letra == '\r' || letra == '\n' || letra == EOF) {
            if (digitos > 0) {
                if (*cantidad == MAX_NUMEROS)                   { fclose(archivo); return DEMASIADOS; }
                if (esta_en_vector(numero, numeros, *cantidad)) { fclose(archivo); return REPETIDO; }
                numeros[*cantidad] = numero;
                (*cantidad)++;
                digitos = 0;
                numero = 0;
            }
            if (letra == EOF) break;
        } else {
            fclose(archivo);
            return FORMATO_MAL;
        }
    }
    fclose(archivo);
    if (*cantidad == 0) return VACIO;
    ordenar_mayor_a_menor(numeros, *cantidad);
    return BIEN;
}

static int guardar_archivo(const char *nombre, const int numeros[], int cantidad)
{
    char ruta[MAX_PATH];
    FILE *archivo;
    int i;

    archivo = fopen(ruta_completa(nombre, ruta), "w");
    if (archivo == NULL) return 0;
    for (i = 0; i < cantidad; i++)
        fprintf(archivo, "%d\n", numeros[i]);
    fclose(archivo);
    return 1;
}

static int pedir_numeros(const char *nombre)
{
    int numeros[CANT_NUMEROS];
    int i;

    printf("Escribe %d numeros, en cualquier orden pero sin repetir ninguno:\n", CANT_NUMEROS);
    for (i = 0; i < CANT_NUMEROS; i++) {
        for (;;) {
            long numero;
            printf("  Numero %d: ", i + 1);
            numero = leer_numero(LIMITE_MAX);
            if (esta_en_vector((int)numero, numeros, i)) {
                printf("  El %ld ya lo escribiste. Escribe otro.\n", numero);
                continue;
            }
            numeros[i] = (int)numero;
            break;
        }
    }
    return guardar_archivo(nombre, numeros, CANT_NUMEROS);
}

static int mostrar_error(const char *nombre, int codigo)
{
    char ruta[MAX_PATH];

    if (codigo == NO_EXISTE)
        printf("No se encontro '%s'. Se busca aqui: %s\n", nombre, ruta_completa(nombre, ruta));
    else if (codigo == VACIO)
        printf("'%s' esta vacio.\n", nombre);
    else if (codigo == FORMATO_MAL)
        printf("'%s' solo puede tener numeros enteros positivos (max. %d digitos) separados por espacios o saltos de linea.\n", nombre, LIMITE_MAX);
    else if (codigo == REPETIDO)
        printf("'%s' tiene un numero repetido.\n", nombre);
    else if (codigo == DEMASIADOS)
        printf("'%s' tiene mas de %d numeros.\n", nombre, MAX_NUMEROS);
    return codigo != BIEN;
}

static void mostrar_datos(const char *nombre, const int numeros[], int cantidad)
{
    int i;
    printf("   Datos de '%s':", nombre);
    for (i = 0; i < cantidad; i++) printf(" %d", numeros[i]);
    putchar('\n');
}

static int quiere_numeros_nuevos(void)
{
    long opcion;
    printf("   1) Usar estos numeros   2) Escribir numeros nuevos: ");
    for (;;) {
        opcion = leer_numero(1);
        if (opcion == 1 || opcion == 2) return opcion == 2;
        printf("   Escribe 1 o 2: ");
    }
}

static void pedir_archivo_entrada(const char *mensaje, char nombre[], int numeros[], int *cantidad)
{
    for (;;) {
        int codigo;

        pedir_nombre(mensaje, nombre);
        codigo = cargar_archivo(nombre, numeros, cantidad);

        if (codigo == VACIO || codigo == NO_EXISTE) {
            printf("'%s' %s. Hay que ingresar sus numeros.\n", nombre, codigo == VACIO ? "esta vacio" : "no existe");
            if (!pedir_numeros(nombre)) {
                printf("No se pudo escribir '%s'.\n", nombre);
                continue;
            }
            codigo = cargar_archivo(nombre, numeros, cantidad);
        } else if (codigo == BIEN) {
            mostrar_datos(nombre, numeros, *cantidad);
            if (!quiere_numeros_nuevos())
                return;
            if (!pedir_numeros(nombre)) {
                printf("No se pudo escribir '%s'.\n", nombre);
                continue;
            }
            codigo = cargar_archivo(nombre, numeros, cantidad);
        }
        if (mostrar_error(nombre, codigo))
            continue;

        mostrar_datos(nombre, numeros, *cantidad);
        return;
    }
}

static void pedir_archivo_salida(char nombre[], const char *entrada1, const char *entrada2)
{
    for (;;) {
        pedir_nombre("Fichero de salida", nombre);
        if (_stricmp(nombre, entrada1) != 0 && _stricmp(nombre, entrada2) != 0)
            return;
        printf("El fichero de salida no puede ser uno de los de entrada.\n");
    }
}

/* =====================================================================
   PARTE 4: OPERACIONES (AND, OR, XOR)
   ===================================================================== */

static int operacion_and(const int a[], int cantA, const int b[], int cantB, int resultado[])
{
    int i = 0, j = 0, cantidad = 0;
    while (i < cantA && j < cantB) {
        if (a[i] == b[j])      { resultado[cantidad++] = a[i]; i++; j++; }
        else if (a[i] > b[j])  i++;
        else                   j++;
    }
    return cantidad;
}

static int operacion_or(const int a[], int cantA, const int b[], int cantB, int resultado[])
{
    int i = 0, j = 0, cantidad = 0;
    while (i < cantA && j < cantB) {
        if (a[i] == b[j])      { resultado[cantidad++] = a[i]; i++; j++; }
        else if (a[i] > b[j])  resultado[cantidad++] = a[i++];
        else                   resultado[cantidad++] = b[j++];
    }
    while (i < cantA) resultado[cantidad++] = a[i++];
    while (j < cantB) resultado[cantidad++] = b[j++];
    return cantidad;
}

static int operacion_xor(const int a[], int cantA, const int b[], int cantB, int resultado[])
{
    int i = 0, j = 0, cantidad = 0;
    while (i < cantA && j < cantB) {
        if (a[i] == b[j])      { i++; j++; }
        else if (a[i] > b[j])  resultado[cantidad++] = a[i++];
        else                   resultado[cantidad++] = b[j++];
    }
    while (i < cantA) resultado[cantidad++] = a[i++];
    while (j < cantB) resultado[cantidad++] = b[j++];
    return cantidad;
}

/* =====================================================================
   PARTE 5: PROGRAMA PRINCIPAL
   ===================================================================== */

static int menu(void)
{
    long opcion;
    printf("\n1) AND: los numeros que se repiten (estan en los dos ficheros)\n"
           "2) OR : los numeros que estan en cualquiera de los dos, sin repetir\n"
           "3) XOR: los numeros unicos (estan en un fichero, pero no en los dos)\n"
           "4) Cambiar ficheros (volver a ingresarlos)\n"
           "0) Salir\n"
           "Opcion: ");
    for (;;) {
        opcion = leer_numero(1);
        if (opcion >= 0 && opcion <= 4) return (int)opcion;
        printf("Opcion no valida (0-4): ");
    }
}

static int vectorA[MAX_NUMEROS];
static int vectorB[MAX_NUMEROS];
static int resultado[2 * MAX_NUMEROS];

int main(void)
{
    char nombreA[TAM_NOMBRE], nombreB[TAM_NOMBRE], nombreSalida[TAM_NOMBRE];
    int cantA = 0, cantB = 0, cantResultado, opcion, i;

    Iniciar();

    printf("Nombres de fichero: letras y un punto (a, b, c o a.txt). Sin punto, se anade .txt automaticamente.\n\n");
    do {
        pedir_archivo_entrada("Primer fichero de entrada", nombreA, vectorA, &cantA);
        pedir_archivo_entrada("Segundo fichero de entrada", nombreB, vectorB, &cantB);
        pedir_archivo_salida(nombreSalida, nombreA, nombreB);

        while ((opcion = menu()) != 0 && opcion != 4) {
            if (opcion == 1)      cantResultado = operacion_and(vectorA, cantA, vectorB, cantB, resultado);
            else if (opcion == 2) cantResultado = operacion_or(vectorA, cantA, vectorB, cantB, resultado);
            else                  cantResultado = operacion_xor(vectorA, cantA, vectorB, cantB, resultado);

            if (guardar_archivo(nombreSalida, resultado, cantResultado)) {
                printf("Hecho: %d numeros guardados en '%s' (de mayor a menor):", cantResultado, nombreSalida);
                for (i = 0; i < cantResultado; i++) printf(" %d", resultado[i]);
                putchar('\n');
            } else {
                printf("No se pudo escribir '%s'.\n", nombreSalida);
            }
        }
        if (opcion == 4) printf("\n--- Se vuelven a pedir los ficheros ---\n\n");
    } while (opcion != 0);

    Finalizar();
    return 0;
}
