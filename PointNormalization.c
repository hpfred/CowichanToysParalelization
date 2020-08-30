#include <stdlib.h>
#include <stdio.h>

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

///i é uma variavel do tamanho do vetor, é o indice do vetor

///Mudar código para que posição começe em 1, e não em 0 (matrizes não tem linha e coluna 0)

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

    Vector = malloc(sizeof(Point));
    ///Aidiconar texto explicando para informar pares de coordenadas de pontos, e digitar -1 para finalizar
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
        for(j=0;j<=Ymax;j++){
            for(k=0;k<=Xmax;k++){
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

    ///Faz o cálculo da normalização dos pontos
    //printf("Entra na normalizacao\n");    //Foi usado para testar quando valores da normalização estavam dando errado
    for(j=0;j<i;j++){
        Normalized[j].PointX = (float)(Vector[j].PointX - Xmin)/(float)(Xmax - Xmin);
        Normalized[j].PointY = (float)(Vector[j].PointY - Ymin)/(float)(Ymax - Ymin);
        //printf("Normalizou %d\n",j);    //Foi usado para testar quando valores da normalização estavam dando errado
    }
    //printf("Sai da normalizacao\n");    //Foi usado para testar quando valores da normalização estavam dando errado

    ///Imprime todas coordenadas de pontos normalizadas
    for(j=0;j<i;j++){
        printf("(%.2f,%.2f) ",Normalized[j].PointX,Normalized[j].PointY);
    }
    printf("\n");

    //(?) Descobrir qual a melhor forma de representar graficamente valor normalizados entre 0 e 1
    ///Representação gráfica dos pontos normalizados, em uma matriz
    printf("Digite 'Y' para ver representacao grafica: ");
    getchar();
    scanf("%c",&YesNot);
    if(YesNot == 'Y'){
        for(j=0;j<i;j++){
            for(k=0;k<i;k++){
                for(l=0;l<i;l++){
                    if(Normalized[l].PointX == ((float)k/(float)i) && Normalized[l].PointY == ((float)j/(float)i)){
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

    ///Resolver paralelização

    ///Ao fim do progarama dar free no Vector, por boas práticas
    free(Vector);
    free(Normalized);
    return 0;
}
