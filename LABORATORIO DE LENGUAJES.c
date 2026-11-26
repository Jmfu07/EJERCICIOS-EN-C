#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
	
/*
	ELABORADO POR JUAN MEDINA V-31708027 TERMINAR A LAS 9:30

	INGRESAR UNA OPCION NUMERICA PARA REALIZAR LAS SIGUIENTES ACTIVIDADES EN LENGUAJE C
	1- INGRESE POR TECLADO UN VALOR NUMERICO DE A Y CALCULE EL VALOR DE K CUANDO K ES = A LA RAIZ CUADRADA DE A
	2- INGRESE POR TECLAOD 3 VALORES A, B, C, INTERCAMBIE EL VALOR DE B EN A, DE C EN B, Y A EN C
	3- INGRESE POR TECLADO NOMBRE, EDAD, SEXO Y PROMEDIO DE UN ALUMNO.IMPRIMA EL NOMBRE Y EL SEXO SI EL ALUMNO ESTA APROBADO, EN CASO CONTRARIO IMPRIMA UN MENSAJE DE REPROBADO
	4- VALIDAR TODAS LAS OPCIONES E INCLUSO EN CASO INCORRECTO
	5- IMPRIMIR LOS RESULTADOS DE CADA ACTIVIDAD

*/
	
