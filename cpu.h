#include "Stack.h"
#include "Masks.h"
#include "DSL.h"
#include "logs.h"

const int CPU_STACK_SIZE = 50;
const int CPU_CALL_STACK_SIZE = 50;
const int MAX_CODE_LENGTH = 10000;
const int REGS_QUANTITY = 4;
const int RAM_SIZE = 10000;

struct Cpu
{
    Stack_t stack = {};
    Stack_t call_stack = {};
    int regs[REGS_QUANTITY] = {};
    char *code = nullptr;
    int *RAM = nullptr;
    int ip = 0; 
};

enum Errors
{
    TOO_MANY_ARGS_FOR_CPU = -2,
    TOO_FEW_ARGS_FOR_CPU = -1,
    BAD_HEADER = 1,
    UNKNOWN_ARGS_TYPE,
    DIVISION_BY_ZERO, 
    UNKNOWN_COMMAND
};

void CpuCtor(Cpu *proc);

void CpuDtor(Cpu *proc);

void AssertCpu(Cpu *proc);

void GetCode(Cpu *proc, const char *binary_file);

int ExecuteCode(Cpu *proc);

int CheckHeader(Cpu *proc);

void ProcDump(Cpu *proc);

