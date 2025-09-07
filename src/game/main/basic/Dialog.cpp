#include "Dialog.h"
#include "Game.h"
#include "InputProcess.h"
#include "StoryController.h"

Dialog::Dialog(Game& game_logic) : game_logic_(game_logic) {}

void Dialog::addMessage(const std::string& who, const std::string& content) {
    history_.push_back({who, content, std::chrono::steady_clock::now()});

    // 如果历史记录过长，则移除最旧的一条
    if (history_.size() > MAX_HISTORY_SIZE) {
        history_.erase(history_.begin());
    }
}

void Dialog::processPlayerInput(std::string& input) {
    if (input.empty()) {
        return;
    }

    // 命令与对话
    if (input[0] == '/') {
        // 命令，交给 Game 逻辑层处理
        input = trim(input);

        // TODO: 在这里解析并执行具体的命令
        if (input == "/help") {
            game_logic_.getDialog().addMessage("系统", "可用命令: /status, /save, /quit");
        } else if (input == "/status") {
            game_logic_.getDialog().addMessage("系统", "你感觉状态良好。"); // TODO: 需要API接入
        } else {
            game_logic_.getDialog().addMessage("系统", "未知命令: " + input);
        }
    } else {
        // 对话，将其添加到历史记录中
        addMessage(game_logic_.getPlayer().getName(), input);
    }
}

const std::vector<DialogMessage>& Dialog::getHistory() const {
    return history_;
}