// NACHIT Taha, ZOUHARI Dyae errahmane, RIANI Aymen : GLSID ; KRACHEL Fahd : BDCC


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "regression.h"
#include "algorithms.h"
#include "utilities.h"
// Fonction pour traiter les arguments et exécuter le programme en fonction de ceux-ci
int main(int argc, char *argv[]) {
    HANDLE hProcess = GetCurrentProcess();
    // Définir la priorité sur HIGH_PRIORITY_CLASS
    if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS)) {
    } else {
        printf("Erreur lors du changement de priorite.\n");
    }
    FILE *dataFile = fopen("sorting_algorithms_time_data.dat", "w");
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (dataFile == NULL || gnuplotPipe == NULL) {
        printf("Erreur lors de l'ouverture du fichier ou du pipe Gnuplot.\n");
        return 1;
    }
    // Variables pour gérer les arguments
    int algorithms[5];  // 1: bubble, 2: insertion, 3: selection, 4: quick, 5: merge
    int algorithmCount = 0;
    int sizeStart = 10, sizeEnd = 10000, sizeStep = 10;  // Plage de tailles par défaut
    int predict = 0; // Prédiction annulé par défaut
    int algoExistance=0;
    int help=0;
    // Lecture des arguments
    for (int i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], "-algorithm") == 0) 
        {
            algoExistance=1;
            // Lire les algorithmes spécifiés
            for (int j = i + 1; j < argc && argv[j][0] != '-'; j++) 
            {
                if(strcmp(argv[j],"all")==0)
                {
                    for(int k=0;k<5;k++)
                    {
                        algorithms[k]=k+1;
                    }
                    algorithmCount=5;
                    break;
                }
                else
                {
                    if (strcmp(argv[j], "bubblesort") == 0) 
                    {
                        algorithms[algorithmCount++] = 1;
                    }
                    else if (strcmp(argv[j], "insertionsort") == 0) 
                    {
                        algorithms[algorithmCount++] = 2;
                    }
                    else if (strcmp(argv[j], "selectionsort") == 0) 
                    {
                        algorithms[algorithmCount++] = 3;
                    }
                    else if (strcmp(argv[j], "quicksort") == 0) 
                    {
                        algorithms[algorithmCount++] = 4;
                    }
                    else if (strcmp(argv[j], "mergesort") == 0) 
                    {
                        algorithms[algorithmCount++] = 5;
                    }
                }
            }
        } 
        else if (strcmp(argv[i], "-size") == 0) 
        {
            // Lire la plage de tailles (format : start/end/step)
            sscanf(argv[i + 1], "%d/%d/%d", &sizeStart, &sizeEnd, &sizeStep);
            i++;  // Sauter l'argument taille
        }
        else if(strcmp(argv[i], "-predict")==0)
        {
            // Lire la plage de tailles (format : start/end/step)
            sscanf(argv[i + 1], "%d", &predict);
            i++;  // Sauter l'argument taille
        }
        else if(strcmp(argv[i],"-help")==0)
        {
            //Ecrit moi un help sur les commandes de mon programme
            printf("Ce programme compare les performances de plusieurs algorithmes de tri sur un tableau de donnees aleatoires.\n");
            printf("\nOptions :\n");
            printf("  -algorithm [algorithmes]    Specifie les algorithmes de tri a utiliser. Les options possibles sont :\n");
            printf("                               bubblesort, insertionsort, selectionsort, quicksort, mergesort, all\n");
            printf("                               Par defaut, tous les algorithmes sont executes.\n");
            printf("\n");
            printf("  -size start/end/step        Definit la plage de tailles des tableaux a trier. Par defaut : 10/10000/10\n");
            printf("                               - start : taille de depart (par defaut 10)\n");
            printf("                               - end : taille finale (par defaut 10000)\n");
            printf("                               - step : incrementation entre les tailles (par defaut 10)\n");
            printf("\n");
            printf("  -predict [1 ou 0]           Active ou desactive la prediction du modele de regression pour les temps d'execution.\n");
            printf("                               1 pour activer, 0 pour desactiver (par defaut desactive).\n");
            printf("\n");
            printf("  -help                       Affiche ce message d'aide.\n");
            printf("\nExemple d'utilisation :\n");
            printf("  ./%s -algorithm bubblesort insertionsort -size 100/10000/100\n", getFileNameFromPath(argv[0]));
            printf("  -> Execute les tris par bulles et par insertion pour des tailles de tableaux allant de 100 a 10000 par pas de 100.\n");
            printf("\n");
            help=1;
        }
    }
    if(help==0)
    {
        printf("Bienvenue dans le programme de comparation des tris\n");
        printf("Ce programme va vous aider a mieux comprendre les differents algorithmes de tris ainsi que de savoir le comportement de chaque tri\n");
        printf("Si vous voulez plus savoir sur notre programme n'hesitez pas a taper la commande [./%s -help]\n",argv[0]);
        printf("Utilisation : ./%s [OPTIONS]\n", argv[0]);
        if(algoExistance==0)
        {
            for(int k=0;k<5;k++)
            {
                algorithms[k]=k+1;
            }
            algorithmCount=5;
        }
        // Exécution des algorithmes de tri pour chaque taille
        for (int n = sizeStart; n <= sizeEnd; n += sizeStep) 
        {
            fprintf(dataFile, "%d ", n);  // Enregistrer la taille
            measureSortingTime(n, dataFile, algorithms, algorithmCount);
        }
        fclose(dataFile);
        // Génération du graphique avec Gnuplot
        fprintf(gnuplotPipe, "set title 'Sorting Algorithms Time Complexity'\n");
        fprintf(gnuplotPipe, "set xlabel 'Number of Elements'\n");
        fprintf(gnuplotPipe, "set ylabel 'Time (seconds)'\n");
        fprintf(gnuplotPipe, "plot ");
        // Gérer les algorithmes dans Gnuplot en fonction des arguments
        if(algorithmCount>1)
        {
            for (int i = 0; i < algorithmCount; i++) 
            {
                if (algorithms[i] == 1) 
                {
                    fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:%d with points pt 7 ps 0.1 title 'Bubble Sort', ", i + 2);
                }
                else if (algorithms[i] == 2) 
                {
                    fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:%d with points pt 7 ps 0.1 title 'Insertion Sort', ", i + 2);
                }
                else if (algorithms[i] == 3) 
                {
                    fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:%d with points pt 7 ps 0.1 title 'Selection Sort', ", i + 2);
                }
                else if (algorithms[i] == 4) 
                {
                    fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:%d with points pt 7 ps 0.1 title 'Quick Sort', ", i + 2);
                }
                else if (algorithms[i] == 5) 
                {
                    fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:%d with points pt 7 ps 0.1 title 'Merge Sort', ", i + 2);
                }
            }
            fprintf(gnuplotPipe, "\n");
            fprintf(gnuplotPipe, "exit\n");
            fflush(gnuplotPipe);
        }
        else
        {
            if (algorithms[0] == 1) 
            {
                fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:%d with points pt 7 ps 0.1 title 'Bubble Sort', ", 2);
                if(predict==1)
                {
                    plotTheBestModel(gnuplotPipe,"Bubble Sort");
                }
                else
                {
                    fprintf(gnuplotPipe, "\n");
                    fprintf(gnuplotPipe, "exit\n");
                    fflush(gnuplotPipe);
                }
            }
            else if (algorithms[0] == 2) 
            {
                fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:%d with points pt 7 ps 0.1 title 'Insertion Sort', ", 2);
                if(predict==1)
                {
                    plotTheBestModel(gnuplotPipe,"Bubble Sort");
                }
                else
                {
                    fprintf(gnuplotPipe, "\n");
                    fprintf(gnuplotPipe, "exit\n");
                    fflush(gnuplotPipe);
                }
            }
            else if (algorithms[0] == 3) 
            {
                fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:%d with points pt 7 ps 0.1 title 'Selection Sort', ", 2);
                if(predict==1)
                {
                    plotTheBestModel(gnuplotPipe,"Selection Sort");
                }
                else
                {
                    fprintf(gnuplotPipe, "\n");
                    fprintf(gnuplotPipe, "exit\n");
                    fflush(gnuplotPipe);
                }
            }
            else if (algorithms[0] == 4) 
            {
                fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:%d with points pt 7 ps 0.1 title 'Quick Sort', ",2);
                if(predict==1)
                {
                    plotTheBestModel(gnuplotPipe,"Quick Sort");
                }
                else
                {
                    fprintf(gnuplotPipe, "\n");
                    fprintf(gnuplotPipe, "exit\n");
                    fflush(gnuplotPipe);
                }
            }
            else if (algorithms[0] == 5) 
            {
                fprintf(gnuplotPipe, "'sorting_algorithms_time_data.dat' using 1:%d with points pt 7 ps 0.1 title 'Merge Sort', ",2);
                if(predict==1)
                {
                    plotTheBestModel(gnuplotPipe,"Merge Sort");
                }
                else
                {
                    fprintf(gnuplotPipe, "\n");
                    fprintf(gnuplotPipe, "exit\n");
                    fflush(gnuplotPipe);
                }
            }
            
        }
    }
    return 0;
}