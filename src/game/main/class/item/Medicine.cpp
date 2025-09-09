#include "Medicine.h"
#include<string>
#include<iostream>

using namespace std;
// Fridge to store food (max 7 per type)

class Player {
public:
    Stats stats;
    // Drugstore items are not stored, used immediately
    void buy_and_use_medicine(Medicine* med) {
        if (stats.money >= med->price) {
            med->use(stats);
            stats.money -= med->price;
            cout << "Used " << med->name << endl;
        } else {
            cout << "Not enough money." << endl;
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

Player p;
    p.show_stats();

    
    // Simulate buying and using drugs
    p.buy_and_use_medicine(new RecoveryMedication());
    p.buy_and_use_medicine(new StrengthEnhancedMedicine());
    p.buy_and_use_medicine(new SkillPointBuffMedicine());

    p.show_stats();

    return 0;
