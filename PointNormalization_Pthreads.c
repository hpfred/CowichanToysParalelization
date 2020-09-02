/// Artigo: Benchmark Cowichan - Toy: Point Location Normalization
/// Aluno: Frederico Peixoto Antunes
/// Status:
/// Problema: Nenhum... eu acho

/*
This module normalizes point coordinates so that all points lie within the unit square [0..1]�[0..1].
If xmin and xmax are the minimum and maximum x coordinate values in the input vector, then the normalization equation is
        xi'	=	(xi � xmin)/(xmax � xmin)
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
#include <sys/time.h>

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

int freeThreads;

void *NormalizeX(void *in);
void *NormalizeY(void *in);
void *NormalizeXY(void *in);
void *NormalizeAllX(void *in);
void *NormalizeAllY(void *in);

void CoordVec(int i, Point *Vector);
void CoordNorm(int i, Dado *Param);
void GraphVec(int Xmax,int Ymax,int i,Point *Vector);
void GraphNorm(int i,Dado *Param);

int main(){
    Point *Vector;
    int i=0,j,choice,numThreads;
    struct timeval  start, end;

    FILE *arquivo;
    arquivo = fopen("Arquivo","r");

    ///Cria Vector dinamico e recebe coordenadas dos pontos
    Vector = malloc(sizeof(Point));
    ///Agora para possuir muitas entradas possui um gerador de arquivos para se fazer um teste mais observ�vel
    ///Ent�o substituir o pedido para informar por um pedido para gerar um arquivo. Talvez tamb�m implementar um aviso caso ele falhe ao abrir o arquivo.
    //printf("Informe pares de coordenadas dos pontos. Digite -1 para encerrar recebimento de pontos.\n");
    while(fscanf(arquivo,"%d",&(Vector[i].PointX)) != EOF && Vector[i].PointX != -1 && fscanf(arquivo,"%d",&(Vector[i].PointY)) != EOF && Vector[i].PointY != -1){
        i++;
        Vector = realloc(Vector, sizeof(Point)*(i+1));
    }

    /// Inicializa as fun��es de pthreads
    pthread_t tid[i+i];
    pthread_mutex_init(&m, NULL);     // pthread init pode retornar erro, fazer teste
    pthread_cond_init(&cond, NULL);
    ///
    Dado Param[i];
    for(j=0;j<i;j++){
        Param[j].PointX = Vector[j].PointX;
        Param[j].PointY = Vector[j].PointY;
    }

    ///Imprime n�mero de pontos registrados
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

    for(j=0;j<i;j++){
        Param[j].Xmax = Xmax;
        Param[j].Xmin = Xmin;
        Param[j].Ymax = Ymax;
        Param[j].Ymin = Ymin;
        //Param[j].i = i;

        //printf("Xmx=%d Xmn=%d Ymx=%d Ymn=%d\n",Param[j].Xmax,Param[j].Xmin,Param[j].Ymax,Param[j].Ymin);
    }

    ///Representa��o gr�fica dos pontos informados, em uma matriz
    //GraphVec(Xmax,Ymax,i,Vector);

    ///Pergunta qual implementa��o de Pthreads quer fazer
    printf("Escolha a implementacao PThreads que deseja executar:\n");      //Adicionar depois uma explica��o de quais as op��es e pequena explica��o sobre cada
    scanf("%d",&choice);
    switch(choice){
        case 0:
            gettimeofday(&start, NULL);

            ///Para cada ponto, cria duas threads, uma para calcular a normaliza��o de X, e outra de Y
            for(j=0;j<i;j++){
                Param[j].i = j;
                pthread_create(&(tid[j]), NULL, NormalizeX, (void*)&(Param[j]));
                pthread_create(&(tid[i+j]), NULL, NormalizeY, (void*)&(Param[j]));
            }

            for(j=0;j<i+i;j++){                 ///Aguarda o t�rmino da normaliza��o
                pthread_join(tid[j], NULL);
            }
            break;

        case 1:
            gettimeofday(&start, NULL);

            ///Para cada ponto, cria uma thread que calcula a normaliza��o de X e de Y
            for(j=0;j<i;j++){
                Param[j].i = j;
                pthread_create(&(tid[j]), NULL, NormalizeXY, (void*)&(Param[j]));
            }

            for(j=0;j<i;j++){                 ///Aguarda o t�rmino da normaliza��o
                pthread_join(tid[j], NULL);
            }
            break;

        case 2:
            gettimeofday(&start, NULL);

            ///Cria um thread pra X e um pra Y, cada uma ir� fazer um for para normaliza��o de todos os seus
            j=0;
            Param[0].i = i;
            //pthread_create(&(tid[j]), NULL, NormalizeAllX, (void*)&(Param));        //cast de void no endere�o de parametro, ou no parametro em si?
            pthread_create(&(tid[j]), NULL, NormalizeAllX, (void*)Param);
            //pthread_create(&(tid[j+1]), NULL, NormalizeAllY, (void*)&(Param));      //(void*)Param);
            pthread_create(&(tid[j+1]), NULL, NormalizeAllY, (void*)Param);

            for(j=0;j<2;j++){                 ///Aguarda o t�rmino da normaliza��o
                pthread_join(tid[j], NULL);
            }
            break;

        case 3:

            ///Define um n�mero m�ximo de threads. Dentro do for faz um teste se h� threads livres, caso n�o haja fique preso em um cond_wait. Cada thread no fim de sua execu��o enviam um cond_signal.
            // ^Esta �ltima s� funciona no caso das duas primeiras implementa��es (0 e 1)^
            ///Mover pergunta de qual implementa��o pro inicio, e fazer if para caso seja a 3 j� perguntar n�mero de threads logo ap�s
            ///Implementada em cima da primeira aplica��o, pois agora ter� um limite no overhead
            printf("Informe o numero de threads: ");
            scanf("%d",&numThreads);
            freeThreads = numThreads;

            gettimeofday(&start, NULL);
            //Usar cond_wait? cond_wait deve ser usado dentro de uma area em mutex. � necess�rio se fazer mutex?

            for(j=0;j<i;j++){
                pthread_mutex_lock(&m);
                while(freeThreads<=0){
                    pthread_cond_wait(&cond, &m);
                }
                Param[j].i = j;
                pthread_create(&(tid[j]), NULL, NormalizeX, (void*)&(Param[j]));
                freeThreads--;
                pthread_mutex_unlock(&m);
            }
            for(j=0;j<i;j++){
                pthread_mutex_lock(&m);
                while(freeThreads<=0){
                    pthread_cond_wait(&cond, &m);
                }
                pthread_create(&(tid[i+j]), NULL, NormalizeY, (void*)&(Param[j]));
                freeThreads--;
                pthread_mutex_unlock(&m);
            }

            for(j=0;j<i+i;j++){                 ///Aguarda o t�rmino da normaliza��o
                pthread_join(tid[j], NULL);
            }
            break;
    }

    gettimeofday(&end, NULL);

    ///Imprime todas coordenadas de pontos normalizadas
    //CoordNorm(i,Param);

    ///Representa��o gr�fica dos pontos normalizados, em uma matriz
    //GraphNorm(i,Param);

    ///Imprime o tempo registrado
    printf("Total time = %f seconds\n",(double)(end.tv_usec-start.tv_usec)/1000000+(double)(end.tv_sec-start.tv_sec));
    //printf("Total time: %f - %f = %f seconds\n",end.tv_usec,start.tv_usec,(double)(end.tv_usec-start.tv_usec)/1000000+(double)(end.tv_sec-start.tv_sec));
    //printf("\nTempo: %lf\n",(double)(end - start)/CLOCKS_PER_SEC);

    ///Ao fim do progarama dar free no Vector, por boas pr�ticas
    free(Vector);
    fclose(arquivo);
    return 0;
}

void *NormalizeX(void *in){
    Dado *receive = (Dado*) in;

    //Normalized[receive->i].PointX = (float)(receive->Vector[receive->i].PointX - receive->Xmin)/(float)(receive->Xmax - receive->Xmin);
    float temp1 = (receive->PointX - receive->Xmin);
    //printf("X-Xmin: %d-%d = %d\n",receive->PointX,receive->Xmin,temp1);
    float temp2 = (receive->Xmax - receive->Xmin);
    //printf("Xmax-Xmin: %d-%d = %d\n",receive->Xmax,receive->Xmin,temp2);
    receive->NormPointX = temp1/temp2;

    freeThreads++;
    pthread_cond_signal(&cond);
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

    freeThreads++;
    pthread_cond_signal(&cond);
    //printf("NormalizouY %d\n",receive->i);
    return NULL;
}

void *NormalizeXY(void *in){
    Dado *receive = (Dado*) in;

    ///Normaliza X
    float temp1 = (receive->PointX - receive->Xmin);
    float temp2 = (receive->Xmax - receive->Xmin);
    receive->NormPointX = temp1/temp2;
    ///Normaliza Y
    temp1 = (receive->PointY - receive->Ymin);
    temp2 = (receive->Ymax - receive->Ymin);
    receive->NormPointY = temp1/temp2;

    return NULL;
}

void *NormalizeAllX(void *in){
//*
    Dado *receive = (Dado*) in;
    int j;
    float temp1,temp2;
    int i = receive[0].i;
    ///Poderia chamar a fun��o Normalize X, mas para simplicidade de n�o ter que lidar com mais ponteiros para Dado, o c�lculo da normaliza��o est� feito inteiro dentro da fun��o
    for(j=0;j<i;j++){
        temp1 = (receive[j].PointX - receive[j].Xmin);
        temp2 = (receive[j].Xmax - receive[j].Xmin);
        receive[j].NormPointX = temp1/temp2;
    }
//*/
    return NULL;
}

