#ifndef _SHOWSTACK_H
#define _SHOWSTACK_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <backtrace.h>
#include <cxxabi.h>
#include "all.h"

class StackPrinter
{
public:
    static StackPrinter *getInstance();

    bool startup(const char *exename);

    int printStackTrace(const char *str);

protected:
    StackPrinter(){}

private:
    static std::string CxxDemangle(const char* name);

    static void callbackError(void *data, const char *msg, int errnum);

    static void callbackSymbol(void *data, uintptr_t pc,const char *symname, uintptr_t symval,uintptr_t symsize);

    static int callback(void *data, uintptr_t pc,const char *filename, int lineno,const char *function);

    static struct backtrace_state *pbs;
    
    static StackPrinter *instance;
};

#endif
