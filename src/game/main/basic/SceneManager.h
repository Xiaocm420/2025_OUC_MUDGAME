#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "SceneBase.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

class SceneManager {
public:
    // 单例访问
    static SceneManager& getInstance();

    // 禁止复制和赋值
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    // 关闭 SceneManager
    void shutdown();

    // 注册场景
    void registerScene(const std::string& id, std::unique_ptr<SceneBase> scene);

    // 场景层级管理
    void pushScene(const std::string& id, int layer = 0);
    void popScene(const std::string& id);
    void setSceneLayer(const std::string& id, int layer);

    // 获取所有活动场景（按层级排序）
    std::vector<SceneBase*> getActiveScenes() const;

private:
    // 私有构造函数
    SceneManager() = default;

    // 活动场景列表
    std::map<std::string, std::unique_ptr<SceneBase>> scenes;
    std::vector<std::pair<std::string, int>> activeScenes; // <场景ID, 层级>
};

#endif // SCENE_MANAGER_H