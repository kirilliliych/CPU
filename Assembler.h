#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Onegin.h"
#include "Header.h"
#include "Masks.h"

const char COMMENT_SYMBOL = ';';

const int REASSEMBLING_NUMBER = 2; 
const int MAX_COMMAND_NAME_LENGTH = 20;
const int MAX_ARG_SIZE = 100;
const int MAX_LABELS_NUMBER = 100;
const int J_FIRST = 8;
const int J_LAST  = 15;
const int MAX_PROGRAMME_LENGTH = 10000;

struct BinaryCode
{
    unsigned char *code = nullptr;
    size_t current_byte = 0;
};

struct Label
{
    char name[MAX_COMMAND_NAME_LENGTH] = {};
    int byte_number = 0;
};

struct Labels
{
    Label labels_array[MAX_LABELS_NUMBER] = {};
    int labels_number = 0;
};

enum asm_prog_args_errors
{
    TOO_MANY_ARGS_FOR_ASSEMBLER,
    TOO_FEW_ARGS_FOR_ASSEMBLER
};

enum compilation_errors
{
    LABEL_ALREADY_DEFINED = -5,
    UNKNOWN_LABEL,
    WRONG_SYNTAX,
    INVALID_ARGS_NUMBER,
    INVALID_COMMAND_NAME,
    OK = 0
};

int Compilation(Text *input_text, BinaryCode *cpu_code, Labels *labels, int assembly_labels);

void BinaryCodeCtor(BinaryCode *cpu_code);

void BinaryCodeDtor(BinaryCode *cpu_code);

void LabelsCtor(Labels *labels);

void OutputToBinary(BinaryCode *cpu_code, const char *output_file);

int GetArguments(BinaryCode *cpu_code, String *line, size_t shift, int max_args_num, Labels *labels, int assembly_labels);



