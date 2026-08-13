#include <stdio.h>
#include <stdlib.h>
// ELABORADO POR JUAN MEDINA 07/08/2026
/* El programa debe pedir cedula y por quien de los 3 votaste (pedro, maria o juan)
si votaste por alguno de ellos, se debe mostrar un mensaje de confirmación cuando 
presiones 1 o 2 o 3 al respectivo candidato, tambien debemos saber que voto es nulo y
dar un total de votos junto a quien gano las elecciones*/

void menu() 
{
    printf("--------------------------------------------------------\n");
    printf("Bienvenido al sistema de votación.\n");
    printf("--------------------------------------------------------\n");
    printf("Por favor, siga las instrucciones para registrar su voto.\n");
    printf("--------------------------------------------------------\n");
}

void menuvoto() 
{
    printf("--------------------------------------------------------\n");
    printf("POR QUE CANDIDATO VOTASTE?\n");
    printf("--------------------------------------------------------\n");
    printf("1. Pedro\n");
    printf("2. Maria\n");
    printf("3. Juan\n");
    printf("--------------------------------------------------------\n");
    printf("0. Nulo\n");
    printf("--------------------------------------------------------\n");
    
}

void mostrarResultados(int total_pedro, int total_maria, int total_juan, int total_nulo) 
{
    printf("--------------------------------------------------------\n");
    printf("            Resultados de las elecciones:               \n");
    printf("--------------------------------------------------------\n");
    printf("- Total de votos para Pedro: %d\n", total_pedro);
    printf("- Total de votos para Maria: %d\n", total_maria);
    printf("- Total de votos para Juan: %d\n", total_juan);
    printf("- Total de votos nulos: %d\n", total_nulo);
    printf("--------------------------------------------------------\n");
    
    if (total_pedro > total_maria && total_pedro > total_juan) 
    {
        printf("- El ganador es Pedro.\n");
        printf("--------------------------------------------------------\n");
    
    } 
    else if (total_maria > total_pedro && total_maria > total_juan) 
    {
        printf("- El ganador es Maria.\n");
        printf("--------------------------------------------------------\n");
    
    } 
    else if (total_juan > total_pedro && total_juan > total_maria) 
    {
        printf("- El ganador es Juan.\n");
        printf("--------------------------------------------------------\n");
    } 
    else 
    {
        printf("- Hubo un empate entre los candidatos.\n");
        printf("--------------------------------------------------------\n");
    }
}


// Función para vaciar letras del búfer
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int cedularepetida(int cedula, int cedulas[], int num_votantes) 
{
    for (int i = 0; i < num_votantes; i++) 
    {
        if (cedulas[i] == cedula) 
        {
            return 1; // Cedula repetida
        }
    }
    return 0; // Cedula no repetida
}

int validarCedula(int cedula) 
{
    return (cedula > 0 && cedula < 35000000);
}

int validarVoto(int voto) 
{
    return (voto >= 0 && voto <= 3);
}


int main() 
{
    int cedula, voto, total_pedro = 0, total_maria = 0, total_juan = 0, total_nulo = 0, fin;
    int cedulas[100]; // Array para almacenar las cedulas de los votantes
    int num_votantes = 0; // Contador de votantes

    menu();
    do 
    {
        do  
        {
            printf("Ingrese su cedula: ");
    
            // Si scanf devuelve 0, el usuario escribió una letra
            if (scanf("%d", &cedula) != 1) 
            {
                printf("Error: Solo se permiten numeros.\n");
                limpiarBuffer(); // Limpia la letra atrapada para evitar el bucle infinito
                cedula = -1;     // Asignamos un valor inválido para forzar a repetirse
            } 
            else  
            {
                limpiarBuffer(); // Limpia el 'Enter' sobrante
            }

            } while (!validarCedula(cedula));

            // Verificación de cédula repetida
            if (cedularepetida(cedula, cedulas, num_votantes)) {
            printf("--------------------------------------------------------\n");
            printf("La cedula ya ha sido registrada. No puede votar nuevamente.\n");
            printf("--------------------------------------------------------\n");
            continue;
        }

        cedulas[num_votantes] = cedula;
        num_votantes++;

        // 2. Lectura de Voto
        do 
        {
            system("cls");
            menuvoto();
    
            if (scanf("%d", &voto) != 1) {
            printf("Error: Debe ingresar un numero.\n");
            limpiarBuffer();
            voto = -1; // Asignamos valor fuera de rango (0-3) para forzar la repetición
            system("pause"); // Pausa para leer el error antes del system("cls")
            } 
            else 
            {
                limpiarBuffer();
            }
        } while (!validarVoto(voto));

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
        
        do 
        {
            system("cls");
            printf("Desea continuar votando? (1 para si, 0 para no): ");
    
            // Si scanf devuelve 0, significa que ingresaron una letra
            if (scanf("%d", &fin) != 1) 
            {
                printf("\nError: Debe ingresar un numero (1 o 0).\n");
                limpiarBuffer(); // Destruye la letra para evitar el bucle infinito
                fin = -1;        // Forzamos un valor inválido para repetirlo
                system("pause"); // Pausa para leer el error antes del system("cls")
            } 
            else 
            {
                limpiarBuffer(); // Limpiamos el 'Enter' sobrante
            if (fin < 0 || fin > 1) 
            {
                printf("\nOpción fuera de rango (solo 1 o 0).\n");
                system("pause");
            }
            }
        } while (fin < 0 || fin > 1);
        
        if (fin == 1) 
        {
            system("cls");
            menu();
        }

        if (fin == 0) 
        {
            system("cls");
            mostrarResultados(total_pedro, total_maria, total_juan, total_nulo);
            system("pause");
            fin = 2; // Salir del bucle principal
        }

    }
    while (fin == 0 || fin == 1);
    
}