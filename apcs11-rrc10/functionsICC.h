/**
 * Essa Struct guardara uma matriz e um vetor de linha , esse vetor sera usado
 * para guardar o lugar atual das linhas apos os pivoteamentos da matriz
 */
typedef struct tipo_matriz{ 
	double **matriz; 
	long int *vetorLinha;
}tipo_matriz;

typedef struct StructDados{
	void *pointer;	//Endereco
	int size;	//Tamanho
	int flag;	//Ocupado ou nao
} Dados;



void *mialloc (size_t size);

void cipurge ();

void GarbageCollector ();

long int le_parametros(long int argc, char *argv[],long double *erro,unsigned long int *refinamento,char **arquivo_entrada,char **arquivo_saida);

void criaVetorLinha(tipo_matriz* mat,long int tamMatriz);

void criaMatriz(tipo_matriz *mat,long int tamMatriz,int tipo);

void multiplicaMatriz(tipo_matriz* matA,tipo_matriz* matB,tipo_matriz* matR,long int tamMatriz);

void imprime_mat(FILE *arq,tipo_matriz* mat,long int tamMatriz);

void criaIdentidade(tipo_matriz *mat,long int tamMatriz);

int verificaLinha(tipo_matriz *mat,long int tamMatriz);

int verificaColuna(tipo_matriz *mat,long int tamMatriz);

void calculaZ(tipo_matriz *mat, tipo_matriz *matrizZ,tipo_matriz *matrizB,long int tamMatriz,long int k);

void calculaX(tipo_matriz *mat, tipo_matriz *matrizX,tipo_matriz *matrizZ,long int tamMatriz,long int k);

void calculaResiduo(tipo_matriz *matrizA, tipo_matriz *matrizX,tipo_matriz *matrizResiduo,tipo_matriz *matrizZ,long int tamMatriz);

void calculaNorma(tipo_matriz *mat,long int tamMatriz,double *norma);

void copiaMatrizR(tipo_matriz *matLU,tipo_matriz *mat1,tipo_matriz *mat2,long int tamMatriz,long int col);

int resolveRefinado(tipo_matriz *matR,tipo_matriz *matLU,tipo_matriz *matX,long int tamMatriz,double *norma,long double erro,long int *numRef);

int refinar(tipo_matriz *matA,tipo_matriz *matX,tipo_matriz *matLU,long double erro,long int tamMatriz,double *norma,long int *numRef);

void fatoracaoLU(tipo_matriz *mat,tipo_matriz *matrizX,long int tamMatriz);

long int le_mat(FILE *arq,tipo_matriz* mat);

long int procuraMaior(tipo_matriz* mat,long int i, long int tamMatriz);

void trocaLinhas (tipo_matriz *mat, long int linhaOri, long int linhaDest, long int tamMatriz);

void zeraColuna(tipo_matriz* mat,long int linZerada,long int i,long int tamMatriz);

void pivoteamento(tipo_matriz* mat,long int i, long int tamMatriz);

void copiaMatriz(tipo_matriz *mat1,tipo_matriz *mat2,long int tamMatriz);

void resolve_mat(tipo_matriz* mat,long int tamMatriz);

void imprimeResultado(tipo_matriz *mat,double *norma,long int *numRef,long int tamMatriz);

void escreve_arquivo(FILE *arqSai,double *norma,long int *numRef,tipo_matriz *matrizX,long int tamMatriz);

void inicializaVetor(long int *numRef,long int tamMatriz);