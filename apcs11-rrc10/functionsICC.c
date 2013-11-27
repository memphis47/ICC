#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functionsICC.h"

Dados *VetorDados;
int contador=0;
int GCcontrol=0;

double timestamp(void){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return((double)(tp.tv_sec + tp.tv_usec/1000000.0));
}

void *mialloc (size_t size)
{
	void *ptr;
	if (contador)
	{
		int i;
		for (i=0; i<contador; i++)
		{
			if ( (VetorDados[i].flag ==0) && (size <= VetorDados[i].size) )
			{
				VetorDados[i].flag = 1;
				return VetorDados[i].pointer;
			}
		}
		VetorDados = (Dados *) realloc (VetorDados, sizeof(Dados)*(contador+1) );
		ptr = malloc (size);
		VetorDados[i].pointer = ptr;
		VetorDados[i].size = size;
		VetorDados[i].flag = 1;
		contador++;
		GCcontrol++;
		if ( GCcontrol==5 )
			GarbageCollector();
		return ptr;
	}
	VetorDados = (Dados *) malloc (sizeof(Dados));
	ptr = malloc (size);
	VetorDados->pointer = ptr;
	VetorDados->size = size;
	VetorDados->flag = 1;
	contador++;
	GCcontrol++;
	if ( GCcontrol==5 )
		GarbageCollector();
	return ptr;
}

void cipurge ()
{
	int i;
	for (i=0; i<contador; i++)
		free (VetorDados[i].pointer);
	free (VetorDados);
	contador=0;
}

void GarbageCollector ()
{
	int i,j;
	for (i=0; i<contador; i++)
		if (VetorDados[i].flag==0)
		{
			free (VetorDados[i].pointer);
			for (j=i; j<contador-1; j++)
				VetorDados[j]=VetorDados[j+1];
			i--;
			contador--;
		}
	VetorDados = (Dados *) realloc ( VetorDados, sizeof(Dados)*(contador) );
	GCcontrol=0;
}

