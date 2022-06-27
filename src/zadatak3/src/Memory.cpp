//
// Created by user2 on 18.9.21..
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <expat.h>
#include <instruction_codes.hpp>
#include <chrono>
#include <Helper.hpp>
#include "Memory.hpp"
int const Memory::MEMORY_SIZE = 0xFFFF;
int const Memory::TIM_CFG_ADDRESS = 0xFF10;
int const Memory::TERM_OUT_ADDRESS = 0xFF00;
int const Memory::TERM_IN_ADDRESS = 0xFF02;
int const Memory::IVT_TIMER = 2;
int const Memory::IVT_TERMINAL = 3;

Memory* Memory::instance = nullptr;

Memory *Memory::getInstance() {
    if (instance == nullptr) {
        instance = new Memory();
    }
    return instance;
}

Memory::Memory():
        ram(nullptr),
        timerWait(500),
        startTime(0)
{
    ram = new byte[MEMORY_SIZE];
    if (ram == nullptr) {
        Helper::resetTerminal();
        printf("Cannot allocate memory");
        exit(__LINE__);
    }
    for (int i = 0; i < MEMORY_SIZE; i++) {
       ram[i] = 0;
    }

    for (int i = 0; i < 8; i++) {
       signalsInterrupt[i] = 0;
    }
    for (int i = 0; i < 5; i++) {
       instructionBytes[i] = 0;
    }
}

Memory::~Memory() {
    delete ram;
    ram = nullptr;
}

void Memory::loadFile(std::string filePath) {
    std::ifstream in(filePath);

    if (!in.is_open()) {
        Helper::resetTerminal();
        printf("Cannot open input file\n");
        exit(__LINE__);
    }
    std::string line;
    while (std::getline(in,line)) {
        if (line.length() == 0) {
            continue;
        }
        std::stringstream stream(line);
        std::string strOffset;
        std::string strByte;
        stream >> strOffset;
        strOffset = strOffset.substr(0, 4);
        int offset = hexToInt(strOffset);

        for (int i = 0; i < 8; i++) {
            stream >> strByte;
            int b = hexToInt(strByte);
            if (b != 0) {
                byte x = (byte)(b & 0xFF);
                ram[offset + i] = x;
            }
        }

    }



    in.close();
}

int Memory::hexToInt(std::string const& strOffset) {
    return std::stoi(strOffset, nullptr, 16) & 0xFFFF;
}

