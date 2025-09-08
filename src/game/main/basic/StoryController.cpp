#include "Dialog.h"
#include "Game.h"
#include "StoryController.h"
#include "../GameStory/DialogRegistry.h"
#include "../GameStory/Story.h"
#include <string>

StoryController::StoryController(Game& game) : game_(game) {
    registerDialogs();
}

void StoryController::registerDialogs() {
    DialogRegistry::runAll(dialog_database_);
}

void StoryController::startStory(unsigned int dialogNodeID) {
    processNodeByID(dialogNodeID);
}

void StoryController::processNodeByID(unsigned int dialogNodeID) {
    // ID为0通常被视为剧情流的结束标志
    if (dialogNodeID == 0) return;

    auto it = dialog_database_.find(dialogNodeID);
    if (it != dialog_database_.end()) {
        processNode(it->second);
    } else {
        // 在此可以添加日志记录，表示未找到指定的对话ID
    }
}

void StoryController::processNode(const DialogNode* node) {
    if (!node) return;

    // --- before: 占位符替换逻辑 ---
    std::vector<std::string> processed = { node->who, node->content} ;
    for (auto& str : processed) {
        // <PLAYER_NAME> 占位符
        std::string placeholder_1 = "<PLAYER_NAME>";
        size_t pos = str.find(placeholder_1);
        if (pos != std::string::npos) {
            // 如果找到，用当前的玩家名替换它
            str.replace(pos, placeholder_1.length(), game_.getPlayer().getName());
        }
        
        // <xxx> 其他占位符
    }

    // 步骤 1: 显示当前节点的对话内容
    game_.getDialog().addMessage(processed.at(0), processed.at(1));

    // 步骤 2: 检查并处理当前节点的选项（交互式 & 线性）
    if (!node->choices.empty()) {
        // --- 交互式节点：处理选项 ---
        std::vector<std::string> choiceTexts;
        for (const auto& choice : node->choices) {
            choiceTexts.push_back(choice.text);
        }
        
        game_.requestChoice(
            // 参数1: prompt
            "请做出选择:",
            // 参数2: choices - 选项的文本列表
            choiceTexts,
            // 参数3: on_select - 选中后的回调函数
            [this, node](int index, const std::string& text) {
                // 这个回调是在 View 层中，当玩家点击按钮时被触发的
                // 玩家做出选择后，由StoryController控制流程
                game_.clearInputRequest(); // 清理输入状态，准备进行下一步

                const Choice& selectedChoice = node->choices[index];
                
                // 步骤 2.1: 执行与该选项关联的动作(Action)
                if (selectedChoice.action) {
                    selectedChoice.action(this->game_);
                }
                
                // 步骤 2.2: 跳转到下一个对话节点
                this->processNodeByID(selectedChoice.nextNodeID);
            }
        );
    }  else {
        // --- 线性节点：按顺序处理动作和跳转 ---
        
        // 步骤 2a: 执行当前节点的 Action (如果存在)
        if (node->action) {
             node->action(game_);
        }

        // 步骤 2b: 跳转到下一个节点 (如果存在)
        if (node->nextNodeID != 0) {
            processNodeByID(node->nextNodeID);
        }
    }
}