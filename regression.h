#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_POINTS 10000  // Définir un maximum pour les points

// Lire les données depuis le fichier
int lire_donnees(const char *filename, double x[], double y[], int *n) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return -1;
    }

    *n = 0;
    while (fscanf(file, "%lf %lf", &x[*n], &y[*n]) == 2) {
        (*n)++;
        if (*n >= MAX_POINTS) {
            break;
        }
    }

    fclose(file);
    return 0;
}

// Calcul de l'efficacité (R^2) en pourcentage
double calculer_efficacite(double y[], double y_pred[], int n) 
{
    double sum_y = 0, sum_y_diff2 = 0, sum_residual2 = 0;

    // Calculer la moyenne de y
    for (int i = 0; i < n; i++) {
        sum_y += y[i];
    }
    double mean_y = sum_y / n;

    // Calculer SST et SSR
    for (int i = 0; i < n; i++) {
        double y_diff = y[i] - mean_y;
        double residual = y[i] - y_pred[i];
        sum_y_diff2 += y_diff * y_diff;  // SST
        sum_residual2 += residual * residual;  // SSR
    }

    // Calculer R^2
    double r_squared = 1 - (sum_residual2 / sum_y_diff2);
    return r_squared * 100;
}

// Régression linéaire
void regression_lineaire(double x[], double y[], int n, double *a, double *b) 
{
    double sum_x = 0, sum_y = 0, sum_xx = 0, sum_xy = 0;

    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xx += x[i] * x[i];
        sum_xy += x[i] * y[i];
    }

    *a = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    *b = (sum_y - *a * sum_x) / n;
}

// Prédiction pour la régression linéaire
void calculer_y_pred_lineaire(double x[], double y_pred[], int n, double a, double b) 
{
    for (int i = 0; i < n; i++) {
        y_pred[i] = a * x[i] + b;
    }
}

// Prédiction pour la régression exponentielle
void calculer_y_pred_exponentielle(double x[], double y_pred[], int n, double A, double a) 
{
    for (int i = 0; i < n; i++) {
        y_pred[i] = A * exp(a * x[i]);
    }
}

// Régression logarithmique
void regression_logarithmique(double x[], double y[], int n, double *A, double *B)
{
    double sum_log_x = 0, sum_y = 0, sum_log_xx = 0, sum_log_x_y = 0;

    for (int i = 0; i < n; i++) {
        double log_x = log(x[i]);
        sum_log_x += log_x;
        sum_y += y[i];
        sum_log_xx += log_x * log_x;
        sum_log_x_y += log_x * y[i];
    }

    *A = (n * sum_log_x_y - sum_log_x * sum_y) / (n * sum_log_xx - sum_log_x * sum_log_x);
    *B = (sum_y - *A * sum_log_x) / n;
}

// Prédiction pour la régression logarithmique
void calculer_y_pred_logarithmique(double x[], double y_pred[], int n, double A, double B) 
{
    for (int i = 0; i < n; i++) {
        y_pred[i] = A * log(x[i]) + B;
    }
}

// Régression Axlog(x)
void regression_axlogx(double x[], double y[], int n, double *A, double *B) 
{
    double sum_xlogx = 0, sum_y = 0, sum_xlogx_2 = 0, sum_xlogx_y = 0;

    for (int i = 0; i < n; i++) {
        double xlogx = x[i] * log(x[i]);
        sum_xlogx += xlogx;
        sum_y += y[i];
        sum_xlogx_2 += xlogx * xlogx;
        sum_xlogx_y += xlogx * y[i];
    }

    *A = (n * sum_xlogx_y - sum_xlogx * sum_y) / (n * sum_xlogx_2 - sum_xlogx * sum_xlogx);
    *B = (sum_y - *A * sum_xlogx) / n;
}

// Prédiction pour la régression Axlog(x)
void calculer_y_pred_axlogx(double x[], double y_pred[], int n, double A, double B) 
{
    for (int i = 0; i < n; i++) {
        y_pred[i] = A * x[i] * log(x[i]) + B;
    }
}

// Régression polynomiale (y = Ax^2 + B)
void regression_polynomiale(double x[], double y[], int n, double *A, double *B) 
{
    double sum_x2 = 0, sum_y = 0, sum_x4 = 0, sum_x2_y = 0;

    for (int i = 0; i < n; i++) {
        double x2 = x[i] * x[i];
        sum_x2 += x2;
        sum_y += y[i];
        sum_x4 += x2 * x2;
        sum_x2_y += x2 * y[i];
    }

    *A = (n * sum_x2_y - sum_x2 * sum_y) / (n * sum_x4 - sum_x2 * sum_x2);
    *B = (sum_y - *A * sum_x2) / n;
}

