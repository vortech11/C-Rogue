#include <stdio.h>   // For printf()

#define ESC "\e["

void print_8_colours()
{
    for (int i = 0; i < 256; i++)
    {
        if (i % 16 == 0 && i != 0)
            printf("\n");
        printf(ESC "8;48;2;2;2;%dm %3d" ESC "m", i, i);
    }
}

int main(){
    //printf("\033[?25l");
    //printf("\e[1;1H\e[2J");
    //printf("\e[0;37m");
    //printf("what\nerm\n");
    ////printf("\e[2;1H");
    //printf("huh\n");
    //printf("\e[0;0;0m");

    //print_8_colours();
    printf(ESC "48;2;7;82;7m  " ESC "0m\n");
}