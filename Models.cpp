
#include <iostream>
#include "Models.h"

Game::~Game() {
    std::cout << "~Game() " << this << std::endl;
    for (auto it = history.begin(); it != history.end(); ++it) {
        delete *it;
    }
    history.clear();
}

Game::Game() {
    this->answer = 1234;
}

bool Game::guessNumber(int number) {
    if (number == this->answer) {
        Record *r = new Record;
        r->guess = number;
        r->respond = "4A0B";
        history.push_back(r);
    }
    return false;
}
