#include "Map.h"
#include "Buffer.h"
#include "Simulator.h"
#include <iostream>
#include <fstream>

int main() {
    try {
        Simulator simulator(10, 20);

        simulator.start();

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}