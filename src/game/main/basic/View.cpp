#include "View.h"
#include "Game.h"
#include "Dialog.h"
#include "../class/entity/Player.h"

#include "FTXUI/component/component.hpp"
#include "FTXUI/component/screen_interactive.hpp"
#include "FTXUI/dom/elements.hpp"
#include "FTXUI/screen/string.hpp"

#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include <atomic>

View::View(Game& game_logic) : game_logic_(game_logic) {}

void View::showMainMenu() {
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    std::vector<std::string> menuItems = {
        "开始新游戏", "读取存档", "游戏介绍", "游戏设置", "退出游戏"
    };

    // 按钮的回调函数现在通过 game_logic_ 引用来调用 Game 类的方法
    auto menu = Container::Vertical({
        Button(menuItems[0], [&] { game_logic_.startNewGame(); }, ButtonOption::Animated()),
        Button(menuItems[1], [&] { game_logic_.loadGame(); }, ButtonOption::Animated()),
        Button(menuItems[2], [&] { game_logic_.showGameIntro(); }, ButtonOption::Animated()),
        Button(menuItems[3], [&] { game_logic_.showGameSettings(); }, ButtonOption::Animated()),
        Button(menuItems[4], [&] { game_logic_.exitGame(); screen.ExitLoopClosure()(); }, ButtonOption::Animated())
    });

    auto component = Renderer(menu, [&] {
        return vbox({
            vbox({
                text("    :=   #%-  :.                                 :%*           .::.::.   -%*      ") | color(Color::Red),
                text("    -@+ -@%  *@+      *@@@@@@@@@@@@@@@#           #@%.         *@##*@@.  %@%###*. ") | color(Color::Red),
                text("  @@@@@@@@@@@@@@@#           +@%           ********@#*****-    *@:  @@.:@@+::#@#  ") | color(Color::Red),
                text("      :%@=                   +@@           ############@@@*    *@@@@@@#@@@@-=@%   ") | color(Color::Red),
                text(".###%@@@####@@@####          +@%                      #@@-     .::@@::    +@@*    ") | color(Color::Red),
                text("  :*@@%-=+**+#@%-      #@@@@@@@@@@@@@@              +@@+       +# @@.   -%@%#@@+. ") | color(Color::Red),
                text("=@@%= -:=@%    #@@@=         +@%                  =@@#         #@ @@@@@@@#:  .#@%.") | color(Color::Red),
                text("    @@@@@@@@@@@@             +@@                %@@*           #@ @@   .@@@@@@@@: ") | color(Color::Red),
                text("  ####%#%@@##%####           +@%            +#@@@=             #@ @@+*+.@@    @@: ") | color(Color::Red),
                text("        :@%          #@@@@@@@@@@@@@@@@@%  *@@++%@%*=====+++*+ *@@@@@#*=.@@++=+@@: ") | color(Color::Red),
                text("      #@@@+          :::::::::::::::::::   *     -*@@@@@%@%@. .:        @@==+=@@. ") | color(Color::Red)
            }) | center | border,
            separator(),
            menu->Render() | flex | center,
            separator(),
            hbox(text("使用方向键上下移动，Enter键选择") | color(Color::Blue) | center) | flex,
            hbox(text("游戏中请尽量不要命令行界面大小, 全屏游玩体验最佳") | color(Color::Blue) | center) | flex,
        }) | border | flex;
    });

    screen.Loop(component);
}

