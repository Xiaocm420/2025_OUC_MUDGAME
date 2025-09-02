#include "Skill.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// ========== 构造函数实现 ==========

// 基础攻击技能
Skill::Skill(const std::string& name, const std::string& desc,
             int baseDmg, float dmgCoeff,
             int baseHit, float hitCoeff,
             int baseStamina, float staminaCoeff, int skillPoints)
    : skillName(name), description(desc), effectType(SkillEffectType::BASIC_ATTACK),
      isLocked(true), baseDamage(baseDmg), damageCoefficient(dmgCoeff),
      baseHitRate(baseHit), hitRateCoefficient(hitCoeff),
      baseStaminaCost(baseStamina), staminaCostCoefficient(staminaCoeff),
      minAttributeValue(0), attributeType(""),
      trainingMultiplier(0.0f), effectValue(0.0f), customEffect(nullptr) , unlockCost(skillPoints){}

// 属性增益技能
Skill::Skill(const std::string& name, const std::string& desc,
             int minValue, const std::string& attrType, int skillPoints)
    : skillName(name), description(desc), effectType(SkillEffectType::ATTRIBUTE_BOOST),
      isLocked(true), baseDamage(0), damageCoefficient(0.0f),
      baseHitRate(0), hitRateCoefficient(0.0f),
      baseStaminaCost(0), staminaCostCoefficient(0.0f),
      minAttributeValue(minValue), attributeType(attrType),
      trainingMultiplier(0.0f), effectValue(0.0f), customEffect(nullptr), unlockCost(skillPoints) {}

// 训练效率技能
Skill::Skill(const std::string& name, const std::string& desc,
             float multiplier, const std::string& attrType, int skillPoints)
    : skillName(name), description(desc), effectType(SkillEffectType::TRAINING_EFFICIENCY),
      isLocked(true), baseDamage(0), damageCoefficient(0.0f),
      baseHitRate(0), hitRateCoefficient(0.0f),
      baseStaminaCost(0), staminaCostCoefficient(0.0f),
      minAttributeValue(0), attributeType(attrType),
      trainingMultiplier(multiplier), effectValue(0.0f), customEffect(nullptr) , unlockCost(skillPoints){}

// 特殊效果技能
Skill::Skill(const std::string& name, const std::string& desc,
             SkillEffectType type, float value, int skillPoints)
    : skillName(name), description(desc), effectType(type),
      isLocked(true), baseDamage(0), damageCoefficient(0.0f),
      baseHitRate(0), hitRateCoefficient(0.0f),
      baseStaminaCost(0), staminaCostCoefficient(0.0f),
      minAttributeValue(0), attributeType(""),
      trainingMultiplier(0.0f), effectValue(value), customEffect(nullptr) , unlockCost(skillPoints){}

// 自定义效果技能
Skill::Skill(const std::string& name, const std::string& desc,
             std::function<void(Player&, Player&)> effect, int skillPoints)
    : skillName(name), description(desc), effectType(SkillEffectType::NONE),
      isLocked(true), baseDamage(0), damageCoefficient(0.0f),
      baseHitRate(0), hitRateCoefficient(0.0f),
      baseStaminaCost(0), staminaCostCoefficient(0.0f),
      minAttributeValue(0), attributeType(""),
      trainingMultiplier(0.0f), effectValue(0.0f), customEffect(effect) , unlockCost(skillPoints){}


// ========== Getter 实现 ==========
std::string Skill::getSkillName() const { return skillName; }
std::string Skill::getDescription() const { return description; }
SkillEffectType Skill::getEffectType() const { return effectType; }

int Skill::getBaseDamage() const { return baseDamage; }
float Skill::getDamageCoefficient() const { return damageCoefficient; }
int Skill::getBaseHitRate() const { return baseHitRate; }
float Skill::getHitRateCoefficient() const { return hitRateCoefficient; }
int Skill::getBaseStaminaCost() const { return baseStaminaCost; }
float Skill::getStaminaCostCoefficient() const { return staminaCostCoefficient; }

