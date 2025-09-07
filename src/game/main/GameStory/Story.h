#pragma once

#include <string>
#include <vector>

struct DialogNode;

// 选项结构体
struct Choice {
    Choice(const std::string text, const unsigned int nextNodeID, const unsigned int questID=0,
        const unsigned int questStage=0) :
    text(text), nextNodeID(nextNodeID), questID(questID), questStage(questStage) {}

    std::string text;                          // 选项显示的文本，例如 [是] [否]
    unsigned int nextNodeID;                   // 点击后跳转到的下一个对话节点的ID
    unsigned int questID;                  // (可选) 点击后触发的 quest ID
    unsigned int questStage;               // (可选) 将 quest 设置到哪个阶段
};

// 对话结构体
struct DialogNode {
    DialogNode() = default;

    DialogNode(const unsigned int id = 0, const std::string& who = "", const char* content = "",
        const std::vector<Choice>& choices={}, const DialogNode *next=nullptr) :
        id(id), who(who), content(content), choices(choices), nextDialogNode(next) {
    };

    unsigned int id;                             // 独一无二的节点ID
    const std::string who;                       // 说话者的名字 (e.g., "教练", "系统", "你")
    const std::string content;                   // 对话内容
    const std::vector<Choice> choices;           // 玩家可以做出的选项 (如果没有选项，则为线性对话)
    const DialogNode *nextDialogNode;            // 下一个对话
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