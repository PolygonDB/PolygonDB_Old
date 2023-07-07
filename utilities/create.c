#include <stdio.h>
#include <sys/stat.h>

int create(char *name) {
    printf(name);

    const char* folderName = "databases";
    struct stat st;

    // Check if folder exists
    if (stat(folderName, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            printf("Folder exists.\n");
        }
        else {
            printf("A file with the same name already exists.\n");
        }
    }
    else {
        // Create the folder
        if (mkdir(folderName) == 0) {
            printf("Folder created.\n");
        }
        else {
            printf("Failed to create folder.\n");
        }
    }

    return 0;
}