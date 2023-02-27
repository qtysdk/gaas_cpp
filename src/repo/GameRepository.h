#pragma once

#include <string>
#include <map>
#include "../model/Game.h"


class IGameRepository {
public:
    virtual std::shared_ptr<Game> create(std::string playerName) = 0;

    virtual std::shared_ptr<Game> findGameById(std::string gameId) = 0;

    virtual void save(std::shared_ptr<Game> game) = 0;
};


class InMemoryGameRepository : public IGameRepository {

private:
    std::map<std::string, std::shared_ptr<Game>> storage;

public:
    ~InMemoryGameRepository();

    std::shared_ptr<Game> create(std::string playerName);

    std::shared_ptr<Game> findGameById(std::string gameId);

    void save(std::shared_ptr<Game> game);
};

std::shared_ptr<IGameRepository> getGameRepo();

