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

char *run_subcommand(const char *cmd) {
    static char buffer[4096];
    char line[256];
    buffer[0] = '\0';

    FILE *fp = _popen(cmd, "r");
    if (!fp) return NULL;

    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }
        strcat(buffer, line);
        strcat(buffer, " ");
    }
    _pclose(fp);

    return buffer;
}

void expand_backticks(char *dest, const char *src) {
    const char *p = src;
    while (*p) {
        if (*p == '`') {
            const char *end = strchr(p + 1, '`');
            if (!end) break; // unmatched backtick, skip

            char cmd[1024] = {0};
            strncpy(cmd, p + 1, end - p - 1);

            char *output = run_subcommand(cmd);
            if (output) strcat(dest, output);

            p = end + 1;
        } else {
            size_t len = strlen(dest);
            dest[len] = *p;
            dest[len + 1] = '\0';
            p++;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args with wildcards or `subcommands`]\n", argv[0]);
        return 1;
    }

    char cmdline[MAX_CMDLINE] = {0};

    for (int i = 1; i < argc; i++) {
        char expanded[MAX_CMDLINE] = {0};

        // Handle `backtick` substitution first
        if (strchr(argv[i], '`')) {
            expand_backticks(expanded, argv[i]);
        } else {
            strcpy(expanded, argv[i]);
        }

        if (is_wildcard(expanded)) {
            WIN32_FIND_DATAA findData;
            HANDLE hFind = FindFirstFileA(expanded, &findData);

            if (hFind == INVALID_HANDLE_VALUE) {
                append_arg(cmdline, expanded);
                continue;
            }

            do {
                append_arg(cmdline, findData.cFileName);
            } while (FindNextFileA(hFind, &findData));

            FindClose(hFind);
        } else {
            append_arg(cmdline, expanded);
        }
    }

    printf("Running:%s\n", cmdline);
    return system(cmdline);
}
