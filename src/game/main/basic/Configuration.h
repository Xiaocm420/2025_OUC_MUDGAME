#ifndef INC_2025_OUC_MUDGAME_CONFIGURATION_H
#define INC_2025_OUC_MUDGAME_CONFIGURATION_H

#include <toml.hpp>
#include <string>
#include <filesystem>

class Configuration {
public:
    static Configuration* getInstance();

    bool loadConfig(const std::string& filePath);
    void saveConfig() const;
    [[nodiscard]] std::string getConfigFilePath() const;

    // Game basic configuration
    [[nodiscard]] std::string getGameName() const;
    [[nodiscard]] std::string getGameVersion() const;
    void setGameName(const std::string& name);
    void setGameVersion(const std::string& version);

    // Player initial attributes configuration
    [[nodiscard]] int getPlayerInitialHealth() const;
    [[nodiscard]] int getPlayerInitialStamina() const;
    [[nodiscard]] int getPlayerInitialStrength() const;
    [[nodiscard]] int getPlayerInitialSpeed() const;
    [[nodiscard]] int getPlayerInitialDefense() const;
    void setPlayerInitialHealth(int health);
    void setPlayerInitialStamina(int stamina);
    void setPlayerInitialStrength(int strength);
    void setPlayerInitialSpeed(int speed);
    void setPlayerInitialDefense(int defense);

    // Game difficulty settings
    [[nodiscard]] int getDifficultyLevel() const;
    [[nodiscard]] double getEnemyDamageMultiplier() const;
    [[nodiscard]] double getEnemyHealthMultiplier() const;
    void setDifficultyLevel(int level);
    void setEnemyDamageMultiplier(double multiplier);
    void setEnemyHealthMultiplier(double multiplier);

    // Combat system settings
    [[nodiscard]] double getCriticalHitChance() const;
    [[nodiscard]] double getCriticalHitMultiplier() const;
    [[nodiscard]] int getStaminaRegenerationRate() const;
    void setCriticalHitChance(double chance);
    void setCriticalHitMultiplier(double multiplier);
    void setStaminaRegenerationRate(int rate);

    // Training system settings
    [[nodiscard]] double getBasicTrainingEffect() const;
    [[nodiscard]] double getAdvancedTrainingEffect() const;
    [[nodiscard]] int getTrainingCost() const;
    [[nodiscard]] int getRecoveryTime() const;
    void setBasicTrainingEffect(double effect);
    void setAdvancedTrainingEffect(double effect);
    void setTrainingCost(int cost);
    void setRecoveryTime(int time);

    // Match settings
    [[nodiscard]] int getBasicReward() const;
    [[nodiscard]] int getWinBonus() const;
    [[nodiscard]] int getTitleMatchReward() const;
    [[nodiscard]] int getChallengeCooldown() const;
    void setBasicReward(int reward);
    void setWinBonus(int bonus);
    void setTitleMatchReward(int reward);
    void setChallengeCooldown(int cooldown);

    // Shop settings
    [[nodiscard]] int getHealthPotionCost() const;
    [[nodiscard]] int getHealthPotionEffect() const;
    [[nodiscard]] int getStaminaPotionCost() const;
    [[nodiscard]] int getStaminaPotionEffect() const;
    [[nodiscard]] int getEquipmentUpgradeCost() const;
    void setHealthPotionCost(int cost);
    void setHealthPotionEffect(int effect);
    void setStaminaPotionCost(int cost);
    void setStaminaPotionEffect(int effect);
    void setEquipmentUpgradeCost(int cost);

    void initializeDefaultConfig();

    ~Configuration();

private:
    static Configuration* instance;
    toml::value configData;
    std::filesystem::path configFilePath;

    Configuration();
};

#endif //INC_2025_OUC_MUDGAME_CONFIGURATION_H