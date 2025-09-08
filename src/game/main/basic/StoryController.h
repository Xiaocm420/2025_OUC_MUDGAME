#ifndef STORYCONTROLLER_H
#define STORYCONTROLLER_H

#include "Types.h"
#include <map>
#include <string>

struct DialogNode;

/**
 * @class StoryController
 * @brief 游戏剧情的核心驱动引擎。
 * @details 负责解析对话数据、处理玩家选择，并根据剧情数据驱动游戏流程。
 */
class StoryController {
public:
    /**
     * @brief 构造函数。
     * @param game 对Game核心对象的引用。
     */
    explicit StoryController(Game& game);

    /**
     * @brief 从指定的对话节点ID开始一段剧情。
     * @param dialogNodeID 剧情起始节点的唯一ID。
     */
    void startStory(unsigned int dialogNodeID);
    
    /**
     * @brief 处理单个对话节点，通常由内部逻辑调用。
     * @param dialogNodeID 要处理的节点的唯一ID。
     */
    void processNodeByID(unsigned int dialogNodeID);

private:
    /**
     * @brief 真正处理DialogNode对象的内部函数。
     * @param node 指向要处理的DialogNode对象的指针。
     */
    void processNode(const DialogNode* node);

    /**
     * @brief 注册所有的对话数据到数据库中。
     * @details 通过DialogRegistry自动查找并加载所有章节的对话数据。
     */
    void registerDialogs();

    Game& game_; ///< 对Game核心对象的引用。
    std::map<unsigned int, const DialogNode*> dialog_database_; ///< 存储所有对话节点的数据库。
};

#endif // STORYCONTROLLER_H