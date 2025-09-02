#ifndef PLAYER_SERVICE_H
#define PLAYER_SERVICE_H

#include "../class/entity/player/Player.h"

/*
 *  PlayerService 类：
 *  单例，通过 PlayerService& playerService = PlayerService::getInstance(); 获取。
 *  持有 Player 对象，可通过 playerService.getPlayer() 获取玩家的引用，
 *  进而操作玩家对象。
 */
class PlayerService {
public:
    // 禁用复制和赋值
    PlayerService(const PlayerService&) = delete;
    PlayerService& operator=(const PlayerService&) = delete;

    // 单例访问
    static PlayerService& getInstance();

    // 关闭 PlayerService
    static void shutdown();
    
    // 玩家注册
    void registerPlayer(const std::string& name);

    Player& getPlayer();
private:
    PlayerService();
    ~PlayerService();

    Player player;
};

#endif // PLAYER_SERVICE_H