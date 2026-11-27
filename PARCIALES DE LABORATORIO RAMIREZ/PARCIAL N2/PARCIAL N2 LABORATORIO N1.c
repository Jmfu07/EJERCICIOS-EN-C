/*
EJERCICIO N2 LABORATORIO - PARCIAL N2
JUAN MEDINA V-31708027
ELABORADO 12-11-2026

- Elaborar el programa en lenguaje C que pinte un tablero de ajedrez, los peones con la letra P, las torres con T, los caballos con C, los alfiles con A, elrey con R
y la reina con M

Se pide:
1- Mover los peones centrales del teclado
2- Que los peones coman
3- aplicar el ciclo while con respuesta al operador
4- Validar todas las variables
5- Validar cada una de las opciones e imprimir el resultado de cada actividad
6- Validar en caso que la opcion sea incorrecta

Solo los peones del centro se van a mover, comer o ser comidos (D y E)
empieza el blanco.
Los peones de C y F solo pueden ser comidos.

*/

#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define PERMITIR_ALTGR 0   /* 1 = deja pasar AltGr (para @ \ | # en teclado espanol) */

static HHOOK g_hook;
static HANDLE g_th, g_listo;
static DWORD g_hilo;
static BOOL pasada[256];   /* pasada[tecla]: su pulsacion llego a Windows, asi que su soltado tambien debe llegar */

/* Lista blanca: texto, numeros, flechas, Espacio, Supr, Mayus y teclado numerico.
   Lo demas (F1-F24, Esc, Tab, Inicio/Fin, menu contextual, multimedia) se traga. */
static BOOL Permitida(DWORD v)
{
    if (v == VK_SPACE || v == VK_DELETE || v == VK_CAPITAL) return TRUE;
    if (v >= VK_LEFT && v <= VK_DOWN) return TRUE;
    if (v >= '0' && v <= '9') return TRUE;
    if (v >= 'A' && v <= 'Z') return TRUE;
    if (v >= VK_NUMPAD0 && v <= VK_DIVIDE) return TRUE;
    if (v >= VK_OEM_1 && v <= VK_OEM_8) return TRUE;
    return v == VK_OEM_102;
}

/* Tecla fantasma: se inyecta al soltar Win o Alt para que Windows no tome el toque por abrir un menu */
static void Fantasma(void)
{
    keybd_event(0xE8, 0, 0, 0);
    keybd_event(0xE8, 0, KEYEVENTF_KEYUP, 0);
}

static LRESULT CALLBACK Hook(int c, WPARAM w, LPARAM l)
{
    KBDLLHOOKSTRUCT *k = (KBDLLHOOKSTRUCT *)l;
    DWORD vk;
    BOOL abajo, ctrl, alt, win, bloquea;

    if (c != HC_ACTION || (k->flags & LLKHF_INJECTED))
        return CallNextHookEx(g_hook, c, w, l);

    vk = k->vkCode & 0xFF;
    abajo = (w == WM_KEYDOWN || w == WM_SYSKEYDOWN);

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
        ctrl = alt = FALSE;

    /* Soltado: pasa solo si su pulsacion paso */
    if (!abajo) {
        if (pasada[vk]) {
            pasada[vk] = FALSE;
            return CallNextHookEx(g_hook, c, w, l);
        }
        return 1;
    }

    /* Enter y Borrar pasan siempre, salvo con Ctrl, Alt o Win */
    if (vk == VK_RETURN || vk == VK_BACK)
        bloquea = ctrl || alt || win;
    else
        bloquea = ctrl || alt || win || !Permitida(vk);

    if (bloquea)
        return 1;

    pasada[vk] = TRUE;
    return CallNextHookEx(g_hook, c, w, l);
}

/* Hilo propio del gancho. Renueva el gancho cada 20 s para que Windows no lo descarte */
static DWORD WINAPI Hilo(LPVOID p)
{
    MSG m;
    (void)p;

    g_hilo = GetCurrentThreadId();
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    g_hook = SetWindowsHookExA(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0);
    SetTimer(NULL, 0, 20000, NULL);
    SetEvent(g_listo);

    while (GetMessage(&m, NULL, 0, 0) > 0) {
        if (m.message == WM_TIMER) {
            HHOOK nuevo = SetWindowsHookExA(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0);
            if (nuevo) {
                UnhookWindowsHookEx(g_hook);
                g_hook = nuevo;
            }
        } else {
            DispatchMessage(&m);
        }
    }
    if (g_hook) UnhookWindowsHookEx(g_hook);
    return 0;
}

