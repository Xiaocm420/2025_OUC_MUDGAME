#ifndef INC_2025_OUC_MUDGAME_PHONELAYOUT_H
#define INC_2025_OUC_MUDGAME_PHONELAYOUT_H

#include "FTXUI/component/component_base.hpp"
#include "FTXUI/component/component.hpp"

class Game; // 前向声明

/**
 * @class PhoneLayout
 * @brief 实现了手机界面的FTXUI组件。
 * @details 包含应用图标和退出按钮，作为一个全屏覆盖层显示。
 */
class PhoneLayout : public ftxui::ComponentBase {
public:
    explicit PhoneLayout(Game& game_logic, std::function<void()> onMapClick);

    ftxui::Element Render() override;

    // 控制显示和隐藏的方法
    void show();
    void hide();
    bool isShowing() const;

private:
    Game& game_logic_;
    bool isShowing_ = false;

    // --- 子组件成员 ---
    ftxui::Component buttonMap_;
    ftxui::Component buttonShop_;
    ftxui::Component buttonInfo_;
    ftxui::Component buttonHome_;
    ftxui::Component mainContainer_;

    std::function<void()> on_map_click_; // 地图按钮
};

#endif //INC_2025_OUC_MUDGAME_PHONELAYOUT_H