void Skill::setMinAttributeValue(Player& user) {
    // 根据 attributeType 设置玩家最低属性
    // 这里注释是因为依赖 Player 的接口实现，需要 Player 有 setMinStrength(), getMinStrength() 等6个接口
    // 比如 Player 有 setStrength / setAgility / setStamina 方法
    if (attributeType == "strength") {
        // user.setMinStrength(std::max(user.getMinStrength(), minAttributeValue));
    } else if (attributeType == "agility") {
        // user.setMinAgility(std::max(user.getMinAgility(), minAttributeValue));
    } else if (attributeType == "stamina") {
        // user.setMinStamina(std::max(user.getMinStamina(), minAttributeValue));
    }
}

std::string Skill::getAttributeType() const { return attributeType; }
float Skill::getTrainingMultiplier() const { return trainingMultiplier; }
float Skill::getEffectValue() const { return effectValue; }


// ========== 计算公式实现 ==========
// 伤害 = 固定 + X * 系数 , 其中 X = 力量
int Skill::calculateDamage(int playerStrength) const {
    return baseDamage + static_cast<int>(playerStrength * damageCoefficient);
}

// 命中率 = 固定 + Y * 系数 , 其中 Y = (敏捷 / (力量 + 敏捷 + 耐力)) * 3
float Skill::calculateHitRate(int playerAgility, int playerStrength, int playerStamina) const {
    int total = playerStrength + playerAgility + playerStamina;
    float Y = (total > 0) ? (static_cast<float>(playerAgility) / total) * 3.0f : 0.0f;
    return baseHitRate + hitRateCoefficient * Y;
}

// 体力消耗 = 固定 + X * 系数 , 其中 X = 力量
int Skill::calculateStaminaCost(int playerStrength) const {
    return baseStaminaCost + static_cast<int>(playerStrength * staminaCostCoefficient);
}


// ========== 使用相关 ==========

// 检查技能是否解锁，体力值是否足够，由于Player类尚未给出完整定义，暂时注释掉这一方法
/*
bool Skill::canUse(const bool isLocked, const Player& user) const {
    if (isLocked) return false;
    if(体力槽 * 疲劳值 < calculateStaminaCost(user.getStrength())) 
        return false; // TODO: 换成 user.getStrength()
    return true;
}
*/

// 技能执行 尚不完善
/*
void Skill::execute(Player& user, Player& target) {
    if (effectType == SkillEffectType::BASIC_ATTACK) {
        int dmg = calculateDamage(user.getStrength()); // TODO: 换成 user.getStrength()
        // target.takeDamage(dmg);
    } else if (customEffect) {
        customEffect(user, target);
    }
}
*/


// ========== 类型判断 ==========
// 基础攻击类技能
bool Skill::isAttackSkill() const { return effectType == SkillEffectType::BASIC_ATTACK; }
// 属性增益类技能
bool Skill::isAttributeBoostSkill() const { return effectType == SkillEffectType::ATTRIBUTE_BOOST; }
// 训练效率类技能
bool Skill::isTrainingSkill() const { return effectType == SkillEffectType::TRAINING_EFFICIENCY; }
// 其他特殊效果类技能
bool Skill::isSpecialEffectSkill() const {
    return effectType != SkillEffectType::BASIC_ATTACK &&
           effectType != SkillEffectType::ATTRIBUTE_BOOST &&
           effectType != SkillEffectType::TRAINING_EFFICIENCY;
}


