#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOCALES 10
#define CANT_TRIMESTRE 3

typedef struct {
    int localNum;
    int trimestreNum;
    int importe;
} Local;

// Prototipos de las funciones
void menu();
void grabarLocal();
void mostrarLocales();
void calcularTotales(int ventas[][CANT_TRIMESTRE], int localIDs[], int numLocales);
void vaciarArchivo();

int main() {
    menu();
    return 0;
}

// Función para mostrar el menú y gestionar las opciones
void menu() {
    int opcion = -1;

    while (opcion != 0) {
        printf("\n---------- *MENU* ----------\n");
        printf("\nOpcion: 1 Grabar Local\n");
        printf("\nOpcion: 2 Mostrar Locales\n");
        printf("\nOpcion: 3 Vaciar archivo\n");
        printf("\nOpcion: 0 Salir del menu\n");
        printf("\nIngrese la opcion deseada: ");
        scanf("%d", &opcion);

        if (opcion > 3 || opcion < 0) {
            printf("\nERROR OPCION NO EXISTENTE.\n");
        }

        switch (opcion) {
            case 0:
                printf("Saliste del menu con exito.\n");
                break;
            case 1:
                grabarLocal();
                break;
            case 2:
                mostrarLocales();
                break;
            case 3:
                vaciarArchivo();
                break;
        }
    }
}

// Función para grabar los datos de un local
void grabarLocal() {
    FILE *file = fopen("locales.txt", "a");

    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    Local local;

    while (1) {
        printf("Ingrese el numero de local: ");
        scanf("%d", &local.localNum);

        if (local.localNum == 0) {
            break;
        }

        for (int i = 1; i <= CANT_TRIMESTRE; i++) {
            local.trimestreNum = i;
            printf("Ingrese el importe vendido del trimestre %d: ", i);
            scanf("%d", &local.importe);

            // Guardar la información en el archivo
            fprintf(file, "%d %d %d\n", local.localNum, local.trimestreNum, local.importe);
        }

        printf("Si desea dejar de grabar ingrese un 0\n");
    }

    fclose(file);
}

// Función para mostrar los locales y sus totales
void mostrarLocales() {
    FILE *file = fopen("locales.txt", "r");

    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    Local local;
    int ventas[MAX_LOCALES][CANT_TRIMESTRE] = {0};
    int localIDs[MAX_LOCALES] = {0};
    int numLocales = 0;

    // Leer los datos del archivo y almacenarlos en el arreglo
    while (fscanf(file, "%d %d %d", &local.localNum, &local.trimestreNum, &local.importe) == 3) {
        int id = -1;
        for (int i = 0; i < numLocales; i++) {
            if (localIDs[i] == local.localNum) {
                id = i;
                break;
            }
        }

        if (id == -1) {
            if (numLocales < MAX_LOCALES) {
                id = numLocales++;
                localIDs[id] = local.localNum;
            } else {
                printf("Error: el número máximo de locales permitidos (%d) ha sido alcanzado.\n", MAX_LOCALES);
                fclose(file);
                return;
            }
        }

        ventas[id][local.trimestreNum - 1] += local.importe;
    }

    fclose(file);

    // Calcular y mostrar los totales
    calcularTotales(ventas, localIDs, numLocales);
}

// Función para calcular y mostrar los totales de ventas
void calcularTotales(int ventas[][CANT_TRIMESTRE], int localIDs[], int numLocales) {
    printf("\n\t\tTrimestre 1\t\tTrimestre 2\t\tTrimestre 3\t\tTotal Local\n");

    int totalTrimestres[CANT_TRIMESTRE] = {0};
    int maxVentasTrimestre[CANT_TRIMESTRE] = {0};
    int localMaxVentas[CANT_TRIMESTRE] = {0};

    for (int i = 0; i < numLocales; i++) {
        int totalLocal = 0;

        for (int j = 0; j < CANT_TRIMESTRE; j++) {
            totalLocal += ventas[i][j];
        }

        if(localIDs[i] < 10){
            printf("Local %d\t\t\t", localIDs[i]);
        }else{
            printf("Local %d\t\t", localIDs[i]);
        }

        for (int j = 0; j < CANT_TRIMESTRE; j++) {
            printf("%d\t\t\t", ventas[i][j]);
            totalTrimestres[j] += ventas[i][j];

            if (ventas[i][j] > maxVentasTrimestre[j]) {
                maxVentasTrimestre[j] = ventas[i][j];
                localMaxVentas[j] = localIDs[i];
            }
        }

        printf("%d\n", totalLocal);
    }

    printf("Total Trimestre\t\t");
    int totalGeneral = 0;

    for (int j = 0; j < CANT_TRIMESTRE; j++) {
        printf("%d\t\t\t", totalTrimestres[j]);
        totalGeneral += totalTrimestres[j];
    }

    printf("%d\n", totalGeneral);

    printf("\nLocales con mas ventas en cada trimestre:\n");
    printf("\nTrimestre 1\t Trimestre 2\t Trimestre 3\n");
    printf("\t%d\t \t%d\t \t%d\n", localMaxVentas[0], localMaxVentas[1], localMaxVentas[2]);
}


// Función para vaciar el contenido del archivo
void vaciarArchivo() {
    FILE *file = fopen("locales.txt", "w");

    if (file != NULL) {
        fclose(file);
        printf("Archivo vaciado exitosamente.\n");
    } else {
        printf("Error al vaciar el archivo\n");
    }
}
