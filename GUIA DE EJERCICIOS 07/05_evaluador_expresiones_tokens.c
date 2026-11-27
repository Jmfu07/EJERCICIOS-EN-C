/*
 * ============================================================================
 * EJERCICIO 5: Sistema de Evaluacion de Expresiones con Punteros y Matrices
 * ============================================================================
 * Algoritmo: Lee una expresion matematica como cadena (soporta +, -, *, /, ^ y
 *            parentesis). Tokeniza la expresion en una matriz dinamica de N x 3
 *            donde cada fila representa un token:
 *              Columna 0: Tipo (0 = Numero, 1 = Operador)
 *              Columna 1: Valor numerico o codigo ASCII del operador
 *              Columna 2: Nivel de prioridad efectiva (considerando parentesis)
 *            Una funcion recursiva busca el operador con mayor prioridad en la
 *            matriz de tokens, evalua los operandos adyacentes usando punteros,
 *            reduce la matriz de tokens mostrando cada paso intermedio y
 *            retorna el resultado final.
 * Funciones:
 *   - tokenizar_expresion(): Convierte la cadena en matriz de tokens.
 *   - evaluar_tokens_rec(): Reduce recursivamente la matriz por prioridad.
 * Punteros y Recursividad:
 *   - Matriz de tokens double (*)[3] manipulada con aritmetica de punteros.
 * Tipos de datos: double, int, char *.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void imprimir_matriz_tokens(double (*tokens)[3], int n, int paso) {
    if (paso == 0) {
        printf("\n--- MATRIZ INICIAL DE TOKENS (Tipo | Valor/Op | Prioridad) ---\n");
    } else {
        printf("\n  [Paso %d] Expresion reducida: ", paso);
        for (int i = 0; i < n; i++) {
            int tipo = (int)(*(*(tokens + i) + 0));
            double val = *(*(tokens + i) + 1);
            if (tipo == 0) {
                printf("%.2f ", val);
            } else {
                printf("%c ", (char)((int)val));
            }
        }
        printf("\n");
    }

    for (int i = 0; i < n; i++) {
        int tipo = (int)(*(*(tokens + i) + 0));
        double val = *(*(tokens + i) + 1);
        int prio = (int)(*(*(tokens + i) + 2));
        if (tipo == 0) {
            printf("    Fila %2d: [ NUMERO   | %8.2f | Prioridad: %2d ]\n", i, val, prio);
        } else {
            printf("    Fila %2d: [ OPERADOR |    '%c'   | Prioridad: %2d ]\n",
                   i, (char)((int)val), prio);
        }
    }
}

int prioridad_base(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

int tokenizar_expresion(const char *expr, double (*tokens)[3], int max_tokens) {
    int n = 0;
    int bono_parentesis = 0;
    int i = 0;
    int esperando_numero = 1;

    while (*(expr + i) != '\0') {
        if (isspace((unsigned char)*(expr + i))) {
            i++;
            continue;
        }
        if (*(expr + i) == '(') {
            if (!esperando_numero) return -1;
            bono_parentesis += 10;
            i++;
            continue;
        }
        if (*(expr + i) == ')') {
            if (esperando_numero) return -1;
            bono_parentesis -= 10;
            if (bono_parentesis < 0) return -1;
            i++;
            continue;
        }

        if (esperando_numero &&
            (isdigit((unsigned char)*(expr + i)) ||
             ((*(expr + i) == '+' || *(expr + i) == '-') &&
              isdigit((unsigned char)*(expr + i + 1))))) {
            char *fin_num = NULL;
            double num = strtod(expr + i, &fin_num);
            if (n >= max_tokens) return -1;
            *(*(tokens + n) + 0) = 0.0;
            *(*(tokens + n) + 1) = num;
            *(*(tokens + n) + 2) = 0.0;
            n++;
            i = (int)(fin_num - expr);
            esperando_numero = 0;
        } else if (!esperando_numero && prioridad_base(*(expr + i)) > 0) {
            char op = *(expr + i);
            if (n >= max_tokens) return -1;
            *(*(tokens + n) + 0) = 1.0;
            *(*(tokens + n) + 1) = (double)op;
            *(*(tokens + n) + 2) = (double)(prioridad_base(op) + bono_parentesis);
            n++;
            i++;
            esperando_numero = 1;
        } else {
            return -1;
        }
    }

    if (bono_parentesis != 0 || esperando_numero || n == 0) {
        return -1;
    }
    return n;
}

double evaluar_tokens_rec(double (*tokens)[3], int n, int paso, int *error) {
    if (n == 1) {
        return *(*(tokens + 0) + 1);
    }

    int idx_max = -1;
    int max_prio = -1;
    for (int i = 0; i < n; i++) {
        int tipo = (int)(*(*(tokens + i) + 0));
        int prio = (int)(*(*(tokens + i) + 2));
        char op_i = (char)((int)(*(*(tokens + i) + 1)));
        if (tipo == 1 && (prio > max_prio || (prio == max_prio && op_i == '^'))) {
            max_prio = prio;
            idx_max = i;
        }
    }

    if (idx_max <= 0 || idx_max >= n - 1) {
        *error = 1;
        return 0.0;
    }

    double izq = *(*(tokens + idx_max - 1) + 1);
    double der = *(*(tokens + idx_max + 1) + 1);
    char op = (char)((int)(*(*(tokens + idx_max) + 1)));
    double res = 0.0;

    if (op == '+') res = izq + der;
    else if (op == '-') res = izq - der;
    else if (op == '*') res = izq * der;
    else if (op == '/') {
        if (fabs(der) < 1e-12) {
            printf("  [Error] Division por cero detectada.\n");
            *error = 1;
            return 0.0;
        }
        res = izq / der;
    } else if (op == '^') {
        res = pow(izq, der);
    }

    printf("\n  -> Evaluando: %.2f %c %.2f = %.2f (Prioridad %d)",
           izq, op, der, res, max_prio);

    *(*(tokens + idx_max - 1) + 0) = 0.0;
    *(*(tokens + idx_max - 1) + 1) = res;
    *(*(tokens + idx_max - 1) + 2) = 0.0;

    for (int i = idx_max; i < n - 2; i++) {
        *(*(tokens + i) + 0) = *(*(tokens + i + 2) + 0);
        *(*(tokens + i) + 1) = *(*(tokens + i + 2) + 1);
        *(*(tokens + i) + 2) = *(*(tokens + i + 2) + 2);
    }

    imprimir_matriz_tokens(tokens, n - 2, paso);
    return evaluar_tokens_rec(tokens, n - 2, paso + 1, error);
}

int main(void) {
    printf("=================================================================\n");
    printf("   EVALUADOR RECURSIVO DE EXPRESIONES CON MATRIZ DE TOKENS\n");
    printf("=================================================================\n");
    printf("Operadores soportados: +, -, *, /, ^ y parentesis ( )\n");
    printf("Ejemplo: 3 + 4 * (2 - 1) ^ 2 / 2\n\n");

    char expr[256];
    double (*tokens)[3] = malloc(128 * sizeof(*tokens));
    if (!tokens) {
        printf("Error de memoria.\n");
        system("pause");
        return 1;
    }
    int num_tokens = -1;

    while (num_tokens < 0) {
        printf("Ingrese una expresion matematica (o presione Enter para usar ejemplo): ");
        if (!fgets(expr, sizeof(expr), stdin)) {
            strcpy(expr, "10 + 3 * (8 - 4) / 2");
        }
        expr[strcspn(expr, "\r\n")] = '\0';
        if (strlen(expr) == 0) {
            strcpy(expr, "10 + 3 * (8 - 4) / 2");
            printf("  Usando expresion de ejemplo: %s\n", expr);
        }
        num_tokens = tokenizar_expresion(expr, tokens, 128);
        if (num_tokens < 0) {
            printf("  [Error] Expresion mal formada. Verifique operadores y parentesis.\n");
        }
    }

    imprimir_matriz_tokens(tokens, num_tokens, 0);

    printf("\n--- PASOS INTERMEDIOS DE LA EVALUACION RECURSIVA ---");
    int error = 0;
    double resultado = evaluar_tokens_rec(tokens, num_tokens, 1, &error);

    if (!error) {
        printf("\n=================================================================\n");
        printf("   RESULTADO FINAL DE \"%s\" = %.4f\n", expr, resultado);
        printf("=================================================================\n\n");
    } else {
        printf("\n  [Evaluacion abortada por error matematico]\n\n");
    }

    free(tokens);
    system("pause");
    return 0;
}
