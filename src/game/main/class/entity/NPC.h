#ifndef ABSTRACTNPC_H
#define ABSTRACTNPC_H

#include <string>

class NPC {
public:
    NPC(const NPC&) = default;
    NPC(const std::string& name, const std::string& intro);

    std::string getName();
    std::string getIntro();
private:
    std::string _name_;
    std::string _intro_;

};

#endif // ABSTRACTNPC_H
