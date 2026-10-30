#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

int main()
{
    char * filepath = "/home/joshua/dev/Hex-Editor/test.txt";
    FILE * file = fopen(filepath, "rb");

    if(file == NULL){
        printf("Error Opening File\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    char buffer[filesize + 1];
    fread(buffer, sizeof(char), filesize, file);

    double lineCount = ceil(sizeof(buffer) / 16.0);
    size_t offset = 0;
    
    for (size_t i = 0; i < lineCount; i++)
    {
        printf("%08zX ", offset);
        for (size_t j = 0; j < 16; j++)
        {
            printf("%02X ", buffer[offset]);
            offset++;
        }

        for (size_t k = offset - 16; k < offset; k++)
        {
            printf("%c", buffer[k]);
        }
        printf("\n");    
    }
    
    
}