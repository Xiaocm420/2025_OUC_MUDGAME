#ifndef INC_2025_OUC_MUDGAME_GAMEPROCESS_H
#define INC_2025_OUC_MUDGAME_GAMEPROCESS_H

#include "../basic/Game.h"
#include "../basic/InputProcess.h"
#include "Story/chapter1.h"

/**
 * @class GameProcess
 * @brief 处理特定游戏流程的静态工具类，如游戏开场。
 */
class GameProcess {
public:
    /**
     * @brief 处理新游戏的开场流程。
     * @param game 对Game核心对象的引用。
     */
    static void newStart(Game& game) {
        // 显示第一句引导语
        game.getStoryController().processNodeByID(1);
        
        // 请求玩家输入名字 also a example of requestTextInput()
        game.requestTextInput(
            // 参数1: prompt - 输入提示
            "输入你的名字:",
            // 参数2: rules - 一个规则列表，用于处理特殊输入
            {
                // 可以留空，或者添加规则，例如：
                {
                    // condition: 输入 "God"
                    .condition = [](std::string_view input){ return trim(std::string(input)) == "God"; },
                    // action: 给予特殊回应，并重新请求输入
                    .action = [](Game& g, std::string_view input){
                        g.getDialog().addMessage("系统", "这个名字过于强大，凡人无法使用。");
                        newStart(g); // 重新发起 newStart 流程
                    }
                }
            },
            // 参数3: on_submit_default - 默认动作 (当所有规则都不匹配时执行)
            [](Game& g, std::string_view input) {
                std::string name = trim(std::string(input));
                // 进行名字有效性验证
                if (name.empty() || name.length() > 8 || name.length() < 2) {
                    g.getDialog().addMessage(UNKNOWN, "名字不合适，请重新输入一个2-8个字的名字。");
                    newStart(g);
                    return;
                }
                
                g.getPlayer().setName(name);
                PLAYER = name; // 更新全局玩家名
                
                // 名字输入成功后，继续剧情
                g.getStoryController().startStory(2);
            }
        );
        
        // 步骤 4: 启动游戏主界面和循环
        game.getView().showGameScreen();
    }
};

#endif //INC_2025_OUC_MUDGAME_GAMEPROCESS_H