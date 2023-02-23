#define DROGON_TEST_MAIN

#include <drogon/drogon_test.h>
#include <drogon/drogon.h>
#include <nlohmann/json.hpp>
#include "../GameRepository.h"

using namespace drogon;
using json = nlohmann::json;

DROGON_TEST(NothingAPITest) {
    auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
    auto req = HttpRequest::newHttpRequest();
    req->setPath("/");
    client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr &resp) {
        REQUIRE(res == ReqResult::Ok);
        REQUIRE(resp != nullptr);

        CHECK(resp->getStatusCode() == HttpStatusCode::k200OK);
        CHECK(resp->contentType() == CT_APPLICATION_JSON);

        // TODO check the response in that format
        // {"game_id": "", "playerName": "", "history": ""}
    });

}

DROGON_TEST(GameAPITest) {

    std::mutex mtx;
    std::condition_variable cv;
    bool finished = false;

    std::string gameId = "";
    {
        auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
        auto req = HttpRequest::newHttpRequest();
        req->setMethod(HttpMethod::Post);

        req->setPath("/guess_number_game:start");
        auto createGameData = json{{"player_name", "I have no name"}};
        req->setBody(createGameData.dump());


        client->sendRequest(req, [TEST_CTX, &gameId, &mtx, &finished, &cv](ReqResult res, const HttpResponsePtr &resp) {
            REQUIRE(res == ReqResult::Ok);
            REQUIRE(resp != nullptr);

            CHECK(resp->getStatusCode() == HttpStatusCode::k200OK);
            CHECK(resp->contentType() == CT_APPLICATION_JSON);

            auto result = json::parse(resp->getBody());
            auto game = gameRepository.findGameById(result["game_id"]);

            CHECK("I have no name" == result["player_name"]);
            CHECK("I have no name" == game->playerName);
            CHECK(result["history"].empty());

            std::unique_lock<std::mutex> lock(mtx);
            finished = true;
            cv.notify_all();
            gameId = game->id;
        });
    }


    // Wait for the callback to complete
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]() { return finished; });

    {
        auto client = HttpClient::newHttpClient("http://127.0.0.1:8848");
        auto req = HttpRequest::newHttpRequest();
        req->setMethod(HttpMethod::Post);

        req->setPath("/guess_number_game:guess");
        auto guessNumberData = json{{"game_id", gameId},
                                    {"number",  1234}};

        req->setBody(guessNumberData.dump());
        client->sendRequest(req, [TEST_CTX](ReqResult res, const HttpResponsePtr &resp) {
            REQUIRE(res == ReqResult::Ok);
            REQUIRE(resp != nullptr);

            CHECK(resp->getStatusCode() == HttpStatusCode::k200OK);
            CHECK(resp->contentType() == CT_APPLICATION_JSON);

            auto result = json::parse(resp->getBody());
            auto game = gameRepository.findGameById(result["game_id"]);
            CHECK(result["history"].size() == 1);

            auto expected = json{{"guess", 1234}, {"respond", "4A0B"}};
            CHECK(expected == result["history"][0]);
        });
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
