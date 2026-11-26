#include <stdio.h>
#include <conio.h>
#include <ctype.h>

/*  

ELABORADO 8/10/2026 -- JUAN MEDINA CI:31708027

En una universidad estan interesados en que elabore un programa en lenguaje C para controlar el proceso de 
suscripcion. Para ello le suministra los siguientes datos de cada alumno:

- Numero de expediente
- Nombre
- Edad
- Sexo
- Unidades de credito inscritas
- Valor de la unidad de credito
- Facultad a estudiar
	- A = Administracion
	- I = Ingenieria 
	- E = Educacion

Se pide:
- Calcular el monto a pagar de inscripcion por cada alumno e imprimirlo con su nombre
 RECUERDA: CADA ALUMNO DE ADMINISTRACION Y EDUCACION PAGA LO MISMO EN UNIDADES DE CREDITO, PERO LOS
 ESTUDIANTES DE INGENIERIA PAGAN 50BS POR UNIDAD DE CREDITO POR DERECHO AL LABORATORIO
 
 - Porcentaje de alumnos inscritos por facultad 
 
 - Promedio de edad de los alumnos inscritos en administracion (utiliza el ciclo while con respuesta
 al operador)

*/


main ()
{
	
	int nexp=0, edad=0, uci=0, alum=1;
	double vuc=0, ins, por, promed, a, i, e;
	char nombre[100], sexo, op, tr;
	
	for (alum=1; alum <= alum; alum++)
	{
		do 
		{	
			system("cls");
			printf("\nIngrese el numero de expendiente del alumno (de 1 a 1000): \n");
			do
			{
				scanf("%d", &nexp);
				if (nexp <= 0 || nexp < 1 || nexp >1001) printf ("\n [ERROR] NUMERO DE EXPEDIENTE INVALIDO (de 1 a 1000)\n");
				else  printf ("\nNUMERO DE EXPEDIENTE VALIDO\n");
			} while (nexp <= 0 || nexp < 1 || nexp >1001);
			
			system("cls");
			printf("\nIngrese el nombre del alumno: ");
			scanf("%s", nombre);
			
			system("cls");
			printf("\nIngrese la edad del alumno: \n");
			do
			{
				scanf("%d%c", &edad, &tr);
				if (edad <= 0 || edad <16 || edad >45) printf ("\nEDAD INVALIDA (de 16 a 45)\n");
				else  printf ("\nEDAD VALIDA\n\n");
			} while (edad <= 0 || edad<16 || edad>45);
			
			system("cls");
			printf("INGRESE EL SEXO DEL ALUMNO:\n");
			printf("- MASCULINO = M\n");
			printf("- FEMENINO = F\n");    
			do 
			{
				scanf ("%c", &sexo);
				sexo = tolower(getchar()); 
				if (sexo != 'm' && sexo != 'f') printf("\n[ERROR] OPCION INVALIDA. Solo debes introducir M o F.\n");
				else printf("\nSexo guardado exitosamente: %c\n", sexo); 
			} while (sexo != 'm' && sexo != 'f'); 
			
			system("cls");
			printf("INGRESE LA CANTIDAD DE CREDITOS QUE INSCRIBIO:\n");
			do
			{
				// no puedo inscribir menos de 5 creditos ni mas de 20
				scanf ("%d", &uci);
				if (uci < 5 || uci > 20) printf ("\n[ERROR] NO PUEDES TENER MAS DE 20 CREDITOS INSCRITOS\n");
				else printf("\nDato guardado exitosamente\n");
			} while (uci < 5 || uci > 20);
			
			system("cls");
			printf("INGRESE EL VALOR DE LA UNIDAD DE CREDITO:\n");
			do
			{
				// no puedo inscribir menos de 5 creditos ni mas de 20
				scanf ("%lf", &vuc);
				if (vuc <=0 || vuc > 100) printf ("\n[ERROR] NO PUEDES TENER MAS DE 20 CREDITOS INSCRITOS\n");
				else printf("\nDato guardado exitosamente\n");
			} while (vuc <= 0 || vuc > 100);
			
			system("cls");
			printf("INGRESE LA FACULTAD DONDE ESTUDIA:\n");
			printf("- ADMINISTRACION = A\n");
			printf("- INGENIERIA = I\n");
			printf("- EDUCACION = E\n");    
			do 
			{
				scanf ("%s", &op);
				printf("Respuesta: ");
			    op = tolower(getchar()); 
				if (op != 'a' && op != 'i' && op !='e') printf("\n[ERROR] OPCION INVALIDA.\n");
				else printf("\nFacultad guardada exitosamente\n");
				if (op=='a' || op == 'e')
				{
					ins=uci*vuc;
					printf ("\nEL ALUMNO %s PAGARA %.2lf\n", nombre, ins);
					if (op=='a') a++;
					else e++;
				}
				if (op=='i')
				{
					ins=(uci*vuc)+50;
					printf ("\nEL ALUMNO %s PAGARA %.2lf\n", nombre, ins);
					i++;
				} 
			} while (op != 'a' && op != 'i' && op !='e'); 
			
			
			promed += (edad/alum);
			printf("DESEA SEGUIR CALCULANDO?");
			scanf ("%s", &op);
			op = tolower(getchar()); 
			if (op != 'y' && op != 'n') printf("\n[ERROR] OPCION INVALIDA. Solo debes introducir Y o N.\n");
			else printf("\n Opcion guardada exitosamente"); 
			if (op == 'y') alum += alum;
			else{
				system ("cls");
				printf ("\nEl promedio de edad total es de %.2f\n", promed);
				double prom;
				a =  (a/alum)*100;
				e =  (e/alum)*100;
				i =  (i/alum)*100;
				printf ("\nEL promedio de alumnos inscritos por facultad es de: \n");
				printf ("- ADMINISTRACION %.2lf\n%", a);
				printf ("- INGENIERIA %.2lf\n%", i);
				printf ("- EDUCACION %.2lf\n%", e);
				return 0;
			} 
			
		} while (op != 'y' && op != 'n'); 
		
	}
	
	
}
