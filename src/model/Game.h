#pragma once

#include <string>
#include <list>
#include <memory>
#include <algorithm>

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
    bool won;
    std::list<std::shared_ptr<Record>> history;

    Game();

    ~Game();

    bool guessNumber(int number);

    bool operator==(const Game &other) const {
        return id == other.id
               && playerName == other.playerName
               && answer == other.answer
               && won == other.won;
    }
};


int createAnswer();

bool validateNumber(int number);

std::string createRespond(int answer, int number);