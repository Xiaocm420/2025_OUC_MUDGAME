#include "PlayerService.h"


void PlayerService::initialize() {
    // 初始化玩家服务
    // TODO
}

PlayerService& PlayerService::getInstance() {
    static PlayerService instance;
    return instance;
}

void PlayerService::shutdown() {
    // TODO
    // 玩家数据保存校徽逻辑
}

void PlayerService::registerPlayer(const std::string& name) {
    player.setName(name);
}

Player &PlayerService::getPlayer() {
    return player;
}

PlayerService::PlayerService() = default;

PlayerService::~PlayerService() = default;
