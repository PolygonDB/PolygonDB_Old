#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON.h"

typedef struct {
    const char* Dbname;
    const char* Loc;
    const char* Act;
    void* Val;
} Input;

void sassignJsonValue(cJSON* jsonValue, void** outputValue) {
    if (cJSON_IsBool(jsonValue)) {
        *outputValue = malloc(sizeof(int));
        *((int*)*outputValue) = cJSON_IsTrue(jsonValue) ? 1 : 0;
    } else if (cJSON_IsNumber(jsonValue)) {
        *outputValue = malloc(sizeof(double));
        *((double*)*outputValue) = jsonValue->valuedouble;
    } else if (cJSON_IsString(jsonValue)) {
        *outputValue = strdup(jsonValue->valuestring);
    } else if (cJSON_IsArray(jsonValue)) {
        // Handling array values depends on your specific requirements
        // You can choose to store them as an array or any other appropriate data structure
        // This example shows storing the first element of the array as a string
        cJSON* firstElement = cJSON_GetArrayItem(jsonValue, 0);
        if (cJSON_IsString(firstElement)) {
            *outputValue = strdup(firstElement->valuestring);
        }
    } else if (cJSON_IsObject(jsonValue)) {
        // Handling object values depends on your specific requirements
        // You can choose to store them as a nested struct or any other appropriate data structure
        // This example shows storing the first key-value pair as two separate strings
        cJSON* child = jsonValue->child;
        if (child != NULL && cJSON_IsString(child) && cJSON_IsString(child->next)) {
            *outputValue = malloc(sizeof(char*) * 2);
            *((char**)*outputValue) = strdup(child->valuestring);
            *((char**)(*outputValue + sizeof(char*))) = strdup(child->next->valuestring);
        }
    }
    // You can add additional logic for other value types as needed
}

void parseInputJson(const char* jsonString, Input* input) {
    // Parse the JSON string
    cJSON* root = cJSON_Parse(jsonString);
    if (root == NULL) {
        printf("Failed to parse JSON.\n");
        return;
    }

    cJSON* dbname = cJSON_GetObjectItemCaseSensitive(root, "dbname");
    if (cJSON_IsString(dbname)) {
        input->Dbname = strdup(dbname->valuestring);
    }

    cJSON* loc = cJSON_GetObjectItemCaseSensitive(root, "location");
    if (cJSON_IsString(loc)) {
        input->Loc = strdup(loc->valuestring);
    }

    cJSON* act = cJSON_GetObjectItemCaseSensitive(root, "action");
    if (cJSON_IsString(act)) {
        input->Act = strdup(act->valuestring);
    }

    cJSON* val = cJSON_GetObjectItemCaseSensitive(root, "value");
    if (val != NULL) {
        sassignJsonValue(val, &input->Val);
    }

    // Clean up cJSON
    cJSON_Delete(root);
}

int main2() {
    const char* jsonString = "{\"dbname\":\"CatoDB\",\"location\":\"rows.0.age\",\"action\":\"record\",\"value\":5.1}";

    Input input;
    memset(&input, 0, sizeof(Input));

    parseInputJson(jsonString, &input);

    // Print the values in the Input struct
    printf("Dbname: %s\n", input.Dbname);
    printf("Loc: %s\n", input.Loc);
    printf("Act: %s\n", input.Act);
    printf_s("Value: %f\n", *((double*)input.Val));

    // Clean up allocated memory
    free((void*)input.Dbname);
    free((void*)input.Loc);
    free((void*)input.Act);
    free(input.Val);

    return 0;
}
