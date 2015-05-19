#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct lista_num{
    int chave;
    int offset;
};

//########
int main(){
    int escolha;
    struct lista_num indice_ID_R[100];
    int count_ID_R = 0;
    struct lista_num indice_ID_I[100];
    int count_ID_I = 0;

    do{
    	escolha = escolheOpcao();
    	switch(escolha){
    		case 1:
                converterArquivo("individuos.txt", indice_ID_I, &count_ID_I);
                break;
            case 2:
                converterArquivo("racas-grupos.txt", indice_ID_R, &count_ID_R);
                break;
            case 3:
                buscarIndicePorNumero(indice_ID_I,count_ID_I,"Digite o id do individuo que deseja recuperar:","individuos.txt");
                break;
    	}
    }while(escolha != 0);
}
void mostrarOpcoes(){
	printf("              /\\___/\\ \n");
	printf("              `)o o('  \n");
	printf("              {_:Y:.}_ \n");
	printf("--------------( )U-'( )----------\n");
	printf("Convercao de Arquivos\n\n");
	printf("Escolha o tipo de operacao:\n\n");
	printf("1- Converter Arquivo individuo.txt\n");
    printf("2- Converter Arquivo racas-grupos.txt\n");
    printf("3- Buscar individuo por id\n");
	// outras operacoes
	printf("0- Sair\n");
}
int escolheOpcao(){
	int operacao;

	do{
		mostrarOpcoes();
		scanf("%i", &operacao);
        getchar();
		if(operacao > 3 || operacao < 0){
			printf("Opcao invalida\n");
		}
	}while(operacao > 3 || operacao < 0);
	return operacao;
}

void converterArquivo(char nome[], struct lista_num list_ID_I[], int* qntd){
	FILE *fimport;
    FILE *fimportaux;
	FILE *faux;

    char aux, field_buffer[400], buffer[400];
    int count = 0, i = 0, offset_count = 0;
    *qntd = 0;
    short length;

	if ((fimport = fopen(nome,"r+")) == NULL) {
        printf("Meça as tentativa de abrir seus arquivos Parça!");
        exit(1);
    }
    if ((faux = fopen("aux.txt","w+")) == NULL) {
        printf("Meça as tentativa de abrir seus arquivos Parça!");
        exit(1);
    }

    aux = fgetc(fimport);
    buffer[0] = '\0';
    while(aux != EOF) {
    	if(aux!= 10 && aux!= 13){
	    	field_buffer[i] = aux;
	        aux = fgetc(fimport);
	        i++;
    	}else if(aux == 13){
            field_buffer[i] ='\0';
            strcat(buffer, field_buffer);
            strcat(buffer, "|");
            i++;
            count++;
            aux = fgetc(fimport);
            if(count == 1){
                list_ID_I[*qntd].chave = atoi(field_buffer);
                list_ID_I[*qntd].offset = offset_count;
                *qntd+=1;
            }else if(count == 4){
            	length = strlen(buffer);
            	fwrite(&length, 1, sizeof(length), faux);
            	fwrite(buffer, 1, length, faux);
            	offset_count+=length+2;
            	buffer[0]='\0';
            	count = 0;
            }
    	}else{
        	i = 0;
            field_buffer[0]='\0';
            aux = fgetc(fimport);
    	}
    }
    fclose(fimport);
    if ((fimportaux = fopen(nome,"w+")) == NULL) {
        printf("Meça as tentativa de abrir seus arquivos Parça!");
        exit(1);
    }
    rewind(faux);
    aux = fgetc(faux);
    while(aux != EOF){
        fputc(aux, fimportaux);
        aux = fgetc(faux);
    }
    remove("aux.txt");

    quickSort(list_ID_I,0,*qntd-1);
    fclose(fimportaux);
}

void trocaValores(struct lista_num* a, struct lista_num* b){
    struct lista_num aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

int divide(struct lista_num vec[], int esquerdo, int direito){
    int i, j;

    i = esquerdo;
    for (j = esquerdo + 1; j <= direito; ++j){
        if (vec[j].chave < vec[esquerdo].chave){
            ++i;
            trocaValores(&vec[i], &vec[j]);
        }
    }
    trocaValores(&vec[esquerdo], &vec[i]);

    return i;
}

void quickSort(struct lista_num vec[], int esquerdo, int direito){
    int r;
    if (direito > esquerdo){
        r = divide(vec, esquerdo, direito);
        quickSort(vec, esquerdo, r - 1);
        quickSort(vec, r + 1, direito);
    }
}

void buscarIndicePorNumero(struct lista_num list[], int count, char frase[], char arqName[]){
    int id;

    puts(frase);
    scanf("%i", &id);
    int aux = buscaBinariaNumero(list,0,count-1,id);


    if(aux != -1){
        FILE *fp;
        if ((fp = fopen(arqName,"r+")) == NULL) {
            printf("Meça as tentativa de abrir seus arquivos Parça!");
            exit(1);
        }
        if(fseek(fp,list[aux].offset, SEEK_SET) == 0){
            short length;
            char buffer[500];
            printf("offset -- %i\n", list[aux].offset);
            fread(&length, 1, sizeof(length),fp);
            fread(buffer, 1, length, fp);

            printf("registro %s, length %i\n", buffer, length);
        }
    }

}

int buscaBinariaNumero(struct lista_num list[], int inicio, int fim, int value){
    if(inicio <= fim){
        int aux =((fim-inicio)/2)+inicio;
        if(list[aux].chave > value){
            return buscaBinariaNumero(list,inicio,aux-1,value);
        }else if(list[aux].chave < value){
            return buscaBinariaNumero(list,aux+1,fim,value);
        }else{
            return aux;
        }
    }else{
        return -1;
    }
}

