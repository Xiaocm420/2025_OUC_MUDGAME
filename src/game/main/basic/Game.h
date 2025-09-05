// Game.h
#pragma once

#include "../class/entity/Player.h"
#include "Dialog.h"


static std::string SYSTEM = "系统";

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

    View* view_;
    Dialog* dialog_;
    Player* player_;
};