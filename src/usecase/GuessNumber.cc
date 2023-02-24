#include "GuessNumber.h"
#include "nlohmann/json.hpp"
#include "../repo/GameRepository.h"

using namespace std;
using json = nlohmann::json;




void GuessNumberUseCase::execute(GuessNumberInput input, Output &output) {
    std::shared_ptr<Game> game = gameRepository.findGameById(input.gameId);
    game->guessNumber(input.number);
    gameRepository.save(game);
    output.buildGameStatus(game);
    return;
}

GuessNumberInput::GuessNumberInput(const string &gameId, int number) : gameId(gameId), number(number) {}
