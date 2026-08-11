#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// ELABORADO POR JUAN MEDINA 07/08/2026
/* El programa debe pedir cedula y por quien de los 3 votaste (pedro, maria o juan)
si votaste por alguno de ellos, se debe mostrar un mensaje de confirmación cuando 
presiones 1 o 2 o 3 al respectivo candidato, tambien debemos saber que voto es nulo y
dar un total de votos junto a quien gano las elecciones*/

int main() 
{

    int cedula, voto, total_pedro = 0, total_maria = 0, total_juan = 0, total_nulo = 0, ganador;

    do
        {
            printf("Ingrese su cedula: ");
            scanf("%d", &cedula);
        }
    while (cedula <= 0 || cedula >= 35000000);

    do
        {
            system("cls");
            printf("POR QUE CANDIDATO VOTASTE?\n");
            printf("1. Pedro\n");
            printf("2. Maria\n");
            printf("3. Juan\n");
            printf("0. Nulo\n");
            scanf("%d", &voto);
        }
    while (voto < 0 || voto > 3);

    switch (voto)
        {
            case 1:
                total_pedro++;
                printf("Voto registrado para Pedro.\n");
                break;
            case 2:
                total_maria++;
                printf("Voto registrado para Maria.\n");
                break;
            case 3:
                total_juan++;
                printf("Voto registrado para Juan.\n");
                break;
            case 0:
                total_nulo++;
                printf("Voto nulo registrado.\n");
                break;
        }
        
    return 0;
}