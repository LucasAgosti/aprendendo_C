#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ABSURDOMEUDEUS 1000000

FILE *pgm;
FILE *newPgm;
int janela;
short int laplace[3][3] = {{0, -1, 0},
                           {-1, 4, -1},
                           {0, -1, 0}};

//cria matriz x,y
unsigned char** criarMatriz(int x, int y){
	unsigned char **temp;

	temp = (unsigned char**)malloc(x* sizeof(char*));
	
	int i;

	for(i = 0; i < x; i++){
		temp[i] = (unsigned char*)malloc(y*sizeof(char));
	}
	return temp;
}

//le informacoes do arquivo
unsigned char** lerImagem(unsigned char **M, int x, int y){

	int i,j;
	char lixo[1000];
	unsigned char temp;

	for(i = janela/2; i < x + janela/2 - 1; i++){
		for(j = janela/2; j < y + janela/2 - 1; j++){
	        char k;
	        
	        k = fgetc(pgm);
	        if( k == '#'){
	            j--;
	            fgets(lixo,ABSURDOMEUDEUS,pgm);
	            continue;	
            }
            fseek(pgm,-1,SEEK_CUR);
		    fscanf(pgm, "%hhu ", &temp);
			M[i][j] = temp;	
		}
	}
	return M;
}

//cria arquivo .pgm
void criarArquivoBorda(unsigned char **M, int x, int y, int scale,char *nome){

	int i,j;
	
	newPgm = fopen(nome, "w");

	if(newPgm == NULL){
		printf("Impossivel criar arquivo\n");
		exit(1);
	}

	fprintf(newPgm,"P2\n");
	fprintf(newPgm,"%d %d\n", y, x);
	fprintf(newPgm,"%d\n", scale);

	for(i = janela/2; i < x+janela/2 ; i++){
		for(j = janela/2; j < y+janela/2 ; j++){
			fprintf(newPgm,"%d ", M[i][j]);
		}
		fprintf(newPgm,"\n");
	}
	fclose(newPgm);
	printf("Arquivo criado com sucesso.\n");
}

//aplica filtro de suavizacao
unsigned char** filtro(unsigned char **original, int x, int y, int jan){

    int aux = jan/2;
            
    unsigned char** M = criarMatriz(x + 2*aux, y + 2*aux);
	for(int i = 0; i < x + 2*aux; i++){
    	for(int j = 0; j < y + 2*aux; j++){
    		M[i][j] = 0;
    	}
    }
    for(int i = aux; i < x + 1; i++){
    	for(int j = aux; j < y + 1; j++){
    		M[i][j] = original[i][j];
    	}
    }

	int soma = 0;
	for(int linha = aux; linha < x + 1; linha++){
		for(int coluna = aux; coluna < y + 1; coluna++){
			for(int i = linha - aux; i < linha + aux + 1; i++){
				for(int j = coluna - aux; j < coluna + aux + 1; j++){
					soma += M[i][j];
				}
			}
			M[linha][coluna] = soma / (jan*jan);
			soma = 0;
		}
	}
	return M;
}

//gera borda
int Bordar(int i, int j, short int laplace[][3], unsigned char **pgm){
    int p, q, temp;
    int element = 0;
    i = i - 1;
    j = j - 1;
    temp = j;
    while(p < 3){
        while(q < 3){
            element += pgm[i][j] * laplace[p][q];
            j++;
        }
		p++;
        j = temp;
        i++;
    }
    return element;
}

//libera memoria alocada
void freeMatriz(unsigned char **M, int x){
	int i;
	for (i = 0; i < x; ++i){
		free(M[i]);
	}

	free(M);
}

//le cabecalho do arquivo
void lerCabecalho(int *x,int *y,int *scale){

	char lixo[1000];
	unsigned char c;
	fgets(lixo,ABSURDOMEUDEUS,pgm);
	*y = -1;
	*x = -1;
	*scale = -1;
	while(*scale < 0 ){
	    
	    char k;
		k = fgetc(pgm);
        
        if( k == '#'){
			fgets(lixo,ABSURDOMEUDEUS,pgm);
            continue;	
        }     
        
        fseek(pgm,-1,SEEK_CUR);
		
        fscanf(pgm, "%c",&c);
        		
			if(*y == -1){
				fseek(pgm,-1,SEEK_CUR);
				fscanf(pgm, "%d", &(*y));

			}else if(*x == -1){
				fseek(pgm,-1,SEEK_CUR);
				fscanf(pgm, "%d", &(*x));	

			}else if(*scale == -1){	
				fseek(pgm,-1,SEEK_CUR);
				fscanf(pgm, "%d", &(*scale));
		}	
	}
}
