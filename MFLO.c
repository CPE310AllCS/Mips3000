/*
* Author: @Jacob Kosinski
* Date: 04/2/2025
* ByteForge Systems
* MIPS-Translatron 3000
*/



#include "Instruction.h"

void mflo_reg_assm(void) {
	// Checking that the op code matches
	// strcmp(string1, string2) return 0 if they match
	if (strcmp(OP_CODE, "MFLO") != 0) {
		// If the op code doesnt match, this isnt the correct command
		state = WRONG_COMMAND;
		return;
	}

	/*
		Checking the type of parameters
	*/

	// The only parameter should be a register
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}


	/*
		Checking the value of parameters
	*/
	// Param 1 should be 31 or less
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	/*
	Putting the binary together
	*/

	// Set the opcode
	setBits_str(31, "000000");

	// set Rd
	setBits_num(15, PARAM1.value, 5);

	// Set the funct 
	setBits_str(5, "010010"); // Changed the function code from 010000 to the correct 010010

	// set 25-20 as 0s 
	setBits_str(25, "000000"); // Changed to start address from 10 to 25 

	// set 20-15 as 0s 
	setBits_str(20, "000000"); // Changed to start address from 10 to 20 to get from 20-15

	// set 10-6 as 0s 
	setBits_str(10, "00000");

	// tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void mflo_reg_bin(void) {
	// Check if the op code bits match
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	//  any x will be skipped
	// If the manual shows (0), then the value of that bit doesnt matter
	// Change the function code from 010000 to 010010
	if (checkBits(31, "000000") != 0 || checkBits(5, "010010") != 0 || checkBits(25, "0000000000") != 0 || checkBits(10, "00000") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	// If the op code bits match, then the rest can be read as correctly

	/*
		Finding values in the binary
	*/

	// getBits(start_bit, width)
	uint32_t Rd = getBits(15, 5);
	/*
		Setting Instuciton values
	*/
	setOp("MFLO");
	//setParam(param_num, param_type, param_value)
	setParam(1, REGISTER, Rd); //destination
	

	// tell the system the decoding is done
	state = COMPLETE_DECODE;
	
}


