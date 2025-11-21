

#include "City.h"
#include <iostream>
#include <algorithm>
#include "Map.h"
#include "Caravan.h"
#include "Item.h"

City::City(char name, int x, int y, char gridvalue)
    : x(x), y(y), name(name), gridvalue(gridvalue){}

City::~City() {
    for (auto& caravan : caravans) {
        delete caravan;
    }
    caravans.clear();
}

void City::addCaravan(Caravan* caravan)
{
    caravans.push_back(caravan);
    std::cout << "Caravan " << caravan->getId() << " added to city " << name << ".\n";
}

void City::removeCaravan(Caravan* caravan)
{
    for (auto it = caravans.begin(); it != caravans.end(); ++it)
    {
        if (*it == caravan)
        {
            caravans.erase(it);
            std::cout << "Caravan " << caravan->getId() << " removed from city " << name << ".\n";
            return;
        }
    }
    std::cout << "Caravan " << caravan->getId() << " not found in city " << name << ".\n";
}

void City::showCaravans() const
{
    std::cout << "Caravans in city " << name << ":\n";
    for (const auto& c : caravans)
    {
        std::cout << c->getAsString() << "\n";
    }
}

void City::refillWater(Caravan* caravan)
{
    caravan->refillWater();
    std::cout << "Caravan " << caravan->getId() << " water refilled in city " << name << ".\n";
}

void City::buyGoods(Caravan* caravan, int tons, Map& map) {
    int cost = buyPrice * tons;
    if (map.getCoins() >= cost && caravan->loadCargo(tons)) {
        map.removeCoins(cost);
        std::cout << "Comprou " << tons << " toneladas de mercadorias por " << cost << " moedas.\n";
    } else {
        std::cout << "Não foi possível comprar as mercadorias. Verifique as suas moedas ou o espaço na caravana.\n";
    }
}

void City::sellGoods(Caravan* caravan, Map& map) {
    int tons = caravan->getCargo();
    if (tons > 0) {
        int earnings = sellPrice * tons;
        map.addCoins(earnings);
        caravan->unloadCargo(tons);
        std::cout << "Vendeu " << tons << " toneladas de mercadorias por " << earnings << " moedas.\n";
    } else {
        std::cout << "Não há mercadorias para vender.\n";
    }
}

void City::buyCrew(Caravan* caravan, int crewMembers, Map& map) {
    int cost = buyCrewPrice * crewMembers;
    if (map.getCoins() >= cost) {
        caravan->addTripulation(crewMembers);
        map.deductCoins(cost);
        std::cout << "Contratou " << crewMembers << " tripulantes por " << cost << " moedas.\n";
    } else {
        std::cout << "Não tem moedas suficientes para contratar tripulação.\n";
    }
}

void City::showParkedCaravans() const
{
    std::cout << "Parked caravans in city " << name << ":\n";
    for (const auto& c : caravans)
    {
        std::cout << c->getAsString() << "\n";
    }
}

void City::showForSaleCaravans() const
{
    cout << "Carvanas para venda: " << endl;

    if(ComercioForSale == 0 && MilitarForSale == 0 && SecretoForSale == 0)
    {
        std::cout << "No caravans for sale in city " << name << ".\n";
        return;
    }

    if(ComercioForSale == 1)
    {
        std::cout << "Comercial" << endl;
    }

    if(MilitarForSale == 1)
    {
        std::cout << "Militar" << endl;
    }

    if(SecretoForSale == 1)
    {
        std::cout << "Secreta" << endl;
    }
}

void City::sellCaravan(char type, Map& map) {
    if(type == 'C')
    {
        if(ComercioForSale == 1)
        {
            int nextId = map.getCounterCaravans() + 1;
            std::string stringNumber = std::to_string(nextId);
            char id = stringNumber[0];
            map.addCaravan(std::make_unique<ComercialCaravan>(id, x, y, &map));
            map.deductCoins(100);
            ComercioForSale = 0;
            map.setCounterCaravans(nextId);
        }else
        {
            cout << "Caravana Comercial esgotada" << endl;
        }
    }

    if(type == 'M')
    {
        if(MilitarForSale == 1)
        {
            int nextId = map.getCounterCaravans() + 1;
            std::string stringNumber = std::to_string(nextId);
            char id = stringNumber[0];
            map.addCaravan(std::make_unique<MilitaryCaravan>(id, x, y, &map));
            map.deductCoins(100);
            MilitarForSale = 0;
            map.setCounterCaravans(nextId);
        }else
        {
            cout << "Caravana Militar esgotada" << endl;
        }
    }

    if(type == 'S')
    {
        if(SecretoForSale == 1)
        {
            int nextId = map.getCounterCaravans() + 1;
            std::string stringNumber = std::to_string(nextId);
            char id = stringNumber[0];
            map.addCaravan(std::make_unique<SecretCaravan>(id, x, y, &map));
            map.deductCoins(100);
            SecretoForSale = 0;
            map.setCounterCaravans(nextId);
        }else
        {
            cout << "Caravana Secreta esgotada" << endl;
        }
    }

}

int City::getX() const
{
    return x;
}

int City::getY() const
{
    return y;
}

void City::setY(int y)
{
    this->y = y;
}

void City::setX(int x)
{
    this->x = x;
}


void City::showCityInfo() const
{
    std::cout << "City " << name << " info:\n";
    std::cout << "Position: (" << x << ", " << y << ")\n";
    std::cout << "Buy price: " << buyPrice << "\n";
    std::cout << "Sell price: " << sellPrice << "\n";
    std::cout << "Buy crew price: " << buyCrewPrice << "\n";
    showParkedCaravans();
    showForSaleCaravans();
}

char City::getGridValue() const
{
    return gridvalue;
}








