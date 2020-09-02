/// Artigo: Benchmark Cowichan - Toy: Point Location Normalization
/// Aluno: Frederico Peixoto Antunes
/// Status: Funcionando
/// Descrição: Implementação do toy sem nenhum sistema de paralelização

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
#include <sys/time.h>

typedef struct Point{
    int PointX;
    int PointY;
}Point;

typedef struct FloatPoint{
    float PointX;
    float PointY;
}FloatPoint;

int main(){
    Point *Vector;
    FloatPoint *Normalized;
    int i=0,j,k,l,PointFlag=0;
    struct timeval  start, end;

    Vector = malloc(sizeof(Point));
    printf("Informe pares de coordenadas dos pontos. Digite -1 para encerrar recebimento de pontos.\n");
    while(scanf("%d",&(Vector[i].PointX)) != EOF && Vector[i].PointX != -1 && scanf("%d",&(Vector[i].PointY)) != EOF && Vector[i].PointY != -1){
        i++;
        Vector = realloc(Vector, sizeof(Point)*(i+1));
    }
    Normalized = malloc(sizeof(FloatPoint)*(i+1));

    ///Imprime número de pontos registrados
    printf("%d\n",i);
    ///Imprime todas coordenadas registradas em Vector
    for(j=0;j<i;j++){
        printf("(%d,%d) ",Vector[j].PointX,Vector[j].PointY);
    }
    printf("\n");

    ///Percorrer o Vector para encontrar xmax, xmin, ymax, ymin
    int Xmax=0, Xmin=2147483647, Ymax=0, Ymin=2147483647;
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
    char YesNot;
    printf("Digite 'Y' para ver representacao grafica: ");
    getchar();
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

    gettimeofday(&start, NULL);

    ///Faz o cálculo da normalização dos pontos
    for(j=0;j<i;j++){
        Normalized[j].PointX = (float)(Vector[j].PointX - Xmin)/(float)(Xmax - Xmin);
        Normalized[j].PointY = (float)(Vector[j].PointY - Ymin)/(float)(Ymax - Ymin);
    }

    gettimeofday(&end, NULL);

    ///Imprime todas coordenadas de pontos normalizadas
    for(j=0;j<i;j++){
        printf("(%.2f,%.2f) ",Normalized[j].PointX,Normalized[j].PointY);
    }
    printf("\n");

    ///Representação gráfica dos pontos normalizados, em uma matriz
    printf("Digite 'Y' para ver representacao grafica: ");
    getchar();
    scanf("%c",&YesNot);
    if(YesNot == 'Y'){
        for(j=0;j<=100;j++){
            for(k=0;k<=100;k++){
                for(l=0;l<i;l++){
                    if((round(Normalized[l].PointX*100)/100) == ((float)k/(float)100) && (round(Normalized[l].PointY*100)/100) == ((float)j/(float)100)){
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

    ///Imprime o tempo registrado
    //printf("\nTempo: %lf\n",(double)(end - start)/CLOCKS_PER_SEC);
    printf("Total time = %f seconds\n",(double)(tv2.tv_usec-tv1.tv_usec)/1000000+(double)(tv2.tv_sec-tv1.tv_sec));

    ///Ao fim do progarama dar free no Vector, por boas práticas
    free(Vector);
    free(Normalized);
    return 0;
}
