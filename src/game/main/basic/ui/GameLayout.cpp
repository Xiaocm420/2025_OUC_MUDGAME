#include "GameLayout.h"
#include "../Game.h"
#include "../Dialog.h"
#include "../../class/entity/Player.h"

#include "FTXUI/component/screen_interactive.hpp"
#include "FTXUI/dom/elements.hpp"
#include "FTXUI/screen/string.hpp"

#include <chrono>

using namespace ftxui;

GameLayout::GameLayout(Game& game_logic) : game_logic_(game_logic) {
    // --- 1. 在构造函数中初始化所有成员组件 ---

    // -- 对话历史渲染器 --
    auto mainViewRenderer = Renderer([this] {
        const auto& messages = game_logic_.getDialog().getHistory();
        Elements messageElements;
        size_t total = messages.size();
        size_t end = total > static_cast<size_t>(scroll_index_) ? total - scroll_index_ : 0;

        if (!messages.empty() && current_message_index_ < messages.size()) {
            const auto& current_msg = messages[current_message_index_];
            auto now = std::chrono::steady_clock::now();
            auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - current_msg.start_time).count();
            size_t contentSize = Utf8ToGlyphs(current_msg.content).size();
            size_t shownChars = std::min(contentSize, static_cast<size_t>(elapsedMs / 20));

            if (shownChars >= contentSize && current_message_index_ < messages.size() - 1) {
                if (elapsedMs > contentSize * 20 + 500) {
                    current_message_index_++;
                }
            }
        } else if (!messages.empty()) {
            current_message_index_ = messages.size() - 1;
        } else {
            current_message_index_ = 0;
        }

        for (size_t i = 0; i < end; ++i) {
           const auto& msg = messages[i];
           auto whoColor = (msg.who == game_logic_.getPlayer().getName()) ? Color::Green : Color::Cyan;
           auto glyphs = Utf8ToGlyphs(msg.content);
           size_t contentSize = glyphs.size();
           size_t shownChars = contentSize;

           if (i < current_message_index_) {
               shownChars = contentSize;
           } else if (i == current_message_index_) {
               shownChars = std::min(contentSize, static_cast<size_t>(
                   std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - msg.start_time).count() / 20));
           } else {
               shownChars = 0;
           }

           std::string shownContent;
           for (size_t j = 0; j < shownChars && j < glyphs.size(); ++j) shownContent += glyphs[j];
           if ((i == current_message_index_) && (shownChars < contentSize)) {
              shownContent += "_";
           }
           messageElements.push_back(hbox({
               text(msg.who) | bold | color(whoColor),
               text(": "),
               paragraph(shownContent) | flex
           }));
        }
        if (!messageElements.empty()) messageElements.back() |= focus;
        return window(text(" 对话记录 "), vbox(messageElements) | flex | yframe);
    });

    interactiveMainView_ = CatchEvent(mainViewRenderer, [this](Event e) {
        const auto& messages = game_logic_.getDialog().getHistory();
        int max_offset = static_cast<int>(messages.size()) - 1;
        if (max_offset < 0) max_offset = 0;
        if (e == Event::ArrowUp || e == Event::PageUp) {
            scroll_index_ = std::min(scroll_index_ + (e == Event::PageUp ? 5 : 1), max_offset);
            return true;
        }
        if (e == Event::ArrowDown || e == Event::PageDown) {
            scroll_index_ = std::max(scroll_index_ - (e == Event::PageDown ? 5 : 1), 0);
            return true;
        }
        if (e == Event::End) { scroll_index_ = 0; return true; }
        if (e == Event::Home) { scroll_index_ = max_offset; return true; }
        return false;
    });

    // -- 右侧功能菜单 --
    auto button_phone = Button(" 我的手机 ", []{}, ButtonOption::Animated());
    auto button_settings = Button(" 游戏设置 ", [&] { game_logic_.showGameSettings(); }, ButtonOption::Animated());
    auto button_bag = Button("   背包  ", []{}, ButtonOption::Animated());
    auto button_schedule = Button(" 我的日程 ", []{}, ButtonOption::Animated());
    navigationContainer_ = Container::Vertical({
        button_phone,
        button_settings,
        button_bag,
        button_schedule,
    });

    // -- 输入组件 --
    command_input_component_ = Input(&command_input_str_, "输入指令或对话...", {
        .on_enter = [&] {
            game_logic_.getDialog().processPlayerInput(command_input_str_);
            command_input_str_.clear();
        }
    });

    text_input_component_ = Input(&text_input_str_, "...", {
        .on_enter = [&] {
            auto request_opt = game_logic_.getCurrentInputRequest();
            if (!request_opt) return;
            
            bool rule_matched = false;
            for (const auto& rule : request_opt->rules) {
                if (rule.condition(text_input_str_)) {
                    rule.action(game_logic_, text_input_str_);
                    rule_matched = true;
                    break;
                }
            }
            if (!rule_matched && request_opt->on_text_submit_default) {
                request_opt->on_text_submit_default(game_logic_, text_input_str_);
            }
            text_input_str_.clear();
        }
    });

    choice_container_ = Container::Vertical({});

    inputArea_ = Container::Tab(
        {
            command_input_component_,
            text_input_component_,
            choice_container_,
        },
        &selected_input_mode_
    );

    // 将所有子组件添加到父组件中，确保事件能够正确传递
    Add(Container::Vertical({
        interactiveMainView_,
        navigationContainer_,
        inputArea_,
    }));
}


