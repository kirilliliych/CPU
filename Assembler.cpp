#include "Assembler.h"

#define test printf("%d\n", __LINE__);

int main(int argc, const char *argv[])
{
    if (argc > 3)
    {
        printf("Too many arguments for assembler. You only need to give names of input and output files\n");
        return TOO_MANY_ARGS_FOR_ASSEMBLER;
    }
    else if (argc < 3)
    {
        printf("You haven't input name of input or output file\n");
        return TOO_FEW_ARGS_FOR_ASSEMBLER;
    }

    Text input_text = {};
    TextInput(&input_text, argv[1]);

    BinaryCode cpu_code = {};
    BinaryCodeCtor(&cpu_code);

    Labels labels = {};
    LabelsCtor(&labels);
    
    for (int i = 0; i < REASSEMBLING_NUMBER; ++i)
    {
        printf("%d iteration: \n", i + 1);
        int res = Compilation(&input_text, &cpu_code, &labels, i);
        printf("res = %d\n", res);
        if (res < 0)
        {
            printf("Compilation troubles, exiting");
            abort();
        }
    }
    
    FreeMemory(&input_text);
    OutputToBinary(&cpu_code, argv[2]);
    return OK;
}

void BinaryCodeCtor(BinaryCode *cpu_code)
{
    assert(cpu_code != nullptr);

    cpu_code->code = (unsigned char *) calloc(MAX_PROGRAMME_LENGTH, sizeof(cpu_code->code));
    cpu_code->current_byte = 0;
}

void BinaryCodeDtor(BinaryCode *cpu_code)
{
    assert(cpu_code != nullptr);
    assert(cpu_code->code != nullptr);

    free(cpu_code->code);
    cpu_code = nullptr;
    cpu_code->current_byte = 0;
}

void LabelsCtor(Labels *labels)
{
    labels->labels_number = 0;
}