int main () 
{
	int op, edad;
	double a,b,c,k, prom;
	double aux;
	char nombre[30], sexo;
	int esval, i;
			
	
	// MENU
	do 
	{
		printf ("\n----- MENU -----\n");
		printf ("1- CALCULO DE RAIZ CUADRADA\n");
		printf ("2- INTERCAMBIO DE A, B Y C\n");
		printf ("3- APROBADOS Y REPROBADOS\n");
		printf ("4- SALIR\n");
		printf ("----- Seleccione una opcion: ");
		
		// Validación de letras: scanf devuelve 0 si no lee un número entero
        while (scanf("%d", &op) != 1) 
		{
            printf("\n[ERROR] No se permiten letras. Intente de nuevo.\n\n");
            while (getchar() != '\n'); // Limpia el búfer de entrada
            printf("----- Seleccione una opcion: ");
        }
        
        if (op < 1 || op > 4) 
		{
            printf("\n[ERROR] Opcion invalida (Debe ser del 1 al 4).\n\n");
            op = 0;
			while (getchar() != '\n'); 
        }
		
		if (op == 1) 
		{
			// 1- INGRESE POR TECLADO UN VALOR NUMERICO DE A Y CALCULE EL VALOR DE K CUANDO K ES = A LA RAIZ CUADRADA DE A
			
			printf("INGRESE EL NUMERO PARA CALCULAR LA RAIZ CUADRADA: \n");
	
			do 
			{
				// Guardamos si scanf pudo leer un número (1) o no (0)
				while (scanf("%lf", &a) !=1)
				{
					printf("\n[ERROR] No se permiten letras. Intente de nuevo.\n\n");
	            	while (getchar() != '\n'); 
				}
				if (a<0 || a > 1000000) printf ("\n[ERROR NO SE PERMITEN NUMEROS NEGATIVOS]\n");				
			} while (a < 0 || a > 1000000);
			
					
			k = sqrt(a);
			system("cls");
			printf ("\nLa raiz de %.2f es %.2f\n", a, k);
		}
	
	
		if (op == 2) {
			// 2- INGRESE POR TECLAOD 3 VALORES A, B, C, INTERCAMBIE EL VALOR DE B EN A, DE C EN B, Y A EN C
			
			printf("INGRESE UN VALOR PARA A: \n");
			while (scanf("%lf", &a) !=1)
			{
				printf("\n[ERROR] No se permiten letras. Intente de nuevo.\n\n");
	           	while (getchar() != '\n'); 
			}
			
			printf("INGRESE UN VALOR PARA B: \n");
			while (scanf("%lf", &b) !=1)
			{
				printf("\n[ERROR] No se permiten letras. Intente de nuevo.\n\n");
	           	while (getchar() != '\n'); 
			}
			
			printf("INGRESE UN VALOR PARA C: \n");
			while (scanf("%lf", &c) !=1)
			{
				printf("\n[ERROR] No se permiten letras. Intente de nuevo.\n\n");
	           	while (getchar() != '\n'); 
			}
			
			system("cls");
			printf ("A VALIA EN EL INICIO %.2f\n", a);
			printf ("B VALIA EN EL INICIO %.2f\n", b);
			printf ("C VALIA EN EL INICIO %.2f\n", c);
 
    		aux = a; // Guardamos el valor original de A en la variable auxiliar"
    		a = b;   // El valor de B pasa a A
		    b = c;   // El valor de C pasa a B
		    c = aux; // El valor original de A (guardado en aux) pasa a C
			
			printf ("A AHORA VALE = %.2f\n", a);
			printf ("B AHORA VALE = %.2f\n", b);
			printf ("C AHORA VALE = %.2f\n", c);
		}
	
		if (op == 3) 
		{
			// 3- INGRESE POR TECLADO NOMBRE, EDAD, SEXO Y PROMEDIO DE UN ALUMNO.IMPRIMA EL NOMBRE Y EL SEXO SI EL ALUMNO ESTA APROBADO, EN CASO CONTRARIO IMPRIMA UN MENSAJE DE REPROBADO
		    do 
			{
		        printf("INGRESE EL NOMBRE DEL ALUMNO: ");
		        scanf("%s", nombre);
		
		        esval = 1; // Asumimos que está bien
		        i = 0;         // Empezamos a revisar desde la primera letra
		
		        // Este bucle revisa la palabra de principio a fin
		        while (nombre[i] != '\0') {
		            // Si encuentra CUALQUIER cosa que no sea letra, se vuelve inválido
		            if (!isalpha(nombre[i])) {
		                esval = 0;
		            }
		            i = i + 1;
		        }
		
		        // Si falló la validación, avisamos al usuario y limpiamos teclado
		        if (esval == 0) {
		            printf("[ERROR] El nombre solo debe contener letras.\n\n");
		            while (getchar() != '\n'); 
		        }		
		    } while (esval == 0); // Se repite si es inválido
			printf("Nombre guardado exitosamente: %s\n", nombre);
			
		    
	
			printf("INGRESE LA EDAD DEL ALUMNO: \n");
			while (scanf("%d", &edad) !=1 || edad < 1 || edad > 18)
			{
				printf("\n[ERROR] No se permiten letras y numeros negativos, solo edades de 1 y 18. Intente de nuevo.\n\n");
	           	while (getchar() != '\n'); 
			}
			
			printf("INGRESE EL SEXO DEL ALUMNO:\n");
			printf("- MASCULINO = M\n");
			printf("- FEMENINO = F\n");    
			while (getchar() != '\n');
			do {
			    printf("Respuesta: ");
			    
			    // 1. Leemos el primer carácter y lo pasamos a minúscula de una vez
			    sexo = tolower(getchar()); 
			
			    // 2. Limpiamos el búfer por si el usuario escribió de más o presionó Enter
			    if (sexo != '\n' && sexo != EOF) {
			        while (getchar() != '\n'); 
			    }
			
			    // 3. Validamos que sea exactamente 'm' o 'f'
			    if (sexo != 'm' && sexo != 'f') {
			        printf("[ERROR] OPCION INVALIDA. Solo debes introducir M o F.\n\n");
			    }
			} while (sexo != 'm' && sexo != 'f'); // Se repite hasta que sea válido
			printf("Sexo guardado exitosamente: %c\n", sexo);
			
				
			printf("INGRESE EL PROMEDIO DEL ALUMNO: \n");
			while (scanf("%lf", &prom) != 1 || prom < 0 || prom > 20) 
			{
    			printf("\n[ERROR] Opcion invalida. Ingrese una nota entre 0 y 20 (sin letras): \n\n");
    			while (getchar() != '\n'); // Limpia el búfer del teclado para evitar bucle infinito
			}
			
			system("cls");
			printf("\n================ RESULTADO ================\n");
			if (prom >= 9,5) 
			{
			    printf("ALUMNO APROBADO\n");
			    printf("Nombre: %s\n", nombre);
			    
			    // Mostramos el sexo de forma más elegante aprovechando que ya está validado
			    if (sexo == 'm') {
			        printf("Sexo: Masculino\n");
			    } else {
			        printf("Sexo: Femenino\n");
			    }
			} else {
			    printf("EL ALUMNO ESTA REPROBADO (Promedio insuficiente: %.2f)\n", prom);
			}
			printf("===========================================\n");
			}
	} while (op != 4);			

}
	
