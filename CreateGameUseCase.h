#pragma once

#include <string>
#include <list>

using namespace std;

class Input {

public:
    string playerName;

    Input(string playerName);
};

class Output {


public:
    string gameId;
    string playerName;

//    TODO define the history entry
//    list<Record> history;

    string to_json();
};

class CreateGameUseCase {

public:
    void execute(Input input, Output &output);
};
