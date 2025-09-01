#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Terminal.h"
#include <string>

class SceneBase {
public:
    virtual ~SceneBase() = default;

    // 生命周期管理
    virtual void onEnter() = 0;
    virtual void onExit() = 0;

    virtual void processInput(const std::string& input) = 0;
    virtual void processImmediateKey(char key) = 0;
    virtual void update() = 0;
    virtual void render() const = 0;

    // 设置场景位置和尺寸
    void setPosition(int x, int y);
    void setSize(int width, int height);

    // 相对坐标绘制方法
    void drawRelative(int relX, int relY, const std::string& content) const;
    void drawRelativeBorder(int relX, int relY, int width, int height) const;
    void drawRelativeTitledBorder(int relX, int relY, int width, int height,
                                  const std::string& title) const;
    /// 居中绘制
    void drawInCenter(int relY, const std::string& content) const;

    // 边框绘制功能
    void drawBorder(int x, int y, int width, int height) const;
    void drawTitledBorder(int x, int y, int width, int height,
                          const std::string& title) const;

protected:
    int x = 0; // 场景左上角X坐标
    int y = 0; // 场景左上角Y坐标
    int width = 0; // 场景宽度
    int height = 0; // 场景高度
    Terminal& terminal = Terminal::getInstance();

private:
    // 获取边框字符
    void getBorderChars(std::string& topLeft, std::string& topRight,
                        std::string& bottomLeft, std::string& bottomRight,
                        std::string& horizontal, std::string& vertical) const;
};

#endif // SCENE_BASE_H