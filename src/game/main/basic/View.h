#pragma once

#include <string>

#include "FTXUI/component/component.hpp"
#include "FTXUI/component/screen_interactive.hpp"

// 告知 Game 类存在
class Game;

class View {
public:
    // 构造函数接收一个 Game 逻辑控制器的引用
    explicit View(Game& game_logic);

    // 主菜单
    void showMainMenu();

    // 加载栏
    static void showLoadingScreen(const std::string& subtitle);

    // 游戏界面
    void showGameScreen();

    // 游戏介绍
    static void showGameIntroScreen();

    // 主UI
    ftxui::Component makeGameLayout(
        bool showRightButtons,                // 是否显示右侧按钮
        bool showStatusBar,                   // 是否显示状态栏
        bool allowInput,                      // 是否允许输入
        bool isVoiceOver,                     // 是否为旁白
        const std::string& playerLocation,    // 玩家位置
        const std::string& who,               // 谁说的，isVoiceOver 为 true 时此项应传入 ""
        const std::string& inputPlaceholder,  // 输入提示词
        std::optional<ftxui::Component> externalInput = std::nullopt); // 可选输入组件

private:
    // 保存 Game 对象的引用，以便在按钮点击时调用其方法
    Game& game_logic_;
};