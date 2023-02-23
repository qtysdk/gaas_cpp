#pragma once

#include <string>
#include <map>
#include "Models.h"

class GameRepository {

private:
    std::map<std::string, Game *> storage;

public:
    ~GameRepository();

    const Game &create(std::string playerName);

    const Game & findGameById(std::string gameId);
};

extern GameRepository gameRepository;
