#pragma once

#include <string>
#include <list>

class Record {
public:
    int guess;
    std::string respond;
};

class Game {

public:
    std::string id;
    std::string playerName;
    int answer;
    std::list<Record *> history;

    Game();

    ~Game();
};
