// standard c libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// C libs
#include "cJSON.h"
#include "wslib/include/ws.h"
#define WS_IMPLEMENTATION


// PolygonDB libs
#include "utilities/type.c"
#include "utilities/create.c"
#include "utilities/splitstrings.c"
#include "utilities/record.c"
#include "utilities/structparse.c"
#include "utilities/search.c"
#include "utilities/remove.c"
#include "utilities/appened.c"


// int help() {
//     printf("\nPolygonDB Commands\n\n");

//     printf("help - Displays all PolygonDB commands\n\n");
//     printf("File Manipulation Commands\n");
//     printf("create (name) - Creates a json file\n\n");
//     printf("version - Displays the current version of PolygonDB\n\n");
//     printf("exit - Exits PolygonDB\n\n");

//     printf("\n\n");

//     return 0;

// }

void onopen(ws_cli_conn_t *client){
	((void)client);
	printf("Connected!\n");
}

void onmessage(ws_cli_conn_t *client,
	const unsigned char *wsmsg, uint64_t size, int type)
{
	// ((void)client);
	// ((void)wsmsg);
	// ((void)size);
	// ((void)type);

    // update this version number every time you make a new commit and note the PULL NUMBER ex ACTIVE VERSION|BUILD-ACTIVE VERSION.GHP#!
    const VERSION = "0.0.2|BUILD-0.0.2.GHP7";
    // set the msg to be a clone of the wsmsg
    char msg[size + 1];
    memcpy(msg, wsmsg, size);
    msg[size] = '\0';

    printf("Received message: %s\n", msg);
    // fgets(msg, sizeof(msg), stdin);
    //     int length = strlen(msg);
    //     if (length > 0 && msg[length - 1] == '\n') {
    //         msg[length - 1] = '\0';
    //     }
        cJSON* root = cJSON_Parse(msg);
        //Checks if input can be parsed.
        if (root != NULL) {
            
            Input inputdata;
            memset(&inputdata, 0, sizeof(inputdata));

            parseInputJson(msg, &inputdata);
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
            char *result = malloc(100);
            printf("Dbname: %s\n", inputdata.Dbname);
            printf("Loc: %s\n", inputdata.Loc);
            printf("Act: %s\n", inputdata.Act);
            if(strcmp(inputdata.Act, "record") == 0) {
                result = record(inputdata.Dbname, inputdata.Loc, inputdata.Val, inputdata.ValType);
            }else if(strcmp(inputdata.Act, "search") == 0) {
                result = search(inputdata.Dbname, inputdata.Val, inputdata.Loc,inputdata.ValType);
            }else if(strcmp(inputdata.Act, "remove") == 0){
                result = removeRow(inputdata.Dbname, inputdata.Row);
            }else if (strcmp(inputdata.Act, "appened") == 0) {
                result = appened(inputdata.Dbname,inputdata.Val);
            }
            printf("Sending Result: %s\n", result);
            ws_sendframe_txt(client, result);
            printf("Result Sent!\n");
            // Clean up allocated memory
            // free(result);
            free((void*)inputdata.Dbname);
            free((void*)inputdata.Loc);
            free((void*)inputdata.Act);
            free(inputdata.Val);
            
        } else {
            printf("Invalid input.\n");
        }
            // free(msg);
            printf("waiting for new msg. . .\n");
}

void onclose(ws_cli_conn_t *client)
{
	((void)client);
	printf("Disconnected!\n");
}

int main(void) {        
	struct ws_events evs;
	evs.onopen    = &onopen;
	evs.onclose   = &onclose;
	evs.onmessage = &onmessage;
	ws_socket(&evs, 8000, 1, 1000);

	/*
	 * Periodically send ping frames in the main thread
	 * and aborts inactive connections.
	 */
	while (1)
	{

		/* Sleep 100 seconds. */
		sleep(100);
	}
   return 0;
}
