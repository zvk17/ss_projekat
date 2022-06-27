//
// Created by user2 on 19.9.21..
//

#ifndef EMULATOR_HELPER_HPP
#define EMULATOR_HELPER_HPP

#include <termios.h>

class Helper {
private:
    static struct termios originalTermios;
    static bool isSet;
public:
    static void setTerminal();
    static char readTerminal();
    static void resetTerminal();
    static void exit();
};


#endif //EMULATOR_HELPER_HPP
