#include "Skill.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include "../entity/Player.h"
#include <cstdlib>
#include <ctime>

// ========== 统一构造函数实现 ==========
Skill::Skill(const std::string& name, const std::string& desc, SkillEffectType type, int cost,
             double baseDmg, double dmgCoeff, double baseHit, double hitCoeff,
             double baseStamina, double staminaCoeff, double minValue,
             const std::string& attrType, double multiplier, double value,
             std::function<void(Player&, Player&)> effect)
    : skillName(name), description(desc), effectType(type),
      isLocked(true), unlockCost(cost), baseDamage(baseDmg), damageCoefficient(dmgCoeff),
      baseHitRate(baseHit), hitRateCoefficient(hitCoeff),
      baseStaminaCost(baseStamina), staminaCostCoefficient(staminaCoeff),
      minAttributeValue(minValue), attributeType(attrType),
      trainingMultiplier(multiplier), effectValue(value), customEffect(effect) {}




// ========== Getter 实现 ==========
std::string Skill::getSkillName() const { return skillName; }
std::string Skill::getDescription() const { return description; }
SkillEffectType Skill::getEffectType() const { return effectType; }
int Skill::getUnlockCost() const { return unlockCost; }
bool Skill::getIsLocked() const { return isLocked; }

double Skill::getBaseDamage() const { return baseDamage; }
double Skill::getDamageCoefficient() const { return damageCoefficient; }
double Skill::getBaseHitRate() const { return baseHitRate; }
double Skill::getHitRateCoefficient() const { return hitRateCoefficient; }
double Skill::getBaseStaminaCost() const { return baseStaminaCost; }
double Skill::getStaminaCostCoefficient() const { return staminaCostCoefficient; }

double Skill::getMinAttributeValue() const { return minAttributeValue; }
std::string Skill::getAttributeType() const { return attributeType; }
double Skill::getTrainingMultiplier() const { return trainingMultiplier; }
double Skill::getEffectValue() const { return effectValue; }

// ========== 属性设置方法实现 ==========
/*
void Skill::setMinAttributeValue(Player& user) {
    // 根据 attributeType 设置玩家最低属性
    if (attributeType == "strength") {
        user.setMinStrength(std::max(user.getMinStrength(), minAttributeValue));
    } else if (attributeType == "agility") {
        user.setMinAgility(std::max(user.getMinAgility(), minAttributeValue));
    } else if (attributeType == "stamina") {
        user.setMinStamina(std::max(user.getMinStamina(), minAttributeValue));
    }
}
*/

// ========== 计算公式实现 ==========
// 伤害 = 固定 + X * 系数 , 其中 X = 力量
double Skill::calculateDamage(double playerStrength) const {
    return baseDamage + playerStrength * damageCoefficient;
}

// 命中率 = 固定 + Y * 系数 , 其中 Y = (敏捷 / (力量 + 敏捷 + 耐力)) * 3
double Skill::calculateHitRate(double playerAgility, double playerStrength, double playerStamina) const {
    double total = playerStrength + playerAgility + playerStamina;
    double Y = (total > 0) ? (playerAgility / total) * 3.0 : 0.0;
    return baseHitRate + hitRateCoefficient * Y;
}

// 体力消耗 = 固定 + X * 系数 , 其中 X = 力量
double Skill::calculateStaminaCost(double playerStrength) const {
    return baseStaminaCost + playerStrength * staminaCostCoefficient;
}

// ========== 解锁相关方法实现 ==========
void Skill::unlock() {
    isLocked = false;
}

void Skill::lock() {
    isLocked = true;
}

bool Skill::canUnlock(int playerSkillPoints) const {
    return playerSkillPoints >= unlockCost;
}

// ========== 使用条件检查 ==========
bool Skill::canUse(const Player& user) const {
    if (isLocked) {
        return false;
    }
    
    // 检查体力是否足够
    double requiredStamina = calculateStaminaCost(user.getStrength());
    if (user.getEnergy() * user.getFatigue() < requiredStamina) {
        return false;
    }
    
    return true;
}

