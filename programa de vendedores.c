// Programa de matrices para vendedores (100% Optimizado - Estilo C89 estricto)

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#pragma comment(lib, "user32.lib")

#define MAX 200
#define ARTICULOS 3
#define NOMBRE 50

/* Variable global para el gancho (Hook) */
HHOOK hHook = NULL; 

/* Prototipos de la API de Windows */
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
DWORD WINAPI HiloBloqueo(LPVOID lpParam);
void ActivacionBloqueoSistema();

/* =========================================================================
   1. FUNCIONES DE VALIDACION (El blindaje sin scanf ni punteros)
   ========================================================================= */

int leer_entero_estricto() {
    int entrada_valida;
    char buffer[100];
    int longitud;
    int c_drenaje;
    int es_valido;
    int tiene_numero;
    int i;
    char c;
    int es_digito;
    int numero_final;

    entrada_valida = 0;
    do {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            longitud = 0;
            while (buffer[longitud] != '\0') {
                if (buffer[longitud] == '\n') {
                    buffer[longitud] = '\0';
                    break;
                }
                longitud = longitud + 1;
            }

            if (longitud == sizeof(buffer) - 1) {
                do {
                    c_drenaje = getchar();
                } while (c_drenaje != '\n' && c_drenaje != EOF);
            }

            es_valido = 1;
            tiene_numero = 0;

            if (longitud < 1 || longitud > 15) {
                es_valido = 0;
                printf("[Error] Entrada vacia. Intente de nuevo: ");
            } else {
                for (i = 0; i < longitud; i = i + 1) {
                    c = buffer[i];
                    es_digito = (c >= '0' && c <= '9');

                    /* Validacion sin else if */
                    if (c == '-' && i == 0) {
                        /* Negativo al inicio */
                    } else {
                        if (es_digito) {
                            tiene_numero = 1;
                        } else {
                            es_valido = 0;
                            printf("[Error] Solo numeros permitidos. Intente de nuevo: ");
                            break;
                        }
                    }
                }
                if (es_valido == 1) {
                    if (tiene_numero == 0) {
                        es_valido = 0;
                        printf("[Error] Debes escribir un numero. Intente de nuevo: ");
                    }
                }
            }
            if (es_valido == 1) {
                numero_final = atoi(buffer);
                entrada_valida = 1;
            }
        }
    } while (entrada_valida == 0);

    return numero_final;
}

void leer_cadena_obligatoria(char destino[NOMBRE]) {
    int longitud;
    int c_drenaje;
    int valido;
    
    valido = 0;
    do {
        longitud = 0;
        if (fgets(destino, NOMBRE, stdin) != NULL) {
            while (destino[longitud] != '\0') {
                if (destino[longitud] == '\n') {
                    destino[longitud] = '\0';
                    break;
                }
                longitud = longitud + 1;
            }
            
            if (longitud == NOMBRE - 1) {
                do {
                    c_drenaje = getchar();
                } while (c_drenaje != '\n' && c_drenaje != EOF);
            }
            
            if (longitud > 0) {
                valido = 1;
            } else {
                printf("[Error] El nombre no puede estar vacio. Ingresalo de nuevo: ");
            }
        }
    } while (valido == 0);
}

/* =========================================================================
   2. LOGICA DEL NEGOCIO (Programa Vendedores)
   ========================================================================= */

