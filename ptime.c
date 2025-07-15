#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: ptime <command> [args...]\n");
        return 1;
    }

    // Combine arguments into a single command line
    char cmdline[1024] = "";
    for (int i = 1; i < argc; ++i)
    {
        strcat(cmdline, argv[i]);
        if (i < argc - 1)
            strcat(cmdline, " ");
    }

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    LARGE_INTEGER freq, start, end;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    if (!CreateProcessA(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        fprintf(stderr, "Failed to run command: %s\n", cmdline);
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    QueryPerformanceCounter(&end);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    double elapsed = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    printf("Ptime by Andy, %.3f\n", elapsed);

    return 0;
}
