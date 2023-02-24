#include "CreateGameCtrl.h"
#include "../usecase/GuessNumber.h"
#include "nlohmann/json.hpp"
#include "../repo/GameRepository.h"
#include "GuessNumberCtrl.h"
#include "../usecase/CreateGame.h"

using json = nlohmann::json;


void CreateGameCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req,
                                            std::function<void(const HttpResponsePtr &)> &&callback) {
    CreateGameUseCase uc;
    Output output;

    auto data = json::parse(req->getBody());
    uc.execute(CreateGameInput(data["player_name"]), output);

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_HTML);
    resp->setBody(output.to_json());
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}

