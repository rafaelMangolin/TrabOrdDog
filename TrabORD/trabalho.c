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
    int escolha, offset_count;

    struct lista lIdR[100];
    int cIdR = 0;

    struct lista lIdG[100];
    int cIdG = 0;

    struct lista lIdI[100];
    int cIdI = 0;

    struct lista lIdRS[100];
    int cIdRS = 0;

    do{
    	escolha = escolheOpcao();
    	switch(escolha){
    		case 1:
                converterArquivo("individuos.txt", lIdI, &cIdI, &offset_count);
                gerarListaInvertida(lIdR, &cIdR, lIdI, &cIdI, "individuos.txt", 2);
                gerarListaInvertida2(lIdRS, &cIdRS, lIdI, &cIdI, "individuos.txt", 2,4);
                break;
            case 2:
                converterArquivo("racas-grupos.txt", lIdR, &cIdR);
                gerarListaInvertida(lIdG, &cIdG, lIdR, &cIdR, "racas-grupos.txt", 3);
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
                buscarPorListaInvertida(lIdR, lIdI, cIdR,"Digite o id da raça dos individuos deseja recuperar:","individuos.txt",NULL);
                break;
            case 7:
                buscarPorListaInvertida2(lIdRS, lIdI, cIdRS,"Digite o id da raça dos individuos deseja recuperar:","Digite o sexo dos individuos que deseja recuperar[F/M]:","individuos.txt",NULL);
                break;
            case 8:
                inserirIndividuo(lIdI, &cIdI, lIdR,&cIdR, &offset_count);
                gerarListaInvertida(lIdR, &cIdR, lIdI, &cIdI, "individuos.txt", 2);
                gerarListaInvertida2(lIdRS, &cIdRS, lIdI, &cIdI, "individuos.txt", 2,4);
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
    printf("7- Buscar individuos por id de raca e sexo\n");
    printf("8- Inserir individuo\n");
	printf("0- Sair\n");
}

int escolheOpcao(){
	int operacao;

	do{
		mostrarOpcoes();
		scanf("%i", &operacao);
        getchar();
		if(operacao > 8 || operacao < 0){
			printf("Opcao invalida\n");
		}
	}while(operacao > 8 || operacao < 0);
	return operacao;
}

void converterArquivo(char nome[], struct lista list_id[], int *count_id, int *offset){
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
                list_id[*count_id].prox_f = -1;
                list_id[*count_id].prox_i1 = -1;
                list_id[*count_id].prox_i2 = -1;
            }else if(count == 4){
            	length = strlen(buffer);
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

    fclose(fimportaux);

    if(strcmp("individuos.txt", nome)==0){
        *offset = offset_count;
    }
    quickSort(list_id,0,*count_id-1);
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
            fread(&length, 1, sizeof(length),fp);
            fread(buffer, 1, length, fp);
            buffer[length]='\0';
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

void buscarPorListaInvertida2(struct lista list[], struct lista list2[], int count, char frase1[],char frase2[], char arqName[], char indice[100]){
    char valor1[100];
    char valor2[100];
    if(frase1 == NULL){
        strcpy(valor1,indice);
    }else{
        puts(frase1);
        gets(valor1);
        puts(frase2);
        gets(valor2);
        char string[100];
        if(strlen(valor1)==1){
            string[0] = '0';
            string[1] = '\0';
        } else{
            string[0]='\0';
        }
        strcat(string,valor1);
        strcat(string,valor2);
        strcpy(valor1,string);
    }
    puts(valor1);
    int aux = buscaBinariaNumero(list,0,count-1,valor1);

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
            j=list2[j].prox_i2;
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
    if(list[indice].prox_i1 == -1){
        return indice;
    }else{
        return caminharListaInvertida(list, list[indice].prox_i1);
    }
}

int caminharListaInvertida2(struct lista list[], int indice){
    if(list[indice].prox_i2 == -1){
        return indice;
    }else{
        return caminharListaInvertida2(list, list[indice].prox_i2);
    }
}

void gerarListaInvertida(struct lista lista1[], int *count1, struct lista lista2[], int *count2, char nome[], int breakPoint){
    FILE *fp;
    if ((fp = fopen(nome,"r+")) == NULL) {
        printf("Meça as tentativa de abrir seus arquivos Parça!");
        exit(1);
    }

    int i;

    for(i=0;i<*count1;i++){
        lista1[i].prox_f = -1;
    }
    for(i=0;i<*count2;i++){
        lista2[i].prox_i1 = -1;
    }

    for(i=0;i<*count2;i++){
        fseek(fp,lista2[i].offset, SEEK_SET);
        short length;
        char buf[500];
        fread(&length, 1, sizeof(length),fp);
        fread(buf, 1, length, fp);
        char *token;
        int count = 0;
        buf[length] = '\0';
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
                if(aux == -1){
                    strcpy(lista1[*count1].chave,string);
                    lista1[*count1].prox_f = i;
                    quickSort(lista1,0,*count1);
                    *count1+=1;
                }else if(lista1[aux].prox_f == -1){
                    lista1[aux].prox_f = i;
                }else{
                    aux = caminharListaInvertida(lista2,lista1[aux].prox_f);
                    lista2[aux].prox_i1 = i;
                    lista2[aux].prox_f = -1;
                }
            }
            token = strtok(NULL, "|");
        }
    }
    fclose(fp);
}

