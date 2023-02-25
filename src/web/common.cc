#include "common.h"


std::shared_ptr<HttpResponse> newJsonResponse(std::string jsonBody) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    resp->setBody(jsonBody);
    return resp;
}
