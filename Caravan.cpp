//
// Created by frede on 30/11/2024.
//

#include "Caravan.h"
#include <iostream>
#include <cmath>
#include <random>
#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <bits/random.h>
#include <cmath>
#include <random>

#include "Map.h"

Caravan::Caravan(char id, int x, int y, int tripulation, int capacity, int water, int coins,int maxWater, std::string type, Map* map)
    : id(id), x(x), y(y), tripulation(tripulation), capacity(capacity), water(water), coins(coins),maxWater(maxWater), type(type), map(map) {}

Caravan* findCaravan(char caravan_id, const vector<unique_ptr<Caravan>>& caravans) {

    Caravan* caravan = nullptr;

    for (const auto& c : caravans) {
        if (c->getId() == caravan_id) {
            caravan = c.get();
            break;
        }
    }

    return caravan;
}

void Caravan::refillWater()
{
    water = maxWater;
}

void Caravan::reduceWater(int quantity)
{
    water -= quantity;
}

void Caravan::reduceTripulation(int n)
{
    tripulation -= n;
}

char Caravan::getId() const
{
    return id;
}

int Caravan::getTripulation() const
{
    return tripulation;
}

int Caravan::getCapacity() const
{
    return capacity;
}

string Caravan::getType() const
{
    return type;
}

int Caravan::getWater() const
{
    return water;
}

int Caravan::getCoins() const
{
    return coins;
}

bool Caravan::getAutoMode()
{
    return automode;
}

void Caravan::deductCoins(int cost)
{
    coins -= cost;
}

void Caravan::addCoins(int earnings)
{
    coins += earnings;
}

bool Caravan::loadCargo(int quantity)
{
    if (capacity - cargo >= quantity)
    {
        cargo += quantity;
        return true;
    }
    return false;
}

void Caravan::unloadCargo(int quantity)
{
    cargo -= quantity;
}

void Caravan::addTripulation(int n)
{
    tripulation += n;
}

void Caravan::setTripulation(int n)
{
    tripulation = n;
}

bool Caravan::fight(Caravan &enemy)
{
    int myStrength = tripulation * 10;
    int enemyStrength = enemy.tripulation * 10;
    return myStrength > enemyStrength;
}

string Caravan::getAsString() const
{
    return "Caravana " + std::to_string(id) + " - Tipo: " + type + " - Tripulacao: " + std::to_string(tripulation) +
           " - Capacidade: " + std::to_string(capacity) + " - Agua: " + std::to_string(water) + "/" +
           std::to_string(maxWater) + " - Carga: " + std::to_string(cargo) + " - Tripulacao: " + std::to_string(tripulation);
}

void Caravan::move(int newX, int newY)
{
    x = newX;
    y = newY;
}

int Caravan::getX() const
{
    return x;
}

int Caravan::getY() const
{
    return y;
}

void MilitaryCaravan::move(int newX, int newY)
{
    int waterConsumed = 0;
    if (getTripulation() > 0) {
        if (getTripulation() >= getCapacity() / 2) {
            waterConsumed = 3;
        } else {
            waterConsumed = 1;
        }
        reduceWater(waterConsumed);
        cout << "Caravan " << getId() << " consumed " << waterConsumed << " liters of water. Remaining water: " << getWater() << " liters.\n";
    }
    x = newX;
    y = newY;
}

