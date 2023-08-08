#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON.h"

char *appened(const char *dbname,void *val ){
    const cJSON *valson = cJSON_Parse(val);
char filepath[100];
    sprintf(filepath, "databases/%s.json", dbname);
    FILE *file = fopen(filepath, "r+");
    if (file == NULL) {
        return *"{\"error\":\"File '%s' does not exist.\"}", filepath;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size == 0) {
        return *"{\"error\":\"Error: File '%s' is empty.\"}", filepath;
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
         return *"{\"error\": Rows not found.\"}";
    }
    cJSON_AddItemToArray(rows, valson);
    // print the updated JSON
    // empty the file contents to prepare for writing
    fclose(file);
    file = fopen(filepath, "w");
    if (file == NULL) {
         return *"{\"error\": Failed to open file for writing.\"}";
    }
    // Write the updated JSON back to the file
        fseek(file, 0, SEEK_SET);
        char *jsonStr = cJSON_Print(json);
        fwrite(jsonStr, strlen(jsonStr), 1, file);
        free(jsonStr);
        cJSON_Delete(json);
        fclose(file);
    printf("Row Added!\n");
    return "{'msg':'Success!\"}";
}