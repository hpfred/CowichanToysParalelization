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
    printf("Quantos pontos devem ser gerados?\n");
    scanf("%d",&num);
    for(i=0;i<num*2;i++){
        fprintf(arquivo,"%d",rand());
        fprintf(arquivo,"\n");
    }
    fprintf(arquivo,"-1");
    fclose(arquivo);
    printf("Gerados %d pontos\n",num);
    printf("Dado escrito para arquivo");
    return 0;
}
