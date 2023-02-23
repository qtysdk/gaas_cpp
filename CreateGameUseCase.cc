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
    output.game = game;
}


string Output::to_json() {
    json history = json::array();
    for (const auto &record: this->game.history) {
        json entry;
        entry["guess"] = record->guess;
        entry["respond"] = record->respond;
        history.push_back(entry);
    }

    return json{{"game_id",     this->game.id},
                {"player_name", this->game.playerName},
                {"history",     history}
    }.dump();
}
