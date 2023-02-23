
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

int createAnswer() {
    return 0;
}

bool validateNumber(int number) {
    if (number < 1000 || number > 9999) {
        return false;
    }
    int s[10];
    std::fill(s, s + 10, 0);

    s[number / 1000]++;
    s[number % 1000 / 100]++;
    s[number % 100 / 10]++;
    s[number % 10]++;

    for (const auto &item: s) {
        if (item > 1) {
            return false;
        }
    }

    return true;
}

std::string createRespond(int answer, int number) {
    return std::string();
}
