
#include "Models.h"

Game::~Game() {
    for (auto it = history.begin(); it != history.end(); ++it) {
        delete *it;
    }
}

Game::Game() {
    this->answer = 1234;
}
