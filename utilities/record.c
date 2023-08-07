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

void setValue(cJSON *json, const int *row, const char *location, void *val, ValueType valType) {
   // step one go to the targeted row
   cJSON *rows = cJSON_GetObjectItemCaseSensitive(json, "rows");
    if (rows == NULL) {
         rows = cJSON_AddArrayToObject(json, "rows");
    }
    cJSON *rowItem = cJSON_GetArrayItem(rows, *row);
    // chcek if the row exists
    if (rowItem == NULL) {
        rowItem = cJSON_CreateObject();
        cJSON_AddItemToArray(rows, rowItem);
    }
    printf("rowItem: %s\n", cJSON_Print(rowItem));
    // step two go to the targeted location
    cJSON *locationItem = cJSON_GetObjectItemCaseSensitive(rowItem, location);
    if (locationItem == NULL) {
        locationItem = cJSON_AddObjectToObject(rowItem, location);
    }
    // step three set the value
    cJSON *valItem = createValue(val, valType);
    if (valItem == NULL) {
        return;
    }
    cJSON_ReplaceItemInObject(locationItem, location, valItem);
    return;
}



char *record(const char *dbname, const char *location, const int *row, void *val, ValueType valType) {
    // Step 1: Check if the file exists
    char filepath[100];
    sprintf(filepath, "databases/%s.json", dbname);
    FILE *file = fopen(filepath, "r+");
    if (file == NULL) {
         return *"{'error': File '%s' does not exist.'}", filepath;
    }

    // Step 2: Check if the file is empty
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size == 0) {
        // File is empty, create a new JSON object
        cJSON *json = cJSON_CreateObject();
        setValue(json, row, location, val, valType);

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
        printf("Setting value...\n");
        setValue(json, row, location, val, valType);
// empty the file contents to prepare for writing
    fclose(file);
    file = fopen(filepath, "w");
    if (file == NULL) {
        return *"{'error': Failed to open file for writing.'}";
    }
    // Write the updated JSON back to the file
        fseek(file, 0, SEEK_SET);
        char *jsonStr = cJSON_Print(json);
        // printf("jsonStr: %s\n", jsonStr);
        fwrite(jsonStr, strlen(jsonStr), 1, file);
        free(jsonStr);
        cJSON_Delete(json);
        fclose(file);
        return "{'msg':'Record Success!'}";
    }

    fclose(file);

}