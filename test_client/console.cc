#include <iostream>
#include "../src/usecase/CreateGame.h"
#include "../src/usecase/GuessNumber.h"


void game_loop();

bool isInteger(const std::string &str);

std::string startGame(Output &output);

bool isGameFinished(const Output &output);

void showGuessRespond(Output &output, const string &gameId, const pair<bool, int> &input);

std::pair<bool, int> askForNumber();

int main(int argc, char **argv) {
    game_loop();
    return 0;
}

std::pair<bool, int> askForNumber() {
    std::string my_number;
    std::cout << "請輸入要猜的數字：";
    std::cin >> my_number;

    if (!isInteger(my_number) || !validateNumber(std::stoi(my_number))) {
        std::cout << "數字格式不正確！" << std::endl;
        return {false, 0};
    }

    return {true, std::stoi(my_number)};
}

void game_loop() {
    Output output;

    const std::string gameId = startGame(output);
    while (true) {
        if (isGameFinished(output)) {
            std::cout << "你贏了！我斑願稱你為最強。" << std::endl;
            break;
        }

        auto input = askForNumber();
        if (!input.first) {
            continue;
        }
        showGuessRespond(output, gameId, input);
    }

}

void showGuessRespond(Output &output, const string &gameId, const pair<bool, int> &input) {
    GuessNumberUseCase uc;
    uc.execute(GuessNumberInput(gameId, input.second), output);
    auto history = output.gameStatus["history"];
    cout << "第 " << history.size() << " 次猜測 => " << history[history.size() - 1]["respond"] << endl;
    return;
}

bool isGameFinished(const Output &output) { return output.gameStatus["won"].asBool(); }

std::string startGame(Output &output) {
    string playerName = "";
    cout << "請輸入玩家名稱：";
    cin >> playerName;


    CreateGameUseCase createGameUseCase;
    createGameUseCase.execute(CreateGameInput(playerName), output);
    cout << "Hi, " << output.gameStatus["player_name"] << endl;

    return output.gameStatus["game_id"].asString();
}

bool isInteger(const std::string &str) {
    try {
        std::stoi(str);
        return true;
    }
    catch (const std::exception &) {
        return false;
    }
}