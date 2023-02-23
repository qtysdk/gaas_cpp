#include "CreateGameUseCase.h"

using namespace std;

Input::Input(string player_name) {
    this->player_name = player_name;
}

void CreateGameUseCase::execute(Input input, Output output) {

}

string Output::to_json() {
    return "";
}
