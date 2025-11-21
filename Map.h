#ifndef MAP_H
#define MAP_H


#include <vector>
#include <memory>

#include "Buffer.h"
#include "Item.h"
#include "City.h"
using namespace std;

class Caravan;
class City;

class Map {
private:
    int rows, columns;
    unique_ptr<unique_ptr<char[]>[]> grid;
    vector<City> cities;
    vector<unique_ptr<Caravan>> caravans;
    vector<unique_ptr<Item>> items;
    int counterCaravans = 0;
    int nextBarbarianId = 1;
    int playerCoins = 1000;
    int currentTurn = 0;
    int itemSpawnInterval = 0;
    int maxItems = 5;
    int itemDuration = 5;
    int turnsNewBarbarian = 40;
    int durationBarbarian = 60;

public:
    Map(int rows, int columns);
    ~Map() = default;
    void loadMap(const string& filename);
    std::string generateUUID();
    void spawnBarbarian();
    int getRows() const;
    int getColumns() const;
    void setPosition(int x, int y, char value);
    char getPosition(int x, int y) const;


    void addCaravan(unique_ptr<Caravan> caravan);
    void addCity(City city);
    void addItem(unique_ptr<Item> item);

    void removeCaravan(char caravanId);

    vector<City> getCities() const;
    const vector<unique_ptr<Caravan>>& getCaravans() const;
    const vector<unique_ptr<Item>>& getItems() const;
    void listAllItens() const;
    void moveCaravan(char caravanId, int newX, int newY);

    void moveCaravan(char caravanId, string direction);
    void trade(char caravanId, char cityId, bool buying, int amount);
    void resolveCombat(char userCaravanId, char barbarianId);
    bool areCaravansAdjacent(char caravan1Id, char caravan2Id) const;
    void interactWithItem(char carvanId, int itemIndex);
    void render(Buffer& buffer) const;
    void addCoins(int amount);
    void removeCoins(int amount);
    int getCoins() const;
    void buyCaravan(char city_id, char type);
    void deductCoins(int cost);
    int getSellPrice() const;
    int getBuyPrice() const;
    void listCityInfo(char city_id);
    void listCaravanInfo(char caravan_id);
    void buyGoods(char caravanId, int amount);
    void sellGoods(char caravan_id);
    void hireCrew(char caravan_id, int amount);
    void handleStorm(int i, int y, int radius);
    void setAutoMode(char caravanId, bool mode);
    void updateAutonomousBehaviors();
    void handleCombats();
    void spawnItem();
    void removeExpiredItems();
    void handleItemPickup();
    void setCurrentTurn();
    void listPrices() const;
    int getCounterCaravans() const;
    void setCounterCaravans(int value);
    void listAllCaravans() const;
};

#endif // MAP_H
