#include "GuessNumberCtrl.h"
#include "common.h"

void GuessNumberCtrl::asyncHandleHttpRequest(const drogon::HttpRequestPtr &req,
                                             function<void(const drogon::HttpResponsePtr &)> &&callback) {

    GuessNumberUseCase uc;
    Output output;

    Json::Value data;
    Json::Reader reader;
    bool parsed = reader.parse(std::string(req->getBody()), data);
    if (!parsed) {
        // TODO handle parsing error
    }

    uc.execute(GuessNumberInput(data["game_id"].asString(), data["number"].asInt()), output);
    callback(newJsonResponse(output.to_json()));
}