// ========== 序列化实现 ==========
// 暂不确定序列化标准，暂时注释
/*
std::string Skill::serialize() const {
    std::ostringstream oss;
    oss << skillName << "|" << description << "|" << static_cast<int>(effectType) << "|"
        << isLocked << "|" << baseDamage << "|" << damageCoefficient << "|"
        << baseHitRate << "|" << hitRateCoefficient << "|"
        << baseStaminaCost << "|" << staminaCostCoefficient << "|"
        << minAttributeValue << "|" << attributeType << "|"
        << trainingMultiplier << "|" << effectValue;
    return oss.str();
}

void Skill::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string token;

    getline(iss, skillName, '|');
    getline(iss, description, '|');
    getline(iss, token, '|'); effectType = static_cast<SkillEffectType>(std::stoi(token));
    getline(iss, token, '|'); isLocked = (token == "1");
    getline(iss, token, '|'); baseDamage = std::stoi(token);
    getline(iss, token, '|'); damageCoefficient = std::stof(token);
    getline(iss, token, '|'); baseHitRate = std::stoi(token);
    getline(iss, token, '|'); hitRateCoefficient = std::stof(token);
    getline(iss, token, '|'); baseStaminaCost = std::stoi(token);
    getline(iss, token, '|'); staminaCostCoefficient = std::stof(token);
    getline(iss, token, '|'); minAttributeValue = std::stoi(token);
    getline(iss, attributeType, '|');
    getline(iss, token, '|'); trainingMultiplier = std::stof(token);
    getline(iss, token, '|'); effectValue = std::stof(token);
}
*/

