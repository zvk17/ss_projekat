//
// Created by user2 on 12.9.21..
//

#ifndef ASSEMBLER_TYPES_HPP
#define ASSEMBLER_TYPES_HPP
using byte = unsigned char;
enum class RegisterType {
    R0, R1, R2, R3, R4, R5, SP, PC, PSW
};

typedef struct {
    RegisterType destination;
    RegisterType source;

    byte toByte() {
        return ((byte)destination << 4) | (byte)source;
    }
} RegisterOperation;

#endif //ASSEMBLER_TYPES_HPP
