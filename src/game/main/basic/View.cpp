#include "View.h"
#include "Game.h"

#include "FTXUI/component/component.hpp"
#include "FTXUI/component/screen_interactive.hpp"
#include "FTXUI/dom/elements.hpp"

#include <chrono>
#include <random>
#include <thread>
#include <vector>

View::View(Game& game_logic) : game_logic_(game_logic) {}

void View::ShowMainMenu() {
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
            hbox(text("使用方向键上下移动，Enter键选择") | color(Color::Blue) | center) | flex
        }) | border | flex;
    });

    screen.Loop(component);
}

void View::ShowLoadingScreen(const std::string& subtitle) {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();
    float progress = 0.0f;

    auto component = Renderer([&] {
        return vbox({
            text("Loading") | color(Color::Green) | bold | center,
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