void *NormalizeAllY(void *in){
//*
    Dado *receive = (Dado*) in;
    int j;
    float temp1,temp2;
    int i = receive[0].i;
    for(j=0;j<i;j++){
        temp1 = (receive[j].PointY - receive[j].Ymin);
        temp2 = (receive[j].Ymax - receive[j].Ymin);
        receive[j].NormPointY = temp1/temp2;
    }
//*/
    return NULL;
}

void CoordVec(int i, Point *Vector){
    int j;
    for(j=0;j<i;j++){
        printf("(%d,%d) ",Vector[j].PointX,Vector[j].PointY);
    }
    printf("\n");
}

void CoordNorm(int i, Dado *Param){
    int j;
    for(j=0;j<i;j++){
        printf("(%.2f,%.2f) ",Param[j].NormPointX,Param[j].NormPointY);
    }
    printf("\n");
}

void GraphVec(int Xmax,int Ymax,int i,Point *Vector){
    char YesNot;
    int PointFlag = 0;
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

void GraphNorm(int i,Dado *Param){
    #define res 50                  //res standing for resolution
    char YesNot;
    int PointFlag = 0;
    int j,k,l;

    printf("Digite 'Y' para ver representacao grafica: ");
    getchar();
    scanf("%c",&YesNot);
    if(YesNot == 'Y'){
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
}
