#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "../basic/SceneBase.h"

class StartScene : public SceneBase {
public:
    void onEnter() override;
    void onExit() override;

    void processInput(const std::string& input) override;
    void processImmediateKey(char key) override;
    void update() override;
    void render() const override;
};

#endif // STARTSCENE_H