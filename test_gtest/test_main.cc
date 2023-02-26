#include <gtest/gtest.h>
#include <drogon/drogon.h>
#include <drogon/HttpAppFramework.h>
#include "../src/model/Game.h"
#include "../src/repo/GameRepository.h"


using namespace drogon;


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


TEST(TDD, GAME_RespondTest) {

    ASSERT_TRUE("0A0B" == createRespond(2345, 234577));
    ASSERT_TRUE("4A0B" == createRespond(2345, 2345));
    ASSERT_TRUE("2A2B" == createRespond(1234, 2134));
    ASSERT_TRUE("0A1B" == createRespond(1234, 4567));
    ASSERT_TRUE("0A0B" == createRespond(1234, 5678));
}

TEST(TDD, GAME_NumberValidatorTest) {

    ASSERT_TRUE(false == validateNumber(1));
    ASSERT_TRUE(false == validateNumber(12456));

    ASSERT_TRUE(false == validateNumber(5666));
    ASSERT_TRUE(false == validateNumber(5566));
    ASSERT_TRUE(false == validateNumber(5556));
    ASSERT_TRUE(false == validateNumber(5555));

    ASSERT_TRUE(true == validateNumber(1234));
    ASSERT_TRUE(true == validateNumber(9527));

}

TEST(TDD, GAME_AnswerGeneratorTest) {

    for (int i = 0; i < 100; ++i) {
        ASSERT_TRUE(validateNumber(createAnswer()));
    }

}


std::string create_game_input(std::string playerName) {
    Json::Value requestData;
    Json::FastWriter writer;
    requestData["player_name"] = playerName;
    return writer.write(requestData);
}

std::string create_guess_input(std::string gameId, int number) {
    Json::Value requestData;
    Json::FastWriter writer;
    requestData["game_id"] = gameId;
    requestData["number"] = number;
    return writer.write(requestData);
}

Json::Value create_respond_entry(std::string respond, int number) {
    Json::Value requestData;
    requestData["respond"] = respond;
    requestData["guess"] = number;
    return requestData;
}

Json::Value parsed_output(std::string_view response) {
    Json::Reader reader;
    Json::Value v;
    reader.parse(std::string(response), v);
    return v;
}

TEST(ATDD, GameAPITest) {


    std::string gameId = "";
    {
        auto resp = http_post("/guess_number_game:start", create_game_input("I have no name"));
        ASSERT_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
        ASSERT_EQ(resp->getStatusCode(), HttpStatusCode::k200OK);
        ASSERT_EQ(resp->contentType(), CT_APPLICATION_JSON);

        Json::Value result = parsed_output(resp->getBody());
        auto game = gameRepository.findGameById(result["game_id"].asString());

        ASSERT_EQ("I have no name", result["player_name"].asString());
        ASSERT_EQ("I have no name", game->playerName);
        ASSERT_TRUE(result["history"].empty());
        ASSERT_EQ(result["won"], false);
        gameId = game->id;
    }

    {
        // arrange the final answer
        std::shared_ptr<Game> game = gameRepository.findGameById(gameId);
        game->answer = 1234;

        auto resp = http_post("/guess_number_game:guess", create_guess_input(gameId, 1234));


        auto result = parsed_output(resp->getBody());
        ASSERT_EQ(result["history"].size(), 1);

        ASSERT_EQ(create_respond_entry("4A0B", 1234), result["history"][0]);
        ASSERT_TRUE(result["won"]);
    }

}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

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
    int status = RUN_ALL_TESTS();

    // Ask the event loop to shutdown and wait
    app().getLoop()->queueInLoop([&status]() {
        app().quit();
    });
    thr.join();
    return status;
}
