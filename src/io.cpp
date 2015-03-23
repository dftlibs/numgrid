

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#include "io.h"


namespace io
{
    print_function stdout_function = &default_stdout_function;
    print_function stderr_function = &default_stderr_function;

    int default_stdout_function(const char* line)
    {
        fprintf(stdout, "%s", line);
        return 0;
    }


    int default_stderr_function(const char* line)
    {
        fprintf(stderr, "%s", line);
        return 1;
    }

    void set_stdout_function(print_function fun)
    {
        stdout_function = fun;
    }


    void set_stderr_function(print_function fun)
    {
        stderr_function = fun;
    }


    void speak_your_mind(const char *format, ...)
    {
        va_list args;
        va_start(args, format);

        char buffer[1000];

        int need;
        int n = vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        (*stdout_function)(buffer);
    }
}
