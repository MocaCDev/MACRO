#ifndef WRAPPERS
#define WRAPPERS

#define MAX_FIELDS    10

enum WRAPPER_ID {
    Var_Wrapper
};

typedef struct WrapperInfo
{
    struct {
        enum WRAPPER_ID id;
        char *fields[MAX_FIELDS];
        void *function_call;
    };
} _WrapperInfo;

void VarWrapper() {
    
}

_WrapperInfo info[] = {
    {
        .id = Var_Wrapper,
        .fields = {(char*)"bro", (char*)"dude"},
        .function_call = (void *)VarWrapper
    }
};

#endif