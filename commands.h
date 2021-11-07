DEF_CMD(hlt, 0, 0, 0, 
    
    break;
)

DEF_CMD(push, 1, 1, 2, 

    if ((CODE[IP] & ARGS_MASK) == IMM_CONST_MASK)
    {
        ++IP;
        StackPush(STACK, *((int *) (CODE + IP)));
        IP += sizeof(int);
    }
    else if ((CODE[IP] & ARGS_MASK) == REG_MASK) 
    {
        ++IP;
        StackPush(STACK, REGS[(int) (*(CODE + IP))]);
        ++IP;
    }
    else if ((CODE[IP] & ARGS_MASK) == RAM_MASK + IMM_CONST_MASK)
    {
        ++IP;
        StackPush(STACK, RAM_[*((int *) (CODE + IP))]);
        IP += sizeof(int);
    }
    else if ((CODE[IP] & ARGS_MASK) == RAM_MASK + REG_MASK)
    {
        ++IP;
        StackPush(STACK, RAM_[REGS[(int) (*(CODE + IP))]]);
        ++IP;
    }
    else if ((CODE[IP] & ARGS_MASK) == RAM_MASK + REG_MASK + IMM_CONST_MASK)
    {
        ++IP;
        int ram_index = REGS[*(CODE + IP)];
        ++IP;
        ram_index += *((int *) (CODE + IP));
        StackPush(STACK, RAM_[ram_index]);
        IP += sizeof(int);
    }
    else
    {
        printf("Unknown args type in byte %d\n", IP);
        return UNKNOWN_ARGS_TYPE;
    }
)

//We know that we use char as arr sybscript

DEF_CMD(pop, 2, 0, 2, 

    if ((CODE[IP] & ARGS_MASK) == 0)
    {
        StackPop(STACK);
        ++IP;
    }
    else if ((CODE[IP] & ARGS_MASK) == REG_MASK)
    {
        ++IP;
        REGS[*(CODE + IP)] = StackPop(STACK);
        ++IP;
    }
    else if ((CODE[IP] & ARGS_MASK) == RAM_MASK + REG_MASK)
    {
        ++IP;
        RAM_[REGS[*(CODE + IP)]] = StackPop(STACK);
        ++IP;
    }
    else if ((CODE[IP] & ARGS_MASK) == RAM_MASK + REG_MASK + IMM_CONST_MASK)
    {
        ++IP;
        int ram_index = REGS[*(CODE + IP)];
        ++IP;
        ram_index += *((int *) (CODE + IP));
        RAM_[ram_index] = StackPop(STACK);
        IP += sizeof(int);
    }
    else
    {
        printf("Unknown args type in byte %d\n", IP);
        return UNKNOWN_ARGS_TYPE;
    }
)

DEF_CMD(add, 3, 0, 0, 

    StackPush(STACK, POP + POP);
    ++IP;
)

DEF_CMD(sub, 4, 0, 0, 

    stack_element_t subtr = POP;
    StackPush(STACK, POP - subtr);
    ++IP;
)

DEF_CMD(mul, 5, 0, 0, 

    StackPush(STACK, POP * POP);
    ++IP;
)

DEF_CMD(div, 6, 0, 0,

    stack_element_t divider = POP;
    if (!divider)
    {
        printf("Division by zero attempt\n");
        return DIVISION_BY_ZERO;
    }

    StackPush(STACK, POP / divider);
    ++IP;
)

DEF_CMD(dump, 7, 0, 0, 
    
    ProcDump(proc);
    printf("dump is in %s", DUMPFILE);
)

DEF_CMD(jmp, 8, 1, 1, 

    ++IP;
    IP = *((int *) (CODE + IP));
)

DEF_CMD(ja, 9, 1, 1, 

    stack_element_t arg1 = POP;

    if (POP > arg1)
    {
        ++IP;
        IP = *((int *) (CODE + IP));
    }
    else
    {
        IP += sizeof(char) + sizeof(int);
    }
)

DEF_CMD(jb, 10, 1, 1, 

    stack_element_t arg1 = POP;

    if (POP < arg1)
    {
        ++IP;
        IP = *((int *) (CODE + IP));
    }
    else
    {
        IP += sizeof(char) + sizeof(int);
    }
)

DEF_CMD(je, 11, 1, 1, 

    stack_element_t arg1 = POP;

    if (POP == arg1)
    {
        ++IP;
        IP = *((int *) (CODE + IP));
    }
    else
    {
        IP += sizeof(char) + sizeof(int);
    }
)

DEF_CMD(jne, 12, 1, 1, 

    stack_element_t arg1 = POP;

    if (POP != arg1)
    {
        ++IP;
        IP = *((int *) (CODE + IP));
    }
    else
    {
        IP += sizeof(char) + sizeof(int);
    }
)

DEF_CMD(jae, 13, 1, 1, 

    stack_element_t arg1 = POP;

    if (POP >= arg1)
    {
        ++IP;
        IP = *((int *) (CODE + IP));
    }
    else
    {
        IP += sizeof(char) + sizeof(int);
    }
)

DEF_CMD(jbe, 14, 1, 1, 

    stack_element_t arg1 = POP;

    if (POP <= arg1)
    {
        ++IP;
        IP = *((int *) (CODE + IP));
    }
    else
    {
        IP += sizeof(char) + sizeof(int);
    }
)

DEF_CMD(call, 15, 1, 1,

    StackPush(CALL_STACK, IP + sizeof(char) + sizeof(int));
    ++IP;
    IP = *((int *) (CODE + IP));
)

DEF_CMD(ret, 16, 0, 0,
    IP = POP_C;
)

DEF_CMD(in, 17, 0, 0,
    ++IP;
    printf("Enter number: ");
    stack_element_t entered_number = 0;
    scanf("%d", &entered_number);
    StackPush(STACK, entered_number); 
)

DEF_CMD(out, 18, 0, 0,
    ++IP;
    printf("%d\n", POP);
)

DEF_CMD(srt, 19, 0, 0,
    ++IP;
    double num = sqrt((double) POP);
    StackPush(STACK, (int) ceil(num));
)