void Memory::run() {
    startTime = getCurrentTime();
    setSP(0xE000);
    setPC(readLitteEndian(0));
    bool running = 1;
    while (running) {
        instructionBytes[0] = readBytePC();
        switch (instructionBytes[0]) {
            case HALT_OPCODE:
                running = false;
                break;
            case INTERRUPT_OPCODE:
                instructionBytes[1] = readBytePC();
                doInterrupt();
                break;
            case IRET_OPCODE:
                //std::cout << "before iret routine " << std::dec << (int)getI() << " " << getPC() << "  " << getSP() << " " << std::hex << getPSW() <<"\n";
                doIretInstruction();
                //std::cout << "after iret routine " << std::dec << (int)getI() << " " << getPC() << "  " << getSP() << " " << std::hex << getPSW() <<"\n";
                //fflush(stdout);
                break;
            case RET_OPCODE:
                registers[(int)RegisterType::PC] = popStack();
                break;
            case CALL_OPCODE:
            case JMP_OPCODE:
            case JGT_OPCODE:
            case JNE_OPCODE:
            case JEQ_OPCODE:
                instructionBytes[1] = readBytePC();
                instructionBytes[2] = readBytePC();
                jumpInstructions();
                break;
            case XCHG_OPCODE:
                instructionBytes[1] = readBytePC();
                exchangeInstruction();
                break;
            case ADD_OPCODE:
            case SUB_OPCODE:
            case MUL_OPCODE:
            case DIV_OPCODE:
            case CMP_OPCODE:
                instructionBytes[1] = readBytePC();
                aritInstruction();
                break;
            case NOT_OPCODE:
            case AND_OPCODE:
            case OR_OPCODE:
            case XOR_OPCODE:
            case TEST_OPCODE:
                instructionBytes[1] = readBytePC();
                logicInstruction();
                break;
            case SHR_OPCODE:
                instructionBytes[1] = readBytePC();
                shiftRightInstruction();
                break;
            case SHL_OPCODE:
                instructionBytes[1] = readBytePC();
                shiftLeftInstruction();
                break;
            case LDR_OPCODE:
                instructionBytes[1] = readBytePC();
                instructionBytes[2] = readBytePC();
                loadStoreInstruction(LoadStoreType::LOAD);
                break;
            case STR_OPCODE:
                //std::cout << "STORE " << std::dec<<getPC() << "\n";
                instructionBytes[1] = readBytePC();
                instructionBytes[2] = readBytePC();
                loadStoreInstruction(LoadStoreType::STORE);
                break;
            default:
                //todo isto kao kod neispravnog store
                //todo not tested!!
                invalidInstruction();
                break;
        }
        if (!getI()) {
            if (!getTerminal() && signalsInterrupt[IVT_TERMINAL]) {
                //std::cout << "TERMINAL before routine " << std::dec << (int)getI() << " " << getPC() << "  " << getSP() << " " << std::hex << getPSW() <<"\n";
                signalsInterrupt[IVT_TERMINAL] = false;
                interruptIVT(IVT_TERMINAL);
                setI(true);
                //std::cout << "TERMINAL after routine " << std::dec << (int)getI() << " " << getPC() << "  " << getSP() << " " << std::hex << getPSW() <<"\n";
               // fflush(stdout);
            } else if (!getTimer() && signalsInterrupt[IVT_TIMER]) {

                //Helper::resetTerminal();
                //printf("f");
                //Helper::setTerminal();
                //std::cout << "timer before routine " << std::dec << (int)getI() << " " << getPC() << "  " << getSP() << " " << std::hex << getPSW() <<"\n";
                signalsInterrupt[IVT_TIMER] = false;
                interruptIVT(IVT_TIMER);
                setI(true);
                //std::cout << "timer after routine " << std::dec << (int)getI() << " " << getPC() << "  " << getSP() << " " << std::hex << getPSW() <<"\n";

            }
        }
        //timer app
        timerApp();
        terminalApp();
    }
}

inline word Memory::getRegister(RegisterType reg) const {
    return registers[(int)reg];
}
inline word Memory::getRegister(int reg) const {
    return registers[reg];
}
inline void Memory::setRegister(RegisterType reg, word value) {
    registers[(int)reg] = value;
}
inline void Memory::setRegister(int reg, word value) {
    registers[reg] = value;
}

word Memory::getPSW() const {
    return getRegister(RegisterType::PSW);
}

void Memory::setPSW(word val) {
    setRegister(RegisterType::PSW, val);
}

inline word Memory::getPC() const {
    return getRegister(RegisterType::PC);
}

inline void Memory::setPC(word val) {
    setRegister(RegisterType::PC, val);
}

inline void Memory::setSP(word val) {
    setRegister(RegisterType::SP, val);
}

inline word Memory::getSP() const {
    return getRegister(RegisterType::SP);
}

inline short Memory::readLitteEndian(int offset) const{
    byte first = ram[offset];
    byte second = ram[offset + 1];
    short num = first | (unsigned short)second << 8;
    return num;
}
inline short Memory::readBigEndian(byte *ptr, int offset) const{
    byte first = ptr[offset];
    byte second = ptr[offset + 1];
    short num = second | (unsigned short)first << 8;
    return num;
}

inline void Memory::writeLittleEndian(int offset, short value) {
    byte lower = (short)( (unsigned short)value & 0xFF);
    byte upper = (short)( ((unsigned short)value >> 8) & 0xFF);
    ram[offset] = lower;
    ram[offset + 1] = upper;
    //std::cout << "WRITE at " <<std::dec << offset << " " << value << std::endl;
}

