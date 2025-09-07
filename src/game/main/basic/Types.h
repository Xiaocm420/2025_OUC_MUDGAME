#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <optional>
#include <vector>

class Game;

/**
 * @brief 用于选项(Choice)的动作类型。
 * @details 这种动作是无上下文的，仅操作Game对象。
 * @param game 对Game核心对象的引用。
 */
using ChoiceAction = std::function<void(Game&)>;

/**
 * @brief 用于文本输入(TextInput)相关动作的类型。
 * @details 这种动作会接收玩家输入的文本作为额外参数。
 * @param game 对Game核心对象的引用。
 * @param input 玩家输入的文本。
 */
using TextInputAction = std::function<void(Game&, std::string_view)>;

/**
 * @brief 输入规则的条件判断函数类型。
 * @param input 待检查的输入文本。
 * @return 如果输入满足条件，则返回true。
 */
using RuleCondition = std::function<bool(std::string_view)>;

/**
 * @brief 与输入规则匹配时执行的动作类型。
 * @param game 对Game核心对象的引用。
 * @param input 匹配规则的输入文本。
 */
using RuleAction = std::function<void(Game&, std::string_view)>;

/**
 * @struct InputRule
 * @brief 定义一条文本输入的处理规则。
 */
struct InputRule {
    RuleCondition condition; ///< 规则的匹配条件。
    RuleAction action;       ///< 条件满足时执行的动作。
};