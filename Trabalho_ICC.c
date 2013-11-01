#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct tipo_matriz{
	double **matriz;
	long int *vetorLinha;

}tipo_matriz;

long int le_parametros(long int argc, char *argv[],long double *erro,unsigned long int *refinamento,char **arquivo_entrada,char **arquivo_saida){
    long int i;
    char *end;
    for(i=1;i<argc;i++){//if com as condicoes do parametro
        if(strcmp(argv[i],"-e")==0)
            *erro=strtod(argv[i+1],&end); //recebe o valor do erro para o cálculo
        else if(strcmp(argv[i],"-r")==0)
            *refinamento=atoi(argv[i+1]); //recebe o valor do refinamento para o cálculo
        else if(strcmp(argv[i],"-i")==0){
        	while(*arquivo_entrada==NULL){
        		*arquivo_entrada = malloc (sizeof(char)*strlen(argv[i+1])+1);
        	}
        	strcpy(*arquivo_entrada,argv[i+1]);
        }
        else if(strcmp(argv[i],"-o")==0){
        	while(*arquivo_saida==NULL){
        		*arquivo_saida = malloc (sizeof(char)*strlen(argv[i+1])+1);
        	}
        	strcpy(*arquivo_entrada,argv[i+1]); //recebe o caminho para o arquivo de saida
        }
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
void criaVetorLinha(tipo_matriz* mat,long int tamMatriz){
        long int i;
        mat->vetorLinha = (long int*)malloc (sizeof(long int)*tamMatriz);
        for(i=0;i<tamMatriz;i++)
               mat->vetorLinha[i]=i;

}

void criaMatriz(tipo_matriz *mat,long int tamMatriz){
	long int i;
	(mat->matriz) = malloc((sizeof(double *))*tamMatriz); // aloca a mat no tamanho passado para a ordem
    criaVetorLinha(mat,tamMatriz);
    for(i=0;i<tamMatriz;i++){
        mat->matriz[i] = malloc((sizeof(double))*tamMatriz); // aloca as colunas da mat
	}
}

void multiplicaMatriz(tipo_matriz* matA,tipo_matriz* matB,tipo_matriz* matR,long int tamMatriz){
	long int i,j,k;
	for(i=0;i<tamMatriz;i++){
		for(j=0;j<tamMatriz;j++){
			for(k=0;k<tamMatriz;k++){
				matR->matriz[i][j]+=(matA->matriz[i][k]*matB->matriz[k][j]);
			}
		}
	}
}

/**
 *  funcao para imprimir a mat
 */
void imprime_mat(tipo_matriz* mat,long int tamMatriz){
	long int i,j;
	for(i=0;i<tamMatriz;i++){
		for(j=0;j<tamMatriz;j++)
			printf("%.20lf ", mat->matriz[mat->vetorLinha[i]][j]);
		printf("\n");
	}
}

void criaIdentidade(tipo_matriz *mat,long int tamMatriz){
	long int i,j;

	(mat->matriz) = malloc((sizeof(double *))*tamMatriz); // aloca a mat no tamanho passado para a ordem
	    criaVetorLinha(mat,tamMatriz);
	    for(i=0;i<tamMatriz;i++){
	    	mat->matriz[i] = malloc((sizeof(double))*tamMatriz); // aloca as colunas da mat
			for(j=0;j<tamMatriz;j++)
				if(i == j)
					mat->matriz[i][j]=1.0;
				else
					mat->matriz[i][j]=0.0;
	}
}



int verificaLinha(tipo_matriz *mat,long int tamMatriz){
	long int i,j,cont;
	for(j=0;j<tamMatriz;j++){
		for(i=0;i<tamMatriz;i++){
			if(mat->matriz[i][j]==0)
				cont++;
		}
		if(cont==tamMatriz-1){
			fprintf(stderr,"Linha Zerada !!!\n");
			exit(1);
		}
	}
	return 1;
}

int verificaColuna(tipo_matriz *mat,long int tamMatriz){
	long int i,j,cont;
	for(i=0;j<tamMatriz;j++){
		for(j=0;i<tamMatriz;i++){
			if(mat->matriz[i][j]==0)
				cont++;
		}
		if(cont==tamMatriz-1){
			fprintf(stderr,"Coluna Zerada !!!\n");
			exit(1);
		}
	}
	return 1;
}



void calculaZ(tipo_matriz *mat, tipo_matriz *matrizZ,tipo_matriz *matrizB,long int tamMatriz,long int k){
	long int i,j;
	double soma;
	// calcula L
	matrizZ->matriz[0][k]=matrizB->matriz[mat->vetorLinha[0]][k];
	for(i=1;i<tamMatriz;i++){
		soma=0;
		for(j=0;j<i;j++){
			soma=soma+matrizZ->matriz[j][k]*(mat->matriz[mat->vetorLinha[i]][j]);
		}
		matrizZ->matriz[i][k]=matrizB->matriz[mat->vetorLinha[i]][k]-soma;
	}
}

void calculaX(tipo_matriz *mat, tipo_matriz *matrizX,tipo_matriz *matrizZ,long int tamMatriz,long int k){
	long int i,j;
	double soma;
	//Calcula U
	matrizX->matriz[tamMatriz-1][k]=matrizZ->matriz[i-1][k]/mat->matriz[mat->vetorLinha[tamMatriz-1]][tamMatriz-1];
	for(i=tamMatriz-2;i>=0;i--){
		soma=0;
		for(j=tamMatriz-1;j>i;j--){
			soma=soma+(matrizX->matriz[j][k]*(mat->matriz[mat->vetorLinha[i]][j]));
		}
		matrizX->matriz[i][k]=(matrizZ->matriz[i][k]-soma)/mat->matriz[mat->vetorLinha[i]][j];
	}
}

void calculaResiduo(tipo_matriz *matrizA, tipo_matriz *matrizX,tipo_matriz *matrizResiduo,tipo_matriz *matrizZ,long int tamMatriz){
	long int i,j,k;
	double soma;
	tipo_matriz* matrizAxi = (tipo_matriz*) malloc(sizeof(tipo_matriz));
	criaMatriz(matrizAxi,tamMatriz);
	//Calcula Residuo
	for(k=0;k<tamMatriz;k++){
		for(i=0;i<tamMatriz;i++){
			soma=0.0;
			for(j=0;j<tamMatriz;j++){
				soma+=matrizA->matriz[i][j]*matrizX->matriz[j][k];
			}
			matrizAxi->matriz[i][k]=soma;
		}
		for(i=0;i<tamMatriz;i++){
			matrizResiduo->matriz[i][k]=matrizZ->matriz[i][k]-matrizAxi->matriz[i][k];
		}
	}
}

void calculaNorma(tipo_matriz *mat,long int tamMatriz,long double erro,double *norma){
	long int i,j;
	double soma;
	for(i=0;i<tamMatriz;i++){
		soma=0.0;
		for(j=0;j<tamMatriz;j++){
			soma+=mat->matriz[j][i]*mat->matriz[j][i];
		}
		norma[i]=sqrt(soma);
	}
	
}




int resolveRefinado(tipo_matriz *matR,tipo_matriz *matLU,tipo_matriz *matX,long int tamMatriz,double *norma,long double erro,long int *numRef){
	//guardar coluna R*L nela mesma
	long int i,j,k,l,cont;
	tipo_matriz* matW = (tipo_matriz*) malloc(sizeof(tipo_matriz));
	criaMatriz(matW,tamMatriz);
	cont=0;
	for(j=0;j<tamMatriz;j++){
		if(norma[j]>erro){
			calculaZ(matLU,matR,matR,tamMatriz,j);
			calculaX(matLU,matW,matR,tamMatriz,j);
			for(l=0;l<tamMatriz;l++)
				matX->matriz[l][j]+=matW->matriz[l][j];
			numRef[j]++;
		}
		else{
			cont++;
		}
	}
	if(cont==tamMatriz-1)
		return 0;
	else
		return 1;
	
}

int refinar(tipo_matriz *matA,tipo_matriz *matX,tipo_matriz *matLU,long double erro,long int tamMatriz,double *norma,long int *numRef){
	long int i,j,k;
	double soma;
	int res;
	

	tipo_matriz* matrizAxi = (tipo_matriz*) malloc(sizeof(tipo_matriz));
	tipo_matriz* matrizResiduo = (tipo_matriz*) malloc(sizeof(tipo_matriz));
	tipo_matriz* matrizId = (tipo_matriz*) malloc(sizeof(tipo_matriz));

	criaMatriz(matrizResiduo,tamMatriz);
	criaMatriz(matrizAxi,tamMatriz);

	criaIdentidade(matrizId,tamMatriz);
	calculaResiduo(matA, matX,matrizResiduo, matrizId,tamMatriz);
	calculaNorma(matrizResiduo,tamMatriz,erro,norma);
	
	res=resolveRefinado(matrizResiduo,matLU,matX,tamMatriz,norma,erro,numRef);
	if(res==0)
		return 0;
	else
		return 1;
}	

void fatoracaoLU(tipo_matriz *mat,tipo_matriz *matrizX,long int tamMatriz){
	long int i,j,k;
	double soma;
	tipo_matriz* matrizId = (tipo_matriz*) malloc(sizeof(tipo_matriz));
	tipo_matriz* matrizZ = (tipo_matriz*) malloc(sizeof(tipo_matriz));

	criaMatriz (matrizZ,tamMatriz);
	criaIdentidade(matrizId,tamMatriz);

	for(k=0;k<tamMatriz;k++){
		// calcula L
		calculaZ(mat,matrizZ,matrizId,tamMatriz,k);

		//Calcula U
		calculaX(mat,matrizX,matrizZ,tamMatriz,k);
	}

	
}


/**
 * le a mat usando arq como parametro para saber se existe um arquivo para ler
 * ou se sera usado o terminal para a leitura da mat
 */
long int le_mat(FILE *arq,tipo_matriz* mat){
	long int i,j,tamMatriz;
	FILE *le;
	if(arq==NULL){ // caso não exista o arquivo de entrada, a ordem da mat sera lida pelo terminal
			le = stdin;
			fscanf(le,"%ld",&tamMatriz);
	}
	else{ // caso exista o arquivo de entrada, a ordem da mat sera lido da primeira linha do arquivo
			le = arq;
			fscanf(le,"%ld",&tamMatriz);
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
long int procuraMaior(tipo_matriz* mat,long int i, long int tamMatriz){
        long int lin,maiorLin=-1;
        double maiorAux=mat->matriz[mat->vetorLinha[i]][i];// o primeiro maior elemento sera o da linha atual para efeito de comparações
        for(lin=i+1;lin<tamMatriz;lin++){
                if(mat->matriz[mat->vetorLinha[lin]][i]>maiorAux){
                	if(mat->matriz[mat->vetorLinha[lin]][i]==0){
                		fprintf(stderr,"Pivo igual a Zero !!!\n");
                		exit(1);
                	}
                	else{
						maiorAux=mat->matriz[mat->vetorLinha[lin]][i];
						maiorLin=lin;
                	}
                }
        }
        return maiorLin; // retorna a linha que esta esse elemento
}

/**
 *  troca as linhas da mat, ou seja troca de posição os elementos entre as duas linhas
 */
void trocaLinhas (tipo_matriz *mat, long int linhaOri, long int linhaDest, long int tamMatriz){
        long int aux;
        aux=mat->vetorLinha[linhaOri];
        mat->vetorLinha[linhaOri]=mat->vetorLinha[linhaDest];
        mat->vetorLinha[linhaDest]=aux;
}

/**
 *  Zera a coluna da das linhas abaixo da linha atual passada no parametro i
 */
void zeraColuna(tipo_matriz* mat,long int linZerada,long int i,long int tamMatriz){
        long int col;
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
void pivoteamento(tipo_matriz* mat,long int i, long int tamMatriz){
	long int novaLinha;
	long int lin;
	novaLinha=procuraMaior(mat,i,tamMatriz); // procura o maior elemento dentro da coluna
	if(novaLinha>=0)
		trocaLinhas(mat,i,novaLinha,tamMatriz); // caso exista esse elemento troca a linha atual com a linha do novo elemento

	for(lin=i+1;lin<tamMatriz;lin++)
		zeraColuna(mat,lin,i,tamMatriz); // zera a coluna abaixo da linha atual, seguindo o metodo de Gauss

        
}

void copiaMatriz(tipo_matriz *mat1,tipo_matriz *mat2,long int tamMatriz){
	long int i,j;
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
long int resolve_mat(tipo_matriz* mat,long int tamMatriz){
        long int i;
        long int vetorLinha[tamMatriz];
        if(verificaLinha(mat,tamMatriz)==1 && verificaColuna(mat,tamMatriz)==1){
			for(i=0;i<tamMatriz-1;i++){
					pivoteamento(mat,i,tamMatriz); // pivoteia as linhas da mat para zerar as colunas
			}
        }
}

void imprimeResultado(double *norma,long int *numRef,long int tamMatriz){
	long int i;
	printf("\n#\n# Norma L2: ");
	for(i=0;i<tamMatriz;i++){
		printf("%.16lf ",norma[i]);
	}
	printf("\n# NumRefinamento: ");
	for(i=0;i<tamMatriz;i++){
			printf("%ld ",numRef[i]);
	}
	printf("\n#\n%ld\n",tamMatriz);

}


long int main(long int argc, char *argv[]){
    long int saidaArq,tamMatriz,i,j,nRef;
    int semRefi=1;
    long double erro=0.0001; // valor padrão definido pelo professor
    double *norma;
    long int *numRef;
    unsigned long int refinamento=0; // valor padrão definido pelo professor
    char *arquivo_entrada=NULL;
    char *arquivo_saida=NULL;
    tipo_matriz* matrizLU = (tipo_matriz*) malloc(sizeof(tipo_matriz));// aloca a matriz LU
    tipo_matriz* matrizA = (tipo_matriz*) malloc(sizeof(tipo_matriz));// aloca a matriz A
    tipo_matriz* matrizX = (tipo_matriz*) malloc(sizeof(tipo_matriz));// aloca a matriz X
    tipo_matriz* matrizR = (tipo_matriz*) malloc(sizeof(tipo_matriz));// aloca a matriz R
    FILE *arq=NULL;
    saidaArq=le_parametros(argc,argv,&erro,&refinamento,&arquivo_entrada,&arquivo_saida); // verfica os valores passados por parametro
    nRef=0;
    if(saidaArq!=0){
            if(arquivo_entrada!=NULL){
                    arq=fopen(arquivo_entrada,"r"); // caso exista arquivo de entrada , abre ele para ler a mat
            }
            tamMatriz=le_mat(arq,matrizA); //le a mat, seja pelo terminal , ou por um arquivo texto
            norma=(double *) malloc(sizeof(double)*tamMatriz);
            numRef=(long int *) malloc(sizeof(long int)*tamMatriz);
            criaMatriz (matrizLU,tamMatriz);
            copiaMatriz(matrizA, matrizLU,tamMatriz);
            criaMatriz (matrizX,tamMatriz);
            resolve_mat(matrizLU,tamMatriz); // resolve a matriz por gauss
            fatoracaoLU(matrizLU,matrizX,tamMatriz);
            imprime_mat(matrizX,tamMatriz);
            printf("\n");
            criaMatriz (matrizR,tamMatriz);
            multiplicaMatriz(matrizA,matrizX,matrizR,tamMatriz);
            imprime_mat(matrizR,tamMatriz);
            printf("\n");
            while(nRef<refinamento){

            	semRefi=refinar(matrizA,matrizX,matrizLU,erro,tamMatriz,norma,numRef);
            	if(semRefi==0)
            		break;

            	nRef++;
            }
            imprimeResultado(norma,numRef,tamMatriz);
            imprime_mat(matrizX,tamMatriz);
            printf("\n");
            criaMatriz (matrizR,tamMatriz);
            multiplicaMatriz(matrizA,matrizX,matrizR,tamMatriz);
            imprime_mat(matrizR,tamMatriz);
            printf("\n");
    }

}
