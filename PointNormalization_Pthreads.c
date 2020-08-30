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
    for(j=0;j<i;j++){
        Param[j].PointX = Vector[j].PointX;
        Param[j].PointY = Vector[j].PointY;
    }

    ///Imprime número de pontos registrados
    //printf("%d\n",i);
    ///Imprime todas coordenadas registradas em Vector
    for(j=0;j<i;j++){
        printf("(%d,%d) ",Vector[j].PointX,Vector[j].PointY);
    }
    printf("\n");

    int Xmax=0,Xmin=2147483647,Ymax=0,Ymin=2147483647;
    ///Percorrer o Vector para encontrar xmax, xmin, ymax, ymin
    for(j=0;j<i;j++){
        if(Vector[j].PointX > Xmax)
            Xmax = Vector[j].PointX;
        if(Vector[j].PointX < Xmin)
            Xmin = Vector[j].PointX;
        if(Vector[j].PointY > Ymax)
            Ymax = Vector[j].PointY;
        if(Vector[j].PointY < Ymin)
            Ymin = Vector[j].PointY;
    }

    for(j=0;j<i;j++){
        Param[j].Xmax = Xmax;
        Param[j].Xmin = Xmin;
        Param[j].Ymax = Ymax;
        Param[j].Ymin = Ymin;

        //printf("Xmx=%d Xmn=%d Ymx=%d Ymn=%d\n",Param[j].Xmax,Param[j].Xmin,Param[j].Ymax,Param[j].Ymin);
    }

    ///Representação gráfica dos pontos informados, em uma matriz
    char YesNot;
    printf("Digite 'Y' para ver representacao grafica: ");
    getchar();
    scanf("%c",&YesNot);
    if(YesNot == 'Y'){
        /*Se for paralelizar, ver depois, representação gráfica não faz parte do toy*/
        for(j=1;j<=Ymax;j++){
            for(k=1;k<=Xmax;k++){
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
        printf("(%.2f,%.2f) ",Param[j].NormPointX,Param[j].NormPointY);
    }
    printf("\n");

    ///Representação gráfica dos pontos normalizados, em uma matriz
    printf("Digite 'Y' para ver representacao grafica: ");
    getchar();
    scanf("%c",&YesNot);
    if(YesNot == 'Y'){
        /*Se for paralelizar, ver depois, representação gráfica não faz parte do toy*/
        #define res 50                  //res standing fro resoltion
        for(j=0;j<=res;j++){
            for(k=0;k<=res;k++){
                for(l=0;l<i;l++){
                    if((round(Param[l].NormPointX*res)/res) == ((float)k/(float)res) && (round(Param[l].NormPointY*res)/res) == ((float)j/(float)res)){
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
    return 0;
}

//void *NormalizeX(int j){
void *NormalizeX(void *in){
    Dado *receive = (Dado*) in;

    //Normalized[receive->i].PointX = (float)(receive->Vector[receive->i].PointX - receive->Xmin)/(float)(receive->Xmax - receive->Xmin);
    float temp1 = (receive->PointX - receive->Xmin);
    //printf("X-Xmin: %d-%d = %d\n",receive->PointX,receive->Xmin,temp1);
    float temp2 = (receive->Xmax - receive->Xmin);
    //printf("Xmax-Xmin: %d-%d = %d\n",receive->Xmax,receive->Xmin,temp2);
    receive->NormPointX = temp1/temp2;

    //printf("NormalizouX %d\n",receive->i);
    return NULL;
}

void *NormalizeY(void *in){
    Dado *receive = (Dado*) in;

    //Normalized[receive->i].PointY = (float)(receive->Vector[receive->i].PointY - receive->Ymin)/(float)(receive->Ymax - receive->Ymin);
    float temp1 = (receive->PointY - receive->Ymin);
    //printf("Y-Ymin: %d-%d = %d\n",receive->PointY,receive->Ymin,temp1);
    float temp2 = (receive->Ymax - receive->Ymin);
    //printf("Ymax-Ymin: %d-%d = %d\n",receive->Ymax,receive->Ymin,temp2);
    receive->NormPointY = temp1/temp2;

    //printf("NormalizouY %d\n",receive->i);
    return NULL;
}
