#include "GameLayout.h"
#include "../Game.h"
#include "../Dialog.h"
#include "../../class/entity/Player.h"

#include "FTXUI/component/screen_interactive.hpp"
#include "FTXUI/dom/elements.hpp"
#include "FTXUI/screen/string.hpp"

using namespace ftxui;

/**
 * @brief GameLayout的构造函数。
 * @details 在此初始化所有UI组件，并设置它们的交互逻辑。
 *          关键在于，所有组件都作为成员变量，保证了其生命周期与GameLayout实例一致。
 */
GameLayout::GameLayout(Game& game_logic) : game_logic_(game_logic), 
                                           animation_start_time_(std::chrono::steady_clock::now()) {

    // -- 对话历史渲染器 --
    auto mainViewRenderer = Renderer([this] {
        const auto& messages = game_logic_.getDialog().getHistory();

        // -- 动画状态重置逻辑 --
        // 检查数据源是否已被重置，如果是，则同步UI的动画状态。
        if (game_logic_.getDialog().historyWasClearedAndConsume()) {
            if (!messages.empty()) {
                // 如果历史记录被清空后又有了新内容，则从头开始播放动画。
                current_message_index_ = 0;
                animation_start_time_ = std::chrono::steady_clock::now();
            } else {
                // 如果历史记录被清空后是空的，则只重置索引。
                current_message_index_ = 0;
            }
        }

        // -- 动画状态推进逻辑 --
        // 只有当有消息且当前动画索引有效时才进行判断
        if (!messages.empty() && current_message_index_ < messages.size()) {
            const auto& current_msg = messages[current_message_index_];
            auto content_size = Utf8ToGlyphs(current_msg.content).size();
            
            // 计算完成当前消息动画所需的总时间（打字时间 + 结束后延迟）
            auto typing_duration = std::chrono::milliseconds(content_size * 20);
            auto post_delay = std::chrono::milliseconds(500);
            auto total_animation_time = typing_duration + post_delay;

            // 检查自当前动画开始以来，是否已经过了所需的总时间
            if (std::chrono::steady_clock::now() - animation_start_time_ > total_animation_time) {
                // 如果动画已结束，并且后面还有更多消息，则准备播放下一条
                if (current_message_index_ < messages.size() - 1) {
                    current_message_index_++;
                    // 为下一条消息的动画重置计时器
                    animation_start_time_ = std::chrono::steady_clock::now();
                }
            }
        } else if (!messages.empty()) {
            // 如果消息列表不为空，但索引越界，则修正索引到最后一条消息
            current_message_index_ = messages.size() - 1;
        }

        // -- 渲染逻辑 --
        Elements messageElements;
        size_t total = messages.size();
        size_t end = total > static_cast<size_t>(scroll_index_) ? total - scroll_index_ : 0;
        
        for (size_t i = 0; i < end; ++i) {
            // 只处理和渲染那些已经播放完毕或正在播放的消息。
            // 完全跳过未来的消息，避免它们的 "who" 提前显示。
            if (i > current_message_index_) {
                continue; // 跳过此循环的剩余部分
            }

            const auto& msg = messages[i];
            auto whoColor = (msg.who == game_logic_.getPlayer().getName()) ? Color::Green : Color::Cyan;
            auto glyphs = Utf8ToGlyphs(msg.content);
            size_t contentSize = glyphs.size();
            size_t shownChars = 0;

            if (i < current_message_index_) {
                // 已播放完毕的消息，完整显示
                shownChars = contentSize;
            } else { // 循环开头已有 i > current_message_index_ 判断，故此处必为 i == current_message_index_
                // 正在播放的当前消息，根据独立的动画计时器计算显示长度
                auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - animation_start_time_
                ).count();
                shownChars = std::min(contentSize, static_cast<size_t>(elapsedMs / 20));
            }
            // (i > current_message_index_) 的未来消息，shownChars 保持为0，不显示

            std::string shownContent;
            for (size_t j = 0; j < shownChars && j < glyphs.size(); ++j) {
                shownContent += glyphs[j];
            }
            if ((i == current_message_index_) && (shownChars < contentSize)) {
               shownContent += "_";
            }
            messageElements.push_back(hbox({
                text(msg.who) | bold | color(whoColor),
                text(": "),
                paragraph(shownContent) | flex
            }));
        }
        if (!messageElements.empty()) {
            messageElements.back() |= focus;
        }
        
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


/**
 * @brief 渲染函数，FTXUI每一帧都会调用。
 * @details 负责根据当前游戏状态同步UI，并组合所有子组件来构建最终的界面布局。
 */
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