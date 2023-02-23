#include "CreateGameUseCase.h"
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

Input::Input(string playerName) {
    this->playerName = playerName;
}

void CreateGameUseCase::execute(Input input, Output &output) {
    output.gameId = "5566";
    output.playerName = input.playerName;
}

string Output::to_json() {
    // TODO add real history
    return json{{"game_id",     this->gameId},
                {"player_name", this->playerName},
    }.dump();
}
