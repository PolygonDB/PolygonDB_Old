#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON.h"

void removeRow(const char *dbname,const int *row){
    printf("Removing Row: %d\n", row);
char filepath[100];
    sprintf(filepath, "databases/%s.json", dbname);
    FILE *file = fopen(filepath, "r+");
    if (file == NULL) {
        printf("Error: File '%s' does not exist.\n", filepath);
        return;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size == 0) {
        printf("Error: File '%s' is empty.\n", filepath);
        return;
    }

    // File is not empty, read the existing JSON data
        fseek(file, 0, SEEK_SET);
        char *fileContent = (char *)malloc(size + 1);
        fread(fileContent, size, 1, file);
        fileContent[size] = '\0';

        // Parse the existing JSON data
        cJSON *json = cJSON_Parse(fileContent);
        free(fileContent);

    // find the row in the rows array
    cJSON *rows = cJSON_GetObjectItemCaseSensitive(json, "rows");
    if (rows == NULL) {
        printf("Error: Rows not found.\n");
        return;
    }
    // check if row is in bounds
    if (row < 0 || row >= cJSON_GetArraySize(rows)) {
        printf("Error: Row index out of bounds.\n");
        return;
    }
    cJSON_DeleteItemFromArray(rows, row);
    
    // empty the file contents to prepare for writing
    fclose(file);
    file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Error: Failed to open file for writing.\n");
        return;
    }
    // Write the updated JSON back to the file
        fseek(file, 0, SEEK_SET);
        char *jsonStr = cJSON_Print(json);
        // printf("jsonStr: %s\n", jsonStr);
        fwrite(jsonStr, strlen(jsonStr), 1, file);
        free(jsonStr);
        cJSON_Delete(json);
        fclose(file);
    printf("Row deleted!\n");
    return "Row deleted!";
}