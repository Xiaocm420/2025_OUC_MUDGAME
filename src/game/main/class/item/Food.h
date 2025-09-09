#ifndef FOOD_H
#define FOOD_H
#include<iostream>
#include<string>
#include<map>
class Player;

struct Stats {
    int blood_volume = 100;
    int satiety_level = 100;
    int stamina = 100;
    int strength = 10;
    int agility = 10;
    int skill_point = 0;
    int money = 1000;
};

class Item {
public:
    string name;
    int price;
    Item(string n, int p) : name(n), price(p) {}
    virtual void use(Stats& stats) = 0;
    virtual ~Item() {}
};

// Food base
class Food : public Item {
public:
    Food(string n, int p) : Item(n, p) {}
    virtual void use(Stats& stats) override {}
};
// Store Foods
class Meat : public Food {
public:
    Meat() : Food("Meat", 20) {}
    void use(Stats& stats) override {
        stats.blood_volume += 10;
        stats.satiety_level += 40;
    }
};
class SparklingWater : public Food {
public:
    SparklingWater() : Food("Sparkling Water", 6) {}
    void use(Stats& stats) override {
        stats.satiety_level += 9;
    }
};
class EnergyDrink : public Food {
public:
    EnergyDrink() : Food("Energy Drink", 14) {}
    void use(Stats& stats) override {
        stats.satiety_level += 5;
        stats.stamina += 10;
    }
};
class FrozenPizza : public Food {
public:
    FrozenPizza() : Food("Frozen Pizza", 9) {}
    void use(Stats& stats) override {
        stats.blood_volume += 10;
        stats.satiety_level += 17;
    }
};

// Fitness Center Foods
class ChocolateBar : public Food {
public:
    ChocolateBar() : Food("Chocolate Bar", 12) {}
    void use(Stats& stats) override {
        stats.satiety_level += 5;
        stats.stamina += 5;
    }
};
class ProteinBar : public Food {
public:
    ProteinBar() : Food("Protein Bar", 18) {}
    void use(Stats& stats) override {
        stats.satiety_level += 15;
    }
};


#endif // MUDGAME_FOOD_H
