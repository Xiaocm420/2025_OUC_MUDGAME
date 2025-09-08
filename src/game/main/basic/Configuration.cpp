#include "Configuration.h"
#include <iostream>
#include <fstream>

#define GAMENAME "拳王之路"
#define VERSION "1.0.0"

Configuration* Configuration::instance = nullptr;

Configuration::Configuration() : configFilePath("config.toml") {
    if (!loadConfig(configFilePath.string())) {
        std::cout << "无法加载配置文件，将使用默认配置。" << std::endl;
        initializeDefaultConfig();
        saveConfig();
    }
}

Configuration* Configuration::getInstance() {
    if (instance == nullptr) {
        instance = new Configuration();
    }
    return instance;
}

bool Configuration::loadConfig(const std::string& filePath) {
    try {
        configFilePath = filePath;
        configData = toml::parse(filePath);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "加载配置文件时发生错误: " << e.what() << std::endl;
        return false;
    }
}

void Configuration::saveConfig() const {
    try {
        if (configFilePath.has_parent_path() && !std::filesystem::exists(configFilePath.parent_path())) {
            std::filesystem::create_directories(configFilePath.parent_path());
        }

        std::ofstream file(configFilePath.string());
        if (!file.is_open()) {
            std::cerr << "无法打开配置文件进行写入: " << configFilePath.string() << std::endl;
            return ;
        }

        file << toml::format(configData);
    } catch (const std::exception& e) {
        std::cerr << "保存配置文件时发生错误: " << e.what() << std::endl;
    }
}

std::string Configuration::getConfigFilePath() const {
    return configFilePath.string();
}

// Game basic configuration
std::string Configuration::getGameName() const {
    return toml::find_or<std::string>(configData, "game", "name", "拳击选手模拟器");
}

std::string Configuration::getGameVersion() const {
    return toml::find_or<std::string>(configData, "game", "version", "1.0.0");
}

void Configuration::setGameName(const std::string& name) {
    configData["game"]["name"] = name;
}

void Configuration::setGameVersion(const std::string& version) {
    configData["game"]["version"] = version;
}

// Player initial attributes configuration
int Configuration::getPlayerInitialHealth() const {
    return toml::find_or<int>(configData, "player", "initial_health", 100);
}

int Configuration::getPlayerInitialStamina() const {
    return toml::find_or<int>(configData, "player", "initial_stamina", 50);
}

int Configuration::getPlayerInitialStrength() const {
    return toml::find_or<int>(configData, "player", "initial_strength", 10);
}

int Configuration::getPlayerInitialSpeed() const {
    return toml::find_or<int>(configData, "player", "initial_speed", 10);
}

int Configuration::getPlayerInitialDefense() const {
    return toml::find_or<int>(configData, "player", "initial_defense", 5);
}

void Configuration::setPlayerInitialHealth(int health) {
    configData["player"]["initial_health"] = health;
}

void Configuration::setPlayerInitialStamina(int stamina) {
    configData["player"]["initial_stamina"] = stamina;
}

void Configuration::setPlayerInitialStrength(int strength) {
    configData["player"]["initial_strength"] = strength;
}

void Configuration::setPlayerInitialSpeed(int speed) {
    configData["player"]["initial_speed"] = speed;
}

void Configuration::setPlayerInitialDefense(int defense) {
    configData["player"]["initial_defense"] = defense;
}

// Game difficulty settings
int Configuration::getDifficultyLevel() const {
    return toml::find_or<int>(configData, "difficulty", "level", 1);
}

double Configuration::getEnemyDamageMultiplier() const {
    return toml::find_or<double>(configData, "difficulty", "enemy_damage_multiplier", 1.0);
}

double Configuration::getEnemyHealthMultiplier() const {
    return toml::find_or<double>(configData, "difficulty", "enemy_health_multiplier", 1.0);
}

void Configuration::setDifficultyLevel(int level) {
    configData["difficulty"]["level"] = level;
    switch (level) {
        case 1: // Easy
            setEnemyDamageMultiplier(0.8);
            setEnemyHealthMultiplier(0.8);
            break;
        case 2: // Normal
            setEnemyDamageMultiplier(1.0);
            setEnemyHealthMultiplier(1.0);
            break;
        case 3: // Hard
            setEnemyDamageMultiplier(1.2);
            setEnemyHealthMultiplier(1.2);
            break;
        default: // Default to Normal
            setEnemyDamageMultiplier(1.0);
            setEnemyHealthMultiplier(1.0);
            break;
    }
}

void Configuration::setEnemyDamageMultiplier(double multiplier) {
    configData["difficulty"]["enemy_damage_multiplier"] = multiplier;
}

void Configuration::setEnemyHealthMultiplier(double multiplier) {
    configData["difficulty"]["enemy_health_multiplier"] = multiplier;
}


// Combat system settings
double Configuration::getCriticalHitChance() const {
    return toml::find_or<double>(configData, "combat", "critical_hit_chance", 0.1);
}

double Configuration::getCriticalHitMultiplier() const {
    return toml::find_or<double>(configData, "combat", "critical_hit_multiplier", 1.5);
}

