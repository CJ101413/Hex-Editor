#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) (k & 0x1F)

struct termios original;

void die(char * s)
{
    perror(s);
    exit(1);
}

char editorReadKey()
{
    int nread;
    char c;
    while((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if(nread == -1){
            die("Read Error");
        }
    }

    return c;
}

void editorProcessKeyPress()
{
    char c = editorReadKey();
    char seq[2];

    switch (c)
    {
    case CTRL_KEY('q'):
        exit(0);
        break;
    case 0x1B:
        if(read(STDIN_FILENO, &seq[0], 1) != 1 || read(STDIN_FILENO, &seq[1], 1) != 1){
            die("ESC read error");
        }
        else if(seq[0] == '[' && seq[1] == 'A'){
            printf("UP ARROW KEY\r\n");
        }
    }
}

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

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void PrintOffset(size_t offset)
{
    printf("%08zX ; ", offset);
}

int PrintHex(unsigned char * buffer, size_t * offset, long * total_bytes)
{
    int j = 0;
    while(*total_bytes > 0 && j < 16)
    {
        printf("%02X ", buffer[*offset]);
        *offset += 1;
        *total_bytes -= 1;
        j++;
    }

    for (int i = 0; i < 16 - j; i++)
    {
        printf("   ");
    }

    printf(" ; ");

    return j;
}

void PrintASCII(unsigned char * buffer, size_t start, int bytesRead)
{
    for (size_t k = start - bytesRead; k < start; k++)
    {
        if(buffer[k] >= 32 && buffer[k] <= 126)
            printf("%c", buffer[k]);
        else
            printf(".");
    }

    printf("\n");  
}

int main()
{
    // enableRawMode();
    // while(1)
    // {
    //     editorProcessKeyPress();
    // }

    char * filepath = "/home/joshua/dev/Hex-Editor/test.txt";
    FILE * file = fopen(filepath, "rb");

    if(file == NULL){
        perror("Error Opening File\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    unsigned char * buffer = malloc(filesize);
    if(buffer == NULL)
    {
        perror("Memory Allocation Failure!\n");
        return 1;
    }

    fread(buffer, sizeof(char), filesize, file);

    size_t offset = 0;
    double lineCount = ceil(filesize / 16.0);
    long total_bytes = filesize;

    for (size_t i = 0; i < lineCount; i++)
    {
        PrintOffset(offset);
        int bytesRead = PrintHex(buffer, &offset, &total_bytes);
        PrintASCII(buffer, offset, bytesRead);
    }
    
    while(1)
    {
        editorProcessKeyPress();
    }

    free(buffer);
    buffer = NULL;
    fclose(file);
}