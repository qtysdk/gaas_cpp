
#include <iostream>
#include <random>
#include <sstream>
#include "Game.h"

Game::~Game() {
    history.clear();
}

Game::Game() {
    this->won = false;
    this->answer = createAnswer();
}


bool Game::guessNumber(int number) {
    auto r = std::make_shared<Record>();
    if (!validateNumber(number)) {
        return false;
    }
    r->guess = number;
    r->respond = createRespond(this->answer, number);
    history.push_back(r);
    if (r->respond == "4A0B") {
        this->won = true;
    }
    return true;
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
    auto valid = validateNumber(answer) && validateNumber(number);
    if (!valid) {
        return "0A0B";
    }

    std::string m = std::to_string(answer);
    std::string n = std::to_string(number);

    int a = 0;
    int b = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (m[i] == n[j]) {
                if (i == j) {
                    a++;
                } else {
                    b++;
                }
            }
        }
    }

    std::stringstream output;
    output << a << "A" << b << "B";

    return output.str();
}
