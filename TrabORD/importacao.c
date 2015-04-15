#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    FILE *fpi;
    fpi = fopen("individuos.txt","r+");
    FILE *fpif;
    fpif = fopen("individuosFormatados.txt","w+");
    char aux, field_buffer[400], buffer[400];
    int count = 0, i = 0;
    short length;

    aux = fgetc(fpi);
    field_buffer[0]='\0';
    buffer[0] = '\0';
    while(aux != EOF) {
    	field_buffer[i] = aux;
        aux = fgetc(fpi);
        i++;
    	if(aux == 13){
            field_buffer[i] ='\0';
            strcat(buffer, field_buffer);
            strcat(buffer, "|");
            count++;
            if(count == 4){
                puts(buffer);
            	length = strlen(buffer);
            	printf("%d\n", length);
            	puts("Salvar length");
            	fwrite(&length, 1, sizeof(length), fpif);
            	puts("Salvar Buffer");
            	fwrite(buffer, 1, length, fpif);
            	buffer[0]='\0';
            }
    	}else if(aux == 10){
    	getchar();
            i = 0;
            getchar();
            field_buffer[0]='\0';
            getchar();
            aux = fgetc(fpi);
    	}
    }

    fclose(fpi);
    fclose(fpif);
}
