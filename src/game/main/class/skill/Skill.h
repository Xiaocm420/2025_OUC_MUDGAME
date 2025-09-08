#ifndef SKILL_H
#define SKILL_H

#include <string>
#include <functional>

class Player;

// 技能效果类型枚举
enum class SkillEffectType {
    NONE,                   // 无效果
    BASIC_ATTACK,           // 基础攻击
    ATTRIBUTE_BOOST,        // 属性增益
    TRAINING_EFFICIENCY,    // 训练效率
    DAMAGE_REFLECT,         // 伤害反射
    DAMAGE_REDUCTION,       // 伤害减免
    HIT_RATE_MODIFIER,      // 命中率修改
    STAMINA_MODIFIER,       // 体力消耗修改
    ATTACK_TYPE_DEFENSE,    // 攻击类型防御
    ENEMY_ATTRIBUTE_MOD,    // 敌人属性修改
    GLOBAL_HIT_RATE_BOOST   // 全局命中率提升
};

class Skill {
private:
    // 基础属性
    std::string skillName;          // 技能名称
    std::string description;        // 技能描述
    SkillEffectType effectType;     // 技能效果类型
    bool isLocked;                  // 是否锁定（未解锁则无法使用）
    int unlockCost;                 // 解锁所需技能点数
    
    // 攻击相关属性（对非攻击技能这些值为0）
    double baseDamage;              // 基础伤害值
    double damageCoefficient;       // 伤害系数
    double baseHitRate;             // 基础命中率
    double hitRateCoefficient;      // 命中率系数
    double baseStaminaCost;         // 基础体力消耗
    double staminaCostCoefficient;  // 体力消耗系数
    
    // 属性增益相关（对非属性增益技能这些值为0）
    double minAttributeValue;       // 最低属性值
    std::string attributeType;      // 属性类型："agility"-敏捷, "stamina"-耐力, "strength"-力量
    
    // 训练效率相关（对非训练技能这些值为0）
    double trainingMultiplier;      // 训练效率倍率
    
    // 特殊效果相关
    double effectValue;             // 效果数值
    std::function<void(Player&, Player&)> customEffect;  // 自定义效果函数

public:
    // 构造函数 - 基础攻击技能
    Skill(const std::string& name, const std::string& desc, 
          int baseDmg, float dmgCoeff, 
          int baseHit, float hitCoeff, 
          int baseStamina, float staminaCoeff);
    
    // 构造函数 - 属性增益技能
    Skill(const std::string& name, const std::string& desc,
          int minValue, const std::string& attrType);
    
    // 构造函数 - 训练效率技能  
    Skill(const std::string& name, const std::string& desc,
          float multiplier, const std::string& attrType);
    
    // 构造函数 - 特殊效果技能
    Skill(const std::string& name, const std::string& desc,
          SkillEffectType type, float value);
    
    // 构造函数 - 自定义效果技能
    Skill(const std::string& name, const std::string& desc,
          std::function<void(Player&, Player&)> effect);

    // 获取技能基本信息
    std::string getSkillName() const;           // 获取技能名称
    std::string getDescription() const;         // 获取技能描述
    SkillEffectType getEffectType() const;      // 获取技能效果类型
    
    // 攻击相关属性获取
    int getBaseDamage() const;                  // 获取基础伤害值
    float getDamageCoefficient() const;         // 获取伤害系数
    int getBaseHitRate() const;                 // 获取基础命中率
    float getHitRateCoefficient() const;        // 获取命中率系数
    int getBaseStaminaCost() const;             // 获取基础体力消耗
    float getStaminaCostCoefficient() const;    // 获取体力消耗系数
    

    // 获取最低属性值有问题，应该设置为属性最低值为多少
    int setMinAttributeValue(Player& user);     // 设置玩家最低属性值( 初始三个属性都默认为0 )
    std::string getAttributeType() const;       // 获取要设置的最低值的属性类型（力量，敏捷，耐力）
    
    // 训练效率相关获取
    float getTrainingMultiplier() const;        // 获取训练效率倍率
    
    // 特殊效果相关获取
    float getEffectValue() const;               // 获取效果数值

    // 技能效果计算方法
    int calculateDamage(int playerStrength) const;                          // 计算伤害值
    float calculateHitRate(int playerAgility, int playerStrength, int playerStamina) const;  // 计算命中率
    int calculateStaminaCost(int playerStrength) const;                     // 计算体力消耗
    
    // 使用条件检查
    bool canUse(const bool isLocked) const;                                  // 检查是否可以使用技能
    
    // 执行技能
    void execute(Player& user, Player& target);                             // 执行技能效果
    
    // 技能类型判断（ 简单分为四种技能 ）
    bool isAttackSkill() const;              // 是否是攻击技能
    bool isAttributeBoostSkill() const;      // 是否是属性增益技能
    bool isTrainingSkill() const;            // 是否是训练效率技能
    bool isSpecialEffectSkill() const;       // 是否是特殊效果技能

    // 序列化功能（用于存档）
    std::string serialize() const;           // 序列化为字符串
    void deserialize(const std::string& data); // 从字符串反序列化
};

#endif // SKILL_H