void View::showGameIntroScreen() {
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    auto exitButtion = Button(" 退 出 ", screen.ExitLoopClosure(), ButtonOption::Animated());

    auto gameIntro = vbox({
        text("《拳王之路》—— 你的拳头，决定你的天下！") | bold | color(Color::RedLight) | center,
        separatorEmpty(),

        hflow(
            paragraph("《拳王之路》是一款融合了硬核格斗、深度经营与角色养成的多元化角色扮演游戏。"),
            paragraph("在这里，你并非只是一个只会出拳的机器。你将扮演一位怀揣拳王梦想的格斗新星，"),
            paragraph("从零开始，全面主宰自己的职业生涯")
            ),
        separatorEmpty(),

        vbox({
            text("• 作为运动员：") | bold | color(Color::Cyan),
            hflow(
                paragraph("你需要在健身房里挥汗如雨，进行刻苦训练，提升力量、速度、耐力。"),
                paragraph("学习全新的拳法组合，研究对手的弱点，制定专属战术，在擂台上用实力击败每一个敌人。")
                ),
        }),
        separatorEmpty(),

        vbox({
            text("• 作为管理者：") | bold | color(Color::Cyan),
            hflow(
                paragraph("你必须精明地经营自己的事业。合理安排时间平衡好训练和打工收入，"),
                paragraph("管理个人收支。每一步决策都至关重要，都影响着你的发展轨迹。")
                ),
        }),
        separatorEmpty(),

        vbox({
            text("• 作为追梦者：") | bold | color(Color::Cyan),
            hflow(
                paragraph("你将从籍籍无名的俱乐部底层开始打拼，通过一场场胜利积累声望，"),
                paragraph("最终在世界级的格斗殿堂中争夺至高无上的「拳王」金腰带。")
                ),
        }),
        separatorEmpty(),

        hflow(
            paragraph("然而，通往巅峰的道路绝非一帆风顺。你需要把握转瞬即逝的机遇，"),
            paragraph("应对突如其来的伤病与强大的对手。你的智慧、毅力与抉择，将共同谱写属于你的传奇。")
            ),
        separatorEmpty(),
        separator(),

        vbox({
            text("游戏基础操作介绍：") | bold | color(Color::Yellow),
            text("1. 游戏内使用输入 /help 获取命令提示"),
            text("2. 在遇到需要玩家自由操纵人物的场景，使用 wasd 键以移动人物"),
            text("3. 使用键盘 tab 键切换按钮或输入框，或者直接使用鼠标光标选取"),
        }) | border,
    });

    auto renderer = Renderer(exitButtion, [&] {
        return vbox({
                   text("游 戏 介 绍") | bold | color(Color::RedLight) | center,
                   separator(),
                   gameIntro | vscroll_indicator | yframe | flex ,
                   separator(),
                   exitButtion->Render() | center,
               }) |
               size(WIDTH, LESS_THAN, 120) | size(HEIGHT, LESS_THAN, 35) | border | center;
    });

    screen.Loop(renderer);
}

void View::showLoadingScreen(const std::string& subtitle) {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();
    float progress = 0.0f;

    auto component = Renderer([&] {
        return vbox({
            text("加 载 中") | color(Color::Green) | bold | center,
            text(subtitle) | color(Color::White) | center,
            separator(),
            gauge(progress) | flex | color(Color::Green)
        }) | border | size(WIDTH, EQUAL, 60) | size(HEIGHT, EQUAL, 8) | center;
    });

    std::thread loadingThread([&] {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(2000, 5000);
        int total_duration_ms = distrib(gen);

        int total_steps = 100;
        int sleep_per_step_ms = total_duration_ms / total_steps;

        for (int i = 0; i < total_steps; ++i) {
            float linear_time = static_cast<float>(i + 1) / total_steps;
            progress = 1.0f - (1.0f - linear_time) * (1.0f - linear_time);

            screen.PostEvent(Event::Custom);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_per_step_ms));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        screen.ExitLoopClosure()();
    });

    screen.Loop(component);
    loadingThread.join();
}


