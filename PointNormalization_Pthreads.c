/// Artigo: Benchmark Cowichan - Toy: Point Location Normalization
/// Aluno: Frederico Peixoto Antunes
/// Status:
/// Problema: Nenhum... eu acho

/*
This module normalizes point coordinates so that all points lie within the unit square [0..1]×[0..1].
If xmin and xmax are the minimum and maximum x coordinate values in the input vector, then the normalization equation is
        xi'	=	(xi — xmin)/(xmax — xmin)
y coordinates are normalized in the same fashion.

Inputs
points: a vector of point locations.

Outputs
points: a vector of normalized point locations.
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

pthread_mutex_t m, n;
pthread_cond_t cond;

typedef struct Point{
    int PointX;
    int PointY;
}Point;

typedef struct FloatPoint{
    float PointX;
    float PointY;
}FloatPoint;

typedef struct Dado{
    int PointX, PointY;
    float NormPointX, NormPointY;
    int Xmax,Xmin,Ymax,Ymin;
    int i;
}Dado;

void *NormalizeX(void *in);
void *NormalizeY(void *in);

int main(){
    Point *Vector;
    int i=0,j,k,l,PointFlag=0;

    ///Cria Vector dinamico e recebe coordenadas dos pontos
    Vector = malloc(sizeof(Point));
    printf("Informe pares de coordenadas dos pontos. Digite -1 para encerrar recebimento de pontos.\n");
    while(scanf("%d",&(Vector[i].PointX)) != EOF && Vector[i].PointX != -1 && scanf("%d",&(Vector[i].PointY)) != EOF && Vector[i].PointY != -1){
        i++;
        Vector = realloc(Vector, sizeof(Point)*(i+1));
    }

    /// Inicializa as funções de pthreads
    pthread_t tid[i+i];
    pthread_mutex_init(&m, NULL);   // pthread init pode retornar erro, fazer teste
    pthread_cond_init(&cond, NULL);

    ///
    Dado Param[i];
    for(j-0;j<i;j++){
        Param[j].PointX = Vector[j].PointX;
        Param[j].PointY = Vector[j].PointY;
        Param[j].Xmax = 0;
        Param[j].Xmin = 2147483647;
        Param[j].Ymax = 0;
        Param[j].Ymin = 2147483647;
    }

    ///Imprime número de pontos registrados
    printf("%d\n",i);
    ///Imprime todas coordenadas registradas em Vector
    for(j=0;j<i;j++){
        printf("(%d,%d) ",Vector[j].PointX,Vector[j].PointY);
    }
    printf("\n");

    ///Percorrer o Vector para encontrar xmax, xmin, ymax, ymin
    //int Xmax=0, Xmin=2147483647, Ymax=0, Ymin=2147483647;
    /*Não compensa paralelizar, precisa checar memória compartilhada para tudo, ficando todo tempo em mutex*/
    for(j=0;j<i;j++){
        if(Vector[j].PointX > Param[j].Xmax)
            Param[j].Xmax = Vector[j].PointX;
        if(Vector[j].PointX < Param[j].Xmin)
            Param[j].Xmin = Vector[j].PointX;
        if(Vector[j].PointY > Param[j].Ymax)
            Param[j].Ymax = Vector[j].PointY;
        if(Vector[j].PointY < Param[j].Ymin)
            Param[j].Ymin = Vector[j].PointY;
    }

    ///Representação gráfica dos pontos informados, em uma matriz
    char YesNot;
    printf("Digite 'Y' para ver representacao grafica: ");
    getchar();
    scanf("%c",&YesNot);
    if(YesNot == 'Y'){
        /*Se for paralelizar, ver depois, representação gráfica não faz parte do toy*/
        for(j=1;j<=Param[0].Ymax;j++){
            for(k=1;k<=Param[0].Xmax;k++){
                for(l=0;l<i;l++){
                    if(Vector[l].PointX == k && Vector[l].PointY == j){
                        PointFlag = 1;
                        printf("o ");
                        break;
                    }
                    PointFlag = 0;
                }
                if(PointFlag == 0){
                    printf("- ");
                }
            }
            printf("\n");
        }
    }

    for(j=0;j<i;j++){
        Param[j].Vector = Vector;
        //Param[j].i = i;     //E aí? Aqui ta certo ou errado? Preciso guardar o i em Dado?
    }
    ///Para cada ponto, cria duas threads, uma para calcular a normalização de X, e outra de Y
    for(j=0;j<i;j++){
        Param[j].i = j;
        pthread_create(&(tid[j]), NULL, NormalizeX, (void*)&(Param[j]));
        pthread_create(&(tid[i+j]), NULL, NormalizeY, (void*)&(Param[j]));
    }

    ///Aguarda o término da normalização
    for(j=0;j<i+i;j++){
        pthread_join(tid[j], NULL);
    }

    ///Imprime todas coordenadas de pontos normalizadas
    for(j=0;j<i;j++){
        printf("(%.2f,%.2f) ",Param[j].Normalized[j].PointX,Param[j].Normalized[j].PointY);
    }
    printf("\n");

    ///Representação gráfica dos pontos normalizados, em uma matriz
    printf("Digite 'Y' para ver representacao grafica: ");
    getchar();
    scanf("%c",&YesNot);
    if(YesNot == 'Y'){
        /*Se for paralelizar, ver depois, representação gráfica não faz parte do toy*/
        for(j=0;j<=100;j++){
            for(k=0;k<=100;k++){
                for(l=0;l<i;l++){
                    if((round(Param[l].Normalized[l].PointX*100)/100) == ((float)k/(float)100) && (round(Param[l].Normalized[l].PointY*100)/100) == ((float)j/(float)100)){
                        PointFlag = 1;
                        printf("o ");
                        break;
                    }
                    PointFlag = 0;
                }
                if(PointFlag == 0){
                    printf("- ");
                }
            }
            printf("\n");
        }
    }

    ///Ao fim do progarama dar free no Vector, por boas práticas
    free(Vector);
    for(j=0;j<i;j++)
        free(Param[j].Normalized);
    return 0;
}

//void *NormalizeX(int j){
void *NormalizeX(void *in){
    Dado *receive = (Dado*) in;

    FloatPoint *Normalized = receive->Normalized;
    //int i = receive->i;

    //Normalized[receive->i].PointX = (float)(receive->Vector[receive->i].PointX - receive->Xmin)/(float)(receive->Xmax - receive->Xmin);
    float temp1 = (float)(receive->Vector[receive->i].PointX - receive->Xmin);
    printf("Fez temp1 no NormalizeX %.2f\n",temp1);
    float temp2 = (float)(receive->Xmax - receive->Xmin);
    Normalized[receive->i].PointX = temp1/temp2;

    printf("NormalizouX %d\n",receive->i);
    return NULL;
}

void *NormalizeY(void *in){
    Dado *receive = (Dado*) in;

    FloatPoint *Normalized = receive->Normalized;
    //int i = receive->i;
    printf("Dados recebidos: %d, ")

    //Normalized[receive->i].PointY = (float)(receive->Vector[receive->i].PointY - receive->Ymin)/(float)(receive->Ymax - receive->Ymin);
    float temp1 = (float)(receive->Vector[receive->i].PointY - receive->Ymin);
    printf("Fez temp1 no NormalizeY %.2f\n",temp1);
    float temp2 = (float)(receive->Ymax - receive->Ymin);
    Normalized[receive->i].PointY = temp1/temp2;

    printf("NormalizouY %d\n",receive->i);
    return NULL;
}
