#ifndef STACK_H_INCLUDE

#define STACK_H_INCLUDE

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ProtectionLevels.h"

typedef int stack_element_t;

const size_t EXPAND_MEMORY_COEF = 2;
__attribute__((unused)) static stack_element_t Poison = 666;

struct Stack_t
{
    IF_CANARY_LEVEL_PROTECTION
    (
        canary_t left_struct_canary = 0;

        const char *name = nullptr;

        int error = 0;
        int status = 0;
    )

    size_t capacity = 0;
    size_t size = 0;
    stack_element_t *data = nullptr;

    IF_CANARY_LEVEL_PROTECTION
    (
        canary_t right_struct_canary = 0;
    )

    IF_HASH_LEVEL_PROTECTION
    (
        unsigned int array_hash = 0;
        unsigned int stack_hash = 0;
    )
};

void StackCtor(Stack_t *stack, int capacity);

void MemoryCtor(Stack_t *stack, void *new_memory);

void StackDtor(Stack_t *stack);

void StackPush(Stack_t *stack,  stack_element_t value);

stack_element_t StackPop(Stack_t *stack);

stack_element_t StackTop(Stack_t *stack);

void StackMemoryRealloc(Stack_t *stack);

void StackBackwardMemoryRealloc(Stack_t *stack);

void StackDump(FILE* out, Stack_t *stack, location_t location);

void PrintArray(Stack_t *stack);

#endif