// ========== 执行技能实现 ==========
void Skill::execute(Player& user, Player& target) {
    if (!canUse(user)) {
        return; // 技能不可用
    }
    
    // 消耗疲劳值（先计算出SPCost, 再算出fatigueCost）
    double staminaCost = calculateStaminaCost(user.getStrength());    // 体力消耗
    double fatigueCost = staminaCost / user.getEnergy(); // 疲劳值消耗 = 体力消耗 / 体力槽
    user.addFatigue(-fatigueCost);  // 原Player类只给出了add的端口，add一个负数来实现疲劳值减少
    
    switch (effectType) {
        case SkillEffectType::BASIC_ATTACK: {
            // 基础攻击技能
            double damage = calculateDamage(user.getStrength());
            double healthiness = damage / user.getEnergy(); // 伤害转换成健康值
            double hitRate = calculateHitRate(user.getAgility(), user.getStrength(), user.getStamina());

            srand((unsigned)time(NULL));
            double roll = (double)rand() / RAND_MAX; // 生成0到1之间的随机数
            if(roll <= hitRate) {
                target.addHealthiness(-healthiness); // 造成伤害
            }
            else{
                // 未命中, 无效果
            }
            break;
        }
        
        case SkillEffectType::ATTRIBUTE_BOOST: {
            // 属性增益技能
            // setMinAttributeValue(user); // 暂时注释，等待Player接口实现
            break;
        }
        
        case SkillEffectType::TRAINING_EFFICIENCY: {
            // 训练效率技能 - 需要在训练系统中应用倍率
            // 这里只是标记，实际效果在训练时应用
            break;
        }
        
        case SkillEffectType::DAMAGE_REFLECT: {
            // 伤害反射 - 需要战斗系统支持
            break;
        }
        
        case SkillEffectType::DAMAGE_REDUCTION: {
            // 伤害减免 - 需要战斗系统支持
            break;
        }
        
        case SkillEffectType::HIT_RATE_MODIFIER: {
            // 命中率修改 - 需要战斗系统支持
            break;
        }
        
        case SkillEffectType::STAMINA_MODIFIER: {
            // 体力消耗修改 - 需要战斗系统支持
            break;
        }
        
        case SkillEffectType::ATTACK_TYPE_DEFENSE: {
            // 攻击类型防御 - 需要战斗系统支持
            break;
        }
        
        case SkillEffectType::ENEMY_ATTRIBUTE_MOD: {
            // 敌人属性修改
            // 假设有降低敌人耐力的方法
            // target.reduceStamina(effectValue);
            break;
        }
        
        case SkillEffectType::GLOBAL_HIT_RATE_BOOST: {
            // 全局命中率提升 - 需要战斗系统支持
            break;
        }
        
        case SkillEffectType::NONE: {
            // 自定义效果技能
            if (customEffect) {
                customEffect(user, target);
            }
            break;
        }
    }
}

// ========== 类型判断 ==========
bool Skill::isAttackSkill() const { 
    return effectType == SkillEffectType::BASIC_ATTACK; 
}

bool Skill::isAttributeBoostSkill() const { 
    return effectType == SkillEffectType::ATTRIBUTE_BOOST; 
}

bool Skill::isTrainingSkill() const { 
    return effectType == SkillEffectType::TRAINING_EFFICIENCY; 
}

bool Skill::isSpecialEffectSkill() const {
    return effectType != SkillEffectType::BASIC_ATTACK &&
           effectType != SkillEffectType::ATTRIBUTE_BOOST &&
           effectType != SkillEffectType::TRAINING_EFFICIENCY;
}

// ========== 序列化实现 ==========
// 暂不确定序列化标准，暂时注释
/*
std::string Skill::serialize() const {
    // 实现序列化逻辑
}

void Skill::deserialize(const std::string& data) {
    // 实现反序列化逻辑
}
*/


