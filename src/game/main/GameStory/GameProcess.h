#ifndef INC_2025_OUC_MUDGAME_GAMEPROCESS_H
#define INC_2025_OUC_MUDGAME_GAMEPROCESS_H

#include "Story.h"
#include "../basic/Dialog.h"
#include "../basic/Game.h"
#include "Story/chapter1.h"
#include "../basic/Game.h"
#include "../class/entity/Player.h"
#include "../basic/InputProcess.h"

#include "FTXUI/component/component.hpp"
#include "FTXUI/component/screen_interactive.hpp"
#include "FTXUI/dom/elements.hpp"

class GameProcess : public Dialog {
public:
    static void gameProcess(const DialogNode* index, Dialog* dialog_) {
        while (index == nullptr) {

        }
    }

    static void newStart(Dialog* dialog_, Player* player) {
        using namespace ftxui;

        // 1. 创建一个全屏的交互式屏幕实例
        auto screen = ScreenInteractive::Fullscreen();

        // 2. 准备输入组件所需的数据
        std::string input_name; // 使用一个明确的变量名

        // 引导改名 - 这条消息会作为对话历史的初始内容
        dialog_->addMessage(_00000001.who, _00000001.content);

        // 3. 定义输入组件和它的“完成”事件
        InputOption option;
        option.on_enter = [&] {
            // 当用户按下回车时，设置玩家名字
            player->setName(trim(input_name));
            screen.Exit();
        };
        auto input_command = Input(&input_name, "请在这里输入你的名字...", option);

        // 4. 创建对话历史的渲染器 (Renderer)
        auto main_view_renderer = Renderer([&] {
            // 从 dialog_ 获取对话历史记录
            const auto& messages = dialog_->getHistory();
            Elements message_elements;
            for (const auto& msg : messages) {
                auto who_color = (msg.who == "主角") ? Color::Green : Color::Cyan;
                message_elements.push_back(
                    hbox({
                        text(msg.who) | bold | color(who_color),
                        text(": "),
                        text(msg.content)
                    })
                );
            }

            if (!message_elements.empty()) {
                message_elements.back() |= focus;
            }

            return window(text(" 对话记录 "), vbox(message_elements) | flex) | flex;
        });

        // 5. 将对话历史的渲染器包装，使其可以被放入容器中
        // 这部分也与 showGameScreen 相同
        auto interactive_main_view = CatchEvent(main_view_renderer, [](Event) { return false; });

        // 6. 将界面上的可交互组件（这里只有主视窗和输入框）放入一个容器中
        auto main_container = Container::Vertical({
            interactive_main_view,
            input_command,
        });

        auto final_renderer = Renderer(main_container, [&] {
            // -- 顶部 Header --
            // 保持和主界面一致的风格
            auto header = hbox({
                text("   拳王之路   ") | bold | color(Color::Red),
                filler(),
                text("当前位置: ?????? ") | color(Color::Yellow),
            }) | border;

            auto main_content = hbox({
                interactive_main_view->Render() | vscroll_indicator | yframe | flex,
            });

            auto footer = window(text(" 输入 "), input_command->Render());

            return vbox({
                header,
                main_content | flex,
                footer,
            });
        });

        // 程序会在这里显示UI并“暂停”，等待用户输入名字并按回车
        screen.Loop(final_renderer);

        // 9. 添加游戏正式开始的对话
        dialog_->addMessage(_00000002.who, player->getName() + _00000002.content);
    }
};


#endif //INC_2025_OUC_MUDGAME_GAMEPROCESS_H