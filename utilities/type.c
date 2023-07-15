typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_OBJECT
} ValueType;

typedef struct {
    const char* Dbname;
    const char* Loc;
    const char* Act;
    const int* Row;
    void* Val;
    ValueType ValType;
} Input;