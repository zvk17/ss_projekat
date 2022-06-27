//
// Created by user2 on 15.9.21..
//

#ifndef ASSEMBLER_INSTRUCTION_CODES_HPP
#define ASSEMBLER_INSTRUCTION_CODES_HPP

using byte = unsigned char;
constexpr byte HALT_OPCODE =           0b0000'0000;
constexpr byte INTERRUPT_OPCODE =      0b0001'0000;
constexpr byte IRET_OPCODE =           0b0010'0000;
constexpr byte CALL_OPCODE =           0b0011'0000;
constexpr byte RET_OPCODE =            0b0100'0000;

constexpr byte XCHG_OPCODE =           0b0110'0000;
constexpr byte NOT_OPCODE =            0b1000'0000;
constexpr byte ADD_OPCODE =            0b0111'0000;
constexpr byte JMP_OPCODE =            0b0101'0000;
constexpr byte SHL_OPCODE =            0b1001'0000;
constexpr byte LDR_OPCODE =            0b1010'0000;
constexpr byte STR_OPCODE =            0b1011'0000;

constexpr byte JEQ_OPCODE = JMP_OPCODE | 0b01;
constexpr byte JNE_OPCODE = JMP_OPCODE | 0b10;
constexpr byte JGT_OPCODE = JMP_OPCODE | 0b11;

constexpr byte SUB_OPCODE = ADD_OPCODE | 0b001;
constexpr byte MUL_OPCODE = ADD_OPCODE | 0b010;
constexpr byte DIV_OPCODE = ADD_OPCODE | 0b011;
constexpr byte CMP_OPCODE = ADD_OPCODE | 0b100;

constexpr byte AND_OPCODE = NOT_OPCODE |  0b001;
constexpr byte OR_OPCODE = NOT_OPCODE |   0b010;
constexpr byte XOR_OPCODE = NOT_OPCODE |  0b011;
constexpr byte TEST_OPCODE = NOT_OPCODE | 0b100;

constexpr byte SHR_OPCODE = SHL_OPCODE | 0b1;

constexpr byte NO_INCREMENT =  0b0000;
constexpr byte PRE_DECREMENT =  0b0001;
constexpr byte PRE_INCREMENT =  0b0010;
constexpr byte POST_INCREMENT = 0b0011;
constexpr byte POST_DECREMENT = 0b0100;

constexpr byte IMMEDIATE =                      0b0000;
constexpr byte REGISTER_DIRECT =                0b0001;
constexpr byte REGISTER_DIRECT_DISPLACEMENT =   0b0101;
constexpr byte REGISTER_INDIRECT =              0b0010;
constexpr byte REGISTER_INDIRECT_DISPLACEMENT = 0b0011;
constexpr byte MEMORY =                         0b0100;


#endif //ASSEMBLER_INSTRUCTION_CODES_HPP
