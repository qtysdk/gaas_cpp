#pragma once

#include <drogon/HttpSimpleController.h>
#include "../repo/GameRepository.h"
#include "nlohmann/json.hpp"
#include "../usecase/GuessNumber.h"

class GuessNumberCtrl : public drogon::HttpSimpleController<GuessNumberCtrl> {
public:
    void
    asyncHandleHttpRequest(const drogon::HttpRequestPtr &req,
                           function<void(const drogon::HttpResponsePtr &)> &&callback) override;

    PATH_LIST_BEGIN
        PATH_ADD("/guess_number_game:guess", drogon::Post);
    PATH_LIST_END
};
