//
// Created by frede on 30/11/2024.
//

#ifndef CARAVANA_H
#define CARAVANA_H
#include <string>

#include "Map.h"

using namespace std;

class Map;

class Caravan {

protected:
    char id;
    int x, y;
    int tripulation;
    int capacity;
    int cargo = 0;
    int coins;
    int water;
    int maxWater;
    std::string type;
    bool automode = false;
    Map* map;
public:

    Caravan(char id, int x, int y, int tripulation, int capacity, int water, int coins,int maxWater, std::string type, Map* map);

    virtual ~Caravan() = default;
    virtual void move(int newX, int newY) = 0;
    virtual void autonomousBehavior(bool automatico) = 0;
    void refillWater();
    void reduceWater(int quantity);
    void reduceTripulation(int n);
    char getId() const;
    int getTripulation() const;
    int getCapacity() const;
    int getCargo() const {return cargo;}
    void setMap(Map* map) { this->map = map;}
    int getMaxWater() const {return maxWater;}
    Map* getMap() {return map;}
    void deductCoins(int cost);
    string getType() const;
    int getX() const;
    int getY() const;
    int getWater() const;
    bool loadCargo(int quantity);
    void unloadCargo(int quantity);
    void addTripulation(int n);
    void setTripulation(int n);
    int getCoins() const;
    bool fight(Caravan& enemy);
    string getAsString() const;
    void addCoins(int earnings);
    bool getAutoMode();
};

class ComercialCaravan : public Caravan
{
public:
    ComercialCaravan(char id, int x, int y, Map* map)
        : Caravan(id, x, y, 20, 40, 100, 100, 100, "Comercial", map){}

    ~ComercialCaravan() override;

    void move(int newX, int newY) override;

    void autonomousBehavior(bool automatico) override;

};

class MilitaryCaravan : public Caravan {
public:
    MilitaryCaravan(char id, int x, int y, Map* map)
        : Caravan(id, x, y,40, 5, 400, 100,400, "Military", map) {}

    ~MilitaryCaravan() override;

    void move(int newX, int newY) override;

    void autonomousBehavior(bool automatico) override;

    void chaseBarbarians();
};

class SecretCaravan : public Caravan {
private:
    int turnsAlive = 0;
public:
    SecretCaravan(char id, int x, int y, Map* map)
        : Caravan(id, x, y,50, 10, 300, 100, 300, "Secret", map) {}

    ~SecretCaravan() override;

    void move(int newX, int newY) override;

    void autonomousBehavior(bool automatico) override;
};

class BarbarianCaravan : public Caravan
{
private:
    std::string internalId;
public:
    BarbarianCaravan(char id, int x, int y, Map* map, string internalId) : Caravan(id, x, y, 2, 0, 0, 0, 0,  "Barbarian", map), internalId(internalId)
    {
        automode = true;
    }

    ~BarbarianCaravan() override;

    void autonomousBehavior(bool automatico) override;

    void move(int newX, int newY) override;
    string getInternalId() const {return internalId;}
};

#endif //CARAVANA_H
