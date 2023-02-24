#pragma once

#include <string>
#include <memory>
#include "../model/Model.h"

class Output {

private:
    std::string gameStatus;

public:
    void buildGameStatus(std::shared_ptr<Game> game);

    std::string to_json();
};