#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON.h"


cJSON *createValue(void *val, ValueType valType) {
    switch (valType) {
        case TYPE_INT:
            return cJSON_CreateNumber(*(int *)val);
        case TYPE_DOUBLE:
            return cJSON_CreateNumber(*(double *)val);
        case TYPE_BOOL:
            return cJSON_CreateBool(*(int *)val);
        case TYPE_STRING:
            return cJSON_CreateString((char *)val);
        default:
            return NULL;
    }
}

void setValue(cJSON *json, const char *location, void *val, ValueType valType) {
    cJSON *parent = json;

    char *path = strdup(location);
    char *token = strtok(path, ".");
    cJSON *child = parent;
    char *lastToken = token;

    if (child != NULL) {
        // Update the value of an existing variable at the specified location
        switch (valType) {
            case TYPE_INT:
                cJSON_ReplaceItemInObject(parent, lastToken, cJSON_CreateNumber(*(int *)val));
                break;
            case TYPE_DOUBLE:
                cJSON_ReplaceItemInObject(parent, lastToken, cJSON_CreateNumber(*(double *)val));
                break;
            case TYPE_BOOL:
                cJSON_ReplaceItemInObject(parent, lastToken, cJSON_CreateBool(*(int *)val));
                break;
            case TYPE_STRING:
                cJSON_ReplaceItemInObject(parent, lastToken, cJSON_CreateString((char *)val));
                break;
            case TYPE_ARRAY:
                cJSON_ReplaceItemInObject(parent, lastToken, cJSON_CreateString((char *)val));
                break;
            case TYPE_OBJECT:
                cJSON_ReplaceItemInObject(parent, lastToken, cJSON_CreateString((char *)val));
                break;
            default:
                break;
        }
    } else {
        // Add a new variable to the JSON at the specified location
        cJSON_AddItemToObject(parent, lastToken, createValue(val, valType));
    }

    free(path);
}



void record(const char *dbname, const char *location, void *val, ValueType valType) {
    // Step 1: Check if the file exists
    char filepath[100];
    sprintf(filepath, "databases/%s.json", dbname);
    FILE *file = fopen(filepath, "r+");
    if (file == NULL) {
        printf("Error: File '%s' does not exist.\n", filepath);
        return;
    }

    // Step 2: Check if the file is empty
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size == 0) {
        // File is empty, create a new JSON object
        cJSON *json = cJSON_CreateObject();
        setValue(json, location, val, valType);

        // Write the JSON to the file
        char *jsonStr = cJSON_Print(json);
        fseek(file, 0, SEEK_SET);
        fwrite(jsonStr, strlen(jsonStr), 1, file);
        free(jsonStr);

        cJSON_Delete(json);
    } else {
        // File is not empty, read the existing JSON data
        fseek(file, 0, SEEK_SET);
        char *fileContent = (char *)malloc(size + 1);
        fread(fileContent, size, 1, file);
        fileContent[size] = '\0';

        // Parse the existing JSON data
        cJSON *json = cJSON_Parse(fileContent);
        free(fileContent);

        // Step 3: Update the JSON with new data
        setValue(json, location, val, valType);
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
        return "Record Success!";
    }

    fclose(file);

}