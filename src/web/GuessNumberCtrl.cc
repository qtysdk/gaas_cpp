#include "GuessNumberCtrl.h"

void GuessNumberCtrl::asyncHandleHttpRequest(const drogon::HttpRequestPtr &req,
                                             function<void(const drogon::HttpResponsePtr &)> &&callback) {

    GuessNumberUseCase uc;
    Output output;


    auto data = nlohmann::json::parse(req->getBody());
    uc.execute(GuessNumberInput(data["game_id"], data["number"]), output);

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::k200OK);
    resp->setContentTypeCode(drogon::CT_TEXT_HTML);
    resp->setBody(output.to_json());
    resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
    callback(resp);
}