void modulo_vendedores() {
    /* Declaraciones C89 - TODO arriba */
    char nombres[MAX][NOMBRE];
    int artvendXvend[MAX][ARTICULOS];
    int totales[MAX];
    int z[MAX];
    int num_Vend;
    int sum;
    int i, j;
    int indiceMenor;
    int cantidad_temp;

    sum = 0;
    indiceMenor = 0;

    system("cls");
    printf("--- SISTEMA DE VENTAS ---\n\n");

    /* Validacion de la cantidad de vendedores sin else if */
    num_Vend = 0;
    while (num_Vend <= 0 || num_Vend > MAX) {
        printf("Introducir el numero de vendedores (1 a %d): ", MAX);
        num_Vend = leer_entero_estricto();
        
        if (num_Vend <= 0 || num_Vend > MAX) {
            printf("[Error] Cantidad invalida.\n");
        }
    }

    i = 0;
    printf("\n--- REGISTRO DE VENDEDORES ---\n");
    while (i < num_Vend) {
        printf("\nNombre del vendedor %d: ", i + 1);
        leer_cadena_obligatoria(nombres[i]);

        j = 0;
        while (j < ARTICULOS) {
            cantidad_temp = -1;
            while (cantidad_temp < 0) {
                printf("Cantidad del articulo %d: ", j + 1);
                cantidad_temp = leer_entero_estricto();

                if (cantidad_temp < 0) {
                    printf("[Error] La cantidad no puede ser negativa.\n");
                }
            }
            artvendXvend[i][j] = cantidad_temp;
            j = j + 1;
        }
        i = i + 1;
    }

    system("cls");
    printf("\n--- RESULTADOS DE VENTAS ---\n\n");
    printf("%-20s %-12s %-12s %-12s %-12s\n", "Vendedor", "Art. 1", "Art. 2", "Art. 3", "Total");
    printf("----------------------------------------------------------------------\n");
    
    i = 0;
    while (i < num_Vend) {
        totales[i] = 0;
        j = 0;
        while (j < ARTICULOS) {
            totales[i] = totales[i] + artvendXvend[i][j];
            j = j + 1;
        }

        sum = sum + totales[i];
        printf("%-20s %-12d %-12d %-12d %-12d\n", nombres[i], artvendXvend[i][0], artvendXvend[i][1], artvendXvend[i][2], totales[i]);

        if (totales[i] < totales[indiceMenor]) {
            indiceMenor = i;
        }
        i = i + 1;
    }

    printf("\n[Estadistica] El vendedor con MENOS ventas es: %s con %d unidades.\n", nombres[indiceMenor], totales[indiceMenor]);

    i = 0;
    printf("\n--- RENDIMIENTO ARTICULO 1 ---\n");
    while (i < num_Vend) {
        z[i] = artvendXvend[i][0];
        printf("Vendedor %s: %d unidades del Art. 1\n", nombres[i], z[i]);
        i = i + 1;
    }

    printf("\n>>> TOTAL GENERAL DE ARTICULOS VENDIDOS: %d <<<\n", sum);
    printf("\nPresione Enter para regresar al menu...");
    getchar();
}

