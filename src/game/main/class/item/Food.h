
#ifndef FOOD_H
#define FOOD_H

#include <string>
#include "../../entity/Player.h"
using std::string;

// 食品基类
class Food : public Item {
public:
    explicit Food(const string& name, int price);
    virtual ~Food();

    // 获取食品名称
    const string& getName() const;
    // 获取价格
    int getPrice() const;

    // 食品使用效果
    virtual void use(Player::Stats& stats) = 0;

protected:
    string name_;
    int price_;
};

// 便利店食品
class Meat : public Food {
public:
    Meat();
    void use(Player::Stats& stats) override;
};
class SparklingWater : public Food {
public:
    SparklingWater();
    void use(Player::Stats& stats) override;
};
class EnergyDrink : public Food {
public:
    EnergyDrink();
    void use(Player::Stats& stats) override;
};
class FrozenPizza : public Food {
public:
    FrozenPizza();
    void use(Player::Stats& stats) override;
};

// 健身房食品
class ChocolateBar : public Food {
public:
    ChocolateBar();
    void use(Player::Stats& stats) override;
};
class ProteinBar : public Food {
public:
    ProteinBar();
    void use(Player::Stats& stats) override;
};

#endif // FOOD_H