// 创建所有技能实例（只包含基础攻击类型）
std::vector<Skill*> createAllSkills() {
    std::vector<Skill*> skills;
    
    // ==================== 基础攻击技能 ====================
    // 格式: 技能名称, 描述, 基础伤害, 伤害系数, 基础命中率, 命中系数, 基础体力消耗, 体力消耗系数
    
    // 1技能点基础攻击技能
    skills.push_back(new Skill("直拳", "快速直拳攻击", 
        1, 0.7f, 70, 20.0f, 0, 1.0f, 1));
    
    skills.push_back(new Skill("踢腿", "基础踢腿攻击", 
        3, 0.3f, 30, 30.0f, 0, 1.0f, 1));
    
    // 2技能点基础攻击技能
    skills.push_back(new Skill("高踢腿", "高位踢腿攻击", 
        4, 0.7f, 15, 40.0f, 1, 1.2f, 2));
    
    skills.push_back(new Skill("重拳", "强力拳击", 
        3, 0.9f, 60, 20.0f, 2, 1.2f, 2));
    
    skills.push_back(new Skill("上钩拳", "上勾拳攻击", 
        1, 1.7f, 65, 25.0f, 1, 1.5f, 2));
    
    skills.push_back(new Skill("反手重拳", "反手强力拳击", 
        3, 2.0f, 20, 60.0f, 2, 1.5f, 2));
    
    skills.push_back(new Skill("反手直拳", "反手直拳攻击", 
        2, 1.5f, 45, 35.0f, 2, 1.5f, 2));
    
    skills.push_back(new Skill("交叉拳", "交叉组合拳", 
        2, 2.0f, 65, 25.0f, 1, 1.6f, 2));
    
    // 3技能点基础攻击技能
    skills.push_back(new Skill("空手道踢腿", "空手道踢技", 
        3, 1.0f, 20, 50.0f, 1, 0.8f, 3));
    
    skills.push_back(new Skill("空手道高踢腿", "空手道高踢", 
        5, 1.0f, 10, 55.0f, 2, 1.0f, 3));
    
    skills.push_back(new Skill("爪击", "利爪攻击", 
        8, 1.3f, 40, 20.0f, 4, 1.3f, 3));
    
    skills.push_back(new Skill("折背", "折背攻击", 
        4, 1.5f, 40, 35.0f, 3, 1.0f, 3));
    
    skills.push_back(new Skill("拳术直拳", "拳术直拳", 
        0, 1.7f, 70, 20.0f, 1, 0.7f, 3));
    
    skills.push_back(new Skill("蓄力上勾拳", "蓄力上勾拳", 
        0, 2.0f, 60, 25.0f, 1, 0.8f, 3));
    
    // 4技能点基础攻击技能
    skills.push_back(new Skill("空手道劈斩", "空手道劈砍", 
        2, 1.5f, 20, 55.0f, 2, 1.2f, 4));
    
    skills.push_back(new Skill("迅击", "迅捷攻击", 
        4, 1.5f, 60, 20.0f, 2, 0.6f, 4));
    
    skills.push_back(new Skill("近战缠斗", "近身缠斗", 
        1, 2.5f, 60, 20.0f, 1, 0.7f, 4));
    
    // ==================== 属性增益技能 ====================
    // 格式: 技能名称, 描述, 最低属性值, 属性类型, 解锁所需技能点



    // 这一部分设置最低属性点尚未完善



    
    // 1技能点属性增益技能
    skills.push_back(new Skill("屹立不倒", "敏捷不会低于4", 4, "agility", 1));
    skills.push_back(new Skill("肉食跑者", "耐力不会低于4", 4, "stamina", 1));
    skills.push_back(new Skill("肌肉记忆", "力量不会低于4", 4, "strength", 1));
    
    // 2技能点属性增益技能
    skills.push_back(new Skill("屹立不倒2", "敏捷不会低于8", 8, "agility", 2));
    skills.push_back(new Skill("肉食跑者2", "耐力不会低于8", 8, "stamina", 2));
    skills.push_back(new Skill("肌肉记忆2", "力量不会低于8", 8, "strength", 2));
    
    // 3技能点属性增益技能
    skills.push_back(new Skill("屹立不倒3", "敏捷不会低于13", 13, "agility", 3));
    skills.push_back(new Skill("肉食跑者3", "耐力不会低于13", 13, "stamina", 3));
    skills.push_back(new Skill("肌肉记忆3", "力量不会低于13", 13, "strength", 3));
    
    // ==================== 训练效率技能 ====================
    // 格式: 技能名称, 描述, 训练效率倍率, 属性类型, 解锁所需技能点
    
    // 3技能点训练效率技能
    skills.push_back(new Skill("迅速", "敏捷训练效率增加50%", 1.5f, "agility", 3));
    skills.push_back(new Skill("意志", "耐力训练效率增加50%", 1.5f, "stamina", 3));
    skills.push_back(new Skill("激励", "力量训练效率增加50%", 1.5f, "strength", 3));
    
    // ==================== 特殊效果技能 ====================
    // 格式: 技能名称, 描述, 效果类型, 效果数值, 解锁所需技能点
    
    // 2技能点特殊效果技能
    skills.push_back(new Skill("闪击", "25%概率反伤25%对面造成的伤害", 
        SkillEffectType::DAMAGE_REFLECT, 0.25f, 2));
    
    skills.push_back(new Skill("自杀式袭击", "命中率降低10%，能量消耗降低20%", 
        SkillEffectType::HIT_RATE_MODIFIER, -0.1f, 2));
    
    skills.push_back(new Skill("千手不破", "受到对面上肢攻击降低30%，下肢攻击增加20%", 
        SkillEffectType::ATTACK_TYPE_DEFENSE, 0.3f, 2));
    
    skills.push_back(new Skill("人身重锤", "25%概率降低对面10耐力", 
        SkillEffectType::ENEMY_ATTRIBUTE_MOD, 10.0f, 2));
    
    // 4技能点特殊效果技能
    skills.push_back(new Skill("钝兵挫锐", "免伤20%", 
        SkillEffectType::DAMAGE_REDUCTION, 0.2f, 4));
    
    skills.push_back(new Skill("无限能量", "如果体力低于50%，能量消耗降低15%", 
        SkillEffectType::STAMINA_MODIFIER, 0.15f, 4));
    
    skills.push_back(new Skill("拳击手", "命中率在原本基础上提高10%", 
        SkillEffectType::GLOBAL_HIT_RATE_BOOST, 0.1f, 4));
    // ==================== 其他类型技能占位符 ====================
    
    return skills;
}