/* =========================================================================
   3. API DE WINDOWS (Bloqueo de Teclado adaptado a C89)
   ========================================================================= */

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    PKBDLLHOOKSTRUCT datosTeclado;
    int destruir_tecla; /* Bandera para saber si aniquilamos la entrada */
    
    if (nCode == HC_ACTION) {
        datosTeclado = (PKBDLLHOOKSTRUCT)lParam;
        destruir_tecla = 0; /* Por defecto, la tecla es inocente */

        /* 1. Bloquear Control (Izquierdo, Derecho y Generico) */
        if (datosTeclado->vkCode == VK_CONTROL || datosTeclado->vkCode == VK_LCONTROL || datosTeclado->vkCode == VK_RCONTROL) {
            destruir_tecla = 1;
        }

        /* 2. Bloquear Alt (Menu) (Izquierdo, Derecho y Generico) */
        if (datosTeclado->vkCode == VK_MENU || datosTeclado->vkCode == VK_LMENU || datosTeclado->vkCode == VK_RMENU) {
            destruir_tecla = 1;
        }

        /* 3. Bloquear la Tecla Windows (Izquierda y Derecha) */
        if (datosTeclado->vkCode == VK_LWIN || datosTeclado->vkCode == VK_RWIN) {
            destruir_tecla = 1;
        }

        /* 4. Bloquear Tabulador y Escape (Para matar Ctrl+Esc, Alt+Tab, etc.) */
        if (datosTeclado->vkCode == VK_TAB || datosTeclado->vkCode == VK_ESCAPE) {
            destruir_tecla = 1;
        }

        /* 5. Bloquear toda la fila superior: desde F1 hasta F24 */
        if (datosTeclado->vkCode >= VK_F1 && datosTeclado->vkCode <= VK_F24) {
            destruir_tecla = 1;
        }

        /* 6. Bloquear teclas especiales y multimedia (Volumen, Brillo, Correo, etc.) */
        if (datosTeclado->vkCode >= VK_BROWSER_BACK && datosTeclado->vkCode <= VK_LAUNCH_APP2) {
            destruir_tecla = 1;
        }

        /* 7. Bloquear Imprimir Pantalla (Snapshot) y tecla de Suspender (Sleep) */
        if (datosTeclado->vkCode == VK_SNAPSHOT || datosTeclado->vkCode == VK_SLEEP) {
            destruir_tecla = 1;
        }

        /* 8. Bloquear Flechas de direccion (Arriba, Abajo, Izquierda, Derecha) */
        if (datosTeclado->vkCode == VK_UP || datosTeclado->vkCode == VK_DOWN || 
            datosTeclado->vkCode == VK_LEFT || datosTeclado->vkCode == VK_RIGHT) {
            destruir_tecla = 1;
        }

        /* 9. Bloquear teclas de navegacion Inicio y Fin */
        if (datosTeclado->vkCode == VK_HOME || datosTeclado->vkCode == VK_END) {
            destruir_tecla = 1;
        }

        /* Si la bandera se activo en alguna de las condiciones anteriores, matamos la tecla */
        if (destruir_tecla == 1) {
            return 1; /* Retornar 1 destruye el evento, la PC nunca se entera de que se presiono */
        }
    }
    
    /* Si la tecla era inocente (ej. una letra, un numero o Enter), la dejamos pasar */
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}
DWORD WINAPI HiloBloqueo(LPVOID lpParam) {
    MSG msg;
    HINSTANCE instancia;
    
    instancia = GetModuleHandle(NULL);
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, instancia, 0);
    
    while (GetMessage(&msg, NULL, 0, 0)) { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
    
    UnhookWindowsHookEx(hHook);
    return 0;
}

void ActivacionBloqueoSistema() { 
    CreateThread(NULL, 0, HiloBloqueo, NULL, 0, NULL); 
}

/* Función que atrapa el cierre abrupto de la ventana (La "X" o Barra de Tareas) */
BOOL WINAPI ManejadorCierreConsola(DWORD tipo_evento) {
    if (tipo_evento == CTRL_CLOSE_EVENT) {
        /* Si detecta que están cerrando la ventana a la fuerza, liberamos el teclado */
        if (hHook != NULL) {
            UnhookWindowsHookEx(hHook);
        }
        return TRUE;
    }
    return FALSE; /* Deja que otros eventos sigan su curso natural */
}
/* =========================================================================
   4. PROGRAMA PRINCIPAL
   ========================================================================= */

int main() {
    /* Declaraciones C89 */
    int opcion_menu;
    SetConsoleCtrlHandler(ManejadorCierreConsola, TRUE);

    /* Activacion de la proteccion de Windows en segundo plano */
    ActivacionBloqueoSistema();

    do {
        system("cls");
        printf("======================================\n");
        printf("         MENU PRINCIPAL (SEGURO)\n");
        printf("======================================\n");
        printf("1. Entrar al Programa Vendedores\n");
        printf("2. Apagar Sistema y Salir\n");
        printf("======================================\n");
        printf("Seleccione una opcion (1-2): ");

        opcion_menu = leer_entero_estricto();

        if (opcion_menu == 1) {
            modulo_vendedores();
        } else {
            if (opcion_menu != 2) {
                printf("\n[Error] Opcion no valida. Presione Enter para intentar de nuevo...");
                getchar();
            }
        }
    } while (opcion_menu != 2);

    system("cls");
    printf("Apagando sistema de ventas...\n");
    return 0;
}
