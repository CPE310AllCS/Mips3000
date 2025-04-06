/*
* Author: Tyler Lannon
* Date: 04/05/2025
* ByteForge Systems
* MIPS-Translatron 3000
*/

#include "Instruction.h"

void lui_immd_assm(void) {
    // Checking that the op code matches
    // strcmp(string1, string2) return 0 if they match
    if (strcmp(OP_CODE, "LUI") != 0) {
        // If the op code doesn't match, this isn't the correct command
        state = WRONG_COMMAND;
        return;
    }

    /*
        Checking the type of parameters
    */

    // The first parameter should be a register (Rt - target)
    if (PARAM1.type != REGISTER) {
        state = MISSING_REG;
        return;
    }
    
    // For LUI, we should only have 2 parameters (Rt and immediate)
    // The below check for PARAM2 being a register is incorrect and should be removed
    if (PARAM2.type != IMMEDIATE) {
        state = INVALID_PARAM;
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
    
    // The immediate value is limited to 16 bits (0xFFFF)
    if (PARAM2.value > 0xFFFF) {
        state = INVALID_IMMED;
        return;
    }

    /*
        Putting the binary together
    */

    // Set the opcode for LUI (001111)
    setBits_str(31, "001111");
    
    // Set Rt (target register) at bits 20-16
    setBits_num(20, PARAM1.value, 5);
    
    // Set Rs field to 0 (unused in LUI)
    setBits_num(25, 0, 5);
    
    // Set immediate value (16 bits)
    setBits_num(15, PARAM2.value, 16);

    // Tell the system the encoding is done
    state = COMPLETE_ENCODE;
}

void lui_immd_bin(void) {
    // Check if the op code bits match for LUI
    // checkBits(start_bit, bit_string) returns 0 if the bit_string matches
    if (checkBits(31, "001111") != 0) {
        state = WRONG_COMMAND;
        return;
    }

    /*
        Extracting values from the binary
    */
    // getBits(start_bit, width)
    uint32_t Rs = getBits(25, 5);    // Should be 0 for LUI
    uint32_t Rt = getBits(20, 5);    // Target register
    uint32_t imm16 = getBits(15, 16); // Immediate value

    /*
        Setting Instruction values
    */
    setOp("LUI");
    // Format: LUI $rt, imm
    // Loads imm into the upper 16 bits of $rt and sets lower 16 bits to 0
    setParam(1, REGISTER, Rt);    // Target register
    setParam(2, IMMEDIATE, imm16); // Immediate value to load into upper 16 bits
    
    // Tell the system the decoding is done
    state = COMPLETE_DECODE;
}



