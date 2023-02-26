#include "CreateGameCtrl.h"
#include "../usecase/GuessNumber.h"
#include <json/json.h>
#include "../usecase/CreateGame.h"
#include "common.h"


void CreateGameCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req,
                                            std::function<void(const HttpResponsePtr &)> &&callback) {
    CreateGameUseCase uc;
    Output output;


    Json::Value data;
    Json::Reader reader;
    bool parsed = reader.parse(std::string(req->getBody()), data);
    if (!parsed) {
        // TODO handle parsing error
    }

    uc.execute(CreateGameInput(data["player_name"].asString()), output);
    callback(newJsonResponse(output.to_json()));
}

