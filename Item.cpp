//
// Created by frede on 30/11/2024.
//

#include "Item.h"

#include "Item.h"

#include <algorithm>
#include <ctime>
#include <cmath>
#include <limits>
#include <cmath>
#include <random>
//#include <bits/random.h>
#include <random>
#include "Caravan.h"
#include "Map.h"
#include <iostream>

void PandoraBox::applyEffect(Caravan& caravan) {
    int tripulationLoss = caravan.getTripulation() * 0.2;
    caravan.reduceTripulation(tripulationLoss);
    std::cout << "Caravana " << caravan.getId() << " encontrou a Caixa de Pandora. Perdeu " << tripulationLoss << " tripulantes." << std::endl;
}

string PandoraBox::getAsString() const
{
    return "Caixa de Pandora";
}

void TreasureChest::applyEffect(Caravan& caravan) {
    int goldIncrease = caravan.getCoins() * 0.1;
    caravan.addCoins(goldIncrease);
    std::cout << "Caravana " << caravan.getId() << " encontrou o Baú do Tesouro. Ganhou " << goldIncrease << " moedas." << std::endl;
}

string TreasureChest::getAsString() const
{
    return "Caixa do Tesouro";
}

void Cage::applyEffect(Caravan& caravan) {
    int newTripulation = 10;
    caravan.addTripulation(newTripulation);
    std::cout << "Caravana " << caravan.getId() << " encontrou uma Jaula. Ganhou " << newTripulation << " tripulantes." << std::endl;
}

string Cage::getAsString() const
{
    return "Jaula";
}

void Mine::applyEffect(Caravan& caravan) {
    std::cout << "Caravana " << caravan.getId() << " encontrou uma mina. A caravana foi destruida." << std::endl;
    caravan.setTripulation(0);
    map->removeCaravan(caravan.getId());
}

string Mine::getAsString() const
{
    return "Mina";
}

void Surprise::applyEffect(Caravan& caravan) {
        std::cout << "Caravana " << caravan.getId() << " encontrou uma surpresa. ";

        auto& caravans = map->getCaravans();
        int numCaravans = caravans.size();


        if (numCaravans > 5) {
            std::cout << "Metade das caravanas foram removidas.\n";

            int removeCount = numCaravans / 2;
            for (int i = 0; i < removeCount; i++)
            {
                if (!caravans.empty())
                {
                    map->removeCaravan(caravans[0]->getId());
                }
            }

        } else {
            int tripulationIncrease = 5;
            caravan.addTripulation(tripulationIncrease);
            std::cout << "Ganhou 5 tripulantes. \n";
        }
}

string Surprise::getAsString() const
{
    return "Supresa";
}

PandoraBox::~PandoraBox() {
    std::cout << "Caixa de Pandora destruída." << std::endl;
}

TreasureChest::~TreasureChest() {
    std::cout << "Baú do Tesouro destruído." << std::endl;
}

Cage::~Cage() {
    std::cout << "Jaula destruída." << std::endl;
}

Mine::~Mine() {
    std::cout << "Mina destruída." << std::endl;
}

Surprise::~Surprise() {
    std::cout << "Surpresa destruída." << std::endl;
}
