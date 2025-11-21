#include "Map.h"
#include <fstream>
#include <algorithm>
#include <random>
#include <iostream>
#include "Buffer.h"
#include "Simulator.h"
#include "Caravan.h"
#include "City.h"
#include <sstream>
#include <string>
#include <iomanip>

Map::Map(int rows, int columns) : rows(rows), columns(columns) {
    if (rows <= 0 || columns <= 0) {
        throw invalid_argument("Invalid map dimensions.");
    }
    grid = make_unique<unique_ptr<char[]>[]>(rows);
    for (int i = 0; i < rows; ++i) {
        grid[i] = make_unique<char[]>(columns);
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            grid[i][j] = ' ';
        }
    }
}

/*void Map::loadMap(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        throw runtime_error("Failed to open the map file.");
    }

    // Read map dimensions
    string line;
    int rows = 0, columns = 0;

    // Read "linhas" value
    getline(inputFile, line);
    rows = stoi(line.substr(line.find_last_of(' ') + 1));

    // Read "colunas" value
    getline(inputFile, line);
    columns = stoi(line.substr(line.find_last_of(' ') + 1));

    // Validate dimensions
    if (rows != this->rows || columns != this->columns) {
        throw runtime_error("Map dimensions in file do not match constructor dimensions.");
    }

    string validCharacters = ".+abcdefghijklmnopqrstuvwxyz0123456789!";

    // Read the map grid
    for (int i = 0; i < rows; ++i) {
        getline(inputFile, line);
        if (line.length() < columns) {
            throw runtime_error("Line " + to_string(i) + " is too short.");
        }

        for (int j = 0; j < columns; ++j) {
            char c = line[j];
            if (validCharacters.find(c) == string::npos) {
                throw runtime_error("Invalid character '" + string(1, c) +
                                  "' at position (" + to_string(i) + ", " + to_string(j) + ").");
            }

            grid[i][j] = c;

            // Process elements
            if (islower(c)) { // City
                // Create city with default prices that will be updated later
                cities.emplace_back(c, i, j, c);
            } else if (isdigit(c)) { // User caravan
                caravans.emplace_back(make_unique<ComercialCaravan>(c, i, j, this));
                caravans.back()->setMap(this);
            } else if (c == '!') { // Barbarian caravan
                caravans.emplace_back(make_unique<BarbarianCaravan>(c, i, j, this));
                caravans.back()->setMap(this);
            }
        }
    }

    // Read configuration values
    while (getline(inputFile, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        string key;
        int value;

        iss >> key;
        iss >> value;

        if (key == "moedas") {
            playerCoins = value;
        } else if (key == "instantes_entre_novos_itens") {
            itemSpawnInterval = value;
        } else if (key == "duração_item") {
            itemDuration = value;
        } else if (key == "max_itens") {
            maxItems = value;
        } else if (key == "preço_venda_mercadoria") {
            // Update sell price for all cities
            for (auto& city : cities) {
                city.setSellPrice(value);
            }
        } else if (key == "preço_compra_mercadoria") {
            // Update buy price for all cities
            for (auto& city : cities) {
                city.setBuyPrice(value);
            }
        } else if (key == "preço_caravana") {
            // Update caravan price for all cities
            for (auto& city : cities) {
                city.setCaravanPrice(value);
            }
        }
        else if (key == "instantes_entre_novos_barbaros") {
            turnsNewBarbarian = value;
        } else if (key == "duração_barbaros") {
            durationBarbarian = value;
        }
    }

    inputFile.close();
}*/

/*void Map::loadMap(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        throw runtime_error("Failed to open the map file.");
    }

    // Read map dimensions
    string line;
    int rows = 0, columns = 0;

    // Read "linhas" value
    getline(inputFile, line);
    rows = stoi(line.substr(line.find_last_of(' ') + 1));

    // Read "colunas" value
    getline(inputFile, line);
    columns = stoi(line.substr(line.find_last_of(' ') + 1));

    // Validate dimensions
    if (rows != this->rows || columns != this->columns) {
        throw runtime_error("Map dimensions in file do not match constructor dimensions.");
    }

    string validCharacters = ".+abcdefghijklmnopqrstuvwxyz0123456789!";

    // Read the map grid
    for (int i = 0; i < rows; ++i) {
        getline(inputFile, line);
        if (line.length() < columns) {
            throw runtime_error("Line " + to_string(i) + " is too short.");
        }

        for (int j = 0; j < columns; ++j) {
            char c = line[j];
            if (validCharacters.find(c) == string::npos) {
                throw runtime_error("Invalid character '" + string(1, c) +
                                  "' at position (" + to_string(i) + ", " + to_string(j) + ").");
            }

            grid[i][j] = c;

            // Process elements
            if (islower(c)) { // City
                // Create city with default prices that will be updated later
                cities.emplace_back(c, i, j, c);
            }
            // Removido a lógica de inicialização das caravanas do utilizador do loadMap
            else if (c == '!') { // Barbarian caravan
                // Generate a unique internal ID for Barbarian caravan
                std::string stringNumber = std::to_string(nextBarbarianId);
                 char internalId = stringNumber[0];
                caravans.emplace_back(make_unique<BarbarianCaravan>('!', i, j, this, internalId));
                 caravans.back()->setMap(this);
                 nextBarbarianId++;
            }
        }
    }

    //Incrementa o counter das caravanas do utilizador para não haver sobreposiçao com o id da caravana barbara
    counterCaravans++;

    // Read configuration values
    while (getline(inputFile, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        string key;
        int value;

        iss >> key;
        iss >> value;

        if (key == "moedas") {
            playerCoins = value;
        } else if (key == "instantes_entre_novos_itens") {
            itemSpawnInterval = value;
        } else if (key == "duração_item") {
            itemDuration = value;
        } else if (key == "max_itens") {
            maxItems = value;
        } else if (key == "preço_venda_mercadoria") {
            // Update sell price for all cities
            for (auto& city : cities) {
                city.setSellPrice(value);
            }
        } else if (key == "preço_compra_mercadoria") {
            // Update buy price for all cities
            for (auto& city : cities) {
                city.setBuyPrice(value);
            }
        } else if (key == "preço_caravana") {
            // Update caravan price for all cities
            for (auto& city : cities) {
                city.setCaravanPrice(value);
            }
        }
        else if (key == "instantes_entre_novos_barbaros") {
            turnsNewBarbarian = value;
        } else if (key == "duração_barbaros") {
            durationBarbarian = value;
        }
    }

    inputFile.close();
}*/

void Map::loadMap(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        throw runtime_error("Failed to open the map file.");
    }
    string line;
    int rows = 0, columns = 0;
    getline(inputFile, line);
    rows = stoi(line.substr(line.find_last_of(' ') + 1));
    getline(inputFile, line);
    columns = stoi(line.substr(line.find_last_of(' ') + 1));
    if (rows != this->rows || columns != this->columns) {
        throw runtime_error("Map dimensions in file do not match constructor dimensions.");
    }

    string validCharacters = ".+abcdefghijklmnopqrstuvwxyz0123456789!";

    for (int i = 0; i < rows; ++i) {
        getline(inputFile, line);
        if (line.length() < columns) {
            throw runtime_error("Line " + to_string(i) + " is too short.");
        }

        for (int j = 0; j < columns; ++j) {
            char c = line[j];
            if (validCharacters.find(c) == string::npos) {
                throw runtime_error("Invalid character '" + string(1, c) +
                                  "' at position (" + to_string(i) + ", " + to_string(j) + ").");
            }

            grid[i][j] = c;

            if (islower(c)) {
                cities.emplace_back(c, i, j, c);
            }
            else if (c == '!') {
                  std::string internalId = generateUUID();
                caravans.emplace_back(make_unique<BarbarianCaravan>('!', i, j, this, internalId));
                 caravans.back()->setMap(this);

            }
        }
    }
    counterCaravans++;
    while (getline(inputFile, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        string key;
        int value;

        iss >> key;
        iss >> value;

        if (key == "moedas") {
            playerCoins = value;
        } else if (key == "instantes_entre_novos_itens") {
            itemSpawnInterval = value;
        } else if (key == "duração_item") {
            itemDuration = value;
        } else if (key == "max_itens") {
            maxItems = value;
        } else if (key == "preço_venda_mercadoria") {
            for (auto& city : cities) {
                city.setSellPrice(value);
            }
        } else if (key == "preço_compra_mercadoria") {
            for (auto& city : cities) {
                city.setBuyPrice(value);
            }
        } else if (key == "preço_caravana") {
            for (auto& city : cities) {
                city.setCaravanPrice(value);
            }
        }
        else if (key == "instantes_entre_novos_barbaros") {
            turnsNewBarbarian = value;
        } else if (key == "duração_barbaros") {
            durationBarbarian = value;
        }
    }

    inputFile.close();
}

//Método para gerar ids unicos para as caravanas barbaras
std::string Map::generateUUID() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    }
    return ss.str();
}

int Map::getRows() const {
    return rows;
}

int Map::getColumns() const {
    return columns;
}

vector<City> Map::getCities() const {
    return cities;
}

const vector<unique_ptr<Caravan>>& Map::getCaravans() const {
    return caravans;
}

const vector<unique_ptr<Item>>& Map::getItems() const {
    return items;
}

void Map::listAllItens() const
{
    for(const auto& item : items)
    {
        cout << item->getName() << endl;
    }
}

/*void Map::moveCaravan(char caravanId, int newX, int newY) {
    // Check bounds
    if (newX < 0 || newX >= rows || newY < 0 || newY >= columns) {
        throw out_of_range("Move out of bounds");
    }

    // Check if the destination is valid (not a mountain)
    if (grid[newX][newY] == '+') {
        throw runtime_error("Invalid move: destination is a mountain.");
    }

    // Check if the destination is occupied by other caravans
    for(const auto& caravan : caravans)
    {
        if(caravan->getX() == newX && caravan->getY() == newY)
        {
            throw runtime_error("Invalid move: destination is occupied by another caravan.");
        }
    }

    // Find the caravan by ID
    for (auto& caravan : caravans) {
        if (caravan->getId() == caravanId) {
            // Update grid (clear old position)
            grid[caravan->getX()][caravan->getY()] = '.';

            // Move the caravan
            //caravan->move(newX, newY); //TODO: Perguntar ao Afonso para quê que é preciso o move

            // Update grid with new position
            grid[newX][newY] = caravanId;

            // Check for interactions
            for (const auto& city : cities) {
                if (city.getX() == newX && city.getY() == newY) {
                    cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                }
            }
            return;
        }
    }

    throw runtime_error("Caravan not found.");
}*/

/*void Map::moveCaravan(char caravanId, int newX, int newY) {
        // Check bounds
        if (newX < 0 || newX >= rows || newY < 0 || newY >= columns) {
            throw out_of_range("Move out of bounds");
        }

        // Check if the destination is valid (not a mountain)
        if (grid[newX][newY] == '+') {
            throw runtime_error("Invalid move: destination is a mountain.");
        }
        // Check if the destination is occupied by other caravans
        for(const auto& caravan : caravans)
        {
            if(caravan->getX() == newX && caravan->getY() == newY)
            {
                throw runtime_error("Invalid move: destination is occupied by another caravan.");
            }
        }

         // Find the caravan by ID
        for (auto& caravan : caravans) {
            if (caravan->getId() == caravanId) {
                // Store the old position
                int oldX = caravan->getX();
                int oldY = caravan->getY();

                // Move the caravan
                caravan->move(newX, newY);

                 // Update grid with new position
                grid[newX][newY] = caravanId;

               // Check if the old position was a city
                bool oldPositionIsCity = false;
                for (const auto& city : cities) {
                    if (city.getX() == oldX && city.getY() == oldY) {
                        // Restore city character at the old position
                        grid[oldX][oldY] = city.getGridValue();
                        oldPositionIsCity = true;
                         cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                        break;
                    }
                }
                 // If not city, set to empty
                 if(!oldPositionIsCity){
                  grid[oldX][oldY] = '.';
                 }

                for (auto& city : cities) {
                    if (city.getX() == newX && city.getY() == newY) {
                        cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                        grid[newX][newY] = city.getGridValue();
                        city.addCaravan(caravan.get());
                        caravan->refillWater();
                        break;
                    }
                }

                  return;
            }
        }
     throw runtime_error("Caravan not found.");
    }

void Map::moveCaravan(char caravanId, std::string direction) {
    int newX, newY;

     // Find the caravan by ID
        Caravan* caravan = nullptr;
        for (auto& c : caravans) {
            if (c->getId() == caravanId) {
                caravan = c.get();
                break;
            }
        }
        if (!caravan) {
            throw runtime_error("Caravan not found.");
        }


    newX = caravan->getX();
    newY = caravan->getY();

     if (direction == "D") {
        newY++;
    } else if (direction == "E") {
        newY--;
    } else if (direction == "C") {
        newX--;
    } else if (direction == "B") {
        newX++;
    } else if (direction == "CE") {
        newX--;
        newY--;
    } else if (direction == "CD") {
        newX--;
        newY++;
    } else if (direction == "BE") {
        newX++;
        newY--;
    } else if (direction == "BD") {
        newX++;
        newY++;
    } else {
        throw runtime_error("Invalid movement direction.");
    }


     // Check bounds
    if (newX < 0 || newX >= rows || newY < 0 || newY >= columns) {
        throw out_of_range("Move out of bounds");
    }

    // Check if the destination is valid (not a mountain)
    if (grid[newX][newY] == '+') {
        throw runtime_error("Invalid move: destination is a mountain.");
    }

    // Check if the destination is occupied by other caravans
     for(const auto& caravanCheck : caravans)
        {
            if(caravanCheck->getX() == newX && caravanCheck->getY() == newY)
            {
                throw runtime_error("Invalid move: destination is occupied by another caravan.");
            }
        }

    // Move the caravan with the Map method
     for (auto& caravanToMove : caravans) {
            if (caravanToMove->getId() == caravanId) {
                 // Store the old position
                int oldX = caravanToMove->getX();
                int oldY = caravanToMove->getY();

                // Move the caravan
                caravanToMove->move(newX, newY);

                 // Update grid with new position
                grid[newX][newY] = caravanId;

               // Check if the old position was a city
                bool oldPositionIsCity = false;
                for (const auto& city : cities) {
                    if (city.getX() == oldX && city.getY() == oldY) {
                        // Restore city character at the old position
                        grid[oldX][oldY] = city.getGridValue();
                        oldPositionIsCity = true;
                         cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                        break;
                    }
                }
                 // If not city, set to empty
                 if(!oldPositionIsCity){
                  grid[oldX][oldY] = '.';
                 }

                for (auto& city : cities) {
                    if (city.getX() == newX && city.getY() == newY) {
                        cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                        grid[newX][newY] = city.getGridValue();
                        city.addCaravan(caravanToMove.get());
                        caravanToMove->refillWater();
                        break;
                    }
                }

                  return;
            }
        }

    throw runtime_error("Caravan not found.");
}*/

