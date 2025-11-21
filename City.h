/*#ifndef CITY_H
#define CITY_H

#include <iostream>
#include <vector>
#include <string>
#include "Caravan.h" // Supondo que a classe Caravan já tenha sido definida
#include "Map.h"

class Map;
class Caravan;
class Item;

class City {
private:
    int x, y;
    char name; // Nome da cidade (letra única)
    std::vector<Caravan*> caravans;// Lista de caravanas na cidade
    std::vector<Caravan*> caravansForSale; // Lista de caravanas para venda
    int buyPrice; // Preço de compra de mercadorias por tonelada
    int sellPrice; // Preço de venda de mercadorias por tonelada
    int buyCrewPrice; // Preço de compra de tripulantes por tripulante
    Map& map;

public:
    // Construtor da cidade, recebe o nome e preços
    City(char name, int buyPrice, int sellPrice, int buyCrewPrice, int x, int y, Map& map);
    ~City();

    // Método para adicionar uma caravana à cidade
    void addCaravan(Caravan* caravan);

    // Método para remover uma caravana da cidade
    void removeCaravan(Caravan* caravan);

    // Método para mostrar todas as caravanas na cidade
    void showCaravans() const;

    // Método para reabastecer a água das caravanas
    void refillWater(Caravan* caravan);

    // Método para comprar mercadorias para a caravana
    void buyGoods(Caravan* caravan, int tons, Map& map);

    // Método para vender mercadorias da caravana
    void sellGoods(Caravan* caravan, Map& map);

    // Método para comprar tripulantes para a caravana
    void buyCrew(Caravan* caravan, int crewMembers, Map& map);

    void initializeCaravansForSale();

    // Método para mostrar todas as caravanas estacionadas na cidade
    void showParkedCaravans() const;

    // Método para mostrar todas as caravanas disponíveis para venda
    void showForSaleCaravans() const;

    void sellCaravan(char type, Map& map);

    // Getters
    char getName() const { return name; }
    int getBuyPrice() const { return buyPrice; }
    int getSellPrice() const { return sellPrice; }
    int getBuyCrewPrice() const { return buyCrewPrice; }
    int getX() const;
    int getY() const;

    void setY(int y);
    void setX(int x);
    std::vector<Caravan*> getCaravanForSale() const;

    // Método para exibir as informações da cidade
    void showCityInfo() const;
    bool hasCaravanForSale(char type) const;
};

#endif // CITY_H*/


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


