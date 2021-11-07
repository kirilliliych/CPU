#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Masks.h"
#include "DSL.h"
#include "Header.h"
#include "Assembler.h"

const int MAX_CODE_LENGTH = 10000;

const char *DISASM_FILE_NAME = "disasm.txt";

enum DErrors
{
    BAD_HEADER,
    UNKNOWN_COMMAND,
};

size_t GetBinCode(const char *binary_file, char *binary_code);

int TranslateCode(const char *binary_code, const char *output_file, size_t file_size);