int Configuration::getStaminaRegenerationRate() const {
    return toml::find_or<int>(configData, "combat", "stamina_regeneration_rate", 2);
}

void Configuration::setCriticalHitChance(double chance) {
    configData["combat"]["critical_hit_chance"] = chance;
}

void Configuration::setCriticalHitMultiplier(double multiplier) {
    configData["combat"]["critical_hit_multiplier"] = multiplier;
}

void Configuration::setStaminaRegenerationRate(int rate) {
    configData["combat"]["stamina_regeneration_rate"] = rate;
}

// Training system settings
double Configuration::getBasicTrainingEffect() const {
    return toml::find_or<double>(configData, "training", "basic_training_effect", 1.2);
}

double Configuration::getAdvancedTrainingEffect() const {
    return toml::find_or<double>(configData, "training", "advanced_training_effect", 1.5);
}

int Configuration::getTrainingCost() const {
    return toml::find_or<int>(configData, "training", "training_cost", 10);
}

int Configuration::getRecoveryTime() const {
    return toml::find_or<int>(configData, "training", "recovery_time", 3);
}

void Configuration::setBasicTrainingEffect(double effect) {
    configData["training"]["basic_training_effect"] = effect;
}

void Configuration::setAdvancedTrainingEffect(double effect) {
    configData["training"]["advanced_training_effect"] = effect;
}

void Configuration::setTrainingCost(int cost) {
    configData["training"]["training_cost"] = cost;
}

void Configuration::setRecoveryTime(int time) {
    configData["training"]["recovery_time"] = time;
}

// Match settings
int Configuration::getBasicReward() const {
    return toml::find_or<int>(configData, "match", "basic_reward", 50);
}

int Configuration::getWinBonus() const {
    return toml::find_or<int>(configData, "match", "win_bonus", 100);
}

int Configuration::getTitleMatchReward() const {
    return toml::find_or<int>(configData, "match", "title_match_reward", 500);
}

int Configuration::getChallengeCooldown() const {
    return toml::find_or<int>(configData, "match", "challenge_cooldown", 10);
}

void Configuration::setBasicReward(int reward) {
    configData["match"]["basic_reward"] = reward;
}

void Configuration::setWinBonus(int bonus) {
    configData["match"]["win_bonus"] = bonus;
}

void Configuration::setTitleMatchReward(int reward) {
    configData["match"]["title_match_reward"] = reward;
}

void Configuration::setChallengeCooldown(int cooldown) {
    configData["match"]["challenge_cooldown"] = cooldown;
}

// Shop settings
int Configuration::getHealthPotionCost() const {
    return toml::find_or<int>(configData, "shop", "health_potion_cost", 20);
}

int Configuration::getHealthPotionEffect() const {
    return toml::find_or<int>(configData, "shop", "health_potion_effect", 30);
}

int Configuration::getStaminaPotionCost() const {
    return toml::find_or<int>(configData, "shop", "stamina_potion_cost", 15);
}

int Configuration::getStaminaPotionEffect() const {
    return toml::find_or<int>(configData, "shop", "stamina_potion_effect", 20);
}

int Configuration::getEquipmentUpgradeCost() const {
    return toml::find_or<int>(configData, "shop", "equipment_upgrade_cost", 100);
}

void Configuration::setHealthPotionCost(int cost) {
    configData["shop"]["health_potion_cost"] = cost;
}

void Configuration::setHealthPotionEffect(int effect) {
    configData["shop"]["health_potion_effect"] = effect;
}

void Configuration::setStaminaPotionCost(int cost) {
    configData["shop"]["stamina_potion_cost"] = cost;
}

void Configuration::setStaminaPotionEffect(int effect) {
    configData["shop"]["stamina_potion_effect"] = effect;
}

void Configuration::setEquipmentUpgradeCost(int cost) {
    configData["shop"]["equipment_upgrade_cost"] = cost;
}

void Configuration::initializeDefaultConfig() {
    // Game basic configuration
    setGameName(GAMENAME);
    setGameVersion(VERSION);

    // Player initial attributes configuration
    setPlayerInitialHealth(100);
    setPlayerInitialStamina(50);
    setPlayerInitialStrength(10);
    setPlayerInitialSpeed(10);
    setPlayerInitialDefense(5);

    // Game difficulty settings
    setDifficultyLevel(2);

    // Combat system settings
    setCriticalHitChance(0.1);
    setCriticalHitMultiplier(1.5);
    setStaminaRegenerationRate(2);

    // Training system settings
    setBasicTrainingEffect(1.2);
    setAdvancedTrainingEffect(1.5);
    setTrainingCost(10);
    setRecoveryTime(3);

    // Match settings
    setBasicReward(50);
    setWinBonus(100);
    setTitleMatchReward(500);
    setChallengeCooldown(10);

    // Shop settings
    setHealthPotionCost(20);
    setHealthPotionEffect(30);
    setStaminaPotionCost(15);
    setStaminaPotionEffect(20);
    setEquipmentUpgradeCost(100);
}

Configuration::~Configuration() {
    saveConfig();
}