std::vector<Skill*> createAllSkills() {
    std::vector<Skill*> skills;
    
    // ==================== 基础攻击技能 ====================
    // 格式: 技能名称, 描述, 技能类型, 解锁所需技能点, 基础伤害, 伤害系数, 基础命中率, 命中系数, 基础体力消耗, 体力消耗系数
    
    // 1技能点基础攻击技能
    skills.push_back(new Skill("直拳", "快速直拳攻击", SkillEffectType::BASIC_ATTACK, 1,
        1.0, 0.7, 70.0, 20.0, 0.0, 1.0));
    
    skills.push_back(new Skill("踢腿", "基础踢腿攻击", SkillEffectType::BASIC_ATTACK, 1,
        3.0, 0.3, 30.0, 30.0, 0.0, 1.0));
    
    // 2技能点基础攻击技能
    skills.push_back(new Skill("高踢腿", "高位踢腿攻击", SkillEffectType::BASIC_ATTACK, 2,
        4.0, 0.7, 15.0, 40.0, 1.0, 1.2));
    
    skills.push_back(new Skill("重拳", "强力拳击", SkillEffectType::BASIC_ATTACK, 2,
        3.0, 0.9, 60.0, 20.0, 2.0, 1.2));
    
    skills.push_back(new Skill("上钩拳", "上勾拳攻击", SkillEffectType::BASIC_ATTACK, 2,
        1.5, 1.7, 65.0, 25.0, 1.0, 1.5));
    
    skills.push_back(new Skill("反手重拳", "反手强力拳击", SkillEffectType::BASIC_ATTACK, 2,
        3.0, 2.0, 20.0, 60.0, 2.0, 1.5));
    
    skills.push_back(new Skill("反手直拳", "反手直拳攻击", SkillEffectType::BASIC_ATTACK, 2,
        2.0, 1.5, 45.0, 35.0, 2.0, 1.5));
    
    skills.push_back(new Skill("交叉拳", "交叉组合拳", SkillEffectType::BASIC_ATTACK, 2,
        2.5, 2.0, 65.0, 25.0, 1.5, 1.6));
    
    // 3技能点基础攻击技能
    skills.push_back(new Skill("空手道踢腿", "空手道踢技", SkillEffectType::BASIC_ATTACK, 3,
        3.0, 1.0, 20.0, 50.0, 1.5, 0.8));
    
    skills.push_back(new Skill("空手道高踢腿", "空手道高踢", SkillEffectType::BASIC_ATTACK, 3,
        5.0, 1.0, 10.0, 55.0, 2.0, 1.0));
    
    skills.push_back(new Skill("爪击", "利爪攻击", SkillEffectType::BASIC_ATTACK, 3,
        8.0, 1.3, 40.0, 20.0, 4.0, 1.3));
    
    skills.push_back(new Skill("折背", "折背攻击", SkillEffectType::BASIC_ATTACK, 3,
        4.0, 1.5, 40.0, 35.0, 3.0, 1.0));
    
    skills.push_back(new Skill("拳术直拳", "拳术直拳", SkillEffectType::BASIC_ATTACK, 3,
        0.0, 1.7, 70.0, 20.0, 1.0, 0.7));
    
    skills.push_back(new Skill("蓄力上勾拳", "蓄力上勾拳", SkillEffectType::BASIC_ATTACK, 3,
        0.0, 2.0, 60.0, 25.0, 1.0, 0.8));
    
    // 4技能点基础攻击技能
    skills.push_back(new Skill("空手道劈斩", "空手道劈砍", SkillEffectType::BASIC_ATTACK, 4,
        2.0, 1.5, 20.0, 55.0, 2.0, 1.2));
    
    skills.push_back(new Skill("迅击", "迅捷攻击", SkillEffectType::BASIC_ATTACK, 4,
        4.0, 1.5, 60.0, 20.0, 2.0, 0.6));
    
    skills.push_back(new Skill("近战缠斗", "近身缠斗", SkillEffectType::BASIC_ATTACK, 4,
        1.0, 2.5, 60.0, 20.0, 1.0, 0.7));
    
    // ==================== 属性增益技能 ====================
    // 格式: 技能名称, 描述, 技能类型, 解锁所需技能点, 最低属性值, 属性类型
    
    skills.push_back(new Skill("屹立不倒", "敏捷不会低于4", SkillEffectType::ATTRIBUTE_BOOST, 1,
        0, 0, 0, 0, 0, 0, 4.0, "agility"));
    
    skills.push_back(new Skill("肉食跑者", "耐力不会低于4", SkillEffectType::ATTRIBUTE_BOOST, 1,
        0, 0, 0, 0, 0, 0, 4.0, "stamina"));
    
    skills.push_back(new Skill("肌肉记忆", "力量不会低于4", SkillEffectType::ATTRIBUTE_BOOST, 1,
        0, 0, 0, 0, 0, 0, 4.0, "strength"));
    
    skills.push_back(new Skill("屹立不倒2", "敏捷不会低于8", SkillEffectType::ATTRIBUTE_BOOST, 2,
        0, 0, 0, 0, 0, 0, 8.0, "agility"));
    
    skills.push_back(new Skill("肉食跑者2", "耐力不会低于8", SkillEffectType::ATTRIBUTE_BOOST, 2,
        0, 0, 0, 0, 0, 0, 8.0, "stamina"));
    
    skills.push_back(new Skill("肌肉记忆2", "力量不会低于8", SkillEffectType::ATTRIBUTE_BOOST, 2,
        0, 0, 0, 0, 0, 0, 8.0, "strength"));
    
    skills.push_back(new Skill("屹立不倒3", "敏捷不会低于13", SkillEffectType::ATTRIBUTE_BOOST, 3,
        0, 0, 0, 0, 0, 0, 13.0, "agility"));
    
    skills.push_back(new Skill("肉食跑者3", "耐力不会低于13", SkillEffectType::ATTRIBUTE_BOOST, 3,
        0, 0, 0, 0, 0, 0, 13.0, "stamina"));
    
    skills.push_back(new Skill("肌肉记忆3", "力量不会低于13", SkillEffectType::ATTRIBUTE_BOOST, 3,
        0, 0, 0, 0, 0, 0, 13.0, "strength"));
    
    // ==================== 训练效率技能 ====================
    // 格式: 技能名称, 描述, 技能类型, 解锁所需技能点, 训练效率倍率, 属性类型
    
    skills.push_back(new Skill("迅速", "敏捷训练效率增加50%", SkillEffectType::TRAINING_EFFICIENCY, 3,
        0, 0, 0, 0, 0, 0, 0, "agility", 1.5));
    
    skills.push_back(new Skill("意志", "耐力训练效率增加50%", SkillEffectType::TRAINING_EFFICIENCY, 3,
        0, 0, 0, 0, 0, 0, 0, "stamina", 1.5));
    
    skills.push_back(new Skill("激励", "力量训练效率增加50%", SkillEffectType::TRAINING_EFFICIENCY, 3,
        0, 0, 0, 0, 0, 0, 0, "strength", 1.5));
    
    // ==================== 特殊效果技能 ====================
    // 格式: 技能名称, 描述, 技能类型, 解锁所需技能点, 效果数值
    
    skills.push_back(new Skill("闪击", "25%概率反伤25%对面造成的伤害", SkillEffectType::DAMAGE_REFLECT, 2,
        0, 0, 0, 0, 0, 0, 0, "", 0, 0.25));
    
    skills.push_back(new Skill("自杀式袭击", "命中率降低10%，能量消耗降低20%", SkillEffectType::HIT_RATE_MODIFIER, 2,
        0, 0, 0, 0, 0, 0, 0, "", 0, -0.1));
    
    skills.push_back(new Skill("千手不破", "受到对面上肢攻击降低30%，下肢攻击增加20%", SkillEffectType::ATTACK_TYPE_DEFENSE, 2,
        0, 0, 0, 0, 0, 0, 0, "", 0, 0.3));
    
    skills.push_back(new Skill("人身重锤", "25%概率降低对面10耐力", SkillEffectType::ENEMY_ATTRIBUTE_MOD, 2,
        0, 0, 0, 0, 0, 0, 0, "", 0, 10.0));
    
    skills.push_back(new Skill("钝兵挫锐", "免伤20%", SkillEffectType::DAMAGE_REDUCTION, 4,
        0, 0, 0, 0, 0, 0, 0, "", 0, 0.2));
    
    skills.push_back(new Skill("无限能量", "如果体力低于50%，能量消耗降低15%", SkillEffectType::STAMINA_MODIFIER, 4,
        0, 0, 0, 0, 0, 0, 0, "", 0, 0.15));
    
    skills.push_back(new Skill("拳击手", "命中率在原本基础上提高10%", SkillEffectType::GLOBAL_HIT_RATE_BOOST, 4,
        0, 0, 0, 0, 0, 0, 0, "", 0, 0.1));
    
    return skills;
}