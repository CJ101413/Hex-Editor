#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) (k & 0x1F)
#define UP_ARROW 1
#define DOWN_ARROW 2
#define LEFT_ARROW 3
#define RIGHT_ARROW 4

struct termios original;

void die(char * s)
{
    perror(s);
    exit(1);
}

void HighlightByte(unsigned char * isHighlighted, int arrowKey, long filesize)
{
    int hIndex;

    for (int i = 0; i < filesize; i++)
    {
        if(isHighlighted[i] == '1'){
            hIndex = i;
            break;
        }
    }
    

    if(arrowKey == RIGHT_ARROW)
    {
        if(hIndex + 1 < filesize){
            isHighlighted[hIndex + 1] = '1';
            isHighlighted[hIndex] = '0';    
        }
    }
    if(arrowKey == LEFT_ARROW)
    {
        if(hIndex - 1 >= 0){
            isHighlighted[hIndex - 1] = '1';
            isHighlighted[hIndex] = '0'; 
        }
    }
    if(arrowKey == DOWN_ARROW)
    {
        if(hIndex + 16 < filesize){
            isHighlighted[hIndex + 16] = '1';
            isHighlighted[hIndex] = '0'; 
        }
    }
    if(arrowKey == UP_ARROW)
    {
        if(hIndex - 16 >= 0){
            isHighlighted[hIndex - 16] = '1';
            isHighlighted[hIndex] = '0'; 
        }
    }
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

void editorProcessKeyPress(unsigned char * isHighlighted,long filesize)
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
            HighlightByte(isHighlighted, UP_ARROW, filesize);
        }
        else if(seq[0] == '[' && seq[1] == 'B'){
            HighlightByte(isHighlighted, DOWN_ARROW, filesize);
        }
        else if(seq[0] == '[' && seq[1] == 'C'){
            HighlightByte(isHighlighted, RIGHT_ARROW, filesize);
        }
         else if(seq[0] == '[' && seq[1] == 'D'){
            HighlightByte(isHighlighted, LEFT_ARROW, filesize);
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

void RefreshScreen()
{
    printf("\033[2J\033[3J\033[H");
}

void PrintOffset(size_t offset)
{
    printf("%08zX ; ", offset);
}

int PrintHex(unsigned char * buffer, unsigned char * isHighlighted, size_t * offset, long * total_bytes)
{
    int j = 0;
    while(*total_bytes > 0 && j < 16)
    {
        if(isHighlighted[*offset] == '1')
        {
            printf("\033[30;43m%02X\033[0m ", buffer[*offset]);
        }
        else
        {
            printf("%02X ", buffer[*offset]);
        }
        
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

    printf("\r\n");  
}

int main()
{
    enableRawMode();

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

    unsigned char * isHightlight = malloc(filesize);
    if(isHightlight == NULL)
    {
        perror("Memory Allocation Failure!\n");
        return 1;
    }

    memset(isHightlight, '0', filesize);
    isHightlight[0] = '1';

    fread(buffer, sizeof(char), filesize, file);

    size_t offset = 0;
    double lineCount = ceil(filesize / 16.0);
    
    
    while(1)
    {
        long total_bytes = filesize;
        RefreshScreen();

        for (size_t i = 0; i < lineCount; i++)
        {
        PrintOffset(offset);
        int bytesRead = PrintHex(buffer, isHightlight, &offset, &total_bytes);
        PrintASCII(buffer, offset, bytesRead);
        }

        offset = 0;
        editorProcessKeyPress(isHightlight, filesize);
    }

    free(buffer);
    buffer = NULL;
    fclose(file);
}