void MilitaryCaravan::autonomousBehavior(bool automatico) {
    automode = automatico;
    if(automode == true){
        if(getTripulation() > 0)
        {
            std::cout << "Military caravan " << getId() << " is patrolling the area.\n";
            Caravan* closestBarbarian = nullptr;
            int minDistance = 7;
            for (const auto& otherCaravan : map->getCaravans())
            {
                if (otherCaravan->getType() == "Barbarian" && otherCaravan->getId() != getId())
                {
                   BarbarianCaravan* barbarianCaravan = dynamic_cast<BarbarianCaravan*>(otherCaravan.get());
                    if(barbarianCaravan)
                    {
                       int distance = std::max(std::abs(getX() - otherCaravan->getX()), std::abs(getY() - otherCaravan->getY()));
                    if (distance <= minDistance)
                     {
                        minDistance = distance;
                        closestBarbarian = otherCaravan.get();
                        std::cout << "Military caravan " << getId() << " is chasing barbarian caravan " << closestBarbarian->getId() << ".\n";
                     }
                 }
                }
            }
            if (closestBarbarian != nullptr)
            {
                int newX = getX();
                int newY = getY();
                if(closestBarbarian->getX() > getX())
                {
                    newX++;
                } else if(closestBarbarian->getX() < getX())
                {
                    newX--;
                }
                if(closestBarbarian->getY() > getY())
                {
                    newY++;
                } else if(closestBarbarian->getY() < getY())
                {
                    newY--;
                }


                 try
                 {
                        map->moveCaravan(getId(), newX, newY);
                        std::cout << "Military caravan " << getId() << " moved towards barbarian caravan " << closestBarbarian->getId() << ".\n";
                  }catch (const std::runtime_error& e) {
                      std::cerr << "Erro ao mover caravana " << getId() << ": " << e.what() << std::endl;
                 }


            }

        }
    }else
    {
        cout << "Mode automatico desativado" << endl;
    }
}

void ComercialCaravan::move(int newX, int newY)
{
    int waterConsumed = 0;
    if (getTripulation() > 0) {
        if (getTripulation() >= getCapacity() / 2) {
            waterConsumed = 2;
        } else {
            waterConsumed = 1;
        }

        reduceWater(waterConsumed);
        cout << "Caravan " << getId() << " consumed " << waterConsumed << " liters of water. Remaining water: " << getWater() << " liters.\n";
    }
    x = newX;
    y = newY;
}

void ComercialCaravan::autonomousBehavior(bool automatico) {
    automode = automatico;
    if (automode == true) {
        if (getTripulation() > 0) {
            std::cout << "Comercial caravan " << getId() << " is trading with nearby cities.\n";
            Caravan* closestUserCaravan = nullptr;
            int minUserDistance = 10;
            for (const auto& otherCaravan : map->getCaravans())
            {
                if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != getId())
                {
                    int distance = std::max(std::abs(getX() - otherCaravan->getX()), std::abs(getY() - otherCaravan->getY()));


                    if (distance < minUserDistance)
                    {
                        minUserDistance = distance;
                        closestUserCaravan = otherCaravan.get();
                    }
                }
            }
            if (closestUserCaravan != nullptr) {
                int newX = getX();
                int newY = getY();
                if(closestUserCaravan->getX() > getX())
                {
                    newX++;
                } else if(closestUserCaravan->getX() < getX())
                {
                    newX--;
                }
                if(closestUserCaravan->getY() > getY())
                {
                    newY++;
                } else if(closestUserCaravan->getY() < getY())
                {
                    newY--;
                }
                try{
                    map->moveCaravan(getId(), newX, newY);
                    std::cout << "Comercial caravan " << getId() << " moved towards user caravan " << closestUserCaravan->getId() << ".\n";
                }  catch(const std::runtime_error& e)
                {
                    std::cerr << "Erro ao mover caravana " << getId() << ": " << e.what() << std::endl;
                }

            }
            else
            {
                Item* closestItem = nullptr;
                int minItemDistance = 3;

                map->listAllItens();

                for (const auto& item : map->getItems())
                {
                    int distance = std::max(std::abs(getX() - item->spawnX), std::abs(getY() - item->spawnY));


                    if(distance < minItemDistance)
                    {
                        minItemDistance = distance;
                        closestItem = item.get();
                    }

                }
                if(closestItem != nullptr)
                {
                    int newX = getX();
                    int newY = getY();

                    if(closestItem->spawnX > getX())
                    {
                        newX++;
                    } else if(closestItem->spawnX < getX())
                    {
                        newX--;
                    }
                    if(closestItem->spawnY > getY())
                    {
                        newY++;
                    } else if(closestItem->spawnY < getY())
                    {
                        newY--;
                    }
                    try{
                        map->moveCaravan(getId(), newX, newY);
                        std::cout << "Comercial caravan " << getId() << " moved towards item.\n";
                    }  catch(const std::runtime_error& e)
                    {
                        std::cerr << "Erro ao mover caravana " << getId() << ": " << e.what() << std::endl;
                    }

                }
            }
        }
    } else {
        cout << "Modo Automatico desativado" << endl;
    }
}

