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
		}
	}
	return tamMatriz;
}

int procuraMaior(double **matriz,int i, int tamMatriz){
	int lin,maiorLin=-1;
	double maiorAux=matriz[i][i];
	for(lin=i+1;lin<tamMatriz;lin++){
		if(matriz[lin][i]>maiorAux){
			maiorAux=matriz[lin][i];
			maiorLin=lin;
		}
	}
	return maiorLin;
}

void trocaLinhas (double **matriz, int linhaOri, int linhaDest, int tamMatriz){
	double aux;
	int i;
	for(i=0;i<tamMatriz;i++){
		aux = matriz[linhaDest][i];
		matriz[linhaDest][i] = matriz[linhaOri][i];
		matriz[linhaOri][i] = aux;
	}
}

void imprime_matriz(double **matriz,int tamMatriz){
	int i,j;
	for(i=0;i<tamMatriz;i++){
		for(j=0;j<tamMatriz;j++)
			printf("\t%lf ", matriz[i][j]);
		printf("\n");
	}
}

void zeraColuna(double **matriz,int linZerada,int i,int tamMatriz){
	int col;
	double pivo;
	pivo=matriz[linZerada][i]/matriz[i][i];
	matriz[linZerada][i]=pivo;
	for(col=i+1;col<tamMatriz;col++){
		matriz[linZerada][col]=matriz[linZerada][col]-(pivo*matriz[i][col]);
	}
}

void pivoteamento(double **matriz,int i, int tamMatriz){
	int novaLinha;
	int lin;
	novaLinha=procuraMaior(matriz,i,tamMatriz);
	if(novaLinha>=0)
		trocaLinhas(matriz,i,novaLinha,tamMatriz);
	imprime_matriz(matriz,tamMatriz);
	printf("\n");
	for(lin=i+1;lin<tamMatriz;lin++)
		zeraColuna(matriz,lin,i,tamMatriz);
	imprime_matriz(matriz,tamMatriz);
	printf("\n");
	
}

int resolve_matriz(double **matriz,int tamMatriz,int erro, unsigned int refinamento){
	int i;
	imprime_matriz(matriz,tamMatriz);
	printf("\n");
	for(i=0;i<tamMatriz;i++){
		pivoteamento(matriz,i,tamMatriz);
		imprime_matriz(matriz,tamMatriz);
		printf("\n");
	}
	imprime_matriz(matriz,tamMatriz);
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

