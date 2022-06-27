//
// Created by user2 on 19.9.21..
//

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#include "Helper.hpp"

bool Helper::isSet = false;
struct termios Helper::originalTermios;

void Helper::setTerminal() {
    if (!isSet) {
        isSet = true;
        tcgetattr(0, &originalTermios);
        struct termios newTermios;
        newTermios = originalTermios;

        newTermios.c_lflag &= ~ICANON;
        newTermios.c_lflag &= ~ECHO;
        newTermios.c_cc[VMIN] = 0;
        newTermios.c_cc[VTIME] = 0;
        tcsetattr(0, TCSANOW, &newTermios);

    }
}

void Helper::resetTerminal() {
    if (isSet) {
        tcsetattr(0, TCSANOW, &originalTermios);
    }
}

char Helper::readTerminal() {
    unsigned char c = 0;
    read(0, &c, sizeof(c));
    return c;
}

