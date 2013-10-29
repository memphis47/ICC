#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tipo_matriz{
	double **matriz;
	int *vetorLinha;
}tipo_matriz;

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
                    "\n-i arquivo_entrada:\n\tParametro opcional no qual arquivo_entrada eh o caminho completo para o arquivo contendo a mat a ser invertida. Em caso de ausencia do parametro, a entrada deve ser lida de stdin.\n"
                    "\n-o arquivo_saida:\n\tParametro opcional no qual arquivo_saida eh o caminho completo para o arquivo que vai conter a mat inversa. Em caso de ausencia do parametro, a saida deve ser impressa em stdout.\n"
                    "\n-h help:\n\tMostra as definicoes dos argumentos\n\n");
                return(0);
        }
    }
    return(1);


}

//cria um vetor linha para o 
void criaVetorLinha(tipo_matriz* mat,int tamMatriz){
        int i;
        mat->vetorLinha = (int*)malloc (sizeof(int)*tamMatriz);
        for(i=0;i<tamMatriz;i++)
               mat->vetorLinha[i]=i;

}

void criaMatriz(tipo_matriz *mat,int tamMatriz){
	int i;
	(mat->matriz) = malloc((sizeof(double *))*tamMatriz); // aloca a mat no tamanho passado para a ordem
    criaVetorLinha(mat,tamMatriz);
    for(i=0;i<tamMatriz;i++){
        mat->matriz[i] = malloc((sizeof(double))*tamMatriz); // aloca as colunas da mat
	}
}

/**
 *  funcao para imprimir a mat
 */
void imprime_mat(tipo_matriz* mat,int tamMatriz){
        int i,j;
        for(i=0;i<tamMatriz;i++){
                for(j=0;j<tamMatriz;j++)
                        printf("\t%lf ", mat->matriz[mat->vetorLinha[i]][j]);
                printf("\n");
        }
}

void fatoracaoLU(tipo_matriz *mat,double *resultado,int tamMatriz){
	int i,j,k;
	double soma;
	double *iden;
	iden=(double*)malloc (sizeof(double)*tamMatriz);
	tipo_matriz* matrizX = (tipo_matriz*) malloc(sizeof(tipo_matriz));
	criaMatriz(matrizX,tamMatriz);
	for(i=0;i<tamMatriz;i++){
		iden[i]=0.0;
	}
	for(k=0;k<tamMatriz;k++){
		printf("k=%d\n",k);
		iden[k]=1.0;
		resultado[0]=iden[mat->vetorLinha[0]];
		for(i=0;i<tamMatriz;i++)
			printf("[%lf]\n", iden[mat->vetorLinha[i]]);

		for(i=1;i<tamMatriz;i++){
			soma=0;
			for(j=0;j<i;j++){
				soma=soma+resultado[j]*(mat->matriz[mat->vetorLinha[i]][j]);
			}
			resultado[i]=iden[mat->vetorLinha[i]]-soma;
		}

		printf("--------------\n");

		for(i=0;i<tamMatriz;i++)
			printf("Z%d:=%lf\n",i,resultado[i]);


		matrizX->matriz[tamMatriz-1][k]=resultado[i-1]/mat->matriz[mat->vetorLinha[tamMatriz-1]][tamMatriz-1];
		printf("Resultado da divisão do ultimo elemento=%lf\n\n",matrizX->matriz[tamMatriz-1][k]);
		imprime_mat(mat,tamMatriz);
		printf("\n");
		for(i=tamMatriz-2;i>=0;i--){
			soma=0;
			printf("Z[%d]= %lf\n",i,resultado[i]);
			for(j=tamMatriz-1;j>i;j--){
				soma=soma+(matrizX->matriz[j][k]*(mat->matriz[mat->vetorLinha[i]][j]));

			}
			printf("SomaUX= %lf\n\n", soma);
			matrizX->matriz[i][k]=(resultado[i]-soma)/mat->matriz[mat->vetorLinha[i]][j];
			printf("Resultado da divisão= %lf\n\n", matrizX->matriz[i][k]);
//			for(i=0;i<tamMatriz;i++){
//			printf("%lf ",matrizX->matriz[mat->vetorLinha[i]][j]);
//
//			}
			printf("\n");
		}

		printf("Fatoração LU para k=%d\n",k);
		printf("\n");
		iden[k]=0;
	}
	printf("\n");
	imprime_mat(matrizX,tamMatriz);
	printf("\n");
	
}


/**
 * le a mat usando arq como parametro para saber se existe um arquivo para ler
 * ou se sera usado o terminal para a leitura da mat
 */
int le_mat(FILE *arq,tipo_matriz* mat){
        int i,j,tamMatriz;
        FILE *le;
        if(arq==NULL){ // caso não exista o arquivo de entrada, a ordem da mat sera lida pelo terminal
                le = stdin;
                fscanf(le,"%d",&tamMatriz);
        }
        else{ // caso exista o arquivo de entrada, a ordem da mat sera lido da primeira linha do arquivo
                le = arq;
                fscanf(le,"%d",&tamMatriz);
        }
        criaMatriz(mat,tamMatriz);
        for(i=0;i<tamMatriz;i++){
                for(j=0;j<tamMatriz;j++){
                        fscanf(le,"%lf",&mat->matriz[i][j]); // le tanto do arquivo quanto do terminal a mat
                }
        }
        return tamMatriz;
}


