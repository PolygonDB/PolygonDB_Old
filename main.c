#include "utilities/type.c"
#include "cJSON.h"
#include "utilities/create.c"
#include "utilities/splitstrings.c"
#include "utilities/record.c"
#include "utilities/structparse.c"
#include "utilities/search.c"
#include "utilities/remove.c"
#include "utilities/appened.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int help() {
    printf("\nPolygonDB Commands\n\n");

    printf("help - Displays all PolygonDB commands\n\n");
    printf("File Manipulation Commands\n");
    printf("create (name) - Creates a json file\n\n");
    printf("version - Displays the current version of PolygonDB\n\n");
    printf("exit - Exits PolygonDB\n\n");

    printf("\n\n");

    return 0;

}


int main() {
    char input[9999];
    int count = 0;
    // update this version number every time you make a new release and note the PULL NUMBER!
    const VERSION = "0.0.1|BUILD-0.0.1.GHP6";
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

            if(inputdata.ValType != NULL) {
            switch (inputdata.ValType) {
                case TYPE_INT:
                    printf("Value (int): %d\n", *((int*)inputdata.Val));
                    break;
                case TYPE_DOUBLE:
                    printf("Value (double): %f\n", *((double*)inputdata.Val));
                    break;
                case TYPE_BOOL:
                    printf("Value (bool): %s\n", *((int*)inputdata.Val) ? "true" : "false");
                    break;
                case TYPE_STRING:
                    printf("Value (string): %s\n", (char*)inputdata.Val);
                    break;
                case TYPE_ARRAY:
                    printf("Value (array):%s\n", (char*)inputdata.Val);
                    break;
                case TYPE_OBJECT:
                    printf("Value (object): %s\n", (char*)inputdata.Val);
                    break;
                default:
                    printf("Unknown value type.\n");
            }
            }
            if(strcmp(inputdata.Act, "record") == 0) {
                record(inputdata.Dbname, inputdata.Loc, inputdata.Val, inputdata.ValType);
            }if(strcmp(inputdata.Act, "search") == 0) {
                search(inputdata.Dbname, inputdata.Val, inputdata.Loc,inputdata.ValType);
            }if(strcmp(inputdata.Act, "remove") == 0){
                removeRow(inputdata.Dbname, inputdata.Row);
            }if (strcmp(inputdata.Act, "appened") == 0) {
                appened(inputdata.Dbname,inputdata.Val);
            }
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
            
            }else if(strcmp(input, "version") == 0){
                    //Shows version
                    printf("PolygonDB Version %s\n", VERSION);
            }else if(strcmp(input, "exit") == 0){
                //Exits program
                printf("Exiting...\n");
                break;
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