inline byte Memory::readBytePC() {
    return ram[ registers[(int)RegisterType::PC]++ ];
}

void Memory::doIretInstruction() {
    setPSW(popStack());
    setPC(popStack());
}

void Memory::doInterrupt() {
    //std::cout << "TODO intterupt\n" << __LINE__ << "\n";
    int index = getRegister(getInstructionRegisters().destination) % 8;
    interruptIVT(index);
}

inline void Memory::pushStack(word val) {
    word& b = registers[(int)RegisterType::SP];
    b -= 2;
    writeLittleEndian(b, val);
    //std::cout << "PUSH " << std::dec << readLitteEndian(b) << "\n";
}
inline word Memory::popStack() {
    word& b = registers[(int)RegisterType::SP];
    word w = readLitteEndian(b);
    b += 2;
    //std::cout << "POP " << std::dec << w << "\n";
    return w;
}
inline RegisterOperation Memory::getInstructionRegisters() {
    RegisterOperation ro;
    int destination = instructionBytes[1] >> 4;
    int source = instructionBytes[1] & 0xF;
    ro.destination = (RegisterType)destination;
    ro.source = (RegisterType)source;
    return ro;
}

void Memory::aritInstruction() {
    RegisterOperation && ro = getInstructionRegisters();
    word& a = registers[(int)ro.destination];
    word& b = registers[(int)ro.source];
    switch (instructionBytes[0]) {
        case ADD_OPCODE:
            a += b;
            break;
        case SUB_OPCODE:
            a -= b;
            break;
        case MUL_OPCODE:
            a *= b;
            break;
        case DIV_OPCODE:
            a /= b;
            break;
        case CMP_OPCODE:
            setZ(a == b);
            setN((a - b) < 0);
            setC(a < b);
            setO(  (a > 0 && b < 0 && (a-b < 0)) || (a < 0 && b > 0 && (a-b > 0))  );
            break;
    }
}
void Memory::logicInstruction() {
    RegisterOperation && ro = getInstructionRegisters();
    word& a = registers[(int)ro.destination];
    word& b = registers[(int)ro.source % 9];
    switch (instructionBytes[0]) {
        case NOT_OPCODE:
            a = ~a;
            break;
        case AND_OPCODE:
            a &= b;
            break;
        case OR_OPCODE:
            a |= b;
            break;
        case XOR_OPCODE:
            a ^= b;
            break;
        case TEST_OPCODE:
            setZ((a&b) == 0);
            setN((short)(a&b) < 0);
            break;
    }
}
void Memory::shiftRightInstruction() {
    RegisterOperation && ro = getInstructionRegisters();
    word& a = registers[(int)ro.destination];
    word& b = registers[(int)ro.source];

    setC((a & 1) != 0);

    a >>= b;

    setN((short)a < 0);
    setZ(a == 0);
}
void Memory::shiftLeftInstruction() {
    RegisterOperation && ro = getInstructionRegisters();
    word& a = registers[(int)ro.destination];
    word& b = registers[(int)ro.source];

    setC(((a >> 15) & 1) != 0);
    a <<= b;

    setN((short)a < 0);
    setZ(a == 0);
}
void Memory::exchangeInstruction() {
    RegisterOperation&& ro = getInstructionRegisters();
    word& a = registers[(int)ro.destination];
    word& b = registers[(int)ro.source];
    word temp = a;
    a = b;
    b = temp;
}

inline void Memory::setZ(bool b) {
    setPSWNthBit(b, 0);
}
inline bool Memory::getZ() {
    return getPSWNthBit(0);
}

inline bool Memory::getPSWNthBit(int n) {
    word psw = registers[(int)RegisterType::PSW];
    psw &= (1 << n);
    return psw != 0;
}

inline void Memory::setN(bool b) {
    setPSWNthBit(b, 3);
}
inline bool Memory::getN() {
    return getPSWNthBit(3);
}

