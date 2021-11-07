#include "cpu.h"
#include "Assembler.h"
#include "Location.h"

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        printf("You need to give binary file's name\n");
        return TOO_FEW_ARGS_FOR_CPU;
    }
    else if (argc > 2)
    {
        printf("You only need to give binary file's name\n");
        return TOO_MANY_ARGS_FOR_CPU;
    }

    Cpu proc = {};
    CpuCtor(&proc);

    GetCode(&proc, argv[1]);
    ExecuteCode(&proc);

    CpuDtor(&proc);

    return OK;
}

void CpuCtor(Cpu *proc)
{
    assert(proc != nullptr);

    StackCtor(STACK, CPU_STACK_SIZE);
    StackCtor(CALL_STACK, CPU_CALL_STACK_SIZE);

    CODE = (char *) calloc(MAX_CODE_LENGTH, sizeof(char));
    assert(CODE != nullptr);

    RAM_ = (int *) calloc(RAM_SIZE, sizeof(int));
    assert(RAM_ != nullptr);
}

void CpuDtor(Cpu *proc)
{
    AssertCpu(proc);

    free(CODE);
    free(RAM_);
    free(STACK.data);
    free(CALL_STACK.data);

    for (int i = 0; i < REGS_QUANTITY; ++i)
    {
        REGS[i] = Poison;
    }

    IP = -1;
}

void AssertCpu(Cpu *proc)
{
    assert(proc != nullptr);
    assert(CODE != nullptr);
    assert(RAM_ != nullptr);
    assert(STACK.data != nullptr);
    assert(CALL_STACK.data != nullptr);
}

void GetCode(Cpu *proc, const char *binary_file)
{
    AssertCpu(proc);
    assert(binary_file != nullptr);

    FILE *binary_file_ptr = fopen(binary_file, "rb");
    assert(binary_file_ptr != nullptr);

    size_t file_size = GetFileSize(binary_file_ptr);
    file_size = fread(CODE, sizeof(char), file_size, binary_file_ptr);

    fclose(binary_file_ptr);
}

size_t GetFileSize(FILE *file)
{
    assert(file != nullptr);
    
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    return file_size;
}

int ExecuteCode(Cpu *proc)
{
    AssertCpu(proc);

    if (CheckHeader(proc))
    {
        printf("Bad version or signature, try to use another cpu version");
        return BAD_HEADER;
    }

    #define DEF_CMD(name, number, min_args_num, max_args_num, code)                 \
        if ((CODE[IP] & COMMAND_MASK) == number)                                    \
        {                                                                           \
            code                                                                    \
        }
        else                                                                           

    while (true)
    {
        #include "commands.h"

        /*else*/
        {
            return UNKNOWN_COMMAND;
        }
    }

    #undef DEF_CMD

    return OK;
}

int CheckHeader(Cpu *proc)
{
    AssertCpu(proc);

    Header good_header = {};

    Header *my_header = (Header *) proc->code;

    if(memcmp(my_header, &good_header, sizeof(Header)) != 0)
    {
        return BAD_HEADER;
    }

    IP += sizeof(Header);
    return OK;
}

void ProcDump(Cpu *proc)
{
    AssertCpu(proc);

    location_t location = __LOCATION__;
    fprintf(logs, "\t\t\tSTACK:\n\n\n");
    StackDump(logs, STACK, location);

    location = __LOCATION__;
    fprintf(logs, "\t\t\tCALL_STACK:\n\n\n");
    StackDump(logs, CALL_STACK, location);

    for (int i = 0; i < 2; ++i)                                                 // two lines: regs and their values
    {
        for (int j = 0; j < REGS_QUANTITY; ++j)
        {
            if (i == 0)
            {
                fprintf(logs, "  %cx  ", 'a' + j);
            }
            else
            {
                fprintf(logs, "%6d", REGS[j]);
            }
        }

        fprintf(logs, "\n");
    }

    fprintf(logs, "ip: %06x", (unsigned) IP);
    fflush(logs);
}