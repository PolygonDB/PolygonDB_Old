#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int create(char *name) {
    printf(name);

    char folderName[] = "databases";
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


    // Create the file
    char fileName[100];
    sprintf(fileName, "%s/%s.json", folderName, name);

    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Failed to create file.\n");
        return 1;
    }

    printf("File created: %s\n", fileName);

    fclose(file);
    
    return 0;
}