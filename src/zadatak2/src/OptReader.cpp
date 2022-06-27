//
// Created by user2 on 17.9.21..
//

#include "OptReader.hpp"
#include <unistd.h>
#include <getopt.h>
#include <cstdio>
#include <iostream>
#include <regex>

OptReader::OptReader(int argc, char **argv):
    linkable(false),
    hex(false)
{
    std::string inputFile;
    static struct option long_options[] =
    {
            {"o", required_argument, NULL, 'o'},
            {"place", required_argument, NULL, 'p'},
            {"hex", no_argument, NULL, 'h'},
            {"linkable", no_argument, NULL, 'l'}
    };
    char ch;
    while ((ch = getopt_long_only(argc, argv, ":o:p:hl", long_options, NULL)) != -1) {
        // check to see if a single character or long option came through
        switch (ch) {
            case 'h':
                hex = true;
                break;
            case 'l':
                linkable = true;
                break;
            case 'p':
                parseSectionPlacement(optarg);
                break;
            case 'o':
                outputFile = optarg;
                break;
            default:
                std::cout << ch;
                std::cout << "Unexpected argument\n";
                exit(__LINE__);
                break;
        }
    }
    argc -= optind;
    argv += optind;
    if (argc > 0) {
        for (int i = 0; i < argc; i++) {
            filePaths.push_back(argv[i]);
        }
    } else {
        std::cout << "All arguments required\n";
        std::exit(__LINE__);
    }
}

const std::list<std::string> &OptReader::getFilePaths() const {
    return filePaths;
}

bool OptReader::isHex() const {
    return hex;
}

bool OptReader::isLinkable() const {
    return linkable;
}

const std::string &OptReader::getOutputFile() const {
    return outputFile;
}

void OptReader::parseSectionPlacement(std::string optarg) {
    //printf("oarg %s\n", optarg.c_str());
    std::regex rx("(.*)@(0x[0-9a-fA-F]+)");
    std::smatch result;
    if (regex_match(optarg, result, rx)) {
        std::string section = result.str(1);
        int offset = std::stoi(result.str(2), nullptr, 16) & 0xFFFF;
        sections.push_back({section, offset});
    } else {
        std::cout << "Bad format: place argument";
        exit(__LINE__);
    }

}

std::list<std::pair<std::string, short>> &OptReader::getSections()  {
    return sections;
}
