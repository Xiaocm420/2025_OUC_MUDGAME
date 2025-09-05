#ifndef INC_2025_OUC_MUDGAME_INPUTPROCESS_H
#define INC_2025_OUC_MUDGAME_INPUTPROCESS_H
#include <string>
#include <algorithm>

// 用来移除字符串左侧的空白
inline std::string ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

// 用来移除字符串右侧的空白
inline std::string rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

// 移除字符串两端的空白
inline std::string trim(std::string s) {
    return ltrim(rtrim(s));
}
#endif //INC_2025_OUC_MUDGAME_INPUTPROCESS_H