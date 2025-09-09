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

    // 步骤 1: 显示当前节点的对话内容
    game_.getDialog().addMessage(node->who, node->content);

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
    } else {
        pending_next_node_id_ = node->nextNodeID;   // 记录跳转ID
        // --- 线性节点：启动序列，然后计划跳转 ---
        if (!node->sequence.empty()) {
            active_sequence_ = node->sequence;      // 将节点的序列复制到活动队列
            sequence_step_ = 0;                     // 从第一步开始
            wait_until_ = {};                       // 清理上一个等待计时器
        } else {
            // 如果没有序列，立即尝试跳转
            update();
        }
    }
}

void StoryController::update() {
    // 检查是否正在等待
    if (std::chrono::steady_clock::now() < wait_until_) {
        return; // 等待时间未到，直接返回
    }

    if (active_sequence_.empty() || sequence_step_ >= active_sequence_.size()) {
        // 序列为空或已执行完毕，检查是否有待处理的跳转
        if (pending_next_node_id_ != 0) {
            unsigned int next_id = pending_next_node_id_;
            pending_next_node_id_ = 0; // 清理，防止重复跳转
            processNodeByID(next_id);
        }
        return;
    }

    // 循环处理，直到遇到一个需要等待的步骤
    while (sequence_step_ < active_sequence_.size()) {
        const auto& step = active_sequence_[sequence_step_];
        bool should_wait = false;

        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, SpeakAction>) {
                game_.getDialog().addMessage(arg.who, arg.content);
            }
            else if constexpr (std::is_same_v<T, WaitAction>) {
                wait_until_ = std::chrono::steady_clock::now() + arg.duration;
                should_wait = true;
            }
            else if constexpr (std::is_same_v<T, ExecuteAction>) {
                if (arg.function) {
                    arg.function(game_);
                }
            }
        }, step);

        sequence_step_++; // 移动到下一步

        if (should_wait) {
            return; // 如果是等待步骤，则退出update循环，等待下一帧
        }
    }

    // 如果循环自然结束
    if (sequence_step_ >= active_sequence_.size()) {
        active_sequence_.clear();
        // 再次检查跳转，确保序列最后一步执行完后能立即跳转
        if (pending_next_node_id_ != 0) {
            unsigned int next_id = pending_next_node_id_;
            pending_next_node_id_ = 0;
            processNodeByID(next_id);
        }
    }
}