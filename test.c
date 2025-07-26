#include <termios.h> // For termios functions
#include <unistd.h>  // For read()
#include <stdio.h>   // For printf()

int main(){
    printf("\033[?25l");
    printf("\e[1;1H\e[2J");
    printf("what\nerm\n");
    //printf("\e[2;1H");
    printf("huh\n");
}