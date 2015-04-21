#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//########
int main(){
    int escolha;

    do{
    	escolha = escolheOpcao();
    	switch(escolha){
    		case 1:
                converterArquivo();
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
	printf("1- Converter Arquivo\n");
	// outras operacoes
	printf("0- Sair\n");
}
int escolheOpcao(){
	int operacao;
	do{
		mostrarOpcoes();
		scanf("%i", &operacao);
        getchar();
		if(operacao > 1 && operacao < 0){
			printf("Opcao invalida\n");
		}
	}while(operacao > 1 && operacao < 0);
	return operacao;
}

void converterArquivo(){
	FILE *fimport;
    FILE *fimportaux;
	FILE *faux;
	char arqName[130];
    char aux, field_buffer[400], buffer[400];
    int count = 0, i = 0, qntd_reg;
    short length;

	printf("Digite o nome do arquivo que deve ser convertido\n");
	gets(arqName);

	if ((fimport = fopen(arqName,"r+")) == NULL) {
        printf("Meça as tentativa de abrir seus arquivos Parça!");
        exit(1);
    }
    if ((faux = fopen("aux.txt","w+")) == NULL) {
        printf("Meça as tentativa de abrir seus arquivos Parça!");
        exit(1);
    }

    printf("Digite a quantidade de campos por registro\n");
    scanf("%i", &qntd_reg);
    getchar();

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
            if(count == 4){
                printf("%s\n",buffer);
            	length = strlen(buffer);
            	fwrite(&length, 1, sizeof(length), faux);

            	fwrite(buffer, 1, length, faux);
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
    if ((fimportaux = fopen(arqName,"w+")) == NULL) {
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
}
