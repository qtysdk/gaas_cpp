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
    std::list<std::shared_ptr<Record>> history;

    Game();

    ~Game();

    bool guessNumber(int number);
};


int createAnswer();

bool validateNumber(int number);

std::string createRespond(int answer, int number);