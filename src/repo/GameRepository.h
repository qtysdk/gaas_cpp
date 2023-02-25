#pragma once

#include <string>
#include <map>
#include "../model/Game.h"

class GameRepository {

private:
    std::map<std::string, std::shared_ptr<Game>> storage;

public:
    ~GameRepository();

    std::shared_ptr<Game> create(std::string playerName);

    std::shared_ptr<Game> findGameById(std::string gameId);

    void save(std::shared_ptr<Game> game);
};

extern GameRepository gameRepository;
