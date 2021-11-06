// -------------------------------------------------------
//! @file Onegin.cpp
//! @brief Declarations of functions not related to sorting
//! @author kirilliliych (https://github.com/kirilliliych)
//! @date 2021-09-23
//! @copyright Copyright (c) 2021
// -------------------------------------------------------

#include "Onegin.h"

void HelpIfNeed(int args_num, const char **args)
{
    assert(args != nullptr);

    for (int check_args = 1; check_args < args_num; ++check_args)
    {
        if (strcmp(args[check_args], "-help") == 0)
        {
            printf("This is a text sorter.\n\n"
                   "Sorts the given file in lexicographical order.\n"
                   "First from the beginning, than from the end.\n"
                   "Both variants are put in one file, after that"
                   " the original text is put there as well.\n\n");
        }
    }
}

int TextInput(Text *input_text, const char *name_of_file)
{
    assert(input_text   != nullptr);
    assert(name_of_file != nullptr);

    /*if (GetFileName(input_text) == CHECK_FAILED)
    {
        printf("ERROR int TextInput(): file name was not input\n");                                 change for cpu !!!
        return INPUT_ERROR;
    }*/

    FILE *file_ptr = fopen(name_of_file, "r");

    if (file_ptr == nullptr)
    {
        printf("ERROR int TextInput(): file was not found");
        return INPUT_ERROR;
    }

    input_text->buffer = GetTextFromFile(input_text, file_ptr);

    input_text->lines_number = CountStrings(input_text->buffer);

    input_text->lines  = PlacePointers(input_text);

    fclose(file_ptr);

    return 0;
}

size_t GetFileSize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    return file_size;
}

char *GetTextFromFile(Text *input_text, FILE *file_ptr)
{
    assert(input_text != nullptr);

    input_text->file_size = GetFileSize(file_ptr) + 2;                                        //if there is only one string in file we need space for '\n' and '\0'

    char *buffer = (char *) calloc(input_text->file_size, sizeof(char));

    input_text->file_size = fread(buffer, sizeof(char), input_text->file_size, file_ptr) + 2; //if there is only one string in file we need space for '\n' and '\0'
    buffer[input_text->file_size - 2] = '\n';
    buffer[input_text->file_size - 1] = '\0';

    return buffer;
}

int CountStrings(char *buffer)
{
    size_t string_counter = 0;
    int checking_strings  = 0;

    while (buffer[checking_strings] != '\0')
    {
        if (buffer[checking_strings] == '\n')
        {
            ++string_counter;
        }

        ++checking_strings;
    }

    return string_counter;
}

String *PlacePointers(Text *input_text)
{
    assert(input_text != nullptr);
    
    String *strings = (String *) calloc(input_text->lines_number, sizeof(String));

    char *pointer_end = input_text->buffer;
    size_t counter = 0;

    for (char *pointer_begin = input_text->buffer; *pointer_begin != '\0'; ++pointer_begin)
    {
        if (*(pointer_begin - 1) == '\n')
        {
            *(pointer_begin - 1) = '\0';                                                                          //added for cpu!!!
        }

        if (*pointer_begin == '\n')
        {
            strings[counter].str = pointer_end;
            strings[counter].len = pointer_begin - pointer_end;
            pointer_end = pointer_begin + 1;

            ++counter;
        }
    }

    return strings;
}

void FreeMemory(Text *input_text)
{
    free(input_text->buffer);
    free(input_text->lines);

    input_text->buffer = nullptr;
    input_text->lines  = nullptr;
}
