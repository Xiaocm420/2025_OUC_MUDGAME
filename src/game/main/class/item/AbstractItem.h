#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <string>
#include "../entity/Player.h"

class AbstractItem {
public:
    // 虚析构函数，确保派生类析构函数能正确调用
    virtual ~AbstractItem() = default;

    // 纯虚函数：获取物品名称
    virtual const std::string& getName() const = 0;

    // 纯虚函数：获取物品价格（返回int类型，统一价格接口）
    virtual int getPrice() const = 0;

    // 纯虚函数：使用物品（参数为Player对象，物品使用效果作用于玩家）
    virtual void use(Player& user) = 0;
};

#endif // ABSTRACTITEM_H