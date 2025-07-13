#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMDLINE 8192

int is_wildcard(const char *arg) {
    return strchr(arg, '*') || strchr(arg, '?');
}

void append_arg(char *dest, const char *src) {
    strcat(dest, " ");
    if (strchr(src, ' ')) {
        strcat(dest, "\"");
        strcat(dest, src);
        strcat(dest, "\"");
    } else {
        strcat(dest, src);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args with optional wildcards]\n", argv[0]);
        return 1;
    }

    char cmdline[MAX_CMDLINE] = {0};

    for (int i = 1; i < argc; i++) {
        if (is_wildcard(argv[i])) {
            WIN32_FIND_DATAA findData;
            HANDLE hFind = FindFirstFileA(argv[i], &findData);

            if (hFind == INVALID_HANDLE_VALUE) {
                // No matches - include literal
                append_arg(cmdline, argv[i]);
                continue;
            }

            do {
                append_arg(cmdline, findData.cFileName);
            } while (FindNextFileA(hFind, &findData));

            FindClose(hFind);
        } else {
            append_arg(cmdline, argv[i]);
        }
    }

    printf("Running:%s\n", cmdline);
    return system(cmdline);
}
