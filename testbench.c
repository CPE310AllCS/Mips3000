#include "MIPS_Interpreter.h"
#include "testbench.h"

extern Assm_Instruct assm_instruct;
char* assm;
extern uint32_t instruct;
extern uint16_t state;

/*
	Purpose: prints a parameter
	Params: Param* param - the parameter to print
	Return: none
*/
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
	char* token = strtok(str, delimiter);
	int i = 0;
	while (token != NULL) {
		res[i++] = token;
		token = strtok(NULL, " ,\n");
	}
	res[i] = NULL; // Null-terminate the array of tokens
}

// void compareassm(char* assm, char* final) {
// 	char* upperAssm = strdup(assm); // Duplicate the string to avoid modifying the original
// 	if (!upperAssm) {
// 		perror("Memory allocation failed");
// 		return;
// 	}
// }
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
int testbench(void)
{
    char buffer[BUFF_SIZE] = { '\0' };
    char* mach = tb_assembly2machine(buffer);
	printf("%s\n", mach);
	char* final = tb_binary2assembly(mach);
	printf("%s\n", final);

	char* upperAssm = strtoupper(assm);

	char * upperFinal = strtoupper(final);

	char* tokens1[5];
	tokenize(tokens1, upperAssm, ", ");

	char* tokens2[5];
	tokenize(tokens2, upperFinal, ", ");
	int flag = 0;
	for (int i = 0; i < 5; i++) {
		if (tokens1[i] != NULL && tokens2[i] != NULL) {
			if (strcmp(tokens1[i], tokens2[i]) != 0) {
				printf("Mismatch at Parameter %d: %s vs %s\n", i, tokens1[i], tokens2[i]);
				flag =1;
			}
		}
		else if (tokens1[i] != NULL || tokens2[i] != NULL) {
			printf("Mismatch at Parameter %d: %s vs %s\n", i, tokens1[i], tokens2[i]);
			flag =1;
		}
	}
	if (flag == 0) {
		printf("The assembly and final strings match.\n");
	}

	free(mach);
	return 0;
}