#include "common.h"
#include <json/json.h>

void Output::buildGameStatus(std::shared_ptr<Game> game) {
    Json::Value history(Json::arrayValue);
    for (const auto &record: game->history) {
        Json::Value entry;
        entry["guess"] = record->guess;
        entry["respond"] = record->respond;
        history.append(entry);
    }

    Json::Value status;
    status["game_id"] = game->id;
    status["player_name"] = game->playerName;
    status["won"] = game->won;
    status["history"] = history;

    this->gameStatus = status;
}

std::string Output::to_json() {
    Json::FastWriter writer;
    std::string json_str = writer.write(this->gameStatus);
    return json_str;
}
