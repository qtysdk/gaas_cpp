#include <random>
#include <iostream>
#include "GameRepository.h"


std::shared_ptr<IGameRepository> getGameRepo() {
    return std::make_shared<InMemoryGameRepository>();
}

InMemoryGameRepository::~InMemoryGameRepository() {
    storage.clear();
}


const std::string randomId() {
    srand(time(0));
    return std::to_string(rand());
}

std::shared_ptr<Game> InMemoryGameRepository::create(std::string playerName) {
    std::shared_ptr<Game> game = std::make_shared<Game>();
    game->id = randomId();
    game->playerName = playerName;
    storage[game->id] = game;
    return game;
}

std::shared_ptr<Game> InMemoryGameRepository::findGameById(std::string gameId) {
    return storage[gameId];
}

void InMemoryGameRepository::save(std::shared_ptr<Game> game) {
    // do nothing, there are all in the memory
}
