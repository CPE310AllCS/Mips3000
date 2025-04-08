#include "MIPS_Interpreter.h"
#include "testbench.h"

extern Assm_Instruct assm_instruct;
char* assm;
extern uint32_t instruct;
extern uint16_t state;

char* stringParam(struct Param* param) {
	// checks the type of parameter and prints accordingly
	char* paramstr = malloc(20);
	if (!paramstr) {
        return NULL; // Handle allocation failure
    }
	switch (param->type) {
	case EMPTY: {
		strcpy(paramstr, "<>");
		break;
	}
	case REGISTER: {
		uint32_t temp = param->value;
		if (param->value == 0) {
			strcpy(paramstr, "$zero");
		}
		else if (param->value == 2 || param->value == 3) {
			sprintf(paramstr, "$v%d", temp - 2);
		} 
		else if (param->value >= 4 && param->value <= 7) {
			sprintf(paramstr, "$a%d", temp - 4);
		}
		 else if (param->value >= 8 && param->value <= 15) {
			sprintf(paramstr, "$t%d", temp - 8);
		}
		 else if (param->value >= 16 && param->value <= 23) {
			sprintf(paramstr, "$s%d", temp - 16);
		} 
		else if (param->value == 24 || param->value == 25) {
			sprintf(paramstr, "$t%d", temp - 16);
		}
		else if (param->value == 28) {
			strcpy(paramstr, "$gp");
		}
		else if (param->value == 29) {
			strcpy(paramstr, "$sp");
		}
		else if (param->value == 30) {
			strcpy(paramstr, "$fp");
		}
		else if (param->value == 31) {
			strcpy(paramstr, "$ra");
		}
		break;
	}
	case IMMEDIATE: {
		sprintf(paramstr, "#0x%X", param->value);
		break;
	}
	default: {
		sprintf(paramstr, "<unknown: %d, %d>", param->type, param->value);
		break;
	}
	}
	return paramstr;
}


/*
	Purpose: prints the text instruction
	Params: none
	Return: none
*/
char* strAssm(void) {
	// prints the op code
	char* line = malloc(100);
	if (!line) {
        return NULL; // Handle allocation failure
    }

	sprintf(line, "%s", assm_instruct.op);

	// checks param 1 and prints if it isn't empty
	if (PARAM1.type != EMPTY) {
		char* param1 = stringParam(&PARAM1);
        strcat(line, " ");
        strcat(line, param1);
        free(param1);
	}

	// checks param 2 and prints if it isn't empty
	if (PARAM2.type != EMPTY) {
		char* param2 = stringParam(&PARAM2);
        strcat(line, ", ");
        strcat(line, param2);
        free(param2);
	}

	// checks param 3 and prints if it isn't empty
	if (PARAM3.type != EMPTY) {
		char* param3 = stringParam(&PARAM3);
		if (PARAM3.type == REGISTER && (strcmp(OP_CODE, "LW") == 0 || strcmp(OP_CODE, "SW") == 0)) {
			strcat(line, "(");
            strcat(line, param3);
            strcat(line, ")");
		}
		else {
			strcat(line, ", ");
            strcat(line, param3);
		}
		free(param3);
	}

	// checks param 4 and prints if it isn't empty
	if (PARAM4.type != EMPTY) {
		char* param4 = stringParam(&PARAM4);
        strcat(line, ", ");
        strcat(line, param4);
        free(param4);
	}
	return line;
}



char * machinetostr(void) {
	char* line = malloc(40);
	// prints the binary instrution in binary form
	int pos = 0;
	for (int i = 31; i > -1; i--) {
		line[pos++] = ((BIN32 >> i) & 1) ? '1' : '0';

		if (i % 4 == 0) {
			line[pos++] = ' ' ;
		}
	}

	line[pos] = '\0';
	return line;
}


char* strResult(void) {
	// string to be returned
	char* line = malloc(40);
	// checks the current state and prints a corresponding message
	switch (state) {
	case NO_ERROR: {
		puts("System is Error Free");
		break;
	}
	case COMPLETE_ENCODE: {
		line = machinetostr();
		break;
	}
	case COMPLETE_DECODE: {
		line = strAssm();
		break;
	}
	case UNRECOGNIZED_COMMAND: {
		error("The given instruction was not recognized");
		break;
	}
	case UNRECOGNIZED_COND: {
		error("The given conditional is not recognized");
		break;
	}
	case MISSING_REG: {
		error("Missing register parameter");
		break;
	}
	case INVALID_REG: {
		error("The given register is invalid for the specified command");
		break;
	}
	case MISSING_PARAM: {
		error("Expected a param, none was found");
		break;
	}
	case INVALID_PARAM: {
		error("The given parameter is invalid for the specified command");
		break;
	}
	case UNEXPECTED_PARAM: {
		error("Found a parameter when none was expected");
		break;
	}
	case INVALID_IMMED: {
		error("The given immediate value is invalid for the specified command");
		break;
	}
	case MISSING_SPACE: {
		error("Expected a space, none was found");
		break;
	}
	case MISSING_COMMA: {
		error("Expected a comma, none was found");
		break;
	}
	case INVALID_SHIFT: {
		error("The given shift is invalid");
		break;
	}
	case MISSING_SHIFT: {
		error("Expected a shift value but none was found");
		break;
	}
	case UNDEF_ERROR:
	default: {
		error("An unknown error code has occured");
		break;
	}
	}
	return line;
}
char * tb_binary2assembly(char* buff) {
	
		// tries to parse the number
		parseBin(buff);

		// checks if there was an error, and decodes if there wasn't
		if (state == NO_ERROR) {
			decode();
		}

		// either prints an error message or the encoded instruction
		return strResult();
}


