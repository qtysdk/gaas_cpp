
#include <iostream>
#include <random>
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
    int v[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(v, v + 10, g);
    if (v[0] == 0) {
        return createAnswer();
    }
    return v[0] * 1000 + v[1] * 100 + v[2] * 10 + v[3];
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
