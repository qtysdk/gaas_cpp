#include "CreateGameCtrl.h"

void CreateGameCtrl::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    //write your application logic here
    auto resp=HttpResponse::newHttpResponse();
    //NOTE: The enum constant below is named "k200OK" (as in 200 OK), not "k2000K".
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_HTML);
    resp->setBody("{\"hello\": \"world\"}");
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}
