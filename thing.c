#include <stdio.h>
#include <windows.h>

int main() {
    printf("Press W, A, S, or D (ESC to quit)...\n");

    while (1) {
        // Check if keys are being held
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;

        if (GetAsyncKeyState('W') & 0x8000)
            printf("W is being held\n");

        if (GetAsyncKeyState('A') & 0x8000)
            printf("A is being held\n");

        if (GetAsyncKeyState('S') & 0x8000)
            printf("S is being held\n");

        if (GetAsyncKeyState('D') & 0x8000)
            printf("D is being held\n");

        Sleep(50); // Small delay to reduce CPU usage
    }

    return 0;
}
