#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON.h"

void appened(const char *dbname,void *val ){
    const cJSON *valson = cJSON_Parse(val);
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
    // add the value to the rows array
    cJSON *rows = cJSON_GetObjectItemCaseSensitive(json, "rows");
    if (rows == NULL) {
        printf("Error: Rows not found.\n");
        return;
    }
    cJSON_AddItemToArray(rows, valson);
    // print the updated JSON
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
        fwrite(jsonStr, strlen(jsonStr), 1, file);
        free(jsonStr);
        cJSON_Delete(json);
        fclose(file);
    printf("Row Added!\n");
    return "Row Added!";
}