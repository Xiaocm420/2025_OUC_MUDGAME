#include "Game.h"
#include "SceneManager.h"
#include "Terminal.h"
#include "../scene/StartScene.h"
#include <algorithm>
#include <memory>

Game::Game() {
    init();
}

Game::~Game() {
    playerService.shutdown();
    sceneManager.shutdown();
}

void Game::run() {
    running = true;

    // 游戏主循环
    while (running) {
        terminal.clearScreen();

        // 获取当前场景
        auto activeScenes = sceneManager.getActiveScenes();

        // 渲染所有活动场景
        for (auto* scene : activeScenes) {
            scene->render();
        }

        // 处理输入
        processInput();
    }
}

void Game::init() {
    // 初始化终端
    Terminal::init();

    // 初始化游戏场景
    initScenes();
}

void Game::initScenes() {
    sceneManager.registerScene("startMenu", std::make_unique<StartScene>());
    sceneManager.pushScene("startMenu", 0);
}

void Game::processInput() {
    // 获取所有活动场景（按层级倒序，高层级优先处理输入）
    auto activeScenes = sceneManager.getActiveScenes();
    std::reverse(activeScenes.begin(), activeScenes.end());

    char immediateKey = getchar();
    if (immediateKey != ':') {
        for (auto* scene : activeScenes) {
            scene->processImmediateKey(immediateKey);
        }
    }
}
