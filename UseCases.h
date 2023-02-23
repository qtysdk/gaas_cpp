#pragma once

#include <string>
#include <list>
#include "Models.h"

using namespace std;

class CreateGameInput {

public:
    string playerName;

    CreateGameInput(string playerName);
};

class Output {

private:
    string gameStatus;

public:
    void buildGameStatus(Game *game);

    string to_json();
};

class CreateGameUseCase {

public:
    void execute(CreateGameInput input, Output &output);
};


class GuessNumberInput {

public:
    string gameId;
    int number;

    GuessNumberInput(const string &gameId, int number);
};

class GuessNumberUseCase {
public:
    void execute(GuessNumberInput input, Output &output);
};