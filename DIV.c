/*
* Author: Tyler Lannon
* Date: 04/05/2025
* ByteForge Systems
* MIPS-Translatron 3000
*/

#include "Instruction.h"

void div_reg_assm(void) {
    // Checking that the op code matches
    // strcmp(string1, string2) return 0 if they match
    if (strcmp(OP_CODE, "DIV") != 0) {
        // If the op code doesn't match, this isn't the correct command
        state = WRONG_COMMAND;
        return;
    }

    /*
        Checking the type of parameters
    */

    // The first parameter should be a register (Rs)
    if (PARAM1.type != REGISTER) {
        state = MISSING_REG;
        return;
    }

    // The second parameter should be a register (Rt)
    if (PARAM2.type != REGISTER) {
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

    // Rt should be 31 or less
    if (PARAM2.value > 31) {
        state = INVALID_REG;
        return;
    }

    /*
        Putting the binary together
    */

    // Set the opcode (000000 for R-format)
    setBits_str(31, "000000");
    
    // Set Rs (first source register) at bits 25-21
    setBits_num(25, PARAM1.value, 5);
    
    // Set Rt (second source register) at bits 20-16
    setBits_num(20, PARAM2.value, 5);
    
    // Set the function code for DIV (011010)
    setBits_str(5, "011010");
    
    // Set bits 15-6 to 0 (no Rd field needed for DIV)
    setBits_num(15, 0, 10);
    
    // Tell the system the encoding is done
    state = COMPLETE_ENCODE;
}

void div_reg_bin(void) {
    // Check if the op code bits and function code match for DIV
    // checkBits(start_bit, bit_string) returns 0 if the bit_string matches
    if (checkBits(31, "000000") != 0 || checkBits(5, "011010") != 0) {
        state = WRONG_COMMAND;
        return;
    }

    /*
        Extracting values from the binary
    */
    // getBits(start_bit, width)
    uint32_t Rs = getBits(25, 5);    // First source register (bits 25-21)
    uint32_t Rt = getBits(20, 5);    // Second source register (bits 20-16)

    /*
        Setting Instruction values
    */
    setOp("DIV");
    // Format: DIV $rs, $rt
    // Divides $rs by $rt, storing quotient in LO and remainder in HI
    setParam(1, REGISTER, Rs);    // First source register operand (dividend)
    setParam(2, REGISTER, Rt);    // Second source register operand (divisor)

    // Tell the system the decoding is done
    state = COMPLETE_DECODE;
}


