/*
* @Jacob Kosinski
* Date: 4/2/2025
* ByteForge Systems
* MIPS-Translatron 3000
*/


#include "Instruction.h"

void lw_immd_assm(void) {
	// Checking that the op code matches
	// strcmp(string1, string2) return 0 if they match
	if (strcmp(OP_CODE, "LW") != 0) {
		// If the op code doesnt match, this isnt the correct command
		state = WRONG_COMMAND;
		return;
	}
	
	/*
		Checking the type of parameters
	*/

	// First parameter should be a register if not throw error
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	//for lw second parameter is a register. This was changed from an intermediate to a register
	if (PARAM2.type !=  IMMEDIATE) {
		state = INVALID_IMMED; 
		return;
	}

	// for lw the third parameter should be an immediate
	if (PARAM3.type != REGISTER) {
		state = MISSING_REG; 
		return;
	}

	
	/*
		Checking the value of parameters
	*/

	// Rs should be 31 or less
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}
	// The offset value is limited to 16 bits, this is 0xFFFF
	if ( PARAM2.value > 0xFFFF) {
		state = INVALID_IMMED;
		return;
	}
	// Rt should be 31 or less
	if ( PARAM3.value > 31) {
		state = INVALID_REG;
		return;
	}
	
	/*
		Putting the binary together
	*/
	
	//Set the func
	setBits_str(31, "100011");
	// set Rt
	setBits_num(20, PARAM1.value, 5);
	// Set the offset
	setBits_num(15, PARAM2.value, 16);
	// set Rs
	setBits_num(25, PARAM3.value, 5);
	
	

	//tell the system the encoding is done
	state = COMPLETE_ENCODE;
}


void lw_immd_bin(void) {

	// Check if the op code bits match
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	if (checkBits(31, "100011") != 0) {
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
	uint32_t imm16 = getBits(15, 16);

	/*
		Setting Instuciton values
	*/
	// Format: LW $rt, offset($rs)
	setOp("LW");
	setParam(1, REGISTER, Rt); // destination register
	setParam(2, IMMEDIATE, imm16); // Offset value
	setParam(3, REGISTER, Rs); // base register
	// tell the system the decoding is done
	state = COMPLETE_DECODE;

}
