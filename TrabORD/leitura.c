#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    FILE *fp;
    // fp = fopen("racas-gruposFormatados.txt","r+");
    if ((fp = fopen("racas-gruposFormatados.txt","r+")) == NULL) {
        printf("Meça as tentativa de abrir seus arquivos Parça!");
        exit(1);
    }

    char buffer[400];
    int aux;
    short length;
	aux = fread(&length, 1, sizeof(length), fp);
	fread(buffer, 1, length, fp);

    while(aux != 0){
		buffer[length] = '\0';
		puts(buffer);
		getchar();
		aux = fread(&length, 1, sizeof(length), fp);
		fread(buffer, 1, length, fp);
    }

    fclose(fp);
}