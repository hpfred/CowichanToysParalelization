#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *arquivo;
    int i,j;

    arquivo = fopen("Arquivo","w");
    if(arquivo==NULL){
        printf("Falha ao abrir");
        return 1;
    }
    printf("Arquivo abriu\n");
    for(i=0;i<10000;i++){
        fprintf(arquivo,"%d",rand());
        fprintf(arquivo,"\n");
    }
    fprintf(arquivo,"-1");
    fclose(arquivo);
    printf("Dado escrito para arquivo");
    return 0;
}
