#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    char * filepath = "/home/joshua/dev/Hex-Editor/test.txt";
    FILE * file = fopen(filepath, "r");

    if(file == NULL){
        printf("Error Opening File\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    char buffer[filesize + 1];
    fread(buffer, sizeof(char), filesize, file);

    for (size_t i = 0; i < filesize; i++)
    {
        printf("%02X ", buffer[i]);
    }
    
    printf("; %s\n", buffer);
}