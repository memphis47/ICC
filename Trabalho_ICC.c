#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void le_parametros(int argc, char *argv[],int *erro,unsigned int *refinamento,char *arquivo_entrada,char *arquivo_saida){
    int i;
    for(i=0;i<argc;i++){//if com as condicoes do parametro
        if(argv[i]=="-e")
            *erro=*argv[i+1];
        else if(argv[i]=="-r")
            *refinamento=*argv[i+1];   
        else if(argv[i]=="-i")
            *arquivo_entrada=*argv[i+1];
        else if(argv[i]=="-o")
            *arquivo_saida=*argv[i+1];
        else if(argv[i]=="-h" && argc==2)
            printf( "Definicao dos Parametros do Trabalho de ICC :\n"
                    "-e erro:\n parametro opcional no qual 0 < erro < 1 eh o erro maximo aceitavel para a norma L^2. O valor padrao para este parametro deve ser erro=0.0001.\n"
                    "-r ref:\n parametro opcional no qual 0 <= ref < 32768 eh o numero maximo de iteracoes a serem executadas no refinamento da solucao. O valor padrao para este parametro deve ser ref=0.\n"
                    "-i arquivo_entrada:\n parametro opcional no qual arquivo_entrada eh o caminho completo para o arquivo contendo a matriz a ser invertida. Em caso de ausencia do parametro, a entrada deve ser lida de stdin.\n"
                    "-o arquivo_saida:\n parametro opcional no qual arquivo_saida eh o caminho completo para o arquivo que vai conter a matriz inversa. Em caso de ausencia do parametro, a saida deve ser impressa em stdout.\n"
                    "-h help:\n Mostra as definicoes dos argumentos\n");
    }
    


}

/**
 * le
 */
void le_matriz(FILE *arq,double **matriz){
	int i,j,tamMatriz;
	FILE *le;

	if(arq==NULL){
		le = stdin;
		fscanf(le,"%d",&tamMatriz);
	}
	else{
		le = arq;
		fscanf(le,"%d",&tamMatriz);
	}
	*matriz = malloc((sizeof(double *))*tamMatriz);
	for(i=0;i<tamMatriz;i++){
		matriz[i] = malloc((sizeof(double))*tamMatriz);
		for(j=0;j<tamMatriz;j++){
			fscanf(le,"%lf",&matriz[i][j]);
			printf("%lf\n",matriz[i][j]);
		}
	}
}


int main(int argc, char *argv[]){
    int erro;
    unsigned int refinamento;
    char *arquivo_entrada=NULL;
    char *arquivo_saida=NULL;
    double **matriz;
    matriz = malloc(sizeof(double **));
    FILE *arq=NULL;
    le_parametros(argc,argv,&erro,&refinamento,arquivo_entrada,arquivo_saida);
//    le_matriz(arq,matriz);
//    resolve_matriz();
    



}

