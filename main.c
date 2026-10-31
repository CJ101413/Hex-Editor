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

    double lineCount = ceil(filesize / 16.0);
    size_t offset = 0;
    
    for (size_t i = 0; i < lineCount; i++)
    {
        //print offset
        printf("%08zX ; ", offset);

        //print last line
        if (i == lineCount - 1 && filesize % 16 != 0)
        {
            for (size_t k = 0; k < (filesize % 16); k++)
            {
                printf("%02X ", buffer[offset]);
                offset++;
            }

            for (size_t i = 0; i < 16 - (filesize % 16); i++)
            {
                printf("   ");
            }

            printf(" ; ");
            for (size_t k = offset - (filesize % 16); k < offset; k++)
            {
                printf("%c", buffer[k]);
            }
            printf("\n");  

            continue;
        }

        //print Hex values (Except last line)
        for (size_t j = 0; j < 16; j++)
        {
            printf("%02X ", buffer[offset]);
            offset++;
        }

        //print english text (Except last line)
        printf(" ; ");
        for (size_t k = offset - 16; k < offset; k++)
        {
            printf("%c", buffer[k]);
        }
        printf("\n");    
    }
}