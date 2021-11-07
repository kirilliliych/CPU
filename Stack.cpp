#include "Protection.h"

void StackCtor(Stack_t *stack, const int capacity)
{
    IF_CANARY_LEVEL_PROTECTION
    (
        StackNullCheck(stack);

        if (capacity < 0)
        {
            stack->error = NEGATIVE_CAPACITY;
            ASSERT_OK(stack);

            return;
        }
    )

    stack->capacity = capacity;

    if (capacity == 0)
    {
        stack->data = nullptr;
    }
    else
    {
        int place_for_canary = 0;

        IF_CANARY_LEVEL_PROTECTION
        (
            place_for_canary = 2;

            stack->left_struct_canary  = Canary;
            stack->right_struct_canary = Canary;
        )

        void *memory = calloc(stack->capacity * sizeof(stack_element_t) + place_for_canary * sizeof(canary_t), 1);

        IF_CANARY_LEVEL_PROTECTION
        (
            if (memory == nullptr)
            {
                stack->error = OUT_OF_MEMORY;
                ASSERT_OK(stack);
            }
        )

        MemoryCtor(stack, memory);
    }

    stack->error = 0;
    stack->status = CONSTRUCTED;
    stack->size = 0;

    IF_HASH_LEVEL_PROTECTION
    (
        if (stack->capacity != 0)
        {
            FillingPoison(stack);
            PlacingHash(stack);
        }
    )
}

void MemoryCtor(Stack_t *stack, void *new_memory)
{
    IF_CANARY_LEVEL_PROTECTION
    (
        PlacingCanary(stack, new_memory);
    )

    IF_NO_PROTECTION
    (
        stack->data = (stack_element_t *) new_memory;
    )
}

void StackPush(Stack_t *stack, stack_element_t value)
{
    IF_CANARY_LEVEL_PROTECTION
    (
        StackNullCheck(stack);
    )

    if ((stack->capacity == 0) || (stack->capacity - 1 == stack->size))
    {
        StackMemoryRealloc(stack);
    }

    ASSERT_OK(stack);

    stack->data[stack->size++] = value;

    IF_HASH_LEVEL_PROTECTION
    (
        PlacingHash(stack);
    )
}

stack_element_t StackPop(Stack_t *stack)
{
    IF_CANARY_LEVEL_PROTECTION
    (
        StackNullCheck(stack);

        if (stack->size == 0)
        {
            stack->error = NULL_POP;
        }

        ASSERT_OK(stack);
    )

    if ((stack->capacity >= 4) && (stack->size < ((stack->capacity) / 4)))
    {
        StackBackwardMemoryRealloc(stack);
    }

    ASSERT_OK(stack);

    stack_element_t popped_element = stack->data[--(stack->size)];
    stack->data[stack->size] = Poison;

    IF_HASH_LEVEL_PROTECTION
    (
        PlacingHash(stack);
    )

    ASSERT_OK(stack);

    return popped_element;
}

stack_element_t StackTop(Stack_t *stack)
{
    IF_CANARY_LEVEL_PROTECTION
    (
        StackNullCheck(stack);
        ASSERT_OK(stack);

        if (stack->size == 0)
        {
            stack->error = EMPTY_TOP_ATTEMPT;
            ASSERT_OK(stack);
        }
    )

    return stack->data[stack->size - 1];
}

void StackMemoryRealloc(Stack_t *stack)
{
    if (UsingStackZeroCapacity(stack))
    {
        StackCtor(stack, stack->capacity + 1);
    }

    ASSERT_OK(stack);

    int place_for_canary = 0;
    void *old_memory_pointer = stack->data;

    IF_CANARY_LEVEL_PROTECTION
    (
        StackNullCheck(stack);

        place_for_canary = 2;
        old_memory_pointer = ((canary_t *) stack->data) - 1;
    )

    void *memory = realloc(old_memory_pointer, EXPAND_MEMORY_COEF * stack->capacity * sizeof(stack_element_t) + place_for_canary * sizeof(canary_t));

    IF_CANARY_LEVEL_PROTECTION
    (
        if (memory == nullptr)
        {
            stack->error = OUT_OF_MEMORY;
            ASSERT_OK(stack);
        }
    )

    stack->capacity *= EXPAND_MEMORY_COEF;

    MemoryCtor(stack, memory);

    IF_HASH_LEVEL_PROTECTION
    (
        FillingPoison(stack);
        PlacingHash(stack);
    )

    ASSERT_OK(stack);
}

