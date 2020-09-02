#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *arquivo;
    int i,num=40000;

    arquivo = fopen("Arquivo","w");
    if(arquivo==NULL){
        printf("Falha ao abrir");
        return 1;
    }
    printf("Arquivo abriu\n");
    for(i=0;i<num;i++){
        fprintf(arquivo,"%d",rand());
        fprintf(arquivo,"\n");
    }
    fprintf(arquivo,"-1");
    fclose(arquivo);
    printf("Gerados %d pontos\n",num/2);
    printf("Dado escrito para arquivo");
    return 0;
}
