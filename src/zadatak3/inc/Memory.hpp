//
// Created by user2 on 18.9.21..
//

#ifndef EMULATOR_MEMORY_HPP
#define EMULATOR_MEMORY_HPP

#include <string>
#include "types.hpp"

using byte = unsigned char;

class Memory {
    Memory( );
    static Memory* instance;
    byte* ram;
    byte instructionBytes[5];
    word registers[9];
    bool signalsInterrupt[8];

    static int hexToInt(std::string const& strOffset);


    unsigned long long timerWait;
    unsigned long long startTime;

public:
    enum class LoadStoreType {
        LOAD,
        STORE
    };
    static int const MEMORY_SIZE;
    static int const TIM_CFG_ADDRESS;
    static int const TERM_OUT_ADDRESS;
    static int const TERM_IN_ADDRESS;

    static Memory* getInstance( );
    void loadFile(std::string filePath);
    void run();
    ~Memory( );


    short readLitteEndian(int offset) const;
    short readBigEndian(byte *ptr, int offset) const;
    void writeLittleEndian(int offset, short value);

    byte readBytePC();

    void pushStack(word val);


    word getRegister(RegisterType reg) const;
    word getRegister(int reg) const;
    word getPSW() const;
    word getPC() const;
    word getSP() const;


    void setPSW(word val);
    void setPC(word val);
    void setSP(word val);
    void setRegister(int reg, word value);
    void setRegister(RegisterType reg, word value);


    void doIretInstruction();

    void doInterrupt();

    void jumpInstructions();

    word popStack();

    RegisterOperation getInstructionRegisters();

    void aritInstruction();

    void setZ(bool b);
    void setN(bool b);
    void setC(bool b);

    void setPSWNthBit(bool b, int i);

    void setO(bool b);
    void setTimer(bool b);
    void setTerminal(bool b);
    void setI(bool b);

    void logicInstruction();

    void exchangeInstruction();

    void shiftRightInstruction();

    void shiftLeftInstruction();

    int getAddressingType();

    int getIncrementionType();

    void loadStoreInstruction(LoadStoreType lst);

    void loadAdditionalBytes();

    bool getZ();

    bool getPSWNthBit(int n);

    bool getO();
    bool getN();
    bool getC();

    bool getI();

    bool getTerminal();

    bool getTimer();


    void invalidInstruction();

    void interruptIVT(int index);

    void signal(int i);

    void timerApp();


    static unsigned long long int getCurrentTime();

    static const int IVT_TERMINAL;
    static const int IVT_TIMER;

    void calculateTimerWait();

    void terminalApp();
};


#endif //EMULATOR_MEMORY_HPP
