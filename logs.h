#ifndef LOGS_H

#define LOGS_H

static FILE *logs = stderr;
static const int LOG_FILE_NULLPTR = -1;

int PrintToLogs(const char *format ...);

void CloseLogs();

#endif