void SecretCaravan::move(int newX, int newY)
{
    int waterConsumed = (getTripulation() > 20) ? 3 : 1;
    reduceWater(waterConsumed);
    cout << "Caravan " << getId() << " consumed " << waterConsumed << " liters of water. Remaining water: " << getWater() << " liters.\n";
    y = newY;
    x = newX;
}

void SecretCaravan::autonomousBehavior(bool automatico)
{
    if (getTripulation() > 0) {
        if (automatico) {
             std::cout << "Secret caravan " << getId() << " is moving stealthily.\n";
            Caravan* closestUserCaravan = nullptr;
            int minUserDistance = 10;

            for (const auto& otherCaravan : map->getCaravans())
            {
               // Verifica se a caravana é do utilizador e não é ela própria
               if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != getId())
               {
                 int distance = std::max(std::abs(getX() - otherCaravan->getX()), std::abs(getY() - otherCaravan->getY()));


                  if(distance < minUserDistance)
                    {
                       minUserDistance = distance;
                       closestUserCaravan = otherCaravan.get();
                    }
                }
            }
              if (closestUserCaravan != nullptr) {
                int newX = getX();
                int newY = getY();
                if(closestUserCaravan->getX() > getX())
                {
                    newX++;
                }
                  else if(closestUserCaravan->getX() < getX())
                {
                    newX--;
                }
                  if(closestUserCaravan->getY() > getY())
                {
                    newY++;
                }
                  else if(closestUserCaravan->getY() < getY())
                {
                    newY--;
                }
                 try{
                        map->moveCaravan(getId(), newX, newY);
                        std::cout << "Secret caravan " << getId() << " moved towards user caravan " << closestUserCaravan->getId() << ".\n";
                 } catch (const std::runtime_error& e) {
                    std::cerr << "Erro ao mover caravana " << getId() << ": " << e.what() << std::endl;
                 }

               if(map->areCaravansAdjacent(getId(), closestUserCaravan->getId()))
                {
                   closestUserCaravan->refillWater();
                   std::cout << "Secret caravan " << getId() << " refilled water from caravan " << closestUserCaravan->getId() << ".\n";
                }
            }
             else
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distr(-1, 1);

                int newX = getX() + distr(gen);
                int newY = getY() + distr(gen);
               if (newX < 0) newX = map->getRows() -1;
               if (newX >= map->getRows()) newX = 0;
               if (newY < 0) newY = map->getColumns() -1;
               if (newY >= map->getColumns()) newY = 0;

                    try{
                     map->moveCaravan(getId(), newX, newY);
                     std::cout << "Secret caravan " << getId() << " moved stealthily to: (" << newX << ", " << newY << ").\n";

                     }
                    catch (const std::runtime_error& e) {
                        std::cerr << "Erro ao mover caravana " << getId() << ": " << e.what() << std::endl;
                    }
            }
            if ((double)getWater() / getMaxWater() < 0.20)
            {
              City* closestCity = nullptr;
              int minCityDistance = 10;

                for (auto& city : map->getCities())
                 {
                    int distance = std::max(std::abs(getX() - city.getX()), std::abs(getY() - city.getY()));
                    if(distance < minCityDistance)
                     {
                            minCityDistance = distance;
                            closestCity = &city;
                    }
                 }

                if(closestCity)
                {
                  try
                   {
                       map->moveCaravan(getId(), closestCity->getX(), closestCity->getY());
                       cout << "Secret caravan " << getId() << " entered city " << closestCity->getName() << " to refill water.\n";
                       map->moveCaravan(getId(), getX(), getY());
                       cout << "Secret caravan " << getId() << " exited city " << closestCity->getName() << ".\n";

                   } catch (const std::runtime_error& e) {
                    std::cerr << "Erro ao mover caravana " << getId() << ": " << e.what() << std::endl;
                }

              }

           }
             turnsAlive++;
             if (turnsAlive > 10)
             {
                std::cout << "Secret caravan " << getId() << " has vanished after 10 turns." << std::endl;
                 map->removeCaravan(getId());
             }

        }
        else{
             std::cout << "Secret caravan " << getId() << " is waiting for instructions.\n";
        }
    }
}

