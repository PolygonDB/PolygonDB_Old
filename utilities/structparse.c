#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON.c"



void sassignJsonValue(cJSON* jsonValue, void** outputValue, ValueType* outputType) {
    if (cJSON_IsNumber(jsonValue)) {
        if (jsonValue->valuedouble == (double)jsonValue->valueint) {
            *outputType = TYPE_INT;
            *outputValue = malloc(sizeof(int));
            *((int*)*outputValue) = jsonValue->valueint;
        } else {
            *outputType = TYPE_DOUBLE;
            *outputValue = malloc(sizeof(double));
            *((double*)*outputValue) = jsonValue->valuedouble;
        }
    } else if (cJSON_IsBool(jsonValue)) {
        *outputType = TYPE_BOOL;
        *outputValue = malloc(sizeof(int));
        *((int*)*outputValue) = cJSON_IsTrue(jsonValue) ? 1 : 0;
    } else if (cJSON_IsString(jsonValue)) {
        *outputType = TYPE_STRING;
        *outputValue = strdup(jsonValue->valuestring);
    } else if (cJSON_IsArray(jsonValue)) {
        *outputType = TYPE_ARRAY;
        *outputValue = cJSON_Print(jsonValue);
    } else if (cJSON_IsObject(jsonValue)) {
        *outputType = TYPE_OBJECT;
        *outputValue = cJSON_Print(jsonValue);
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
        sassignJsonValue(val, &input->Val, &input->ValType);
    }

    // Clean up cJSON
    cJSON_Delete(root);
}

int ex() {
    const char* jsonString = "{\"dbname\":\"CatoDB\",\"location\":\"rows.0.age\",\"action\":\"record\",\"value\":{\"key\":\"value\"}}";

    Input input;
    memset(&input, 0, sizeof(Input));

    parseInputJson(jsonString, &input);

    // Print the values in the Input struct
    printf("Dbname: %s\n", input.Dbname);
    printf("Loc: %s\n", input.Loc);
    printf("Act: %s\n", input.Act);

    // Determine the variable type of Val
    switch (input.ValType) {
        case TYPE_INT:
            printf("Value (int): %d\n", *((int*)input.Val));
            break;
        case TYPE_DOUBLE:
            printf("Value (double): %f\n", *((double*)input.Val));
            break;
        case TYPE_BOOL:
            printf("Value (bool): %s\n", *((int*)input.Val) ? "true" : "false");
            break;
        case TYPE_STRING:
            printf("Value (string): %s\n", (char*)input.Val);
            break;
        case TYPE_ARRAY:
            printf("Value (array):%s\n", (char*)input.Val);
            break;
        case TYPE_OBJECT:
            printf("Value (object): %s\n", (char*)input.Val);
            break;
        default:
            printf("Unknown value type.\n");
    }

    // Clean up allocated memory
    free((void*)input.Dbname);
    free((void*)input.Loc);
    free((void*)input.Act);
    free(input.Val);

    return 0;
}
