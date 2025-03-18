#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
// Fonction pour récupérer uniquement le nom de fichier à partir d'un chemin
char* getFileNameFromPath(char *path) {
    char *fileName = strrchr(path, '\\');
    if (fileName == NULL) {
        fileName = strrchr(path, '/');
    }
    if (fileName != NULL) {
        return fileName + 1;  // Sauter le caractère '\\' ou '/'
    }
    return path;  // Si aucun slash trouvé, c'est déjà le nom de fichier
}
// Fonction pour remplir le tableau avec des valeurs aléatoires
void fillArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
}

// Fonction pour copier un tableau
void copyArray(int *src, int *dest, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

void measureSortingTime(int n, FILE *dataFile, int algorithms[], int algorithmCount) {
    int *arr = (int *)malloc(n * sizeof(int));
    int *arrCopy = (int *)malloc(n * sizeof(int));
    fillArray(arr, n);

    LARGE_INTEGER start, end, frequency;
    double timeSpent;
    
    QueryPerformanceFrequency(&frequency);  // Fréquence du compteur

    for (int i = 0; i < algorithmCount; i++) {
        copyArray(arr, arrCopy, n);
        QueryPerformanceCounter(&start);

        switch (algorithms[i]) {
            case 1:
                bubbleSort(arrCopy, n);
                break;
            case 2:
                insertionSort(arrCopy, n);
                break;
            case 3:
                selectionSort(arrCopy, n);
                break;
            case 4:
                quickSort(arrCopy, 0, n - 1);
                break;
            case 5:
                mergeSort(arrCopy, 0, n - 1);
                break;
        }

        QueryPerformanceCounter(&end);
        timeSpent = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        fprintf(dataFile, "%.10f ", timeSpent);
    }

    fprintf(dataFile, "\n");
    free(arr);
    free(arrCopy);
}