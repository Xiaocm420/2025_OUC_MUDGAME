#include "Player.h"

Player::Player(Game& game_logic) : game_logic_(game_logic), name("NOT_SET"), healthiness(100), strength(1),
                   stamina(1), agility(1), hunger(100), fatigue(100), money(0) {
}

// TODO: 保存数据
Player::~Player() = default;

const std::string &Player::getName() {
    return name;
}

void Player::setName(const std::string& name) {
    this->name = name;
}

double Player::getHealthiness() const {
    return healthiness;
}

double Player::getStrength() const {
    return strength;
}

double Player::getStamina() const {
    return stamina;
}

double Player::getAgility() const {
    return agility;
}

double Player::getHunger() const {
    return hunger;
}

double Player::getFatigue() const {
    return fatigue;
}

double Player::getSavings() const {
    return money;
}

double Player::getSpeed() const {
    return 1.1 * agility;
}

double Player::getHealth() const {
    return 100 + 15 * (stamina - 1);
}

double Player::getEnergy() const {
    return 100 + 5 * (stamina - 1);
}

void Player::addHealthiness(const double value) {
    healthiness += value;
}

void Player::addStrength(const double value) {
    strength += value;
}

void Player::addStamina(const double value) {
    stamina += value;
}

void Player::addAgility(const double value) {
    agility += value;
}

void Player::addHunger(const double value) {
    hunger += value;
}

void Player::addFatigue(const double value) {
    fatigue += value;
}

void Player::addSavings(const double value) {
    money += value;
}

