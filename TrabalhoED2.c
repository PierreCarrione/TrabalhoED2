#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int tmpAux = 1;
int varNomes = 0;

typedef struct CatalogoFilmes {
    char titleType[13];
    char primaryTitle[422];
    char originalTitle[422];
    unsigned char isAdult;
    unsigned short int startYear;
    unsigned short int endYear;
    char runtimeMinutes[6];
    char genres[33];
} CatalogoFilmes;

char **alocarMatrizChar(int lin, int col){
    
    char **matriz = (char**) malloc(lin * sizeof(char*));

    for(int i = 0; i < lin; i++){
        matriz[i] = (char*) malloc(col* sizeof(char));
    }
    return matriz;
}

void liberarMatriz(char **matriz, int lin){
    
    for(int i = 0; i < lin; i++){
        free(matriz[i]);
        //matriz[i] = NULL;
    }
    free(matriz);
    //matriz = NULL;
}

void fillNullMatriz(char **matriz, int lin){

    for(int i = 0; i < lin; i++){
        strcpy(matriz[i], "");
    }
} 

void copyMatriz(char **matrizOrigem, char **matrizDestino, int lin){

    for(int i = 0; i < lin; i++){
        strcpy(matrizDestino[i],matrizOrigem[i]);
    }
}

char *gerarNomeArquivo(){
    
    char a[6];
    sprintf(a, "%d",tmpAux);
    char *b = (char*) malloc(12 * sizeof(char));
    strcpy(b,"tmp");
    strcat(b,a);
    strcat(b,".dat");
    tmpAux++;
    return b;
}

char *gerarNomeArquivoSaida(){
    
    char a[6];
    sprintf(a, "%d",tmpAux);
    char *b = (char*) malloc(12 * sizeof(char));
    strcpy(b,"saida");
    strcat(b,a);
    strcat(b,".dat");
    tmpAux++;
    return b;
}

void copyStruct(CatalogoFilmes *var, char *nomeArquivo, int tamStruct){

    FILE *file = fopen(nomeArquivo, "rb");

    for(int i = 0; i < tamStruct; i++){
        fread(&var[i],sizeof(var[i]),1,file);
    }
    fclose(file);
}

int menorString(CatalogoFilmes *vetor, int tamVetor){
    
    int pos = -1;
    char menor[450] = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
    for(int i = 0; i < tamVetor;i++){
        if(strcmp(vetor[i].originalTitle,menor) < 0 || strcmp(vetor[i].originalTitle,menor) == 0){
            strcpy(menor,vetor[i].originalTitle);
            pos = i;
        }
    }
    return pos;
}