/*void Map::moveCaravan(char caravanId, int newX, int newY) {
        // Check bounds
        if (newX < 0 || newX >= rows || newY < 0 || newY >= columns) {
            throw out_of_range("Move out of bounds");
        }

        // Check if the destination is valid (not a mountain)
        if (grid[newX][newY] == '+') {
            throw runtime_error("Invalid move: destination is a mountain.");
        }
        // Check if the destination is occupied by other caravans
        for(const auto& caravan : caravans)
        {
            if(caravan->getX() == newX && caravan->getY() == newY)
            {
                throw runtime_error("Invalid move: destination is occupied by another caravan.");
            }
        }

         // Find the caravan by ID
        for (auto& caravan : caravans) {
            if (caravan->getId() == caravanId) {
                // Store the old position
                int oldX = caravan->getX();
                int oldY = caravan->getY();

                // Move the caravan
                caravan->move(newX, newY);

                 // Update grid with new position
                 // Mantemos '!' para todas as bárbaras no grid
                if (caravan->getType() == "Barbarian")
                {
                    grid[newX][newY] = '!';
                } else {
                    grid[newX][newY] = caravanId;
                }

               // Check if the old position was a city
                bool oldPositionIsCity = false;
                for (const auto& city : cities) {
                    if (city.getX() == oldX && city.getY() == oldY) {
                        // Restore city character at the old position
                        grid[oldX][oldY] = city.getGridValue();
                        oldPositionIsCity = true;
                         cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                        break;
                    }
                }
                 // If not city, set to empty
                 if(!oldPositionIsCity){
                  grid[oldX][oldY] = '.';
                 }

                for (auto& city : cities) {
                    if (city.getX() == newX && city.getY() == newY) {
                        cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                        grid[newX][newY] = city.getGridValue();
                        city.addCaravan(caravan.get());
                        caravan->refillWater();
                        break;
                    }
                }

                  return;
            }
        }
     throw runtime_error("Caravan not found.");
    }


void Map::moveCaravan(char caravanId, std::string direction) {
    int newX, newY;

     // Find the caravan by ID
        Caravan* caravan = nullptr;
        for (auto& c : caravans) {
            if (c->getId() == caravanId) {
                caravan = c.get();
                break;
            }
        }
        if (!caravan) {
            throw runtime_error("Caravan not found.");
        }


    newX = caravan->getX();
    newY = caravan->getY();

     if (direction == "D") {
        newY++;
    } else if (direction == "E") {
        newY--;
    } else if (direction == "C") {
        newX--;
    } else if (direction == "B") {
        newX++;
    } else if (direction == "CE") {
        newX--;
        newY--;
    } else if (direction == "CD") {
        newX--;
        newY++;
    } else if (direction == "BE") {
        newX++;
        newY--;
    } else if (direction == "BD") {
        newX++;
        newY++;
    } else {
        throw runtime_error("Invalid movement direction.");
    }


     // Check bounds
    if (newX < 0 || newX >= rows || newY < 0 || newY >= columns) {
        throw out_of_range("Move out of bounds");
    }

    // Check if the destination is valid (not a mountain)
    if (grid[newX][newY] == '+') {
        throw runtime_error("Invalid move: destination is a mountain.");
    }

    // Check if the destination is occupied by other caravans
     for(const auto& caravanCheck : caravans)
        {
            if(caravanCheck->getX() == newX && caravanCheck->getY() == newY)
            {
                throw runtime_error("Invalid move: destination is occupied by another caravan.");
            }
        }

    // Move the caravan with the Map method
     for (auto& caravanToMove : caravans) {
            if (caravanToMove->getId() == caravanId) {
                 // Store the old position
                int oldX = caravanToMove->getX();
                int oldY = caravanToMove->getY();

                // Move the caravan
                caravanToMove->move(newX, newY);

                // Update grid with new position
                 // Mantemos '!' para todas as bárbaras no grid
                if (caravanToMove->getType() == "Barbarian")
                {
                    grid[newX][newY] = '!';
                } else {
                    grid[newX][newY] = caravanId;
                }

               // Check if the old position was a city
                bool oldPositionIsCity = false;
                for (const auto& city : cities) {
                    if (city.getX() == oldX && city.getY() == oldY) {
                        // Restore city character at the old position
                        grid[oldX][oldY] = city.getGridValue();
                        oldPositionIsCity = true;
                         cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                        break;
                    }
                }
                 // If not city, set to empty
                 if(!oldPositionIsCity){
                  grid[oldX][oldY] = '.';
                 }

                for (auto& city : cities) {
                    if (city.getX() == newX && city.getY() == newY) {
                        cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                        grid[newX][newY] = city.getGridValue();
                        city.addCaravan(caravanToMove.get());
                        caravanToMove->refillWater();
                        break;
                    }
                }

                  return;
            }
        }

    throw runtime_error("Caravan not found.");
}*/

/*void Map::moveCaravan(char caravanId, int newX, int newY) {
    // Check bounds
    if (newX < 0 || newX >= rows || newY < 0 || newY >= columns) {
        throw out_of_range("Move out of bounds");
    }

    // Check if the destination is valid (not a mountain)
    if (grid[newX][newY] == '+') {
        throw runtime_error("Invalid move: destination is a mountain.");
    }
    // Check if the destination is occupied by other caravans
    for(const auto& caravan : caravans)
    {
        if(caravan->getX() == newX && caravan->getY() == newY)
        {
            throw runtime_error("Invalid move: destination is occupied by another caravan.");
        }
    }

     // Find the caravan by ID
        for (auto& caravan : caravans) {
              // Verificação se a caravana é bárbara
                if (caravan->getType() == "Barbarian") {
                    BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(caravan.get());
                       // Se for barbara, usa o InternalID
                     if (barbarianCaravan && barbarianCaravan->getInternalId() == caravanId)
                    {
                            // Store the old position
                            int oldX = barbarianCaravan->getX();
                            int oldY = barbarianCaravan->getY();

                            // Move the caravan
                            barbarianCaravan->move(newX, newY);

                             // Update grid with new position
                             // Mantemos '!' para todas as bárbaras no grid
                            grid[newX][newY] = '!';

                           // Check if the old position was a city
                            bool oldPositionIsCity = false;
                            for (const auto& city : cities) {
                                if (city.getX() == oldX && city.getY() == oldY) {
                                    // Restore city character at the old position
                                    grid[oldX][oldY] = city.getGridValue();
                                    oldPositionIsCity = true;
                                     cout << "Caravan " << barbarianCaravan->getId() << " entered city " << city.getName() << endl;
                                    break;
                                }
                            }
                             // If not city, set to empty
                             if(!oldPositionIsCity){
                              grid[oldX][oldY] = '.';
                             }
                            for (auto& city : cities) {
                                if (city.getX() == newX && city.getY() == newY) {
                                    cout << "Caravan " << barbarianCaravan->getId() << " entered city " << city.getName() << endl;
                                    grid[newX][newY] = city.getGridValue();
                                    city.addCaravan(barbarianCaravan);
                                    barbarianCaravan->refillWater();
                                    break;
                                }
                            }

                              return;
                     }
                }
                // Se nao for bárbara, procura pelo ID normal
                else if (caravan->getId() == caravanId) {
                    // Store the old position
                    int oldX = caravan->getX();
                    int oldY = caravan->getY();

                    // Move the caravan
                    caravan->move(newX, newY);

                     // Update grid with new position
                    grid[newX][newY] = caravanId;
                   // Check if the old position was a city
                    bool oldPositionIsCity = false;
                    for (const auto& city : cities) {
                        if (city.getX() == oldX && city.getY() == oldY) {
                            // Restore city character at the old position
                            grid[oldX][oldY] = city.getGridValue();
                            oldPositionIsCity = true;
                             cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                            break;
                        }
                    }
                     // If not city, set to empty
                     if(!oldPositionIsCity){
                      grid[oldX][oldY] = '.';
                     }

                    for (auto& city : cities) {
                        if (city.getX() == newX && city.getY() == newY) {
                            cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                            grid[newX][newY] = city.getGridValue();
                             city.addCaravan(caravan.get());
                            caravan->refillWater();
                            break;
                        }
                    }

                      return;
                }
         }
       throw runtime_error("Caravan not found.");
    }


void Map::moveCaravan(char caravanId, std::string direction) {
    int newX, newY;

     // Find the caravan by ID
        Caravan* caravan = nullptr;
        for (auto& c : caravans) {
            if (c->getId() == caravanId) {
                caravan = c.get();
                break;
            }
        }
        if (!caravan) {
            throw runtime_error("Caravan not found.");
        }


    newX = caravan->getX();
    newY = caravan->getY();

     if (direction == "D") {
        newY++;
    } else if (direction == "E") {
        newY--;
    } else if (direction == "C") {
        newX--;
    } else if (direction == "B") {
        newX++;
    } else if (direction == "CE") {
        newX--;
        newY--;
    } else if (direction == "CD") {
        newX--;
        newY++;
    } else if (direction == "BE") {
        newX++;
        newY--;
    } else if (direction == "BD") {
        newX++;
        newY++;
    } else {
        throw runtime_error("Invalid movement direction.");
    }


     // Check bounds
    if (newX < 0 || newX >= rows || newY < 0 || newY >= columns) {
        throw out_of_range("Move out of bounds");
    }

    // Check if the destination is valid (not a mountain)
    if (grid[newX][newY] == '+') {
        throw runtime_error("Invalid move: destination is a mountain.");
    }

    // Check if the destination is occupied by other caravans
     for(const auto& caravanCheck : caravans)
        {
            if(caravanCheck->getX() == newX && caravanCheck->getY() == newY)
            {
                throw runtime_error("Invalid move: destination is occupied by another caravan.");
            }
        }

    // Move the caravan with the Map method
    for (auto& caravan : caravans) {
              // Verificação se a caravana é bárbara
                if (caravan->getType() == "Barbarian") {
                   BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(caravan.get());
                    // Se for barbara, usa o InternalID
                     if (barbarianCaravan && barbarianCaravan->getInternalId() == caravanId)
                    {
                            // Store the old position
                            int oldX = barbarianCaravan->getX();
                            int oldY = barbarianCaravan->getY();

                            // Move the caravan
                            barbarianCaravan->move(newX, newY);

                            // Update grid with new position
                             // Mantemos '!' para todas as bárbaras no grid
                             grid[newX][newY] = '!';


                            // Check if the old position was a city
                            bool oldPositionIsCity = false;
                            for (const auto& city : cities) {
                                if (city.getX() == oldX && city.getY() == oldY) {
                                    // Restore city character at the old position
                                    grid[oldX][oldY] = city.getGridValue();
                                    oldPositionIsCity = true;
                                     cout << "Caravan " << barbarianCaravan->getId() << " entered city " << city.getName() << endl;
                                    break;
                                }
                            }
                            // If not city, set to empty
                             if(!oldPositionIsCity){
                              grid[oldX][oldY] = '.';
                             }
                                  for (auto& city : cities) {
                                    if (city.getX() == newX && city.getY() == newY) {
                                        cout << "Caravan " << barbarianCaravan->getId() << " entered city " << city.getName() << endl;
                                        grid[newX][newY] = city.getGridValue();
                                         city.addCaravan(barbarianCaravan);
                                        barbarianCaravan->refillWater();
                                        break;
                                    }
                                }
                              return;
                     }

                }
                // Se nao for bárbara, procura pelo ID normal
                else if (caravan->getId() == caravanId)
                 {
                     // Store the old position
                     int oldX = caravan->getX();
                     int oldY = caravan->getY();
                     // Move the caravan
                    caravan->move(newX, newY);
                    // Update grid with new position
                    grid[newX][newY] = caravanId;

                   // Check if the old position was a city
                   bool oldPositionIsCity = false;
                    for (const auto& city : cities) {
                         if (city.getX() == oldX && city.getY() == oldY) {
                            // Restore city character at the old position
                            grid[oldX][oldY] = city.getGridValue();
                           oldPositionIsCity = true;
                            cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                           break;
                        }
                    }
                  // If not city, set to empty
                  if(!oldPositionIsCity){
                       grid[oldX][oldY] = '.';
                  }

                    for (auto& city : cities) {
                        if (city.getX() == newX && city.getY() == newY) {
                            cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                            grid[newX][newY] = city.getGridValue();
                            city.addCaravan(caravan.get());
                            caravan->refillWater();
                            break;
                        }
                   }
                    return;
                }
        }
      throw runtime_error("Caravan not found.");
    }*/

/*void Map::moveCaravan(char caravanId, int newX, int newY) {
    // Check bounds
    if (newX < 0 || newX >= rows || newY < 0 || newY >= columns) {
        throw out_of_range("Move out of bounds");
    }

    // Check if the destination is valid (not a mountain)
    if (grid[newX][newY] == '+') {
        throw runtime_error("Invalid move: destination is a mountain.");
    }
    // Check if the destination is occupied by other caravans
    for(const auto& caravan : caravans)
    {
        if(caravan->getX() == newX && caravan->getY() == newY)
        {
            throw runtime_error("Invalid move: destination is occupied by another caravan.");
        }
    }

     // Find the caravan by ID
        for (auto& caravan : caravans) {
              // Verificação se a caravana é bárbara
                if (caravan->getType() == "Barbarian") {
                    BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(caravan.get());
                       // Se for barbara, usa o InternalID
                     if (barbarianCaravan && barbarianCaravan->getInternalId()[0] == caravanId)
                    {
                            // Store the old position
                            int oldX = barbarianCaravan->getX();
                            int oldY = barbarianCaravan->getY();

                            // Move the caravan
                            barbarianCaravan->move(newX, newY);

                             // Update grid with new position
                             // Mantemos '!' para todas as bárbaras no grid
                            grid[newX][newY] = '!';

                           // Check if the old position was a city
                            bool oldPositionIsCity = false;
                            for (const auto& city : cities) {
                                if (city.getX() == oldX && city.getY() == oldY) {
                                    // Restore city character at the old position
                                    grid[oldX][oldY] = city.getGridValue();
                                    oldPositionIsCity = true;
                                     cout << "Caravan " << barbarianCaravan->getId() << " entered city " << city.getName() << endl;
                                    break;
                                }
                            }
                             // If not city, set to empty
                             if(!oldPositionIsCity){
                              grid[oldX][oldY] = '.';
                             }
                            for (auto& city : cities) {
                                if (city.getX() == newX && city.getY() == newY) {
                                    cout << "Caravan " << barbarianCaravan->getId() << " entered city " << city.getName() << endl;
                                    grid[newX][newY] = city.getGridValue();
                                    city.addCaravan(barbarianCaravan);
                                    barbarianCaravan->refillWater();
                                    break;
                                }
                            }

                              return;
                     }
                }
                // Se nao for bárbara, procura pelo ID normal
                else if (caravan->getId() == caravanId) {
                    // Store the old position
                    int oldX = caravan->getX();
                    int oldY = caravan->getY();

                    // Move the caravan
                    caravan->move(newX, newY);

                     // Update grid with new position
                    grid[newX][newY] = caravanId;
                   // Check if the old position was a city
                    bool oldPositionIsCity = false;
                    for (const auto& city : cities) {
                        if (city.getX() == oldX && city.getY() == oldY) {
                            // Restore city character at the old position
                            grid[oldX][oldY] = city.getGridValue();
                            oldPositionIsCity = true;
                             cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                            break;
                        }
                    }
                     // If not city, set to empty
                     if(!oldPositionIsCity){
                      grid[oldX][oldY] = '.';
                     }

                    for (auto& city : cities) {
                        if (city.getX() == newX && city.getY() == newY) {
                            cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                            grid[newX][newY] = city.getGridValue();
                             city.addCaravan(caravan.get());
                            caravan->refillWater();
                            break;
                        }
                    }

                      return;
                }
         }
       throw runtime_error("Caravan not found.");
    }*/

