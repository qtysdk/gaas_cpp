#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class CreateGameCtrl : public drogon::HttpSimpleController<CreateGameCtrl>
{
  public:
    void asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) override;
    PATH_LIST_BEGIN
    // list path definitions here;
    PATH_ADD("/guess_number_game:start", Post);
    PATH_LIST_END
};
