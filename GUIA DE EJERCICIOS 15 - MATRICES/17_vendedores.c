/* EJERCICIO 17: VENDEDORES
   Funcionamiento: una matriz 18x10 guarda los ingresos de cada vendedor por
   producto. El menu permite registrar ingresos y consultar totales.
   Tipos: double para ingresos; int para vendedores, productos e indices.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    double ingresos[18][10] = {0};
    double totalVendedor;
    double totalEmpresa;
    double monto;
    int vendedor;
    int producto;
    int opcion = -1;
    int ch;

    do
    {
        printf("\n--- MENU DE VENDEDORES ---\n");
        printf("1. Registrar ingreso\n");
        printf("2. Total de un vendedor\n");
        printf("3. Total de la empresa\n");
        printf("0. Salir\nOpcion: ");
        if (scanf("%d", &opcion) != 1)
        {
            opcion = -1;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
        }

        switch (opcion)
        {
            case 1:
                printf("Vendedor (1 a 18): ");
                if (scanf("%d", &vendedor) != 1) { vendedor = 0; while ((ch = getchar()) != '\n' && ch != EOF) {} }
                printf("Producto (1 a 10): ");
                if (scanf("%d", &producto) != 1) { producto = 0; while ((ch = getchar()) != '\n' && ch != EOF) {} }
                if (vendedor < 1 || vendedor > 18 ||
                    producto < 1 || producto > 10)
                {
                    printf("Datos invalidos.\n");
                }
                else
                {
                    printf("Ingreso (>= 0): ");
                    if (scanf("%lf", &monto) != 1 || monto < 0.0)
                    {
                        while ((ch = getchar()) != '\n' && ch != EOF) {}
                        printf("Monto invalido.\n");
                    }
                    else
                    {
                        ingresos[vendedor - 1][producto - 1] += monto;
                        printf("Ingreso acumulado correctamente.\n");
                    }
                }
                break;

            case 2:
                printf("Vendedor (1 a 18): ");
                scanf("%d", &vendedor);
                if (vendedor < 1 || vendedor > 18)
                {
                    printf("Vendedor invalido.\n");
                }
                else
                {
                    totalVendedor = 0.0;
                    for (producto = 0; producto < 10; producto++)
                    {
                        totalVendedor = totalVendedor +
                                        ingresos[vendedor - 1][producto];
                    }
                    printf("Total del vendedor: %.2f\n", totalVendedor);
                }
                break;

            case 3:
                totalEmpresa = 0.0;
                for (vendedor = 0; vendedor < 18; vendedor++)
                {
                    for (producto = 0; producto < 10; producto++)
                    {
                        totalEmpresa = totalEmpresa + ingresos[vendedor][producto];
                    }
                }
                printf("Total de la empresa: %.2f\n", totalEmpresa);
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