void Map::moveCaravan(char caravanId, int newX, int newY) {
    if (newX < 0 || newX >= rows || newY < 0 || newY >= columns) {
        throw out_of_range("Move out of bounds");
    }

    if (grid[newX][newY] == '+') {
        throw runtime_error("Invalid move: destination is a mountain.");
    }

    for(const auto& caravan : caravans)
    {
        if(caravan->getX() == newX && caravan->getY() == newY)
        {
            throw runtime_error("Invalid move: destination is occupied by another caravan.");
        }
    }

        for (auto& caravan : caravans) {
                if (caravan->getType() == "Barbarian") {
                    BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(caravan.get());

                     if (barbarianCaravan && barbarianCaravan->getInternalId()[0] == caravanId)
                    {

                            int oldX = barbarianCaravan->getX();
                            int oldY = barbarianCaravan->getY();
                            barbarianCaravan->move(newX, newY);
                            grid[newX][newY] = '!';

                            bool oldPositionIsCity = false;
                            for (const auto& city : cities) {
                                if (city.getX() == oldX && city.getY() == oldY) {

                                    grid[oldX][oldY] = city.getGridValue();
                                    oldPositionIsCity = true;
                                     cout << "Caravan " << barbarianCaravan->getId() << " entered city " << city.getName() << endl;
                                    break;
                                }
                            }

                             if(!oldPositionIsCity){
                              grid[oldX][oldY] = '.';
                             }
                            for (auto& city : cities) {
                                if (city.getX() == newX && city.getY() == newY) {
                                    cout << "Caravan " << barbarianCaravan->getId() << " entered city " << city.getName() << endl;
                                    grid[newX][newY] = city.getGridValue();
                                    city.addCaravan(barbarianCaravan);
                                    barbarianCaravan->refillWater();
                                    break;
                                }
                            }

                              return;
                     }
                }

                else if (caravan->getId() == caravanId) {

                    int oldX = caravan->getX();
                    int oldY = caravan->getY();


                    caravan->move(newX, newY);


                    grid[newX][newY] = caravanId;
                    bool oldPositionIsCity = false;
                    for (auto& city : cities) {
                        if (city.getX() == oldX && city.getY() == oldY) {

                            grid[oldX][oldY] = city.getGridValue();
                            city.removeCaravan(caravan.get());
                            oldPositionIsCity = true;
                             cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                            break;
                        }
                    }

                     if(!oldPositionIsCity){
                      grid[oldX][oldY] = '.';
                     }

                    for (auto& city : cities) {
                        if (city.getX() == newX && city.getY() == newY) {
                            cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                            grid[newX][newY] = city.getGridValue();
                             city.addCaravan(caravan.get());
                            caravan->refillWater();
                            break;
                        }
                    }

                      return;
                }
         }
       throw runtime_error("Caravan not found.");
    }

void Map::moveCaravan(char caravanId, std::string direction) {
    int newX, newY;


        Caravan* caravan = nullptr;
        for (auto& c : caravans) {
            if (c->getId() == caravanId) {
                caravan = c.get();
                break;
            }
        }
        if (!caravan) {
            throw runtime_error("Caravan not found.");
        }


    newX = caravan->getX();
    newY = caravan->getY();

     if (direction == "D") {
        newY++;
    } else if (direction == "E") {
        newY--;
    } else if (direction == "C") {
        newX--;
    } else if (direction == "B") {
        newX++;
    } else if (direction == "CE") {
        newX--;
        newY--;
    } else if (direction == "CD") {
        newX--;
        newY++;
    } else if (direction == "BE") {
        newX++;
        newY--;
    } else if (direction == "BD") {
        newX++;
        newY++;
    } else {
        throw runtime_error("Invalid movement direction.");
    }

    if (newX < 0 || newX >= rows || newY < 0 || newY >= columns) {
        throw out_of_range("Move out of bounds");
    }

    if (grid[newX][newY] == '+') {
        throw runtime_error("Invalid move: destination is a mountain.");
    }

     for(const auto& caravanCheck : caravans)
        {
            if(caravanCheck->getX() == newX && caravanCheck->getY() == newY)
            {
                throw runtime_error("Invalid move: destination is occupied by another caravan.");
            }
        }


    for (auto& caravan : caravans) {

                if (caravan->getType() == "Barbarian") {
                   BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(caravan.get());

                     if (barbarianCaravan && barbarianCaravan->getInternalId()[0] == caravanId)
                    {

                            int oldX = barbarianCaravan->getX();
                            int oldY = barbarianCaravan->getY();
                            barbarianCaravan->move(newX, newY);
                             grid[newX][newY] = '!';

                            bool oldPositionIsCity = false;
                            for (const auto& city : cities) {
                                if (city.getX() == oldX && city.getY() == oldY) {

                                    grid[oldX][oldY] = city.getGridValue();
                                    oldPositionIsCity = true;
                                     cout << "Caravan " << barbarianCaravan->getId() << " entered city " << city.getName() << endl;
                                    break;
                                }
                            }

                             if(!oldPositionIsCity){
                              grid[oldX][oldY] = '.';
                             }
                                  for (auto& city : cities) {
                                    if (city.getX() == newX && city.getY() == newY) {
                                        cout << "Caravan " << barbarianCaravan->getId() << " entered city " << city.getName() << endl;
                                        grid[newX][newY] = city.getGridValue();
                                         city.addCaravan(barbarianCaravan);
                                        barbarianCaravan->refillWater();
                                        break;
                                    }
                                }
                              return;
                     }

                }

                else if (caravan->getId() == caravanId)
                 {

                     int oldX = caravan->getX();
                     int oldY = caravan->getY();

                    caravan->move(newX, newY);

                    grid[newX][newY] = caravanId;


                   bool oldPositionIsCity = false;
                    for (auto& city : cities) {
                         if (city.getX() == oldX && city.getY() == oldY) {

                            grid[oldX][oldY] = city.getGridValue();
                            city.removeCaravan(caravan.get());
                            oldPositionIsCity = true;
                            cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                           break;
                        }
                    }

                  if(!oldPositionIsCity){
                       grid[oldX][oldY] = '.';
                  }

                    for (auto& city : cities) {
                        if (city.getX() == newX && city.getY() == newY) {
                            cout << "Caravan " << caravanId << " entered city " << city.getName() << endl;
                            grid[newX][newY] = city.getGridValue();
                            city.addCaravan(caravan.get());
                            caravan->refillWater();
                            break;
                        }
                   }
                    return;
                }
        }
      throw runtime_error("Caravan not found.");
    }

void Map::buyCaravan(char cityId, char type) {

    City* city = nullptr;
    for (auto& c : cities) {
        if (c.getName() == cityId) {
            city = &c;
            break;
        }
    }

    if (!city) {
        throw std::runtime_error("Cidade não encontrada.");
    }
    city->sellCaravan(type, *this);
}

/*void Map::resolveCombat(char userCaravanId, char barbarianId) {
    // Check if caravans are adjacent
    if (!areCaravansAdjacent(userCaravanId, barbarianId)) {
        cout << "No combat: Caravans are not adjacent." << endl;
        return;
    }

    // Find user caravan
    Caravan* userCaravan = nullptr;
    for (auto& c : caravans) {
        if (c->getId() == userCaravanId) {
            userCaravan = c.get();
            break;
        }
    }
    if (!userCaravan) {
        throw runtime_error("User caravan not found.");
    }

    // Find barbarian caravan
    cout << "barbarianId: " << barbarianId << endl;
    Caravan* barbarianCaravan = nullptr;
    for (auto& c : caravans) {
        if (c->getId() == barbarianId) {
            barbarianCaravan = c.get();
            break;
        }
    }
    if (!barbarianCaravan) {
        throw runtime_error("Barbarian caravan not found.");
    }

    cout << "userCaravanId: " << userCaravanId << endl;
    // Perform combat logic (unchanged)
    srand(time(nullptr));
    int userScore = rand() % (userCaravan->getTripulation() + 1);
    int barbarianScore = rand() % (barbarianCaravan->getTripulation() + 1);

    cout << "userScore: " << userScore << " | barbarianScore: " << barbarianScore << endl;

    if (userScore >= barbarianScore) {
        // User wins
        int crewLoss = barbarianCaravan->getTripulation() / 2;
        cout << crewLoss << endl;
        userCaravan->reduceTripulation(crewLoss / 2);
        cout << "Caravan removed";
        removeCaravan(barbarianId);
        cout << "User caravan " << userCaravanId << " defeated barbarian " << barbarianId << "." << endl;
        //userCaravan->addCoins(50); // Reward for winning //TODO: Perguntar ao Afonso para quê que é preciso o addCoins
        cout << "User caravan " << userCaravanId << " defeated barbarian " << barbarianId << "." << endl;
    } else {
        // Barbarian wins
        cout << "userCaravanId: " << userCaravanId << endl;
        int crewLoss = userCaravan->getTripulation() / 2;
        cout << crewLoss << endl;
        barbarianCaravan->reduceTripulation(crewLoss / 2);
        cout << "barbarian crewloss" << crewLoss << endl;
        removeCaravan(userCaravanId);
        cout << "Caravan removed 1";
        //cout << "Barbarian " << barbarianId << " defeated user caravan " << userCaravanId << "." << endl;
    }
}*/

/*void Map::resolveCombat(char userCaravanId, char barbarianId) {
    // Check if caravans are adjacent
    if (!areCaravansAdjacent(userCaravanId, barbarianId)) {
        cout << "No combat: Caravans are not adjacent." << endl;
        return;
    }

    // Find user caravan
    Caravan* userCaravan = nullptr;
    for (auto& c : caravans) {
        if (c->getId() == userCaravanId) {
            userCaravan = c.get();
            break;
        }
    }
    if (!userCaravan) {
        throw runtime_error("User caravan not found.");
    }

    // Find barbarian caravan
    cout << "barbarianId: " << barbarianId << endl;
    Caravan* barbarianCaravan = nullptr;
    for (auto& c : caravans) {
        if (c->getId() == barbarianId) {
            barbarianCaravan = c.get();
            break;
        }
    }
    if (!barbarianCaravan) {
         throw runtime_error("Barbarian caravan not found.");
    }

    cout << "userCaravanId: " << userCaravanId << endl;
    // Perform combat logic (unchanged)
    srand(time(nullptr));
    int userScore = rand() % (userCaravan->getTripulation() + 1);
    int barbarianScore = rand() % (barbarianCaravan->getTripulation() + 1);

    cout << "userScore: " << userScore << " | barbarianScore: " << barbarianScore << endl;

    if (userScore >= barbarianScore) {
        // User wins
        int crewLoss = barbarianCaravan->getTripulation() / 2;
        cout << crewLoss << endl;
        userCaravan->reduceTripulation(crewLoss / 2);
        cout << "Caravan removed";
         removeCaravan(barbarianId);
         cout << "User caravan " << userCaravanId << " defeated barbarian " << barbarianId << "." << endl;

    } else {
        // Barbarian wins
        cout << "userCaravanId: " << userCaravanId << endl;
        int crewLoss = userCaravan->getTripulation() / 2;
        cout << crewLoss << endl;
        barbarianCaravan->reduceTripulation(crewLoss / 2);
         removeCaravan(userCaravanId);
        cout << "Caravan removed 1";
        cout << "Barbarian " << barbarianId << " defeated user caravan " << userCaravanId << "." << endl;
    }
}*/

//REVER ESTE MÉTODO PORQUE ESTÁ A DAR ERRO
/*void Map::removeCaravan(char caravanId) {
    for (auto it = caravans.begin(); it != caravans.end(); ) {
        if ((*it)->getType() == "Barbarian") {

            BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());

            if (barbarianCaravan && barbarianCaravan->getInternalId()[0] == caravanId)
            {
                cout << "Removendo caravana " << caravanId << endl;
                int x = (*it)->getX();
                int y = (*it)->getY();
                if (x >= 0 && x < rows && y >= 0 && y < columns) {
                    grid[x][y] = '.';
                }
                it = caravans.erase(it);
                cout << "Caravana removida" << endl;
                return;
            }
        }
        else if ((*it)->getId() == caravanId) {
            cout << "Removendo caravana " << caravanId << endl;
            int x = (*it)->getX();
            int y = (*it)->getY();
            if (x >= 0 && x < rows && y >= 0 && y < columns) {
                grid[x][y] = '.';
            }
            it = caravans.erase(it);
            cout << "Caravana removida" << endl;
            return;
        } else {
            ++it;
        }
    }
    throw runtime_error("Caravan not found for removal.");
}*/

/*void Map::resolveCombat(char userCaravanId, char barbarianId) {
    // Check if caravans are adjacent
    if (!areCaravansAdjacent(userCaravanId, barbarianId)) {
        cout << "No combat: Caravans are not adjacent." << endl;
        return;
    }

    // Find user caravan
    Caravan* userCaravan = nullptr;
    for (auto& c : caravans) {
        if (c->getId() == userCaravanId) {
            userCaravan = c.get();
            break;
        }
    }
    if (!userCaravan) {
        throw runtime_error("User caravan not found.");
    }

    // Find barbarian caravan
    cout << "barbarianId: " << barbarianId << endl;
    Caravan* barbarianCaravan = nullptr;
    for (auto& c : caravans) {
        if (c->getId() == barbarianId) {
            barbarianCaravan = c.get();
            break;
        }
    }
    if (!barbarianCaravan) {
        throw runtime_error("Barbarian caravan not found.");
    }

    cout << "userCaravanId: " << userCaravanId << endl;
    // Perform combat logic
    srand(time(nullptr));
    int userScore = rand() % (userCaravan->getTripulation() + 1);
    int barbarianScore = rand() % (barbarianCaravan->getTripulation() + 1);

    cout << "userScore: " << userScore << " | barbarianScore: " << barbarianScore << endl;

    int winningTripulationLoss = 0;
    int losingTripulationLoss = 0;

    if (userScore >= barbarianScore) {
        // User wins
        double doubleWinningTripulationLoss = userCaravan->getTripulation() * 0.2;
        double doubleLosingTripulationLoss = barbarianCaravan->getTripulation() * 0.4;

        winningTripulationLoss = static_cast<int>(std::ceil(doubleWinningTripulationLoss));
        losingTripulationLoss = static_cast<int>(std::ceil(doubleLosingTripulationLoss));

        userCaravan->reduceTripulation(winningTripulationLoss);
        barbarianCaravan->reduceTripulation(losingTripulationLoss);

        if(barbarianCaravan->getTripulation() <= 0)
        {
            removeCaravan(barbarianId);
            cout << "User caravan " << userCaravanId << " defeated barbarian " << barbarianId << "." << endl;
            cout << userCaravan->getTripulation() << endl;

            userCaravan->refillWater();

        }
         else
         {
             cout << "User caravan " << userCaravanId << " won the battle against barbarian caravan " << barbarianId << "." << endl;
             cout << barbarianCaravan->getTripulation() << endl;
         }


    } else {
        // Barbarian wins
        double doubleWinningTripulationLoss = barbarianCaravan->getTripulation() * 0.2;
        double doubleLosingTripulationLoss = userCaravan->getTripulation() * 0.4;

        winningTripulationLoss = static_cast<int>(std::ceil(doubleWinningTripulationLoss));
        losingTripulationLoss = static_cast<int>(std::ceil(doubleLosingTripulationLoss));

        barbarianCaravan->reduceTripulation(winningTripulationLoss);
        userCaravan->reduceTripulation(losingTripulationLoss);

         if(userCaravan->getTripulation() <= 0)
         {
              removeCaravan(userCaravanId);
                cout << "Barbarian " << barbarianId << " defeated user caravan " << userCaravanId << "." << endl;
                 barbarianCaravan->refillWater();
                cout << barbarianCaravan->getTripulation() << endl;
         }
         else{
               cout << "Barbarian caravan " << barbarianId << " won the battle against user caravan " << userCaravanId << "." << endl;
             cout << userCaravan->getTripulation() << endl;
         }

    }

}*/