/**
 * procura o maior elemento pertencente a coluna i nas linhas abaixo da linha atual
 */
int procuraMaior(tipo_matriz* mat,int i, int tamMatriz){
        int lin,maiorLin=-1;
        double maiorAux=mat->matriz[mat->vetorLinha[i]][i];// o primeiro maior elemento sera o da linha atual para efeito de comparações
        for(lin=i+1;lin<tamMatriz;lin++){
                if(mat->matriz[mat->vetorLinha[lin]][i]>maiorAux){ 
                        maiorAux=mat->matriz[mat->vetorLinha[lin]][i];
                        maiorLin=lin;
                }
        }
        return maiorLin; // retorna a linha que esta esse elemento
}

/**
 *  troca as linhas da mat, ou seja troca de posição os elementos entre as duas linhas
 */
void trocaLinhas (tipo_matriz *mat, int linhaOri, int linhaDest, int tamMatriz){
        int aux;
        aux=mat->vetorLinha[linhaOri];
        mat->vetorLinha[linhaOri]=mat->vetorLinha[linhaDest];
        mat->vetorLinha[linhaDest]=aux;
}




/**
 *  Zera a coluna da das linhas abaixo da linha atual passada no parametro i
 */
void zeraColuna(tipo_matriz* mat,int linZerada,int i,int tamMatriz){
        int col;
        double pivo;
        pivo=mat->matriz[mat->vetorLinha[linZerada]][i]/mat->matriz[mat->vetorLinha[i]][i]; // acha o pivo da mat, ous eja o termo que sera usado para zerar a linha abaixo da linha atual
        mat->matriz[mat->vetorLinha[linZerada]][i]=pivo; //para evitar que a criação de uma nova mat esse pivo sera guardado no lugar do elemento que seria zerado
        for(col=i+1;col<tamMatriz;col++){
                mat->matriz[mat->vetorLinha[linZerada]][col]=mat->matriz[mat->vetorLinha[linZerada]][col]-(pivo*mat->matriz[mat->vetorLinha[i]][col]); // usa o pivo para ajustar a linha que esta sendo modificada
        }
}

/**
 * chama a funcao para achar o maior elemento das colunas, caso exista esse elemento
 * faz a troca de linha entre os dois, se nao deixa as linhas como estao
 */
void pivoteamento(tipo_matriz* mat,int i, int tamMatriz){
        int novaLinha;
        int lin;
        novaLinha=procuraMaior(mat,i,tamMatriz); // procura o maior elemento dentro da coluna
        if(novaLinha>=0)
                trocaLinhas(mat,i,novaLinha,tamMatriz); // caso exista esse elemento troca a linha atual com a linha do novo elemento

        for(lin=i+1;lin<tamMatriz;lin++)
                zeraColuna(mat,lin,i,tamMatriz); // zera a coluna abaixo da linha atual, seguindo o metodo de Gauss
        
        
}



void copiaMatriz(tipo_matriz *mat1,tipo_matriz *mat2,int tamMatriz){
	int i,j;
	for(i=0;i<tamMatriz;i++){
		for(j=0;j<tamMatriz;j++){
			mat2->matriz[i][j]=mat1->matriz[i][j];
		}
	}
	
	
}

/**
 * Chama as funcoes necessarias para resolver a mat por gauss
 * 
 */
int resolve_mat(tipo_matriz* mat,int tamMatriz,int erro, unsigned int refinamento){
        int i;
        int vetorLinha[tamMatriz];
        for(i=0;i<tamMatriz-1;i++){
                imprime_mat(mat,tamMatriz);
                printf("\n");
                pivoteamento(mat,i,tamMatriz); // pivoteia as linhas da mat para zerar as colunas
                imprime_mat(mat,tamMatriz);
                printf("\n");
        }
}

int main(int argc, char *argv[]){
    int saidaArq,tamMatriz,i,j;
    double erro=0.0001; // valor padrão definido pelo professor
    unsigned int refinamento=0; // valor padrão definido pelo professor
    char *arquivo_entrada=NULL;
    char *arquivo_saida=NULL;
    tipo_matriz* matrizLU = (tipo_matriz*) malloc(sizeof(tipo_matriz));// aloca a mat
    tipo_matriz* matrizA = (tipo_matriz*) malloc(sizeof(tipo_matriz));// aloca a mat
    double *vetorDahora = (double*)malloc (sizeof(double)*tamMatriz);
    FILE *arq=NULL;
    saidaArq=le_parametros(argc,argv,&erro,&refinamento,arquivo_entrada,arquivo_saida); // verfica os valores passados por parametro
    if(saidaArq!=0){
            if(arquivo_entrada!=NULL){
                    arq=fopen(arquivo_entrada,"r"); // caso exista arquivo de entrada , abre ele para ler a mat
            }
            tamMatriz=le_mat(arq,matrizA); //le a mat, seja pelo terminal , ou por um arquivo texto
            
            criaMatriz (matrizLU,tamMatriz);
            copiaMatriz(matrizA, matrizLU,tamMatriz);
			printf("\nImprimindo Matriz LU:\n");
            imprime_mat(matrizLU,tamMatriz);
            printf("\n");
            resolve_mat(matrizLU,tamMatriz,erro,refinamento); // resolve a mat por gauss
            fatoracaoLU(matrizLU,vetorDahora,tamMatriz);
            printf("\nImprimindo Matriz A:\n");
            imprime_mat(matrizA,tamMatriz);
    }

}
