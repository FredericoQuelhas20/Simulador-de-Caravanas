#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>
#include "Caravan.h"

class Caravan;
class Map;

class Item {
protected:
    int x, y;
    std::string name;
    int duration;
    Map* map;

public:
    int spawnTurn = 0;
    int spawnX = 0;
    int spawnY = 0;

    Item(std::string name, int duration, int x, int y, Map* map) : name(name), duration(duration), x(x), y(y), map(map) {}

    virtual ~Item() = default;


    virtual void applyEffect(Caravan& caravan) = 0;
    virtual string getAsString() const { return name; }

    void decreaseDuration() { if (duration > 0) duration--; }
    bool isActive() const { return duration > 0; }
    std::string getName() const { return name; }
    int getX() const;
    int getY() const;
};

class PandoraBox : public Item {
public:
    PandoraBox(int x, int y, Map* map) : Item("Pandora's Box", 20, x, y, map) {}
    ~PandoraBox();

    void applyEffect(Caravan& caravan) override;
    string getAsString() const override;
};

class TreasureChest : public Item {
public:
    TreasureChest(int x, int y, Map* map) : Item("Treasure Chest", 20, x, y, map) {}
    ~TreasureChest() override;

    void applyEffect(Caravan& caravan) override;
    string getAsString() const override;
};

class Cage : public Item {
public:
    Cage(int x, int y, Map* map) : Item("Cage", 20, x, y, map) {}
    ~Cage() override;


    void applyEffect(Caravan& caravan) override;
    string getAsString() const override;
};


class Mine : public Item {
public:
    Mine(int x, int y, Map* map) : Item("Mine", 20, x, y, map) {}
    ~Mine() override;


    void applyEffect(Caravan& caravan) override;
    string getAsString() const override;
};


class Surprise : public Item {
public:
    Surprise(int x, int y, Map* map) : Item("Surprise", 20, x, y, map) {}
    ~Surprise() override;


    void applyEffect(Caravan& caravan) override;
    string getAsString() const override;
};

#endif // ITEM_H