/*void Map::resolveCombat(char userCaravanId, char barbarianId) {
    // Check if caravans are adjacent
    if (!areCaravansAdjacent(userCaravanId, barbarianId)) {
        cout << "No combat: Caravans are not adjacent." << endl;
        return;
    }

    // Find user caravan
    Caravan* userCaravan = nullptr;
    for (auto& c : caravans) {
        if (c->getId() == userCaravanId) {
            userCaravan = c.get();
            break;
        }
    }
    if (!userCaravan) {
        throw runtime_error("User caravan not found.");
    }

    // Find barbarian caravan
    cout << "barbarianId: " << barbarianId << endl;
     BarbarianCaravan* barbarianCaravan = nullptr;
    for (auto& c : caravans) {
        //Mudança para encontrar caravana bárbara com ID correto
        if (c->getType() == "Barbarian") {
            BarbarianCaravan* temp = dynamic_cast<BarbarianCaravan*>(c.get());
                if (temp && temp->getInternalId() == barbarianId)
            {
               barbarianCaravan = temp;
                 break;
            }

        }

    }
    if (!barbarianCaravan) {
        throw runtime_error("Barbarian caravan not found.");
    }

    cout << "userCaravanId: " << userCaravanId << endl;
    // Perform combat logic
    srand(time(nullptr));
    int userScore = rand() % (userCaravan->getTripulation() + 1);
    int barbarianScore = rand() % (barbarianCaravan->getTripulation() + 1);

    cout << "userScore: " << userScore << " | barbarianScore: " << barbarianScore << endl;

    int winningTripulationLoss = 0;
    int losingTripulationLoss = 0;

    if (userScore >= barbarianScore) {
        // User wins
        double doubleWinningTripulationLoss = userCaravan->getTripulation() * 0.2;
        double doubleLosingTripulationLoss = barbarianCaravan->getTripulation() * 0.4;

        winningTripulationLoss = static_cast<int>(std::ceil(doubleWinningTripulationLoss));
        losingTripulationLoss = static_cast<int>(std::ceil(doubleLosingTripulationLoss));

        userCaravan->reduceTripulation(winningTripulationLoss);
        barbarianCaravan->reduceTripulation(losingTripulationLoss);

        if(barbarianCaravan->getTripulation() <= 0)
        {
            removeCaravan(barbarianCaravan->getId());
            cout << "User caravan " << userCaravanId << " defeated barbarian " << barbarianId << "." << endl;
            cout << userCaravan->getTripulation() << endl;

            userCaravan->refillWater();

        }
         else
         {
             cout << "User caravan " << userCaravanId << " won the battle against barbarian caravan " << barbarianId << "." << endl;
             cout << barbarianCaravan->getTripulation() << endl;
         }


    } else {
        // Barbarian wins
        double doubleWinningTripulationLoss = barbarianCaravan->getTripulation() * 0.2;
        double doubleLosingTripulationLoss = userCaravan->getTripulation() * 0.4;

        winningTripulationLoss = static_cast<int>(std::ceil(doubleWinningTripulationLoss));
        losingTripulationLoss = static_cast<int>(std::ceil(doubleLosingTripulationLoss));

        barbarianCaravan->reduceTripulation(winningTripulationLoss);
        userCaravan->reduceTripulation(losingTripulationLoss);

         if(userCaravan->getTripulation() <= 0)
         {
              removeCaravan(userCaravanId);
                cout << "Barbarian " << barbarianId << " defeated user caravan " << userCaravanId << "." << endl;
                 barbarianCaravan->refillWater();
                cout << barbarianCaravan->getTripulation() << endl;
         }
         else{
               cout << "Barbarian caravan " << barbarianId << " won the battle against user caravan " << userCaravanId << "." << endl;
             cout << userCaravan->getTripulation() << endl;
         }

    }

}*/

/*void Map::resolveCombat(char userCaravanId, char barbarianId) {
    // Check if caravans are adjacent
    if (!areCaravansAdjacent(userCaravanId, barbarianId)) {
        cout << "No combat: Caravans are not adjacent." << endl;
        return;
    }

    // Find user caravan
    Caravan* userCaravan = nullptr;
    for (auto& c : caravans) {
        if (c->getId() == userCaravanId) {
            userCaravan = c.get();
            break;
        }
    }
    if (!userCaravan) {
        throw runtime_error("User caravan not found.");
    }

    // Find barbarian caravan
    cout << "barbarianId: " << barbarianId << endl;
     BarbarianCaravan* barbarianCaravan = nullptr;
    for (auto& c : caravans) {
        //Mudança para encontrar caravana bárbara com ID correto
        if (c->getType() == "Barbarian") {
            BarbarianCaravan* temp = dynamic_cast<BarbarianCaravan*>(c.get());
                if (temp && temp->getInternalId()[0] == barbarianId)
            {
               barbarianCaravan = temp;
                 break;
            }

        }

    }
    if (!barbarianCaravan) {
        throw runtime_error("Barbarian caravan not found.");
    }

    cout << "userCaravanId: " << userCaravanId << endl;
    // Perform combat logic
    srand(time(nullptr));
    int userScore = rand() % (userCaravan->getTripulation() + 1);
    int barbarianScore = rand() % (barbarianCaravan->getTripulation() + 1);

    cout << "userScore: " << userScore << " | barbarianScore: " << barbarianScore << endl;

    int winningTripulationLoss = 0;
    int losingTripulationLoss = 0;

    if (userScore >= barbarianScore) {
        // User wins
        double doubleWinningTripulationLoss = userCaravan->getTripulation() * 0.2;
        double doubleLosingTripulationLoss = barbarianCaravan->getTripulation() * 0.4;

        winningTripulationLoss = static_cast<int>(std::ceil(doubleWinningTripulationLoss));
        losingTripulationLoss = static_cast<int>(std::ceil(doubleLosingTripulationLoss));

        userCaravan->reduceTripulation(winningTripulationLoss);
        barbarianCaravan->reduceTripulation(losingTripulationLoss);

        if(barbarianCaravan->getTripulation() <= 0)
        {
            removeCaravan(barbarianCaravan->getId());
            cout << "User caravan " << userCaravanId << " defeated barbarian " << barbarianId << "." << endl;
            cout << userCaravan->getTripulation() << endl;

            userCaravan->refillWater();

        }
         else
         {
             cout << "User caravan " << userCaravanId << " won the battle against barbarian caravan " << barbarianId << "." << endl;
             cout << barbarianCaravan->getTripulation() << endl;
         }


    } else {
        // Barbarian wins
        double doubleWinningTripulationLoss = barbarianCaravan->getTripulation() * 0.2;
        double doubleLosingTripulationLoss = userCaravan->getTripulation() * 0.4;

        winningTripulationLoss = static_cast<int>(std::ceil(doubleWinningTripulationLoss));
        losingTripulationLoss = static_cast<int>(std::ceil(doubleLosingTripulationLoss));

        barbarianCaravan->reduceTripulation(winningTripulationLoss);
        userCaravan->reduceTripulation(losingTripulationLoss);

         if(userCaravan->getTripulation() <= 0)
         {
              removeCaravan(userCaravanId);
                cout << "Barbarian " << barbarianId << " defeated user caravan " << userCaravanId << "." << endl;
                 barbarianCaravan->refillWater();
                cout << barbarianCaravan->getTripulation() << endl;
         }
         else{
               cout << "Barbarian caravan " << barbarianId << " won the battle against user caravan " << userCaravanId << "." << endl;
             cout << userCaravan->getTripulation() << endl;
         }

    }

}*/

void Map::resolveCombat(char userCaravanId, char barbarianId) {

    if (!areCaravansAdjacent(userCaravanId, barbarianId)) {
        cout << "No combat: Caravans are not adjacent." << endl;
        return;
    }

    Caravan* userCaravan = nullptr;
    for (auto& c : caravans) {
        if (c->getId() == userCaravanId) {
            userCaravan = c.get();
            break;
        }
    }
    if (!userCaravan) {
        throw runtime_error("User caravan not found.");
    }

    cout << "barbarianId: " << barbarianId << endl;
     BarbarianCaravan* barbarianCaravan = nullptr;
    for (auto& c : caravans) {

        if (c->getType() == "Barbarian") {
            BarbarianCaravan* temp = dynamic_cast<BarbarianCaravan*>(c.get());

                if (temp && temp->getInternalId()[0] == barbarianId)
            {
               barbarianCaravan = temp;
                 break;
            }

        }

    }
    if (!barbarianCaravan) {
        throw runtime_error("Barbarian caravan not found.");
    }

    cout << "userCaravanId: " << userCaravanId << endl;
    srand(time(nullptr));
    int userScore = rand() % (userCaravan->getTripulation() + 1);
    int barbarianScore = rand() % (barbarianCaravan->getTripulation() + 1);

    cout << "userScore: " << userScore << " | barbarianScore: " << barbarianScore << endl;

    int winningTripulationLoss = 0;
    int losingTripulationLoss = 0;

    if (userScore >= barbarianScore) {

        double doubleWinningTripulationLoss = userCaravan->getTripulation() * 0.2;
        double doubleLosingTripulationLoss = barbarianCaravan->getTripulation() * 0.4;

        winningTripulationLoss = static_cast<int>(std::ceil(doubleWinningTripulationLoss));
        losingTripulationLoss = static_cast<int>(std::ceil(doubleLosingTripulationLoss));

        userCaravan->reduceTripulation(winningTripulationLoss);
        barbarianCaravan->reduceTripulation(losingTripulationLoss);

        if(barbarianCaravan->getTripulation() <= 0)
        {
            removeCaravan(barbarianCaravan->getInternalId()[0]);
            cout << "User caravan " << userCaravanId << " defeated barbarian " << barbarianId << "." << endl;
            cout << userCaravan->getTripulation() << endl;

            userCaravan->refillWater();

        }
         else
         {
             cout << "User caravan " << userCaravanId << " won the battle against barbarian caravan " << barbarianId << "." << endl;
             cout << barbarianCaravan->getTripulation() << endl;
         }


    } else {

        double doubleWinningTripulationLoss = barbarianCaravan->getTripulation() * 0.2;
        double doubleLosingTripulationLoss = userCaravan->getTripulation() * 0.4;

        winningTripulationLoss = static_cast<int>(std::ceil(doubleWinningTripulationLoss));
        losingTripulationLoss = static_cast<int>(std::ceil(doubleLosingTripulationLoss));

        barbarianCaravan->reduceTripulation(winningTripulationLoss);
        userCaravan->reduceTripulation(losingTripulationLoss);

         if(userCaravan->getTripulation() <= 0)
         {
              removeCaravan(userCaravanId);
                cout << "Barbarian " << barbarianId << " defeated user caravan " << userCaravanId << "." << endl;
                 barbarianCaravan->refillWater();
                cout << barbarianCaravan->getTripulation() << endl;
         }
         else{
               cout << "Barbarian caravan " << barbarianId << " won the battle against user caravan " << userCaravanId << "." << endl;
             cout << userCaravan->getTripulation() << endl;
         }

    }

}

/*bool Map::areCaravansAdjacent(char caravan1Id, char caravan2Id) const {
    // Find the first caravan
    const Caravan* caravan1 = nullptr;
    for (const auto& c : caravans) {
        if (c->getId() == caravan1Id) {
            caravan1 = c.get();
            break;
        }
    }
    if (!caravan1) {
        throw runtime_error("First caravan not found.");
    }

    // Find the second caravan
    const Caravan* caravan2 = nullptr;
    for (const auto& c : caravans) {
        if (c->getId() == caravan2Id) {
            caravan2 = c.get();
            break;
        }
    }
    if (!caravan2) {
        throw runtime_error("Second caravan not found.");
    }

    // Check adjacency
    int dx = abs(caravan1->getX() - caravan2->getX());
    int dy = abs(caravan1->getY() - caravan2->getY());

    return (dx <= 1 && dy <= 1); // True if adjacent or same position
}*/

/*bool Map::areCaravansAdjacent(char caravan1Id, char caravan2Id) const {
    // Find the first caravan
    const Caravan* caravan1 = nullptr;
    for (const auto& c : caravans) {
        if(c->getType() == "Barbarian")
        {
            const BarbarianCaravan* temp = dynamic_cast<const BarbarianCaravan*>(c.get());
            if(temp && temp->getInternalId()[0] == caravan1Id)
            {
                caravan1 = temp;
                break;
            }
        }
        else if (c->getId() == caravan1Id) {
            caravan1 = c.get();
            break;
        }
    }
    if (!caravan1) {
        throw runtime_error("First caravan not found.");
    }

    // Find the second caravan
    const Caravan* caravan2 = nullptr;
    for (const auto& c : caravans) {
        if(c->getType() == "Barbarian")
        {
            const BarbarianCaravan* temp = dynamic_cast<const BarbarianCaravan*>(c.get());
            if(temp && temp->getInternalId()[0] == caravan2Id)
            {
                caravan2 = temp;
                break;
            }
        }
        else if (c->getId() == caravan2Id) {
            caravan2 = c.get();
            break;
        }
    }

    if (!caravan2) {
        throw runtime_error("Second caravan not found.");
    }


    // Check adjacency
    int dx = abs(caravan1->getX() - caravan2->getX());
    int dy = abs(caravan1->getY() - caravan2->getY());

    return (dx <= 1 && dy <= 1); // True if adjacent or same position
}*/

