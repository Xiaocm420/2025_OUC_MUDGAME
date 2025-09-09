
#ifndef MEDICINE_H
#define MEDICINE_H

#include <string>
#include "../../entity/Player.h"
using std::string;

// 药品基类
class Medicine : public Item {
public:
    explicit Medicine(const string& name, int price);
    virtual ~Medicine();

    // 获取药品名称
    const string& getName() const;
    // 获取价格
    int getPrice() const;

    // 药品使用效果
    virtual void use(Player::Stats& stats) = 0;

protected:
    string name_;
    int price_;
};

// 药店药品
class RecoveryMedication : public Medicine {
public:
    RecoveryMedication();
    void use(Player::Stats& stats) override;
};
class StrengthEnhancedMedicine : public Medicine {
public:
    StrengthEnhancedMedicine();
    void use(Player::Stats& stats) override;
};
class StaminaEnhancedMedicine : public Medicine {
public:
    StaminaEnhancedMedicine();
    void use(Player::Stats& stats) override;
};
class AgilityEnhancedMedicine : public Medicine {
public:
    AgilityEnhancedMedicine();
    void use(Player::Stats& stats) override;
};
class SkillPointBuffMedicine : public Medicine {
public:
    SkillPointBuffMedicine();
    void use(Player::Stats& stats) override;
};

#endif // MEDICINE_H
