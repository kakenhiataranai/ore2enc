#!/bin/sh

g++ ore2enc.cpp -std=c++17 -o ore2enc -O3
x86_64-w64-mingw32-g++ -std=c++17 ore2enc.cpp -static -o ore2enc.exe -O3
