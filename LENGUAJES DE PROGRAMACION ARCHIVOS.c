#define _WIN32_WINNT 0x0600
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
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
 * El siguiente programa pide por teclado al usuario los dos ficheros de entrada y el nombre del
 * fichero en que quiere generar la salida. A continuacion le da la posibilidad de generar
 * cualquiera de los tres enunciados que se proponen en el ejercicio. Como en realidad tenemos
 * tres programas en uno, hemos recurrido al uso de funciones (para tener un codigo bien
 * estructurado) y a la directiva goto para acceder a una u otra parte del programa. Las
 * funciones que generan el vector vout estan disenadas para que en el los elementos ya queden
 * ordenados de mayor a menor. He supuesto que en ninguno de los ficheros de entrada va a haber
 * un entero repetido (en el mismo fichero). Notese que dicha estructuracion del programa
 * facilita la lectura del mismo y que en los programas de mayor envergadura la funcion main
 * practicamente tan solo contiene llamadas a otras funciones.
 *
 * COMO FUNCIONA ESTA VERSION
 * - Los nombres de fichero se escriben con letras y, como mucho, un punto (a, b, c o a.txt).
 *   Si no se escribe extension, se anade ".txt".
 * - Los ficheros estan en la carpeta CARPETA_DATOS (mas abajo).
 * - Si un fichero esta vacio o no existe, el programa pide 6 numeros por teclado (en cualquier
 *   orden, pero sin repetir) y los guarda en el fichero.
 * - Los ficheros solo pueden tener numeros enteros positivos (hasta 9 digitos) separados por
 *   espacios o saltos de linea, sin repetidos. No hace falta que esten ordenados: el programa
 *   los ordena de mayor a menor en memoria despues de leerlos.
 * - Si un fichero ya tiene numeros, se muestran y se pregunta si usarlos o escribir numeros nuevos.
 * - Con los dos ficheros cargados, un menu permite hacer AND, OR o XOR (y la opcion 4 vuelve a pedir los ficheros). El resultado sale
 *   tambien de mayor a menor, se guarda en el fichero de salida y se muestra en pantalla.
 * - Se usa un menu y funciones (sin goto).
 * - Incluye el bloqueo de atajos de teclado: se activa al iniciar y se restaura al salir.
 *
 * Compilar con: gcc ficheros_and_or_xor_bloqueo.c -o ficheros.exe -lshell32 -ladvapi32
 */

/* Ctrl+C y Ctrl+Break se ignoran. La X de la consola SI funciona: restaura antes de cerrar */

#define PERMITIR_ALTGR 0    /* 1 = deja pasar AltGr (para @ \ | # en teclado espanol) */
#define POLITICA "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\"
#define RUNONCE  "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"

/* Carpeta donde estan (y se crean) los ficheros. Debe terminar en \\.
   Si se deja vacia (""), se usa la carpeta donde esta el .exe. */
#define CARPETA_DATOS "C:\\Users\\jmfus\\Desktop\\LENGUAJES\\"

#define LIMITE_MAX     9      /* maximo de digitos de un numero */
#define TAM_TEXTO      (LIMITE_MAX + 1)
#define CANT_NUMEROS   6      /* cuantos numeros se piden para llenar un fichero vacio */
#define MAX_NUMEROS    1000   /* maximo de numeros que cabe en un fichero */
#define MAX_LETRAS     30     /* maximo de caracteres de un nombre de fichero */
#define TAM_NOMBRE     (MAX_LETRAS + 5)   /* letras + ".txt" + fin de texto */

/* Codigos que devuelve cargar_archivo */
enum { BIEN = 0, NO_EXISTE, VACIO, FORMATO_MAL, REPETIDO, DEMASIADOS };

/* =====================================================================
   PARTE 1: BLOQUEO DE ATAJOS DE TECLADO
   (misma logica que el bloque de teclas original, con nombres en espanol)
   ===================================================================== */

/* ---------- Estado ---------- */

