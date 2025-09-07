#include "StoryController.h"
#include "Game.h"
#include "Dialog.h"
#include "../GameStory/Story.h"
#include "../GameStory/DialogRegistry.h"
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
    std::string processed_content = node->content;
    // <PLAYER_NAME> 占位符
    std::string placeholder_1 = "<PLAYER_NAME>";
    size_t pos = processed_content.find(placeholder_1);
    if (pos != std::string::npos) {
        // 如果找到，用当前的玩家名替换它
        processed_content.replace(pos, placeholder_1.length(), game_.getPlayer().getName());
    }

    // 步骤 1: 显示当前节点的对话内容
    game_.getDialog().addMessage(node->who, processed_content);

    // 步骤 2: 检查并处理当前节点的选项
    if (!node->choices.empty()) {
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
    } 
    // 如果没有选项，检查是否有下一个线性节点
    else if (node->nextNodeID != 0) {
        processNodeByID(node->nextNodeID);
    }
    // 步骤 3: 如果没有选项，检查节点本身是否附加了动作
    else if (node->action) {
         node->action(game_);
    }
}