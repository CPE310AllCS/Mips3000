/*
* Author: Tyler Lannon
* Date: 04/05/2025
* ByteForge Systems
* MIPS-Translatron 3000
*/

#include "Instruction.h"

void sw_immd_assm(void) {
    // Checking that the op code matches
    // strcmp(string1, string2) return 0 if they match
    if (strcmp(OP_CODE, "SW") != 0) {
        // If the op code doesn't match, this isn't the correct command
        state = WRONG_COMMAND;
        return;
    }

    /*
        Checking the type of parameters
    */

    // The first parameter should be a register (Rt)
    if (PARAM1.type != REGISTER) {
        state = MISSING_REG;
        return;
    }

    // Param 2 needs to be an immediate (offset)
    if (PARAM2.type != IMMEDIATE) {
        state = INVALID_PARAM;
        return;
    }

    // Param 3 needs to be a register (Rs)
    if (PARAM3.type != REGISTER) {
        state = MISSING_REG;
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

    // The immediate value is limited to 16 bits, this is 0xFFFF
    if (PARAM2.value > 0xFFFF) {
        state = INVALID_IMMED;
        return;
    }

    // Rs should be 31 or less
    if (PARAM3.value > 31) {
        state = INVALID_REG;
        return;
    }

    /*
        Putting the binary together
    */

    // Set the opcode for SW (101011)
    setBits_str(31, "101011");
    
    // Set Rs (base register)
    setBits_num(25, PARAM3.value, 5);
    
    // Set Rt (source register)
    setBits_num(20, PARAM1.value, 5);
    
    // Set immediate offset
    setBits_num(15, PARAM2.value, 16);

    // Tell the system the encoding is done
    state = COMPLETE_ENCODE;
}

void sw_immd_bin(void) {
    // Check if the op code bits match for SW (101011)
    // checkBits(start_bit, bit_string) returns 0 if the bit_string matches
    if (checkBits(31, "101011") != 0) {
        state = WRONG_COMMAND;
        return;
    }

    /*
        Extracting values from the binary
    */
    // getBits(start_bit, width)
    uint32_t Rs = getBits(25, 5);    // Base register
    uint32_t Rt = getBits(20, 5);    // Source register
    uint32_t offset = getBits(15, 16); // Immediate offset

    /*
        Setting Instruction values
    */
    setOp("SW");
    // Format: SW $rt, offset($rs)
    setParam(1, REGISTER, Rt);    // Source register
    setParam(2, IMMEDIATE, offset); // Immediate offset
    setParam(3, REGISTER, Rs);    // Base register

    // Tell the system the decoding is done
    state = COMPLETE_DECODE;
}