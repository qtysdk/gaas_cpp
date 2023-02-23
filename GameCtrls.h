#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class GameCtrls : public drogon::HttpSimpleController<GameCtrls> {
public:
    void
    asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) override;

    PATH_LIST_BEGIN
        PATH_ADD("/guess_number_game:start", Post);
    PATH_LIST_END
};

class GuessNumberCtrl : public drogon::HttpSimpleController<GameCtrls> {
public:
    void
    asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) override;

    PATH_LIST_BEGIN
        PATH_ADD("/guess_number_game:guess", Post);
    PATH_LIST_END
};
