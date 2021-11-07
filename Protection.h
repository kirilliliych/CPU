#ifndef PROTECTION_H_INCLUDE

#define PROTECTION_H_INCLUDE

#include "ProtectionLevels.h"
#include "Stack.h"
#include "logs.h"

static const int NEGATIVE_SIZE_T = 10000000;

enum Errors
{
    SIZE_OUT_OF_CAPACITY = 1,
    OUT_OF_MEMORY,
    NEGATIVE_SIZE,
    NEGATIVE_CAPACITY,
    STACK_USING_ZERO_CAPACITY,
    NULLPTR_TO_ARRAY,
    NULL_POP,
    EMPTY_TOP_ATTEMPT,
    WRONG_SIZE,
    STACK_IS_DESTRUCTED,
    STACK_IS_NOT_CONSTRUCTED,
    WRONG_LEFT_ARRAY_CANARY,
    WRONG_RIGHT_ARRAY_CANARY,
    WRONG_LEFT_STRUCT_CANARY,
    WRONG_RIGHT_STRUCT_CANARY,
    WRONG_ARRAY_HASH,
    WRONG_STACK_HASH
};

enum StackStatus
{
    DESTRUCTED = 1,
    CONSTRUCTED
};

void StackNullCheck(Stack_t *stack);

void FillingPoison(Stack_t *stack);

int  IsValid(Stack_t *stack);

int UsingStackZeroCapacity(Stack_t *stack);

const char *TextError(Stack_t *stack);

void PlacingCanary(Stack_t *stack, void *temp);

unsigned int CalculatingHash(const void *hash_from, size_t bytes_to_hash);

void PlacingHash(Stack_t *stack);

#endif
