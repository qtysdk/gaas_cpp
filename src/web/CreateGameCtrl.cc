#include "CreateGameCtrl.h"
#include "../usecase/GuessNumber.h"
#include "nlohmann/json.hpp"
#include "../usecase/CreateGame.h"
#include "common.h"


using json = nlohmann::json;


void CreateGameCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req,
                                            std::function<void(const HttpResponsePtr &)> &&callback) {
    CreateGameUseCase uc;
    Output output;

    auto data = json::parse(req->getBody());
    uc.execute(CreateGameInput(data["player_name"]), output);
    callback(newJsonResponse(output.to_json()));
}

