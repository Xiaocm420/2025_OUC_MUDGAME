#pragma once

#include <string>

// 告知 Game 类存在
class Game;

class View {
public:
    // 构造函数接收一个 Game 逻辑控制器的引用
    explicit View(Game& game_logic);

    // 主菜单
    void ShowMainMenu();

    // 加载栏
    void ShowLoadingScreen(const std::string& subtitle);

private:
    // 保存 Game 对象的引用，以便在按钮点击时调用其方法
    Game& game_logic_;
};