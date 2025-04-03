/*
* Author: @Jacob Kosinski
* Date: 4/2/2025
* ByteForge Systems
* MIPS-Translatron 3000
*/



#include "Instruction.h"

void slti_immd_assm(void) {
	// Checking that the op code matches
	// strcmp(string1, string2) return 0 if they match
	if (strcmp(OP_CODE, "SLTI") != 0) {
		// If the op code doesnt match, this isnt the correct command
		state = WRONG_COMMAND;
		return;
	}

	/*
		Checking the type of parameters 
	*/

	// Generally the first parameter should always be a register
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// This is AND register, so param 2 needs to be a register
	if (PARAM2.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// The offset value is limited to 16 bits, this is 0xFFFF
	if (PARAM3.value > 0xFFFF) {
		state = INVALID_IMMED;
		return;
	}


	/*
		Checking the value of parameters
	*/

	// Rt should be 31 or less
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	// Rs should be 31 or less
	if (PARAM2.value > 31) {
		state = INVALID_REG;
		return;
	}

	// The offset value is limited to 16 bits, this is 0xFFFF
	if (PARAM3.value > 0xFFFF) {
		state = INVALID_IMMED;
		return;
	}

	/*
		Putting the binary together
	*/

	// Set the funct
	setBits_str(31, "001010");
	

	// set Rs
	setBits_num(25, PARAM2.value, 5);

	// set Rt
	setBits_num(20, PARAM1.value, 5);

	// set immediate
	setBits_num(15, PARAM3.value, 16);

	// tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void slti_immd_bin(void) {
	// Check if the op code bits match
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	//  any x will be skipped
	// If the manual shows (0), then the value of that bit doesnt matter
	if (checkBits(31, "001010") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	// If the op code bits match, then the rest can be read as correctly

	/*
		Finding values in the binary
	*/
	// getBits(start_bit, width)
	uint32_t Rs = getBits(25, 5);
	uint32_t Rt = getBits(20, 5);
	uint32_t offset = getBits(15, 16);
	/*
		Setting Instuciton values
	*/

	setOp("SLTI");
	//setCond_num(cond);
	//setParam(param_num, param_type, param_value)
	setParam(1, REGISTER,Rt ); //destination
	setParam(2, REGISTER, Rs); //first source register operand
	setParam(3, IMMEDIATE, offset); // immediate operand

	// tell the system the decoding is done
	state = COMPLETE_DECODE;
}




