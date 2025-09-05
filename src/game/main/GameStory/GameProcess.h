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
#include <chrono>
#include <thread>
#include <atomic>

class GameProcess : public Dialog {
public:
    static void gameProcess(const DialogNode* index, Dialog* dialog_) {
        while (index == nullptr) {

        }
    }

    static std::string newStart(Dialog* dialog_, Player* player, View* view) {
        using namespace ftxui;

        auto screen = ScreenInteractive::Fullscreen();

        // 1. 定义专用于此场景的输入组件及其行为
        std::string name;
        auto nameInputComponent = Input(&name, "输入你的名字", {
            .on_enter = [&] {
                name = trim(name);
                if (name.empty()) {
                    dialog_->addMessage(UNKNOWN, "你没名字吗？给你自己取一个名字吧");
                    name.clear();
                    return; // 名字无效，界面不退出
                } else if (8 <= getLength(name)) {
                    dialog_->addMessage(UNKNOWN, "名字长度太长了，记不住，换个短一点的，比如 8 个字以内的");
                    name.clear();
                    return;
                } else if (1 >= getLength(name)) {
                    dialog_->addMessage(UNKNOWN, "你名字也太短了吧，有没有个长一点的名字");
                    name.clear();
                    return;
                }

                dialog_->addMessage(_00000002.who, name + _00000002.content);

                // 名字有效，给名字赋值
                player->setName(name);

                // 任务完成，退出当前屏幕循环
                screen.Exit();
            },
        });

        // 2. 调用 View 的通用布局函数来构建UI
        //    我们传入定制的参数和刚刚创建的输入组件
        auto layout_component = view->makeGameLayout(
            false,                   // 不显示右侧按钮
            false,                   // 不显示状态栏
            true,                    // 禁用 makeGameLayout 内部的默认输入框
            false,                   // 不是旁白
            "???",                // 玩家位置
            "",                   // 自己的称呼（用于高亮对话）
            "",                   // 默认输入框的占位符（此场景下无用）
            nameInputComponent    // <-- 这里！注入我们自定义的输入组件
        );

        // 每20ms触发一次重绘以驱动打字机动画
        std::atomic<bool> refresh_running{true};
        std::thread refresh_thread([&] {
            while (refresh_running) {
                screen.PostEvent(Event::Custom);
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        });

        // 在循环开始前，添加第一条引导消息
        dialog_->addMessage(_00000001.who, _00000001.content);

        // 启动屏幕循环，程序会在这里等待用户输入
        screen.Loop(layout_component);

        refresh_running = false;
        if (refresh_thread.joinable()) refresh_thread.join();

        // 4. 返回玩家的名字，此时所有UI和对话逻辑都已完成
        return player->getName();
    }
};


#endif //INC_2025_OUC_MUDGAME_GAMEPROCESS_H