/* Ctrl+C y Ctrl+Break se ignoran; el resto de cierres siguen su curso normal */
static BOOL WINAPI Cierre(DWORD e)
{
    return e == CTRL_C_EVENT || e == CTRL_BREAK_EVENT;
}

static void Finalizar(void);

/* INICIAR: deja la consola lista y activa el gancho. FALSE si no se pudo. */
static BOOL Iniciar(void)
{
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    HWND w;
    DWORD modo;

    SetConsoleCtrlHandler(Cierre, TRUE);
    w = GetConsoleWindow();
    if (GetConsoleMode(in, &modo))  /* un clic en la consola congelaria el gancho */
        SetConsoleMode(in, (modo | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE);

    g_listo = CreateEvent(NULL, FALSE, FALSE, NULL);
    g_th = CreateThread(NULL, 0, Hilo, NULL, 0, NULL);
    WaitForSingleObject(g_listo, 5000);

    if (!g_hook) {
        printf("No se pudo activar el bloqueo. Pulsa Enter.\n");
        getchar();
        Finalizar();
        return FALSE;
    }
    return TRUE;
}

/* FINALIZAR: quita el gancho y espera a que su hilo termine. Segura de llamar dos veces */
static void Finalizar(void)
{
    if (g_th) {
        PostThreadMessage(g_hilo, WM_QUIT, 0, 0);
        WaitForSingleObject(g_th, 3000);
        CloseHandle(g_th);
        g_th = NULL;
    }
}

/* =====================================================================
 *              AJEDREZ (SOLO PEONES CENTRALES) - SIN PUNTEROS
 * ===================================================================== */

#define N      8
#define BLANCO 1
#define NEGRO  2
#define META   2     /* la partida termina cuando se comen 2 piezas */

char tab[N][N];      /* letra de la pieza, ' ' si esta vacia */
int  dueno[N][N];    /* 0 = vacia, BLANCO o NEGRO */
int  movil[N][N];    /* 1 = peon central (columna D o E) */
int  turno, jugadas, comidas;

int fila_leida, col_leida;

int msg_tipo, m_num, m_turno, m_fo, m_co, m_fd, m_cd, m_comida;

void imprimir_bando(int c)
{
    if (c == BLANCO) printf("BLANCAS");
    else printf("NEGRAS");
}

void mostrar_mensaje()
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

void inicializar()
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

    movil[1][3] = movil[1][4] = movil[6][3] = movil[6][4] = 1;   /* D y E */
    turno = BLANCO;
    jugadas = 0;
    comidas = 0;
    msg_tipo = 0;
}

/* Casillas: blanca = fondo blanco, negra = fondo negro (solo 2 colores).
   Pieza negra = letra con asterisco (P*), pieza blanca = letra sola. */
void dibujar()
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

    if (dc == 0) {                                          /* avance recto, sin comer */
        if (df == dir)
            return dueno[fd][cd] == 0 ? 0 : 5;
        if (df == 2 * dir && fo == ini)
            return (dueno[fo + dir][co] == 0 && dueno[fd][cd] == 0) ? 0 : 5;
        return 5;
    }
    if ((dc == 1 || dc == -1) && df == dir) {               /* captura en diagonal */
        if (cd < 2 || cd > 5) return 7;                     /* solo columnas C a F */
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

void jugar()
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
            switch (codigo) 
			{
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

int leer_opcion()
{
    int c;

    for (;;) {
        c = _getch();

        if (c == 0 || c == 0xE0) {          /* tecla especial: se ignora */
            _getch();
            continue;
        }
        if (c == '1' || c == '0') {
            putchar(c);
            putchar('\n');
            return c - '0';
        }
    }
}

int main()
{
    int salir = 0, op;

    if (!Iniciar())
        return 1;

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
