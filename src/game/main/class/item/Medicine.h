#include "Medicine.h"


// 构造函数实现
Medicine::Medicine(MedicineType type, 
                   std::string name, 
                   double price, 
                   int attribute_boost, 
                   int skill_points,
                   bool permanent)
    : type(type),
      name(std::move(name)),
      price(price),
      attribute_boost(attribute_boost),
      skill_points(skill_points),
      is_permanent(permanent) {}

// 获取药物名称
const std::string& Medicine::getName() const {
    return name;
}

// 获取药物类型
MedicineType Medicine::getType() const {
    return type;
}

// 获取药物价格
double Medicine::getPrice() const {
    return price;
}

// 使用药物逻辑实现
void Medicine::useOnPlayer(Player& player) const {
    switch (type) {
        case MedicineType::WOUND_RECOVERY:
            // 战败受伤直接恢复伤病状态
            player.recoverAllWounds();      // 恢复所有伤病
            player.setHealthToMax();        // 生命值回满
            player.clearNegativeStatus();   // 清除负面状态
            break;

        case MedicineType::STRENGTH_BOOST:
            // 提升力量属性
            player.increaseStrength(attribute_boost);
            
            // 副作用：能力下降速度提升一倍
            player.setDecayRateMultiplier(2.0);
            break;

        case MedicineType::ENDURANCE_BOOST:
            // 提升耐力属性
            player.increaseEndurance(attribute_boost);
            
            // 副作用：能力下降速度提升一倍
            player.setDecayRateMultiplier(2.0);
            break;

        case MedicineType::AGILITY_BOOST:
            // 提升敏捷属性
            player.increaseAgility(attribute_boost);
            
            // 副作用：能力下降速度提升一倍
            player.setDecayRateMultiplier(2.0);
            break;

        case MedicineType::SKILL_POINT:
            // 增加技能点
            player.addSkillPoints(skill_points);
            break;
    }
}

// 药物创建方法
Medicine Medicine::createWoundRecovery() {
    return Medicine(
        MedicineType::WOUND_RECOVERY,
        "创伤愈合剂",
        50.0  // 价格50
    );
}

Medicine Medicine::createStrengthBoost() {
    return Medicine(
        MedicineType::STRENGTH_BOOST,
        "力量强化剂",
        200.0,  // 单价200
        1,      // 力量+1
        0,
        true    // 永久buff
    );
}

Medicine Medicine::createEnduranceBoost() {
    return Medicine(
        MedicineType::ENDURANCE_BOOST,
        "耐力强化剂",
        200.0,  // 单价200
        1,      // 耐力+1
        0,
        true    // 永久buff
    );
}

Medicine Medicine::createAgilityBoost() {
    return Medicine(
        MedicineType::AGILITY_BOOST,
        "敏捷强化剂",
        200.0,  // 单价200
        1,      // 敏捷+1
        0,
        true    // 永久buff
    );
}

Medicine Medicine::createSkillPointPotion() {
    return Medicine(
        MedicineType::SKILL_POINT,
        "技能点药剂",
        100.0,  // 1个技能点100
        0,
        1       // 增加1个技能点
    )
}