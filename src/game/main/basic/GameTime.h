#ifndef INC_2025_OUC_MUDGAME_GAMETIME_H
#define INC_2025_OUC_MUDGAME_GAMETIME_H

/**
 * @class GameTime
 * @brief 一个单例类，用于管理和操作游戏内的虚拟时间。
 */
class GameTime {
public:
    /**
     * @brief 默认构造函数。
     * @details 初始化游戏时间为 2007年8月31日 14:39。
     *          注意：这是一个单例模式，您不应该手动创建 GameTime 的实例。
     *          直接通过静态方法（如 GameTime::addMinute）来使用即可。
     */
    explicit GameTime();

    // --- 时间修改函数 ---
    static void addYear(const unsigned int YEAR);
    static void addMonth(const unsigned int MONTH);
    static void addDay(const unsigned int DAY);
    static void addHour(const unsigned int HOUR);
    static void addMinute(const unsigned int MINUTE);

    /**
     * @brief 设置一个新的游戏时间。
     * @details 不允许将时间设置为比当前时间更早的时间（防止时光倒流）。
     *          同时会验证输入时间的有效性。
     * @return 如果设置成功，返回 true；如果时间倒流或参数无效，返回 false。
     */
    static bool setTime(const unsigned int YEAR,
                        const unsigned int MONTH,
                        const unsigned int DAY,
                        const unsigned int HOUR,
                        const unsigned int MINUTE);

    // --- 时间获取函数 ---
    static unsigned int getYear();
    static unsigned int getMonth();
    static unsigned int getDay();
    static unsigned int getHour();
    static unsigned int getMinute();

    // --- 静态成员 ---
    static GameTime _Time_; // 唯一的游戏时间实例（单例对象）

private:
    unsigned int YEAR_;
    unsigned int MONTH_;
    unsigned int DAY_;
    unsigned int HOUR_;
    unsigned int MINUTE_;
    bool isLeapYear = false;

    // 静态常量，存储每个月的天数（非闰年）
    static const unsigned int month[13];

    static void updateLeapYearStatus();
};

#endif //INC_2025_OUC_MUDGAME_GAMETIME_H