/*bool Map::areCaravansAdjacent(char caravan1Id, char caravan2Id) const {
    // Find the first caravan
    const Caravan* caravan1 = nullptr;
    for (const auto& c : caravans) {
        if(c->getType() == "Barbarian")
        {
            const BarbarianCaravan* temp = dynamic_cast<const BarbarianCaravan*>(c.get());
            if(temp && temp->getInternalId()[0] == caravan1Id)
            {
                caravan1 = temp;
                break;
            }
        }
        else if (c->getId() == caravan1Id) {
            caravan1 = c.get();
            break;
        }
    }
    if (!caravan1) {
        throw runtime_error("First caravan not found.");
    }

    // Find the second caravan
    const Caravan* caravan2 = nullptr;
    for (const auto& c : caravans) {
        if(c->getType() == "Barbarian")
        {
            const BarbarianCaravan* temp = dynamic_cast<const BarbarianCaravan*>(c.get());
            if(temp && temp->getInternalId()[0] == caravan2Id)
            {
                caravan2 = temp;
                break;
            }
        }
        else if (c->getId() == caravan2Id) {
            caravan2 = c.get();
            break;
        }
    }

    if (!caravan2) {
        throw runtime_error("Second caravan not found.");
    }


    // Check adjacency
    int dx = abs(caravan1->getX() - caravan2->getX());
    int dy = abs(caravan1->getY() - caravan2->getY());

    return (dx <= 1 && dy <= 1); // True if adjacent or same position
}*/

bool Map::areCaravansAdjacent(char caravan1Id, char caravan2Id) const {

    const Caravan* caravan1 = nullptr;
    for (const auto& c : caravans) {
        if(c->getType() == "Barbarian")
        {
            const BarbarianCaravan* temp = dynamic_cast<const BarbarianCaravan*>(c.get());
            if(temp && temp->getInternalId()[0] == caravan1Id)
            {
                caravan1 = temp;
                break;
            }
        }
        else if (c->getId() == caravan1Id) {
            caravan1 = c.get();
            break;
        }
    }
    if (!caravan1) {
        throw runtime_error("First caravan not found.");
    }


    const Caravan* caravan2 = nullptr;
    for (const auto& c : caravans) {
        if(c->getType() == "Barbarian")
        {
            const BarbarianCaravan* temp = dynamic_cast<const BarbarianCaravan*>(c.get());
            if(temp && temp->getInternalId()[0] == caravan2Id)
            {
                caravan2 = temp;
                break;
            }
        }
        else if (c->getId() == caravan2Id) {
            caravan2 = c.get();
            break;
        }
    }

    if (!caravan2) {
        throw runtime_error("Second caravan not found.");
    }


    //Verifica se adjacente
    int dx = abs(caravan1->getX() - caravan2->getX());
    int dy = abs(caravan1->getY() - caravan2->getY());

    return (dx <= 1 && dy <= 1);
}

void Map::trade(char caravanId, char cityId, bool buying, int amount) {

    Caravan* caravan = nullptr;
    for (auto& c : caravans) {
        if (c->getId() == caravanId) {
            caravan = c.get();
            break;
        }
    }
    if (!caravan) {
        throw runtime_error("Caravana nao encontrada.");
    }


    const City* city = nullptr;
    for (const auto& c : cities) {
        if (c.getName() == cityId) {
            city = &c;
            break;
        }
    }
    if (!city) {
        throw runtime_error("Cidade nao encontrada.");
    }

    if (caravan->getX() != city->getX() || caravan->getY() != city->getY()) {
        throw runtime_error("Caravana nao esta na cidade.");
    }

    if (buying) {

        int cost = city->getBuyPrice() * amount;
        cout<<"cost: "<<cost<<"|cidade price buy:"<<city->getBuyPrice()<<"\nicoins caravan"<<caravan->getCoins()<<endl;
        if (cost > caravan->getCoins()) {
            cout << "Caravana " << caravanId << " nao tem moedas suficientes para comprar " << amount
                 << " unidades de mercadorias." << endl;
            return;
        }
        caravan->deductCoins(cost);
        caravan->loadCargo(amount);
        cout << "Caravana " << caravanId << " comprou " << amount << " unidades de mercadorias da cidade " << cityId
             << "." << endl;
    } else {
        cout<<"amount: "<<amount<<"|caravan goods amount:"<<caravan->getCargo()<<endl;

        if (amount > caravan->getCargo()) {
            cout << "Caravana " << caravanId << " nao tem mercadorias suficientes para vender " << amount << " unidades."
                 << endl;
            return;
        }
        int earnings = city->getSellPrice() * amount;
        caravan->addCoins(earnings);
        caravan->unloadCargo(amount);
        cout << "Caravana " << caravanId << " vendeu " << amount << " unidades de mercadorias para a cidade " << cityId
             << "." << endl;
    }
cout<<"carga caravan:"<<caravan->getCargo()<<"| icoins caravan"<<caravan->getCoins()<<endl;
}

void Map::setPosition(int x, int y, char value) {
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        throw out_of_range("Position is out of map bounds.");
    }
    grid[x][y] = value;
}

char Map::getPosition(int x, int y) const {
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        throw out_of_range("Position is out of map bounds.");
    }
    return grid[x][y];
}

void Map::removeCaravan(char caravanId) {
    //listAllCaravans();
    for (size_t i = 0; i < caravans.size(); ++i) {
        if(caravans[i]->getType() == "Barbarian")
        {
            BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(caravans[i].get());

            if (barbarianCaravan && barbarianCaravan->getInternalId()[0] == caravanId)
            {
                cout << "Removendo caravana" << caravans[i]->getId() << endl;
                int x = caravans[i]->getX();
                int y = caravans[i]->getY();
                if (x >= 0 && x < rows && y >= 0 && y < columns) {
                    grid[x][y] = '.';
                }
                caravans.erase(caravans.begin() + i);
                cout << "Caravana removida" << endl;
                listAllCaravans();
                return;
            }

        }else if (caravans[i]->getId() == caravanId) {

            cout << "Removendo caravana " << caravanId << endl;
            int x = caravans[i]->getX();
            int y = caravans[i]->getY();
            if (x >= 0 && x < rows && y >= 0 && y < columns) {
                grid[x][y] = '.';
            }
            caravans.erase(caravans.begin() + i);
            cout << "Caravana removida" << endl;
            //listAllCaravans();
            return;
        }
    }
    throw runtime_error("Caravan not found for removal.");
}

void Map::render(Buffer& buffer) const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            buffer.moveCursor(i, j);
            buffer.escrever(grid[i][j]);
        }
    }
}

/*void Map::render(Buffer& buffer) const {
    // Add column numbers
    for (int j = 0; j < columns; j++) {
        if(j < 10)
        {
            buffer.writeAt(0, j, j + '0');
        }
        else
        {
            buffer.writeAt(0, j, (j / 10) + '0');
            buffer.writeAt(1,j,(j%10) + '0');

        }
    }

    // Add row numbers and the map
    for (int i = 0; i < rows; ++i) {
        // Add row numbers
        if(i < 10)
        {
            buffer.writeAt(i+ (i > 1 ? 2 : 1), 0, i + '0');
            for (int j = 0; j < columns; ++j) {
                buffer.writeAt(i + (i > 1 ? 2 : 1), j, grid[i][j]);
            }
        }
        else{
            buffer.writeAt(i+ (i > 1 ? 2 : 1), 0, (i / 10) + '0');
            buffer.writeAt(i + (i > 1 ? 2 : 1), 1, (i % 10) + '0');
            for (int j = 0; j < columns; ++j) {
                buffer.writeAt(i + (i > 1 ? 2 : 1), j, grid[i][j]);
            }
        }
    }
}*/

void Map::addCoins(int amount) {
    playerCoins += amount;
}

void Map::removeCoins(int amount)
{
    playerCoins -= amount;
}

int Map::getCoins() const {
    return playerCoins;
}

void Map::listCityInfo(char city_id)
{
    City* city = nullptr;
    for (auto& c : cities) {
        if (c.getName() == city_id) {
            city = &c;
            break;
        }
    }

    if (!city) {
        throw std::runtime_error("Cidade não encontrada.");
    }

    city->showCityInfo();
}

void Map::listCaravanInfo(char caravan_id)
{
    bool found = false;
    for (const auto& c : caravans) {
        if (c->getId() == caravan_id) {
            std::cout << "Informaçoes da Caravana " << caravan_id << ":" << std::endl;
            std::cout << c->getAsString() << std::endl;
            found = true;
            break;
        }
    }

    if(!found)
    {
        std::cout<< "Caravana não encontrada." << std::endl;
    }

}

void Map::buyGoods(char caravanId, int amount) {

    Caravan* caravan = nullptr;
    for (const auto& c : caravans) {
        if (c->getId() == caravanId) {
            caravan = c.get();
            break;
        }
    }
    if (!caravan) {
        throw std::runtime_error("Caravana não encontrada com o ID: " + std::string(1, caravanId));
    }


    char cityId = getPosition(caravan->getX(), caravan->getY());
    if (!islower(cityId)) {
        throw std::runtime_error("Caravana " + std::string(1, caravanId) + " não está numa cidade.");
    }


    City* city = nullptr;
    for (auto& c : cities) {
        if(c.getName() == cityId)
        {
            city = &c;
            break;
        }
    }

    if (!city) {
        throw std::runtime_error("Cidade não encontrada com o ID: " + std::string(1, cityId));
    }

    city->buyGoods(caravan, amount, *this);
}

void Map::sellGoods(char caravanId) {

    Caravan* caravan = nullptr;
    for (const auto& c : caravans) {
        if (c->getId() == caravanId) {
            caravan = c.get();
            break;
        }
    }
    if (!caravan) {
        throw std::runtime_error("Caravana não encontrada com o ID: " + std::string(1, caravanId));
    }


    char cityId = getPosition(caravan->getX(), caravan->getY());
    if (!islower(cityId)) {
        throw std::runtime_error("Caravana " + std::string(1, caravanId) + " não está numa cidade.");
    }


    City* city = nullptr;
    for (auto& c : cities) {
        if(c.getName() == cityId)
        {
            city = &c;
            break;
        }
    }
    if (!city) {
        throw std::runtime_error("Cidade não encontrada com o ID: " + std::string(1, cityId));
    }

    city->sellGoods(caravan, *this);
}

void Map::hireCrew(char caravan_id, int amount)
{
    Caravan* caravan = nullptr;
    for (const auto& c : caravans) {
        if (c->getId() == caravan_id) {
            caravan = c.get();
            break;
        }
    }
    if (!caravan) {
        throw std::runtime_error("Caravana não encontrada com o ID: " + std::string(1, caravan_id));
    }

    char cityId = getPosition(caravan->getX(), caravan->getY());
    if (!islower(cityId)) {
        throw std::runtime_error("Caravana " + std::string(1, caravan_id) + " não está numa cidade.");
    }

    City* city = nullptr;
    for (auto& c : cities) {
        if(c.getName() == cityId)
        {
            city = &c;
            break;
        }
    }
    if (!city) {
        throw std::runtime_error("Cidade não encontrada com o ID: " + std::string(1, cityId));
    }

    city->buyCrew(caravan, amount, *this);
}

void Map::handleStorm(int l, int c, int radius) {
    std::cout << "Tempestade de areia iniciada em (" << l << ", " << c << ") com raio " << radius << ".\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (auto& caravan : caravans) {
        int caravanX = caravan->getX();
        int caravanY = caravan->getY();

        if (std::abs(caravanX - l) <= radius && std::abs(caravanY - c) <= radius) {
            std::cout << "Caravana " << caravan->getId() << " atingida pela tempestade.\n";

           if(caravan->getType() == "Comercial")
            {
                double destructionChance = (caravan->getCargo() > caravan->getCapacity() / 2.0) ? 0.5 : 0.25;
                if (dis(gen) < destructionChance) {
                    std::cout << "Caravana " << caravan->getId() << " foi destruída pela tempestade.\n";
                     removeCaravan(caravan->getId());

                } else {
                    int cargoLoss = caravan->getCargo() * 0.25;
                    caravan->unloadCargo(cargoLoss);
                    std::cout << "Caravana " << caravan->getId() << " perdeu " << cargoLoss << " unidades de carga.\n";
                }
            } else if(caravan->getType() == "Military")
            {
                cout << "Caravana militar atingida pela tempestade.\n";
                 int tripLoss = caravan->getTripulation() * 0.1;
                caravan->reduceTripulation(tripLoss);
                std::cout << "Caravana " << caravan->getId() << " perdeu " << tripLoss << " tripulantes.\n";

                if(dis(gen) < 0.33)
                {
                    std::cout << "Caravana " << caravan->getId() << " foi destruída pela tempestade.\n";
                      removeCaravan(caravan->getId());
                }

            }
             else if(caravan->getType() == "Barbarian")
            {

                 int tripLoss = caravan->getTripulation() * 0.1;
                caravan->reduceTripulation(tripLoss);
                std::cout << "Caravana " << caravan->getId() << " perdeu " << tripLoss << " tripulantes.\n";

                if(dis(gen) < 0.25)
                {
                     std::cout << "Caravana " << caravan->getId() << " foi destruída pela tempestade.\n";
                     removeCaravan(caravan->getId());
                }
            }
            else
            {
                  std::cout << "Caravana " << caravan->getId() << " não foi afetada pela tempestade.\n";

            }

        }
    }
    std::cout << "Tempestade de areia terminada.\n";
}

void Map::setAutoMode(char caravanId, bool autoMode) {

    Caravan* caravan = nullptr;
    for (const auto& c : caravans) {
        if (c->getId() == caravanId) {
            caravan = c.get();
            break;
        }
    }
    if (!caravan) {
        throw std::runtime_error("Caravana não encontrada com o ID: " + std::string(1, caravanId));
    }

    if (autoMode)
    {
        caravan->autonomousBehavior(true);
        std::cout << "Caravana " << caravanId << " colocada em modo automático.\n";
    }
    else
    {
        caravan->autonomousBehavior(false);
        std::cout << "Caravana " << caravanId << " colocada em modo manual.\n";
    }
}

/*void Map::updateAutonomousBehaviors() {

    for (const auto& caravan : caravans) {
        listAllCaravans();
        if (caravan->getTripulation() > 0) // Verificar se a caravana tem tripulantes antes de tentar mover-se
        {
            if (caravan->getAutoMode())
            {
                caravan->autonomousBehavior(true); // Chama o comportamento autônomo
            }

            cout << "Passei aqui mf" << endl;
            listAllCaravans();
            // Verificar se a caravana está numa cidade
            char cityId = getPosition(caravan->getX(), caravan->getY());
            if (islower(cityId))
            {
                std::cout << "Caravana " << caravan->getId() << " em modo automatico. A descansar na cidade " << cityId << ".\n";

            }

            cout << "Passei aqui mf" << endl;
            //listAllCaravans();
        }
    }

    cout << "Passei aqui mf" << endl;
}*/

