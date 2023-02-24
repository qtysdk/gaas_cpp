#include <random>
#include <iostream>
#include "GameRepository.h"

GameRepository gameRepository;

GameRepository::~GameRepository() {
    storage.clear();
}


const std::string randomId() {
    srand(time(0));
    return std::to_string(rand());
}

std::shared_ptr<Game> GameRepository::create(std::string playerName) {
    std::shared_ptr<Game> game = std::make_shared<Game>();
    game->id = randomId();
    game->playerName = playerName;
    storage[game->id] = game;
    return game;
}

std::shared_ptr<Game> GameRepository::findGameById(std::string gameId) {
    return storage[gameId];
}

void GameRepository::save(std::shared_ptr<Game> game) {
    // do nothing, there are all in the memory
}
