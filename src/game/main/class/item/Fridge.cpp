#include "Fridge.h"
#include <iostream>

#include "Meat.h"
#include "SparklingWater.h"
#include "EnergyDrink.h"
#include "FrozenPizza.h"
#include "ChocolateBar.h"
#include "ProteinBar.h"

// 构造函数：初始化每种食物的实例
Fridge::Fridge() {
    food_types_["Meat"] = new Meat();
    food_types_["Sparkling Water"] = new SparklingWater();
    food_types_["Energy Drink"] = new EnergyDrink();
    food_types_["Frozen Pizza"] = new FrozenPizza();
    food_types_["Chocolate Bar"] = new ChocolateBar();
    food_types_["Protein Bar"] = new ProteinBar();
}

Fridge::~Fridge() {
    for (auto& p : food_types_) {
        delete p.second; // 释放单个食物实例
    }
}
bool Fridge::add(const std::string& name) {
    if (food_types_.find(name) == food_types_.end()) {
        std::cout << "Error: " << name << " is not a supported food type!\n";
        return false;
    }
    if (storage_[name] < MAX_PER_TYPE) {
        storage_[name]++;
        std::cout << "Successfully added: " << name << " (Current: " << storage_[name] << ")\n";
        return true;
    }
    std::cout << "Failed to add: " << name << " (Reached max storage: " << MAX_PER_TYPE << ")\n";
    return false;
}

bool Fridge::use(const std::string& name) {
    if (food_types_.find(name) == food_types_.end()) {
        std::cout << "Error: " << name << " is not in the fridge!\n";
        return false;
    }
    if (storage_[name] <= 0) {
        std::cout << "Error: No " << name << " left in the fridge!\n";
        return false;
    }

    food_types_[name]->use(); 

    storage_[name]--;
    std::cout << "Successfully used: " << name << " (Remaining: " << storage_[name] << ")\n";
    return true;
}

void Fridge::print() const {
    std::cout << "\n===== Fridge Contents =====" << std::endl;
    for (const auto& p : storage_) {
        if (p.second > 0) {
            std::cout << p.first << ": " << p.second << " piece(s)\n";
        }
    }
    if (storage_.empty() || all_zero(storage_)) {
        std::cout << "The fridge is empty!\n";
    }
    std::cout << "===========================\n" << std::endl;
}

bool Fridge::all_zero(const std::unordered_map<std::string, int>& map) const {
    for (const auto& p : map) {
        if (p.second > 0) return false;
    }
    return true;
}