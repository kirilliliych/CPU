#define STACK &proc->stack
#define CALL_STACK &proc->call_stack
#define REGS proc->regs
#define CODE proc->code
#define RAM_ proc->RAM
#define IP proc->ip
#define POP StackPop(STACK)
#define POP_C StackPop(CALL_STACK)
