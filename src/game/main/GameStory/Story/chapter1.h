#ifndef CHAPTER1_H
#define CHAPTER1_H

#include "../DialogRegistry.h"
#include "../Story.h"
#include "../../basic/Game.h"

#include <chrono>
#include <map>
#include <string>

/* 对话id规则：8位数字，如 ABBCCDDD A 为章节数 BB 为章节的某个部分 CC 为场景 id，从 0 开始，指发生在哪个场景的对话
 * 选择id规则：9位数字，如 ABBCCDDDE 前8位同上，E为对话编号，从 0 开始
 * 任务id规则：4位数字，如 AACC AA为主任务ID，CC为子任务ID
 */

/**
 * @namespace Chapter1
 * @brief 包含第一章所有剧情数据的命名空间。
 */
namespace Chapter1 {

    inline DialogNode _00000001(00000001, "<UNKNOWN>", "欢迎！这是你第一次进入这个游戏，来给你自己起个名字吧：");
    
    inline DialogNode _00000002(00000002, "<UNKNOWN>", "<PLAYER_NAME>吗？嗯嗯，确实是个好名字呢，准备好醒来了吗？", {} , 3);
    
    inline const DialogNode _00000003(3, "<SYSTEM>", "说：「是」或「否」\n（选择否将退出游戏！）", {
        Choice("是", 4), // 选择“是”，跳转到节点4
        Choice("否", 5), // 选择“否”，跳转到节点5
    });
    
    inline const DialogNode _00000004(4, "<SYSTEM>", "游戏正式开始！", {}, 6, {
        /**
         * sequence 是一个vector
         * // eg1: 停顿1秒，给玩家反应时间
         * WaitAction{std::chrono::seconds(1)},
         * // eg2: 玩家说话
         * SpeakAction{PLAYER, "我这是……在哪？"},
         * // eg3: 停顿500毫秒
         * WaitAction{std::chrono::milliseconds(500)},
         */
        // 附加动作：清理对话历史
        ExecuteAction{
            [](Game& game) {
            game.getDialog().clearHistory();
            }
        }
    });

    inline const DialogNode _00000005(5, "<SYSTEM>", "下次再见。", {}, 0, {
        ExecuteAction{
            [](Game& game) {
                game.getDialog().clearHistory();
                game.exitGame();
            }
        }}
    );
    
    inline DialogNode _00000006(6, "<UNKNOWN>", "快醒醒！<PLAYER_NAME>！", {}, 0, {
        SpeakAction{"<PLAYER_NAME>", "我这是......在哪？"},
        WaitAction{std::chrono::seconds(3)},
        SpeakAction{"<UNKNOWN>", "你醒了，<PLAYER_NAME>！"},
    });

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