inline void Memory::setC(bool b) {
    setPSWNthBit(b, 2);
}
inline bool Memory::getC() {
    return getPSWNthBit(2);
}
inline void Memory::setO(bool b) {
    setPSWNthBit(b, 1);
}
inline bool Memory::getO() {
    return getPSWNthBit(1);
}
inline void Memory::setI(bool b) {
    setPSWNthBit(b, 15);
}
inline bool Memory::getI() {
    return getPSWNthBit(15);
}

inline void Memory::setTerminal(bool b) {
    setPSWNthBit(b, 14);
}
inline bool Memory::getTerminal() {
    return getPSWNthBit(14);
}
inline void Memory::setTimer(bool b) {
    setPSWNthBit(b, 13);
}
inline bool Memory::getTimer() {
    return getPSWNthBit(13);
}

inline void Memory::setPSWNthBit(bool b, int n) {
    word& psw = registers[(int)RegisterType::PSW];
    psw &= ~(1 << n);
    psw |= (b << n);
}
inline int Memory::getIncrementionType() {
    return (instructionBytes[2] >> 4) & 0xF;
}
inline int Memory::getAddressingType() {
    return instructionBytes[2] & 0xF;
}
void Memory::loadStoreInstruction(LoadStoreType lst) {

    loadAdditionalBytes();
    RegisterOperation&& ro = getInstructionRegisters();
    word& destination = registers[(int)ro.destination];
    word& source = registers[(int)ro.source % 9];
    word operand;

    int incrementType = getIncrementionType();
    if (incrementType == PRE_DECREMENT) {
        source -= 2;
    }
    if (incrementType == PRE_INCREMENT) {
        source += 2;
    }

    if (LoadStoreType::LOAD == lst) {
        switch (getAddressingType()) {
            case REGISTER_DIRECT:
                destination = source;
                break;
            case REGISTER_INDIRECT:
                destination = readLitteEndian(source);
                break;
            case IMMEDIATE:
                destination = readBigEndian(instructionBytes, 3);
                break;
            case REGISTER_DIRECT_DISPLACEMENT:
                operand = readBigEndian(instructionBytes, 3);
                destination = source + operand;
                break;
            case REGISTER_INDIRECT_DISPLACEMENT:
                operand = readBigEndian(instructionBytes, 3);
                destination = readLitteEndian(operand + source);
                break;
            case MEMORY:
                operand = readBigEndian(instructionBytes, 3);
                destination = readLitteEndian(operand);
                break;
        }
    } else {//else store
        switch (getAddressingType()) {
            case REGISTER_DIRECT:
                source = destination;
                break;
            case REGISTER_INDIRECT:
                writeLittleEndian(source, destination);
                break;
            case IMMEDIATE:
            case REGISTER_DIRECT_DISPLACEMENT:
                //todo not tested!!
                invalidInstruction();
                //printf("immediate store error");
                //fflush(stdout);
                break;
            case MEMORY:
                operand = readBigEndian(instructionBytes, 3);
                writeLittleEndian(operand, destination);
                break;
            case REGISTER_INDIRECT_DISPLACEMENT:
                operand = readBigEndian(instructionBytes, 3);
                writeLittleEndian(operand + source, destination);
                break;
        }
        //std::cout << "OUTSIDE lStore\n";
    }


    if (incrementType == POST_DECREMENT) {
        source -= 2;
    }
    if (incrementType == POST_INCREMENT) {
        source += 2;
    }

}
void Memory::jumpInstructions() {
    loadAdditionalBytes();

    RegisterOperation&& ro = getInstructionRegisters();
    word& destination = registers[(int)ro.destination];
    word& source = registers[(int)ro.source % 9];
    word operand;
    bool shouldJump = false;

    switch (instructionBytes[0]) {
        case CALL_OPCODE:
            shouldJump = true;
            pushStack(getPC());
            break;
        case JMP_OPCODE:
            shouldJump = true;
            break;
        case JEQ_OPCODE:
            shouldJump = getZ();
            break;
        case JNE_OPCODE:
            shouldJump = !getZ();
            break;
        case JGT_OPCODE:
            //da li radi??
            shouldJump = (!(getN() ^ getO())) && (!getZ());
            //printf("TODO JGT");
            break;
    }
    if (!shouldJump) {
        return;
    }


    //todo jumps
    int incrementType = getIncrementionType();


    if (incrementType == PRE_DECREMENT) {
        source -= 2;
    }
    if (incrementType == PRE_INCREMENT) {
        source += 2;
    }

    //todo jump
    switch (getAddressingType()) {
        case IMMEDIATE:
            setPC(readBigEndian(instructionBytes,+3));
            break;
        case REGISTER_DIRECT:
            setPC(source);
            break;
        case REGISTER_INDIRECT:
            setPC(readLitteEndian(source));
            break;
        case MEMORY:
            operand = readBigEndian(instructionBytes,+3);
            setPC(readLitteEndian(operand));
            break;
        case REGISTER_DIRECT_DISPLACEMENT:
            setPC(readBigEndian(instructionBytes,+3) + source);
            break;
        case REGISTER_INDIRECT_DISPLACEMENT:
            operand = readBigEndian(instructionBytes,+3) + source;
            setPC(readLitteEndian(operand));
            break;
    }


    if (incrementType == POST_DECREMENT) {
        source -= 2;
    }
    if (incrementType == POST_INCREMENT) {
        source += 2;
    }

}

