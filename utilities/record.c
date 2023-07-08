#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON.c"


void record(char *db) {
    // Specify the file path
    char file_path[100];
    sprintf(file_path, "databases/%s.json", db);

    // Open the file in read mode
    FILE* file = fopen(file_path, "r");

    // Check if the file exists
    if (file == NULL) {
        printf("File does not exist or cannot be opened.\n");
        return;
    }

    // Check if the file is empty
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size == 0) {
        printf("File is empty.\n");
        fclose(file);
        return;
    }

    // Reset the file position indicator to the beginning of the file
    rewind(file);

    // Read the contents of the file into a buffer
    char* file_contents = (char*)malloc(file_size + 1);
    if (file_contents == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    size_t read_size = fread(file_contents, 1, file_size, file);
    fclose(file);

    // Parse the file contents using cJSON
    cJSON* root = cJSON_Parse(file_contents);
    if (root == NULL) {
        printf("Failed to parse JSON.\n");
        free(file_contents);
        return;
    }

    // Access and modify the JSON data as needed
    cJSON* name = cJSON_GetObjectItemCaseSensitive(root, "name");
    if (cJSON_IsString(name)) {
        cJSON_ReplaceItemInObjectCaseSensitive(root, "name", cJSON_CreateString("Jane Doe"));
    }

    cJSON* age = cJSON_GetObjectItemCaseSensitive(root, "age");
    if (cJSON_IsNumber(age)) {
        cJSON_SetNumberValue(age, 30);
    }

    cJSON* hobbies = cJSON_GetObjectItemCaseSensitive(root, "hobbies");
    if (cJSON_IsArray(hobbies)) {
        cJSON* newHobby = cJSON_CreateString("Painting");
        cJSON_AddItemToArray(hobbies, newHobby);
    }

    // Convert cJSON object to a JSON string
    char* modifiedJsonString = cJSON_Print(root);

    // Open the file in write mode
    file = fopen(file_path, "w");
    if (file == NULL) {
        printf("File cannot be opened for writing.\n");
        cJSON_Delete(root);
        free(file_contents);
        free(modifiedJsonString);
        return;
    }

    // Write the modified JSON string to the file
    fprintf(file, "%s", modifiedJsonString);
    fclose(file);

    // Clean up memory
    cJSON_Delete(root);
    free(file_contents);
    free(modifiedJsonString);

    printf("File successfully updated.\n");
}