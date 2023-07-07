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
    char input[1000];
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


        //Searching for Appriopriate Command
        if(strcmp(input, "help") == 0){
            //Shows command options
            help();

        } else if (strcmp(input, "create") == 0 && count >= 2) {
            //Creates json file
            create(result[1]);
        
        }

        
        for (int i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);

    }

   return 0;
}
