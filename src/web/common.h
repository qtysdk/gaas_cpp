#pragma once

#include <string>
#include <drogon/drogon.h>

using namespace drogon;

std::shared_ptr<HttpResponse> newJsonResponse(std::string jsonBody);
