#include "Protection.h"

IF_CANARY_LEVEL_PROTECTION
(
    void StackNullCheck(Stack_t *stack)
    {
        if (stack == nullptr)
        {
            PrintToLogs("Stack (ERROR NULLPTR) [0x000000], file %s line %d function %s\n\n\n", _LOCATION_);

            printf("ERROR: exiting programme, check logs.txt\n");

            abort();
        }
    }

    int UsingStackZeroCapacity(Stack_t *stack)
    {
        if ((stack->data == nullptr) &&
            (stack->capacity == 0)   &&
            (stack->size == 0))
        {
            return STACK_USING_ZERO_CAPACITY;
        }

        return 0;
    }

    int IsValid(Stack_t *stack)
    {
        StackNullCheck(stack);

        if (stack->error != 0)
        {
            return stack->error;
        }
        else
        {
            if (UsingStackZeroCapacity(stack))
            {
                stack->error = STACK_USING_ZERO_CAPACITY;
                return STACK_USING_ZERO_CAPACITY;
            }

            if (stack->status == DESTRUCTED)
            {
                stack->error = STACK_IS_DESTRUCTED;
                return STACK_IS_DESTRUCTED;
            }

            if (stack->status != CONSTRUCTED)
            {
                stack->error = STACK_IS_NOT_CONSTRUCTED;
                return STACK_IS_NOT_CONSTRUCTED;
            }

            if (stack->size >= stack->capacity)
            {
                stack->error = SIZE_OUT_OF_CAPACITY;
                return SIZE_OUT_OF_CAPACITY;
            }

            if (stack->size > NEGATIVE_SIZE_T)
            {
                stack->error = NEGATIVE_SIZE;
                return NEGATIVE_SIZE;
            }

            if (stack->capacity > NEGATIVE_SIZE_T)
            {
                stack->error = NEGATIVE_CAPACITY;
                return NEGATIVE_CAPACITY;
            }

            if (stack->data == nullptr)
            {
                stack->error = NULLPTR_TO_ARRAY;
                return NULLPTR_TO_ARRAY;
            }

            if (*(((canary_t *) stack->data) - 1) != Canary)
            {
                stack->error = WRONG_LEFT_ARRAY_CANARY;
                return WRONG_LEFT_ARRAY_CANARY;
            }

            if (*((canary_t *) (stack->data + stack->capacity)) != Canary)
            {
                stack->error = WRONG_RIGHT_ARRAY_CANARY;
                return WRONG_RIGHT_ARRAY_CANARY;
            }

            if (stack->left_struct_canary != Canary)
            {
                stack->error = WRONG_LEFT_STRUCT_CANARY;
                return WRONG_LEFT_STRUCT_CANARY;
            }

            if (stack->right_struct_canary != Canary)
            {
                stack->error = WRONG_RIGHT_STRUCT_CANARY;
                return WRONG_RIGHT_STRUCT_CANARY;
            }

            IF_HASH_LEVEL_PROTECTION
            (
                for (size_t poison_checker = stack->size; poison_checker < stack->capacity; ++poison_checker)
                {
                    if (stack->data[poison_checker] != Poison)
                    {
                        stack->error = WRONG_SIZE;
                        return WRONG_SIZE;
                    }
                }

                if (stack->array_hash != CalculatingHash(((canary_t *) stack->data) - 1, stack->capacity *
                                                            sizeof(stack_element_t) + 2 * sizeof(canary_t)))
                {
                    stack->error = WRONG_ARRAY_HASH;
                    return WRONG_ARRAY_HASH;
                }

                if (stack->stack_hash != CalculatingHash(stack, 2 * sizeof(size_t) + sizeof(char *) + sizeof(stack_element_t *) +
                                                         2 * sizeof(canary_t) + 2 * sizeof(int)))
                {
                    stack->error = WRONG_STACK_HASH;
                    return WRONG_STACK_HASH;
                }
            )

            return 0;
        }
    }

    const char *TextError(Stack_t *stack)
    {
        StackNullCheck(stack);

        switch (stack->error)
        {
            case 0:  return "NO_ERRORS";
            case 1:  return "SIZE_OUT_OF_CAPACITY";
            case 2:  return "OUT_OF_MEMORY";
            case 3:  return "NEGATIVE_SIZE";
            case 4:  return "NEGATIVE_CAPACITY";
            case 5:  return "STACK_USING_ZERO_CAPACITY";
            case 6:  return "NULLPTR_TO_ARRAY";
            case 7:  return "NULL_POP";
            case 8:  return "EMPTY_TOP_ATTEMPT";
            case 9:  return "WRONG_SIZE";
            case 10: return "STACK_IS_DESTRUCTED";
            case 11: return "STACK_IS_NOT_CONSTRUCTED";
            case 12: return "WRONG_LEFT_ARRAY_CANARY";
            case 13: return "WRONG_RIGHT_ARRAY_CANARY";
            case 14: return "WRONG_LEFT_STRUCT_CANARY";
            case 15: return "WRONG_RIGHT_STRUCT_CANARY";
            case 16: return "WRONG_ARRAY_HASH";
            case 17: return "WRONG_STACK_HASH";
            default: return "UNKNOWN ERROR";
        }
    }

    void PlacingCanary(Stack_t *stack, void *memory)
    {
        StackNullCheck(stack);
        assert(memory != nullptr);

        canary_t *left_canary = (canary_t *) memory;
        *left_canary = Canary;

        stack->data = (stack_element_t *) (left_canary + 1);

        canary_t *right_canary = (canary_t *) (stack->data + stack->capacity);
        *right_canary = Canary;
    }
)

IF_HASH_LEVEL_PROTECTION
(
    void FillingPoison(Stack_t *stack)
    {
        assert(stack != nullptr);

        for (size_t poisoner = stack->size; poisoner < stack->capacity; ++poisoner)
        {
            stack->data[poisoner] = Poison;
        }
    }

    unsigned int CalculatingHash(const void *hash_from, size_t bytes_to_hash)
    {
        assert(hash_from != nullptr);

        unsigned int hash = 0;

        for (size_t cur_byte = 0; cur_byte < bytes_to_hash; ++cur_byte)
        {
            hash += (unsigned char) (*((char *) hash_from + cur_byte));
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }

        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash ^= (hash << 15);

        unsigned int stack_address = (unsigned int) hash_from;

        hash ^= stack_address;

        return hash;
    }

    void PlacingHash(Stack_t *stack)
    {
        stack->array_hash = CalculatingHash(((canary_t *) stack->data) - 1, stack->capacity *
                                                  sizeof(stack_element_t) + 2 * sizeof(canary_t));

        stack->stack_hash = CalculatingHash(stack, 2 * sizeof(size_t) + sizeof(char *) + sizeof(stack_element_t *) +
                                                2 * sizeof(canary_t) + 2 * sizeof(int));
    }

)
