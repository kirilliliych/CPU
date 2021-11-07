#include "Disassembler.h"

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        printf("You need to give binary file's name\n");
        return -1;
    }
    else if (argc > 2)
    {
        printf("You only need to give binary file's name\n");
        return -1;
    }

    char *binary_code = (char *) calloc(MAX_CODE_LENGTH, sizeof(char));

    size_t file_size = GetBinCode(argv[1], binary_code);

    TranslateCode(binary_code, DISASM_FILE_NAME, file_size);

    free(binary_code);
    binary_code = nullptr;
    
    return OK;
}

size_t GetBinCode(const char *binary_file, char *binary_code)
{
    assert(binary_file != nullptr);

    FILE *binary_file_ptr = fopen(binary_file, "rb");
    assert(binary_file_ptr != nullptr);

    size_t file_size = GetFileSize(binary_file_ptr);
    fread(binary_code, sizeof(char), file_size, binary_file_ptr);
    return file_size;
}

int TranslateCode(const char *binary_code, const char *output_file, size_t file_size)
{
    assert(binary_code != nullptr);
    assert(output_file != nullptr);
    
    FILE *output_file_ptr = fopen(output_file, "w");

    size_t ip = 0;

    Header good_header = {};

    const Header *my_header = (const Header *) binary_code;

    if(memcmp(my_header, &good_header, sizeof(Header)) != 0)
    {
        return BAD_HEADER;
    }

    ip += sizeof(Header);

    const char *command_name = nullptr;
    #define DEF_CMD(name, number, min_args_num, max_args_num, code)                 \
        if ((binary_code[ip] & COMMAND_MASK) == number)                              \
        {                                                                           \
            command_name = #name;                                                    \
        }                                                                           \
        else

    while(ip < file_size){
        #include "commands.h"
        /*else*/
        {
            return UNKNOWN_COMMAND;
        }

        unsigned char command = binary_code[ip];
        ip++;                                                                     
        fprintf(output_file_ptr, "%s ", command_name);  

        if((command & COMMAND_MASK) >= J_FIRST && (command & COMMAND_MASK) <= J_LAST)
        {
            fprintf(output_file_ptr, "%#04x\n",  *((const unsigned *) (binary_code + ip)));
            ip += sizeof(int);
            continue;
        }                                

        if(command & RAM_MASK)
        {
            fprintf(output_file_ptr, "[");
        }

        if(command & REG_MASK)
        {
            fprintf(output_file_ptr, "%cx", 'a' + binary_code[ip]);
            ip += sizeof(char);
        }

        if((command & REG_MASK) && (command & IMM_CONST_MASK))
        {
            fprintf(output_file_ptr, "+");
        }

        if(command & IMM_CONST_MASK)
        {
            fprintf(output_file_ptr, "%d", *((const int *) (binary_code + ip)));
            ip += sizeof(int);
        }
        if(command & RAM_MASK)
        {
            fprintf(output_file_ptr, "]");
        }

        fprintf(output_file_ptr, "\n");
    }

    return OK;
}
