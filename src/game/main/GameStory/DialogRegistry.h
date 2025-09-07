#pragma once

#include <functional>
#include <map>
#include <vector>

struct DialogNode;

/**
 * @brief 定义用于注册对话数据的函数指针类型。
 * @param db 指向要填充的对话数据库的引用。
 */
using DialogRegistrationFunc = std::function<void(std::map<unsigned int, const DialogNode*>&)>;

/**
 * @class DialogRegistry
 * @brief 对话数据的自动注册中心。
 * @details 使用静态注册表模式，允许各个章节的对话数据在程序启动时自动注册，
 *          无需手动修改StoryController。
 */
class DialogRegistry {
public:
    /**
     * @brief 向注册中心添加一个对话注册函数。
     * @param func 要添加的注册函数。
     */
    static void add(DialogRegistrationFunc func) {
        getInitializers().push_back(std::move(func));
    }

    /**
     * @brief 执行所有已注册的函数，以填充对话数据库。
     * @param db 指向StoryController中的对话数据库的引用。
     */
    static void runAll(std::map<unsigned int, const DialogNode*>& db) {
        for (const auto& func : getInitializers()) {
            func(db);
        }
    }
private:
    /**
     * @brief 使用函数内静态变量实现单例模式，获取注册函数列表。
     * @return 对注册函数向量的引用。
     */
    static std::vector<DialogRegistrationFunc>& getInitializers() {
        static std::vector<DialogRegistrationFunc> initializers;
        return initializers;
    }
};

/**
 * @struct DialogRegistrar
 * @brief 一个辅助结构体，利用其构造函数实现自动注册。
 * @details 在每个章节文件中创建一个此结构体的静态实例，即可将其注册函数自动添加到DialogRegistry中。
 */
struct DialogRegistrar {
    explicit DialogRegistrar(DialogRegistrationFunc func) {
        DialogRegistry::add(std::move(func));
    }
};