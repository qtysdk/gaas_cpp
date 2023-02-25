#pragma once

#include <string>
#include <memory>
#include "../model/Game.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Output {

public:
    json gameStatus;

public:
    void buildGameStatus(std::shared_ptr<Game> game);

    std::string to_json();
};