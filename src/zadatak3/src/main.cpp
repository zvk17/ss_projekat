#include <iostream>
#include <Memory.hpp>
#include <Helper.hpp>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Wrong argument format";
        exit(__LINE__);
    }

    Helper::setTerminal();
    Memory* m = Memory::getInstance();
    m->loadFile(argv[1]);
    m->run();

    Helper::resetTerminal();
    return 0;
}
