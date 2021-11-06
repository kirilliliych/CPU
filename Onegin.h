// -------------------------------------------------------
//! @file Onegin.h
//! @brief Prototypes & documentation for Onegin.cpp
//! @author kirilliliych (https://github.com/kirilliliych)
//! @date 2021-09-23
//! @copyright Copyright (c) 2021
// -------------------------------------------------------

#ifndef ONEGIN_H
#define ONEGIN_H

#include <assert.h>
#include <cstring>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

const size_t MAX_NAME_SIZE = 1024;                                    // maximum size for the name of file
const int INPUT_ERROR = -2;                                          // means there is a mistake in input

/**
 *  Contains all possible comparison functions results
 */
enum ComparisonResult
{
    SMALLER = -1,                                              // means first string is lexicografically smaller than second
    BIGGER  =  1,                                              // means first string is lexicografically bigger  than second
    EQUAL   =  0                                               // means first string is equal to second
};

/**
 *  Contains all possible results of getting input file name
 */
enum CheckResult
{
    CHECK_OK = 0,                                              // means check has succeeded
    CHECK_FAILED = 1                                           // means check has failed
};

/**
 *  String struct, contains string and its length
 */
struct String
{
    char *str = nullptr;
    size_t len = 0;
};

/**
 *  Text struct, contains information about input text
 */
struct Text
{
    char *buffer = nullptr;
    String *lines = nullptr;
    size_t lines_number = 0;
    size_t file_size = 0;
    /*char file_name[MAX_NAME_SIZE] = {0};
    FILE *file_ptr = nullptr;                                                       change for cpu!!! */                                                     
};

/**
 *  Prints helping information if there is -help in argv
 *
 *  @param args_num - number of arguments
 *  @param args - pointer to *argv[]
 *
 */
void HelpIfNeed(int args_num, const char **args);

/**
 *  Gets name of the input file
 *
 *  @param [out] input_text - pointer to text struct
 *
 */
int GetFileName(Text *input_text);

/**
 *  Fills text struct
 *
 *  @param [out] input_text - pointer to text struct
 *
 *  @return 0 if input is ok and INPUT_ERROR if if there is an error
 */
int TextInput(Text *input_text, const char *name_of_file);

/**
 *  Creates buffer and fills it with text
 *
 *  @param [out] input_text - pointer to text struct
 *
 *  @return pointer to the buffer with text
 *
 */
char *GetTextFromFile(Text *input_text, FILE *file_ptr);

/**
 *  Finds out size of input file
 *
 *  @param [in] file - pointer to input file
 *
 *  @return Size of file
 *
 */
size_t GetFileSize(FILE *file);

/**
 *  Counts how many strings are there in text
 *
 *  @param buffer - pointer to buffer with a text
 *
 *  @return number of strings in text
 */
int CountStrings(char *buffer);

/**
 *  Places a pointer in the buffer at the beginning of each line in the text. Also counts the length of these lines
 *
 *  @param [out] input_text - pointer to text_struct
 *
 *  @return Pointer to array of filled in string struct
 *
 */
String *PlacePointers(Text *input_text);

/**
 *  Finds out if symbol is from cyrillic alphabet
 *
 *  @param [in] letter - checked symbol
 *
 *  @return True if it is from cyrillic alphabet and false if it is not
 *
 */
bool IsCyrillic(char letter);

/**
 * Makes cyrillic letter a lowercase letter
 *
 * @param [in] letter - transformed letter
 *
 * @return Code of lowercase letter of cyrillic alphabet
 *
 */
int ToLowerCyrillic(char letter);

/**
 *  Finds out if symbol is from latin alphabet
 *
 *  @param [in] letter - checked symbol
 *
 *  @return True if it is from latin alphabet and false if it is not
 *
 */
bool IsLatin(char letter);

/**
 *  Makes letter a lowercase letter
 *
 *  @param [in] letter - transformed letter
 *
 *  @return Code of lowercase letter of cyrillic or latin alphabet
 *
 */
int ToLower(char letter);

/**
 *  Finds out if symbol is from cyrillic or latin alphabet
 *
 *  @param [in] letter - checked symbol
 *
 *  @return True if it is from cyrillic or latin alphabet and false if it is not
 *
 */
bool IsLetter(char letter);

/**
 *  Makes the name of output file by adding "_sorted.txt" to the end of input file name
 *
 *  @param [in]  input_text - pointer to the text struct
 *  @param [out] file_name_output - pointer to the output file name
 */
void OutputCtor(Text *input_text, char *file_name_output);

/**
 *  Sorts array and outputs it to the file according to parameters
 *
 *  @param [out] input_text pointer to struct that contains information about input text
 *  @param [in]  sorting function - pointer to function that sorts array in a certain way
 *  @param [in]  type_size - size of array's element
 *  @param [in]  comparator - pointer to comparing function
 *  @param [in]  output_function - pointer to function that outputs data to the file
 *  @param [in]  file_name_output - pointer to the output file
 *  @param [in]  mode - pointer to the mode of output file opening
 *  @param [in]  message - string that is output to the file
 *
 */
void SortAndPrint(Text *input_text, int (comparator)(const void *, const void *),
                 void (sorting_function)(void *data, size_t number_of_elements,
                 size_t type_size, int (comparator)(const void *, const void *)),
                 void (output_function)(FILE* file, Text *text),
                 const char *file_name_output, const char *mode, const char *message);

/**
 *  Prints the result text to a file after sorting elements
 *
 *  @param [in]  file - pointer to output file
 *  @param [out] input_text - pointer to text struct
 *
 */
void PrintText(FILE *file, Text *input_text);

/**
 *  Prints text from buffer to a file
 *
 *  @param [in] file - pointer to output file
 *  @param [in] input_text - pointer to text struct
 *
 */
void PrintBuffer(FILE *file, Text *input_text);

/**
 *  Frees memory used by buffer and array of string struct
 *
 *  @param [in] input_text - pointer to the text struct
 *
 */
void FreeMemory(Text *input_text);

#endif
