#include <iostream>
#include "CreateGame.h"
#include "common.h"
#include "../repo/GameRepository.h"

CreateGameInput::CreateGameInput(std::string playerName) {
    this->playerName = playerName;
}

void CreateGameUseCase::execute(CreateGameInput input, Output &output) {
    // 用 repo 查改存推
    auto game = gameRepository.create(input.playerName);
    std::cout << "create game for id: " << game->id << ", answer: " << game->answer << std::endl;
    output.buildGameStatus(game);
}
