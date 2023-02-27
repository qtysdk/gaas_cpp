#include "GuessNumber.h"
#include <json/json.h>
#include "../repo/GameRepository.h"

using namespace std;


void GuessNumberUseCase::execute(GuessNumberInput input, Output &output) {
    std::shared_ptr<Game> game = getGameRepo()->findGameById(input.gameId);
    if (!game->won) {
        game->guessNumber(input.number);
        getGameRepo()->save(game);
    }
    output.buildGameStatus(game);
    return;
}

GuessNumberInput::GuessNumberInput(const string &gameId, int number) : gameId(gameId), number(number) {}
