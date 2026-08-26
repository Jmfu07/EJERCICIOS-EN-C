#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/* 
Los empleados de una fabrica trabajan en dos turnos 

las horas diurnas se pagan a 1000bs
las horas nocturnas se pagan a 1600bs
los domingos las diurnas tienen un incremento de 400bs y 
las nocturnas uno de 600

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Devuelve 'D' para Domingo u 'O' para Otro día
char validarDia() 
{
    char opc;
    do 
    {
        system("cls");
        printf("¿El dia trabajado es Domingo? (S/N): ");
        if (scanf(" %c", &opc) != 1) 
        {
            limpiarBuffer();
            printf("Error: Ingrese 'S' (Si) o 'N' (No).\n");
            sleep(3);
            continue;
        }
        limpiarBuffer();
        opc = toupper(opc);

        if (opc == 'S') 
        {
            return 'D';
        } 
        else if (opc == 'N') 
        {
            return 'O';
        } 
        else 
        {
            printf("Error: Opción inválida. Ingrese 'S' o 'N'.\n");
            sleep(3);
        }
    } while (1);
}


/*
===============================================================================
EXPLICACIÓN DE LOS PARÁMETROS EN pedirHoras()
===============================================================================

Firma de la función:
int pedirHoras(const char *mensaje, int maxHoras);

Llamada desde el main():
hd = pedirHoras("Ingrese la cantidad de horas diurnas trabajadas: ", maxHD);

1. const char *mensaje:
   Recibe la cadena de texto "Ingrese la cantidad de horas diurnas trabajadas: "
   y la imprime en pantalla con printf("%s", mensaje).

2. int maxHoras:
   Recibe el valor de la variable 'maxHD' (o 'maxHN'), es decir, el límite
   de horas permitido según el día (12 para Domingo, u 8/7 de Lunes a Sábado).
   
   - Se utiliza en la validación: (horas > maxHoras)
   - Se muestra dinámicamente en el error: 
     printf("Error: Ingrese un numero entero entre 0 y %d.\n", maxHoras);
===============================================================================
*/

// Recibe el mensaje y el limite maximo de horas permitidas
int pedirHoras(const char *mensaje, int maxHoras) 
{
    int horas;
    do 
    {
        system("cls");
        printf("%s", mensaje);
        if (scanf("%d", &horas) != 1 || horas < 0 || horas > maxHoras) 
        {
            printf("Error: Ingrese un numero entero entre 0 y %d.\n", maxHoras);
            limpiarBuffer();
            sleep(3);
        } 
        else 
        {
            limpiarBuffer();
            return horas;
        }
    } while (1);
}


int main() 
{
    char dia;
    int hd, hn;
    int tarifaDiurna = 1000;
    int tarifaNocturna = 1600;
    int totalPagar;
    int maxHD, maxHN;

    // 1. Obtener tipo de día y horas trabajadas
    dia = validarDia();

    // Establecer limites segun el dia
    if (dia == 'D') 
    {
        maxHD = 12; // Maximo 12 horas el domingo
        maxHN = 12;
    } 
    else 
    {
        maxHD = 72;  // Jornada diurna legal (Lunes a Sabado)
        maxHN = 72;  // Jornada nocturna legal (Lunes a Sabado)
    }

    // Pedir horas pasando el limite correspondiente
    hd = pedirHoras("Ingrese la cantidad de horas diurnas trabajadas: ", maxHD);
    hn = pedirHoras("Ingrese la cantidad de horas nocturnas trabajadas: ", maxHN);

    // 2. Aplicar incrementos si es domingo
    if (dia == 'D') 
    {
        tarifaDiurna += 400;   // 1400 Bs
        tarifaNocturna += 600;  // 2200 Bs
    }

    // 3. Cálculo del sueldo
    totalPagar = (hd * tarifaDiurna) + (hn * tarifaNocturna);

    // 4. Mostrar resultados
    system("cls");
    printf("===========================================\n");
    printf("           RESUMEN DE PAGO                 \n");
    printf("===========================================\n");
    printf(" Dia trabajado:           %s\n", (dia == 'D') ? "Domingo" : "Lunes a Sabado");
    printf(" Horas Diurnas (%d Bs/h):  %d hrs -> %d Bs\n", tarifaDiurna, hd, hd * tarifaDiurna);
    printf(" Horas Nocturnas (%d Bs/h): %d hrs -> %d Bs\n", tarifaNocturna, hn, hn * tarifaNocturna);
    printf("-------------------------------------------\n");
    printf(" TOTAL A PAGAR:           %d Bs\n", totalPagar);
    printf("===========================================\n");

    sleep(15);
    return 0;  

    /*
    ===============================================================================
    EXPLICACIÓN DE LAS LÍNEAS DE SALIDA DE DATOS
    ===============================================================================

    LÍNEA 1:
    printf(" Dia trabajado:           %s\n", (dia == 'D') ? "Domingo" : "Lunes a Sabado");

    - %s: Especificador de formato para imprimir una cadena de texto (string).
    - (dia == 'D') ? "Domingo" : "Lunes a Sabado": Operador ternario (if/else en una línea). 
    Si 'dia' equivale a 'D', retorna "Domingo"; de lo contrario, "Lunes a Sabado".


    LÍNEA 2:
    printf(" Horas Diurnas (%d Bs/h):  %d hrs -> %d Bs\n", tarifaDiurna, hd, hd * tarifaDiurna);

    - Tres %d: Imprimen números enteros según su posición de variable:
    1. Primer %d  -> Muestra el valor de 'tarifaDiurna' (1000 o 1400 Bs).
    2. Segundo %d -> Muestra la cantidad de horas ingresadas en 'hd'.
    3. Tercer %d  -> Realiza e imprime la multiplicación 'hd * tarifaDiurna'.


    LÍNEA 3:
    printf(" Horas Nocturnas (%d Bs/h): %d hrs -> %d Bs\n", tarifaNocturna, hn, hn * tarifaNocturna);

    - Misma estructura aplicada al turno nocturno:
    1. Primer %d  -> Muestra el valor de 'tarifaNocturna' (1600 o 2200 Bs).
    2. Segundo %d -> Muestra la cantidad de horas ingresadas en 'hn'.
    3. Tercer %d  -> Realiza e imprime la multiplicación 'hn * tarifaNocturna'.
    
    ===============================================================================
    */


}

