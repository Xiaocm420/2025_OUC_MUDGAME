#pragma once

#include <string>
#include <vector>
#include <chrono>

class Game;

// 用于存储单条对话消息的结构体
struct DialogMessage {
    std::string who;
    std::string content; // 完整内容
    std::chrono::steady_clock::time_point start_time; // 开始显示时间
};

class Dialog {
public:
    // 构造函数需要 Game 的引用来执行命令
    explicit Dialog(Game& game_logic);

    // 添加一条消息到对话历史（来自NPC、系统等）
    void addMessage(const std::string& who, const std::string& content);

    // 处理玩家在输入框中按下回车后的字符串
    void processPlayerInput(std::string& input);

    // 获取对话历史记录，用于渲染
    [[nodiscard]] const std::vector<DialogMessage>& getHistory() const;

    // 清除对话历史记录
    void clearHistory() {
        history_.clear();
    }

protected:
    Game& game_logic_;

private:
    std::vector<DialogMessage> history_;
    static const size_t MAX_HISTORY_SIZE = 256; // 限制历史记录最大长度
};