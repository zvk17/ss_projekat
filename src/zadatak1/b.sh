flex -o "./src/lex.yy.c" "./src/flex.lex"

g++ -o asembler -I ./inc "./src/lex.yy.c" ./src/*.cpp  -std=c++14
