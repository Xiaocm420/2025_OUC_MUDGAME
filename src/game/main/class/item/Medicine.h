#ifndef MEDICINE_H
#define MEDICINE_H
#include<map>
#include<string>
// Stats for a player
struct Stats {
    int blood_volume ;
    int satiety_level ;
    int stamina ;
    int strength ;
    int agility ;
    int skill_point ;
    int money ;
};

// Base class
class Item {
public:
    string name;
    int price;
    Item(string n, int p) : name(n), price(p) {}
    virtual void use(Stats& stats) = 0;
    virtual ~Item() {}
};

// Medicine base
class Medicine : public Item {
public:
    Medicine(string n, int p) : Item(n, p) {}
    virtual void use(Stats& stats) override {}
};

// DrugStore Medicines
class RecoveryMedication : public Medicine {
public:
    RecoveryMedication() : Medicine("Recovery Medication", 50) {}
    void use(Stats& stats) override {
        stats.blood_volume = 100;
    }
};
class StrengthEnhancedMedicine : public Medicine {
public:
    StrengthEnhancedMedicine() : Medicine("Strength Enhanced Medicine", 200) {}
    void use(Stats& stats) override {
        stats.strength += 1;
        // Ability decrease speed logic in main or player class
    }
};
class StaminaEnhancedMedicine : public Medicine {
public:
    StaminaEnhancedMedicine() : Medicine("Stamina Enhanced Medicine", 200) {}
    void use(Stats& stats) override {
        stats.stamina += 1;
    }
};
class AgilityEnhancedMedicine : public Medicine {
public:
    AgilityEnhancedMedicine() : Medicine("Agility Enhanced Medicine", 200) {}
    void use(Stats& stats) override {
        stats.agility += 1;
    }
};
class SkillPointBuffMedicine : public Medicine {
public:
    SkillPointBuffMedicine() : Medicine("Skill Point Buff Medicine", 100) {}
    void use(Stats& stats) override {
        stats.skill_point += 1;
    }
};

#endif // MEDICINE_H