static HHOOK g_gancho;           /* gancho de teclado */
static HANDLE g_th;              /* hilo que mantiene el gancho */
static DWORD g_hilo;             /* id de ese hilo */
static HANDLE g_listo;           /* aviso de que el gancho ya esta puesto */
static HANDLE g_hPrincipal;      /* hilo del menu */
static HANDLE g_guardia;         /* ayudante elevado que aplica las politicas de HKLM */
static HANDLE g_evOk, g_evStop;  /* eventos para hablar con el ayudante */
static BOOL pasada[256];         /* pasada[tecla]: esa pulsacion llego a Windows, asi que su soltado tambien debe llegar */

/* ---------- Utilidades ---------- */

/* Dice si el programa se esta ejecutando como administrador */
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

/* Muestra un mensaje y espera un Enter */
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
static LRESULT CALLBACK GanchoTeclado(int c, WPARAM w, LPARAM l)
{
    KBDLLHOOKSTRUCT *k = (KBDLLHOOKSTRUCT *)l;
    DWORD vk;
    BOOL abajo, ctrl, alt, win, bloquea;

    if (c != HC_ACTION || (k->flags & LLKHF_INJECTED))
        return CallNextHookEx(g_gancho, c, w, l);

    vk = k->vkCode & 0xFF;
    abajo = (w == WM_KEYDOWN || w == WM_SYSKEYDOWN);

    /* Fn+4 del Lenovo L490 (apaga la pantalla): llega como vk 0xFF, scan 0x63 */
    if (vk == 0xFF)
        return 1;

    /* Win y Alt pasan a Windows; al soltarlas se manda la tecla fantasma */
    if (vk == VK_LWIN || vk == VK_RWIN || vk == VK_MENU || vk == VK_LMENU || vk == VK_RMENU) {
        if (!abajo) Fantasma();
        return CallNextHookEx(g_gancho, c, w, l);
    }

    /* Ctrl y Mayus pasan siempre */
    if (vk == VK_SHIFT || vk == VK_LSHIFT || vk == VK_RSHIFT || vk == VK_CONTROL || vk == VK_LCONTROL || vk == VK_RCONTROL)
        return CallNextHookEx(g_gancho, c, w, l);

    ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
    alt  = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
    win  = ((GetAsyncKeyState(VK_LWIN) | GetAsyncKeyState(VK_RWIN)) & 0x8000) != 0;
    if (PERMITIR_ALTGR && GetAsyncKeyState(VK_RMENU) < 0)
        ctrl = alt = FALSE; /* AltGr: Windows manda un Ctrl "falso" */

    /* Soltado: si su pulsacion paso, pasa; si fue bloqueada, se traga */
    if (!abajo) {
        if (pasada[vk]) {
            pasada[vk] = FALSE;
            return CallNextHookEx(g_gancho, c, w, l);
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
    return CallNextHookEx(g_gancho, c, w, l);
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
    UINT_PTR tPol, tGancho;
    (void)p;

    g_hilo = GetCurrentThreadId();
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL); /* un gancho lento lo descarta Windows */
    tPol    = SetTimer(NULL, 0, 2000, NULL);  /* reaplica politicas de usuario cada 2 s */
    tGancho = SetTimer(NULL, 0, 20000, NULL); /* renueva el gancho cada 20 s */

    Politicas(TRUE, g_guardia != NULL);       /* antes del gancho: no hay hueco para Win+L */
    g_gancho = SetWindowsHookExA(WH_KEYBOARD_LL, GanchoTeclado, GetModuleHandle(NULL), 0);
    SetEvent(g_listo);

    while (g_gancho && GetMessage(&m, NULL, 0, 0) > 0) {
        if (m.message != WM_TIMER) {
            DispatchMessage(&m);
        } else if (m.wParam == tPol) {
            Politicas(TRUE, TRUE);
        } else if (m.wParam == tGancho) {
            HHOOK nuevo = SetWindowsHookExA(WH_KEYBOARD_LL, GanchoTeclado, GetModuleHandle(NULL), 0);
            if (nuevo) {
                UnhookWindowsHookEx(g_gancho);
                g_gancho = nuevo;
            }
        }
    }

    if (g_gancho) UnhookWindowsHookEx(g_gancho);
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

    if (!g_gancho) {
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

/* =====================================================================
   PARTE 2: LEER DATOS DEL TECLADO (validacion)
   ===================================================================== */

/* Lee solo digitos, tecla a tecla (sin signo ni punto).
 * maximo = cuantos digitos como mucho. Enter confirma (al menos un digito). Backspace borra.
 * texto debe tener TAM_TEXTO espacios. */
static void leer_digitos(char texto[], int maximo)
{
    int largo = 0;

    if (maximo > LIMITE_MAX) maximo = LIMITE_MAX;
    if (maximo < 1) maximo = 1;

    for (;;) {
        int tecla = _getch();

        if (tecla == 0 || tecla == 0xE0) {      /* tecla especial (flechas, F1...): se ignora */
            _getch();
            continue;
        }
        if (tecla == '\r') {                    /* Enter */
            if (largo > 0) break;
            continue;
        }
        if (tecla == '\b') {                    /* Backspace */
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

/* Lee un numero entero positivo (usa leer_digitos y lo convierte a numero) */
static long leer_numero(int maximo)
{
    char texto[TAM_TEXTO];

    leer_digitos(texto, maximo);
    return strtol(texto, NULL, 10);
}

/* Lee un nombre de fichero tecla a tecla:
 *  - acepta letras y, como mucho, UN punto (para la extension)
 *  - el punto no puede ser el primero ni el ultimo caracter
 *  - maximo = cuantos caracteres como mucho. Enter confirma. Backspace borra. */
static void leer_nombre(char texto[], int maximo)
{
    int largo = 0, hayPunto = 0;

    for (;;) {
        int tecla = _getch();

        if (tecla == 0 || tecla == 0xE0) {      /* tecla especial: se ignora */
            _getch();
            continue;
        }
        if (tecla == '\r') {                    /* Enter: hace falta algo escrito y que no acabe en punto */
            if (largo > 0 && texto[largo - 1] != '.') break;
            continue;
        }
        if (tecla == '\b') {                    /* Backspace */
            if (largo > 0) {
                if (texto[largo - 1] == '.') hayPunto = 0;
                largo--;
                printf("\b \b");
            }
            continue;
        }
        if (largo >= maximo) continue;          /* ya no caben mas caracteres */

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

/* Pide un nombre de fichero. Si se escribio sin punto (a, b, c), le anade ".txt". */
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

/* Devuelve la ruta completa de un fichero: la carpeta de datos + el nombre.
 * Si CARPETA_DATOS esta vacia, usa la carpeta donde esta el .exe. */
static const char *ruta_completa(const char *nombre, char destino[])
{
    char *barra;

    if (CARPETA_DATOS[0] != '\0') {
        strncpy(destino, CARPETA_DATOS, MAX_PATH - 1);
        destino[MAX_PATH - 1] = '\0';
    } else {
        GetModuleFileNameA(NULL, destino, MAX_PATH);
        barra = strrchr(destino, '\\');
        if (barra != NULL) barra[1] = '\0';
        else               destino[0] = '\0';
    }
    strncat(destino, nombre, MAX_PATH - strlen(destino) - 1);
    return destino;
}

/* Dice si un numero ya esta en el vector (1 = si, 0 = no). Sirve para no aceptar repetidos. */
static int esta_en_vector(int numero, const int numeros[], int cantidad)
{
    int i;

    for (i = 0; i < cantidad; i++)
        if (numeros[i] == numero) return 1;
    return 0;
}

/* Ordena el vector de MAYOR a MENOR (metodo de la burbuja): compara cada numero con el siguiente
 * y los intercambia si el primero es menor. Se repite hasta que todo queda en su sitio. */
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

/* Lee un fichero y guarda sus numeros en el vector. Va revisando letra a letra:
 *  - solo se aceptan digitos y separadores (espacio, tabulador, salto de linea)
 *  - cada numero tiene como maximo LIMITE_MAX digitos
 *  - los numeros no pueden repetirse (pueden venir en cualquier orden)
 * Al terminar, deja los numeros ordenados de mayor a menor.
 * Devuelve BIEN o el motivo del error (NO_EXISTE, VACIO, FORMATO_MAL, REPETIDO, DEMASIADOS). */
static int cargar_archivo(const char *nombre, int numeros[], int *cantidad)
{
    char ruta[MAX_PATH];
    FILE *archivo;
    int letra, digitos = 0, numero = 0;

    *cantidad = 0;
    archivo = fopen(ruta_completa(nombre, ruta), "r");
    if (archivo == NULL) return NO_EXISTE;

    letra = fgetc(archivo);                     /* salta la marca BOM que a veces pone el Bloc de notas */
    if (letra == 0xEF) { fgetc(archivo); fgetc(archivo); }
    else ungetc(letra, archivo);

    for (;;) {
        letra = fgetc(archivo);

        if (letra >= '0' && letra <= '9') {     /* un digito mas del numero */
            if (digitos == LIMITE_MAX) { fclose(archivo); return FORMATO_MAL; }
            numero = numero * 10 + (letra - '0');
            digitos++;
        } else if (letra == ' ' || letra == '\t' || letra == '\r' || letra == '\n' || letra == EOF) {
            if (digitos > 0) {                  /* termino un numero: se revisa y se guarda */
                if (*cantidad == MAX_NUMEROS)                       { fclose(archivo); return DEMASIADOS; }
                if (esta_en_vector(numero, numeros, *cantidad))     { fclose(archivo); return REPETIDO; }
                numeros[*cantidad] = numero;
                (*cantidad)++;
                digitos = 0;
                numero = 0;
            }
            if (letra == EOF) break;            /* fin del fichero */
        } else {                                /* cualquier otro caracter no vale */
            fclose(archivo);
            return FORMATO_MAL;
        }
    }
    fclose(archivo);
    if (*cantidad == 0) return VACIO;
    ordenar_mayor_a_menor(numeros, *cantidad);  /* para poder hacer AND, OR y XOR */
    return BIEN;
}

/* Escribe los numeros del vector en un fichero, uno por linea. Devuelve 1 si pudo, 0 si no. */
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

/* Pide CANT_NUMEROS numeros por teclado y los guarda en el fichero.
 * Pueden escribirse en cualquier orden, pero no se puede repetir ninguno. */
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
                continue;                       /* se repite la pregunta */
            }
            numeros[i] = (int)numero;
            break;
        }
    }
    return guardar_archivo(nombre, numeros, CANT_NUMEROS);
}

/* Explica por pantalla por que fallo la lectura. Devuelve 1 si hubo error, 0 si todo estaba bien. */
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

/* Muestra por pantalla los numeros que se cargaron de un fichero */
static void mostrar_datos(const char *nombre, const int numeros[], int cantidad)
{
    int i;

    printf("   Datos de '%s':", nombre);
    for (i = 0; i < cantidad; i++) printf(" %d", numeros[i]);
    putchar('\n');
}

/* Pregunta si se quieren escribir numeros nuevos en un fichero que ya tiene datos.
 * Devuelve 1 si el usuario quiere numeros nuevos, 0 si quiere usar los que hay. */
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

/* Pide el nombre de un fichero de entrada y carga sus numeros.
 *  - Si esta vacio o no existe, pide los 6 numeros por teclado.
 *  - Si ya tiene numeros, los muestra y pregunta si se quieren usar o reemplazar por otros nuevos.
 * Repite hasta que el fichero quede cargado y valido. */
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
                return;                         /* se usan los numeros que ya tiene */
            if (!pedir_numeros(nombre)) {       /* se reemplazan por numeros nuevos */
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

/* Pide el nombre del fichero de salida. No puede ser igual a uno de los de entrada. */
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
   Los dos vectores ya estan ordenados de mayor a menor (cargar_archivo los ordena). Se recorren a la vez, desde el principio,
   con dos "dedos": i en el vector a y j en el vector b. En cada paso se compara a[i] con b[j].
   Asi el resultado tambien sale ordenado de mayor a menor. Cada funcion devuelve cuantos
   numeros puso en el resultado.
   ===================================================================== */

/* AND: los numeros que estan en a Y en b */
static int operacion_and(const int a[], int cantA, const int b[], int cantB, int resultado[])
{
    int i = 0, j = 0, cantidad = 0;

    while (i < cantA && j < cantB) {
        if (a[i] == b[j]) {                     /* esta en los dos: se guarda */
            resultado[cantidad++] = a[i];
            i++;
            j++;
        } else if (a[i] > b[j]) {               /* a[i] es mayor y b ya paso de el: no esta en b */
            i++;
        } else {                                /* b[j] es mayor y a ya paso de el: no esta en a */
            j++;
        }
    }
    return cantidad;
}

/* OR: los numeros que estan en a O en b (sin repetir) */
static int operacion_or(const int a[], int cantA, const int b[], int cantB, int resultado[])
{
    int i = 0, j = 0, cantidad = 0;

    while (i < cantA && j < cantB) {
        if (a[i] == b[j]) {                     /* esta en los dos: se guarda una sola vez */
            resultado[cantidad++] = a[i];
            i++;
            j++;
        } else if (a[i] > b[j]) {               /* se guarda el mayor de los dos */
            resultado[cantidad++] = a[i++];
        } else {
            resultado[cantidad++] = b[j++];
        }
    }
    while (i < cantA) resultado[cantidad++] = a[i++];   /* lo que sobre de a */
    while (j < cantB) resultado[cantidad++] = b[j++];   /* lo que sobre de b */
    return cantidad;
}

/* XOR: los numeros que estan en a o en b, pero NO en los dos */
static int operacion_xor(const int a[], int cantA, const int b[], int cantB, int resultado[])
{
    int i = 0, j = 0, cantidad = 0;

    while (i < cantA && j < cantB) {
        if (a[i] == b[j]) {                     /* esta en los dos: no se guarda */
            i++;
            j++;
        } else if (a[i] > b[j]) {               /* solo esta en uno: se guarda el mayor */
            resultado[cantidad++] = a[i++];
        } else {
            resultado[cantidad++] = b[j++];
        }
    }
    while (i < cantA) resultado[cantidad++] = a[i++];   /* lo que sobre de a */
    while (j < cantB) resultado[cantidad++] = b[j++];   /* lo que sobre de b */
    return cantidad;
}

/* =====================================================================
   PARTE 5: PROGRAMA PRINCIPAL
   ===================================================================== */

/* Muestra el menu y devuelve la opcion elegida (0 a 4) */
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

/* Vectores grandes: fuera de main para no gastar la pila */
static int vectorA[MAX_NUMEROS];
static int vectorB[MAX_NUMEROS];
static int resultado[2 * MAX_NUMEROS];

int main()
{
    char nombreA[TAM_NOMBRE], nombreB[TAM_NOMBRE], nombreSalida[TAM_NOMBRE];
    int cantA = 0, cantB = 0, cantResultado, opcion, i;

    Iniciar();   /* activa el bloqueo de atajos (o atiende los modos internos guardia / limpiar) */

    printf("Nombres de fichero: letras y un punto (a, b, c o a.txt). Sin punto, se anade .txt automaticamente.\n\n");
    do {
        /* Cada vuelta se piden los ficheros de nuevo (la primera vez y cada vez que se elige "4") */
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
    return 0;   /* atexit(Finalizar) detiene el gancho y restaura las politicas */
}
