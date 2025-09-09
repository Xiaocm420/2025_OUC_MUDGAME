#ifndef CHAPTER1_H
#define CHAPTER1_H

#include "../DialogRegistry.h"
#include "../Story.h"
#include "../../basic/Game.h"

#include <string>
#include <map>

/* 对话id规则：8位数字，如 ABBCCDDD A 为章节数 BB 为章节的某个部分 CC 为场景 id，从 0 开始，指发生在哪个场景的对话
 * 选择id规则：9位数字，如 ABBCCDDDE 前8位同上，E为对话编号，从 0 开始
 * 任务id规则：4位数字，如 AACC AA为主任务ID，CC为子任务ID
 */

/**
 * @namespace Chapter1
 * @brief 包含第一章所有剧情数据的命名空间。
 */
namespace Chapter1 {

    inline DialogNode _00000001(00000001, SYSTEM, "欢迎！这是你第一次进入这个游戏，来给你自己起个名字吧：");
    
    inline DialogNode _00000002(00000002, SYSTEM, "<PLAYER_NAME>吗？嗯嗯，确实是个好名字呢，准备好醒来了吗？", {} , 3);
    
    inline const DialogNode _00000003(3, SYSTEM, "说：「是」或「否」\n（选择否将退出游戏！）", {
        Choice("是", 4), // 选择“是”，跳转到节点4
        Choice("否", 5), // 选择“否”，跳转到节点5
    });
    
    inline const DialogNode _00000004(4, SYSTEM, "游戏正式开始！", {}, 6, 
        // 附加动作：清理对话历史
        [](Game& game) {
            game.getDialog().clearHistory();
            game.getPlayer().setLocation("拳击馆外");
        }
    );

    inline const DialogNode _00000005(5, SYSTEM, "下次再见。", {}, 0,
        [](Game& game) {
            game.getDialog().clearHistory();
            game.exitGame();
        }
    );
    
    inline DialogNode _00000006(6, "", "我从小有一个梦想，就是当上拳王", {}, 0,
        [](Game& game) {
            game.getDialog().addMessage("", "我呆呆的看着拳击馆里不断挥拳的几个正在训练的拳击手，心中羡慕不已");
            game.getDialog().addMessage(UNKNOWN, "嘿，小伙子，你看什么呢");
            game.getDialog().addMessage(game.getPlayer().getName(), "我...我也想练拳...只可惜我的财力不足，不足以支撑我学这个...");
            game.getDialog().addMessage("", "他看着我失望的神情，不由动了容，他上下打量着我的身体，心里嘀咕着什么...");
            game.getDialog().addMessage("???", "我看你身材不错嘛，蛮结实的，你以前是做什么工作的？");
            game.getDialog().addMessage(game.getPlayer().getName(), "以前当过几年小兵，期间什么荣誉都没得到，也就身体好一些");
            game.getDialog().addMessage("???", "我看你体格不错，看上去有些打拳天赋，想在拳击界里混出个名堂吗？");
            game.getDialog().addMessage("弗兰克", "你可以叫我弗兰克，这里的万事通。我看你骨骼精奇，就来我们这里练拳吧");
            game.getDialog().addMessage(game.getPlayer().getName(), "可...我并没有钱呀");
            game.getDialog().addMessage("弗兰克", "那有什么，这点小钱，我帮你垫了，这是我的名片，以后随时联系");
            game.getDialog().addMessage("", "他从兜里掏出一张名片，居然是金色的，同时，他又掏出了一张卡片");
            game.getDialog().addMessage("弗兰克", "这是这家拳击馆的通行证，明天记得带上，不然你进不去");
            game.getDialog().addMessage("", "说完，他便以有事为由，搭上一辆计程车走了，留下我一个人在原地");
            game.getDialog().addMessage(game.getPlayer().getName(), "嗯...");
        }
    );

    /**
     * @brief 将本章节的所有对话节点注册到数据库中。
     * @param db 对话数据库的引用。
     */
    inline void registerAllDialogs(std::map<unsigned int, const DialogNode*>& db) {
        db[1] = &_00000001;
        db[2] = &_00000002;
        db[3] = &_00000003;
        db[4] = &_00000004;
        db[5] = &_00000005;
        db[6] = &_00000006;
    }

    /// @brief 创建一个静态的注册器实例，以实现自动注册。
    inline DialogRegistrar registrar(registerAllDialogs);

} // namespace Chapter1

#endif // CHAPTER1_H