
#define TESTBENCH_H
/// @brief Take the originial Parmater function but this functions version returns a string
/// @param paramthe parameter to print
/// @return char* the string representation of the parameter
char* stringParam(struct Param* param);

/// @brief Returns a string version of the assembly instructions 
/// @return char* the string representation of the assembly instruction 
char* strAssm(void);

/// @brief takes the originial machine tranformation function and returns a string version of the machine instruction 
/// @return char* the string representation of the machine instruction
char * machinetostr(void);

/// @brief breaks up the assembly instructions into tokens based on the delimiter
/// @param res The array to be returned with the tokens
/// @param str The assembly instruction to be tokenized
/// @param delimiter- the delimiter to be used for tokenization
void tokenize(char* res[], char* str, const char* delimiter);

/// @brief Compares the two arrays of strings and prints the differences between them and the location of differences
/// @param arr1 THe original input assembly instruction
/// @param arr2 the outputted assembly instruction after decoding
void compareassm(char* arr1[], char* arr2[]);

/// @brief Takes a decimal input as string and converts to hex string
/// @param buff  buffer to be used to save the hex string
/// @param dec the decimal string to be converted to hex
void dec_to_hex(char* buff, char* dec);

/// @brief Main function that is ran for test bench 
void testbench(void);

/// @brief Takes a string and put it into upper case
/// @param str string to be converted to upper case 
/// @return char * the string in upper case
char * strtoupper(char* str);
