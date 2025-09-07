#include "Game.h"
#include "View.h"
#include "Dialog.h"
#include "../class/entity/Player.h"

#include <iostream>
#include <string>

#include "../GameStory/GameProcess.h"

Game::Game() : running(false) {
    view_ = new View(*this);
    dialog_ = new Dialog(*this);
    player_ = new Player(*this);
}

Game::~Game() {
    // TODO: 游戏退出析构函数
}

void Game::run() {
    running = true;
    if (view_) {
        view_->showMainMenu();
    }
}

void Game::startNewGame() const {
    if (view_) {
        View::showLoadingScreen("正在加载新游戏");
    }
    std::cout << "开始新游戏..." << std::endl;
    // TODO
    // ... 此处是开始新游戏的具体逻辑 ...
    PLAYER = GameProcess::newStart(dialog_, player_, view_);

    if (view_) {
        view_->showGameScreen();
    }
}

void Game::loadGame() const {
    std::string saveName = "default";
    if (view_) {
        View::showLoadingScreen("正在打开存档{" + saveName + "}");
    }
    std::cout << "读取存档..." << std::endl;
    // TODO
    // ... 此处是读取存档的具体逻辑 ...
}

void Game::showGameIntro() const {
    std::cout << "游戏介绍..." << std::endl;
    if (view_) {
        View::showGameIntroScreen();
    }
}

void Game::showGameSettings() {
    std::cout << "游戏设置..." << std::endl;
    // TODO
    // ... 显示设置的具体逻辑 ...
}

void Game::exitGame() {
    running = false;
    std::cout << "退出游戏..." << std::endl;
}

Dialog& Game::getDialog() const {
    return *dialog_;
}

Player& Game::getPlayer() const {
    return *player_;
}
