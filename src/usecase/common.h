#pragma once

#include <string>
#include <memory>
#include "../model/Game.h"
#include <json/json.h>


class Output {

public:
    Json::Value gameStatus;

public:
    void buildGameStatus(std::shared_ptr<Game> game);

    std::string to_json();
};