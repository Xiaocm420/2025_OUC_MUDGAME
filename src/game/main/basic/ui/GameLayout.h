#ifndef GAMELAYOUT_H
#define GAMELAYOUT_H

#include "../Types.h"
#include "FTXUI/component/component_base.hpp"
#include "FTXUI/component/component.hpp"
#include <string>

class Game;

/**
 * @class GameLayout
 * @brief 一个自定义FTXUI组件，封装了整个游戏主界面的布局、状态和交互逻辑。
 * @details 通过将所有子组件和UI状态作为成员变量，解决了生命周期问题，并提高了代码的封装性。
 */
class GameLayout : public ftxui::ComponentBase {
public:
    /**
     * @brief GameLayout的构造函数。
     * @param game_logic 对Game核心对象的引用，用于获取游戏状态和数据。
     */
    explicit GameLayout(Game& game_logic);

    /**
     * @brief 重写的Render方法，FTXUI每一帧都会调用此方法来绘制界面。
     * @return 返回一个代表当前帧UI布局的ftxui::Element。
     */
    ftxui::Element Render() override;

private:
    Game& game_logic_; ///< 对Game核心对象的引用。

    // --- UI状态和数据成员 ---
    std::string command_input_str_;     ///< 存储默认指令输入框的文本。
    std::string text_input_str_;        ///< 存储通用文本输入框的文本。
    int selected_input_mode_ = 0;       ///< 控制输入区域Tab的当前激活项 (0: InGame, 1: TextInput, 2: Choice)。
    int scroll_index_ = 0;              ///< 对话历史的滚动偏移量。
    size_t current_message_index_ = 0;  ///< 用于打字机效果的当前消息索引。

    // --- 子组件成员 ---
    ftxui::Component interactiveMainView_;      ///< 可交互的对话历史显示区。
    ftxui::Component navigationContainer_;      ///< 右侧的功能按钮容器。
    ftxui::Component command_input_component_;  ///< 默认的指令输入组件。
    ftxui::Component text_input_component_;     ///< 通用的文本输入组件。
    ftxui::Component choice_container_;         ///< 用于动态添加选项按钮的容器。
    ftxui::Component inputArea_;                ///< 包含所有输入模式的Tab容器。
};

#endif // GAMELAYOUT_H