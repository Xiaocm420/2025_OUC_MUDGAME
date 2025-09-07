// Game.h
#pragma once

#include "../class/entity/Player.h"
#include "Dialog.h"

class View;

class Dialog;

class Player;

class Game {
public:
    Game();
    ~Game();

    void run();
    void startNewGame() const;
    void loadGame() const;
    void showGameIntro() const;
    void showGameSettings();
    void exitGame();
    [[nodiscard]] Dialog& getDialog() const;
    [[nodiscard]] Player& getPlayer() const;

private:
    bool running;

    std::unique_ptr<View> view_;
    std::unique_ptr<Dialog> dialog_;
    std::unique_ptr<Player> player_;
};

static std::string SYSTEM = "系统";
inline static std::string PLAYER; // 比较方便，以后可能有改名卡道具

inline static const std::string VOICEOVER = ""; // 旁白
inline static const std::string UNKNOWN = "???";