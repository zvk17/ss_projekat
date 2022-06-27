//
// Created by user2 on 17.9.21..
//

#ifndef LINKER_OPTREADER_HPP
#define LINKER_OPTREADER_HPP


#include <list>
#include <string>

class OptReader {
    std::list<std::string> filePaths;
    std::list<std::pair<std::string, short> > sections;
public:
    std::list<std::pair<std::string, short>> &getSections();

private:
    bool hex;
public:
    bool isHex() const;

    bool isLinkable() const;

    const std::string &getOutputFile() const;

private:
    bool linkable;
    std::string outputFile;
public:
    OptReader(int argc, char *argv[]);

    const std::list<std::string> &getFilePaths() const;

    void parseSectionPlacement(std::string optarg);
};


#endif //LINKER_OPTREADER_HPP
