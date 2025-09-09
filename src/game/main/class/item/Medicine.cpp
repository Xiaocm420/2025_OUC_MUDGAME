
#include "Medicine.h"

// Medicine
Medicine::Medicine(const string& name, int price) : name_(name), price_(price) {}
Medicine::~Medicine() = default;
const string& Medicine::getName() const { return name_; }
int Medicine::getPrice() const { return price_; }

// RecoveryMedication
RecoveryMedication::RecoveryMedication() : Medicine("Recovery Medication", 50) {}
void RecoveryMedication::use(Player::Stats& stats) {
    stats.blood_volume = 100;
}

// StrengthEnhancedMedicine
StrengthEnhancedMedicine::StrengthEnhancedMedicine() : Medicine("Strength Enhanced Medicine", 200) {}
void StrengthEnhancedMedicine::use(Player::Stats& stats) {
    stats.strength += 1;
}

// StaminaEnhancedMedicine
StaminaEnhancedMedicine::StaminaEnhancedMedicine() : Medicine("Stamina Enhanced Medicine", 200) {}
void StaminaEnhancedMedicine::use(Player::Stats& stats) {
    stats.stamina += 1;
}

// AgilityEnhancedMedicine
AgilityEnhancedMedicine::AgilityEnhancedMedicine() : Medicine("Agility Enhanced Medicine", 200) {}
void AgilityEnhancedMedicine::use(Player::Stats& stats) {
    stats.agility += 1;
}

// SkillPointBuffMedicine
SkillPointBuffMedicine::SkillPointBuffMedicine() : Medicine("Skill Point Buff Medicine", 100) {}
void SkillPointBuffMedicine::use(Player::Stats& stats) {
    stats.skill_point += 1;
}
