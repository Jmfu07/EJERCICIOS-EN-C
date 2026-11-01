/* EJERCICIO 7: NOTAS POR CURSO Y ASIGNATURA
   Funcionamiento: la matriz [curso][alumno][asignatura] guarda 3 cursos, 5
   alumnos por curso y 2 asignaturas. El menu calcula medias y aprobados.
   Tipos: double para notas; int para indices, cursos y contadores.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    double notas[3][5][2];
    int curso;
    int alumno;
    int asignatura;
    int cursoElegido;
    int asignaturaElegida;
    int opcion;
    int aprobados;
    int suspensos;
    double suma;

    for (curso = 0; curso < 3; curso++)
    {
        for (alumno = 0; alumno < 5; alumno++)
        {
            for (asignatura = 0; asignatura < 2; asignatura++)
            {
                printf("Curso %d, alumno %d, asignatura %d: ",
                       curso + 1, alumno + 1, asignatura + 1);
                scanf("%lf", &notas[curso][alumno][asignatura]);
            }
        }
    }

    do
    {
        printf("\n1. Media de un curso\n");
        printf("2. Aprobados y suspensos\n0. Salir\nOpcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
                printf("Curso (1 a 3): ");
                scanf("%d", &cursoElegido);
                if (cursoElegido < 1 || cursoElegido > 3)
                {
                    printf("Curso invalido.\n");
                    break;
                }
                suma = 0.0;
                for (alumno = 0; alumno < 5; alumno++)
                {
                    for (asignatura = 0; asignatura < 2; asignatura++)
                    {
                        suma = suma + notas[cursoElegido - 1][alumno][asignatura];
                    }
                }
                printf("Media del curso: %.2f\n", suma / 10.0);
                break;

            case 2:
                printf("Curso (1 a 3): ");
                scanf("%d", &cursoElegido);
                printf("Asignatura (1 a 2): ");
                scanf("%d", &asignaturaElegida);
                if (cursoElegido < 1 || cursoElegido > 3 ||
                    asignaturaElegida < 1 || asignaturaElegida > 2)
                {
                    printf("Datos invalidos.\n");
                    break;
                }
                aprobados = 0;
                suspensos = 0;
                for (alumno = 0; alumno < 5; alumno++)
                {
                    if (notas[cursoElegido - 1][alumno][asignaturaElegida - 1] >= 5.0)
                    {
                        aprobados++;
                    }
                    else
                    {
                        suspensos++;
                    }
                }
                printf("Aprobados: %d\nSuspensos: %d\n",
                       aprobados, suspensos);
                break;

            case 0:
                break;

            default:
                printf("Opcion invalida.\n");
        }
    }
    while (opcion != 0);

    system("pause");
    return 0;
}
