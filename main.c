#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    char * filepath = "/home/joshua/dev/Hex-Editor/test.txt";
    FILE * file = fopen(filepath, "r");

    if(file == NULL){
        printf("Error Opening File\n");
    }

    char buffer[13];
    fread(buffer, 1, 12, file);

    for (size_t i = 0; i < 12; i++)
    {
        printf("%02X ", buffer[i]);
    }
    
    printf("\n");
}