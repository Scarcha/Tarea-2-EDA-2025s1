#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para isalpha

// --- Implementación de EDA_N1 (Función de Mezcla de Mergesort) ---
void EDA_N1(char arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1; // Tamaño de la sub-lista izquierda
    int n2 = r - m;     // Tamaño de la sub-lista derecha

    // Crear arreglos temporales L y R
    char *L = (char *)malloc(n1 * sizeof(char));
    char *R = (char *)malloc(n2 * sizeof(char));

    // Manejo de errores de asignación de memoria
    if (L == NULL || R == NULL) {
        fprintf(stderr, "Error CRÍTICO: Fallo de asignación de memoria en EDA_N1.\n");
        fflush(stderr);
        if (L) free(L); // Liberar L si ya se asignó
        if (R) free(R); // Liberar R si ya se asignó
        exit(EXIT_FAILURE);
    }

    // Copiar datos al arreglo temporal L
    for (i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    // Copiar datos al arreglo temporal R
    for (j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    // Mezclar los arreglos temporales de nuevo en arr[l..r]
    i = 0; // Índice inicial de la primera sub-lista (L)
    j = 0; // Índice inicial de la segunda sub-lista (R)
    k = l; // Índice inicial del arreglo original (arr)

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copiar los elementos restantes de L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copiar los elementos restantes de R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

// --- Implementación de EDA_SBKN (Mergesort) ---
void EDA_SBKN(char A_char[], int p, int r) {
    if (p < r) {
        int q = p + (r - p) / 2; // Calcular el punto medio
        EDA_SBKN(A_char, p, q);   // Ordenar la primera mitad recursivamente
        EDA_SBKN(A_char, q + 1, r); // Ordenar la segunda mitad recursivamente
        EDA_N1(A_char, p, q, r);  // Mezclar las mitades ordenadas
    }
}

// --- Implementación de EDA_B (Max-Heapify para Heapsort) ---
void EDA_B(char A_char[], int i, int n) {
    int l = 2 * i + 1; // Índice del hijo izquierdo
    int r = 2 * i + 2; // Índice del hijo derecho
    int largest = i;   // Se asume que el padre es el más grande

    // Si el hijo izquierdo existe y es mayor que el actual largest
    if (l < n && A_char[l] > A_char[largest]) {
        largest = l;
    }

    // Si el hijo derecho existe y es mayor que el actual largest
    if (r < n && A_char[r] > A_char[largest]) {
        largest = r;
    }

    // Si el más grande no es el padre, cambiar
    if (largest != i) {
        char temp = A_char[i];
        A_char[i] = A_char[largest];
        A_char[largest] = temp;
        EDA_B(A_char, largest, n);
    }
}

// --- Implementación de EDA_A (Build-Max-Heap para Heapsort) ---
void EDA_A(char A_char[], int n) {
    int i;
    for (i = n / 2 - 1; i >= 0; i--) {
        EDA_B(A_char, i, n);
    }
}

// --- Implementación de EDA_FTW (Heapsort) ---
void EDA_FTW(char A_char[], int n) {
    EDA_A(A_char, n);
    int i;
    for (i = n - 1; i > 0; i--) {
        // Mover la raíz actual (el más grande) al final del arreglo
        char temp = A_char[0];
        A_char[0] = A_char[i];
        A_char[i] = temp;
        EDA_B(A_char, 0, i);
    }
}

// --- Función principal ---
int main(int argc, char *argv[]) {
    FILE *file = NULL;
    long file_size = 0;
    unsigned char *raw_content = NULL;
    size_t bytes_read = 0;
    int alpha_count = 0;
    char *filtered_content_temp = NULL;
    char *data_for_sort = NULL;
    char *data_mergesort = NULL;
    char *data_heapsort = NULL;
    FILE *output_file = NULL;

    // Verificar el número de argumentos de la línea de comandos
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_archivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Abrir el archivo
    file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        return EXIT_FAILURE;
    }

    // Determinar el tamaño del archivo
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("DEBUG: Tamaño del archivo reportado por ftell: %ld bytes\n", file_size);
    fflush(stdout); 

    // Asignar memoria para el contenido crudo del archivo
    raw_content = (unsigned char *)malloc(file_size + 1);
    if (raw_content == NULL) {
        fprintf(stderr, "Error CRÍTICO: Fallo de asignación de memoria para raw_content.\n");
        fflush(stderr);
        fclose(file);
        return EXIT_FAILURE;
    }

    // Leer el archivo en el buffer
    bytes_read = fread(raw_content, 1, file_size, file);
    raw_content[bytes_read] = '\0'; // Asegurar terminación nula
    fclose(file);

    printf("DEBUG: Bytes leídos por fread: %zu bytes\n", bytes_read);
    fflush(stdout);

    // Muestra del contenido crudo
    printf("DEBUG: Primeros 100 caracteres del contenido crudo (o menos):\n");
    fflush(stdout);
    int i;
    for (i = 0; i < (bytes_read > 100 ? 100 : (int)bytes_read); ++i) {
        if (raw_content[i] >= 32 && raw_content[i] <= 126) { 
            putchar(raw_content[i]);
        } else {
            printf("[%02X]", raw_content[i]);
        }
    }
    printf("\nDEBUG: (Fin de la muestra del contenido crudo)\n\n");
    fflush(stdout);

    // Filtrar solo caracteres alfabéticos ASCII estándar y contarlos
    filtered_content_temp = (char *)malloc(bytes_read + 1);
    if (filtered_content_temp == NULL) {
        fprintf(stderr, "Error CRÍTICO: Fallo de asignación de memoria para filtered_content_temp.\n");
        fflush(stderr);
        free(raw_content);
        return EXIT_FAILURE;
    }
    size_t k;
    for (k = 0; k < bytes_read; k++) {
        unsigned char c = raw_content[k];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            filtered_content_temp[alpha_count++] = (char)c;
        }
    }
    filtered_content_temp[alpha_count] = '\0'; // Añadir nulo

    free(raw_content); // Liberar memoria del contenido crudo

    // Si no hay caracteres alfabéticos, salir
    if (alpha_count == 0) {
        printf("El archivo no contiene caracteres alfabéticos ASCII para ordenar.\n");
        free(filtered_content_temp);
        return EXIT_SUCCESS;
    }

    // Asignar memoria para la versión final de los datos a ordenar
    data_for_sort = (char *)malloc(alpha_count + 1);
    if (data_for_sort == NULL) {
        fprintf(stderr, "Error CRÍTICO: Fallo de asignación de memoria para data_for_sort.\n");
        fflush(stderr);
        free(filtered_content_temp);
        return EXIT_FAILURE;
    }
    strcpy(data_for_sort, filtered_content_temp); // Copiar el contenido filtrado
    free(filtered_content_temp); // Liberar el buffer temporal

    // Crear copias de los datos para cada algoritmo de ordenamiento
    data_mergesort = (char *)malloc(alpha_count + 1);
    data_heapsort = (char *)malloc(alpha_count + 1);

    if (data_mergesort == NULL || data_heapsort == NULL) {
        fprintf(stderr, "Error CRÍTICO: Fallo de asignación de memoria para copias de datos de ordenamiento.\n");
        fflush(stderr);
        free(data_for_sort);
        return EXIT_FAILURE;
    }

    strcpy(data_mergesort, data_for_sort); // Copia para Mergesort
    strcpy(data_heapsort, data_for_sort); // Copia para Heapsort

    printf("Iniciando ordenamiento de %d caracteres ASCII alfabéticos...\n", alpha_count);
    fflush(stdout);

    // --- DEBUGGING: Mostrar entrada a Heapsort ---
    printf("DEBUG: data_heapsort ANTES de EDA_FTW (primeros 100 caracteres):\n");
    fflush(stdout);
    for (i = 0; i < (alpha_count > 100 ? 100 : alpha_count); ++i) {
        putchar(data_heapsort[i]);
    }
    printf("\nDEBUG: (Fin de la muestra antes de Heapsort)\n\n");
    fflush(stdout);
    // --- FIN DEBUGGING ---


    // --- Ejecutar EDA_SBKN (Mergesort) ---
    printf("DEBUG: Iniciando EDA_SBKN (Mergesort)...\n");
    fflush(stdout);
    EDA_SBKN(data_mergesort, 0, alpha_count - 1);
    printf("DEBUG: EDA_SBKN (Mergesort) completado.\n");
    fflush(stdout);


    // --- Ejecutar EDA_FTW (Heapsort) ---
    printf("DEBUG: Iniciando EDA_FTW (Heapsort)...\n");
    fflush(stdout);
    EDA_FTW(data_heapsort, alpha_count);
    printf("DEBUG: EDA_FTW (Heapsort) completado.\n");
    fflush(stdout);


    // --- Escribir resultados en el archivo de salida ---
    output_file = fopen("salida.txt", "w");
    if (output_file == NULL) {
        perror("Error al crear salida.txt");
    } else {
        fprintf(output_file, "--- Texto Ordenado con EDA_SBKN (Mergesort) ---\n");
        fprintf(output_file, "%s\n", data_mergesort);
        fprintf(output_file, "(Length: %zu characters)\n\n", strlen(data_mergesort));


        fprintf(output_file, "--- Texto Ordenado con EDA_FTW (Heapsort) ---\n");
        fprintf(output_file, "%s\n", data_heapsort);
        fprintf(output_file, "(Length: %zu characters)\n", strlen(data_heapsort));
        fclose(output_file);
        printf("Resultados guardados en: salida.txt\n");
        fflush(stdout);
    }

    // Liberar memoria
    free(data_for_sort);
    free(data_mergesort);
    free(data_heapsort);

    return EXIT_SUCCESS;
}