void Memory::loadAdditionalBytes() {
    switch (getAddressingType()) {
        case IMMEDIATE:
        case MEMORY:
        case REGISTER_DIRECT_DISPLACEMENT:
        case REGISTER_INDIRECT_DISPLACEMENT:
            instructionBytes[3] = readBytePC();
            instructionBytes[4] = readBytePC();
            break;
    }
}
void Memory::invalidInstruction() {
    interruptIVT(1);
}
void Memory::interruptIVT(int index) {
    word address = readLitteEndian(index * 2);
    if (address == 0) {
        Helper::resetTerminal();
        std::cout << "no interrupt routine for IVT index " << index;
        fflush(stdout);
        exit(__LINE__);
    }
    pushStack(getPC());
    pushStack(getPSW());
    setPC(address);
}
void Memory::signal(int i) {
    signalsInterrupt[i % 8] = true;
}
unsigned long long Memory::getCurrentTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
void Memory::timerApp() {

    unsigned long long currentTime = getCurrentTime();
    //std::cout << currentTime << "\n";
    unsigned long long delta = currentTime - startTime;
    if (delta < timerWait) {
        return;
    }
    //std::cout << delta << " " << currentTime << "\n";
    //printf(",d,");
    signal(IVT_TIMER);
    startTime = currentTime;
    calculateTimerWait();
}

void Memory::calculateTimerWait() {
    word data = readLitteEndian(Memory::TIM_CFG_ADDRESS);
    switch (data) {
        case 0:
        case 1:
        case 2:
        case 3:
            timerWait = 500 * (data + 1);
            break;
        case 4:
            timerWait = 5000;
            break;
        case 5:
            timerWait = 10 * 1000;
            break;
        case 6:
            timerWait = 30 * 1000;
            break;
        case 7:
            timerWait = 60 * 1000;
            break;
        default:
            timerWait = 500;
            break;
    }
}

void Memory::terminalApp() {

    word w = readLitteEndian(Memory::TERM_OUT_ADDRESS);
    char c = (w & 0xFF);
    if (w != 0) {
        printf("%c", c);
        fflush(stdout);
        writeLittleEndian(Memory::TERM_OUT_ADDRESS, 0);
    }
    //todo read signal
    char ch = Helper::readTerminal();
    w = ch;
    if (ch) {
        //printf("|%c|",ch);
        //fflush(stdout);
        writeLittleEndian(Memory::TERM_IN_ADDRESS, w);
        signal(Memory::IVT_TERMINAL);
    }
}
