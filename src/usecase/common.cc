#include "common.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
                            {"won",         game->won},
                            {"history",     history},
    };

}

std::string Output::to_json() {
    return this->gameStatus.dump();
}
