#pragma once

#include <string>
#include "common.h"

class CreateGameInput {

public:
    std::string playerName;

    CreateGameInput(std::string playerName);
};

class CreateGameUseCase {

public:
    void execute(CreateGameInput input, Output &output);
};
