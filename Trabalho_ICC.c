#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*void le_parametros(int *erro,unsigned int *refinamento,char **arquivo_entrada,char **arquivo_saida){
    int i;
    for(i=0;i<argc;i++){//if com as condiçoes do parametro
        if(argv[i]=="-e")
            *erro=argv[i+1];
        else if(argv[i]=="-r")
            *refinamento=argv[i+1];   
        else if(argv[i]=="-i")
            **arquivo_entrada=argv[i+1];
        else if(argv[i]=="-o")
            **arquivo_saida=argv[i+1];
        else if(argv[i]=="-h" && argc==2)
            printf( "Definição dos Paramêtros do Trabalho de ICC :\n"
                    "-e erro:\n parâmetro opcional no qual 0 < erro < 1 é o erro máximo aceitável para a norma L². O valor padrão para este parâmetro deve ser erro=0.0001.\n"
                    "-r ref:\n parâmetro opcional no qual 0 <= ref < 32768 é o número máximo de iterações a serem executadas no refinamento da solução. O valor padrão para este parâmetro deve ser ref=0.\n"
                    "-i arquivo_entrada:\n parâmetro opcional no qual arquivo_entrada é o caminho completo para o arquivo contendo a matriz a ser invertida. Em caso de ausência do parâmetro, a entrada deve ser lida de stdin.\n"
                    "-o arquivo_saida:\n parâmetro opcional no qual arquivo_saida é o caminho completo para o arquivo que vai conter a matriz inversa. Em caso de ausência do parâmetro, a saída deve ser impressa em stdout.\n"
                    "-h help:\n Mostra as definições dos argumentos\n");
    }
    


}*/

/**
 * le
 */
void le_matrizTerminal(FILE *arq,double **matriz){
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

void le_matrizArquivo(char *arquivo_entrada){




}

int main(int argc, char *argv[]){
    int erro;
    unsigned int refinamento;
    char *arquivo_entrada=NULL;
    char *arquivo_saida=NULL;
    double **matriz;
    matriz = malloc(sizeof(double **));
    FILE *arq=NULL;
        
//    le_parametros(erro,refinamento,&arquivo_entrada,&arquivo_saida);
//    if(*arquivo_entrada!=NULL){
//        le_matriz(*arquivo_entrada);
//    }
//    else{
//
//    }
//
//    resolve_matriz();
    le_matrizTerminal(arq,matriz);



}

