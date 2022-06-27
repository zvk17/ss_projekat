//
// Created by user2 on 18.9.21..
//

#ifndef LINKER_MOVESECTIONS_HPP
#define LINKER_MOVESECTIONS_HPP


#include "DataTable.hpp"

class MoveSections {
public:
    typedef struct intervalStruct{
        int start;
        int end;
        std::string sectionName;
        friend bool operator<(struct intervalStruct const& a, struct intervalStruct const&b);
    } Interval;
    static void checkValidity(DataTable &table, std::list<std::pair<std::string, short> > const& sections);
    static void moveSections(DataTable &table, std::list<std::pair<std::string, short> > const& sections);
    static void moveSymbols(DataTable &table);
    static void doRelocations(DataTable &table);

    static void move(DataTable &table, std::list<std::pair<std::string, short>> &sections);
};


#endif //LINKER_MOVESECTIONS_HPP
