#pragma once
#include "../Story.h"
#include "../../basic/Game.h"
#include "../../class/entity/Player.h"

#include <string>

/* 对话id规则：8位数字，如 ABBCCDDD A 为章节数 BB 为章节的某个部分 CC 为场景 id，从 0 开始，指发生在哪个场景的对话
 * 选择id规则：9位数字，如 ABBCCDDDE 前8位同上，E为对话编号，从 0 开始
 * 任务id规则：4位数字，如 AACC AA为主任务ID，CC为子任务ID
 */

inline DialogNode _00000001(00000001, UNKNOWN, "欢迎！这是你第一次进入这个游戏，来给你自己起个名字吧:");

inline DialogNode _00000002(00000002, UNKNOWN, " 吗？嗯嗯，确实是个好名字呢，祝你玩的开心");


