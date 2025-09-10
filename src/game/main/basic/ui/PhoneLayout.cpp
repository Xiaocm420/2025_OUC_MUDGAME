#include "PhoneLayout.h"
#include "../Game.h" // 假设 Game.h 的路径
#include "FTXUI/dom/elements.hpp"
#include "FTXUI/component/component.hpp" // 引入 component.hpp for ButtonOption
#include <chrono>
#include <ctime>
#include <iomanip>
#include "../GameTime.h"

using namespace ftxui;

// --- 辅助函数：创建应用图标 ---
// 为了代码复用和整洁，我们创建一个函数专门生成图标按钮的样式
Component createAppButton(const std::string& icon, const std::string& label, std::function<void()> onClick) {
    auto option = ButtonOption();
    option.transform = [=](const EntryState& s) {
        // 图标和文字的垂直布局
        auto content = vbox({
            text(icon) | hcenter | bold,
            text(" "),
            text(label) | hcenter,
        }) | center | flex;

        // 当按钮被激活或聚焦时，提供视觉反馈
        if (s.active) {
            content |= inverted;
        }
        if (s.focused) {
            content |= bold;
        }

        // 定义图标的尺寸和边框
        return content | size(WIDTH, EQUAL, 12) | size(HEIGHT, EQUAL, 5) | border;
    };

    return Button("", onClick, option);
}

/**
 * @brief PhoneLayout的构造函数。
 * @details 在此初始化所有UI组件，并将它们添加到容器中以处理事件。
 */
PhoneLayout::PhoneLayout(Game& game_logic, std::function<void()> onMapClick)
    : game_logic_(game_logic), on_map_click_(onMapClick) {

    // 应用按钮
    buttonMap_ = createAppButton(" 🗺 ", "地图", on_map_click_);
    buttonShop_ = createAppButton("🛒", "网购平台", [] { /* TODO: 网购逻辑 */ });
    buttonInfo_ = createAppButton("👤", "我的信息", [] { /* TODO: 信息逻辑 */ });

    // Home键 (退出按钮)
    buttonHome_ = Button(" ○ ", [this] { hide(); }, ButtonOption::Ascii());

    // 将所有按钮添加到一个容器中，这是确保它们能交互的关键
    mainContainer_ = Container::Horizontal({ // 使用Horizontal因为应用图标是横向排列的
        buttonMap_,
        buttonShop_,
        buttonInfo_,
        // Home键是独立的，但为了事件处理也需要加入容器
        Container::Vertical({buttonHome_}),
    });

    // 4. 将主容器作为PhoneLayout的子组件
    Add(mainContainer_);
}

/**
 * @brief 渲染函数，负责绘制手机界面。
 */
Element PhoneLayout::Render() {
    // 如果不显示，则渲染一个空元素
    if (!isShowing_) {
        return emptyElement();
    }

    // --- 模拟手机状态栏 ---
    // 获取游戏内时间
    unsigned int gameHour = GameTime::getHour();
    unsigned int gameMinute = GameTime::getMinute();

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << gameHour << ":"
       << std::setw(2) << std::setfill('0') << gameMinute;
    std::string timeStr = ss.str();

    auto statusBar = hbox({
        text(" " + timeStr),
        filler(),
        text("📶 5G  🔋 98%") | color(Color::Green),
        text(" "),
    });

    // --- 应用图标网格 ---
    // 将三个应用按钮水平排列，并用filler隔开
    auto appGrid = hbox({
        filler(),
        buttonMap_->Render(),
        filler(),
        buttonShop_->Render(),
        filler(),
        buttonInfo_->Render(),
        filler(),
    }) | center;

    // --- 手机屏幕内容 ---
    auto screenContent = vbox({
        statusBar,
        separator(),
        text(" ") | size(HEIGHT, EQUAL, 2), // 顶部留白
        appGrid,
        filler(), // 主要的弹性空间，将Home键推到底部
        buttonHome_->Render() | hcenter,
        text(" "), // 底部留白
    });

    // --- 手机物理外形 ---
    auto phoneBody = screenContent |
                      borderRounded | // 圆角边框
                      color(Color::Default) | bgcolor(Color::Black);

    // --- 最终布局 ---
    // 将手机居中显示，并使用 clear_under 实现全屏覆盖效果
    return vbox({
        filler(),
        hbox({
            filler(),
            phoneBody | size(WIDTH, LESS_THAN, 60) | size(HEIGHT, EQUAL, 28),
            filler(),
        }),
        filler(),
    }) | clear_under;
}

void PhoneLayout::show() {
    isShowing_ = true;
}

void PhoneLayout::hide() {
    isShowing_ = false;
}

bool PhoneLayout::isShowing() const {
    return isShowing_;
}