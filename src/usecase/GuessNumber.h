#pragma once

#include <string>
#include <list>
#include "../model/Model.h"
#include "common.h"

using namespace std;


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