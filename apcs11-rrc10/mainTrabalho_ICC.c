#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functionsICC.h"

int main(int argc, char *argv[]){
	unsigned long int refinamento=0; // valor padrão definido
	long int saidaArq,tamMatriz,nRef; //variaveis que servirao para o controle do programa
    long int *numRef; // vetor que sera usado caso seja necessario mostrar quantos refinamentos cada coluna teve 
    
    int semRefi=1; // caso essa variavel seja igual a 1 a matriz devera fazer refinamento
    
    long double erro=0.0001; // valor padrão definido
    
    double *norma; //guardara as normas de cada coluna da matriz refinada
    
    char *arquivo_entrada=NULL; // variavel que recebera o caminho para o arquivo com a matriz para a leitura
    char *arquivo_saida=NULL;// variavel que recebera o caminho para o arquivo com a matriz para a escrita
    
    tipo_matriz* matrizLU = (tipo_matriz*) malloc(sizeof(tipo_matriz));// aloca a matriz LU
    tipo_matriz* matrizA = (tipo_matriz*) malloc(sizeof(tipo_matriz));// aloca a matriz A
    tipo_matriz* matrizX = (tipo_matriz*) malloc(sizeof(tipo_matriz));// aloca a matriz X
    
    FILE *arq=NULL; //variavel para o uso do arquivo
    
    saidaArq=le_parametros(argc,argv,&erro,&refinamento,&arquivo_entrada,&arquivo_saida); // verifica os valores passados por parametro
    
    nRef=0; // essa variavel sera usada como contador para o numero de refinamentos
    
    if(saidaArq!=0){
            if(arquivo_entrada!=NULL){
                    arq=fopen(arquivo_entrada,"r"); // caso exista arquivo de entrada , abre ele para ler a mat
            }
            
            
            tamMatriz=le_mat(arq,matrizA); //le a mat, seja pelo terminal , ou por um arquivo texto
            if(arquivo_entrada!=NULL){
            	fclose(arq); // fecha o arquivo de entrada aberto caso exista
            }
            
            criaMatriz (matrizLU,tamMatriz); // aloca a matriz LU
            criaMatriz (matrizX,tamMatriz); // aloca a matriz X
            
            norma=(double *) malloc(sizeof(double)*tamMatriz); // aloca o vetor de norma
            
            numRef=(long int *) malloc(sizeof(long int)*tamMatriz); // aloca o vetor do numero de Refinamento
            inicializaVetor(numRef,tamMatriz);
            
            copiaMatriz(matrizA, matrizLU,tamMatriz); // copia a matriz A para a Matriz LU, para ter a matriz original salva
            
            resolve_mat(matrizLU,tamMatriz); // resolve a matriz por gauss
            fatoracaoLU(matrizLU,matrizX,tamMatriz); // fatora a matriz no formato Lz=I e Ux=Z
            
            while(nRef<refinamento){

            	semRefi=refinar(matrizA,matrizX,matrizLU,erro,tamMatriz,norma,numRef); // refina a matriz de acordo com os parametros passados
            	if(semRefi==0) // caso o retorno do refinamento seja igual a zero significa que a matriz não precisa ser mais refinada
            		break;

            	nRef++; // adiciona o contador de refinamento até que ele seja igual ao passado por parametro
            }
            if(arquivo_saida!=NULL){
            	arq=fopen(arquivo_saida,"w"); // abre o arquivo para a escrita
            	escreve_arquivo(arq,norma,numRef,matrizX,tamMatriz); // escreve no arquivo de saida o resultado das contas
            	fclose(arq);// fecha o arquivo de saida
            }
            else{
            	imprimeResultado(matrizX,norma,numRef,tamMatriz); // imprime o resultado na tela caso não exista o arquivo de saida
            
            }
    }
    //libera as alocacoes dos vetores e das matrizes
    free(matrizLU);
    free(matrizA);
    free(matrizX);
    free(norma);
    free(numRef);
    free(arquivo_entrada);
    free(arquivo_saida);
    return(0);
}
