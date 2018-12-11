#include "showstack.hpp"

using namespace std;

StackPrinter *StackPrinter::instance = 0;
struct backtrace_state *StackPrinter::pbs = 0;

StackPrinter *StackPrinter::getInstance()
{
    if( 0 == instance )
    {
        instance = new StackPrinter();
    }
    return instance;
}

bool StackPrinter::startup(const char *exename)
{
    StackPrinter::pbs = backtrace_create_state(exename, 1, callbackError, NULL);
    if(!pbs)
        return false;
    return true;
}

int StackPrinter::printStackTrace(const char *str)
{
    if( "" == str)
        str = "something";
    logError(" > > > > > > > > > > catch %s, call chain begin",str);
    backtrace_full(pbs,0,callback,NULL,NULL);
    logError(" < < < < < < < < < < call chain end");
}

string StackPrinter::CxxDemangle(const char* name)
{
    char buffer[1024] = {0};  
    size_t size = sizeof(buffer);  
    int status;  
    char *ret;
    try 
    {
        ret =  abi::__cxa_demangle(name, buffer, &size, &status);
        if(ret) 
        {  
            return std::string(ret);
        } 
        else 
        {  
            return name;
        }  
    }
    catch(...) 
    {
            return name;
    }
        return name;
}

void StackPrinter::callbackError(void *data, const char *msg, int errnum)
{
    logError("some error in calling libbacktrace");
}

void StackPrinter::callbackSymbol(void *data, uintptr_t pc,const char *symname, uintptr_t symval,uintptr_t symsize)
{
    if(!symname)
    {
        logError(" [%p] in [unknow]",pc);
    }
    else
    {
        logError(" [%p] in %s",pc,symname);
    }
}

int StackPrinter::callback(void *data, uintptr_t pc,const char *filename, int lineno,const char *function)
{
    if( !filename || !function )
    {
        backtrace_syminfo(pbs, pc, callbackSymbol, callbackError, NULL); 
    }
    else
    {
        logError(" [%p] in %s at %s : line %d",pc,StackPrinter::CxxDemangle(function).c_str(),filename,lineno);
    }
    return 0; 
}

