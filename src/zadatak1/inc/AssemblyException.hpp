//
// Created by user2 on 16.9.21..
//

#ifndef ASSEMBLER_ASSEMBLYEXCEPTION_HPP
#define ASSEMBLER_ASSEMBLYEXCEPTION_HPP


#include <string>

class AssemblyException: std::exception {
    std::string message;
public:
    AssemblyException(const std::string &message);
    const std::string &getMessage() const;
};


#endif //ASSEMBLER_ASSEMBLYEXCEPTION_HPP