/**
 * le os parametros passados e coloca eles nas variaveis correspondentes
 */

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
        		*arquivo_entrada = mialloc (sizeof(char)*strlen(argv[i+1])+1);
        	}
        	strcpy(*arquivo_entrada,argv[i+1]); // recebe o caminho do arquivo com a matriz de entrada
        }
        else if(strcmp(argv[i],"-o")==0){
        	while(*arquivo_saida==NULL){
        		*arquivo_saida = mialloc (sizeof(char)*strlen(argv[i+1])+1);
        	}
        	strcpy(*arquivo_saida,argv[i+1]); //recebe o caminho para o arquivo de saida
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

/**
 * cria o vetor linha para a matriz, esse vetor sera usado no pivoteamento da matriz
 */
void criaVetorLinha(tipo_matriz* mat,long int tamMatriz){
        long int i;
        mat->vetorLinha = (long int*)mialloc (sizeof(long int)*tamMatriz);
        for(i=0;i<tamMatriz;i++)
               mat->vetorLinha[i]=i; /* cada elemento do vetor sera correspondente 
        							 a um indice que vai de 0 a tamMatriz-1 e que representa uma linha da matriz */

}


/**
 * aloca a matriz e o chama o alocador do vetor, ambos elementos da Struct tipo_matriz
 */
void criaMatriz(tipo_matriz *mat,long int tamMatriz,int tipo){
	long int i,j;
	if(tipo==2)
		criaVetorLinha(mat,tamMatriz);
	(mat->matriz) = mialloc((sizeof(double *))*tamMatriz); // aloca a mat no tamanho passado para a ordem
    for(i=0;i<tamMatriz;i++){
        mat->matriz[i] = mialloc((sizeof(double))*tamMatriz); // aloca as colunas da matriz
        for(j=0;j<tamMatriz;j++){
        	mat->matriz[i][j]=0.0;
        }
	}
}

/**
 * Funcao chamada para multiplicar duas matrizes A e B e devolver o resultado em uma Matriz R
 */
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
 *  Funcao para imprimir a matriz passada
 */
void imprime_mat(FILE *arq,tipo_matriz* mat,long int tamMatriz){
	long int i,j;
	FILE *le;
	if(arq==NULL)
		le=stdout;
	else
		le=arq;
	for(i=0;i<tamMatriz;i++){
		for(j=0;j<tamMatriz;j++){
			   fprintf(le,"%.15lf ", mat->matriz[mat->vetorLinha[i]][j]);
		}
		fprintf(le,"\n");
	}
}

/**
 * Funcao para criar a matriz identidade
 */
void criaIdentidade(tipo_matriz *mat,long int tamMatriz){
	long int i,j;
	(mat->matriz) = mialloc((sizeof(double *))*tamMatriz); // aloca a mat no tamanho passado para a ordem
	for(i=0;i<tamMatriz;i++){
		mat->matriz[i] = mialloc((sizeof(double))*tamMatriz); // aloca as colunas da mat
		for(j=0;j<tamMatriz;j++){
				mat->matriz[i][j]=0.0;
		}
		mat->matriz[i][i]=1.0;
	}
}


/**
 * Funcao para verificar a linha da matriz, caso a linha esteja zerada a Funcao para o programa
 * pois se torna impossivel calcular a matriz inversa nesse formato
 */
int verificaLinha(tipo_matriz *mat,long int tamMatriz){
	long int i,j,cont;
	for(j=0;j<tamMatriz;j++){
		cont=0;
		for(i=0;i<tamMatriz;i++){
			if(mat->matriz[i][j]==0)
				cont++;
		}
		if(cont==tamMatriz){
			fprintf(stderr,"Linha Zerada !!!\n");
			exit(1);
		}
	}
	return 1;
}

/**
 * Funcao para verificar a coluna da matriz, caso a coluna esteja zerada a Funcao para o programa
 * pois se torna impossivel calcular a matriz inversa nesse formato
 */
int verificaColuna(tipo_matriz *mat,long int tamMatriz){
	long int i,j,cont;
	for(i=0;i<tamMatriz;i++){
		cont=0;
		for(j=0;j<tamMatriz;j++){
			if(mat->matriz[i][j]==0)
				cont++;
		}
		if(cont==tamMatriz){
			fprintf(stderr,"Coluna Zerada !!!\n");
			exit(1);
		}
	}
	return 1;
}


/**
 *  Funcao para o calculo da conta Lz=B, aonde L eh a matriz mat, z eh a matrizZ e B eh a matrizB 
 */
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

/**
 *  Funcao para o calculo da conta Ux=Z, aonde U eh a matriz mat, x eh a matrizX e Z eh a matrizZ 
 */
void calculaX(tipo_matriz *mat, tipo_matriz *matrizX,tipo_matriz *matrizZ,long int tamMatriz,long int k){
	long int i,j;
	double soma;
	//Calcula U
	matrizX->matriz[tamMatriz-1][k]=matrizZ->matriz[tamMatriz-1][k]/mat->matriz[mat->vetorLinha[tamMatriz-1]][tamMatriz-1];
	for(i=tamMatriz-2;i>=0;i--){
		soma=0;
		for(j=tamMatriz-1;j>i;j--){
			soma=soma+(matrizX->matriz[j][k]*(mat->matriz[mat->vetorLinha[i]][j]));
		}
		matrizX->matriz[i][k]=(matrizZ->matriz[i][k]-soma)/mat->matriz[mat->vetorLinha[i]][j];
	}
}


/**
 * Funcao para calcular o residuo entre a matriz identidade e a matriz AX
 */
void calculaResiduo(tipo_matriz *matrizA, tipo_matriz *matrizX,tipo_matriz *matrizResiduo,tipo_matriz *matrizZ,long int tamMatriz){
	long int i,j,k;
	double soma;
	tipo_matriz* matrizAxi = (tipo_matriz*) mialloc(sizeof(tipo_matriz));
	criaMatriz(matrizAxi,tamMatriz,1);
	//Calcula Residuo entre as matrizes
	multiplicaMatriz(matrizA,matrizX,matrizAxi,tamMatriz);
	for(k=0;k<tamMatriz;k++){
		
		for(i=0;i<tamMatriz;i++){
			matrizResiduo->matriz[i][k]=matrizZ->matriz[i][k]-matrizAxi->matriz[i][k];
		}
	}
}

/**
 * Funcao para achar a norma das colunas da matriz residuo, o resultado da conta é guardado no vetor norma
 */
void calculaNorma(tipo_matriz *mat,long int tamMatriz,double *norma){
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

void copiaMatrizR(tipo_matriz *matLU,tipo_matriz *mat1,tipo_matriz *mat2,long int tamMatriz,long int col){
	long int i;
	for(i=0;i<tamMatriz;i++){
			mat2->matriz[matLU->vetorLinha[i]][col]=mat1->matriz[matLU->vetorLinha[i]][col];
	}
	
}


/**
 * Resolve a conta da matriz refinada ou seja Aw=R
 */
int resolveRefinado(tipo_matriz *matR,tipo_matriz *matLU,tipo_matriz *matX,long int tamMatriz,double *norma,long double erro,long int *numRef){
	long int j,l,cont;
	tipo_matriz* matW = (tipo_matriz*) mialloc(sizeof(tipo_matriz));
	criaMatriz(matW,tamMatriz,1);
	tipo_matriz* matZ = (tipo_matriz*) mialloc(sizeof(tipo_matriz));
	criaMatriz(matZ,tamMatriz,1);
	cont=0;
	for(j=0;j<tamMatriz;j++){
		if(norma[j]>erro){ // caso a minha norma seja maior que o meu erro, sera feita a conta Aw=R
			
			calculaZ(matLU,matZ,matR,tamMatriz,j);
			calculaX(matLU,matW,matZ,tamMatriz,j);
			for(l=0;l<tamMatriz;l++)
				matX->matriz[l][j]+=matW->matriz[l][j];
			numRef[j]++;
			copiaMatrizR(matLU,matZ,matR,tamMatriz,j);
		}
		else{
			cont++; // caso não seja o contador de colunas que não precisam ser refinadas sera incrementado
		}
	}
	if(cont==tamMatriz-1)
		return 0; // caso o meu contador tenha o tamanho da matriz entao eu retorno 1 e assim paro o refinamento da matriz
	else
		return 1; // caso não seja do tamanho da matriz eu continuo o refinamento
	
}


/**
 * Chama as funcoes para calcular o residuo, calcular a norma e resolver o refinamento
 */
int refinar(tipo_matriz *matA,tipo_matriz *matX,tipo_matriz *matLU,tipo_matriz *matrizId,long double erro,long int tamMatriz,double *norma,long int *numRef){
	int res;
	

	tipo_matriz* matrizAxi = (tipo_matriz*) mialloc(sizeof(tipo_matriz));
	tipo_matriz* matrizResiduo = (tipo_matriz*) mialloc(sizeof(tipo_matriz));
	

	criaMatriz(matrizResiduo,tamMatriz,1);
	criaMatriz(matrizAxi,tamMatriz,1);

	//criaIdentidade(matrizId,tamMatriz);
	calculaResiduo(matA, matX,matrizResiduo, matrizId,tamMatriz);
	calculaNorma(matrizResiduo,tamMatriz,norma);
	
	res=resolveRefinado(matrizResiduo,matLU,matX,tamMatriz,norma,erro,numRef);
	if(res==0) // caso a funcao retorne 0 siginifica que nao existe mais nenhuma coluna para ser refinada
		return 0;
	else
		return 1;// caso a funcao retorne 1 siginifica que existe pelo menos uma coluna para ser refinada
}	

/**
 *  Funcao para fatorar a matriz LU no formato Lz=I e Ux=Z
 */
void fatoracaoLU(tipo_matriz *mat,tipo_matriz *matrizX,tipo_matriz *matrizId,long int tamMatriz){
	long int k;
	
	tipo_matriz* matrizZ = (tipo_matriz*) mialloc(sizeof(tipo_matriz));

	criaMatriz (matrizZ,tamMatriz,1);

	for(k=0;k<tamMatriz;k++){
		// calcula L
		calculaZ(mat,matrizZ,matrizId,tamMatriz,k);

		//Calcula U
		calculaX(mat,matrizX,matrizZ,tamMatriz,k);
	}
}


/**
 * Funcao para ler a matriz usando arq como parametro para saber se existe um arquivo para ler
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
	criaMatriz(mat,tamMatriz,1);
	for(i=0;i<tamMatriz;i++){
			for(j=0;j<tamMatriz;j++){
					fscanf(le,"%lf",&mat->matriz[i][j]); // le tanto do arquivo quanto do terminal a mat
			}
	}
	return tamMatriz;
}


/**
 * Funcao para procurar o maior elemento pertencente a coluna i nas linhas abaixo da linha atual
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
 *  Funcao que troca as linhas da matriz, ou seja troca de posição os elementos entre as duas linhas
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
 * Chama a funcao para achar o maior elemento das colunas, caso exista esse elemento
 * faz a troca de linha entre os dois, se nao deixa as linhas como estao, e apos isso zera a coluna do pivo
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

/**
 * Funcao para copiar os elementos de uma matriz para outra
 */
void copiaMatriz(tipo_matriz *mat1,tipo_matriz *mat2,long int tamMatriz){
	long int i,j;
	for(i=0;i<tamMatriz;i++){
		for(j=0;j<tamMatriz;j++){
			mat2->matriz[i][j]=mat1->matriz[i][j];
		}
	}
	
}

/**
 * Chama as funcoes necessarias para resolver a matriz por gauss
 * 
 */
void resolve_mat(tipo_matriz* mat,long int tamMatriz){
        long int i;
        
        if((verificaLinha(mat,tamMatriz)==1) && (verificaColuna(mat,tamMatriz)==1)){
			for(i=0;i<tamMatriz-1;i++){
					pivoteamento(mat,i,tamMatriz); // pivoteia as linhas da mat para zerar as colunas
			}
        }
}

/**
 * imprime o resultado no stdin
 */
void imprimeResultado(tipo_matriz *mat,double *norma,long int *numRef,long int tamMatriz){
	long int i;
	if(norma!=NULL){
		printf("#\n# Norma L2: ");
		for(i=0;i<tamMatriz;i++){
			printf("%.20lf ",norma[i]);
		}
	}
	printf("\n# NumRefinamento: ");
	for(i=0;i<tamMatriz;i++){
			printf("%ld ",numRef[i]);
	}
	printf("\n#\n%ld\n",tamMatriz);
	imprime_mat(NULL,mat,tamMatriz);
	printf("\n");

}


/**
 * imprime o resultado no arquivo de saida
 */
void escreve_arquivo(FILE *arqSai,double *norma,long int *numRef,tipo_matriz *matrizX,long int tamMatriz){
	long int i;
	if(norma!=NULL){
		fprintf(arqSai,"%s","#\n# Norma L2: ");
		for(i=0;i<tamMatriz;i++){
			fprintf(arqSai,"%.20lf ",norma[i]);
		}
	}
	fprintf(arqSai,"\n# NumRefinamento: ");
	for(i=0;i<tamMatriz;i++){
		fprintf(arqSai,"%ld ",numRef[i]);
	}
	fprintf(arqSai,"\n#\n%ld\n",tamMatriz);
	imprime_mat(arqSai,matrizX,tamMatriz);
	 
}

void inicializaVetor(long int *numRef,long int tamMatriz){
	int i;
	for(i=0;i<tamMatriz;i++){
		numRef[i]=0;
	}
}