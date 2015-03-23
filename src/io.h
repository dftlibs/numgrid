#ifndef io_h_
#define io_h_

typedef int (*print_function)(const char* line);

namespace io
{
    int default_stdout_function(const char* line);

    int default_stderr_function(const char* line);

    void set_stdout_function(print_function fun);

    void set_stderr_function(print_function fun);

    void speak_your_mind(const char *format, ...);
}

#endif // io_h_
