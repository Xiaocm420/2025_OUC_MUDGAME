#include "Food.h"

// Food
Food::Food(const string& name, int price) : name_(name), price_(price) {}
Food::~Food() = default;
const string& Food::getName() const { return name_; }
int Food::getPrice() const { return price_; }

// Meat
Meat::Meat() : Food("Meat", 20) {}
void Meat::use(Player::Stats& stats) {
    stats.blood_volume += 10;
    stats.satiety_level += 40;
}

// SparklingWater
SparklingWater::SparklingWater() : Food("Sparkling Water", 6) {}
void SparklingWater::use(Player::Stats& stats) {
    stats.satiety_level += 9;
}

// EnergyDrink
EnergyDrink::EnergyDrink() : Food("Energy Drink", 14) {}
void EnergyDrink::use(Player::Stats& stats) {
    stats.satiety_level += 5;
    stats.stamina += 10;
}

// FrozenPizza
FrozenPizza::FrozenPizza() : Food("Frozen Pizza", 9) {}
void FrozenPizza::use(Player::Stats& stats) {
    stats.blood_volume += 10;
    stats.satiety_level += 17;
}

// ChocolateBar
ChocolateBar::ChocolateBar() : Food("Chocolate Bar", 12) {}
void ChocolateBar::use(Player::Stats& stats) {
    stats.satiety_level += 5;
    stats.stamina += 5;
}

// ProteinBar
ProteinBar::ProteinBar() : Food("Protein Bar", 18) {}
void ProteinBar::use(Player::Stats& stats) {
    stats.satiety_level += 15;
    }
    bool add(string name) {
        if (storage[name] < MAX_PER_TYPE) {
            storage[name]++;
            return true;
        }
        return false;
    }
    bool use(string name, Stats& stats) {
        if (storage[name] > 0) {
            food_types[name]->use(stats);
            storage[name]--;
            return true;
        }
        return false;
    }
    void print() {
        cout << "Fridge contents:\n";
        for (auto& p : storage) {
            cout << p.first << ": " << p.second << endl;
        }
    }
    ~Fridge() {
        for (auto& p : food_types) delete p.second;
    }
};

class Player {
public:
    Stats stats;
    Fridge fridge;
    // Drugstore items are not stored, used immediately
    void buy_and_store_food(Food* food) {
        if (stats.money >= food->price) {
            if (fridge.add(food->name)) {
                stats.money -= food->price;
                cout << "Bought and stored " << food->name << endl;
            } else {
                cout << "Fridge full for " << food->name << endl;
            }
        } else {
            cout << "Not enough money." << endl;
        }
    }
    void use_food(string name) {
        if (!fridge.use(name, stats)) {
            cout << "No " << name << " in fridge." << endl;
        } else {
            cout << "Used " << name << endl;
        }
    }
    void show_stats() {
        cout << "Stats: BV=" << stats.blood_volume
             << " SL=" << stats.satiety_level
             << " ST=" << stats.stamina
             << " STR=" << stats.strength
             << " AGI=" << stats.agility
             << " SP=" << stats.skill_point
             << " Money=" << stats.money << endl;
    }
};

