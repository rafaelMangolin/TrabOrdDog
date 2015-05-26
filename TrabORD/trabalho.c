#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct lista{
    char chave[100];
    int offset;
    int prox_f;
    int prox_i1;
    int prox_i2;
};


int main(){
    int escolha;

    //lista de id de raças
    struct lista lIdR[100];
    int cIdR = 0;

    struct lista lIdG[100];
    int cIdG = 0;
    /*lista invertida de raças por id de grupo de raças

    int cAuxIdG = 0;
    struct lista_invertida_num lAuxIdG[100];*/

    //lista de id de individuos
    struct lista lIdI[100];
    int cIdI = 0;

    /*lista invertida de Individuos por id de raças
    struct lista_invertida_num lIdIR[100];
    int cIdIR = 0;
    int cAuxIdIR = 0;
    struct lista_invertida_num lAuxIdIR[100];*/

    do{
    	escolha = escolheOpcao();
    	switch(escolha){
    		case 1:
                converterArquivo("individuos.txt", lIdI, &cIdI,lIdR, &cIdR/*, lIdIR, lAuxIdIR, &cIdIR, &cAuxIdIR,NULL,NULL,NULL,NULL*/);
                break;
            case 2:
                converterArquivo("racas-grupos.txt", lIdR, &cIdR, lIdG, &cIdG);
                break;
            case 3:
                buscarPorIndice(lIdI, cIdI,"Digite o id do individuo que deseja recuperar:","individuos.txt",NULL);
                break;
            case 4:
                buscarPorIndice(lIdR, cIdR,"Digite o id da raça que deseja recuperar:","racas-grupos.txt",NULL);
                break;
            case 5:
                buscarPorListaInvertida(lIdG, lIdR, cIdG,"Digite o id do grupo de raças das raças que deseja recuperar:","racas-grupos.txt",NULL);
                break;
            case 6:
                buscarPorListaInvertida(lIdR, lIdI, cIdR,"Digite o id da raças dos individuos deseja recuperar:","individuos.txt",NULL);
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
    printf("4- Buscar raca por id\n");
    printf("5- Buscar racas por id do grupo de raca\n");
    printf("6- Buscar individuos por id de raca\n");
	// outras operacoes
	printf("0- Sair\n");
}

int escolheOpcao(){
	int operacao;

	do{
		mostrarOpcoes();
		scanf("%i", &operacao);
        getchar();
		if(operacao > 6 || operacao < 0){
			printf("Opcao invalida\n");
		}
	}while(operacao > 6 || operacao < 0);
	return operacao;
}

void converterArquivo(char nome[], struct lista list_id[], int *count_id, struct lista list_i1[], int *count_i1/*, struct lista_invertida_num list_i1_aux[], int *count_i1, int *count_i1_aux, struct lista_invertida_num list_i2[], struct lista_invertida_num list_i2_aux[], int *count_i2, int *count_i2_aux*/){
	FILE *fimport;
    FILE *fimportaux;
	FILE *faux;

    char aux, field_buffer[400], buffer[400];
    int count = 0, i = 0, offset_count = 0;

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
                char string[100];
                if(strlen(field_buffer)==1){
                    string[0] = '0';
                    string[1] = '\0';
                } else{
                    string[0]='\0';
                }
                strcat(string,field_buffer);
                strcpy(list_id[*count_id].chave,string);
                list_id[*count_id].offset = offset_count;
            }else if(count == 4){
            	length = strlen(buffer);
            	//puts(buffer);
            	fwrite(&length, 1, sizeof(length), faux);
            	fwrite(buffer, 1, length, faux);
            	offset_count+=length+2;
            	buffer[0]='\0';
            	count = 0;
            	*count_id += 1;
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


    quickSort(list_id,0,*count_id-1);
    fclose(fimportaux);

    if(strcmp(nome,"individuos.txt")== 0){
    //gerar lista de individuos por
        gerarListaInvertida(list_i1, count_i1, list_id, count_id, "individuos.txt", 2);
    }else{
    //gerar lista de raças por grupo de raças
        gerarListaInvertida(list_i1, count_i1, list_id, count_id, "racas-grupos.txt", 3);
    }

}

void trocaValores(struct lista* a, struct lista* b){
    struct lista aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

int divide(struct lista vec[], int esquerdo, int direito){
    int i, j;

    i = esquerdo;
    for (j = esquerdo + 1; j <= direito; ++j){
        int aux = strcmp(vec[j].chave, vec[esquerdo].chave);
        if (aux < 0){
            ++i;
            trocaValores(&vec[i], &vec[j]);
        }
    }
    trocaValores(&vec[esquerdo], &vec[i]);

    return i;
}

void quickSort(struct lista vec[], int esquerdo, int direito){
    int r;
    if (direito > esquerdo){
        r = divide(vec, esquerdo, direito);
        quickSort(vec, esquerdo, r - 1);
        quickSort(vec, r + 1, direito);
    }
}

void buscarPorIndice(struct lista list[], int count, char frase[], char arqName[], char indice[100]){
    char valor[100];
    if(frase == NULL){
        strcpy(valor,indice);
    }else{
        puts(frase);
        gets(valor);
        char string[100];
        if(strlen(valor)==1){
            string[0] = '0';
            string[1] = '\0';
        } else{
            string[0]='\0';
        }
        strcat(string,valor);
        strcpy(valor,string);
    }

    int aux = buscaBinariaNumero(list,0,count-1,valor);

    if(aux != -1){
        FILE *fp;
        if ((fp = fopen(arqName,"r+")) == NULL) {
            printf("Meça as tentativa de abrir seus arquivos Parça!");
            exit(1);
        }
        if(fseek(fp,list[aux].offset, SEEK_SET) == 0){
            short length;
            char buffer[500];
            buffer[0]='\0';
            printf("offset -- %i\n", list[aux].offset);
            fread(&length, 1, sizeof(length),fp);
            fread(buffer, 1, length, fp);
            buffer[strlen(buffer)]='\0';
            printf("registro %s, length %i\n", buffer, length);
        }
    }else {
        printf("     ____  ____  ____ \n");
        printf("    ||4 ||||0 ||||4 ||\n");
        printf("    ||__||||__||||__||\n");
        printf("    |/__\\||/__\\||/__\\|\n\n    File not found\n");

    }

}

void buscarPorListaInvertida(struct lista list[], struct lista list2[], int count, char frase[], char arqName[], char indice[100]){
    char valor[100];
    if(frase == NULL){
        strcpy(valor,indice);
    }else{
        puts(frase);
        gets(valor);
        char string[100];
        if(strlen(valor)==1){
            string[0] = '0';
            string[1] = '\0';
        } else{
            string[0]='\0';
        }
        strcat(string,valor);
        strcpy(valor,string);
    }

    int aux = buscaBinariaNumero(list,0,count-1,valor);

    if(aux != -1){
        FILE *fp;
        if ((fp = fopen(arqName,"r+")) == NULL) {
            printf("Meça as tentativa de abrir seus arquivos Parça!");
            exit(1);
        }
        int j = list[aux].prox_f;
        while(j!=-1){
            short length;
            char buf[500];
            fseek(fp,list2[j].offset, SEEK_SET);
            fread(&length, 1, sizeof(length),fp);
            fread(buf, 1, length, fp);
            buf[length] = '\0';
            printf("%s\n", buf);
            j=list2[j].prox_i1;
        }
    }else {
        printf("     ____  ____  ____ \n");
        printf("    ||4 ||||0 ||||4 ||\n");
        printf("    ||__||||__||||__||\n");
        printf("    |/__\\||/__\\||/__\\|\n\n    File not found\n");

    }
}

int buscaBinariaNumero(struct lista list[], int inicio, int fim, char value[]){
    if(inicio <= fim){
        int aux =((fim-inicio)/2)+inicio;
        int cmp = strcmp(list[aux].chave,value);
        if(cmp > 0){
            return buscaBinariaNumero(list,inicio,aux-1,value);
        }else if(cmp < 0){
            return buscaBinariaNumero(list,aux+1,fim,value);
        }else{
            return aux;
        }
    }else{
        return -1;
    }
}

int caminharListaInvertida(struct lista list[], int indice){
    printf("%s -->", list[indice].chave);
    if(list[indice].prox_i1 == -1){
        printf("\n\n");
        return indice;
    }else{
        return caminharListaInvertida(list, list[indice].prox_i1);
    }
}

void gerarListaInvertida(struct lista lista1[], int *count1, struct lista lista2[], int *count2, char nome[], int breakPoint){
    FILE *fp;
    if ((fp = fopen(nome,"r+")) == NULL) {
        printf("Meça as tentativa de abrir seus arquivos Parça!");
        exit(1);
    }

    int i;

    for(i=0;i<*count2;i++){
        fseek(fp,lista2[i].offset, SEEK_SET);
        short length;
        char buf[500];
        fread(&length, 1, sizeof(length),fp);
        fread(buf, 1, length, fp);
        char *token;
        int count = 0;
        buf[length] = '\0';
        printf("%s\n",buf);
        token = strtok(buf, "|");


        while( token != NULL ){
            token[strlen(token)] = '\0';
            count++;
            if(count== breakPoint){

                char string[100];
                if(strlen(token)==1){
                    string[0] = '0';
                    string[1] = '\0';
                } else{
                    string[0]='\0';
                }
                strcat(string,token);
                int aux = buscaBinariaNumero(lista1,0,*count1,string);
                printf("%s    %i\n\n", string, aux);
                if(aux==-1){
                    strcpy(lista1[*count1].chave,string);
                    lista1[*count1].prox_f = i;
                    quickSort(lista1,0,*count1);
                    *count1+=1;
                }else{
                    aux = caminharListaInvertida(lista2,lista1[aux].prox_f);
                    lista2[aux].prox_i1 = i;
                }

                lista2[i].prox_i1 = -1;
            }
            token = strtok(NULL, "|");
        }
    }

    /*for(i=0;i<*count1;i++){
        int j=lista1[i].prox_f;
        printf("%s\n",lista1[i].chave);
        while(j!=-1){
            short length;
            char buf[500];
            fseek(fp,lista2[j].offset, SEEK_SET);
            fread(&length, 1, sizeof(length),fp);
            fread(buf, 1, length, fp);
            //buf[length] = '\0';
            printf("%s -->", buf);
            j=lista2[j].prox_i1;
        }
        puts("\n\n");
    }*/

    fclose(fp);
}

