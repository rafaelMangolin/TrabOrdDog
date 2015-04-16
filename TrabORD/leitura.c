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
    buffer[0] = '\0';

    while(aux != EOF) {
    	if(aux!= 10 && aux!= 13){
	    	field_buffer[i] = aux;
	        aux = fgetc(fpi);
	        i++;
    	}else if(aux == 13){
            field_buffer[i] ='\0';
            strcat(buffer, field_buffer);
            strcat(buffer, "|");
            printf("%s   %s\n",field_buffer,buffer);
            i++;
            count++;
            aux = fgetc(fpi);
            if(count == 4){
            	length = strlen(buffer);
            	fwrite(&length, 1, sizeof(length), fpif);
            	fwrite(buffer, 1, length, fpif);
            	buffer[0]='\0';
            	count = 0;
            	getchar();
            }
    	}else{
        	i = 0;
            field_buffer[0]='\0';
            aux = fgetc(fpi);
    	}
    }

    fclose(fpi);
    fclose(fpif);
}