void View::showGameScreen() {
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    // 使用通用布局构建
    auto component = makeGameLayout(
            true,
            true,
            true,
            false,
            "格斗俱乐部", // TODO: 接入API
            PLAYER,
            "输入指令或对话..."
    );

    // 每20ms打印一个字
    std::atomic<bool> refresh_running{true};
    std::thread refresh_thread([&] {
        while (refresh_running) {
            screen.PostEvent(Event::Custom);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    });

    screen.Loop(component);

    refresh_running = false;
    if (refresh_thread.joinable()) refresh_thread.join();
}

ftxui::Component View::makeGameLayout(
    bool showRightButtons,
    bool showStatusBar,
    bool allowInput,
    bool isVoiceOver,
    const std::string& playerLocation,
    const std::string& who,
    const std::string& inputPlaceholder,
     std::optional<ftxui::Component> externalInput) {

    using namespace ftxui;

    // 输入组件
    Component inputCommand;
    if (externalInput) {
        // 如果外部提供了输入组件，就用它
        inputCommand = *externalInput;
    } else {
        // 否则，使用内部默认的输入组件
        static std::string command_input;
        InputOption option;
        if (allowInput) {
            option.on_enter = [&] {
                game_logic_.getDialog().processPlayerInput(command_input);
                command_input.clear();
            };
        }
        inputCommand = Input(&command_input, inputPlaceholder, option);
    }

    // 右侧按钮组件
    auto button_phone = Button(" 我的手机 ", [&] { /* ... */ }, ButtonOption::Animated());
    auto button_settings = Button(" 游戏设置 ", [&] { game_logic_.showGameSettings(); }, ButtonOption::Animated());
    auto button_bag = Button("   背包  ", [&] { /* ... */ }, ButtonOption::Animated());
    auto button_schedule = Button(" 我的日程 ", [&] { /* ... */ }, ButtonOption::Animated());
    auto navigationContainer = Container::Vertical({
        button_phone,
        button_settings,
        button_bag,
        button_schedule,
    });

    static int scroll_index = 0;
    // 用于跟踪当前应该显示的消息索引
    static size_t current_message_index = 0;

    auto mainViewRenderer = Renderer([&, isVoiceOver] {
        // 代码"市中心" :(
        const auto& messages = this->game_logic_.getDialog().getHistory();
        Elements messageElements;
        size_t total = messages.size();
        size_t end = total > static_cast<size_t>(scroll_index) ? total - scroll_index : 0;

        // 更新当前可显示的消息索引
        if (!messages.empty() && current_message_index < messages.size()) {
            const auto& current_msg = messages[current_message_index];
            auto now = std::chrono::steady_clock::now();
            auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - current_msg.start_time).count();
            size_t contentSize = Utf8ToGlyphs(current_msg.content).size();
            size_t shownChars = std::min(contentSize, static_cast<size_t>(elapsedMs / 20));

            // 如果当前消息已经完全显示，且有下一条消息，则更新索引
            if (shownChars >= contentSize && current_message_index < messages.size() - 1) {
                // 轻微延迟后再显示下一条消息
                if (elapsedMs > contentSize * 20 + 500) { // 500ms延迟
                    current_message_index++;
                }
            }
        } else if (!messages.empty()) {
            current_message_index = messages.size() - 1;
        } else {
            current_message_index = 0;
        }

        for (size_t i = 0; i < end; ++i) {
           const auto& msg = messages[i];
           auto whoColor = (msg.who == who) ? Color::Green : Color::Cyan;
           auto glyphs = Utf8ToGlyphs(msg.content); // 防止出字的时候中文乱码
           size_t contentSize = glyphs.size();
           size_t shownChars = contentSize;
           auto now = std::chrono::steady_clock::now();
           auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - msg.start_time).count();

           // 如果是当前消息或之前的消息，完全显示
           if (i < current_message_index) {
               shownChars = contentSize;
           } else if (i == current_message_index) {
               // 当前消息按时间进度显示
               shownChars = std::min(contentSize, static_cast<size_t>(elapsedMs / 20)); // 出字速度 20ms/字
           } else {
               // 未来的消息不显示
               shownChars = 0;
           }

           std::string shownContent;
           for (size_t j = 0; j < shownChars && j < glyphs.size(); ++j) shownContent += glyphs[j];
           bool typing = (i == current_message_index) && (shownChars < contentSize);
           if (typing) shownContent += "_";
           messageElements.push_back(hbox({
               text(msg.who) | bold | color(whoColor),
               text(isVoiceOver ? "" :  ": "),
               paragraph(shownContent) | flex
           }));
        }
        if (!messageElements.empty()) messageElements.back() |= focus;
        return window(text(" 对话记录 "), vbox(messageElements) | flex | yframe); // 移除 vscroll_indicator，因为它在外部
    });

    // 事件处理器 (CatchEvent 是一个 Component)
    auto interactiveMainView = CatchEvent(mainViewRenderer, [&](Event e) {
        const auto& messages = game_logic_.getDialog().getHistory();
        int max_offset = static_cast<int>(messages.size()) - 1; // 偏移量最多是size-1
        if (max_offset < 0) max_offset = 0;
        if (e == Event::ArrowUp || e == Event::PageUp) {
            scroll_index = std::min(scroll_index + (e == Event::PageUp ? 5 : 1), max_offset);
            return true;
        }
        if (e == Event::ArrowDown || e == Event::PageDown) {
            scroll_index = std::max(scroll_index - (e == Event::PageDown ? 5 : 1), 0);
            return true;
        }
        if (e == Event::End) { scroll_index = 0; return true; }
        if (e == Event::Home) { scroll_index = max_offset; return true; }
        return false;
    });

    // 组件聚合，可交互组件
    auto mainContainer = Container::Vertical({
        interactiveMainView,
        navigationContainer,
        inputCommand,
    });


    // 渲染器
    return Renderer(mainContainer, [=] {
        // Header
        auto header = hbox({
            text("   拳王之路   ") | bold | color(Color::Red),
            filler(),
            text("当前位置: " + playerLocation + " ") | color(Color::Yellow),
        }) | border;

        // 左侧主区域
        Elements left_children;
        // 每次渲染都调用 .Render() 来获取最新画面
        left_children.push_back(interactiveMainView->Render() | vscroll_indicator | yframe | flex);
        if (showStatusBar) {
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

        // 右侧面板
        Element rightPanel = emptyElement();
        if (showRightButtons) {
            // 每次渲染都调用 .Render()
            rightPanel = window(text(" 功能菜单 "), navigationContainer->Render()) | size(WIDTH, EQUAL, 22);
        }

        // 中间主体
        auto mainContent = hbox({ left_panel, rightPanel });

        // 底部 Footer
        Element footer = emptyElement();
        if(allowInput) {
            // 每次渲染都调用 .Render()
            footer = window(text(" 指令 "), inputCommand->Render());
        }

        // 最终返回完整的、动态生成的布局
        return vbox({ header, mainContent | flex, footer });
    });
}