#ifndef FRIDGE_H
#define FRIDGE_H

#include <string>
#include <unordered_map>

// 前向声明所有食物类（确保编译通过）
class Meat;
class SparklingWater;
class EnergyDrink;
class FrozenPizza;
class ChocolateBar;
class ProteinBar;

class Fridge {
private:
    // 存储每种食物的实例（用于调用 use 方法）
    std::unordered_map<std::string, Meat*> food_types_;
    // 存储每种食物的数量
    std::unordered_map<std::string, int> storage_;
    // 每种食物的最大存储量（常量）
    static const int MAX_PER_TYPE = 10; // 可根据需求调整数值

public:
    Fridge();          // 构造函数（初始化食物实例）
    ~Fridge();         // 析构函数（释放食物实例内存）
    bool add(const std::string& name);  // 添加食物
    bool use(const std::string& name);  // 使用食物（删除 Stats 参数）
    void print() const;                 // 打印冰箱内容
};

#endif // FRIDGE_H