void buscaBinariaFilms(char *nomeArq, char *oriTitle){

    CatalogoFilmes filmes;
    FILE *file = fopen(nomeArq,"rb");
    if(file == NULL){
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    int achou = 0;
    long inicio, fim;
    int moreResults = 0;
    long ftel = 0;
    
    inicio = 0;
    fseek(file,0,SEEK_END);
    fim = ftell(file) - sizeof(CatalogoFilmes);
    ftel = ftell(file);
    fseek(file,0,SEEK_SET);

    while( inicio <= fim && achou == 0){

        long i = ( (inicio + fim) / sizeof(CatalogoFilmes) )/2;
        
        fseek(file, i, SEEK_SET);
        fread(&filmes, sizeof(filmes), 1, file);

        if(strcmp(filmes.originalTitle,oriTitle) == 0){
            printf("**********************************************");
            printf("\nCaracteristicas do filme.\n");
            printf("Title Type : %s\n",filmes.titleType);
            printf("Primary Title : %s\n",filmes.primaryTitle);
            printf("Original Title : %s\n",filmes.originalTitle);
            printf("Is Adult? : %d\n",filmes.isAdult);
            printf("Start Year : %d\n",filmes.startYear);
            printf("End Year : %d\n",filmes.endYear);
            printf("Runtime minutes: %s\n",filmes.runtimeMinutes);
            printf("Genre : %s\n",filmes.genres);
            printf("**********************************************\n");
            while(moreResults == 0 && ftell(file) != ftel){
                fread(&(filmes), sizeof(filmes), 1, file);
                if(strcmp(filmes.originalTitle,oriTitle) == 0){
                    printf("**********************************************");
                    printf("\nCaracteristicas do filme.\n");
                    printf("Title Type : %s\n",filmes.titleType);
                    printf("Primary Title : %s\n",filmes.primaryTitle);
                    printf("Original Title : %s\n",filmes.originalTitle);
                    printf("Is Adult? : %d\n",filmes.isAdult);
                    printf("Start Year : %d\n",filmes.startYear);
                    printf("End Year : %d\n",filmes.endYear);
                    printf("Runtime minutes: %s\n",filmes.runtimeMinutes);
                    printf("Genre : %s\n",filmes.genres);
                    printf("**********************************************\n");
                }
                else{
                    moreResults = 1;
                }
            }
            achou = 1;
        } 
        
        if(inicio == fim){
            printf("Filme não encontrado\n");
            break;
        }
        else if(strcmp(filmes.originalTitle,oriTitle) < 0){
            inicio = (i * sizeof(CatalogoFilmes))  + sizeof(CatalogoFilmes);
            //printf("inicio %ld\n",inicio);
        } 
        else{
            fim = i * sizeof(CatalogoFilmes);
            //printf("fim %ld\n",fim);
        } 
    }

    if(achou == 0){
        printf("Filme não encontrado\n");
    }
    fclose(file);    
}   


int main(int argc, char const *argv[])
{   
    //O último arquivo gerado é o arquivo ordenado
    long int qtdRegistros = 4341497;
    long int outWhile = 10000;
    long int tamMemoria = 10000;
    int contReservatorio = 0;
    char **nomeFiles = alocarMatrizChar(250,12);
    CatalogoFilmes auxiliar;
    CatalogoFilmes *memoria = (CatalogoFilmes*)malloc(tamMemoria * sizeof(CatalogoFilmes));
    CatalogoFilmes *reservatorio = (CatalogoFilmes*)malloc(tamMemoria * sizeof(CatalogoFilmes));
    FILE *file;
    FILE *filmes = fopen("films.dat","rb");
       
    if(!memoria){
        printf("Não conseguiu alocar\n");
        exit(1);
    }
    if(!reservatorio){
        printf("Não conseguiu alocar\n");
        exit(1);
    }
    if(!nomeFiles){
        printf("Não conseguiu alocar\n");
        exit(1);
    }
    
    copyStruct(memoria,"films.dat",tamMemoria);//copia os 10000 registros pra memoria
    int posMenor = menorString(memoria,tamMemoria);//Acha o menor entre eles
    strcpy(nomeFiles[varNomes],gerarNomeArquivo());//Gera um nome tmp de arquivo
    file = fopen(nomeFiles[varNomes], "wb");//Abre um arquivo com o nome gerado
    fwrite(&memoria[posMenor],sizeof(memoria[posMenor]),1,file);//Escreve o menor no arquivo gerado
    fseek(filmes, tamMemoria * sizeof(CatalogoFilmes),SEEK_SET);//Seta o arquivo para a posição 10000+1
  
    while(outWhile < qtdRegistros){

        fread(&auxiliar,sizeof(auxiliar),1,filmes);
        if(strcmp(memoria[posMenor].originalTitle,auxiliar.originalTitle) < 0 || strcmp(memoria[posMenor].originalTitle,auxiliar.originalTitle) == 0 ){
            //Verifica se a struct que li é menor do que eu salvei no arquivo
            strcpy(memoria[posMenor].titleType,auxiliar.titleType);
            strcpy(memoria[posMenor].primaryTitle,auxiliar.primaryTitle);
            strcpy(memoria[posMenor].originalTitle,auxiliar.originalTitle);
            memoria[posMenor].isAdult = auxiliar.isAdult;
            memoria[posMenor].startYear = auxiliar.startYear;
            memoria[posMenor].endYear = auxiliar.endYear;
            strcpy(memoria[posMenor].runtimeMinutes,auxiliar.runtimeMinutes);
            strcpy(memoria[posMenor].genres,auxiliar.genres);
            posMenor = menorString(memoria,tamMemoria);
            fwrite(&memoria[posMenor],sizeof(memoria[posMenor]),1,file);
        }else{//Se não for menor salva no reservatório
            strcpy(reservatorio[contReservatorio].titleType,auxiliar.titleType);
            strcpy(reservatorio[contReservatorio].primaryTitle,auxiliar.primaryTitle);
            strcpy(reservatorio[contReservatorio].originalTitle,auxiliar.originalTitle);
            reservatorio[contReservatorio].isAdult = auxiliar.isAdult;
            reservatorio[contReservatorio].startYear = auxiliar.startYear;
            reservatorio[contReservatorio].endYear = auxiliar.endYear;
            strcpy(reservatorio[contReservatorio].runtimeMinutes,auxiliar.runtimeMinutes);
            strcpy(reservatorio[contReservatorio].genres,auxiliar.genres);
            contReservatorio++;
        }

        if(contReservatorio == tamMemoria){

            for(int i = 0; i < tamMemoria;i++){
                int posLess = menorString(memoria,tamMemoria);
                //Salva os itens da memoria no arquivo da ordem ordenada
                if(posLess != -1 && posLess != posMenor){
                    fwrite(&memoria[posLess],sizeof(memoria[posLess]),1,file);
                    strcpy(memoria[posLess].originalTitle,"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
                }else{
                    strcpy(memoria[posMenor].originalTitle,"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"); 
                }
            }
            for(int i = 0;i < tamMemoria;i++){
                //passa os arquivos do reservatório para a memoria
                strcpy(memoria[i].titleType,reservatorio[i].titleType);
                strcpy(memoria[i].primaryTitle,reservatorio[i].primaryTitle);
                strcpy(memoria[i].originalTitle,reservatorio[i].originalTitle);
                memoria[i].isAdult = reservatorio[i].isAdult;
                memoria[i].startYear = reservatorio[i].startYear;
                memoria[i].endYear = reservatorio[i].endYear;
                strcpy(memoria[i].runtimeMinutes,reservatorio[i].runtimeMinutes);
                strcpy(memoria[i].genres,reservatorio[i].genres);   
                strcpy(reservatorio[i].originalTitle,"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
            }

            fclose(file);
            varNomes++;//Incrementa o gerador de tmps de nome de arquivos
            strcpy(nomeFiles[varNomes],gerarNomeArquivo());//Gera um novo arquivo de saida
            file = fopen(nomeFiles[varNomes], "wb");
            posMenor = menorString(memoria,tamMemoria);//Acha um novo menor
            fwrite(&memoria[posMenor],sizeof(memoria[posMenor]),1,file);
            contReservatorio = 0;
        }

        outWhile++;

        if(outWhile == qtdRegistros){
            //grava em ordem os itens da memoria para o arquivo
            for(int i = 0; i < tamMemoria;i++){
                int posLess = menorString(memoria,tamMemoria);

                if(posLess != -1 && posLess != posMenor){
                    fwrite(&memoria[posLess],sizeof(memoria[posLess]),1,file);
                    strcpy(memoria[posLess].originalTitle,"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
                }else{
                    strcpy(memoria[posMenor].originalTitle,"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"); 
                }
            }
            fclose(file);
            varNomes++;
            strcpy(nomeFiles[varNomes],gerarNomeArquivo());//Abre um novo arquivo
            file = fopen(nomeFiles[varNomes], "wb");
            for(int i = 0;i < tamMemoria;i++){//Grava o que tem no reservatorio em ordem em um novo arquivo de saida
                int posLess = menorString(reservatorio,tamMemoria);
                if(posLess != -1){
                    fwrite(&reservatorio[posLess],sizeof(reservatorio[posLess]),1,file);
                    strcpy(reservatorio[posLess].originalTitle,"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
                }
                else{
                    strcpy(reservatorio[posMenor].originalTitle,"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz"); 
                }
            }
            fclose(file);
        } 
    }

    free(memoria);
    free(reservatorio);
    fclose(filmes);

    CatalogoFilmes aux1;
    CatalogoFilmes aux2;
    FILE *file1; 
    FILE *file2;
    FILE *file3; 
    long ftell1 = 0;
    long ftell2 = 0;
    int fimLoop1 = 0;
    int fimLoop2 = 0;
    int fimQtdArquivos = 0;
    int qtdArquivos = 0;
    int arqSaida = 0;
    int contSaida = 0;
    tmpAux = 1; // geradora de nome de arquivo
    varNomes = 0; //contadora para arquivos de saida tmp

    for(int i = 0; i < 250; i++){
        if(strlen(nomeFiles[i]) != 0){
            qtdArquivos++;
        }
    }

    char **nomeFilesSaida = alocarMatrizChar((qtdArquivos-1),12);//Vetor de string que irá ter os nomes dos files das intercalações

    while(fimLoop1 == 0){

        if(arqSaida < qtdArquivos - 1){//Faz a intercalação dos arquivos tmps
            file1 = fopen(nomeFiles[arqSaida],"rb");
            fread(&aux1,sizeof(CatalogoFilmes),1,file1);
            ftell1 = ftell(file1);
            arqSaida++;
            file2 = fopen(nomeFiles[arqSaida],"rb");
            fread(&aux2,sizeof(CatalogoFilmes),1,file2);
            ftell2 = ftell(file2);
            arqSaida++;
            strcpy(nomeFilesSaida[varNomes],gerarNomeArquivoSaida());
            file3 = fopen(nomeFilesSaida[varNomes], "wb");
            varNomes++;
            fimLoop2 = 0;
        }
        else{
            fimQtdArquivos = 1;
        }

        if(fimQtdArquivos == 1){//Verifica agora se precisa fazer a intercalação dos arquivos "saida"

            if(arqSaida == (qtdArquivos - 1)){//Se isso for igual quer dizer que tinha numero impar de arquivos criados
                file1 = fopen(nomeFiles[arqSaida],"rb");
                fread(&aux1,sizeof(CatalogoFilmes),1,file1);
                ftell1 = ftell(file1);
                arqSaida++;
                file2 = fopen(nomeFilesSaida[contSaida],"rb");
                fread(&aux2,sizeof(CatalogoFilmes),1,file2);
                ftell2 = ftell(file2);
                contSaida++;
                strcpy(nomeFilesSaida[varNomes],gerarNomeArquivoSaida());
                file3 = fopen(nomeFilesSaida[varNomes], "wb");
                varNomes++;
                fimLoop2 = 0;
            }
            else if(contSaida < (qtdArquivos-2)){//Vai intercalando os arquivos saida"num".dat
                file1 = fopen(nomeFilesSaida[contSaida],"rb");
                fread(&aux1,sizeof(CatalogoFilmes),1,file1);
                ftell1 = ftell(file1);
                contSaida++;
                file2 = fopen(nomeFilesSaida[contSaida],"rb");
                fread(&aux2,sizeof(CatalogoFilmes),1,file2);
                ftell2 = ftell(file2);
                contSaida++;
                strcpy(nomeFilesSaida[varNomes],gerarNomeArquivoSaida());
                file3 = fopen(nomeFilesSaida[varNomes], "wb");
                varNomes++;
                fimLoop2 = 0;
            }
            else{
                fimLoop1 = 1;
                fimLoop2 = 1;
            }
        }

        while (fimLoop2 == 0){
            int posMenor = strcmp(aux1.originalTitle,aux2.originalTitle);
            
            if(posMenor == 0 || posMenor < 0){
                
                fwrite(&aux1,sizeof(CatalogoFilmes),1,file3);
                fread(&aux1,sizeof(CatalogoFilmes),1,file1);
                if(ftell(file1) != ftell1){//Verifica se o arquivo chegou no tamanho max dele, se sim entra no else
                    ftell1 = ftell(file1);
                } 
                else{//Grava o topo do arquivo que não acabou e após grava o restante dos dados dele
                    fwrite(&aux2,sizeof(CatalogoFilmes),1,file3);
                    while(fread(&aux2,sizeof(CatalogoFilmes),1,file2) != 0){
                        fwrite(&aux2,sizeof(CatalogoFilmes),1,file3);
                    }
                    fclose(file1);
                    fclose(file2);
                    fclose(file3);
                    fimLoop2 = 1;
                }
            }
            else{//O mesmo processo que acima so que invertendo os arquivos
                fwrite(&aux2,sizeof(CatalogoFilmes),1,file3);
                fread(&aux2,sizeof(CatalogoFilmes),1,file2);
                if(ftell(file2) != ftell2){
                    ftell2 = ftell(file2);
                }
                else{
                    fwrite(&aux1,sizeof(CatalogoFilmes),1,file3);
                    while(fread(&aux1,sizeof(CatalogoFilmes),1,file1) != 0){
                        fwrite(&aux1,sizeof(CatalogoFilmes),1,file3);
                    }
                    fclose(file1);
                    fclose(file2);
                    fclose(file3);
                    fimLoop2 = 1;
                }
            }
        }
    }
    liberarMatriz(nomeFiles,250);
    liberarMatriz(nomeFilesSaida,qtdArquivos-1);
    return 0;
}