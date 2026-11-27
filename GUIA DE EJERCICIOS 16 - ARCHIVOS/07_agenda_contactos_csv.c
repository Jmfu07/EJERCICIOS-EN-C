/* ============================================================================
 * GUIA DE EJERCICIOS 16 (ARCHIVOS) - EJERCICIO 07:
 * Agenda de contactos en archivo de texto CSV (nombre;telefono;correo) con
 * alta, listado, busqueda, modificacion y eliminacion fisica con archivo tmp.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define ARCH_CSV "agenda_contactos.csv"
#define ARCH_TMP "agenda_tmp.csv"

static void leer_campo(const char *msg, char *dest, int tam) {
    for (;;) {
        printf("%s", msg);
        if (!fgets(dest, tam, stdin)) exit(0);
        dest[strcspn(dest, "\r\n")] = '\0';
        if (dest[0] != '\0' && !strchr(dest, ';')) return;
        printf("  Error: no puede estar vacio ni contener ';'.\n");
    }
}

static int iguales_ci(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == *b;
}

static void agregar_contacto(void) {
    char nom[64], tel[32], mail[64];
    FILE *f = fopen(ARCH_CSV, "a");
    if (!f) { printf("  [Error] No se pudo abrir la agenda.\n"); return; }
    leer_campo("  Nombre   : ", nom, (int)sizeof nom);
    leer_campo("  Telefono : ", tel, (int)sizeof tel);
    leer_campo("  Correo   : ", mail, (int)sizeof mail);
    fprintf(f, "%s;%s;%s\n", nom, tel, mail);
    fclose(f);
    printf("  [OK] Contacto agregado.\n");
}

static void listar_contactos(void) {
    FILE *f = fopen(ARCH_CSV, "r");
    char linea[200], nom[64], tel[32], mail[64];
    int n = 0;
    if (!f) { printf("  (Agenda vacia)\n"); return; }
    printf("\n%-3s | %-22s | %-15s | %-28s\n", "#", "NOMBRE", "TELEFONO", "CORREO");
    printf("----+------------------------+-----------------+-----------------------------\n");
    while (fgets(linea, sizeof linea, f)) {
        if (sscanf(linea, "%63[^;];%31[^;];%63[^\r\n]", nom, tel, mail) == 3)
            printf("%-3d | %-22s | %-15s | %-28s\n", ++n, nom, tel, mail);
    }
    if (n == 0) printf("  (Sin registros)\n");
    fclose(f);
}

static void buscar_o_eliminar(int eliminar) {
    FILE *fin = fopen(ARCH_CSV, "r"), *fout = NULL;
    char buscado[64], linea[200], nom[64], tel[32], mail[64];
    int hallado = 0;
    if (!fin) { printf("  (Agenda vacia)\n"); return; }
    leer_campo(eliminar ? "  Nombre exacto a eliminar: " : "  Nombre a buscar: ", buscado, (int)sizeof buscado);
    if (eliminar) {
        fout = fopen(ARCH_TMP, "w");
        if (!fout) { fclose(fin); return; }
    }
    while (fgets(linea, sizeof linea, fin)) {
        if (sscanf(linea, "%63[^;];%31[^;];%63[^\r\n]", nom, tel, mail) == 3) {
            if (iguales_ci(nom, buscado)) {
                hallado = 1;
                if (!eliminar) printf("  -> Encontrado: %s | Tel: %s | Correo: %s\n", nom, tel, mail);
                else continue;
            }
        }
        if (eliminar) fputs(linea, fout);
    }
    fclose(fin);
    if (eliminar) {
        fclose(fout);
        remove(ARCH_CSV);
        rename(ARCH_TMP, ARCH_CSV);
        printf(hallado ? "  [OK] Contacto eliminado.\n" : "  [Aviso] No se encontro ese nombre.\n");
    } else if (!hallado) {
        printf("  [Aviso] No se encontro ese nombre.\n");
    }
}

int main(void) {
    int op;
    for (;;) {
        printf("\n=== ARCHIVOS 07: AGENDA DE CONTACTOS CSV ===\n");
        printf("1. Agregar contacto\n2. Listar todos\n3. Buscar por nombre\n4. Eliminar contacto\n0. Salir\nOpcion: ");
        do { op = _getch(); if (op == 0 || op == 0xE0) { _getch(); op = 0; } } while (op < '0' || op > '4');
        printf("%c\n", op);
        op -= '0';
        if (op == 0) break;
        if (op == 1) agregar_contacto();
        else if (op == 2) listar_contactos();
        else if (op == 3) buscar_o_eliminar(0);
        else if (op == 4) buscar_o_eliminar(1);
    }
    return 0;
}
