#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON.h"


char *search(const char *dbname,void *val, const char *key, ValueType valType ){
    printf("Searching for %s in %s\n", val, dbname);
char filepath[100];
    sprintf(filepath, "databases/%s.json", dbname);
    FILE *file = fopen(filepath, "r+");
    if (file == NULL) {
        return *"{\"error\":\"DB does not exist.\"}";
    }
        fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size == 0) {
        return *"{\"error\":\"DB is empty.\"}";
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
        return *"{\"error\":\"Schema not found.\"}";
            
        }
        // check schema array for key
        cJSON *schemaItem = NULL;
        cJSON_ArrayForEach(schemaItem, schema) {
            cJSON *keyItem = cJSON_GetObjectItemCaseSensitive(schemaItem, "key");
            if (keyItem == NULL) {
                return *"{\"error\":\"Key not found in schema.\"}";
                
            }
            // check if key matches the key we are looking for
            if( strcmp(keyItem->valuestring, key) == 0) {
                break;
            }
        }
        // loop through the rows array
        cJSON *rows = cJSON_GetObjectItemCaseSensitive(json, "rows");
        if (rows == NULL) {
            return *"{\"error\":\"Rows not found.\"}";
            return;
        }
        cJSON *rowItem = NULL;
        int index = 0;
        // create a new array to store the rows that match the search
        cJSON *resulte = cJSON_CreateArray();

        cJSON_ArrayForEach(rowItem, rows) {
            cJSON *valItem = cJSON_GetObjectItemCaseSensitive(rowItem, key);
            if (valItem == NULL) {
                return *"{\"error\":\"Key not found in row.\"}";
            }
            // check if key matches the key we are looking for
            if( strcmp(valItem->valuestring, val) == 0) {
                // add the index to the row
                cJSON_AddNumberToObject(rowItem, "Index", index);
                // add the row to the result array
                cJSON_AddItemToArray(resulte, rowItem);
            }
            index++;
        }
                char *result = malloc(100);
                result = cJSON_Print(resulte);
                return *"{rows: %s\n}",result;
}