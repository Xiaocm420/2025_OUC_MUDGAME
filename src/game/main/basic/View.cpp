#include "View.h"
#include "Game.h"
#include "Dialog.h"
#include "../class/entity/Player.h"

#include "FTXUI/component/component.hpp"
#include "FTXUI/component/screen_interactive.hpp"
#include "FTXUI/dom/elements.hpp"

#include <chrono>
#include <random>
#include <thread>
#include <vector>

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

    // 游戏状态数据
    std::string player_location = "格斗俱乐部"; // TODO: 应当接入 API
    std::string command_input;

    auto button_phone = Button(" 我的手机 ", [&] { /* ... */ }, ButtonOption::Animated());
    auto button_settings = Button(" 游戏设置 ", [&] { game_logic_.showGameSettings(); }, ButtonOption::Animated());
    auto button_bag = Button("   背包  ", [&] { /* ... */ }, ButtonOption::Animated());
    auto button_schedule = Button(" 我的日程 ", [&] { /* ... */ }, ButtonOption::Animated());


    auto navigation_container = Container::Vertical({
        button_phone,
        button_settings,
        button_bag,
        button_schedule,
    });

    InputOption option;
    option.on_enter = [&] {
        game_logic_.getDialog().processPlayerInput(command_input);
        command_input.clear();
    };
    auto input_command = Input(&command_input, "输入指令或对话...", option);

    auto main_view_renderer = Renderer([&] {
        const auto& messages = game_logic_.getDialog().getHistory();
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
    auto interactive_main_view = CatchEvent(main_view_renderer, [](Event) { return false; });

    // --- 容器管理 ---
    auto main_container = Container::Vertical({
        interactive_main_view,
        navigation_container,
        input_command,
    });

    // --- 最终渲染器 ---
    auto final_renderer = Renderer(main_container, [&] {
        // -- 顶部 --
        auto header = hbox({
            text("   拳王之路   ") | bold | color(Color::Red),
            filler(),
            text("当前位置: " + player_location + " ") | color(Color::Yellow),
        }) | border;

        // -- 中间主区域 --
        auto main_content = hbox({
            // 左侧：主视窗和状态栏
            vbox({
                // [修复] 将滚动条装饰器应用在 Component 的渲染结果上
                interactive_main_view->Render() | vscroll_indicator | yframe | flex,

                // 状态栏
                window(text(" 玩家状态 "),
                    hbox({
                        text(" ♥ 生命值 ") | color(Color::Green),
                        gauge(game_logic_.getPlayer().getHealth()) | flex | color(Color::Green),
                        separator(),
                        text(" ⚡ 疲劳值 ") | color(Color::Yellow),
                        gauge(game_logic_.getPlayer().getFatigue()) | flex | color(Color::Yellow),
                        separator(),
                        text(" 🍗 饥饿值 ") | color(Color::RedLight),
                        gauge(game_logic_.getPlayer().getHunger()) | flex | color(Color::RedLight),
                    })
                )
            }) | flex,

            // 右侧：导航面板
            window(text(" 功能菜单 "), navigation_container->Render()) | size(WIDTH, EQUAL, 22), // 稍微加宽以适应字符画
        });

        // -- 底部 --
        auto footer = window(text(" 指令 "), input_command->Render());

        // -- 整体布局 --
        return vbox({
            header,
            main_content | flex,
            footer,
        });
    });

    screen.Loop(final_renderer);
}
