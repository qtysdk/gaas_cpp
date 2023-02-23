#pragma once

#include <string>

using namespace std;

class Input {

public:
    string player_name;

    Input(string player_name);
};

class Output {

public:
    string to_json();
};

class CreateGameUseCase {

public:
    void execute(Input input, Output output);
};