// Prédiction pour la régression polynomiale (y = Ax^2 + B)
void calculer_y_pred_polynomiale(double x[], double y_pred[], int n, double A, double B) 
{
    for (int i = 0; i < n; i++) {
        y_pred[i] = A * x[i] * x[i] + B;
    }
}
int positionBestModel(double array[],int n)
{
    int pos_max=0;
    double max=array[0];
    for(int i=0;i<n;i++)
    {
        if(array[i]>max)
        {
            max=array[i];
            pos_max=i;
        }        
    }
    return pos_max;
}
void predictWithUser(double a_user,double b_user,int position,char triName[])
{
    long long int n;
    while(1)
    {
        printf("Entrez la valeur que vous voulez predire (-1 pour quitter) : \n");
        scanf("%lld",&n);
        if(n==-1) break;
        long double time_predicted;
        if(position==1)
        {    
            time_predicted=a_user*n+b_user;
            printf("Selon mon modele pour que le %s trie %lld element(s) il faut %Lf seconde(s)!\n",triName,n,time_predicted);
        }
        else if(position==1)
        {
            time_predicted=a_user*log(n)+b_user;
            printf("Selon mon modele pour que le %s trie %lld element(s) il faut %Lf seconde(s)!\n",triName,n,time_predicted);
        }
        else if(position==2)
        {
            time_predicted=a_user*n*log(n)+b_user;
            printf("Selon mon modele pour que le %s trie %lld element(s) il faut %Lf seconde(s)!\n",triName,n,time_predicted);
        }
        else if(position==3)
        {
            time_predicted=a_user*n*n+b_user;
            printf("Selon mon modele pour que le %s trie %lld element(s) il faut %Lf seconde(s)!\n",triName,n,time_predicted);
        }
    }
}
void plotTheBestModel(FILE *gnuplotPipe,char triName[])
{
    double x[MAX_POINTS], y[MAX_POINTS], y_pred[MAX_POINTS];
    int n;
    double efficacite_tableau[4];
    if (gnuplotPipe == NULL) {
        printf("Erreur lors de l'ouverture du fichier ou du pipe Gnuplot.\n");
        return ;
    }
    // Lire les données
    if (lire_donnees("sorting_algorithms_time_data.dat", x, y, &n) != 0) {
        return ;
    }

    // Régression linéaire
    double a, b;
    regression_lineaire(x, y, n, &a, &b);
    printf("Regression lineaire: y = %.10fx + %.10f\n", a, b);

    calculer_y_pred_lineaire(x, y_pred, n, a, b);
    double efficacite_lineaire = calculer_efficacite(y, y_pred, n);
    printf("Efficacite de la regression lineaire: %.10f%%\n", efficacite_lineaire);
    efficacite_tableau[0]=efficacite_lineaire;
    // Régression logarithmique
    double log_A, log_B;
    regression_logarithmique(x, y, n, &log_A, &log_B);
    printf("Regression logarithmique: y = %.10flog(x) + %.10f\n", log_A, log_B);

    calculer_y_pred_logarithmique(x, y_pred, n, log_A, log_B);
    double efficacite_logarithmique = calculer_efficacite(y, y_pred, n);
    printf("Efficacite de la regression logarithmique: %.10f%%\n", efficacite_logarithmique);
    efficacite_tableau[1]=efficacite_logarithmique;
    // Régression Axlog(x)
    double axlog_A, axlog_B;
    regression_axlogx(x, y, n, &axlog_A, &axlog_B);
    printf("Regression Axlog(x): y = %.10fxlog(x) + %.10f\n", axlog_A, axlog_B);

    calculer_y_pred_axlogx(x, y_pred, n, axlog_A, axlog_B);
    double efficacite_axlogx = calculer_efficacite(y, y_pred, n);
    printf("Efficacite de la regression Axlog(x): %.10f%%\n", efficacite_axlogx);
    efficacite_tableau[2]=efficacite_axlogx;
    // Régression polynomiale (y = Ax^2 + B)
    double poly_A, poly_B;
    regression_polynomiale(x, y, n, &poly_A, &poly_B);
    printf("Regression polynomiale: y = %.10fx^2 + %.10f\n", poly_A, poly_B);

    calculer_y_pred_polynomiale(x, y_pred, n, poly_A, poly_B);
    double efficacite_polynomiale = calculer_efficacite(y, y_pred, n);
    printf("Efficacite de la regression polynomiale: %.10f%%\n", efficacite_polynomiale);
    efficacite_tableau[3]=efficacite_polynomiale;
    int position = positionBestModel(efficacite_tableau,4);
    double a_user,b_user,accuracy;
    if(position==0)
    {
        accuracy=efficacite_lineaire;
        fprintf(gnuplotPipe,"(%.10f*x+%.10f) title 'Linear model(ax+b) : (%.10f*x**2+%.10f) accuracy(%.3f%%)'\n",a,b,a,b,accuracy);
        a_user=a;
        b_user=b;
    }
    else if(position==1)
    {
        accuracy=efficacite_logarithmique;
        fprintf(gnuplotPipe,"(%.10f*log(x)+%.10f) title 'Logarithmic model(alog(x)+b) : (%.10f*x**2+%.10f) accuracy(%.3f%%)'\n",log_A, log_B,log_A,log_B,accuracy);
        a_user=log_A;
        b_user=log_B;
    }
    else if(position==2)
    {
        accuracy=efficacite_axlogx;
        fprintf(gnuplotPipe,"(%.10f*x*log(x)+%.10f) title 'Linear-logarithmic model(axlog(x)+b) : (%.10f*x**2+%.10f) accuracy(%.3f%%)'\n",axlog_A, axlog_B,axlog_A,axlog_B,accuracy);
        a_user=axlog_A;
        b_user=axlog_B;
    }
    else if(position==3)
    {
        accuracy=efficacite_polynomiale;
        fprintf(gnuplotPipe,"(%.10f*x**2+%.10f) title 'Polynomial model(ax^2+b) : (%.10f*x**2+%.10f) accuracy(%.3f%%)'\n",poly_A,poly_B,poly_A,poly_B,accuracy);
        a_user=poly_A;
        b_user=poly_B;
    }
    fprintf(gnuplotPipe, "\n");
    fprintf(gnuplotPipe, "exit\n");
    fflush(gnuplotPipe);
    //laisser mon programme partir vers la ligne suivante
    predictWithUser(a_user,b_user,position,triName);
    
}