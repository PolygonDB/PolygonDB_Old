#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON.h"


void search(const char *dbname,void *val, const char *key, ValueType valType ){
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
        // check schema for key
        cJSON *schema = cJSON_GetObjectItemCaseSensitive(json, "schema");
        if (schema == NULL) {
            printf("Error: Schema not found.\n");
            return;
        }
        // check schema array for key
        cJSON *schemaItem = NULL;
        cJSON_ArrayForEach(schemaItem, schema) {
            cJSON *keyItem = cJSON_GetObjectItemCaseSensitive(schemaItem, "key");
            if (keyItem == NULL) {
                printf("Error: Key not found in schema.\n");
                return;
            }
            // check if key matches the key we are looking for
            if( strcmp(keyItem->valuestring, key) == 0) {
                break;
            }
        }

        // printf("Passed schema check!\n");
        // loop through the rows array
        cJSON *rows = cJSON_GetObjectItemCaseSensitive(json, "rows");
        if (rows == NULL) {
            printf("Error: Rows not found.\n");
            return;
        }
        cJSON *rowItem = NULL;
        cJSON_ArrayForEach(rowItem, rows) {
            cJSON *valItem = cJSON_GetObjectItemCaseSensitive(rowItem, key);
            if (valItem == NULL) {
                printf("Error: Key not found in row.\n");
                return;
            }
            // check if key matches the key we are looking for
            if( strcmp(valItem->valuestring, val) == 0) {
                // print the row
                char *result = cJSON_Print(rowItem);
                printf("Row: %s\n", result);
                return result;
                break;
            }
        }
}