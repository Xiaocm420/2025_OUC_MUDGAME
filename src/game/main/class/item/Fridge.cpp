#include "Fridge.h"
#include <iostream>

Fridge::Fridge() {
    food_types_["Meat"] = new Meat();
    food_types_["Sparkling Water"] = new SparklingWater();
    food_types_["Energy Drink"] = new EnergyDrink();
    food_types_["Frozen Pizza"] = new FrozenPizza();
    food_types_["Chocolate Bar"] = new ChocolateBar();
    food_types_["Protein Bar"] = new ProteinBar();
}

Fridge::~Fridge() {
    for (auto& p : food_types_) delete p.second;
}

bool Fridge::add(const std::string& name) {
    if (storage_[name] < MAX_PER_TYPE) {
        storage_[name]++;
        return true;
    }
    return false;
}

bool Fridge::use(const std::string& name, Player::Stats& stats) {
    if (storage_[name] > 0) {
        food_types_[name]->use(stats);
        storage_[name]--;
        return true;
    }
    return false;
}

void Fridge::print() const {
    std::cout << "Fridge contents:\n";
    for (const auto& p : storage_) {
        std::cout << p.first << ": " << p.second << std::endl;
    }
}
