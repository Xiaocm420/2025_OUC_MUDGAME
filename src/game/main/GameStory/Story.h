#pragma once

#include "../basic/Types.h"
#include <string>
#include <utility>
#include <vector>

/**
 * @struct Choice
 * @brief 代表玩家在对话中可以做出的一个选项。
 */
struct Choice {
    /**
     * @brief Choice的构造函数。
     * @param text 选项显示的文本。
     * @param nextNodeID 选择后将跳转到的下一个DialogNode的ID。
     * @param action (可选) 选择该选项后要执行的特定动作。
     */
    Choice(const std::string text, unsigned int nextNodeID, ChoiceAction action = nullptr)
        : text(std::move(text)), nextNodeID(nextNodeID), action(std::move(action)) {}

    std::string text;           ///< 选项的显示文本。
    unsigned int nextNodeID;    ///< 下一个对话节点的ID。
    ChoiceAction action;        ///< 选择后执行的动作。
};


/**
 * @struct DialogNode
 * @brief 代表游戏中的一个原子对话单元。
 */
struct DialogNode {
    DialogNode() = default;

    /**
     * @brief DialogNode的构造函数。
     * @param id 节点的唯一ID。
     * @param who 说话者的名字。
     * @param content 对话的完整内容。
     * @param choices (可选) 玩家可以在此节点做出的选项列表。
     * @param nextNodeID (可选) 如果没有选项，则自动跳转到此ID的节点。
     * @param action (可选) 在显示完此节点内容且无选项时执行的动作。
     */
    DialogNode(const unsigned int id, const std::string& who, const std::string& content,
        const std::vector<Choice>& choices = {}, const unsigned int nextNodeID = 0, ChoiceAction action = nullptr)
        : id(id), who(who), content(content), choices(choices), nextNodeID(nextNodeID), action(std::move(action)) {}

    unsigned int id;                  ///< 节点的唯一ID。
    const std::string who;            ///< 说话者。
    const std::string content;        ///< 对话内容。
    const std::vector<Choice> choices;///< 选项列表。
    const unsigned int nextNodeID;    ///< 线性剧情的下一个节点ID
    const ChoiceAction action;        ///< 节点自身的动作。
};

// 任务状态枚举
enum class QuestStatus {
    Inactive,
    Active,
    Completed,
    Failed,
};

// 任务
struct Quest {
    unsigned int id;                      // 任务id
    std::string name;                     // 任务名
    std::string description;              // 任务描述
    QuestStatus status = QuestStatus::Inactive;
    unsigned int currentStage = 0;        // 任务的当前阶段，用于多步骤任务
};

// 场景、地点
struct Scene {
    std::string id;                       // 场景 id
    std::string name;                     // 场景名字， e.g., "格斗俱乐部"
    std::string description;              // 进入场景时的环境描述
    unsigned int entryDialogNodeID = 0;   // 进入场景时自动触发的对话节点ID
};