#include <windows.h>
#include <stdio.h>

int main() {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;
    double deltaTime;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    // Simulate work
    Sleep(100);  // sleep for 1 second

    QueryPerformanceCounter(&end);

    deltaTime = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Delta Time: %f seconds\n", deltaTime);

    return 0;
}
