#include "GuessNumberCtrl.h"
#include "common.h"

void GuessNumberCtrl::asyncHandleHttpRequest(const drogon::HttpRequestPtr &req,
                                             function<void(const drogon::HttpResponsePtr &)> &&callback) {

    GuessNumberUseCase uc;
    Output output;


    auto data = nlohmann::json::parse(req->getBody());
    uc.execute(GuessNumberInput(data["game_id"], data["number"]), output);
    callback(newJsonResponse(output.to_json()));
}
