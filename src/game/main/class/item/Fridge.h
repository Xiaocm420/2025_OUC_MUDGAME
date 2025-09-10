#ifndef FRIDGE_H
#define FRIDGE_H

#include <map>
#include <string>
#include "Food.h"

class Fridge {
public:
    Fridge();
    ~Fridge();

    // 添加食物，返回是否成功
    bool add(const std::string& name);
    // 使用食物，返回是否成功
    bool use(const std::string& name, Player::Stats& stats);
    // 打印冰箱内容
    void print() const;

private:
    static const int MAX_PER_TYPE = 7;
    std::map<std::string, int> storage_;
    std::map<std::string, Food*> food_types_;
};

#endif // FRIDGE_H
