#define DROGON_TEST_MAIN

#include <drogon/drogon_test.h>
#include <drogon/drogon.h>
#include <nlohmann/json.hpp>
#include "../src/repo/GameRepository.h"

using namespace drogon;
using json = nlohmann::json;


HttpResponsePtr http_post(std::string path, std::string body) {
    auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
    auto req = HttpRequest::newHttpRequest();
    req->setPath(path);
    req->setMethod(HttpMethod::Post);
    req->setBody(body);
    auto resp = client->sendRequest(req);
    assert(resp.first == ReqResult::Ok);
    assert(resp.second != nullptr);

    assert(resp.second->getStatusCode() == HttpStatusCode::k200OK);
    assert(resp.second->contentType() == CT_APPLICATION_JSON);
    return resp.second;
}


DROGON_TEST(GAME_RespondTest) {

    CHECK("0A0B" == createRespond(2345, 234577));
    CHECK("4A0B" == createRespond(2345, 2345));
    CHECK("2A2B" == createRespond(1234, 2134));
    CHECK("0A1B" == createRespond(1234, 4567));
    CHECK("0A0B" == createRespond(1234, 5678));
}

DROGON_TEST(GAME_NumberValidatorTest) {

    CHECK(false == validateNumber(1));
    CHECK(false == validateNumber(12456));

    CHECK(false == validateNumber(5666));
    CHECK(false == validateNumber(5566));
    CHECK(false == validateNumber(5556));
    CHECK(false == validateNumber(5555));

    CHECK(true == validateNumber(1234));
    CHECK(true == validateNumber(9527));

}

DROGON_TEST(GAME_AnswerGeneratorTest) {

    for (int i = 0; i < 100; ++i) {
        CHECK(validateNumber(createAnswer()));
    }

}

DROGON_TEST(GameAPITest) {


    std::string gameId = "";
    {
        auto resp = http_post("/guess_number_game:start", json{{"player_name", "I have no name"}}.dump());
        CHECK(resp->getStatusCode() == HttpStatusCode::k200OK);
        CHECK(resp->contentType() == CT_APPLICATION_JSON);

        auto result = json::parse(resp->getBody());
        auto game = gameRepository.findGameById(result["game_id"]);

        CHECK("I have no name" == result["player_name"]);
        CHECK("I have no name" == game->playerName);
        CHECK(result["history"].empty());
        CHECK(result["won"] == false);
        gameId = game->id;
    }

    {
        // arrange the final answer
        std::shared_ptr<Game> game = gameRepository.findGameById(gameId);
        game->answer = 1234;

        auto resp = http_post("/guess_number_game:guess",
                              json{{"game_id", gameId},
                                   {"number",  1234}}.dump());


        auto result = json::parse(resp->getBody());
        CHECK(result["history"].size() == 1);

        auto expected = json{{"guess",   1234},
                             {"respond", "4A0B"}};
        CHECK(expected == result["history"][0]);
        CHECK(result["won"] == true);
    }

}


int main(int argc, char **argv) {
    using namespace drogon;

    std::promise<void> p1;
    std::future<void> f1 = p1.get_future();

    // Start the main loop on another thread
    std::thread thr([&]() {
        // Queues the promise to be fulfilled after starting the loop
        app().getLoop()->queueInLoop([&p1]() { p1.set_value(); });
        app().addListener("127.0.0.1", 8848);
        app().run();
    });

    // The future is only satisfied after the event loop started
    f1.get();
    int status = test::run(argc, argv);

    // Ask the event loop to shutdown and wait
    app().getLoop()->queueInLoop([]() { app().quit(); });
    thr.join();
    return status;
}
