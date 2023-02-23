#include <random>
#include <iostream>
#include "GameRepository.h"

GameRepository gameRepository;

GameRepository::~GameRepository() {
    std::cout << "Invoke ~GameRepository" << std::endl;
    for (auto it = storage.begin(); it != storage.end(); ++it) {
        delete it->second;
    }
}


const std::string randomId() {
    srand(time(0));
    return std::to_string(rand());
}

const Game &GameRepository::create(std::string playerName) {
    Game *game = new Game();
    game->id = randomId();
    game->playerName = playerName;
    storage[game->id] = game;
    return *game;
}

const Game &GameRepository::findGameById(std::string gameId) {
    return *storage[gameId];
}
