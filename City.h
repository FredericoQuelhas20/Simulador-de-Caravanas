


#ifndef CITY_H
#define CITY_H

#include <iostream>
#include <vector>
#include <string>
#include "Caravan.h"
#include "Map.h"

class Map;
class Caravan;
class Item;

class City {
private:
    int x, y;
    char name;
    std::vector<Caravan*> caravans;
    //std::vector<std::unique_ptr<Caravan>> caravansForSale;
    int buyPrice = 0;
    int sellPrice = 0;
    int buyCrewPrice = 1;
    int caravanPrice = 0;
    int ComercioForSale = 1;
    int MilitarForSale = 1;
    int SecretoForSale = 1;
    char gridvalue;
    //Map* map;

public:
    City(char name, int x, int y, char gridvalue);
    ~City();
    void addCaravan(Caravan* caravan);
    void removeCaravan(Caravan* caravan);
    void showCaravans() const;
    void refillWater(Caravan* caravan);
    void buyGoods(Caravan* caravan, int tons, Map& map);
    void sellGoods(Caravan* caravan, Map& map);
    void buyCrew(Caravan* caravan, int crewMembers, Map& map);
    void initializeCaravansForSale();
    void showParkedCaravans() const;
    void showForSaleCaravans() const;
    void sellCaravan(char type, Map& map);
    char getName() const { return name; }
    int getBuyPrice() const { return buyPrice; }
    void setBuyPrice(int value){buyPrice = value;}
    int getSellPrice() const { return sellPrice; }
    void setSellPrice(int value){sellPrice = value;}
    int getBuyCrewPrice() const { return buyCrewPrice; }
    void setBuyCrewPrice(int value){buyCrewPrice = value;}
    int getX() const;
    int getY() const;
    void setY(int y);
    void setX(int x);
    std::vector<std::unique_ptr<Caravan>> getCaravanForSale() const;
    void showCityInfo() const;
    bool hasCaravanForSale(char type) const;
    char getGridValue() const;
    void setCaravanPrice(int value){caravanPrice = value;}
};

#endif // CITY_H


