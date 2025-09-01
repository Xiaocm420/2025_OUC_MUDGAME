#include "SceneBase.h"
#include "SceneManager.h"
#include <algorithm>

SceneManager& SceneManager::getInstance() {
    static SceneManager instance;
    return instance;
}

void SceneManager::shutdown() {
    // TODO
}

void SceneManager::registerScene(const std::string& id, std::unique_ptr<SceneBase> scene) {
    scenes[id] = std::move(scene);
}

void SceneManager::pushScene(const std::string& id, int layer) {
    if (!scenes.contains(id)) return;

    // 检查场景是否已激活
    bool wasActive = false;
    for (auto& [sceneId, activeLayer] : activeScenes) {
        if (sceneId == id) {
            wasActive = true;
            break;
        }
    }

    // 如果场景之前不活跃，则调用onEnter
    if (!wasActive) {
        scenes[id]->onEnter(); // 核心生命周期方法
    }

    // 移除同名场景（如果已存在）
    const auto it = std::find_if(activeScenes.begin(), activeScenes.end(),
        [&](const auto& pair) { return pair.first == id; });
    if (it != activeScenes.end()) {
        activeScenes.erase(it);
    }
    // 添加新场景并排序
    activeScenes.emplace_back(id, layer);
    std::sort(activeScenes.begin(), activeScenes.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
}

void SceneManager::popScene(const std::string& id) {
    auto it = std::find_if(activeScenes.begin(), activeScenes.end(),
        [&](const auto& pair) { return pair.first == id; });
    if (it != activeScenes.end()) {
        scenes[id]->onExit(); // 核心生命周期方法
        activeScenes.erase(it);
    }
}

void SceneManager::setSceneLayer(const std::string& id, int layer) {
    auto it = std::find_if(activeScenes.begin(), activeScenes.end(),
        [&](const auto& pair) { return pair.first == id; });
    if (it != activeScenes.end()) {
        it->second = layer;
        std::sort(activeScenes.begin(), activeScenes.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });
    }
}

std::vector<SceneBase*> SceneManager::getActiveScenes() const {
    std::vector<SceneBase*> result;
    for (const auto& [id, layer] : activeScenes) {
        auto it = scenes.find(id);
        if (it != scenes.end()) {
            result.push_back(it->second.get());
        }
    }
    return result;
}