int Compilation(Text *input_text, BinaryCode *cpu_code, Labels *labels, int assembly_labels)
{
    assert(input_text != nullptr);
    assert(cpu_code   != nullptr);
    assert(cpu_code->code != nullptr);

    cpu_code->current_byte = 0;

    for (size_t line_index = 0; line_index < input_text->lines_number; ++line_index)
    {
        char *com_symb = strchr(input_text->lines[line_index].str, COMMENT_SYMBOL);
        if (com_symb != nullptr)
        {
            *com_symb = '\0';
            input_text->lines[line_index].len = com_symb - input_text->lines[line_index].str;
        }
        
        // deleting spaces
                                                                                                                    // сделать пропуск пустых строк?
        char *space_ptr = input_text->lines[line_index].str + input_text->lines[line_index].len - 1;
        while (*space_ptr == ' ')
        {
            --space_ptr;
        }
        *(space_ptr + 1) = '\0';
        input_text->lines[line_index].len = space_ptr - input_text->lines[line_index].str + 1;
        
        // command_parsing

        char command_name[MAX_COMMAND_NAME_LENGTH] = "";
        sscanf(input_text->lines[line_index].str, "%s", command_name);
        
        // if meets name of label

        bool if_label = false;
        bool label_found = false;

        if (command_name[strlen(command_name) - 1] == ':')
        {
            if_label = true;
            command_name[strlen(command_name) - 1] = '\0';
            
            for (int i = 0; i < labels->labels_number; ++i)                                                                                     //checking if label was already defined during first assemblering
            {
                printf("label_decl: %s\n", labels->labels_array[i].name);
                if (strcmp(command_name, labels->labels_array[i].name) == 0)
                {
                    if (!assembly_labels)
                    {
                        return LABEL_ALREADY_DEFINED;
                    }
                    else
                    {
                        label_found = true;
                    }
                }
            }   

            if ((!label_found) && (assembly_labels))
            {
                return UNKNOWN_LABEL;
            }

            if (!assembly_labels)
            {
                strcpy(labels->labels_array[labels->labels_number].name, command_name);
                labels->labels_array[labels->labels_number].byte_number = cpu_code->current_byte + VERS_AND_SIGN_LENGTH;                                                                         
                ++labels->labels_number;
            }
        }
         
        if (!if_label)
        {
            //def_cmd

            bool command_recognized = false;
    
            #define DEF_CMD(name, number, min_args_num, max_args_num, ...)                                                                                                                                                                                                                                                            \
                if (!strcmp(#name, command_name))                                                                                                                           \
                {                                                                                                                                                           \
                    command_recognized = true;                                                                                                                              \
                    cpu_code->code[cpu_code->current_byte++] = (unsigned char) number;                                                                                      \
                                                                                                                                                                            \
                    if (max_args_num > 0)                                                                                                                                   \
                    {                                                                                                                                                       \
                        int real_args_number = GetArguments(cpu_code, &input_text->lines[line_index], strlen(command_name) + 1, max_args_num, labels, assembly_labels);     \
                        if (real_args_number < 0)                                                                                                                           \
                        {                                                                                                                                                   \
                            printf("Error in line %u: %d\n", line_index, real_args_number);                                                                                 \
                        }                                                                                                                                                   \
                                                                                                                                                                            \
                        if ((real_args_number < min_args_num) || (real_args_number > max_args_num))                                                                         \
                        {                                                                                                                                                   \
                            printf("Too many or too few arguments for command in line %u\n", line_index + 1);                                                               \
                            return INVALID_ARGS_NUMBER;                                                                                                                     \
                        }                                                                                                                                                   \
                    }                                                                                                                                                       \
                }                                                                                                                                                           \
                else                                                                                                                                                        

                #include "commands.h"

                /*else*/ if (!command_recognized)
                {
                    printf("Unknown command in line %u\n", line_index + 1);
                    return INVALID_COMMAND_NAME;
                }

            #undef DEF_CMD
        }
    }

    return OK;
}

void OutputToBinary(BinaryCode *cpu_code, const char *output_file)
{
    assert(cpu_code != nullptr);
    assert(cpu_code->code != nullptr);
    assert(output_file != nullptr);

    FILE *output_ptr = fopen(output_file, "wb");
    assert(output_ptr != nullptr);

    Header binary_header = {};

    fwrite(binary_header.version, sizeof(binary_header.version), 1, output_ptr);            //todo проверить fwrite
    fwrite(binary_header.signature, sizeof(binary_header.signature), 1, output_ptr);
    fwrite(cpu_code->code, sizeof(char), cpu_code->current_byte, output_ptr);

    fclose(output_ptr);
}

int GetArguments(BinaryCode *cpu_code, String *line, size_t shift, int max_args_num, Labels *labels, int assembly_labels)
{
    assert(cpu_code != nullptr);
    assert(cpu_code->code != nullptr);
    assert(line != nullptr);

    size_t command_info_byte = cpu_code->current_byte - 1;
    int args_quantity = 0;
    bool using_RAM = false;
    
    // checking RAM

    if (*(line->str + shift) == '[')
    {
        ++shift;                                    
        char *back_bracket_ptr = strchr(line->str, ']');

        if (back_bracket_ptr != line->str + line->len - 1) // fix вместо strlen
        {
            return WRONG_SYNTAX;
        }

        cpu_code->code[command_info_byte] |= RAM_MASK;
        using_RAM = true;
    } 

    // checking label

    bool if_arg_read = false;

    char arg_str[MAX_ARG_SIZE] = "";
    int arg_length = 0;

    if ((sscanf(line->str + shift, "%[a-z]%n", arg_str, &arg_length) > 0) && (cpu_code->code[command_info_byte] >= J_FIRST) && (cpu_code->code[command_info_byte] <= J_LAST))
    {
        if (assembly_labels)
        {
            for (int i = 0; i < labels->labels_number; ++i)
            {   
                if (strcmp(labels->labels_array[i].name, arg_str) == 0)
                {
                    /*printf("arg_str: %s\n", arg_str);
                    for(int j = 0; j < labels->labels_number; ++j)
                    {
                        printf("label: %s\n", labels->labels_array[j].name);
                    }*/
                    if_arg_read = true;
                    ++args_quantity;
                    shift += arg_length;

                   *((int *) (cpu_code->code + cpu_code->current_byte)) = labels->labels_array[i].byte_number;
                    cpu_code->current_byte += sizeof(int);
                }
                else
                {
                    return UNKNOWN_LABEL;
                }
            }
        }
        else
        {
            if_arg_read = true;
            ++args_quantity; 
            shift += arg_length;

            *((int *) (cpu_code->code + cpu_code->current_byte)) = 0;
            cpu_code->current_byte += sizeof(labels->labels_array->byte_number);
        }
    }

    // checking arg before plus

    // if number
    
    if (!if_arg_read)
    {
        if (sscanf(line->str + shift, "%[0-9]", arg_str) > 0)            // ты дурак? ты число считываешь лул
        {
            if_arg_read = true;
            ++args_quantity;

            cpu_code->code[command_info_byte] |= IMM_CONST_MASK;
            shift += sizeof(int);

            *((int *) (cpu_code->code + cpu_code->current_byte)) = atoi(arg_str);
            cpu_code->current_byte += sizeof(int);
        }
    }
    
    // if reg
    
    if (!if_arg_read)
    {
        if (sscanf(line->str + shift, "%1[a-d]x", arg_str) > 0)                 // возможно процент n?
        {
            if_arg_read = true;
            ++args_quantity;

            cpu_code->code[command_info_byte] |= REG_MASK;
            cpu_code->code[cpu_code->current_byte++] = (unsigned char) (arg_str[0] - 'a');
            shift += sizeof(unsigned char);    
        }
    }

    if (args_quantity == max_args_num)
    {
        if ((*(line->str + shift) != '\0') && (*(line->str + shift) != ']'))
        {
            return WRONG_SYNTAX;
        }

        if ((*(line->str + shift) == ']') && (!using_RAM))
        {
            return WRONG_SYNTAX;
        }

        return args_quantity;
    }

    // plus check

    bool if_plus = false;

    if (*(line->str + shift) == '+')
    {
        if_plus = true;
        ++shift;
    }

    // checking arg after plus

    if_arg_read = false;

    arg_length = 0;
    
    if (sscanf(line->str + shift, "%[0-9]%n", arg_str, &arg_length) > 0)
    {
        if (!if_plus)
        {
            return WRONG_SYNTAX;
        }

        if_arg_read = true;
        ++args_quantity;

        cpu_code->code[command_info_byte] |= IMM_CONST_MASK;
        shift += arg_length;

        *((int *) (cpu_code->code + cpu_code->current_byte)) = atoi(arg_str);
        cpu_code->current_byte += sizeof(int);
    }
    
    if ((if_plus && !if_arg_read) || (!if_plus && if_arg_read))
    {
        return WRONG_SYNTAX;
    }
    else
    {
        return args_quantity;
    }
}

/*const char *ErrorDescriptions(int error_code)
{
    switch (error_code)
    {
        case UNKNOWN_LABEL:
            return "Unknown label\n";
        
        case WRONG_SYNTAX:
            return "Wrong syntax\n";       
    }
}*/