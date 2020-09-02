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
#include <math.h>
#include <omp.h>
#include <time.h>

typedef struct Point{
    int PointX;
    int PointY;
}Point;

typedef struct FloatPoint{
    float PointX;
    float PointY;
}FloatPoint;

void CoordVec(int i, Point *Vector);
void CoordNorm(int i, FloatPoint *Normalized);
void GraphVec(int Xmax,int Ymax,int i,Point *Vector);
void GraphNorm(int i,FloatPoint *Normalized);

int main(){
    Point *Vector;
    FloatPoint *Normalized;
    int i=0,j;
    double start, end;

    FILE *arquivo;
    arquivo = fopen("Arquivo","r");

    ///Cria Vector dinamico e recebe coordenadas dos pontos
    Vector = malloc(sizeof(Point));
    printf("Informe pares de coordenadas dos pontos. Digite -1 para encerrar recebimento de pontos.\n");
    while(fscanf(arquivo,"%d",&(Vector[i].PointX)) != EOF && Vector[i].PointX != -1 && fscanf(arquivo,"%d",&(Vector[i].PointY)) != EOF && Vector[i].PointY != -1){
        i++;
        Vector = realloc(Vector, sizeof(Point)*(i+1));
    }
    Normalized = malloc(sizeof(FloatPoint)*(i+1));

    omp_set_num_threads(i);

    ///Imprime número de pontos registrados
    printf("No Pontos: %d\n",i);
    ///Imprime todas coordenadas registradas em Vector
    //CoordVec(i,Vector);

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

    ///Representação gráfica dos pontos informados, em uma matriz
    //GraphVec(Xmax,Ymax,i,Normalized);

    start = omp_get_wtime();

    ///Faz o cálculo da normalização paralelizado
    ///Para cada ponto, cria uma thread, calculando a normalização de X e Y
    #pragma omp parallel for
    for(j=0;j<i;j++){
        Normalized[j].PointX = (float)(Vector[j].PointX - Xmin)/(float)(Xmax - Xmin);
        Normalized[j].PointY = (float)(Vector[j].PointY - Ymin)/(float)(Ymax - Ymin);
    }

    ///Outra forma de fazer com provavel maior overhead
    ///Para cada ponto, cria duas threads, uma para calcular a normalização de X, e outra de Y
    /*
    #pragma omp parallel private(i) reduction(+:somatorio)
    {
        #pragma omp for
        for(j=0;j<i;j++){
            Normalized[j].PointY = (float)(Vector[j].PointY - Ymin)/(float)(Ymax - Ymin);
        }
        #pragma omp for
        for(j=0;j<i;j++){
            Normalized[j].PointY = (float)(Vector[j].PointY - Ymin)/(float)(Ymax - Ymin);
        }
    }
    */

    end = omp_get_wtime();

    ///Imprime todas coordenadas de pontos normalizadas
    //CoordNorm(i,Normalized);

    ///Representação gráfica dos pontos normalizados, em uma matriz
    //GraphNorm(i,Normalized);

    ///Imprime o tempo registrado
    printf("\nTempo: %4.5lf\n",end-start);

    ///Ao fim do progarama dar free no Vector, por boas práticas
    free(Vector);
    free(Normalized);
    fclose(arquivo);
    return 0;
}

void CoordVec(int i, Point *Vector){
    int j;
    for(j=0;j<i;j++){
        printf("(%d,%d) ",Vector[j].PointX,Vector[j].PointY);
    }
    printf("\n");
}

void CoordNorm(int i, FloatPoint *Normalized){
    int j;
    for(j=0;j<i;j++){
        printf("(%.2f,%.2f) ",Normalized[j].PointX,Normalized[j].PointY);
    }
    printf("\n");
}

void GraphVec(int Xmax,int Ymax,int i,Point *Vector){
    char YesNot;
    int PointFlag=0;
    int j,k,l;

    printf("Digite 'Y' para ver representacao grafica: ");
    //getchar();
    scanf("%c",&YesNot);
    if(YesNot == 'Y'){
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
}

void GraphNorm(int i,FloatPoint *Normalized){
    #define res 50                  //res standing for resolution
    char YesNot;
    int PointFlag=0;
    int j,k,l;

    printf("Digite 'Y' para ver representacao grafica: ");
    getchar();
    scanf("%c",&YesNot);
    if(YesNot == 'Y'){
        for(j=0;j<=res;j++){
            for(k=0;k<=res;k++){
                for(l=0;l<i;l++){
                    if((round(Normalized[l].PointX*res)/res) == ((float)k/(float)res) && (round(Normalized[l].PointY*res)/res) == ((float)j/(float)res)){
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
}