void gerarListaInvertida2(struct lista lista1[], int *count1, struct lista lista2[], int *count2, char nome[], int breakPointN, int breakPointL){
    FILE *fp;
    if ((fp = fopen(nome,"r+")) == NULL) {
        printf("Meça as tentativa de abrir seus arquivos Parça!");
        exit(1);
    }

    int i;

    for(i=0;i<*count1;i++){
        lista1[i].prox_f = -1;
    }
    for(i=0;i<*count2;i++){
        lista2[i].prox_i2 = -1;
    }

    for(i=0;i<*count2;i++){
        fseek(fp,lista2[i].offset, SEEK_SET);
        short length;
        char buf[500];
        fread(&length, 1, sizeof(length),fp);
        fread(buf, 1, length, fp);
        char *token;
        int count = 0;
        buf[length] = '\0';
        token = strtok(buf, "|");
        char string1[100];
        char string2[100];

        while( token != NULL ){
            token[strlen(token)] = '\0';
            count++;
            if(count== breakPointN){
                if(strlen(token)==1){
                    string1[0] = '0';
                    string1[1] = '\0';
                }else{
                    string1[0]='\0';
                }
                strcat(string1,token);

            }else if(count== breakPointL){
                strcpy(string2,token);
            }
            token = strtok(NULL, "|");
        }
        strcat(string1,string2);
        int aux = buscaBinariaNumero(lista1,0,*count1,string1);
        if(aux == -1){
            strcpy(lista1[*count1].chave,string1);
            lista1[*count1].prox_f = i;
            quickSort(lista1,0,*count1);
            *count1+=1;
        }else if(lista1[aux].prox_f == -1){
            lista1[aux].prox_f = i;
        }else{
            aux = caminharListaInvertida2(lista2,lista1[aux].prox_f);
            lista2[aux].prox_i2 = i;
        }
    }
    fclose(fp);
}

void inserirIndividuo(struct lista lista1[], int *count1, struct lista lista2[], int *count2, int *offset_count){
    FILE *fp;
    if ((fp = fopen("individuos.txt","r+")) == NULL) {
        printf("Meça as tentativa de abrir seus arquivos Parça!");
        exit(1);
    }
    int aux;
    char buffer[500], faux[100], string1[10], string_id[10];
    buffer[0] = '\0';
    do{
        printf("Digite o id do individuo:");
        gets(faux);
        if(strlen(faux)==1){
            string1[0] = '0';
            string1[1] = '\0';
        } else{
            string1[0]='\0';
        }
        strcat(string1,faux);
        strcpy(string_id,string1);
        aux = buscaBinariaNumero(lista1, 0, *count1-1,string1);
        if(aux!=-1){
            printf("Id ja existe, digite outro  %i\n\n", aux);
        }
    }while(aux!=-1);
    strcat(faux,"|");
    strcat(buffer,faux);

    do{
        printf("Digite o id da raca:");
        gets(faux);
        if(strlen(faux)==1){
            string1[0] = '0';
            string1[1] = '\0';
        } else{
            string1[0]='\0';
        }
        strcat(string1,faux);
        aux = buscaBinariaNumero(lista2, 0, *count2-1,string1);
        if(aux==-1){
            printf("Raca nao existe, digite outra\n\n");
        }
    }while(aux==-1);
    strcat(faux,"|");
    strcat(buffer,faux);

    printf("Digite o nome do individuo:");
    gets(faux);
    strcat(faux,"|");
    strcat(buffer,faux);

    printf("Digite o sexo do individuo[F/M]:");
    gets(faux);
    strcat(faux,"|");
    strcat(buffer,faux);

    short length = strlen(buffer);


    lista1[*count1].offset = *offset_count;
    strcpy(lista1[*count1].chave,string_id);
    lista1[*count1].prox_f = -1;
    lista1[*count1].prox_i1 = -1;
    lista1[*count1].prox_i2 = -1;

    *count1+= 1;

    fseek(fp,*offset_count,SEEK_SET);
    *offset_count+= length+2;
    fwrite(&length, 1, sizeof(length), fp);
    fwrite(buffer, 1, length, fp);

    fclose(fp);

    quickSort(lista1,0,*count1-1);
}