/*void Map::updateAutonomousBehaviors() {

    for (auto it = caravans.begin(); it != caravans.end();) {
        if ((*it)->getTripulation() > 0) {
            if ((*it)->getAutoMode()) {
                (*it)->autonomousBehavior(true);
            }
            // Verificar se a caravana está numa cidade
            char cityId = getPosition((*it)->getX(), (*it)->getY());
            if (islower(cityId))
            {
                std::cout << "Caravana " << (*it)->getId() << " em modo automatico. A descansar na cidade " << cityId << ".\n";

            }

            ++it;
        }
        else{
            it = caravans.erase(it);
        }
    }
}*/

/*void Map::updateAutonomousBehaviors() {

    for (auto it = caravans.begin(); it != caravans.end();) {
        if ((*it)->getTripulation() > 0) {
            // Verifica se a caravana é bárbara
            if((*it)->getType() == "Barbarian"){

                BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());
                if (barbarianCaravan) {
                    if(barbarianCaravan->getAutoMode())
                    {
                        barbarianCaravan->autonomousBehavior(true);
                    }
                }
            }
            else if ((*it)->getAutoMode()) {
                (*it)->autonomousBehavior(true);
            }
            // Verificar se a caravana está numa cidade
            char cityId = getPosition((*it)->getX(), (*it)->getY());
            if (islower(cityId))
            {
                std::cout << "Caravana " << (*it)->getId() << " em modo automatico. A descansar na cidade " << cityId << ".\n";

            }

            ++it;
        }
        else{
            it = caravans.erase(it);
        }
    }
}*/

/*void Map::updateAutonomousBehaviors() {

    for (auto it = caravans.begin(); it != caravans.end();) {
        if ((*it)->getTripulation() > 0) {
            // Verificar se a caravana está numa cidade
            char cityId = getPosition((*it)->getX(), (*it)->getY());
            /*if (islower(cityId))
            {
                std::cout << "Caravana " << (*it)->getId() << " em modo automatico. A descansar na cidade " << cityId << ".\n";
                if((*it)->getType() != "Barbarian")
                {
                    //Se for uma caravana do utilizador, remove da cidade quando o ciclo terminar
                    for (auto& city : cities) {
                        if (city.getX() == (*it)->getX() && city.getY() == (*it)->getY()) {
                            city.removeCaravan((*it).get());
                            break;
                        }
                    }
                }
            }
            // Verifica se a caravana é bárbara
            else #1#if((*it)->getType() == "Barbarian"){

                BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());
                if (barbarianCaravan) {
                    if(barbarianCaravan->getAutoMode())
                    {
                        barbarianCaravan->autonomousBehavior(true);
                    }
                }
            }
            else if ((*it)->getAutoMode()) {
                (*it)->autonomousBehavior(true);
            }


            ++it;
        }
        else{
            it = caravans.erase(it);
        }
    }
}*/

/*void Map::updateAutonomousBehaviors() {

    for (auto it = caravans.begin(); it != caravans.end();) {
        if ((*it)->getTripulation() > 0) {
            // Verificar se a caravana está numa cidade
            char cityId = getPosition((*it)->getX(), (*it)->getY());
            /*if (islower(cityId))
            {
                std::cout << "Caravana " << (*it)->getId() << " em modo automatico. A descansar na cidade " << cityId << ".\n";
                if((*it)->getType() != "Barbarian")
                {
                    //Se for uma caravana do utilizador, remove da cidade quando o ciclo terminar
                    for (auto& city : cities) {
                        if (city.getX() == (*it)->getX() && city.getY() == (*it)->getY()) {
                            city.removeCaravan((*it).get());
                            break;
                        }
                    }
                }
            }
            // Verifica se a caravana é bárbara
            else #1#if((*it)->getType() == "Barbarian"){

                BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());
                if (barbarianCaravan) {
                    if(barbarianCaravan->getAutoMode())
                    {
                        // Verificar se é adjacente a uma caravana do utilizador
                        Caravan* closestUserCaravan = nullptr;
                        int minDistance = 9;

                         for (const auto& otherCaravan : caravans)
                            {
                                // Verifica se a caravana é do utilizador e não é ela própria
                                 if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != barbarianCaravan->getId())
                                {
                                   int distance = std::max(std::abs(barbarianCaravan->getX() - otherCaravan->getX()), std::abs(barbarianCaravan->getY() - otherCaravan->getY()));
                                    if (distance <= minDistance)
                                    {
                                        minDistance = distance;
                                        closestUserCaravan = otherCaravan.get();
                                    }
                                }
                            }

                        if (closestUserCaravan != nullptr) {
                           if (areCaravansAdjacent(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]))
                            {
                                // check if they are both inside a city
                                char posUserCaravan = getPosition(closestUserCaravan->getX(), closestUserCaravan->getY());
                                char posBarbarianCaravan = getPosition(barbarianCaravan->getX(), barbarianCaravan->getY());

                                if(!islower(posUserCaravan) && !islower(posBarbarianCaravan))
                                {
                                  resolveCombat(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]);
                                }
                            }
                        }
                        else
                         {
                           barbarianCaravan->autonomousBehavior(true);
                         }

                    }
                }
            }
            else if ((*it)->getAutoMode()) {
                (*it)->autonomousBehavior(true);
            }


            ++it;
        }
        else{
            it = caravans.erase(it);
        }
    }
}*/

/*void Map::updateAutonomousBehaviors() {
  for (auto it = caravans.begin(); it != caravans.end();) {
    if ((*it)->getTripulation() > 0) {
      // Verifica se a caravana está numa cidade
      char cityId = getPosition((*it)->getX(), (*it)->getY());

        if((*it)->getType() == "Barbarian"){

          BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());
          if (barbarianCaravan) {
            if(barbarianCaravan->getAutoMode())
            {
              // Verificar se é adjacente a uma caravana do utilizador
              Caravan* closestUserCaravan = nullptr;
              int minDistance = 9;

               for (const auto& otherCaravan : caravans)
              {
                // Verifica se a caravana é do utilizador e não é ela própria
                if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != barbarianCaravan->getId())
                {
                  int distance = std::max(std::abs(barbarianCaravan->getX() - otherCaravan->getX()), std::abs(barbarianCaravan->getY() - otherCaravan->getY()));
                    if (distance <= minDistance)
                    {
                        minDistance = distance;
                        closestUserCaravan = otherCaravan.get();
                    }
                  }
                }

                if (closestUserCaravan != nullptr) {
                  if (areCaravansAdjacent(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]))
                  {
                    // check if they are both inside a city
                    char posUserCaravan = getPosition(closestUserCaravan->getX(), closestUserCaravan->getY());
                    char posBarbarianCaravan = getPosition(barbarianCaravan->getX(), barbarianCaravan->getY());

                    if(!islower(posUserCaravan) && !islower(posBarbarianCaravan))
                    {
                        resolveCombat(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]);
                        }
                    }
                   else
                    {
                        // If no user caravan is nearby, move autonomously
                        barbarianCaravan->autonomousBehavior(true);
                        }

                }
                  else
                  {
                   barbarianCaravan->autonomousBehavior(true);
                   }
              }
            }
        }
         else if ((*it)->getAutoMode()) {
            (*it)->autonomousBehavior(true);
            }
            ++it;
        }
        else{
          it = caravans.erase(it);
        }
    }
}*/

/*void Map::updateAutonomousBehaviors() {
  for (auto it = caravans.begin(); it != caravans.end();) {
    if ((*it)->getTripulation() > 0) {
      // Verifica se a caravana está numa cidade
      char cityId = getPosition((*it)->getX(), (*it)->getY());

        if((*it)->getType() == "Barbarian"){

          BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());
          if (barbarianCaravan) {
            if(barbarianCaravan->getAutoMode())
            {
              // Verificar se é adjacente a uma caravana do utilizador
              Caravan* closestUserCaravan = nullptr;
              int minDistance = 9;

               for (const auto& otherCaravan : caravans)
              {
                // Verifica se a caravana é do utilizador e não é ela própria
                char posUserCaravan = getPosition(otherCaravan->getX(), otherCaravan->getY());
                 if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != barbarianCaravan->getId() && !islower(posUserCaravan))
                {
                  int distance = std::max(std::abs(barbarianCaravan->getX() - otherCaravan->getX()), std::abs(barbarianCaravan->getY() - otherCaravan->getY()));
                    if (distance <= minDistance)
                    {
                        minDistance = distance;
                        closestUserCaravan = otherCaravan.get();
                    }
                  }
                }

                if (closestUserCaravan != nullptr) {
                  if (areCaravansAdjacent(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]))
                  {
                     // check if they are both outside a city
                    char posUserCaravan = getPosition(closestUserCaravan->getX(), closestUserCaravan->getY());
                    char posBarbarianCaravan = getPosition(barbarianCaravan->getX(), barbarianCaravan->getY());

                   if(!islower(posUserCaravan) && !islower(posBarbarianCaravan))
                      {
                         resolveCombat(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]);
                    }
                 }
                }
                  else
                  {
                   barbarianCaravan->autonomousBehavior(true);
                   }
              }
            }
        }
         else if ((*it)->getAutoMode()) {
            (*it)->autonomousBehavior(true);
            }
            ++it;
        }
        else{
          it = caravans.erase(it);
        }
    }
}*/

/*void Map::updateAutonomousBehaviors() {
  for (auto it = caravans.begin(); it != caravans.end();) {
    if ((*it)->getTripulation() > 0) {

            if((*it)->getType() == "Barbarian"){

                 BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());
                 if (barbarianCaravan) {
                    if(barbarianCaravan->getAutoMode())
                    {
                          // Verificar se é adjacente a uma caravana do utilizador
                         Caravan* closestUserCaravan = nullptr;
                         int minDistance = 9;

                        for (const auto& otherCaravan : caravans)
                        {
                            // Verifica se a caravana é do utilizador e não é ela própria
                             char posUserCaravan = getPosition(otherCaravan->getX(), otherCaravan->getY());
                             if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != barbarianCaravan->getId() && !islower(posUserCaravan))
                             {
                                    int distance = std::max(std::abs(barbarianCaravan->getX() - otherCaravan->getX()), std::abs(barbarianCaravan->getY() - otherCaravan->getY()));
                                     if (distance <= minDistance)
                                    {
                                      minDistance = distance;
                                      closestUserCaravan = otherCaravan.get();
                                    }
                             }
                        }

                        if (closestUserCaravan != nullptr) {
                           if (areCaravansAdjacent(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]))
                           {
                                // check if they are both outside a city
                             char posUserCaravan = getPosition(closestUserCaravan->getX(), closestUserCaravan->getY());
                             char posBarbarianCaravan = getPosition(barbarianCaravan->getX(), barbarianCaravan->getY());

                             if(!islower(posUserCaravan) && !islower(posBarbarianCaravan))
                             {
                                 resolveCombat(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]);
                              }
                            }
                            else
                           {
                             // If no user caravan is nearby, move autonomously
                            barbarianCaravan->autonomousBehavior(true);
                           }
                      }
                      else
                     {
                         // If no user caravan is found, move autonomously
                         barbarianCaravan->autonomousBehavior(true);
                     }
                  }
                }
            }
             else if ((*it)->getAutoMode()) {
                (*it)->autonomousBehavior(true);
             }
             ++it;
        }
        else{
            it = caravans.erase(it);
        }
    }
}*/

/*void Map::updateAutonomousBehaviors() {
  for (auto it = caravans.begin(); it != caravans.end();) {
    if ((*it)->getTripulation() > 0) {

            if((*it)->getType() == "Barbarian"){

                 BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());
                 if (barbarianCaravan) {
                    if(barbarianCaravan->getAutoMode())
                    {
                          // Verificar se é adjacente a uma caravana do utilizador
                         Caravan* closestUserCaravan = nullptr;
                         int minDistance = 9;

                        for (const auto& otherCaravan : caravans)
                        {
                            // Verifica se a caravana é do utilizador e não é ela própria
                             char posUserCaravan = getPosition(otherCaravan->getX(), otherCaravan->getY());
                             if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != barbarianCaravan->getId() && !islower(posUserCaravan))
                             {
                                    int distance = std::max(std::abs(barbarianCaravan->getX() - otherCaravan->getX()), std::abs(barbarianCaravan->getY() - otherCaravan->getY()));
                                     if (distance <= minDistance)
                                    {
                                      minDistance = distance;
                                      closestUserCaravan = otherCaravan.get();
                                    }
                             }
                        }

                        if (closestUserCaravan != nullptr) {
                           if (areCaravansAdjacent(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]))
                           {
                                // check if they are both outside a city
                             char posUserCaravan = getPosition(closestUserCaravan->getX(), closestUserCaravan->getY());
                             char posBarbarianCaravan = getPosition(barbarianCaravan->getX(), barbarianCaravan->getY());

                             if(!islower(posUserCaravan) && !islower(posBarbarianCaravan))
                             {
                                 resolveCombat(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]);
                              }
                            }
                            else
                           {
                             // If no user caravan is nearby, move autonomously
                            barbarianCaravan->autonomousBehavior(true);
                           }
                      }
                      else
                     {
                         // If no user caravan is found, move autonomously
                         barbarianCaravan->autonomousBehavior(true);
                     }
                  }
                }
            }
            else if ((*it)->getType() == "Military")
            {
                 MilitaryCaravan* militaryCaravan = dynamic_cast<MilitaryCaravan*>(it->get());
                 if(militaryCaravan) {
                       if(militaryCaravan->getAutoMode())
                     {
                         Caravan* closestBarbarian = nullptr;
                         int minDistance = 7;
                           for (const auto& otherCaravan : caravans)
                            {
                              if (otherCaravan->getType() == "Barbarian" && otherCaravan->getId() != militaryCaravan->getId())
                                 {
                                    BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(otherCaravan.get());
                                    if(barbarianCaravan)
                                    {
                                       int distance = std::max(std::abs(militaryCaravan->getX() - otherCaravan->getX()), std::abs(militaryCaravan->getY() - otherCaravan->getY()));

                                       // Se a caravana estiver dentro do raio de busca
                                     if (distance <= minDistance)
                                     {
                                        minDistance = distance; //atualiza a distancia mínima
                                         closestBarbarian = otherCaravan.get(); //Guarda o ponteiro da caravana bárbara mais próxima
                                       }
                                   }
                                }
                            }
                            if(closestBarbarian != nullptr){
                                if (areCaravansAdjacent(militaryCaravan->getId(), closestBarbarian->getId()))
                                {
                                    char posMilitaryCaravan = getPosition(militaryCaravan->getX(), militaryCaravan->getY());
                                    char posBarbarianCaravan = getPosition(closestBarbarian->getX(), closestBarbarian->getY());

                                    if(!islower(posMilitaryCaravan) && !islower(posBarbarianCaravan))
                                    {
                                        //Não faz nada para que o método não volte a chamar a função de moveCaravan,
                                         //impedindo a caravana militar de ir para cima da caravana bárbara
                                        resolveCombat(militaryCaravan->getId(), closestBarbarian->getId());
                                    }
                                } else
                                {
                                  militaryCaravan->autonomousBehavior(true);
                                }
                            } else{
                                 militaryCaravan->autonomousBehavior(true);
                                }

                      }
                 }
            }

             else if ((*it)->getAutoMode()) {
                (*it)->autonomousBehavior(true);
             }
            ++it;
        }
        else{
            it = caravans.erase(it);
        }
    }
}*/

