#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>

int main()
{
    // char * filepath = "/home/joshua/dev/Hex-Editor/test.txt";
    // FILE * file = fopen(filepath, "rb");

    // if(file == NULL){
    //     perror("Error Opening File\n");
    //     return 1;
    // }

    // fseek(file, 0, SEEK_END);
    // long filesize = ftell(file);
    // rewind(file);

    // unsigned char * buffer = malloc(filesize);
    // if(buffer == NULL)
    // {
    //     perror("Memory Allocation Failure!\n");
    //     return 1;
    // }

    // fread(buffer, sizeof(char), filesize, file);

    // double lineCount = ceil(filesize / 16.0);
    // size_t offset = 0;
    // long total_bytes = filesize;

    // for (size_t i = 0; i < lineCount; i++)
    // {
    //     //print offset
    //     printf("%08zX ; ", offset);

    //     //print hex values
    //     size_t j = 0;
    //     while (j < 16 && total_bytes != 0)
    //     {
    //         printf("%02X ", buffer[offset]);

    //         offset++;
    //         total_bytes--;
    //         j++;
    //     }
        
    //     //padding
    //     if(j < 16){
    //         for (size_t l = 0; l < 16 - j; l++)
    //         {
    //             printf("   ");
    //         }
    //     }
        
    //     //print ASCII
    //     printf(" ; ");
    //     for (size_t k = offset - j; k < offset; k++)
    //     {
    //         if(buffer[k] >= 31 && buffer[k] <= 126)
    //             printf("%c", buffer[k]);
    //         else
    //             printf(".");
    //     }

    //     printf("\n");  
    // }

    // free(buffer);
    // buffer = NULL;
    // fclose(file);

    struct termios term;
    char c;

    tcgetattr(STDERR_FILENO, &term);
    cfmakeraw(&term);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    while(read(STDIN_FILENO, &c, 1) == 1)
    {
        printf("%02X\r\n", c);
    }
}