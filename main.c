#include "cJSON.h"
#include "utilities/create.c"
#include "utilities/splitstrings.c"
#include "utilities/record.c"
#include "utilities/structparse.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int help() {
    printf("\nPolygonDB Commands\n\n");

    printf("help - Displays all PolygonDB commands\n\n");
    printf("File Manipulation Commands\n");
    printf("init (name) - Creates a json database folder\n");

    printf("\n\n");

    return 0;

}


int main() {
    char input[9999];
    int count = 0;

    while(1){

        //Beginning of Terminal
        printf(">>> ");
        fgets(input, sizeof(input), stdin);
        int length = strlen(input);
        if (length > 0 && input[length - 1] == '\n') {
            input[length - 1] = '\0';
        }

        cJSON* root = cJSON_Parse(input);
        //Checks if input can be parsed.
        if (root != NULL) {
            
            Input inputdata;
            memset(&inputdata, 0, sizeof(inputdata));

            parseInputJson(input, &inputdata);

            printf("Dbname: %s\n", inputdata.Dbname);
            printf("Loc: %s\n", inputdata.Loc);
            printf("Act: %s\n", inputdata.Act);
            printf_s("Value: %f\n", *((double*)inputdata.Val));

            free((void*)inputdata.Dbname);
            free((void*)inputdata.Loc);
            free((void*)inputdata.Act);
            free(inputdata.Val);
            
        } else {

            char **result = fields(input, &count);

            if(strcmp(input, "help") == 0){

                //Shows command options
                help();

            } else if (strcmp(input, "create") == 0 && count >= 2) {

                //Creates json file
                create(result[1]);
            
            }

            //free result
            for (int i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
        }

    }

    free(input);

   return 0;
}