void BarbarianCaravan::autonomousBehavior(bool automatico) {
    if (getTripulation() > 0)
    {
        std::cout << "Barbarian caravan " << getId() << " is moving autonomously.\n";
        Caravan* closestUserCaravan = nullptr;
        int minDistance = 9;
        for (const auto& otherCaravan : map->getCaravans())
        {
             char posUserCaravan = map->getPosition(otherCaravan->getX(), otherCaravan->getY());
            if (isdigit(otherCaravan->getId()) && otherCaravan->getId() != getId() && !islower(posUserCaravan))
            {
                int distance = std::max(std::abs(getX() - otherCaravan->getX()), std::abs(getY() - otherCaravan->getY()));
                if (distance <= minDistance)
                {
                    minDistance = distance;
                    closestUserCaravan = otherCaravan.get();
                    std::cout << "Barbarian caravan " << getId() << " is chasing user caravan " << closestUserCaravan->getId() << ".\n";
                }
            }
        }

        if (closestUserCaravan != nullptr) {
            int newX = getX();
            int newY = getY();
            if(closestUserCaravan->getX() > getX())
            {
                newX++;
            }
            else if(closestUserCaravan->getX() < getX())
            {
                newX--;
            }
            if(closestUserCaravan->getY() > getY())
            {
                newY++;
            } else if(closestUserCaravan->getY() < getY())
            {
                newY--;
            }
             try {
                 char newPos = map->getPosition(newX, newY);
                if(!islower(newPos))
                {
                     map->moveCaravan(getInternalId()[0], newX, newY);
                }
                std::cout << "Barbarian caravan " << getId() << " moved towards user caravan " << closestUserCaravan->getId() << ".\n";
            }
            catch (const std::runtime_error& e)
            {
                std::cerr << "Erro ao mover caravana " << getId() << ": " << e.what() << std::endl;
            }
        }
        else
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(-1, 1);
            int newX = getX();
            int newY = getY();
            int maxAttempts = 100;
            for (int attempt = 0; attempt < maxAttempts; ++attempt) {
                int tempX = getX() + distr(gen);
                int tempY = getY() + distr(gen);
                 if (tempX < 0) tempX = map->getRows() -1;
                if (tempX >= map->getRows()) tempX = 0;
                if (tempY < 0) tempY = map->getColumns() -1;
                if (tempY >= map->getColumns()) tempY = 0;

                char newPos = map->getPosition(tempX, tempY);

                bool isOccupied = false;
                 for(const auto& caravan : map->getCaravans())
                 {
                     if(caravan->getX() == tempX && caravan->getY() == tempY)
                     {
                         isOccupied = true;
                         break;
                     }
                 }

                 if (newPos == '.' && !isOccupied) {

                        newX = tempX;
                        newY = tempY;

                        try {
                            map->moveCaravan(getInternalId()[0], newX, newY);
                            std::cout << "Barbarian caravan " << getId() << " moved randomly to (" << newX << ", " << newY << ").\n";

                           break;
                        } catch (const std::runtime_error& e) {
                           std::cerr << "Erro ao mover caravana " << getId() << ": " << e.what() << std::endl;
                            break;
                        }
                    }
              }
          }
        }

}

ComercialCaravan::~ComercialCaravan() {
    cout << "Comercial caravan " << getId() << " was destroyed.\n";
}

MilitaryCaravan::~MilitaryCaravan() {
    cout << "Military caravan " << getId() << " was destroyed.\n";
}

SecretCaravan::~SecretCaravan()
{
    cout << "Secret caravan " << getId() << " was destroyed.\n";
}

BarbarianCaravan::~BarbarianCaravan()
{
    cout << "Barbarian caravan " << getId() << " was destroyed.\n";
}

void BarbarianCaravan::move(int newX, int newY)
{
    y = newY;
    x = newX;
}