/*void Map::updateAutonomousBehaviors() {
    for (auto it = caravans.begin(); it != caravans.end();) {
        if ((*it)->getTripulation() > 0) {

            if((*it)->getType() == "Barbarian"){

                 BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());
                 if (barbarianCaravan) {
                    if(barbarianCaravan->getAutoMode())
                    {
                          // Verificar se é adjacente a uma caravana do utilizador
                         Caravan* closestUserCaravan = nullptr;
                         int minDistance = 9;

                        for (const auto& otherCaravan : caravans)
                        {
                            // Verifica se a caravana é do utilizador e não é ela própria
                             char posUserCaravan = getPosition(otherCaravan->getX(), otherCaravan->getY());
                             if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != barbarianCaravan->getId() && !islower(posUserCaravan))
                             {
                                    int distance = std::max(std::abs(barbarianCaravan->getX() - otherCaravan->getX()), std::abs(barbarianCaravan->getY() - otherCaravan->getY()));
                                     if (distance <= minDistance)
                                    {
                                      minDistance = distance;
                                      closestUserCaravan = otherCaravan.get();
                                    }
                             }
                        }

                        if (closestUserCaravan != nullptr) {
                           if (areCaravansAdjacent(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]))
                           {
                                // check if they are both outside a city
                             char posUserCaravan = getPosition(closestUserCaravan->getX(), closestUserCaravan->getY());
                             char posBarbarianCaravan = getPosition(barbarianCaravan->getX(), barbarianCaravan->getY());

                             if(!islower(posUserCaravan) && !islower(posBarbarianCaravan))
                             {
                                 resolveCombat(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]);
                              }
                            }
                            else
                           {
                             // If no user caravan is nearby, move autonomously
                            barbarianCaravan->autonomousBehavior(true);
                           }
                      }
                      else
                     {
                         // If no user caravan is found, move autonomously
                         barbarianCaravan->autonomousBehavior(true);
                     }
                  }
                }
            }
            else if ((*it)->getType() == "Military")
            {
                 MilitaryCaravan* militaryCaravan = dynamic_cast<MilitaryCaravan*>(it->get());
                 if(militaryCaravan) {
                       if(militaryCaravan->getAutoMode())
                     {
                         Caravan* closestBarbarian = nullptr;
                         int minDistance = 7;
                           for (const auto& otherCaravan : caravans)
                            {
                              if (otherCaravan->getType() == "Barbarian" && otherCaravan->getId() != militaryCaravan->getId())
                                 {
                                    BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(otherCaravan.get());
                                    if(barbarianCaravan)
                                    {
                                       int distance = std::max(std::abs(militaryCaravan->getX() - otherCaravan->getX()), std::abs(militaryCaravan->getY() - otherCaravan->getY()));

                                       // Se a caravana estiver dentro do raio de busca
                                     if (distance <= minDistance)
                                     {
                                        minDistance = distance; //atualiza a distancia mínima
                                         closestBarbarian = otherCaravan.get(); //Guarda o ponteiro da caravana bárbara mais próxima
                                       }
                                   }
                                }
                            }
                            if(closestBarbarian != nullptr){
                                 BarbarianCaravan* temp = dynamic_cast<BarbarianCaravan*>(closestBarbarian);
                                if (areCaravansAdjacent(militaryCaravan->getId(), temp->getInternalId()[0]))
                                {
                                    char posMilitaryCaravan = getPosition(militaryCaravan->getX(), militaryCaravan->getY());
                                    char posBarbarianCaravan = getPosition(closestBarbarian->getX(), closestBarbarian->getY());

                                    if(!islower(posMilitaryCaravan) && !islower(posBarbarianCaravan))
                                    {
                                        //Não faz nada para que o método não volte a chamar a função de moveCaravan,
                                         //impedindo a caravana militar de ir para cima da caravana bárbara
                                       //resolveCombat(militaryCaravan->getId(), temp->getInternalId()[0]);
                                    }
                                } else
                                {
                                  militaryCaravan->autonomousBehavior(true);
                                }
                            } else{
                                 militaryCaravan->autonomousBehavior(true);
                                }

                      }
                 }
            }else if((*it)->getType() == "Comercial"){
                ComercialCaravan* comercialCaravan = dynamic_cast<ComercialCaravan*>(it->get());
                 if(comercialCaravan) {
                       if(comercialCaravan->getAutoMode())
                     {
                         Caravan* closestCaravan = nullptr;
                         int minDistance = 7;
                           for (const auto& otherCaravan : caravans)
                            {
                              if (otherCaravan->getType() != "Barbarian" && otherCaravan->getId() != comercialCaravan->getId())
                                 {
                                    Caravan* closestCaravan = dynamic_cast<Caravan*>(otherCaravan.get());
                                    if(closestCaravan)
                                    {
                                       int distance = std::max(std::abs(comercialCaravan->getX() - otherCaravan->getX()), std::abs(comercialCaravan->getY() - otherCaravan->getY()));

                                       // Se a caravana estiver dentro do raio de busca
                                     if (distance <= minDistance)
                                     {
                                        minDistance = distance; //atualiza a distancia mínima
                                         closestCaravan = otherCaravan.get(); //Guarda o ponteiro da caravana bárbara mais próxima
                                       }
                                   }
                                }
                            }
                            if(closestCaravan != nullptr){
                                 Caravan* temp = dynamic_cast<Caravan*>(closestCaravan);
                                if (areCaravansAdjacent(comercialCaravan->getId(), temp->getId()))
                                {
                                    char posComercialCaravan = getPosition(comercialCaravan->getX(), comercialCaravan->getY());
                                    char posOtherCaravan = getPosition(closestCaravan->getX(), closestCaravan->getY());

                                    if(!islower(posComercialCaravan) && !islower(posOtherCaravan))
                                    {
                                        //Não faz nada para que o método não volte a chamar a função de moveCaravan,
                                         //impedindo a caravana militar de ir para cima da caravana bárbara
                                       //resolveCombat(militaryCaravan->getId(), temp->getInternalId()[0]);
                                    }
                                } else
                                {
                                  comercialCaravan->autonomousBehavior(true);
                                }
                            } else{
                                 comercialCaravan->autonomousBehavior(true);
                                }

                      }
                 }
            }else if ((*it)->getAutoMode()) {
                (*it)->autonomousBehavior(true);
             }
            ++it;
        }
        else{
            it = caravans.erase(it);
        }
    }
}*/

/*void Map::updateAutonomousBehaviors() {
    for (auto it = caravans.begin(); it != caravans.end();) {
        if ((*it)->getTripulation() > 0) {

            if((*it)->getType() == "Barbarian"){

                 BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());
                 if (barbarianCaravan) {
                    if(barbarianCaravan->getAutoMode())
                    {
                          // Verificar se é adjacente a uma caravana do utilizador
                         Caravan* closestUserCaravan = nullptr;
                         int minDistance = 9;

                        for (const auto& otherCaravan : caravans)
                        {
                            // Verifica se a caravana é do utilizador e não é ela própria
                             char posUserCaravan = getPosition(otherCaravan->getX(), otherCaravan->getY());
                             if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != barbarianCaravan->getId() && !islower(posUserCaravan))
                             {
                                    int distance = std::max(std::abs(barbarianCaravan->getX() - otherCaravan->getX()), std::abs(barbarianCaravan->getY() - otherCaravan->getY()));
                                     if (distance <= minDistance)
                                    {
                                      minDistance = distance;
                                      closestUserCaravan = otherCaravan.get();
                                    }
                             }
                        }

                        if (closestUserCaravan != nullptr) {
                           if (!areCaravansAdjacent(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]))
                           {
                             barbarianCaravan->autonomousBehavior(true);
                           }
                            else
                           {
                             // check if they are both outside a city
                            char posUserCaravan = getPosition(closestUserCaravan->getX(), closestUserCaravan->getY());
                             char posBarbarianCaravan = getPosition(barbarianCaravan->getX(), barbarianCaravan->getY());

                             if(!islower(posUserCaravan) && !islower(posBarbarianCaravan))
                             {
                                 resolveCombat(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]);
                             }
                           }

                      }
                      else
                     {
                         // If no user caravan is found, move autonomously
                         barbarianCaravan->autonomousBehavior(true);
                     }
                  }
                }
            }
            else if ((*it)->getType() == "Military")
            {
                 MilitaryCaravan* militaryCaravan = dynamic_cast<MilitaryCaravan*>(it->get());
                 if(militaryCaravan) {
                       if(militaryCaravan->getAutoMode())
                     {
                         Caravan* closestBarbarian = nullptr;
                         int minDistance = 7;
                           for (const auto& otherCaravan : caravans)
                            {
                              if (otherCaravan->getType() == "Barbarian" && otherCaravan->getId() != militaryCaravan->getId())
                                 {
                                    BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(otherCaravan.get());
                                    if(barbarianCaravan)
                                    {
                                       int distance = std::max(std::abs(militaryCaravan->getX() - otherCaravan->getX()), std::abs(militaryCaravan->getY() - otherCaravan->getY()));

                                       // Se a caravana estiver dentro do raio de busca
                                     if (distance <= minDistance)
                                     {
                                        minDistance = distance; //atualiza a distancia mínima
                                         closestBarbarian = otherCaravan.get(); //Guarda o ponteiro da caravana bárbara mais próxima
                                       }
                                   }
                                }
                            }
                            if(closestBarbarian != nullptr){
                                 BarbarianCaravan* temp = dynamic_cast<BarbarianCaravan*>(closestBarbarian);
                                if (areCaravansAdjacent(militaryCaravan->getId(), temp->getInternalId()[0]))
                                {
                                    char posMilitaryCaravan = getPosition(militaryCaravan->getX(), militaryCaravan->getY());
                                    char posBarbarianCaravan = getPosition(closestBarbarian->getX(), closestBarbarian->getY());

                                    if(!islower(posMilitaryCaravan) && !islower(posBarbarianCaravan))
                                    {
                                       //resolveCombat(militaryCaravan->getId(), temp->getInternalId()[0]);

                                    }
                                } else
                                {
                                  militaryCaravan->autonomousBehavior(true);
                                }
                            } else{
                                 militaryCaravan->autonomousBehavior(true);
                                }

                      }
                 }
            }else if((*it)->getType() == "Comercial"){
                 ComercialCaravan* comercialCaravan = dynamic_cast<ComercialCaravan*>(it->get());
                if(comercialCaravan) {
                    if(comercialCaravan->getAutoMode()){

                         Caravan* closestCaravan = nullptr;
                         int minDistance = 7;
                           for (const auto& otherCaravan : caravans)
                            {
                              if (otherCaravan->getType() != "Barbarian" && otherCaravan->getId() != comercialCaravan->getId())
                                 {
                                    Caravan* closestCaravan = dynamic_cast<Caravan*>(otherCaravan.get());
                                    if(closestCaravan)
                                    {
                                       int distance = std::max(std::abs(comercialCaravan->getX() - otherCaravan->getX()), std::abs(comercialCaravan->getY() - otherCaravan->getY()));

                                       // Se a caravana estiver dentro do raio de busca
                                     if (distance <= minDistance)
                                     {
                                        minDistance = distance; //atualiza a distancia mínima
                                         closestCaravan = otherCaravan.get();
                                       }
                                   }
                                }
                            }
                            if(closestCaravan != nullptr){
                                 Caravan* temp = dynamic_cast<Caravan*>(closestCaravan);
                                 if (!areCaravansAdjacent(comercialCaravan->getId(), temp->getId()))
                                {
                                   comercialCaravan->autonomousBehavior(true);

                                }
                                  else
                                {

                                    char posComercialCaravan = getPosition(comercialCaravan->getX(), comercialCaravan->getY());
                                    char posOtherCaravan = getPosition(closestCaravan->getX(), closestCaravan->getY());

                                    if(!islower(posComercialCaravan) && !islower(posOtherCaravan))
                                     {
                                     //   comercialCaravan->autonomousBehavior(true);

                                      }
                                }
                            }
                         else{
                           comercialCaravan->autonomousBehavior(true);
                         }
                      }
                 }
            }else if ((*it)->getAutoMode()) {
                (*it)->autonomousBehavior(true);
             }
            ++it;
        }
        else{
            it = caravans.erase(it);
        }
    }
}*/

