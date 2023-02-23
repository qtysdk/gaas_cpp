#include "GameCtrls.h"
#include "CreateGameUseCase.h"
#include <nlohmann/json.hpp>
#include "GameRepository.h"

using json = nlohmann::json;


void GameCtrls::asyncHandleHttpRequest(const HttpRequestPtr &req,
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

void GuessNumberCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req,
                                             std::function<void(const HttpResponsePtr &)> &&callback) {

    GuessNumberUseCase uc;
    Output output;


    auto data = json::parse(req->getBody());
    uc.execute(GuessNumberInput(data["game_id"], data["number"]), output);

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_HTML);
    resp->setBody(output.to_json());
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}
