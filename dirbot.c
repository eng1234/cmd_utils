#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>  // for _getcwd

void walk_directory(const char *base);

void print_usage(void)
{
    printf("Dirbot - Recursive File Lister with Size and Timestamp\n");
    printf("Usage:\n");
    printf("  dirbot.exe [start_folder]\n");
    printf("\n");
    printf("If no folder is provided, current directory is used.\n");
    printf("Output format:\n");
    printf("  full_path,\\tfile_size,\\ttimestamp\n");
    printf("Example:\n");
    printf("  dirbot.exe C:\\Projects > out.txt\n");
}

void print_file_info(const char *path, WIN32_FIND_DATAA *fdata)
{
    FILETIME ftLocal;
    SYSTEMTIME st;
    char datetime[20];
    char fullpath[MAX_PATH];
    char formatted_path[MAX_PATH * 2];

    snprintf(fullpath, MAX_PATH, "%s\\%s", path, fdata->cFileName);

    // Convert UTC FILETIME to local time
    FileTimeToLocalFileTime(&fdata->ftLastWriteTime, &ftLocal);
    FileTimeToSystemTime(&ftLocal, &st);
    snprintf(datetime, sizeof(datetime), "%04d-%02d-%02d %02d:%02d:%02d",
             st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

    LARGE_INTEGER filesize;
    filesize.HighPart = fdata->nFileSizeHigh;
    filesize.LowPart = fdata->nFileSizeLow;

    // Wrap path in quotes if it contains a comma
    if (strchr(fullpath, ','))
    {
        snprintf(formatted_path, sizeof(formatted_path), "\"%s\"", fullpath);
    }
    else
    {
        snprintf(formatted_path, sizeof(formatted_path), "%s", fullpath);
    }

    printf("%s,\t%lld,\t%s\n", formatted_path, filesize.QuadPart, datetime);
}

void walk_directory(const char *base)
{
    WIN32_FIND_DATAA fdata;
    char search[MAX_PATH];
    snprintf(search, MAX_PATH, "%s\\*", base);

    HANDLE hFind = FindFirstFileA(search, &fdata);
    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do {
        if (strcmp(fdata.cFileName, ".") == 0 || strcmp(fdata.cFileName, "..") == 0)
            continue;

        char nextpath[MAX_PATH];
        snprintf(nextpath, MAX_PATH, "%s\\%s", base, fdata.cFileName);

        if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            walk_directory(nextpath);
        } else {
            print_file_info(base, &fdata);
        }

    } while (FindNextFileA(hFind, &fdata));

    FindClose(hFind);
}

int main(int argc, char *argv[])
{
    char start[MAX_PATH];

    if (argc > 1)
    {
        if (
            strcmp(argv[1], "-h") == 0 ||
            strcmp(argv[1], "--help") == 0 ||
            strcmp(argv[1], "/?") == 0)
        {
            print_usage();
            return 0;
        }

        // Handle "\" by converting to current drive root
        if (strcmp(argv[1], "\\") == 0)
        {
            char drive[MAX_PATH];
            if (_getcwd(drive, MAX_PATH))
            {
                drive[2] = '\0'; // keep only "C:"
                snprintf(start, MAX_PATH, "%s\\", drive);
            }
            else
            {
                fprintf(stderr, "Error: Failed to determine current drive.\n");
                return 1;
            }
        }
        else
        {
            strncpy(start, argv[1], MAX_PATH - 1);
            start[MAX_PATH - 1] = '\0';
        }

        DWORD attr = GetFileAttributesA(start);
        if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY))
        {
            fprintf(stderr, "Error: '%s' is not a valid directory.\n", argv[1]);
            return 1;
        }
    }
    else
    {
        GetCurrentDirectoryA(MAX_PATH, start);
    }

    walk_directory(start);
    return 0;
}