Element GameLayout::Render() {
    // -- 状态同步：根据Game状态切换Tab的显示，并动态更新内容 --
    GameState state = game_logic_.getCurrentState();
    auto request_opt = game_logic_.getCurrentInputRequest();
    std::string input_prompt = " 指令 ";

    switch(state) {
        case GameState::AwaitingTextInput:
            selected_input_mode_ = 1;
            if (request_opt) input_prompt = request_opt->prompt;
            break;
        case GameState::AwaitingChoice:
            selected_input_mode_ = 2;
            if (request_opt) {
                input_prompt = request_opt->prompt;
                // 动态更新选项按钮
                if (choice_container_->ChildCount() != request_opt->choices.size()) {
                    choice_container_->DetachAllChildren();
                    for (int i = 0; i < request_opt->choices.size(); ++i) {
                         choice_container_->Add(
                             Button(request_opt->choices[i], [this, i, choice = request_opt->choices[i], on_select = request_opt->on_choice_select] {
                                 on_select(i, choice);
                             }, ButtonOption::Animated())
                         );
                    }
                }
            }
            break;
        default: // GameState::InGame 或其他
            selected_input_mode_ = 0;
            break;
    }

    bool show_side_panels = (state == GameState::InGame);

    // -- 布局组装 --
    auto header = hbox({
        text("   拳王之路   ") | bold | color(Color::Red),
        filler(),
        text("当前位置: " + game_logic_.getPlayer().getLocation() + " ") | color(Color::Yellow),
    }) | border;

    Elements left_children;
    left_children.push_back(interactiveMainView_->Render() | vscroll_indicator | yframe | flex);
    if (show_side_panels) {
        left_children.push_back(
            window(text(" 玩家状态 "),
                   hbox({
                       text(" 生命值 ") | color(Color::Green),
                       gauge(game_logic_.getPlayer().getHealth()) | flex | color(Color::Green),
                       separator(),
                       text(" 疲劳值 ") | color(Color::Yellow),
                       gauge(game_logic_.getPlayer().getFatigue()) | flex | color(Color::Yellow),
                       separator(),
                       text(" 饥饿值 ") | color(Color::RedLight),
                       gauge(game_logic_.getPlayer().getHunger()) | flex | color(Color::RedLight),
                   }))
        );
    }
    auto left_panel = vbox(left_children) | flex;

    Element rightPanel = emptyElement();
    if (show_side_panels) {
        rightPanel = window(text(" 功能菜单 "), navigationContainer_->Render()) | size(WIDTH, EQUAL, 22);
    }

    auto mainContent = hbox({ left_panel, rightPanel });
    Element footer = window(text(input_prompt), inputArea_->Render());

    return vbox({ header, mainContent | flex, footer });
}