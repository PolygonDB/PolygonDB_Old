#include "cJSON.h"
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

    printf("Nikhil's Terminal\n\n");
    printf("Welcome To Nikhil's Terminal\n");
    printf("Type \"help\" for more things!\n\n");

    while(1){

        printf(">>> ");
        fflush(stdin);
        scanf("%s", a);

        if(strcmp(a, "help") == 0){

        help();

        }

    }

   return 0;
}
