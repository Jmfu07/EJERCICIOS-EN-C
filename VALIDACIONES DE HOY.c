/* Compilar: gcc validar_teclas.c -o validar_teclas.exe
 *
 * Calculadora simple con switch para probar el hook de teclado:
 * mientras corre, las combinaciones de la lista (Ctrl + C, Ctrl + V,
 * Esc, Alt + F4, etc.) quedan bloqueadas. Se sale con la opcion 0.
 */
#include <windows.h>
#include <stdio.h>
#include <string.h>

static HHOOK g_hook;
static DWORD g_hilo_hook;

/* Funcion de salida: detiene el hook sin usar ninguna tecla */
static void salir(void)
{
    PostThreadMessage(g_hilo_hook, WM_QUIT, 0, 0);
}

/* Devuelve 1 si la tecla pulsada es una de las combinaciones de la lista */
static int validar(DWORD vk, int ctrl, int shift, int alt)
{
    /* Ctrl + letra */
    if (ctrl && !shift && !alt && vk != 0 && strchr("CVXZYAFBPSGNTWM", (int)vk))
        return 1;

    if (vk == VK_ESCAPE)                                /* Esc y sus combinaciones */
        return (ctrl && shift && !alt) ||               /* Ctrl + Shift + Esc */
               (ctrl && !shift && !alt) ||              /* Ctrl + Esc */
               (alt && !ctrl && !shift) ||              /* Alt + Esc */
               (shift && !ctrl && !alt) ||              /* Shift + Esc */
               (!ctrl && !shift && !alt);               /* Esc */

    if (vk == VK_F4 && alt && !ctrl && !shift)          /* Alt + F4 */
        return 1;

    return 0;
}

static LRESULT CALLBACK Hook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT *kb = (KBDLLHOOKSTRUCT *)lParam;

        int ctrl  = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
        int shift = (GetAsyncKeyState(VK_SHIFT)   & 0x8000) != 0;
        int alt   = (kb->flags & LLKHF_ALTDOWN) != 0;

        if (validar(kb->vkCode, ctrl, shift, alt))
            return 1;   /* la tecla no llega a ninguna aplicacion */
    }
    return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

/* El hook corre en su propio hilo para que la calculadora no lo bloquee */
static DWORD WINAPI HiloHook(LPVOID p)
{
    g_hook = SetWindowsHookEx(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(g_hook);
    return 0;
}

int main(void)
{
    SetConsoleCtrlHandler(NULL, TRUE);   /* Ctrl + C no cierra el programa */

    HANDLE hilo = CreateThread(NULL, 0, HiloHook, NULL, 0, &g_hilo_hook);

    int opcion;
    double a, b;

    printf("Hook activo: prueba Ctrl + C, Ctrl + V, Esc, etc. (no deben funcionar)\n");

    do {
        printf("\n===== CALCULADORA =====\n");
        printf("1. Sumar\n");
        printf("2. Restar\n");
        printf("3. Multiplicar\n");
        printf("4. Dividir\n");
        printf("0. Salir\n");
        printf("Opcion: ");

        if (scanf("%d", &opcion) != 1) {      /* entrada no numerica */
            while (getchar() != '\n');
            opcion = -1;
        }

        if (opcion >= 1 && opcion <= 4) {
            printf("Primer numero: ");
            scanf("%lf", &a);
            printf("Segundo numero: ");
            scanf("%lf", &b);
        }

        switch (opcion) {
        case 1:
            printf("Resultado: %.2f\n", a + b);
            break;
        case 2:
            printf("Resultado: %.2f\n", a - b);
            break;
        case 3:
            printf("Resultado: %.2f\n", a * b);
            break;
        case 4:
            if (b != 0)
                printf("Resultado: %.2f\n", a / b);
            else
                printf("Error: no se puede dividir entre cero.\n");
            break;
        case 0:
            printf("Saliendo...\n");
            salir();
 
            break;
        default:
            printf("Opcion no valida.\n");
        }
    } while (opcion != 0);

    WaitForSingleObject(hilo, INFINITE);
    CloseHandle(hilo);
    return 0;
}
