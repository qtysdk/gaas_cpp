#include "CreateGameUseCase.h"
#include <nlohmann/json.hpp>
#include "GameRepository.h"

using namespace std;
using json = nlohmann::json;

Input::Input(string playerName) {
    this->playerName = playerName;
}

void CreateGameUseCase::execute(Input input, Output &output) {
    // 用 repo 查改存推
    auto game = gameRepository.create(input.playerName);
    
    output.gameId = game.id;
    output.playerName = game.playerName;
}

string Output::to_json() {
    // TODO add real history
    return json{{"game_id",     this->gameId},
                {"player_name", this->playerName},
    }.dump();
}
