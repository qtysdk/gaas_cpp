#pragma once

#include <string>
#include <list>
#include "Models.h"

using namespace std;

class Input {

public:
    string playerName;

    Input(string playerName);
};

class Output {


public:
    Game game;

    string to_json();
};

class CreateGameUseCase {

public:
    void execute(Input input, Output &output);
};
