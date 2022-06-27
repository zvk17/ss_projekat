//
// Created by user2 on 16.9.21..
//

#include "AssemblyException.hpp"

AssemblyException::AssemblyException(const std::string &message) :
    message(message)
{}

const std::string &AssemblyException::getMessage() const {
    return message;
}
