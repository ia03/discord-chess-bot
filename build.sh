#!/bin/bash
swig -c++ -python chessbot.i 
g++ -fPIC -c *.h *.cpp lib/*.h lib/*.cpp -I/usr/include/python3.6
gcc -shared *.o -o _chessbot.so -lstdc++
