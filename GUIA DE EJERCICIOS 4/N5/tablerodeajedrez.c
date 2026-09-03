#define __USE_MINGW_ANSI_STDIO 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Un programa que pinte un tablero de ajedrez, los peones con la letra P, las torres con T,
los caballos con C, los alfiles con A, el rey con R y la reina con M.

*/

int main()
{
    
    const char inicio[8][9] = 
    {
        "tcamract",  
        "pppppppp",  
        "........",  
        "........",
        "........",
        "........",
        "PPPPPPPP",  
        "TCAMRACT"   
    };
    // ---------------------------------------------------------------------------
    // ¿Por qué [8][9] y no [8][8]?
    // - Cada fila del tablero se representa como una cadena de caracteres.
    // - En C, las cadenas terminan con un carácter especial llamado '\0' (null terminator),
    //   que indica el final de la cadena.
    // - Ejemplo: "pppppppp" ocupa 9 posiciones en memoria:
    //   'p' 'p' 'p' 'p' 'p' 'p' 'p' 'p' '\0'
    // - Por eso, aunque el tablero es de 8 columnas, necesitamos reservar 9 espacios
    //   para cada fila: 8 piezas + 1 terminador.
    // - Si usáramos [8][8], no habría espacio para el '\0' y aparecerían
    //   comportamientos inesperados en la lógica del juego o en la visualizacion del menu

    // Recorrer filas (i) y columnas (j)
    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            printf("%c ", inicio[i][j]); // Añade un espacio para mejor visualización
        }
        printf("\n"); // Salto de línea al terminar cada fila
    }

    sleep (20);
}