/*void Map::updateAutonomousBehaviors() {
    for (auto it = caravans.begin(); it != caravans.end();) {
        if ((*it)->getTripulation() > 0) {

            if((*it)->getType() == "Barbarian"){

                 BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());
                 if (barbarianCaravan) {
                    if(barbarianCaravan->getAutoMode())
                    {
                          // Verificar se é adjacente a uma caravana do utilizador
                         Caravan* closestUserCaravan = nullptr;
                         int minDistance = 9;

                        for (const auto& otherCaravan : caravans)
                        {
                            // Verifica se a caravana é do utilizador e não é ela própria
                             char posUserCaravan = getPosition(otherCaravan->getX(), otherCaravan->getY());
                             if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != barbarianCaravan->getId() && !islower(posUserCaravan))
                             {
                                    int distance = std::max(std::abs(barbarianCaravan->getX() - otherCaravan->getX()), std::abs(barbarianCaravan->getY() - otherCaravan->getY()));
                                     if (distance <= minDistance)
                                    {
                                      minDistance = distance;
                                      closestUserCaravan = otherCaravan.get();
                                    }
                             }
                        }

                        if (closestUserCaravan != nullptr) {
                           if (!areCaravansAdjacent(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]))
                           {
                             barbarianCaravan->autonomousBehavior(true);
                           }
                            else
                           {
                             // check if they are both outside a city
                            char posUserCaravan = getPosition(closestUserCaravan->getX(), closestUserCaravan->getY());
                             char posBarbarianCaravan = getPosition(barbarianCaravan->getX(), barbarianCaravan->getY());

                             if(!islower(posUserCaravan) && !islower(posBarbarianCaravan))
                             {
                                 resolveCombat(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]);
                             }
                           }

                      }
                      else
                     {
                         // If no user caravan is found, move autonomously
                         barbarianCaravan->autonomousBehavior(true);
                     }
                  }
                }
            }
            else if ((*it)->getType() == "Military")
            {
                 MilitaryCaravan* militaryCaravan = dynamic_cast<MilitaryCaravan*>(it->get());
                 if(militaryCaravan) {
                       if(militaryCaravan->getAutoMode())
                     {
                         Caravan* closestBarbarian = nullptr;
                         int minDistance = 7;
                           for (const auto& otherCaravan : caravans)
                            {
                              if (otherCaravan->getType() == "Barbarian" && otherCaravan->getId() != militaryCaravan->getId())
                                 {
                                    BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(otherCaravan.get());
                                    if(barbarianCaravan)
                                    {
                                       int distance = std::max(std::abs(militaryCaravan->getX() - otherCaravan->getX()), std::abs(militaryCaravan->getY() - otherCaravan->getY()));

                                       // Se a caravana estiver dentro do raio de busca
                                     if (distance <= minDistance)
                                     {
                                        minDistance = distance; //atualiza a distancia mínima
                                         closestBarbarian = otherCaravan.get(); //Guarda o ponteiro da caravana bárbara mais próxima
                                       }
                                   }
                                }
                            }
                            if(closestBarbarian != nullptr){
                                 BarbarianCaravan* temp = dynamic_cast<BarbarianCaravan*>(closestBarbarian);
                                if (areCaravansAdjacent(militaryCaravan->getId(), temp->getInternalId()[0]))
                                {
                                    char posMilitaryCaravan = getPosition(militaryCaravan->getX(), militaryCaravan->getY());
                                    char posBarbarianCaravan = getPosition(closestBarbarian->getX(), closestBarbarian->getY());

                                    if(!islower(posMilitaryCaravan) && !islower(posBarbarianCaravan))
                                    {
                                       //resolveCombat(militaryCaravan->getId(), temp->getInternalId()[0]);

                                    }
                                } else
                                {
                                  militaryCaravan->autonomousBehavior(true);
                                }
                            } else{
                                 militaryCaravan->autonomousBehavior(true);
                                }

                      }
                 }
            }else if((*it)->getType() == "Comercial"){
                 ComercialCaravan* comercialCaravan = dynamic_cast<ComercialCaravan*>(it->get());
                if(comercialCaravan) {
                    if(comercialCaravan->getAutoMode()){

                         Caravan* closestCaravan = nullptr;
                         int minDistance = 7;
                           for (const auto& otherCaravan : caravans)
                            {
                              if (otherCaravan->getType() != "Barbarian" && otherCaravan->getId() != comercialCaravan->getId())
                                 {
                                    Caravan* caravan = dynamic_cast<Caravan*>(otherCaravan.get());
                                    if(caravan)
                                    {
                                       int distance = std::max(std::abs(comercialCaravan->getX() - otherCaravan->getX()), std::abs(comercialCaravan->getY() - otherCaravan->getY()));

                                       // Se a caravana estiver dentro do raio de busca
                                     if (distance <= minDistance)
                                     {
                                        minDistance = distance; //atualiza a distancia mínima
                                         closestCaravan = otherCaravan.get();
                                       }
                                   }
                                }
                            }
                            if(closestCaravan != nullptr){
                                 Caravan* temp = dynamic_cast<Caravan*>(closestCaravan);
                                 //LINE CHANGED
                                 if (!areCaravansAdjacent(comercialCaravan->getId(), temp->getId()))
                                {
                                   comercialCaravan->autonomousBehavior(true);

                                }
                            }
                         else{
                           comercialCaravan->autonomousBehavior(true);
                         }
                      }
                 }
            }else if ((*it)->getAutoMode()) {
                (*it)->autonomousBehavior(true);
             }
            ++it;
        }
        else{
            it = caravans.erase(it);
        }
    }
}*/

void Map::updateAutonomousBehaviors() {

    for (auto it = caravans.begin(); it != caravans.end();) {
        if ((*it)->getTripulation() > 0) {

            if((*it)->getType() == "Barbarian"){
                BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(it->get());
                if (barbarianCaravan)
                  {
                      if(barbarianCaravan->getAutoMode())
                      {
                            // Verificar se é adjacente a uma caravana do utilizador
                            Caravan* closestUserCaravan = nullptr;
                           int minDistance = 9;

                            for (const auto& otherCaravan : caravans)
                             {
                                 // Verifica se a caravana é do utilizador e não é ela própria
                                 char posUserCaravan = getPosition(otherCaravan->getX(), otherCaravan->getY());
                                 if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != barbarianCaravan->getId() && !islower(posUserCaravan))
                                {
                                  int distance = std::max(std::abs(barbarianCaravan->getX() - otherCaravan->getX()), std::abs(barbarianCaravan->getY() - otherCaravan->getY()));
                                    if (distance <= minDistance)
                                     {
                                        minDistance = distance;
                                       closestUserCaravan = otherCaravan.get();
                                      }
                                 }
                              }

                                if (closestUserCaravan != nullptr)
                                 {
                                    if (!areCaravansAdjacent(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]))
                                    {
                                      barbarianCaravan->autonomousBehavior(true);
                                     }
                                   else
                                      {

                                       char posUserCaravan = getPosition(closestUserCaravan->getX(), closestUserCaravan->getY());
                                      char posBarbarianCaravan = getPosition(barbarianCaravan->getX(), barbarianCaravan->getY());

                                         if(!islower(posUserCaravan) && !islower(posBarbarianCaravan))
                                         {
                                            resolveCombat(closestUserCaravan->getId(), barbarianCaravan->getInternalId()[0]);
                                         }
                                       }

                                  }
                               else
                                  {
                                    barbarianCaravan->autonomousBehavior(true);
                                  }
                            }
                   }
                }
                else if ((*it)->getType() == "Military")
                  {
                      MilitaryCaravan* militaryCaravan = dynamic_cast<MilitaryCaravan*>(it->get());
                       if(militaryCaravan) {
                           if(militaryCaravan->getAutoMode())
                            {
                                Caravan* closestBarbarian = nullptr;
                                int minDistance = 7;
                                  for (const auto& otherCaravan : caravans)
                                   {
                                      if (otherCaravan->getType() == "Barbarian" && otherCaravan->getId() != militaryCaravan->getId())
                                      {
                                        BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(otherCaravan.get());
                                          if(barbarianCaravan)
                                          {
                                              int distance = std::max(std::abs(militaryCaravan->getX() - otherCaravan->getX()), std::abs(militaryCaravan->getY() - otherCaravan->getY()));


                                          if (distance <= minDistance)
                                          {
                                             minDistance = distance;
                                              closestBarbarian = otherCaravan.get();
                                           }
                                        }
                                      }
                                   }
                                if(closestBarbarian != nullptr){
                                    BarbarianCaravan* temp = dynamic_cast<BarbarianCaravan*>(closestBarbarian);
                                       if (areCaravansAdjacent(militaryCaravan->getId(), temp->getInternalId()[0]))
                                       {
                                          char posMilitaryCaravan = getPosition(militaryCaravan->getX(), militaryCaravan->getY());
                                          char posBarbarianCaravan = getPosition(closestBarbarian->getX(), closestBarbarian->getY());

                                            if(!islower(posMilitaryCaravan) && !islower(posBarbarianCaravan))
                                          {
                                          //Não faz nada para que o método não volte a chamar a função de moveCaravan,
                                          //impedindo a caravana militar de ir para cima da caravana bárbara
                                             //resolveCombat(militaryCaravan->getId(), temp->getInternalId()[0]);
                                           }
                                       } else
                                      {
                                         militaryCaravan->autonomousBehavior(true);
                                       }
                                   } else{
                                      militaryCaravan->autonomousBehavior(true);
                                        }
                                }
                         }
                    }else if((*it)->getType() == "Comercial"){
                        ComercialCaravan* comercialCaravan = dynamic_cast<ComercialCaravan*>(it->get());
                        if(comercialCaravan) {
                            if(comercialCaravan->getAutoMode()){

                                 Caravan* closestCaravan = nullptr;
                                int minDistance = 7;
                                 for (const auto& otherCaravan : caravans)
                                  {
                                    if (otherCaravan->getType() != "Barbarian" && otherCaravan->getId() != comercialCaravan->getId())
                                     {
                                        Caravan* caravan = dynamic_cast<Caravan*>(otherCaravan.get());
                                       if(caravan)
                                        {
                                            int distance = std::max(std::abs(comercialCaravan->getX() - otherCaravan->getX()), std::abs(comercialCaravan->getY() - otherCaravan->getY()));

                                          if (distance <= minDistance)
                                           {
                                             minDistance = distance;
                                             closestCaravan = otherCaravan.get();
                                           }
                                      }
                                     }
                                 }
                             if(closestCaravan != nullptr){
                                Caravan* temp = dynamic_cast<Caravan*>(closestCaravan);
                                 if (!areCaravansAdjacent(comercialCaravan->getId(), temp->getId()))
                                    {
                                      comercialCaravan->autonomousBehavior(true);

                                    }
                                }
                            else{
                                  comercialCaravan->autonomousBehavior(true);
                             }
                        }
                    }

                }else if ((*it)->getType() == "Secret")
                        {
                            SecretCaravan* secretCaravan = dynamic_cast<SecretCaravan*>(it->get());
                               if(secretCaravan) {
                                   if (secretCaravan->getAutoMode()) {
                                       secretCaravan->autonomousBehavior(true);
                                 }
                             }
                        }
            else if ((*it)->getAutoMode())
              {
                   (*it)->autonomousBehavior(true);
              }

             ++it;
        }
        else
           {
            it = caravans.erase(it);
             }
      }
    }

void Map::handleCombats() {
    std::vector<std::pair<char, char>> combats;
    std::vector<char> processedCaravans;

    for (const auto& caravan1 : caravans) {
        if (std::find(processedCaravans.begin(), processedCaravans.end(), caravan1->getId()) != processedCaravans.end()) {
            continue;
        }

        for (const auto& caravan2 : caravans) {
            if (caravan1 == caravan2) continue;

            if (std::find(processedCaravans.begin(), processedCaravans.end(), caravan2->getId()) != processedCaravans.end()) {
                continue;
            }

            if (areCaravansAdjacent(caravan1->getId(), caravan2->getId())) {
                if ((isdigit(caravan1->getId()) && caravan2->getType() == "Barbarian") || (isdigit(caravan2->getId()) && caravan1->getType() == "Barbarian")) {
                    combats.push_back(std::make_pair(caravan1->getId(), caravan2->getId()));
                }
            }
        }
        processedCaravans.push_back(caravan1->getId());
    }

    for (const auto& combat : combats) {
        try {
            resolveCombat(combat.first, combat.second);
        } catch (const std::runtime_error& e) {
            std::cerr << "Erro ao resolver combate entre " << combat.first << " e " << combat.second << ": " << e.what() << std::endl;
        }
    }
}

void Map::spawnItem() {
    if ((currentTurn % itemSpawnInterval) != 0) {
        return;
    }

      if (items.size() >= maxItems) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4);

    int itemType = dis(gen);

    int spawnX, spawnY;
    int maxAttempts = 100;
    bool foundEmpty = false;

    for (int attempt = 0; attempt < maxAttempts; attempt++) {
         std::uniform_int_distribution<> rowDis(0, rows - 1);
        std::uniform_int_distribution<> colDis(0, columns - 1);
        spawnX = rowDis(gen);
         spawnY = colDis(gen);
        char position = getPosition(spawnX, spawnY);

         if (position == '.') {
             foundEmpty = true;
             break;
         }
    }

    if (!foundEmpty)
    {
        std::cout << "Não foi possível encontrar local vazio para item." << std::endl;
        return;
    }


    std::unique_ptr<Item> newItem;
    switch (itemType) {
        case 0:
            newItem = std::make_unique<PandoraBox>(spawnX, spawnY, this);
             std::cout << "Caixa de Pandora gerada em: (" << spawnX << ", " << spawnY << ")" << std::endl;
            break;
        case 1:
            newItem = std::make_unique<TreasureChest>(spawnX, spawnY, this);
            std::cout << "Baú do tesouro gerado em: (" << spawnX << ", " << spawnY << ")" << std::endl;
            break;
        case 2:
             newItem = std::make_unique<Cage>(spawnX, spawnY, this);
            std::cout << "Jaula gerada em: (" << spawnX << ", " << spawnY << ")" << std::endl;
            break;
        case 3:
             newItem = std::make_unique<Mine>(spawnX, spawnY, this);
            std::cout << "Mina gerada em: (" << spawnX << ", " << spawnY << ")" << std::endl;
            break;
         case 4:
             newItem = std::make_unique<Surprise>(spawnX, spawnY, this);
              std::cout << "Surpresa gerada em: (" << spawnX << ", " << spawnY << ")" << std::endl;
            break;
    }
   newItem->spawnTurn = currentTurn;
   newItem->spawnX = spawnX;
   newItem->spawnY = spawnY;
   items.push_back(std::move(newItem));

}

void Map::removeExpiredItems() {

       for (auto it = items.begin(); it != items.end(); ) {
           if ((currentTurn - (*it)->spawnTurn) >= itemDuration) {
                it = items.erase(it);
                  std::cout << "Item expirado foi removido.\n";
            } else {
                ++it;
            }
    }
}

void Map::handleItemPickup() {

    for(const auto& caravan : caravans)
    {
        for (auto it = items.begin(); it != items.end(); ) {

                 int itemX = (*it)->spawnX;
                 int itemY = (*it)->spawnY;

                if (std::abs(caravan->getX() - itemX) <= 1 && std::abs(caravan->getY() - itemY) <= 1)
                 {
                      (*it)->applyEffect(*caravan);
                     it = items.erase(it);

                    std::cout << "Item recolhido pela caravana " << caravan->getId() << ".\n";
                  }
                  else{
                       ++it;
                  }
            }
      }
}

void Map::setCurrentTurn() {
     currentTurn++;
}

void Map::listPrices() const {
    std::cout << "Precos das Mercadorias:\n";
    if (!cities.empty()) {

        const City& firstCity = cities.front();
        std::cout << "  Preco de Compra: " << firstCity.getBuyPrice() << "\n";
        std::cout << "  Preco de Venda: " << firstCity.getSellPrice() << "\n";
        std::cout << "  Preco de Contratacao de Tripulantes: " << firstCity.getBuyCrewPrice() << "\n";
    } else {
        std::cout << "Nao ha cidades no mapa.\n";
    }
}

void Map::addCaravan(std::unique_ptr<Caravan> caravan) {
    caravans.push_back(std::move(caravan));
    caravans.back()->setMap(this);
    std::cout << "Caravana adicionada ao mapa com ID: " << caravans.back()->getId() << std::endl;

}

void Map::addCity(City city) {
    cities.push_back(city);
    std::cout << "Cidade adicionada ao mapa com nome: " << cities.back().getName() << std::endl;
}

void Map::addItem(std::unique_ptr<Item> item) {
    items.push_back(std::move(item));
    std::cout << "Item adicionado ao mapa." << std::endl;
}

void Map::deductCoins(int cost)
{
    playerCoins -= cost;
}

int Map::getCounterCaravans() const
{
    return counterCaravans;
}

void Map::setCounterCaravans(int value)
{
    counterCaravans = value;
}

void Map::listAllCaravans() const {
    std::cout << "Caravans in the map:" << std::endl;
    for (const auto& caravan : caravans) {
        std::cout << "Caravan ID: " << caravan->getId()
                  << ", Position: (" << caravan->getX() << ", " << caravan->getY() << ")"
                  << ", Tripulation: " << caravan->getTripulation()
                  << ", Cargo: " << caravan->getCargo()
                  << ", Coins: " << caravan->getCoins() << std::endl;
    }
}








