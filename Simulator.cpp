#include "Simulator.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "Map.h"
#include "City.h"

Simulator::Simulator(int rows, int columns)
    : running(true), map(rows, columns), buffer(rows, columns) {}

void Simulator::processCommand(const std::string& command) { //Completar os comandos e verficar ainda comandos
    std::istringstream stream(command);
    std::string cmd;
    stream >> cmd;

    try {
          if (cmd == "config") {
            std::string filename;
            stream >> filename;
            map.loadMap(filename);
            buffer.clear();
            map.render(buffer);
            buffer.render();// Load map from file
        }else if (cmd == "move") {
            char caravanId;
            std::string direction;
            stream >> caravanId >> direction;
            try {
                map.moveCaravan(caravanId, direction);
                buffer.clear();
                map.render(buffer);
                buffer.render();
            } catch(const std::runtime_error& e)
            {
                std::cerr << "Erro ao mover a caravana: " << e.what() << std::endl;
            }

        } else if (cmd == "prox") {
            int turns = 1; // Default to 1 if no number is given
            if(stream >> turns)
            {

            }
            for (int i = 0; i < (turns > 0 ? turns : 1); ++i) {
                simulateTurn();
            }
        }else if(cmd == "precos")
        {
            map.listPrices();
        }else if(cmd == "cidade")
        {
             char cityId;
            stream >> cityId;
            map.listCityInfo(cityId);
        }else if(cmd == "remove")
        {
            char caravanId;
            stream >> caravanId;
            map.removeCaravan(caravanId);
            buffer.clear();
            map.render(buffer);
            buffer.render();
        }else if(cmd == "caravana")
        {
            char caravanId;
            stream >> caravanId;
            map.listCaravanInfo(caravanId);
        } else if (cmd == "comprac") {
            char cityId, type;
            stream >> cityId >> type;
            map.buyCaravan(cityId, type); // Métodos específicos do mapa
             buffer.clear();
             map.render(buffer);
             buffer.render();
        } else if (cmd == "compra") {
            char caravanId;
            int amount;
            stream >> caravanId >> amount;
            map.buyGoods(caravanId, amount);
             buffer.clear();
             map.render(buffer);
             buffer.render();
        }else if (cmd == "vende") {
            char caravanId;
            stream >> caravanId;
            map.sellGoods(caravanId);
             buffer.clear();
             map.render(buffer);
             buffer.render();
        }else if(cmd == "moedas"){
            int amount;
            stream >> amount;
            map.addCoins(amount);
            std::cout << "Moedas: " << map.getCoins() << endl;
        }else if (cmd == "tripul") {
           char caravanId;
            int amount;
            stream >> caravanId >> amount;
            map.hireCrew(caravanId, amount);
             buffer.clear();
             map.render(buffer);
             buffer.render();
        } else if (cmd == "barbaro") {
            int l, c;
            stream >> l >> c;
            spawnBarbarian(l, c);
            buffer.clear();
            map.render(buffer);
            buffer.render();
        }
        else  if (cmd == "saves") {
            std::string bufferName;
            stream >> bufferName;
             buffer.saveBuffer(bufferName);
        }else if (cmd == "loads") {
             std::string bufferName;
            stream >> bufferName;
            //buffer.clear();
            buffer.loadBuffer(bufferName);
            //map.render(buffer);
            //buffer.render();
        }else if (cmd == "lists") {
            buffer.listBuffer();
        }else if (cmd == "dels") {
            std::string bufferName;
             stream >> bufferName;
           buffer.deleteBuffer(bufferName);
        }else if(cmd == "auto")
       {
         char caravanId;
            stream >> caravanId;
            map.setAutoMode(caravanId, true);
        }else if(cmd == "stop")
       {
         char caravanId;
            stream >> caravanId;
            map.setAutoMode(caravanId, false);
        }else if (cmd == "areia") {
            int x, y, radius;
            stream >> x >> y >> radius;
           map.handleStorm(x, y, radius);
        }

          else if(cmd == "exec")
        {
                std::string filename;
                stream >> filename;
                processCommandsFromFile(filename);
        }else if (cmd == "terminar") {
            std::cout << "Simulaçao terminada." << std::endl;
            std::cout << "Pontuaçao final: " << map.getCoins() << " moedas." << std::endl;
        } else if (cmd == "sair") {
            running = false;
        }else {
           std::cout << "Comando desconhecido: " << cmd << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro ao processar comando: " << e.what() << std::endl;
    }
}

void Simulator::simulateTurn() {
    map.updateAutonomousBehaviors();
    map.spawnItem();
    buffer.clear();
    map.render(buffer);
    buffer.render();
    map.setCurrentTurn();
    map.removeExpiredItems();
    map.handleItemPickup();
}


void Simulator::start() {
    std::cout << "Bem-vindo ao Simulador de Viagens no Deserto!" << std::endl;
    buffer.clear();
    map.render(buffer); // Renderizar mapa inicial
    buffer.render();

    while (running) {
        std::string command;
        std::cout << "> ";
        std::getline(std::cin, command);

        if (!command.empty()) {
            processCommand(command);
        }
    }

    std::cout << "Simulador terminado. Obrigado por jogar!" << std::endl;
}

void Simulator::processCommandsFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << "Comando do arquivo '" << filename << "': " << line << std::endl; // Exibe o comando lido (opcional)

        // Remove espaços em branco extras no início e no final da linha
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (!line.empty()) { // Evita processar linhas vazias
            processCommand(line); // Processa o comando
        }
    }

    file.close();
}

void Simulator::spawnBarbarian(int l, int c) {
    std::string internalId = map.generateUUID();
    map.addCaravan(std::make_unique<BarbarianCaravan>('!', l, c, &map, internalId));
    std::cout << "Caravana barbara criada em (" << l << ", " << c << ")." << std::endl;
}