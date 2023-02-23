#include "CreateGameCtrl.h"
#include "CreateGameUseCase.h"


void CreateGameCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req,
                                            std::function<void(const HttpResponsePtr &)> &&callback) {
    CreateGameUseCase uc;
    Output output;

    // TODO extract payload from json body
    uc.execute(Input(req->bodyData()), output);

    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_HTML);
    resp->setBody(output.to_json());
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}
