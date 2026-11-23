#include <stdio.h> // Librería estándar para entrada/salida (printf)
#include <conio.h> // Librería para manejo de consola (getche)

// Ejemplo 2: Lectura de dirección de memoria y contenido mediante un puntero
main () {
    // Declaración múltiple: dos enteros normales (P y P2) en 50, y un puntero a entero (*PP) sin inicializar
    int P = 50, P2 = 50, *PP;
    
    // Se guarda la dirección de memoria de la variable P dentro del puntero PP
    PP = &P;
    
    // Imprime la dirección de memoria almacenada en PP en formato hexadecimal (%p)
    printf("Dirección de P: %p\n", PP); // ESTABA EN MAYUSCULA EL %P, ESE ERA EL ERROR 
    
    // Desreferencia el puntero (*PP) para acceder y mostrar el valor guardado en esa dirección (50)
    printf("Valor de P: %d", *PP);
    
    // Pausa la ejecución esperando una pulsación de tecla antes de cerrar la consola
    getche();
}
