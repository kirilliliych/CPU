#ifndef PROTECTIONLEVELS_H

#define PROTECTIONLEVELS_H

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Location.h"

#define CANARY_LEVEL_PROTECTION

#ifdef HASH_LEVEL_PROTECTION

#undef NO_PROTECTION
#define CANARY_LEVEL_PROTECTION
#define IF_HASH_LEVEL_PROTECTION(code) code

#else

#define IF_HASH_LEVEL_PROTECTION(code)

#endif

#ifdef CANARY_LEVEL_PROTECTION

#undef NO_PROTECTION
#define ASSERT_OK(stack) if (IsValid(stack))                                                                        \
                         {                                                                                          \
                             location_t location = __LOCATION__;                                                    \
                             StackDump(logs, stack, location);                                                      \
                             printf("ERROR: exiting programme, check logs.txt");                                    \
                             abort();                                                                               \
                         }

#define STACK_CONSTRUCT(stack, capacity) stack.name = #stack;                                                       \
                                         StackCtor(&stack, capacity)

#define IF_CANARY_LEVEL_PROTECTION(code) code

#else

#define IF_CANARY_LEVEL_PROTECTION(code)

#endif

#ifdef NO_PROTECTION

#undef HASH_LEVEL_PROTECTION
#undef CANARY_LEVEL_PROTECTION

#define ASSERT_OK(stack)
#define STACK_CONSTRUCT(stack, capacity) StackCtor(&stack, capacity)
#define IF_NO_PROTECTION(code) code

#else

#define IF_NO_PROTECTION(code)

#endif

IF_CANARY_LEVEL_PROTECTION
(
    typedef unsigned long long canary_t;
    const canary_t Canary = 0xBADF00DDEADBEAF;
)

#endif
