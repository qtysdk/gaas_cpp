#pragma once

#include <string>
#include <map>
#include "Models.h"

class GameRepository {

private:
    std::map<std::string, Game *> storage;

public:
    ~GameRepository();

    Game *create(std::string playerName);

    Game *findGameById(std::string gameId);

    void save(Game *game);
};

extern GameRepository gameRepository;
