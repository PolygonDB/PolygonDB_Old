#include "cJSON.h"
#include "utilities/create.c"
#include <stdio.h>
#include <string.h>

int help() {
    printf("\nPolygonDB Commands\n\n");

    printf("help - Displays all PolygonDB commands\n\n");
    printf("File Manipulation Commands\n");
    printf("init (name) - Creates a json database folder\n");

    printf("\n\n");

    return 0;

}


int main() {
    char a[100];

    while(1){

        printf(">>> ");
        fflush(stdin);
        scanf("%s", a);

        if(strcmp(a, "help") == 0){

        help();

        } else if (strcmp(a, "create") == 0) {
            create();
        }

    }

   return 0;
}
