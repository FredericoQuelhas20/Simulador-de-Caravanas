//
// Created by frede on 30/11/2024.
//

#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "Map.h"


class Simulator {
    bool running;
    Map map;
    Buffer buffer;

public:
    Simulator(int rows, int columns);
    void spawnBarbarian(int i, int c);
    void processCommand(const std::string& command);
    void simulateTurn();
    void start();
    void processCommandsFromFile(const std::string& filename);
};



#endif //SIMULATOR_H
