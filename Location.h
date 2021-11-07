#ifndef LOCATION_H_INCLUDE

#define LOCATION_H_INCLUDE

#define __LOCATION__ {__FILE__, __func__, __LINE__}

#define _LOCATION_ __FILE__, __func__, __LINE__

struct location_t
{
    const char *file;
    const char *func;
    int line;
};

#endif