char* tb_assembly2machine(char* buff) {
	// loops forever
	while (1) {
		// prompts and takes input
		puts("\nEnter a line of assembly:");
		printf("> ");
		memset(buff, '\0', BUFF_SIZE);
		gets(buff, BUFF_SIZE);

		// if the string is empty, go back to the previous menu
		if (strlen(buff) == 0) {
			break;
		}

        assm = buff;
		// tries to parse the instruction
		parseAssem(buff);

		// checks if there was an error, and encodes if there wasn't
		if (state == NO_ERROR) {
			encode();
		}

		// either prints an error message or the encoded instruction string
		return strResult();
	}
}

void tokenize(char* res[], char* str, const char* delimiter) {
	// Tokenize the string using the specified delimiter
	char* token = strtok(str, delimiter);
	//keep track of the array index added to
	int i = 0;
	// While there are still tokens in the string add the, to the array
	while (token != NULL) {
		res[i++] = token;
		token = strtok(NULL, " ,\n");
	}
	res[i] = NULL; // Null-terminate the array of tokens
}

void dec_to_hex(char* buff, char* dec){
	// If the string is already in hex format return it as is
	if ((*dec == '0') && (toupper(*(dec + 1)) == 'X')) {
		buff = dec;
		return;
	}
	else{
		// If the first character og dec exists. It will be a hashtag so move all the characters one to the left to overwrite
		if (dec != NULL && dec[0] != '\0') { // Check if string is not NULL and not empty
			memmove(dec, dec + 1, strlen(dec));
		}
		// convert the decimal to an integer and then to hex
		int num = atoi(dec);
		sprintf(buff, "#0X%x", num);
		return;
	}

}

void compareassm(char* arr1[], char* arr2[]){
	// flag to keep track of any errors
	int flag = 0;
	// Loops through both tokenized assembly arrays
	for (int i = 0; i < sizeof(arr1)/sizeof(arr1[0]); i++) {
		// if The parameter is not the opcode or a register conver it to hexadecimal for comparison
		if(i !=0 && *arr1[i]!= '$'){
			char buff[10];
			dec_to_hex(buff,arr1[i]);
			char * upperbuff = strtoupper(buff);
			// If the hexs aren't equal return it as a mismatch
			if (strcmp(upperbuff, arr2[i]) != 0) {
				printf("Mismatch at Parameter %d: %s vs %s\n", i, upperbuff, arr2[i]);
				flag =1;
			}
		}
		//if both arrays still have a member keep iterating and see if they are equal
		else if (arr1[i] != NULL && arr2[i] != NULL) {
			if (strcmp(arr1[i], arr2[i]) != 0) {
				printf("Mismatch at Parameter %d: %s vs %s\n", i, arr1[i], arr2[i]);
				flag =1;
	
			}
		}// if they aren't equal return
		else if (arr1[i] == NULL || arr2[i] == NULL) {
			return;
		}
	}
	// If no mismatches were found return it as a match
	if (flag == 0) {
		printf("The assembly and final strings match.\n");
	}
	return;
}
char * strtoupper(char* str){
	char* upperstr = strdup(str); // Duplicate the string to avoid modifying the original
	if (!upperstr) {
		perror("Memory allocation failed");
		return 0;
	}

	// Convert the entire string to uppercase
	for (char* p = upperstr; *p; ++p) {
		*p = toupper(*p);
	}
	return upperstr;
}
void testbench(void)
{
	// loop until empty string is entered
	while(1){
	
    char buffer[BUFF_SIZE] = { '\0' };
    char* mach = tb_assembly2machine(buffer);
	if (strlen(mach) == 0) {
		break;
	}

	printf("%s\n", mach);
	char* final = tb_binary2assembly(mach);
	printf("%s\n", final);

	char* upperAssm = strtoupper(assm);

	char * upperFinal = strtoupper(final);

	char* tokens1[5];
	tokenize(tokens1, upperAssm, ", ");

	char* tokens2[5];
	tokenize(tokens2, upperFinal, ", ");
	
	compareassm(tokens1,tokens2);
	}
	return;
}