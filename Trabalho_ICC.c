#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int le_parametros(int argc, char *argv[],double *erro,unsigned int *refinamento,char *arquivo_entrada,char *arquivo_saida){
    int i;
    char *end;
    for(i=0;i<argc;i++){//if com as condicoes do parametro
        if(strcmp(argv[i],"-e")==0)
            *erro=strtod(argv[i+1],&end);
        else if(strcmp(argv[i],"-r")==0)
            *refinamento=atoi(argv[i+1]);   
        else if(strcmp(argv[i],"-i")==0)
            *arquivo_entrada=*argv[i+1];
        else if(strcmp(argv[i],"-o")==0)
            *arquivo_saida=*argv[i+1];
        else if((strcmp(argv[i],"-h")==0) && argc==2){
            printf( "\n\nDefinicao dos Parametros do Trabalho de ICC :\n"
                    "\n-e erro:\n\tParametro opcional no qual 0 < erro < 1 eh o erro maximo aceitavel para a norma L^2. O valor padrao para este parametro deve ser erro=0.0001.\n"
                    "\n-r ref:\n\tParametro opcional no qual 0 <= ref < 32768 eh o numero maximo de iteracoes a serem executadas no refinamento da solucao. O valor padrao para este parametro deve ser ref=0.\n"
                    "\n-i arquivo_entrada:\n\tParametro opcional no qual arquivo_entrada eh o caminho completo para o arquivo contendo a matriz a ser invertida. Em caso de ausencia do parametro, a entrada deve ser lida de stdin.\n"
                    "\n-o arquivo_saida:\n\tParametro opcional no qual arquivo_saida eh o caminho completo para o arquivo que vai conter a matriz inversa. Em caso de ausencia do parametro, a saida deve ser impressa em stdout.\n"
                    "\n-h help:\n\tMostra as definicoes dos argumentos\n\n");
        	return(0);
        }
    }
    return(1);


}

/**
 * le a matriz tanto pelo Terminal quanto por arquivo
 */
int le_matriz(FILE *arq,double **matriz){
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
	return tamMatriz;
}

void pivoteamento(double **matriz,int i){
	int novaLinha;
	
	novaLinha=procuraMaior(matriz[i][i]);
	trocaLinha(matriz,novaLinha);
	zeraColuna(matriz);
	
}

int resolve_matriz(double **matriz,int tamMatriz,int erro, unsigned int refinamento){
	int i;
	for(i=0;i<tamMatriz;i++)
		pivoteamento(matriz,i);
	
}

int main(int argc, char *argv[]){
    int saidaArq,tamMatriz;
    double erro=0.0001;
    unsigned int refinamento=0;
    char *arquivo_entrada=NULL;
    char *arquivo_saida=NULL;
    double **matriz;
    matriz = malloc(sizeof(double **));
    FILE *arq=NULL;
    saidaArq=le_parametros(argc,argv,&erro,&refinamento,arquivo_entrada,arquivo_saida);
    if(saidaArq!=0){
    	if(arquivo_entrada!=NULL){
    		arq=fopen(arquivo_entrada,"r");
    	}
    	tamMatriz=le_matriz(arq,matriz);
    	resolve_matriz(matriz,tamMatriz,erro,refinamento);
    }



}

