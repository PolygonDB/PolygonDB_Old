#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../cJSON.h"


void search(const char *dbname,void *val, const char *key, ValueType valType ){
printf("Searching for %s in %s\n", val, dbname);
char filepath[100];
    sprintf(filepath, "databases/%s.json", dbname);
    FILE *file = fopen(filepath, "r+");
    if (file == NULL) {
        printf("Error: File '%s' does not exist.\n", filepath);
        return;
    }else{
        printf("File exists! Scanning for Val.\n");
        
    }
return;
}