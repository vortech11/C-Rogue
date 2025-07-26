#include <conio.h>
#include <stdio.h>

int main() {
    printf("Press arrow keys (press 'q' to quit):\n");
    while (1) {
        int c = _getch();
        if (c == 0 || c == 224) { // Arrow keys or function keys
            printf("%d", c);
            int key = _getch();
            switch (key) {
                case 72: printf("Up arrow\n"); break;
                case 80: printf("Down arrow\n"); break;
                case 75: printf("Left arrow\n"); break;
                case 77: printf("Right arrow\n"); break;
            }
        } else if (c == 'q') {
            break;
        } else {
            printf("Pressed: %c\n", c);
        }
    }
    return 0;
}
