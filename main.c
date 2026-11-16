#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

struct termios original;

void disableRawMode(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original);
}

void enableRawMode(void)
{
    tcgetattr(STDIN_FILENO, &original);
    atexit(disableRawMode);

    struct termios raw = original;

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

int main()
{
    char c;

    enableRawMode();
    
    while (read(STDERR_FILENO, &c, 1) == 1 && c != 'q')
    {
        if(iscntrl(c))
        {
            printf("%d\r\n", c);
        }
        else 
        {
            printf("%d ('%c')\n", c, c);
        }
    }
    return 0;
    

    //char * filepath = "/home/joshua/dev/Hex-Editor/test.txt";
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







    // struct termios term;
    // char c;
    // int i = 0;

    // FILE * file = fopen(filepath, "r+");

    // if(file == NULL){
    //     perror("Error Opening File\n");
    //     return 1;
    // }

    // fseek(file, 0, SEEK_END);
    // long filesize = ftell(file);
    // rewind(file);

    // char * buffer = malloc(filesize);
    // if(buffer == NULL)
    // {
    //     perror("Memory Allocation Failure!\n");
    //     return 1;
    // }

    // fread(buffer, sizeof(char), filesize, file);

    // tcgetattr(STDERR_FILENO, &term);
    // term.c_lflag &= ~ICANON;  
    // term.c_iflag &= ~IXON;
    // tcsetattr(STDIN_FILENO, TCSANOW, &term);

    // while(read(STDIN_FILENO, &c, 1) == 1)
    // {
    //     if(c == 0x1B)
    //     {
    //         //ESC
    //         continue;
    //     }
    //     if(c == 0x13)
    //     {
    //         //Ctrl + S
    //         fprintf(stderr, "Ctrl+S detected!\n");
    //         int written = fwrite(buffer, sizeof(char), filesize + i, file);
    //         fflush(file);
    //         continue;
    //     }

    //     char * temp = realloc(buffer, (filesize + i + 1));
    //     buffer = temp;
    //     buffer[filesize + i] = c;
    //     i++;
    // }

    // fclose(file);
    // free(buffer);


}