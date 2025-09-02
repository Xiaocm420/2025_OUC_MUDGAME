#include "PlayerService.h"

PlayerService::PlayerService() = default;

PlayerService::~PlayerService() = default;

PlayerService& PlayerService::getInstance() {
    static PlayerService instance;
    return instance;
}

void PlayerService::shutdown() {
    // TODO
    // 玩家数据保存逻辑
}

void PlayerService::registerPlayer(const std::string& name) {
    player.setName(name);
}

Player &PlayerService::getPlayer() {
    return player;
}
