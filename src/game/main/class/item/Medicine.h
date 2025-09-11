#ifndef MEDICINE_H
#define MEDICINE_H

#include "Player.h"
#include <string>

// 药物类型枚举
enum class MedicineType {
    WOUND_RECOVERY,    // 创伤恢复药
    STRENGTH_BOOST,    // 力量提升药
    ENDURANCE_BOOST,   // 耐力提升药
    AGILITY_BOOST,     // 敏捷提升药
    SKILL_POINT        // 技能点药
};

class Medicine {
private:
    MedicineType type;
    std::string name;
    double price;
    int attribute_boost;  // 属性提升值
    int skill_points;     // 技能点数
    bool is_permanent;    // 是否为永久buff

public:
    // 构造函数
    Medicine(MedicineType type, 
             std::string name, 
             double price, 
             int attribute_boost = 0, 
             int skill_points = 0,
             bool permanent = false);

    // 获取药物信息
    const std::string& getName() const;
    MedicineType getType() const;
    double getPrice() const;

    // 使用药物
    void useOnPlayer(Player& player) const;

    // 药物创建工厂方法
    static Medicine createWoundRecovery();
    static Medicine createStrengthBoost();
    static Medicine createEnduranceBoost();
    static Medicine createAgilityBoost();
    static Medicine createSkillPointPotion();
};

#endif // MEDICINE_H
