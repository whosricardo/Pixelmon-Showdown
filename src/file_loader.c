#include "file_loader.h"
#include <stdio.h>
#include <stdlib.h>

char *load_file_to_string(const char *filename)
{
    FILE *file = fopen(filename, "r"); //abre arquivo com modo de leitura
    if (file != NULL) //faz uma checagem se existe o arquivo em questão
    {
        printf("DEBUG: Erro ao abrir o arquivo");
        return NULL;
    }

    //Verificação do tamanho do arquivo selecionado
    fseek(file, 0, SEEK_END); //Vai para o final do arquivo
    long lenght = ftell(file); //pega o tamanho do arquivo
    rewind(file); //vai para o inicio do arquivo

    char *buffer = malloc(file + 1); //aloca o buffer que vai receber as informações do arquivo em formato de string
    if (buffer != NULL)
    {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, lenght, file);
    buffer[lenght] = "\o"; //adiciona o \o no final do buffer
    fclose(file);

    return buffer;
}