#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int le_parametros(int argc, char *argv[],double *erro,unsigned int *refinamento,char *arquivo_entrada,char *arquivo_saida){
    int i;
    char *end;
    for(i=0;i<argc;i++){//if com as condicoes do parametro
        if(strcmp(argv[i],"-e")==0) 
            *erro=strtod(argv[i+1],&end); //recebe o valor do erro para o cálculo
        else if(strcmp(argv[i],"-r")==0)
            *refinamento=atoi(argv[i+1]); //recebe o valor do refinamento para o cálculo  
        else if(strcmp(argv[i],"-i")==0)
            *arquivo_entrada=*argv[i+1]; //recebe o caminho para o arquivo de entrada
        else if(strcmp(argv[i],"-o")==0)
            *arquivo_saida=*argv[i+1]; //recebe o caminho para o arquivo de saida
        else if((strcmp(argv[i],"-h")==0) || argc==2){ //imprime na tela uma explicação dos parametros, caso o usuario tenha digitado o parametro errado ou queira saber atraves do parametro -h quais são os parametros
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
 * le a matriz usando arq como parametro para saber se existe um arquivo para ler
 * ou se sera usado o terminal para a leitura da matriz
 */
int le_matriz(FILE *arq,double **matriz){
	int i,j,tamMatriz;
	FILE *le;

	if(arq==NULL){ // caso não exista o arquivo de entrada, a ordem da matriz sera lida pelo terminal
		le = stdin;
		fscanf(le,"%d",&tamMatriz);
	}
	else{ // caso exista o arquivo de entrada, a ordem da matriz sera lido da primeira linha do arquivo
		le = arq;
		fscanf(le,"%d",&tamMatriz);
	}
	*matriz = malloc((sizeof(double *))*tamMatriz); // aloca a matriz no tamanho passado para a ordem
	for(i=0;i<tamMatriz;i++){
		matriz[i] = malloc((sizeof(double))*tamMatriz); // aloca as colunas da matriz
		for(j=0;j<tamMatriz;j++){
			fscanf(le,"%lf",&matriz[i][j]); // le tanto do arquivo quanto do terminal a matriz
		}
	}
	return tamMatriz;
}


/**
 * procura o maior elemento pertencente a coluna i nas linhas abaixo da linha atual
 */
int procuraMaior(double **matriz,int vetorLinha[],int i, int tamMatriz){
	int lin,maiorLin=-1;
	double maiorAux=matriz[vetorLinha[i]][i];// o primeiro maior elemento sera o da linha atual para efeito de comparações
	for(lin=i+1;lin<tamMatriz;lin++){
		if(matriz[vetorLinha[lin]][i]>maiorAux){ 
			maiorAux=matriz[vetorLinha[lin]][i];
			maiorLin=lin;
		}
	}
	return maiorLin; // retorna a linha que esta esse elemento
}

/**
 *  troca as linhas da matriz, ou seja troca de posição os elementos entre as duas linhas
 */
void trocaLinhas (int *vetorLinha, int linhaOri, int linhaDest, int tamMatriz){
	int aux;
	aux=vetorLinha[linhaOri];
	vetorLinha[linhaOri]=vetorLinha[linhaDest];
	vetorLinha[linhaDest]=aux;
}

/**
 *  funcao para imprimir a matriz
 */
void imprime_matriz(double **matriz,int vetorLinha[],int tamMatriz){
	int i,j;
	for(i=0;i<tamMatriz;i++){
		for(j=0;j<tamMatriz;j++)
			printf("\t%lf ", matriz[vetorLinha[i]][j]);
		printf("\n");
	}
}


/**
 *  Zera a coluna da das linhas abaixo da linha atual passada no parametro i
 */
void zeraColuna(double **matriz,int vetorLinha[],int linZerada,int i,int tamMatriz){
	int col;
	double pivo;
	pivo=matriz[vetorLinha[linZerada]][i]/matriz[vetorLinha[i]][i]; // acha o pivo da matriz, ous eja o termo que sera usado para zerar a linha abaixo da linha atual
	matriz[vetorLinha[linZerada]][i]=pivo; //para evitar que a criação de uma nova matriz esse pivo sera guardado no lugar do elemento que seria zerado
	for(col=i+1;col<tamMatriz;col++){
		matriz[vetorLinha[linZerada]][col]=matriz[vetorLinha[linZerada]][col]-(pivo*matriz[vetorLinha[i]][col]); // usa o pivo para ajustar a linha que esta sendo modificada
	}
}

/**
 * chama a funcao para achar o maior elemento das colunas, caso exista esse elemento
 * faz a troca de linha entre os dois, se nao deixa as linhas como estao
 */
void pivoteamento(double **matriz,int vetorLinha[],int i, int tamMatriz){
	int novaLinha;
	int lin;
	novaLinha=procuraMaior(matriz,vetorLinha,i,tamMatriz); // procura o maior elemento dentro da coluna
	if(novaLinha>=0)
		trocaLinhas(vetorLinha,i,novaLinha,tamMatriz); // caso exista esse elemento troca a linha atual com a linha do novo elemento
	
	for(lin=i+1;lin<tamMatriz;lin++)
		zeraColuna(matriz,vetorLinha,lin,i,tamMatriz); // zera a coluna abaixo da linha atual, seguindo o metodo de Gauss
	
	
}


//cria um vetor linha para o 
void criaVetorLinha(int *vetorLinha,int tamMatriz){
	int i;
	for(i=0;i<tamMatriz;i++)
		vetorLinha[i]=i;
}

/**
 * Chama as funcoes necessarias para resolver a matriz por gauss
 * 
 */
int resolve_matriz(double **matriz,int tamMatriz,int erro, unsigned int refinamento){
	int i;
	int vetorLinha[tamMatriz];
	criaVetorLinha(vetorLinha,tamMatriz);
	imprime_matriz(matriz,vetorLinha,tamMatriz);
	printf("\n");
	for(i=0;i<tamMatriz-1;i++){
		imprime_matriz(matriz,vetorLinha,tamMatriz);
		printf("\n");
		pivoteamento(matriz,vetorLinha,i,tamMatriz); // pivoteia as linhas da matriz para zerar as colunas
		imprime_matriz(matriz,vetorLinha,tamMatriz);
		printf("\n");
	}
	imprime_matriz(matriz,vetorLinha,tamMatriz);
}

int main(int argc, char *argv[]){
    int saidaArq,tamMatriz;
    double erro=0.0001; // valor padrão definido pelo professor
    unsigned int refinamento=0; // valor padrão definido pelo professor
    char *arquivo_entrada=NULL;
    char *arquivo_saida=NULL;
    double **matriz;
    matriz = malloc(sizeof(double **));// aloca a matriz
    FILE *arq=NULL;
    saidaArq=le_parametros(argc,argv,&erro,&refinamento,arquivo_entrada,arquivo_saida); // verfica os valores passados por parametro
    if(saidaArq!=0){
    	if(arquivo_entrada!=NULL){
    		arq=fopen(arquivo_entrada,"r"); // caso exista arquivo de entrada , abre ele para ler a matriz
    	}
    	tamMatriz=le_matriz(arq,matriz); //le a matriz, seja pelo terminal , ou por um arquivo texto
    	resolve_matriz(matriz,tamMatriz,erro,refinamento); // resolve a matriz por gauss
    }



}

