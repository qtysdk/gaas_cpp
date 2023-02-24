#include "UseCases.h"
#include <nlohmann/json.hpp>
#include "GameRepository.h"

using namespace std;
using json = nlohmann::json;

CreateGameInput::CreateGameInput(string playerName) {
    this->playerName = playerName;
}

void CreateGameUseCase::execute(CreateGameInput input, Output &output) {
    // 用 repo 查改存推
    auto game = gameRepository.create(input.playerName);
    output.buildGameStatus(game);
}


void Output::buildGameStatus(std::shared_ptr<Game> game) {
    json history = json::array();
    for (const auto &record: game->history) {
        json entry;
        entry["guess"] = record->guess;
        entry["respond"] = record->respond;
        history.push_back(entry);
    }

    this->gameStatus = json{{"game_id",     game->id},
                            {"player_name", game->playerName},
                            {"history",     history}}.dump();
}

string Output::to_json() {
    return this->gameStatus;
}

void GuessNumberUseCase::execute(GuessNumberInput input, Output &output) {
    std::shared_ptr<Game> game = gameRepository.findGameById(input.gameId);
    game->guessNumber(input.number);
    gameRepository.save(game);
    output.buildGameStatus(game);
    return;
}

GuessNumberInput::GuessNumberInput(const string &gameId, int number) : gameId(gameId), number(number) {}
