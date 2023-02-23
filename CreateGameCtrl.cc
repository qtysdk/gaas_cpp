#include "CreateGameCtrl.h"
#include "CreateGameUseCase.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void CreateGameCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req,
                                            std::function<void(const HttpResponsePtr &)> &&callback) {
    CreateGameUseCase uc;
    Output output;

    auto data = json::parse(req->getBody());
    uc.execute(Input(data["player_name"]), output);

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_HTML);
    resp->setBody(output.to_json());
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}