void StackBackwardMemoryRealloc(Stack_t *stack)
{
    IF_CANARY_LEVEL_PROTECTION
    (
        StackNullCheck(stack);
    )

    int place_for_canary = 0;
    void *old_memory_pointer = stack->data;

    stack->capacity /= 4;

    IF_CANARY_LEVEL_PROTECTION
    (
        place_for_canary = 2;
        old_memory_pointer = ((canary_t*) stack->data) - 1;
    )

    void *memory = realloc(old_memory_pointer, (stack->capacity + 1) * sizeof(stack_element_t) + place_for_canary * sizeof(canary_t));

    MemoryCtor(stack, memory);

    IF_HASH_LEVEL_PROTECTION
    (
        PlacingHash(stack);
    )

    ASSERT_OK(stack);
}

void StackDtor(Stack_t *stack)
{
    CloseLogs();

    IF_CANARY_LEVEL_PROTECTION
    (
        StackNullCheck(stack);
        ASSERT_OK(stack);

        free(((canary_t *) stack->data) - 1);
    )

    IF_NO_PROTECTION
    (
        free(stack->data);
    )

    stack->data     = nullptr;
    stack->size     = -1;
    stack->capacity = -1;

    IF_CANARY_LEVEL_PROTECTION
    (
        stack->error = 0;
        stack->status = DESTRUCTED;
        stack->left_struct_canary  = -1;
        stack->right_struct_canary = -1;
    )

    IF_HASH_LEVEL_PROTECTION
    (
        stack->array_hash  = -1;
        stack->stack_hash  = -1;
    )
}

void StackDump(FILE* out, Stack_t *stack, location_t location)
{
    assert(out != nullptr);

    IF_CANARY_LEVEL_PROTECTION
    (
        StackNullCheck(stack);

        const char *error_code = TextError(stack);

        PrintToLogs("ERROR: file %s line %d function %s\n"
                    "Stack (ERROR #%d: %s [%p] \"%s\")\n",

                    location.file, location.line, location.func,
                    stack->error, error_code, stack, stack->name);

        if ((stack->error == STACK_IS_DESTRUCTED) || (stack->error == STACK_USING_ZERO_CAPACITY))
        {
            return;
        }
    )

    IF_NO_PROTECTION
    (
        PrintToLogs(out, "Stack\n");
    )

    PrintToLogs("{\n"
                "\tsize = %u\n"
                "\tcapacity = %u\n",

                stack->size,
                stack->capacity);

    IF_CANARY_LEVEL_PROTECTION
    (
        PrintToLogs("\tleft_struct_canary  = %llx\n"
                    "\tright_struct_canary = %llx\n",

                    stack->left_struct_canary,
                    stack->right_struct_canary);

        if (stack->data != nullptr)
        {
            PrintToLogs("\tleft_array_canary   = %llx\n"
                        "\tright_array_canary  = %llx\n",

                        ((canary_t *) stack->data)[-1],
                        *((canary_t *) (stack->data + stack->capacity)));
        }
    )

    IF_HASH_LEVEL_PROTECTION
    (
        PrintToLogs("\tstack_hash = %x\n"
                    "\tarray_hash = %x\n",

                    stack->stack_hash,
                    stack->array_hash);
    )

    PrintToLogs("\tdata[%p]\n",

                stack->data);

    if (stack->data != nullptr)
    {
        if (stack->error != NEGATIVE_CAPACITY)
        {
            PrintToLogs("\t{\n");
            PrintArray(stack);
            PrintToLogs("\t}\n");
        }
    }

    PrintToLogs("}\n\n\n");

    fflush(out);
}

void PrintArray(Stack_t *stack)
{
    IF_CANARY_LEVEL_PROTECTION
    (
        StackNullCheck(stack);
    )

    for (size_t cur_elem = 0; cur_elem < stack->size; ++cur_elem)
    {
        PrintToLogs("\t\t*[%d] = %ld\n", cur_elem, stack->data[cur_elem]);
    }

    IF_CANARY_LEVEL_PROTECTION
    (
        for (size_t cur_elem = stack->size; cur_elem < stack->capacity; ++cur_elem)
        {
            PrintToLogs("\t\t [%d] = %ld (Poison!)\n", cur_elem, stack->data[cur_elem]);
        }
    )
}
