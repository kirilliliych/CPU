#include <stdarg.h>
#include <stdio.h>
#include "logs.h"

int PrintToLogs(const char *format ...)
{
    if (logs == nullptr)
    {
        return LOG_FILE_NULLPTR;
    }

    va_list arg_ptr;
    va_start(arg_ptr, format);

    vfprintf(logs, format, arg_ptr);

    va_end(arg_ptr);

    fflush(logs);

    return 0;

}

void CloseLogs()
{
    if (logs != nullptr)
    {
        fclose(logs);
    }
}
