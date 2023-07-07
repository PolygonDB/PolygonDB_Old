#include "cJSON.h"
#include "utilities/create.c"
#include "utilities/splitstrings.c"
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
    char input[100];
    int count = 0;

    while(1){

        //Beginning of Terminal
        printf(">>> ");
        fgets(input, sizeof(input), stdin);
        int length = strlen(input);
        if (length > 0 && input[length - 1] == '\n') {
            input[length - 1] = '\0';
        }

        //Parsing
        char **result = fields(input, &count);
        printf("%d\n", count);

        for (int i = 0; i < count; i++) {
            printf("%s\n", result[i]);
            free(result[i]);
        }
        free(result);


        //Searching for Appriopriate Command
        if(strcmp(input, "help") == 0){

            help();

        } else if (strcmp(input, "create") == 0) {
            
            create();
        
        }

    }

   return 0;
}
